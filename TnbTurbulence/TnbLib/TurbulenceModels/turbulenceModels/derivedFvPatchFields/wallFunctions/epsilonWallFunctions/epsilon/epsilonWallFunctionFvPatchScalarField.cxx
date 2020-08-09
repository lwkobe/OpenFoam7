#include <epsilonWallFunctionFvPatchScalarField.hxx>

#include <nutWallFunctionFvPatchScalarField.hxx>
#include <turbulenceModel.hxx>
#include <fvMatrix.hxx>
#include <addToRunTimeSelectionTable.hxx>

#include <Time.hxx>  // added by amir

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

tnbLib::scalar tnbLib::epsilonWallFunctionFvPatchScalarField::tolerance_ = 1e-5;

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void tnbLib::epsilonWallFunctionFvPatchScalarField::setMaster()
{
	if (master_ != -1)
	{
		return;
	}

	const volScalarField& epsilon =
		static_cast<const volScalarField&>(this->internalField());

	const volScalarField::Boundary& bf = epsilon.boundaryField();

	label master = -1;
	forAll(bf, patchi)
	{
		if (isA<epsilonWallFunctionFvPatchScalarField>(bf[patchi]))
		{
			epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);

			if (master == -1)
			{
				master = patchi;
			}

			epf.master() = master;
		}
	}
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::createAveragingWeights()
{
	const volScalarField& epsilon =
		static_cast<const volScalarField&>(this->internalField());

	const volScalarField::Boundary& bf = epsilon.boundaryField();

	const fvMesh& mesh = epsilon.mesh();

	if (initialised_ && !mesh.changing())
	{
		return;
	}

	volScalarField weights
	(
		IOobject
		(
			"weights",
			mesh.time().timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::NO_WRITE,
			false // do not register
		),
		mesh,
		dimensionedScalar(dimless, 0)
	);

	DynamicList<label> epsilonPatches(bf.size());
	forAll(bf, patchi)
	{
		if (isA<epsilonWallFunctionFvPatchScalarField>(bf[patchi]))
		{
			epsilonPatches.append(patchi);

			const labelUList& faceCells = bf[patchi].patch().faceCells();
			forAll(faceCells, i)
			{
				weights[faceCells[i]]++;
			}
		}
	}

	cornerWeights_.setSize(bf.size());
	forAll(epsilonPatches, i)
	{
		label patchi = epsilonPatches[i];
		const fvPatchScalarField& wf = weights.boundaryField()[patchi];
		cornerWeights_[patchi] = 1.0 / wf.patchInternalField();
	}

	G_.setSize(internalField().size(), 0.0);
	epsilon_.setSize(internalField().size(), 0.0);

	initialised_ = true;
}


