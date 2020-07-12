#include <cyclicAMIPolyPatch.hxx>

#include <SubField.hxx>
#include <Time.hxx>
#include <unitConversion.hxx>
#include <addToRunTimeSelectionTable.hxx>

// added by amir
#include <IOobject.hxx> 
#include <OFstream.hxx>
#include <meshTools.hxx>
#include <polyMesh.hxx>
#include <PstreamReduceOps.hxx>
#include <faceList.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	defineTypeNameAndDebug(cyclicAMIPolyPatch, 0);

	addToRunTimeSelectionTable(polyPatch, cyclicAMIPolyPatch, word);
	addToRunTimeSelectionTable(polyPatch, cyclicAMIPolyPatch, dictionary);
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

tnbLib::vector tnbLib::cyclicAMIPolyPatch::findFaceNormalMaxRadius
(
	const pointField& faceCentres
) const
{
	// Determine a face furthest away from the axis

	const vectorField n((faceCentres - rotationCentre_) ^ rotationAxis_);

	const scalarField magRadSqr(magSqr(n));

	label facei = findMax(magRadSqr);

	if (debug)
	{
		Info << "findFaceMaxRadius(const pointField&) : patch: " << name() << nl
			<< "    rotFace  = " << facei << nl
			<< "    point    = " << faceCentres[facei] << nl
			<< "    distance = " << tnbLib::sqrt(magRadSqr[facei])
			<< endl;
	}

	return n[facei];
}


