#include <FreeStream.hxx>

#include <constants.hxx>
#include <triPointRef.hxx>
#include <tetIndices.hxx>

using namespace tnbLib::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
tnbLib::FreeStream<CloudType>::FreeStream
(
	const dictionary& dict,
	CloudType& cloud
)
	:
	InflowBoundaryModel<CloudType>(dict, cloud, typeName),
	patches_(),
	moleculeTypeIds_(),
	numberDensities_(),
	particleFluxAccumulators_()
{
	// Identify which patches to use

	DynamicList<label> patches;

	forAll(cloud.mesh().boundaryMesh(), p)
	{
		const polyPatch& patch = cloud.mesh().boundaryMesh()[p];

		if (isType<polyPatch>(patch))
		{
			patches.append(p);
		}
	}

	patches_.transfer(patches);

	const dictionary& numberDensitiesDict
	(
		this->coeffDict().subDict("numberDensities")
	);

	List<word> molecules(numberDensitiesDict.toc());

	// Initialise the particleFluxAccumulators_
	particleFluxAccumulators_.setSize(patches_.size());

	forAll(patches_, p)
	{
		const polyPatch& patch = cloud.mesh().boundaryMesh()[patches_[p]];

		particleFluxAccumulators_[p] = List<Field<scalar>>
			(
				molecules.size(),
				Field<scalar>(patch.size(), 0.0)
				);
	}

	moleculeTypeIds_.setSize(molecules.size());

	numberDensities_.setSize(molecules.size());

	forAll(molecules, i)
	{
		numberDensities_[i] = readScalar
		(
			numberDensitiesDict.lookup(molecules[i])
		);

		moleculeTypeIds_[i] = findIndex(cloud.typeIdList(), molecules[i]);

		if (moleculeTypeIds_[i] == -1)
		{
			FatalErrorInFunction
				<< "typeId " << molecules[i] << "not defined in cloud." << nl
				<< abort(FatalError);
		}
	}

	numberDensities_ /= cloud.nParticle();
}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
tnbLib::FreeStream<CloudType>::~FreeStream()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void tnbLib::FreeStream<CloudType>::autoMap(const mapPolyMesh& mapper)
{
	CloudType& cloud(this->owner());
	const polyMesh& mesh(cloud.mesh());

	forAll(patches_, p)
	{
		label patchi = patches_[p];

		const polyPatch& patch = mesh.boundaryMesh()[patchi];
		List<Field<scalar>>& pFA = particleFluxAccumulators_[p];

		forAll(pFA, facei)
		{
			pFA[facei].setSize(patch.size(), 0);
		}
	}
}


