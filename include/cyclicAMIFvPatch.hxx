#pragma once
#ifndef _cyclicAMIFvPatch_Header
#define _cyclicAMIFvPatch_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
	 \\/     M anipulation  |
-------------------------------------------------------------------------------
License
	This file is part of OpenFOAM.

	OpenFOAM is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License
	along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
	tnbLib::cyclicAMIFvPatch

Description
	Cyclic patch for Arbitrary Mesh Interface (AMI)

SourceFiles
	cyclicAMIFvPatch.C

\*---------------------------------------------------------------------------*/

#include <coupledFvPatch.hxx>
#include <cyclicAMILduInterface.hxx>
#include <cyclicAMIPolyPatch.hxx>
#include <fvBoundaryMesh.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						  Class cyclicAMIFvPatch Declaration
	\*---------------------------------------------------------------------------*/

	class cyclicAMIFvPatch
		:
		public coupledFvPatch,
		public cyclicAMILduInterface
	{
		// Private Data

		const cyclicAMIPolyPatch& cyclicAMIPolyPatch_;


	protected:

		// Protected Member functions

			//- Return the patch-normal component of the deltas
		FoamFiniteVolume_EXPORT tmp<scalarField> deltan() const;

		//- Return the neighbour patch-normal component of the deltas
		FoamFiniteVolume_EXPORT tmp<scalarField> nbrDeltan() const;

		//- Make patch weighting factors
		FoamFiniteVolume_EXPORT virtual void makeWeights(scalarField&) const;


	public:

		//- Runtime type information
		/*TypeName(cyclicAMIPolyPatch::typeName_());*/
		static const char* typeName_() { return cyclicAMIPolyPatch::typeName_(); }
		static FoamFiniteVolume_EXPORT const ::tnbLib::word typeName;
		static FoamFiniteVolume_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from polyPatch
		cyclicAMIFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
			:
			coupledFvPatch(patch, bm),
			cyclicAMILduInterface(),
			cyclicAMIPolyPatch_(refCast<const cyclicAMIPolyPatch>(patch))
		{}


		// Member Functions

			// Access

				//- Return local reference cast into the cyclic patch
		const cyclicAMIPolyPatch& cyclicAMIPatch() const
		{
			return cyclicAMIPolyPatch_;
		}

		//- Return the neighbour patch ID
		virtual label neighbPatchID() const
		{
			return cyclicAMIPolyPatch_.neighbPatchID();
		}

		//- Is this side the owner?
		virtual bool owner() const
		{
			return cyclicAMIPolyPatch_.owner();
		}

		//- Return the neighbour patch
		virtual const cyclicAMIFvPatch& neighbPatch() const
		{
			return refCast<const cyclicAMIFvPatch>
				(
					this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
					);
		}

		//- Return a reference to the AMI interpolators
		virtual const PtrList<AMIInterpolation>& AMIs() const
		{
			return cyclicAMIPolyPatch_.AMIs();
		}

		//- Return a reference to the AMI transforms
		virtual const List<vectorTensorTransform>& AMITransforms() const
		{
			return cyclicAMIPolyPatch_.AMITransforms();
		}

		//- Return true if applying the low weight correction
		virtual bool applyLowWeightCorrection() const
		{
			return cyclicAMIPolyPatch_.applyLowWeightCorrection();
		}

		//- Are the cyclic planes parallel
		virtual bool parallel() const
		{
			return cyclicAMIPolyPatch_.parallel();
		}

		//- Return face transformation tensor
		virtual const tensorField& forwardT() const
		{
			return cyclicAMIPolyPatch_.forwardT();
		}

		//- Return neighbour-cell transformation tensor
		virtual const tensorField& reverseT() const
		{
			return cyclicAMIPolyPatch_.reverseT();
		}

		const cyclicAMIFvPatch& neighbFvPatch() const
		{
			return refCast<const cyclicAMIFvPatch>
				(
					this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
					);
		}

		//- Return true if this patch is coupled. This is equivalent
		//  to the coupledPolyPatch::coupled() if parallel running or
		//  both sides present, false otherwise
		FoamFiniteVolume_EXPORT virtual bool coupled() const;

		//- Return delta (P to N) vectors across coupled patch
		FoamFiniteVolume_EXPORT virtual tmp<vectorField> delta() const;

		template<class Type>
		tmp<Field<Type>> interpolate
		(
			const Field<Type>& fld,
			const UList<Type>& defaultValues = UList<Type>()
		) const
		{
			return cyclicAMIPolyPatch_.interpolate(fld, defaultValues);
		}

		template<class Type>
		tmp<Field<Type>> interpolate
		(
			const tmp<Field<Type>>& tFld,
			const UList<Type>& defaultValues = UList<Type>()
		) const
		{
			return cyclicAMIPolyPatch_.interpolate(tFld, defaultValues);
		}


		// Interface transfer functions

			//- Return the values of the given internal data adjacent to
			//  the interface as a field
		FoamFiniteVolume_EXPORT virtual tmp<labelField> interfaceInternalField
		(
			const labelUList& internalData
		) const;

		//- Return neighbour field
		FoamFiniteVolume_EXPORT virtual tmp<labelField> internalFieldTransfer
		(
			const Pstream::commsTypes commsType,
			const labelUList& internalData
		) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_cyclicAMIFvPatch_Header