void tnbLib::cyclicAMIPolyPatch::calcTransforms
(
	const primitivePatch& half0,
	const pointField& half0Ctrs,
	const vectorField& half0Areas,
	const pointField& half1Ctrs,
	const vectorField& half1Areas
)
{
	if (transform() != neighbPatch().transform())
	{
		FatalErrorInFunction
			<< "Patch " << name()
			<< " has transform type " << transformTypeNames[transform()]
			<< ", neighbour patch " << neighbPatchName()
			<< " has transform type "
			<< neighbPatch().transformTypeNames[neighbPatch().transform()]
			<< exit(FatalError);
	}


	// Calculate transformation tensors

	switch (transform())
	{
	case ROTATIONAL:
	{
		tensor revT = Zero;

		if (rotationAngleDefined_)
		{
			const tensor T(rotationAxis_*rotationAxis_);

			const tensor S
			(
				0, -rotationAxis_.z(), rotationAxis_.y(),
				rotationAxis_.z(), 0, -rotationAxis_.x(),
				-rotationAxis_.y(), rotationAxis_.x(), 0
			);

			const tensor revTPos
			(
				T
				+ cos(rotationAngle_)*(tensor::I - T)
				+ sin(rotationAngle_)*S
			);

			const tensor revTNeg
			(
				T
				+ cos(-rotationAngle_)*(tensor::I - T)
				+ sin(-rotationAngle_)*S
			);

			// Check - assume correct angle when difference in face areas
			// is the smallest
			const vector transformedAreaPos = gSum(half1Areas & revTPos);
			const vector transformedAreaNeg = gSum(half1Areas & revTNeg);
			const vector area0 = gSum(half0Areas);
			const scalar magArea0 = mag(area0) + rootVSmall;

			// Areas have opposite sign, so sum should be zero when correct
			// rotation applied
			const scalar errorPos = mag(transformedAreaPos + area0);
			const scalar errorNeg = mag(transformedAreaNeg + area0);

			const scalar normErrorPos = errorPos / magArea0;
			const scalar normErrorNeg = errorNeg / magArea0;

			if (errorPos > errorNeg && normErrorNeg < matchTolerance())
			{
				revT = revTNeg;
				rotationAngle_ *= -1;
			}
			else
			{
				revT = revTPos;
			}

			const scalar areaError = min(normErrorPos, normErrorNeg);

			if (areaError > matchTolerance())
			{
				WarningInFunction
					<< "Patch areas are not consistent within "
					<< 100 * matchTolerance()
					<< " % indicating a possible error in the specified "
					<< "angle of rotation" << nl
					<< "    owner patch     : " << name() << nl
					<< "    neighbour patch : " << neighbPatch().name()
					<< nl
					<< "    angle           : "
					<< radToDeg(rotationAngle_) << " deg" << nl
					<< "    area error      : " << 100 * areaError << " %"
					<< "    match tolerance : " << matchTolerance()
					<< endl;
			}

			if (debug)
			{
				scalar theta = radToDeg(rotationAngle_);

				Pout << "cyclicAMIPolyPatch::calcTransforms: patch:"
					<< name()
					<< " Specified rotation:"
					<< " swept angle: " << theta << " [deg]"
					<< " reverse transform: " << revT
					<< endl;
			}
		}
		else
		{
			point n0 = Zero;
			point n1 = Zero;
			if (half0Ctrs.size())
			{
				n0 = findFaceNormalMaxRadius(half0Ctrs);
			}
			if (half1Ctrs.size())
			{
				n1 = -findFaceNormalMaxRadius(half1Ctrs);
			}

			reduce(n0, maxMagSqrOp<point>());
			reduce(n1, maxMagSqrOp<point>());

			n0 /= mag(n0) + vSmall;
			n1 /= mag(n1) + vSmall;

			// Extended tensor from two local coordinate systems calculated
			// using normal and rotation axis
			const tensor E0
			(
				rotationAxis_,
				(n0 ^ rotationAxis_),
				n0
			);
			const tensor E1
			(
				rotationAxis_,
				(-n1 ^ rotationAxis_),
				-n1
			);
			revT = E1.T() & E0;

			if (debug)
			{
				scalar theta = radToDeg(acos(-(n0 & n1)));

				Pout << "cyclicAMIPolyPatch::calcTransforms: patch:"
					<< name()
					<< " Specified rotation:"
					<< " n0:" << n0 << " n1:" << n1
					<< " swept angle: " << theta << " [deg]"
					<< " reverse transform: " << revT
					<< endl;
			}
		}

		const_cast<tensorField&>(forwardT()) = tensorField(1, revT.T());
		const_cast<tensorField&>(reverseT()) = tensorField(1, revT);
		const_cast<vectorField&>(separation()).setSize(0);
		const_cast<boolList&>(collocated()) = boolList(1, false);

		break;
	}
	case TRANSLATIONAL:
	{
		if (debug)
		{
			Pout << "cyclicAMIPolyPatch::calcTransforms : patch:" << name()
				<< " Specified translation : " << separationVector_
				<< endl;
		}

		const_cast<tensorField&>(forwardT()).clear();
		const_cast<tensorField&>(reverseT()).clear();
		const_cast<vectorField&>(separation()) = vectorField
		(
			1,
			separationVector_
		);
		const_cast<boolList&>(collocated()) = boolList(1, false);

		break;
	}
	default:
	{
		if (debug)
		{
			Pout << "patch:" << name()
				<< " Assuming cyclic AMI pairs are colocated" << endl;
		}

		const_cast<tensorField&>(forwardT()).clear();
		const_cast<tensorField&>(reverseT()).clear();
		const_cast<vectorField&>(separation()).setSize(0);
		const_cast<boolList&>(collocated()) = boolList(1, true);

		break;
	}
	}

	if (debug)
	{
		Pout << "patch: " << name() << nl
			<< "    forwardT = " << forwardT() << nl
			<< "    reverseT = " << reverseT() << nl
			<< "    separation = " << separation() << nl
			<< "    collocated = " << collocated() << nl << endl;
	}
}


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