tnbLib::epsilonWallFunctionFvPatchScalarField&
tnbLib::epsilonWallFunctionFvPatchScalarField::epsilonPatch(const label patchi)
{
	const volScalarField& epsilon =
		static_cast<const volScalarField&>(this->internalField());

	const volScalarField::Boundary& bf = epsilon.boundaryField();

	const epsilonWallFunctionFvPatchScalarField& epf =
		refCast<const epsilonWallFunctionFvPatchScalarField>(bf[patchi]);

	return const_cast<epsilonWallFunctionFvPatchScalarField&>(epf);
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::calculateTurbulenceFields
(
	const turbulenceModel& turbulence,
	scalarField& G0,
	scalarField& epsilon0
)
{
	// Accumulate all of the G and epsilon contributions
	forAll(cornerWeights_, patchi)
	{
		if (!cornerWeights_[patchi].empty())
		{
			epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);

			const List<scalar>& w = cornerWeights_[patchi];

			epf.calculate(turbulence, w, epf.patch(), G0, epsilon0);
		}
	}

	// Apply zero-gradient condition for epsilon
	forAll(cornerWeights_, patchi)
	{
		if (!cornerWeights_[patchi].empty())
		{
			epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);

			epf == scalarField(epsilon0, epf.patch().faceCells());
		}
	}
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::calculate
(
	const turbulenceModel& turbModel,
	const List<scalar>& cornerWeights,
	const fvPatch& patch,
	scalarField& G0,
	scalarField& epsilon0
)
{
	const label patchi = patch.index();

	const nutWallFunctionFvPatchScalarField& nutw =
		nutWallFunctionFvPatchScalarField::nutw(turbModel, patchi);

	const scalarField& y = turbModel.y()[patchi];

	const tmp<scalarField> tnuw = turbModel.nu(patchi);
	const scalarField& nuw = tnuw();

	const tmp<volScalarField> tk = turbModel.k();
	const volScalarField& k = tk();

	const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];

	const scalarField magGradUw(mag(Uw.snGrad()));

	const scalar Cmu25 = pow025(nutw.Cmu());
	const scalar Cmu75 = pow(nutw.Cmu(), 0.75);

	// Set epsilon and G
	forAll(nutw, facei)
	{
		const label celli = patch.faceCells()[facei];

		const scalar yPlus = Cmu25 * y[facei] * sqrt(k[celli]) / nuw[facei];

		const scalar w = cornerWeights[facei];

		if (yPlus > nutw.yPlusLam())
		{
			epsilon0[celli] +=
				w * Cmu75*pow(k[celli], 1.5) / (nutw.kappa()*y[facei]);

			G0[celli] +=
				w
				* (nutw[facei] + nuw[facei])
				*magGradUw[facei]
				* Cmu25*sqrt(k[celli])
				/ (nutw.kappa()*y[facei]);
		}
		else
		{
			epsilon0[celli] += w * 2.0*k[celli] * nuw[facei] / sqr(y[facei]);
		}
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF
)
	:
	fixedValueFvPatchField<scalar>(p, iF),
	G_(),
	epsilon_(),
	initialised_(false),
	master_(-1),
	cornerWeights_()
{}


tnbLib::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF,
	const dictionary& dict
)
	:
	fixedValueFvPatchField<scalar>(p, iF, dict),
	G_(),
	epsilon_(),
	initialised_(false),
	master_(-1),
	cornerWeights_()
{
	// Apply zero-gradient condition on start-up
	this->operator==(patchInternalField());
}


tnbLib::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
	const epsilonWallFunctionFvPatchScalarField& ptf,
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF,
	const fvPatchFieldMapper& mapper
)
	:
	fixedValueFvPatchField<scalar>(ptf, p, iF, mapper),
	G_(),
	epsilon_(),
	initialised_(false),
	master_(-1),
	cornerWeights_()
{}


tnbLib::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
	const epsilonWallFunctionFvPatchScalarField& ewfpsf
)
	:
	fixedValueFvPatchField<scalar>(ewfpsf),
	G_(),
	epsilon_(),
	initialised_(false),
	master_(-1),
	cornerWeights_()
{}


tnbLib::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
	const epsilonWallFunctionFvPatchScalarField& ewfpsf,
	const DimensionedField<scalar, volMesh>& iF
)
	:
	fixedValueFvPatchField<scalar>(ewfpsf, iF),
	G_(),
	epsilon_(),
	initialised_(false),
	master_(-1),
	cornerWeights_()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tnbLib::scalarField& tnbLib::epsilonWallFunctionFvPatchScalarField::G(bool init)
{
	if (patch().index() == master_)
	{
		if (init)
		{
			G_ = 0.0;
		}

		return G_;
	}

	return epsilonPatch(master_).G();
}


