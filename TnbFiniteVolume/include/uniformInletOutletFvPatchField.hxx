#pragma once
#ifndef _uniformInletOutletFvPatchField_Header
#define _uniformInletOutletFvPatchField_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2013-2019 OpenFOAM Foundation
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
	tnbLib::uniformInletOutletFvPatchField

Description
	Variant of inletOutlet boundary condition with uniform inletValue.

Usage
	\table
		Property     | Description             | Required    | Default value
		phi          | flux field name         | no          | phi
		uniformInletValue   | inlet value for reverse flow | yes    |
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type                uniformInletOutlet;
		phi                 phi;
		uniformInletValue   0;
		value               uniform 0;
	}
	\endverbatim

	The mode of operation is determined by the sign of the flux across the
	patch faces.

Note
	Sign conventions:
	- positive flux (out of domain): apply zero-gradient condition
	- negative flux (into of domain): apply the user-specified fixed value

See also
	tnbLib::inletOutletFvPatchField
	tnbLib::Function1Types

SourceFiles
	uniformInletOutletFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <mixedFvPatchField.hxx>
#include <Function1.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					   Class uniformInletOutletFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class uniformInletOutletFvPatchField
		:
		public mixedFvPatchField<Type>
	{

	protected:

		// Protected data

			//- Name of flux field
		word phiName_;

		//- Value
		autoPtr<Function1<Type>> uniformInletValue_;


	public:

		//- Runtime type information
		TypeName("uniformInletOutlet");


		// Constructors

			//- Construct from patch and internal field
		uniformInletOutletFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		uniformInletOutletFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given uniformInletOutletFvPatchField
		//  onto a new patch
		uniformInletOutletFvPatchField
		(
			const uniformInletOutletFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		uniformInletOutletFvPatchField
		(
			const uniformInletOutletFvPatchField<Type>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type>> clone() const
		{
			return tmp<fvPatchField<Type>>
				(
					new uniformInletOutletFvPatchField<Type>(*this)
					);
		}

		//- Copy constructor setting internal field reference
		uniformInletOutletFvPatchField
		(
			const uniformInletOutletFvPatchField<Type>&,
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
					new uniformInletOutletFvPatchField<Type>(*this, iF)
					);
		}


		// Member Functions

			// Attributes

				//- Return true: this patch field is altered by assignment
		virtual bool assignable() const
		{
			return true;
		}


		// Mapping functions

			//- Map (and resize as needed) from self given a mapping object
			//  Used to update fields following mesh topology change
		virtual void autoMap(const fvPatchFieldMapper&);

		//- Reverse map the given fvPatchField onto this fvPatchField
		//  Used to reconstruct fields
		virtual void rmap(const fvPatchField<Type>&, const labelList&);


		//- Update the coefficients associated with the patch field
		virtual void updateCoeffs();

		//- Write
		virtual void write(Ostream&) const;


		// Member Operators

		virtual void operator=(const fvPatchField<Type>& pvf);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <uniformInletOutletFvPatchFieldI.hxx>

//#ifdef NoRepository
//#include <uniformInletOutletFvPatchField.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_uniformInletOutletFvPatchField_Header