void tnbLib::cyclicAMIPolyPatch::resetAMI() const
{
	if (owner())
	{
		const polyPatch& nbr = neighbPatch();
		pointField nbrPoints
		(
			neighbPatch().boundaryMesh().mesh().points(),
			neighbPatch().meshPoints()
		);

		if (debug)
		{
			const Time& t = boundaryMesh().mesh().time();
			OFstream os(t.path() / name() + "_neighbourPatch-org.obj");
			meshTools::writeOBJ(os, neighbPatch().localFaces(), nbrPoints);
		}

		// Transform neighbour patch to local system
		transformPosition(nbrPoints);
		primitivePatch nbrPatch0
		(
			SubList<face>
			(
				nbr.localFaces(),
				nbr.size()
				),
			nbrPoints
		);

		if (debug)
		{
			const Time& t = boundaryMesh().mesh().time();
			OFstream osN(t.path() / name() + "_neighbourPatch-trans.obj");
			meshTools::writeOBJ(osN, nbrPatch0.localFaces(), nbrPoints);

			OFstream osO(t.path() / name() + "_ownerPatch.obj");
			meshTools::writeOBJ(osO, this->localFaces(), localPoints());
		}

		// Construct/apply AMI interpolation to determine addressing and weights
		AMIs_.resize(1);
		AMIs_.set
		(
			0,
			new AMIInterpolation
			(
				*this,
				nbrPatch0,
				surfPtr(),
				faceAreaIntersect::tmMesh,
				AMIRequireMatch_,
				AMIMethod_,
				AMILowWeightCorrection_,
				AMIReverse_
			)
		);

		AMITransforms_.resize(1, vectorTensorTransform::I);

		if (debug)
		{
			Pout << "cyclicAMIPolyPatch : " << name()
				<< " constructed AMI with " << nl
				<< "    " << "srcAddress:" << AMIs_[0].srcAddress().size()
				<< nl
				<< "    " << "tgAddress :" << AMIs_[0].tgtAddress().size()
				<< nl << endl;
		}
	}
}


void tnbLib::cyclicAMIPolyPatch::calcTransforms()
{
	const cyclicAMIPolyPatch& half0 = *this;
	vectorField half0Areas(half0.size());
	forAll(half0, facei)
	{
		half0Areas[facei] = half0[facei].area(half0.points());
	}

	const cyclicAMIPolyPatch& half1 = neighbPatch();
	vectorField half1Areas(half1.size());
	forAll(half1, facei)
	{
		half1Areas[facei] = half1[facei].area(half1.points());
	}

	calcTransforms
	(
		half0,
		half0.faceCentres(),
		half0Areas,
		half1.faceCentres(),
		half1Areas
	);

	if (debug)
	{
		Pout << "calcTransforms() : patch: " << name() << nl
			<< "    forwardT = " << forwardT() << nl
			<< "    reverseT = " << reverseT() << nl
			<< "    separation = " << separation() << nl
			<< "    collocated = " << collocated() << nl << endl;
	}
}


void tnbLib::cyclicAMIPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
	// Clear the invalid AMIs and transforms
	AMIs_.clear();
	AMITransforms_.clear();

	polyPatch::initGeometry(pBufs);
}


void tnbLib::cyclicAMIPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
	calcGeometry
	(
		*this,
		faceCentres(),
		faceAreas(),
		faceCellCentres(),
		neighbPatch().faceCentres(),
		neighbPatch().faceAreas(),
		neighbPatch().faceCellCentres()
	);
}


void tnbLib::cyclicAMIPolyPatch::initMovePoints
(
	PstreamBuffers& pBufs,
	const pointField& p
)
{
	// Clear the invalid AMIs and transforms
	AMIs_.clear();
	AMITransforms_.clear();

	polyPatch::initMovePoints(pBufs, p);

	// See below. Clear out any local geometry
	primitivePatch::movePoints(p);
}


void tnbLib::cyclicAMIPolyPatch::movePoints
(
	PstreamBuffers& pBufs,
	const pointField& p
)
{
	polyPatch::movePoints(pBufs, p);

	calcTransforms();
}


void tnbLib::cyclicAMIPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
	// Clear the invalid AMIs and transforms
	AMIs_.clear();
	AMITransforms_.clear();

	polyPatch::initUpdateMesh(pBufs);
}