tnbLib::scalarField& tnbLib::epsilonWallFunctionFvPatchScalarField::epsilon
(
	bool init
)
{
	if (patch().index() == master_)
	{
		if (init)
		{
			epsilon_ = 0.0;
		}

		return epsilon_;
	}

	return epsilonPatch(master_).epsilon(init);
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::updateCoeffs()
{
	if (updated())
	{
		return;
	}

	const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
		(
			IOobject::groupName
			(
				turbulenceModel::propertiesName,
				internalField().group()
			)
			);

	setMaster();

	if (patch().index() == master_)
	{
		createAveragingWeights();
		calculateTurbulenceFields(turbModel, G(true), epsilon(true));
	}

	const scalarField& G0 = this->G();
	const scalarField& epsilon0 = this->epsilon();

	typedef DimensionedField<scalar, volMesh> FieldType;

	FieldType& G =
		const_cast<FieldType&>
		(
			db().lookupObject<FieldType>(turbModel.GName())
			);

	FieldType& epsilon = const_cast<FieldType&>(internalField());

	forAll(*this, facei)
	{
		label celli = patch().faceCells()[facei];

		G[celli] = G0[celli];
		epsilon[celli] = epsilon0[celli];
	}

	fvPatchField<scalar>::updateCoeffs();
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::updateWeightedCoeffs
(
	const scalarField& weights
)
{
	if (updated())
	{
		return;
	}

	const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
		(
			IOobject::groupName
			(
				turbulenceModel::propertiesName,
				internalField().group()
			)
			);

	setMaster();

	if (patch().index() == master_)
	{
		createAveragingWeights();
		calculateTurbulenceFields(turbModel, G(true), epsilon(true));
	}

	const scalarField& G0 = this->G();
	const scalarField& epsilon0 = this->epsilon();

	typedef DimensionedField<scalar, volMesh> FieldType;

	FieldType& G =
		const_cast<FieldType&>
		(
			db().lookupObject<FieldType>(turbModel.GName())
			);

	FieldType& epsilon = const_cast<FieldType&>(internalField());

	scalarField& epsilonf = *this;

	// Only set the values if the weights are > tolerance
	forAll(weights, facei)
	{
		scalar w = weights[facei];

		if (w > tolerance_)
		{
			label celli = patch().faceCells()[facei];

			G[celli] = (1.0 - w)*G[celli] + w * G0[celli];
			epsilon[celli] = (1.0 - w)*epsilon[celli] + w * epsilon0[celli];
			epsilonf[facei] = epsilon[celli];
		}
	}

	fvPatchField<scalar>::updateCoeffs();
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::manipulateMatrix
(
	fvMatrix<scalar>& matrix
)
{
	if (manipulatedMatrix())
	{
		return;
	}

	matrix.setValues(patch().faceCells(), patchInternalField());

	fvPatchField<scalar>::manipulateMatrix(matrix);
}


void tnbLib::epsilonWallFunctionFvPatchScalarField::manipulateMatrix
(
	fvMatrix<scalar>& matrix,
	const Field<scalar>& weights
)
{
	if (manipulatedMatrix())
	{
		return;
	}

	DynamicList<label> constraintCells(weights.size());
	DynamicList<scalar> constraintEpsilon(weights.size());
	const labelUList& faceCells = patch().faceCells();

	const DimensionedField<scalar, volMesh>& epsilon
		= internalField();

	label nConstrainedCells = 0;


	forAll(weights, facei)
	{
		// Only set the values if the weights are > tolerance
		if (weights[facei] > tolerance_)
		{
			nConstrainedCells++;

			label celli = faceCells[facei];

			constraintCells.append(celli);
			constraintEpsilon.append(epsilon[celli]);
		}
	}

	if (debug)
	{
		Pout << "Patch: " << patch().name()
			<< ": number of constrained cells = " << nConstrainedCells
			<< " out of " << patch().size()
			<< endl;
	}

	matrix.setValues
	(
		constraintCells,
		scalarField(constraintEpsilon)
	);

	fvPatchField<scalar>::manipulateMatrix(matrix);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	makePatchTypeField
	(
		fvPatchScalarField,
		epsilonWallFunctionFvPatchScalarField
	);
}


// ************************************************************************* //