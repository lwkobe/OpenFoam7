#include <cyclicACMIPolyPatch.hxx>

#include <SubField.hxx>
#include <Time.hxx>
#include <polyMesh.hxx>  // added by amir
#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	defineTypeNameAndDebug(cyclicACMIPolyPatch, 0);

	addToRunTimeSelectionTable(polyPatch, cyclicACMIPolyPatch, word);
	addToRunTimeSelectionTable(polyPatch, cyclicACMIPolyPatch, dictionary);
}

const tnbLib::scalar tnbLib::cyclicACMIPolyPatch::tolerance_ = 1e-10;

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void tnbLib::cyclicACMIPolyPatch::resetAMI() const
{
	if (owner())
	{
		const polyPatch& nonOverlapPatch = this->nonOverlapPatch();

		if (debug)
		{
			Pout << "cyclicACMIPolyPatch::resetAMI : recalculating weights"
				<< " for " << name() << " and " << nonOverlapPatch.name()
				<< endl;
		}

		if (boundaryMesh().mesh().hasCellCentres())
		{
			if (debug)
			{
				Pout << "cyclicACMIPolyPatch::resetAMI : clearing cellCentres"
					<< " for " << name() << " and " << nonOverlapPatch.name()
					<< endl;
			}

			const_cast<polyMesh&>
				(
					boundaryMesh().mesh()
					).primitiveMesh::clearGeom();
		}

		// Trigger re-building of faceAreas
		(void)boundaryMesh().mesh().faceAreas();

		// Calculate the AMI using partial face-area-weighted. This leaves
		// the weights as fractions of local areas (sum(weights) = 1 means
		// face is fully covered)
		cyclicAMIPolyPatch::resetAMI();

		AMIInterpolation& AMI = this->AMIs_[0];

		srcMask_ =
			min(scalar(1) - tolerance_, max(tolerance_, AMI.srcWeightsSum()));

		tgtMask_ =
			min(scalar(1) - tolerance_, max(tolerance_, AMI.tgtWeightsSum()));


		// Adapt owner side areas. Note that in uncoupled situations (e.g.
		// decomposePar) srcMask, tgtMask can be zero size.
		if (srcMask_.size())
		{
			vectorField::subField Sf = faceAreas();
			vectorField::subField noSf = nonOverlapPatch.faceAreas();

			forAll(Sf, facei)
			{
				Sf[facei] *= srcMask_[facei];
				noSf[facei] *= 1.0 - srcMask_[facei];
			}
		}
		// Adapt slave side areas
		if (tgtMask_.size())
		{
			const cyclicACMIPolyPatch& cp =
				refCast<const cyclicACMIPolyPatch>(this->neighbPatch());
			const polyPatch& pp = cp.nonOverlapPatch();

			vectorField::subField Sf = cp.faceAreas();
			vectorField::subField noSf = pp.faceAreas();

			forAll(Sf, facei)
			{
				Sf[facei] *= tgtMask_[facei];
				noSf[facei] *= 1.0 - tgtMask_[facei];
			}
		}

		// Re-normalise the weights since the effect of overlap is already
		// accounted for in the area.
		{
			scalarListList& srcWeights = AMI.srcWeights();
			scalarField& srcWeightsSum = AMI.srcWeightsSum();
			forAll(srcWeights, i)
			{
				scalarList& wghts = srcWeights[i];
				if (wghts.size())
				{
					scalar& sum = srcWeightsSum[i];

					forAll(wghts, j)
					{
						wghts[j] /= sum;
					}
					sum = 1.0;
				}
			}
		}
		{
			scalarListList& tgtWeights = AMI.tgtWeights();
			scalarField& tgtWeightsSum = AMI.tgtWeightsSum();
			forAll(tgtWeights, i)
			{
				scalarList& wghts = tgtWeights[i];
				if (wghts.size())
				{
					scalar& sum = tgtWeightsSum[i];
					forAll(wghts, j)
					{
						wghts[j] /= sum;
					}
					sum = 1.0;
				}
			}
		}

		// Set the updated flag
		updated_ = true;
	}
}


void tnbLib::cyclicACMIPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
	cyclicAMIPolyPatch::initGeometry(pBufs);

	// Initialise the AMI
	resetAMI();
}


void tnbLib::cyclicACMIPolyPatch::initMovePoints
(
	PstreamBuffers& pBufs,
	const pointField& p
)
{
	cyclicAMIPolyPatch::initMovePoints(pBufs, p);

	// Initialise the AMI
	resetAMI();
}


const tnbLib::scalarField& tnbLib::cyclicACMIPolyPatch::srcMask() const
{
	return srcMask_;
}


const tnbLib::scalarField& tnbLib::cyclicACMIPolyPatch::tgtMask() const
{
	return tgtMask_;
}


// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

