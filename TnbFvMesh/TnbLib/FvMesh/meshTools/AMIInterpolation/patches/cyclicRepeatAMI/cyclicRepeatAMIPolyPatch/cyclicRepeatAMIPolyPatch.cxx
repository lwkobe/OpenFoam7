#include <cyclicRepeatAMIPolyPatch.hxx>

#include <SubField.hxx>
#include <Time.hxx>
#include <addToRunTimeSelectionTable.hxx>

// added by amir
#include <SubList.hxx>
#include <Tuple2.hxx>
#include <PstreamReduceOps.hxx>
#include <faceList.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	defineTypeNameAndDebug(cyclicRepeatAMIPolyPatch, 0);

	addToRunTimeSelectionTable(polyPatch, cyclicRepeatAMIPolyPatch, word);
	addToRunTimeSelectionTable(polyPatch, cyclicRepeatAMIPolyPatch, dictionary);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	template <class Type>
	class keepIfTrueOp
	{
	public:

		Tuple2<bool, Type> operator()
			(
				const Tuple2<bool, Type>& x,
				const Tuple2<bool, Type>& y
				) const
		{
			if (x.first())
			{
				return x;
			}
			else if (y.first())
			{
				return y;
			}
			else
			{
				return Tuple2<bool, Type>(false, Type());
			}
		}
	};

}


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

