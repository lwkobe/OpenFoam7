#pragma once
#ifndef _fixedJumpFvPatchField_Header
#define _fixedJumpFvPatchField_Header

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
	tnbLib::fixedJumpFvPatchField

Description
	This boundary condition provides a jump condition, using the \c cyclic
	condition as a base.

	The jump is specified as a fixed value field, applied as an offset to the
	'owner' patch.

Usage
	\table
		Property     | Description             | Required    | Default value
		patchType    | underlying patch type should be \c cyclic| yes |
		jump         | current jump value      | yes         |
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            fixedJump;
		patchType       cyclic;
		jump            uniform 10;
	}
	\endverbatim

	The above example shows the use of a fixed jump of '10'.

Note
	 The underlying \c patchType should be set to \c cyclic

See also
	tnbLib::jumpCyclicFvPatchField

SourceFiles
	fixedJumpFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <jumpCyclicFvPatchField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						Class fixedJumpFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class fixedJumpFvPatchField
		:
		public jumpCyclicFvPatchField<Type>
	{

	protected:

		// Protected data

			//- "jump" field
		Field<Type> jump_;


	public:

		//- Runtime type information
		TypeName("fixedJump");

		// Constructors

			//- Construct from patch and internal field
		fixedJumpFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		fixedJumpFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given fixedJumpFvPatchField onto a
		//  new patch
		fixedJumpFvPatchField
		(
			const fixedJumpFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		fixedJumpFvPatchField
		(
			const fixedJumpFvPatchField<Type>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type>> clone() const
		{
			return tmp<fvPatchField<Type>>
				(
					new fixedJumpFvPatchField<Type>(*this)
					);
		}

		//- Copy constructor setting internal field reference
		fixedJumpFvPatchField
		(
			const fixedJumpFvPatchField<Type>&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual tmp<fvPatchField<Type>> clone
		(
			const DimensionedField<Type, volMesh>& iF
		) const
		{
			return tmp<fvPatchField<Type>>
				(
					new fixedJumpFvPatchField<Type>(*this, iF)
					);
		}


		// Member Functions

			// Access

				//- Return the "jump" across the patch
		virtual tmp<Field<Type>> jump() const;


		// Mapping functions

			//- Map (and resize as needed) from self given a mapping object
			//  Used to update fields following mesh topology change
		virtual void autoMap(const fvPatchFieldMapper&);

		//- Reverse map the given fvPatchField onto this fvPatchField
		//  Used to reconstruct fields
		virtual void rmap(const fvPatchField<Type>&, const labelList&);


		//- Write
		virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <fixedJumpFvPatchFieldI.hxx>

//#ifdef NoRepository
//#include <fixedJumpFvPatchField.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_fixedJumpFvPatchField_Header
