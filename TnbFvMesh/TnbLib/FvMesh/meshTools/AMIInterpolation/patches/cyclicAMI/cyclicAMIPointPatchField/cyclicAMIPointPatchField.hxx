#pragma once
#ifndef _cyclicAMIPointPatchField_Header
#define _cyclicAMIPointPatchField_Header

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
	tnbLib::cyclicAMIPointPatchField

Description
	Cyclic AMI front and back plane patch field

SourceFiles
	cyclicAMIPointPatchField.C

\*---------------------------------------------------------------------------*/

#include <coupledPointPatchField.hxx>
#include <cyclicAMIPointPatch.hxx>
#include <PrimitivePatchInterpolationTemplate.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					  Class cyclicAMIPointPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class cyclicAMIPointPatchField
		:
		public coupledPointPatchField<Type>
	{
		// Private Data

			//- Local reference cast into the cyclicAMI patch
		const cyclicAMIPointPatch& cyclicAMIPatch_;

		//- Owner side patch interpolation pointer
		mutable autoPtr<PrimitivePatchInterpolation<primitivePatch>> ppiPtr_;

		//- Neighbour side patch interpolation pointer
		mutable autoPtr<PrimitivePatchInterpolation<primitivePatch>>
			nbrPpiPtr_;


		// Private Member Functions

			//- Owner side patch interpolation
		const PrimitivePatchInterpolation<primitivePatch>& ppi() const
		{
			if (!ppiPtr_.valid())
			{
				ppiPtr_.reset
				(
					new PrimitivePatchInterpolation<primitivePatch>
					(
						cyclicAMIPatch_.cyclicAMIPatch()
						)
				);
			}

			return ppiPtr_();
		}

		//- Neighbour side patch interpolation
		const PrimitivePatchInterpolation<primitivePatch>& nbrPpi() const
		{
			if (!nbrPpiPtr_.valid())
			{
				nbrPpiPtr_.reset
				(
					new PrimitivePatchInterpolation<primitivePatch>
					(
						cyclicAMIPatch_.cyclicAMIPatch().neighbPatch()
						)
				);
			}

			return nbrPpiPtr_();
		}


	public:

		//- Runtime type information
		//TypeName(cyclicAMIPointPatch::typeName_());
		static const char* typeName_() { return cyclicAMIPointPatch::typeName_(); }
		static FoamFvMesh_EXPORT const ::tnbLib::word typeName;
		static FoamFvMesh_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		cyclicAMIPointPatchField
		(
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Construct from patch, internal field and dictionary
		cyclicAMIPointPatchField
		(
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patchField<Type> onto a new patch
		cyclicAMIPointPatchField
		(
			const cyclicAMIPointPatchField<Type>&,
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const pointPatchFieldMapper&
		);

		//- Construct and return a clone
		virtual autoPtr<pointPatchField<Type>> clone() const
		{
			return autoPtr<pointPatchField<Type>>
				(
					new cyclicAMIPointPatchField<Type>
					(
						*this
						)
					);
		}

		//- Construct as copy setting internal field reference
		cyclicAMIPointPatchField
		(
			const cyclicAMIPointPatchField<Type>&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual autoPtr<pointPatchField<Type>> clone
		(
			const DimensionedField<Type, pointMesh>& iF
		) const
		{
			return autoPtr<pointPatchField<Type>>
				(
					new cyclicAMIPointPatchField<Type>
					(
						*this, iF
						)
					);
		}


		// Member Functions

			// Constraint handling

				//- Return the constraint type this pointPatchField implements
		virtual const word& constraintType() const
		{
			return cyclicAMIPointPatch::typeName;
		}


		// Cyclic AMI coupled interface functions

			//- Does the patch field perform the transformation
		virtual bool doTransform() const
		{
			return
				!(
					cyclicAMIPatch_.parallel()
					|| pTraits<Type>::rank == 0
					);
		}

		//- Return face transformation tensor
		virtual const tensorField& forwardT() const
		{
			return cyclicAMIPatch_.forwardT();
		}

		//- Return neighbour-cell transformation tensor
		virtual const tensorField& reverseT() const
		{
			return cyclicAMIPatch_.reverseT();
		}


		// Evaluation functions

			//- Return true if coupled. Note that the underlying patch
			//  is not coupled() - the points don't align.
		virtual bool coupled() const;

		//- Evaluate the patch field
		virtual void evaluate
		(
			const Pstream::commsTypes commsType =
			Pstream::commsTypes::blocking
		)
		{}

		//- Complete swap of patch point values and add to local values
		virtual void swapAddSeparated
		(
			const Pstream::commsTypes commsType,
			Field<Type>&
		) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <cyclicAMIPointPatchFieldI.hxx>

//#ifdef NoRepository
//#include <cyclicAMIPointPatchField.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_cyclicAMIPointPatchField_Header