void tnbLib::cyclicAMIPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
	polyPatch::updateMesh(pBufs);
}


void tnbLib::cyclicAMIPolyPatch::clearGeom()
{
	// Clear the invalid AMIs and transforms
	AMIs_.clear();
	AMITransforms_.clear();

	polyPatch::clearGeom();
}


// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

tnbLib::cyclicAMIPolyPatch::cyclicAMIPolyPatch
(
	const word& name,
	const label size,
	const label start,
	const label index,
	const polyBoundaryMesh& bm,
	const word& patchType,
	const transformType transform,
	const bool AMIRequireMatch,
	const AMIInterpolation::interpolationMethod AMIMethod
)
	:
	coupledPolyPatch(name, size, start, index, bm, patchType, transform),
	nbrPatchName_(word::null),
	nbrPatchID_(-1),
	rotationAxis_(Zero),
	rotationCentre_(point::zero),
	rotationAngleDefined_(false),
	rotationAngle_(0.0),
	separationVector_(Zero),
	AMIs_(),
	AMITransforms_(),
	AMIReverse_(false),
	AMIRequireMatch_(AMIRequireMatch),
	AMILowWeightCorrection_(-1.0),
	AMIMethod_(AMIMethod),
	surfPtr_(nullptr),
	surfDict_(fileName("surface"))
{
	// Neighbour patch might not be valid yet so no transformation
	// calculation possible
}


tnbLib::cyclicAMIPolyPatch::cyclicAMIPolyPatch
(
	const word& name,
	const dictionary& dict,
	const label index,
	const polyBoundaryMesh& bm,
	const word& patchType,
	const bool AMIRequireMatch,
	const AMIInterpolation::interpolationMethod AMIMethod
)
	:
	coupledPolyPatch(name, dict, index, bm, patchType),
	nbrPatchName_(dict.lookupOrDefault<word>("neighbourPatch", "")),
	coupleGroup_(dict),
	nbrPatchID_(-1),
	rotationAxis_(Zero),
	rotationCentre_(point::zero),
	rotationAngleDefined_(false),
	rotationAngle_(0.0),
	separationVector_(Zero),
	AMIs_(),
	AMITransforms_(),
	AMIReverse_(dict.lookupOrDefault<bool>("flipNormals", false)),
	AMIRequireMatch_(AMIRequireMatch),
	AMILowWeightCorrection_(dict.lookupOrDefault("lowWeightCorrection", -1.0)),
	AMIMethod_
	(
		dict.found("method")
		? AMIInterpolation::wordTointerpolationMethod
		(
			dict.lookup("method")
		)
		: AMIMethod
	),
	surfPtr_(nullptr),
	surfDict_(dict.subOrEmptyDict("surface"))
{
	if (nbrPatchName_ == word::null && !coupleGroup_.valid())
	{
		FatalIOErrorInFunction
		(
			dict
		) << "No \"neighbourPatch\" or \"coupleGroup\" provided."
			<< exit(FatalIOError);
	}

	if (nbrPatchName_ == name)
	{
		FatalIOErrorInFunction
		(
			dict
		) << "Neighbour patch name " << nbrPatchName_
			<< " cannot be the same as this patch " << name
			<< exit(FatalIOError);
	}

	switch (transform())
	{
	case ROTATIONAL:
	{
		dict.lookup("rotationAxis") >> rotationAxis_;
		dict.lookup("rotationCentre") >> rotationCentre_;
		if (dict.readIfPresent("rotationAngle", rotationAngle_))
		{
			rotationAngleDefined_ = true;
			rotationAngle_ = degToRad(rotationAngle_);

			if (debug)
			{
				Info << "rotationAngle: " << rotationAngle_ << " [rad]"
					<< endl;
			}
		}

		scalar magRot = mag(rotationAxis_);
		if (magRot < small)
		{
			FatalIOErrorInFunction
			(
				dict
			) << "Illegal rotationAxis " << rotationAxis_ << endl
				<< "Please supply a non-zero vector."
				<< exit(FatalIOError);
		}
		rotationAxis_ /= magRot;

		break;
	}
	case TRANSLATIONAL:
	{
		dict.lookup("separationVector") >> separationVector_;
		break;
	}
	default:
	{
		// No additional info required
	}
	}

	// Neighbour patch might not be valid yet so no transformation
	// calculation possible
}