tnbLib::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
	const word& name,
	const label size,
	const label start,
	const label index,
	const polyBoundaryMesh& bm,
	const word& patchType,
	const transformType transform
)
	:
	cyclicAMIPolyPatch
	(
		name,
		size,
		start,
		index,
		bm,
		patchType,
		transform,
		false,
		AMIInterpolation::imPartialFaceAreaWeight
	),
	nonOverlapPatchName_(word::null),
	nonOverlapPatchID_(-1),
	srcMask_(),
	tgtMask_(),
	updated_(false)
{
	// Non-overlapping patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
	const word& name,
	const dictionary& dict,
	const label index,
	const polyBoundaryMesh& bm,
	const word& patchType
)
	:
	cyclicAMIPolyPatch
	(
		name,
		dict,
		index,
		bm,
		patchType,
		false,
		AMIInterpolation::imPartialFaceAreaWeight
	),
	nonOverlapPatchName_(dict.lookup("nonOverlapPatch")),
	nonOverlapPatchID_(-1),
	srcMask_(),
	tgtMask_(),
	updated_(false)
{
	if (nonOverlapPatchName_ == name)
	{
		FatalIOErrorInFunction
		(
			dict
		) << "Non-overlapping patch name " << nonOverlapPatchName_
			<< " cannot be the same as this patch " << name
			<< exit(FatalIOError);
	}

	// Non-overlapping patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
	const cyclicACMIPolyPatch& pp,
	const polyBoundaryMesh& bm
)
	:
	cyclicAMIPolyPatch(pp, bm),
	nonOverlapPatchName_(pp.nonOverlapPatchName_),
	nonOverlapPatchID_(-1),
	srcMask_(),
	tgtMask_(),
	updated_(false)
{
	// Non-overlapping patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
	const cyclicACMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const label newSize,
	const label newStart,
	const word& nbrPatchName,
	const word& nonOverlapPatchName
)
	:
	cyclicAMIPolyPatch(pp, bm, index, newSize, newStart, nbrPatchName),
	nonOverlapPatchName_(nonOverlapPatchName),
	nonOverlapPatchID_(-1),
	srcMask_(),
	tgtMask_(),
	updated_(false)
{
	if (nonOverlapPatchName_ == name())
	{
		FatalErrorInFunction
			<< "Non-overlapping patch name " << nonOverlapPatchName_
			<< " cannot be the same as this patch " << name()
			<< exit(FatalError);
	}

	// Non-overlapping patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
	const cyclicACMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const labelUList& mapAddressing,
	const label newStart
)
	:
	cyclicAMIPolyPatch(pp, bm, index, mapAddressing, newStart),
	nonOverlapPatchName_(pp.nonOverlapPatchName_),
	nonOverlapPatchID_(-1),
	srcMask_(),
	tgtMask_(),
	updated_(false)
{
	// Non-overlapping patch might not be valid yet so cannot determine
	// associated patchID
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tnbLib::cyclicACMIPolyPatch::~cyclicACMIPolyPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const tnbLib::cyclicACMIPolyPatch& tnbLib::cyclicACMIPolyPatch::neighbPatch() const
{
	const polyPatch& pp = this->boundaryMesh()[neighbPatchID()];
	return refCast<const cyclicACMIPolyPatch>(pp);
}


tnbLib::label tnbLib::cyclicACMIPolyPatch::nonOverlapPatchID() const
{
	if (nonOverlapPatchID_ == -1)
	{
		nonOverlapPatchID_ =
			this->boundaryMesh().findPatchID(nonOverlapPatchName_);

		if (nonOverlapPatchID_ == -1)
		{
			FatalErrorInFunction
				<< "Illegal non-overlapping patch name " << nonOverlapPatchName_
				<< nl << "Valid patch names are "
				<< this->boundaryMesh().names()
				<< exit(FatalError);
		}

		if (nonOverlapPatchID_ < index())
		{
			FatalErrorInFunction
				<< "Boundary ordering error: " << type()
				<< " patch must be defined prior to its non-overlapping patch"
				<< nl
				<< type() << " patch: " << name() << ", ID:" << index() << nl
				<< "Non-overlap patch: " << nonOverlapPatchName_
				<< ", ID:" << nonOverlapPatchID_ << nl
				<< exit(FatalError);
		}

		const polyPatch& noPp = this->boundaryMesh()[nonOverlapPatchID_];

		bool ok = true;

		if (size() == noPp.size())
		{
			const scalarField magSf(mag(faceAreas()));
			const scalarField noMagSf(mag(noPp.faceAreas()));

			forAll(magSf, facei)
			{
				scalar ratio = mag(magSf[facei] / (noMagSf[facei] + rootVSmall));

				if (ratio - 1 > tolerance_)
				{
					ok = false;
					break;
				}
			}
		}
		else
		{
			ok = false;
		}

		if (!ok)
		{
			FatalErrorInFunction
				<< "Inconsistent ACMI patches " << name() << " and "
				<< noPp.name() << ".  Patches should have identical topology"
				<< exit(FatalError);
		}
	}

	return nonOverlapPatchID_;
}


void tnbLib::cyclicACMIPolyPatch::calcGeometry
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
	cyclicAMIPolyPatch::calcGeometry
	(
		referPatch,
		thisCtrs,
		thisAreas,
		thisCc,
		nbrCtrs,
		nbrAreas,
		nbrCc
	);
}


void tnbLib::cyclicACMIPolyPatch::initOrder
(
	PstreamBuffers& pBufs,
	const primitivePatch& pp
) const
{
	cyclicAMIPolyPatch::initOrder(pBufs, pp);
}


bool tnbLib::cyclicACMIPolyPatch::order
(
	PstreamBuffers& pBufs,
	const primitivePatch& pp,
	labelList& faceMap,
	labelList& rotation
) const
{
	return cyclicAMIPolyPatch::order(pBufs, pp, faceMap, rotation);
}


void tnbLib::cyclicACMIPolyPatch::write(Ostream& os) const
{
	cyclicAMIPolyPatch::write(os);
	writeEntry(os, "nonOverlapPatch", nonOverlapPatchName_);
}


// ************************************************************************* //