void tnbLib::cyclicRepeatAMIPolyPatch::resetAMI() const
{
	if (!owner())
	{
		return;
	}

	Info << indent << typeName << " : Creating addressing and weights between "
		<< returnReduce(this->size(), sumOp<label>()) << " source faces and "
		<< returnReduce(neighbPatch().size(), sumOp<label>()) << " target faces"
		<< endl;

	// Get the transform associated with the transform patch
	vectorTensorTransform t;
	{
		const coupledPolyPatch& transformPatch = this->transformPatch();

		if (transformPatch.name() != neighbPatch().transformPatch().name())
		{
			FatalErrorInFunction
				<< "Transform patch " << transformPatch.name() << " for "
				<< typeName << " patch " << name() << " is not the same as for "
				<< "the neighbour patch " << neighbPatch().name() << ". "
				<< "This is not allowed." << exit(FatalError);
		}

		if
			(
				transformPatch.separation().size() > 1
				|| transformPatch.forwardT().size() > 1
				)
		{
			FatalErrorInFunction
				<< "Transform patch " << transformPatch.name() << " for "
				<< typeName << " patch " << name() << " has a non-uniform "
				<< "transformation. This is not allowed."
				<< exit(FatalError);
		}

		Tuple2<bool, vectorTensorTransform> bt
		(
			transformPatch.size(),
			vectorTensorTransform
			(
				transformPatch.separation().size() > 0
				? transformPatch.separation()[0]
				: vector::zero,
				transformPatch.forwardT().size() > 0
				? transformPatch.forwardT()[0]
				: tensor::zero,
				transformPatch.forwardT().size() > 0
			)
		);

		reduce(bt, keepIfTrueOp<vectorTensorTransform>());

		if (!bt.first())
		{
			FatalErrorInFunction
				<< "Transform patch " << transformPatch.name() << " for "
				<< typeName << " patch " << name() << " has zero faces. It may "
				<< "have been converted to a processor cyclic during "
				<< "decomposition. Consider adding " << transformPatch.name()
				<< " and it's neighbour to the list of preserved patches."
				<< exit(FatalError);
		}

		t = bt.second();
	}
	const vectorTensorTransform tInv(inv(t));

	// Work out the number of repetitions of the transform that separate this
	// patch from its neighbour
	label n = 0;
	{
		const scalarField thisMagAreas(mag(this->faceAreas()));
		const scalarField nbrMagAreas(mag(neighbPatch().faceAreas()));

		vector thisCentre =
			gSum(this->faceCentres()*thisMagAreas) / gSum(thisMagAreas);
		vector nbrCentre =
			gSum(neighbPatch().faceCentres()*nbrMagAreas) / gSum(nbrMagAreas);

		scalar dLeft = mag(t.transformPosition(thisCentre) - nbrCentre);
		scalar d = mag(thisCentre - nbrCentre);
		scalar dRight = mag(tInv.transformPosition(thisCentre) - nbrCentre);

		while (dLeft < d)
		{
			thisCentre = t.transformPosition(thisCentre);

			dRight = d;
			d = dLeft;
			dLeft = mag(t.transformPosition(thisCentre) - nbrCentre);

			++n;
		}

		while (dRight < d)
		{
			thisCentre = tInv.transformPosition(thisCentre);

			dLeft = d;
			d = dRight;
			dRight = mag(tInv.transformPosition(thisCentre) - nbrCentre);

			--n;
		}
	}

	// Generate the full transformations
	vectorTensorTransform TLeft(t), T(vectorTensorTransform::I), TRight(tInv);
	if (n > 0)
	{
		for (label i = 0; i < n - 1; ++i)
		{
			T = t & T;
		}

		TLeft = T;
		T = t & T;
		TRight = t & T;
	}
	if (n < 0)
	{
		for (label i = 0; i > n + 1; --i)
		{
			T = tInv & T;
		}

		TRight = T;
		T = tInv & T;
		TLeft = tInv & T;
	}

	// Create copies of this patch and the neighbour patch's points
	pointField thisPoints(localPoints());
	const pointField nbrPoints(neighbPatch().localPoints());

	// Create primitive patches
	primitivePatch thisPatch
	(
		SubList<face>(localFaces(), size()),
		thisPoints
	);
	primitivePatch nbrPatch
	(
		SubList<face>(neighbPatch().localFaces(), neighbPatch().size()),
		nbrPoints
	);

	// Do the three bounding AMI interpolations
	thisPoints = TLeft.transformPosition(localPoints());
	thisPatch.movePoints(thisPoints);
	autoPtr<AMIInterpolation> AMILeftPtr
	(
		new AMIInterpolation
		(
			thisPatch,
			nbrPatch,
			faceAreaIntersect::tmMesh,
			false,
			AMIInterpolation::imPartialFaceAreaWeight,
			AMILowWeightCorrection_,
			AMIReverse_,
			false
		)
	);
	const scalar sLeft =
		gSum(AMILeftPtr->srcWeightsSum()*AMILeftPtr->srcMagSf())
		/ gSum(AMILeftPtr->srcMagSf());

	thisPoints = T.transformPosition(localPoints());
	thisPatch.movePoints(thisPoints);
	autoPtr<AMIInterpolation> AMIPtr
	(
		new AMIInterpolation
		(
			thisPatch,
			nbrPatch,
			faceAreaIntersect::tmMesh,
			false,
			AMIInterpolation::imPartialFaceAreaWeight,
			AMILowWeightCorrection_,
			AMIReverse_,
			false
		)
	);
	const scalar s =
		gSum(AMIPtr->srcWeightsSum()*AMIPtr->srcMagSf())
		/ gSum(AMIPtr->srcMagSf());

	thisPoints = TRight.transformPosition(localPoints());
	thisPatch.movePoints(thisPoints);
	autoPtr<AMIInterpolation> AMIRightPtr
	(
		new AMIInterpolation
		(
			thisPatch,
			nbrPatch,
			faceAreaIntersect::tmMesh,
			false,
			AMIInterpolation::imPartialFaceAreaWeight,
			AMILowWeightCorrection_,
			AMIReverse_,
			false
		)
	);
	const scalar sRight =
		gSum(AMIRightPtr->srcWeightsSum()*AMIRightPtr->srcMagSf())
		/ gSum(AMIRightPtr->srcMagSf());

	Info << typeName << ": number of transforms = " << n << endl
		<< typeName << ": left/centre/right sum(weights) = "
		<< sLeft << ", " << s << ", " << sRight << endl;

	// Set the AMI interpolators and transforms using the centre and the most
	// overlapping of the left and right sides
	AMIs_.resize(2);
	AMIs_.set(0, sLeft > sRight ? AMILeftPtr.ptr() : AMIPtr.ptr());
	AMIs_.set(1, sLeft > sRight ? AMIPtr.ptr() : AMIRightPtr.ptr());

	AMITransforms_.resize(2);
	AMITransforms_[0] = sLeft > sRight ? TLeft : T;
	AMITransforms_[1] = sLeft > sRight ? T : TRight;

	// Sum and normalise the two AMI interpolators
	AMIInterpolation::sumWeights(AMIs_);
	AMIInterpolation::reportSumWeights(AMIs_[0]);
	AMIInterpolation::normaliseWeights(AMIs_);
}


// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

tnbLib::cyclicRepeatAMIPolyPatch::cyclicRepeatAMIPolyPatch
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
		AMIInterpolation::imFaceAreaWeight
	),
	transformPatchName_(word::null),
	transformPatchID_(-1)
{
	// Transform patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicRepeatAMIPolyPatch::cyclicRepeatAMIPolyPatch
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
		AMIInterpolation::imFaceAreaWeight
	),
	transformPatchName_(dict.lookup("transformPatch")),
	transformPatchID_(-1)
{
	// Transform patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicRepeatAMIPolyPatch::cyclicRepeatAMIPolyPatch
(
	const cyclicRepeatAMIPolyPatch& pp,
	const polyBoundaryMesh& bm
)
	:
	cyclicAMIPolyPatch(pp, bm),
	transformPatchName_(pp.transformPatchName_),
	transformPatchID_(-1)
{
	// Transform patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicRepeatAMIPolyPatch::cyclicRepeatAMIPolyPatch
(
	const cyclicRepeatAMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const label newSize,
	const label newStart,
	const word& nbrPatchName
)
	:
	cyclicAMIPolyPatch(pp, bm, index, newSize, newStart, nbrPatchName),
	transformPatchName_(pp.transformPatchName_),
	transformPatchID_(-1)
{
	// Transform patch might not be valid yet so cannot determine
	// associated patchID
}


tnbLib::cyclicRepeatAMIPolyPatch::cyclicRepeatAMIPolyPatch
(
	const cyclicRepeatAMIPolyPatch& pp,
	const polyBoundaryMesh& bm,
	const label index,
	const labelUList& mapAddressing,
	const label newStart
)
	:
	cyclicAMIPolyPatch(pp, bm, index, mapAddressing, newStart),
	transformPatchName_(pp.transformPatchName_),
	transformPatchID_(-1)
{
	// Transform patch might not be valid yet so cannot determine
	// associated patchID
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tnbLib::cyclicRepeatAMIPolyPatch::~cyclicRepeatAMIPolyPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const tnbLib::cyclicRepeatAMIPolyPatch&
tnbLib::cyclicRepeatAMIPolyPatch::neighbPatch() const
{
	const polyPatch& pp = this->boundaryMesh()[neighbPatchID()];

	return refCast<const cyclicRepeatAMIPolyPatch>(pp);
}


tnbLib::label tnbLib::cyclicRepeatAMIPolyPatch::transformPatchID() const
{
	if (transformPatchID_ == -1)
	{
		transformPatchID_ =
			this->boundaryMesh().findPatchID(transformPatchName());

		if (transformPatchID_ == -1)
		{
			FatalErrorInFunction
				<< "Illegal transformPatch name " << transformPatchName()
				<< nl << "Valid patch names are "
				<< this->boundaryMesh().names()
				<< exit(FatalError);
		}
	}

	return transformPatchID_;
}


const tnbLib::coupledPolyPatch&
tnbLib::cyclicRepeatAMIPolyPatch::transformPatch() const
{
	const polyPatch& pp = this->boundaryMesh()[transformPatchID()];

	return refCast<const coupledPolyPatch>(pp);
}


const tnbLib::scalarField& tnbLib::cyclicRepeatAMIPolyPatch::weightsSum() const
{
	// The two AMI-interpolation classes have their weights summed together, so
	// both should contain the same weights sum field. We can, therefore
	// delegate to the base class and just return the weights sum of the first.

	return cyclicAMIPolyPatch::weightsSum();
}


const tnbLib::scalarField&
tnbLib::cyclicRepeatAMIPolyPatch::neighbWeightsSum() const
{
	// See above.

	return cyclicAMIPolyPatch::neighbWeightsSum();
}


void tnbLib::cyclicRepeatAMIPolyPatch::write(Ostream& os) const
{
	cyclicAMIPolyPatch::write(os);
	writeEntry(os, "transformPatch", transformPatchName_);
}


// ************************************************************************* //