tnbLib::cyclicAMIPolyPatch::cyclicAMIPolyPatch
(
	const cyclicAMIPolyPatch& pp,
	const polyBoundaryMesh& bm
)
	:
	coupledPolyPatch(pp, bm),
	nbrPatchName_(pp.nbrPatchName_),
	coupleGroup_(pp.coupleGroup_),
	nbrPatchID_(-1),
	rotationAxis_(pp.rotationAxis_),
	rotationCentre_(pp.rotationCentre_),
	rotationAngleDefined_(pp.rotationAngleDefined_),
	rotationAngle_(pp.rotationAngle_),
	separationVector_(pp.separationVector_),
	AMIs_(),
	AMITransforms_(),
	AMIReverse_(pp.AMIReverse_),
	AMIRequireMatch_(pp.AMIRequireMatch_),
	AMILowWeightCorrection_(pp.AMILowWeightCorrection_),
	AMIMethod_(pp.AMIMethod_),
	surfPtr_(nullptr),
	surfDict_(pp.surfDict_)
{
	// Neighbour patch might not be valid yet so no transformation
	// calculation possible
}


tnbLib::cyclicAMIPolyPatch::cyclicAMIPolyPatch
(
	const cyclicAMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const label newSize,
	const label newStart,
	const word& nbrPatchName
)
	:
	coupledPolyPatch(pp, bm, index, newSize, newStart),
	nbrPatchName_(nbrPatchName),
	coupleGroup_(pp.coupleGroup_),
	nbrPatchID_(-1),
	rotationAxis_(pp.rotationAxis_),
	rotationCentre_(pp.rotationCentre_),
	rotationAngleDefined_(pp.rotationAngleDefined_),
	rotationAngle_(pp.rotationAngle_),
	separationVector_(pp.separationVector_),
	AMIs_(),
	AMITransforms_(),
	AMIReverse_(pp.AMIReverse_),
	AMIRequireMatch_(pp.AMIRequireMatch_),
	AMILowWeightCorrection_(pp.AMILowWeightCorrection_),
	AMIMethod_(pp.AMIMethod_),
	surfPtr_(nullptr),
	surfDict_(pp.surfDict_)
{
	if (nbrPatchName_ == name())
	{
		FatalErrorInFunction
			<< "Neighbour patch name " << nbrPatchName_
			<< " cannot be the same as this patch " << name()
			<< exit(FatalError);
	}

	// Neighbour patch might not be valid yet so no transformation
	// calculation possible
}