template<class CloudType>
void tnbLib::FreeStream<CloudType>::inflow()
{
	CloudType& cloud(this->owner());

	const polyMesh& mesh(cloud.mesh());

	const scalar deltaT = mesh.time().deltaTValue();

	Random& rndGen(cloud.rndGen());

	scalar sqrtPi = sqrt(pi);

	label particlesInserted = 0;

	const volScalarField::Boundary& boundaryT
	(
		cloud.boundaryT().boundaryField()
	);

	const volVectorField::Boundary& boundaryU
	(
		cloud.boundaryU().boundaryField()
	);


	forAll(patches_, p)
	{
		label patchi = patches_[p];

		const polyPatch& patch = mesh.boundaryMesh()[patchi];

		// Add mass to the accumulators.  negative face area dotted with the
		// velocity to point flux into the domain.

		// Take a reference to the particleFluxAccumulator for this patch
		List<Field<scalar>>& pFA = particleFluxAccumulators_[p];

		forAll(pFA, i)
		{
			label typeId = moleculeTypeIds_[i];

			scalar mass = cloud.constProps(typeId).mass();

			if (min(boundaryT[patchi]) < small)
			{
				FatalErrorInFunction
					<< "Zero boundary temperature detected, check boundaryT "
					<< "condition." << nl
					<< nl << abort(FatalError);
			}

			scalarField mostProbableSpeed
			(
				cloud.maxwellianMostProbableSpeed
				(
					boundaryT[patchi],
					mass
				)
			);

			// Dotting boundary velocity with the face unit normal
			// (which points out of the domain, so it must be
			// negated), dividing by the most probable speed to form
			// molecularSpeedRatio * cosTheta

			scalarField sCosTheta
			(
				(boundaryU[patchi] & -patch.faceAreas() / mag(patch.faceAreas()))
				/ mostProbableSpeed
			);

			// From Bird eqn 4.22

			pFA[i] +=
				mag(patch.faceAreas())*numberDensities_[i] * deltaT
				*mostProbableSpeed
				*(
					exp(-sqr(sCosTheta)) + sqrtPi * sCosTheta*(1 + erf(sCosTheta))
					)
				/ (2.0*sqrtPi);
		}

		forAll(patch, pFI)
		{
			// Loop over all faces as the outer loop to avoid calculating
			// geometrical properties multiple times.

			const face& f = patch[pFI];

			label globalFaceIndex = pFI + patch.start();

			label celli = mesh.faceOwner()[globalFaceIndex];

			const vector& fC = patch.faceCentres()[pFI];

			scalar fA = mag(patch.faceAreas()[pFI]);

			List<tetIndices> faceTets = polyMeshTetDecomposition::faceTetIndices
			(
				mesh,
				globalFaceIndex,
				celli
			);

			// Cumulative triangle area fractions
			List<scalar> cTriAFracs(faceTets.size(), 0.0);

			scalar previousCummulativeSum = 0.0;

			forAll(faceTets, triI)
			{
				const tetIndices& faceTetIs = faceTets[triI];

				cTriAFracs[triI] =
					faceTetIs.faceTri(mesh).mag() / fA
					+ previousCummulativeSum;

				previousCummulativeSum = cTriAFracs[triI];
			}

			// Force the last area fraction value to 1.0 to avoid any
			// rounding/non-flat face errors giving a value < 1.0
			cTriAFracs.last() = 1.0;

			// Normal unit vector *negative* so normal is pointing into the
			// domain
			vector n = patch.faceAreas()[pFI];
			n /= -mag(n);

			// Wall tangential unit vector. Use the direction between the
			// face centre and the first vertex in the list
			vector t1 = fC - (mesh.points()[f[0]]);
			t1 /= mag(t1);

			// Other tangential unit vector.  Rescaling in case face is not
			// flat and n and t1 aren't perfectly orthogonal
			vector t2 = n ^ t1;
			t2 /= mag(t2);

			scalar faceTemperature = boundaryT[patchi][pFI];

			const vector& faceVelocity = boundaryU[patchi][pFI];

			forAll(pFA, i)
			{
				scalar& faceAccumulator = pFA[i][pFI];

				// Number of whole particles to insert
				label nI = max(label(faceAccumulator), 0);

				// Add another particle with a probability proportional to the
				// remainder of taking the integer part of faceAccumulator
				if ((faceAccumulator - nI) > rndGen.scalar01())
				{
					nI++;
				}

				faceAccumulator -= nI;

				label typeId = moleculeTypeIds_[i];

				scalar mass = cloud.constProps(typeId).mass();

				for (label i = 0; i < nI; i++)
				{
					// Choose a triangle to insert on, based on their relative
					// area

					scalar triSelection = rndGen.scalar01();

					// Selected triangle
					label selectedTriI = -1;

					forAll(cTriAFracs, triI)
					{
						selectedTriI = triI;

						if (cTriAFracs[triI] >= triSelection)
						{
							break;
						}
					}

					// Randomly distribute the points on the triangle.

					const tetIndices& faceTetIs = faceTets[selectedTriI];

					point p = faceTetIs.faceTri(mesh).randomPoint(rndGen);

					// Velocity generation

					scalar mostProbableSpeed
					(
						cloud.maxwellianMostProbableSpeed
						(
							faceTemperature,
							mass
						)
					);

					scalar sCosTheta = (faceVelocity & n) / mostProbableSpeed;

					// Coefficients required for Bird eqn 12.5
					scalar uNormProbCoeffA =
						sCosTheta + sqrt(sqr(sCosTheta) + 2.0);

					scalar uNormProbCoeffB =
						0.5*
						(
							1.0
							+ sCosTheta * (sCosTheta - sqrt(sqr(sCosTheta) + 2.0))
							);

					// Equivalent to the QA value in Bird's DSMC3.FOR
					scalar randomScaling = 3.0;

					if (sCosTheta < -3)
					{
						randomScaling = mag(sCosTheta) + 1;
					}

					scalar P = -1;

					// Normalised candidates for the normal direction velocity
					// component
					scalar uNormal;
					scalar uNormalThermal;

					// Select a velocity using Bird eqn 12.5
					do
					{
						uNormalThermal =
							randomScaling * (2.0*rndGen.scalar01() - 1);

						uNormal = uNormalThermal + sCosTheta;

						if (uNormal < 0.0)
						{
							P = -1;
						}
						else
						{
							P = 2.0*uNormal / uNormProbCoeffA
								* exp(uNormProbCoeffB - sqr(uNormalThermal));
						}

					} while (P < rndGen.scalar01());

					vector U =
						sqrt(physicoChemical::k.value()*faceTemperature / mass)
						*(
							rndGen.scalarNormal()*t1
							+ rndGen.scalarNormal()*t2
							)
						+ (t1 & faceVelocity)*t1
						+ (t2 & faceVelocity)*t2
						+ mostProbableSpeed * uNormal*n;

					scalar Ei = cloud.equipartitionInternalEnergy
					(
						faceTemperature,
						cloud.constProps(typeId).internalDegreesOfFreedom()
					);

					cloud.addNewParcel(p, celli, U, Ei, typeId);

					particlesInserted++;
				}
			}
		}
	}

	reduce(particlesInserted, sumOp<label>());

	Info << "    Particles inserted              = "
		<< particlesInserted << endl;

}


// ************************************************************************* //