tnbLib::cyclicAMIPolyPatch::cyclicAMIPolyPatch
(
	const cyclicAMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const labelUList& mapAddressing,
	const label newStart
)
	:
	coupledPolyPatch(pp, bm, index, mapAddressing, newStart),
	nbrPatchName_(pp.nbrPatchName_),
	coupleGroup_(pp.coupleGroup_),
	nbrPatchID_(-1),
	rotationAxis_(pp.rotationAxis_),
	rotationCentre_(pp.rotationCentre_),
	rotationAngleDefined_(pp.rotationAngleDefined_),
	rotationAngle_(pp.rotationAngle_),
	separationVector_(pp.separationVector_),
	AMIs_(),
	AMITransforms_(),
	AMIReverse_(pp.AMIReverse_),
	AMIRequireMatch_(pp.AMIRequireMatch_),
	AMILowWeightCorrection_(pp.AMILowWeightCorrection_),
	AMIMethod_(pp.AMIMethod_),
	surfPtr_(nullptr),
	surfDict_(pp.surfDict_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tnbLib::cyclicAMIPolyPatch::~cyclicAMIPolyPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tnbLib::label tnbLib::cyclicAMIPolyPatch::neighbPatchID() const
{
	if (nbrPatchID_ == -1)
	{
		nbrPatchID_ = this->boundaryMesh().findPatchID(neighbPatchName());

		if (nbrPatchID_ == -1)
		{
			FatalErrorInFunction
				<< "Illegal neighbourPatch name " << neighbPatchName()
				<< nl << "Valid patch names are "
				<< this->boundaryMesh().names()
				<< exit(FatalError);
		}

		// Check that it is a cyclic AMI patch
		const cyclicAMIPolyPatch& nbrPatch =
			refCast<const cyclicAMIPolyPatch>
			(
				this->boundaryMesh()[nbrPatchID_]
				);

		if (nbrPatch.neighbPatchName() != name())
		{
			WarningInFunction
				<< "Patch " << name()
				<< " specifies neighbour patch " << neighbPatchName()
				<< nl << " but that in return specifies "
				<< nbrPatch.neighbPatchName() << endl;
		}
	}

	return nbrPatchID_;
}


bool tnbLib::cyclicAMIPolyPatch::owner() const
{
	return index() < neighbPatchID();
}


const tnbLib::cyclicAMIPolyPatch& tnbLib::cyclicAMIPolyPatch::neighbPatch() const
{
	const polyPatch& pp = this->boundaryMesh()[neighbPatchID()];
	return refCast<const cyclicAMIPolyPatch>(pp);
}


const tnbLib::autoPtr<tnbLib::searchableSurface>&
tnbLib::cyclicAMIPolyPatch::surfPtr() const
{
	const word surfType(surfDict_.lookupOrDefault<word>("type", "none"));

	if (!surfPtr_.valid() && owner() && surfType != "none")
	{
		word surfName(surfDict_.lookupOrDefault("name", name()));

		const polyMesh& mesh = boundaryMesh().mesh();

		surfPtr_ =
			searchableSurface::New
			(
				surfType,
				IOobject
				(
					surfName,
					mesh.time().constant(),
					"triSurface",
					mesh,
					IOobject::MUST_READ,
					IOobject::NO_WRITE
				),
				surfDict_
			);
	}

	return surfPtr_;
}


const tnbLib::PtrList<tnbLib::AMIInterpolation>&
tnbLib::cyclicAMIPolyPatch::AMIs() const
{
	if (!owner())
	{
		FatalErrorInFunction
			<< "AMI interpolators only available to owner patch"
			<< abort(FatalError);
	}

	if (AMIs_.empty())
	{
		resetAMI();
	}

	return AMIs_;
}


const tnbLib::List<tnbLib::vectorTensorTransform>&
tnbLib::cyclicAMIPolyPatch::AMITransforms() const
{
	if (!owner())
	{
		FatalErrorInFunction
			<< "AMI transforms only available to owner patch"
			<< abort(FatalError);
	}

	if (AMIs_.empty())
	{
		resetAMI();
	}

	return AMITransforms_;
}


bool tnbLib::cyclicAMIPolyPatch::applyLowWeightCorrection() const
{
	if (owner())
	{
		return AMILowWeightCorrection_ > 0;
	}
	else
	{
		return neighbPatch().AMILowWeightCorrection_ > 0;
	}
}


const tnbLib::scalarField& tnbLib::cyclicAMIPolyPatch::weightsSum() const
{
	if (owner())
	{
		return AMIs()[0].srcWeightsSum();
	}
	else
	{
		return neighbPatch().AMIs()[0].tgtWeightsSum();
	}
}


const tnbLib::scalarField& tnbLib::cyclicAMIPolyPatch::neighbWeightsSum() const
{
	if (owner())
	{
		return AMIs()[0].tgtWeightsSum();
	}
	else
	{
		return neighbPatch().AMIs()[0].srcWeightsSum();
	}
}


void tnbLib::cyclicAMIPolyPatch::transformPosition(pointField& l) const
{
	if (!parallel())
	{
		if (transform() == ROTATIONAL)
		{
			l = tnbLib::transform(forwardT(), l - rotationCentre_)
				+ rotationCentre_;
		}
		else
		{
			l = tnbLib::transform(forwardT(), l);
		}
	}
	else if (separated())
	{
		// transformPosition gets called on the receiving side,
		// separation gets calculated on the sending side so subtract

		const vectorField& s = separation();
		if (s.size() == 1)
		{
			forAll(l, i)
			{
				l[i] -= s[0];
			}
		}
		else
		{
			l -= s;
		}
	}
}


void tnbLib::cyclicAMIPolyPatch::transformPosition
(
	point& l,
	const label facei
) const
{
	if (!parallel())
	{
		const tensor& T =
			(
				forwardT().size() == 1
				? forwardT()[0]
				: forwardT()[facei]
				);

		if (transform() == ROTATIONAL)
		{
			l = tnbLib::transform(T, l - rotationCentre_) + rotationCentre_;
		}
		else
		{
			l = tnbLib::transform(T, l);
		}
	}
	else if (separated())
	{
		const vector& s =
			(
				separation().size() == 1
				? separation()[0]
				: separation()[facei]
				);

		l -= s;
	}
}


void tnbLib::cyclicAMIPolyPatch::transformDirection
(
	vector& d,
	const label facei
) const
{
	if (!parallel())
	{
		const tensor& T =
			(
				forwardT().size() == 1
				? forwardT()[0]
				: forwardT()[facei]
				);

		d = tnbLib::transform(T, d);
	}
}


void tnbLib::cyclicAMIPolyPatch::reverseTransformPosition
(
	point& l,
	const label facei
) const
{
	if (!parallel())
	{
		const tensor& T =
			(
				reverseT().size() == 1
				? reverseT()[0]
				: reverseT()[facei]
				);

		if (transform() == ROTATIONAL)
		{
			l = tnbLib::transform(T, l - rotationCentre_) + rotationCentre_;
		}
		else
		{
			l = tnbLib::transform(T, l);
		}
	}
	else if (separated())
	{
		const vector& s =
			(
				separation().size() == 1
				? separation()[0]
				: separation()[facei]
				);

		l += s;
	}
}


void tnbLib::cyclicAMIPolyPatch::reverseTransformDirection
(
	vector& d,
	const label facei
) const
{
	if (!parallel())
	{
		const tensor& T =
			(
				reverseT().size() == 1
				? reverseT()[0]
				: reverseT()[facei]
				);

		d = tnbLib::transform(T, d);
	}
}


tnbLib::tmp<tnbLib::scalarField> tnbLib::cyclicAMIPolyPatch::interpolate
(
	const scalarField& fld,
	const direction cmpt,
	const direction rank,
	const scalarUList& defaultValues
) const
{
	const cyclicAMIPolyPatch& nei = neighbPatch();

	tmp<scalarField> result(new scalarField(size(), Zero));

	if (owner())
	{
		forAll(AMIs(), i)
		{
			const scalar r =
				pow(inv(AMITransforms()[i]).R()(cmpt, cmpt), rank);

			result.ref() +=
				AMIs()[i].interpolateToSource(r*fld, defaultValues);
		}
	}
	else
	{
		forAll(nei.AMIs(), i)
		{
			const scalar r =
				pow(nei.AMITransforms()[i].R()(cmpt, cmpt), rank);

			result.ref() +=
				nei.AMIs()[i].interpolateToTarget(r*fld, defaultValues);
		}
	}

	return result;
}


void tnbLib::cyclicAMIPolyPatch::calcGeometry
(
	const primitivePatch& referPatch,
	const pointField& thisCtrs,
	const vectorField& thisAreas,
	const pointField& thisCc,
	const pointField& nbrCtrs,
	const vectorField& nbrAreas,
	const pointField& nbrCc
)
{
	calcTransforms
	(
		referPatch,
		thisCtrs,
		thisAreas,
		nbrCtrs,
		nbrAreas
	);
}


void tnbLib::cyclicAMIPolyPatch::initOrder
(
	PstreamBuffers& pBufs,
	const primitivePatch& pp
) const
{}


bool tnbLib::cyclicAMIPolyPatch::order
(
	PstreamBuffers& pBufs,
	const primitivePatch& pp,
	labelList& faceMap,
	labelList& rotation
) const
{
	faceMap.setSize(pp.size());
	faceMap = -1;

	rotation.setSize(pp.size());
	rotation = 0;

	return false;
}


tnbLib::labelPair tnbLib::cyclicAMIPolyPatch::pointAMIAndFace
(
	const label facei,
	const vector& n,
	point& p
) const
{
	point pt(p);
	reverseTransformPosition(pt, facei);

	vector nt(n);
	reverseTransformDirection(nt, facei);

	if (owner())
	{
		forAll(AMIs(), i)
		{
			point ptt = AMITransforms()[i].transformPosition(pt);
			const vector ntt = AMITransforms()[i].transform(nt);

			const label nbrFacei =
				AMIs()[i].tgtPointFace(*this, neighbPatch(), ntt, facei, ptt);

			if (nbrFacei >= 0)
			{
				p = ptt;
				return labelPair(i, nbrFacei);
			}
		}
	}
	else
	{
		forAll(neighbPatch().AMIs(), i)
		{
			point ptt =
				neighbPatch().AMITransforms()[i].invTransformPosition(pt);
			const vector ntt =
				neighbPatch().AMITransforms()[i].invTransform(nt);

			const label nbrFacei =
				neighbPatch().AMIs()[i].srcPointFace
				(
					neighbPatch(),
					*this,
					ntt,
					facei,
					ptt
				);

			if (nbrFacei >= 0)
			{
				p = ptt;
				return labelPair(i, nbrFacei);
			}
		}
	}

	return labelPair(-1, -1);
}


tnbLib::label tnbLib::cyclicAMIPolyPatch::singlePatchProc() const
{
	const cyclicAMIPolyPatch& patch = owner() ? *this : neighbPatch();

	const label proc = patch.AMIs()[0].singlePatchProc();

	for (label i = 1; i < patch.AMIs().size(); ++i)
	{
		if (patch.AMIs()[i].singlePatchProc() != proc)
		{
			return -1;
		}
	}

	return proc;
}


void tnbLib::cyclicAMIPolyPatch::write(Ostream& os) const
{
	coupledPolyPatch::write(os);
	if (!nbrPatchName_.empty())
	{
		writeEntry(os, "neighbourPatch", nbrPatchName_);
	}
	coupleGroup_.write(os);

	switch (transform())
	{
	case ROTATIONAL:
	{
		writeEntry(os, "rotationAxis", rotationAxis_);
		writeEntry(os, "rotationCentre", rotationCentre_);

		if (rotationAngleDefined_)
		{
			writeEntry(os, "rotationAngle", radToDeg(rotationAngle_));
		}

		break;
	}
	case TRANSLATIONAL:
	{
		writeEntry(os, "separationVector", separationVector_);
		break;
	}
	case NOORDERING:
	{
		break;
	}
	default:
	{
		// No additional info to write
	}
	}

	if (AMIReverse_)
	{
		writeEntry(os, "flipNormals", AMIReverse_);
	}

	if (AMILowWeightCorrection_ > 0)
	{
		writeEntry(os, "lowWeightCorrection", AMILowWeightCorrection_);
	}

	writeEntry
	(
		os,
		"method",
		AMIInterpolation::interpolationMethodToWord(AMIMethod_)
	);

	if (!surfDict_.empty())
	{
		os.writeKeyword(surfDict_.dictName());
		os << surfDict_;
	}
}


// ************************************************************************* //