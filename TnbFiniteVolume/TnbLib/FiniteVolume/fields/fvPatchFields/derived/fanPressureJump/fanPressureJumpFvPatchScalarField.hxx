#pragma once
#ifndef _fanPressureJumpFvPatchScalarField_Header
#define _fanPressureJumpFvPatchScalarField_Header

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
	tnbLib::fanPressureJumpFvPatchScalarField

Description
	This boundary condition provides a pressure jump condition, using
	the \c cyclic condition as a base.

	The jump is specified as a \c Function1 type, to enable the use of, e.g.
	constant, polynomial, table values. This boundary condition can operate
	in two modes - standard and backward compatibility.
	In standard mode, jump is specified as a function of total volumetric
	flow rate through the patch. In backward compatibility mode, the boundary
	conditions serves as a direct replacement for the fanFvPatchField, where
	jump is defined as a function of local velocity.

Usage
	\table
		Property     | Description             | Required    | Default value
		patchType    | underlying patch type should be \c cyclic| yes |
		fanCurve     | fan curve, e.g. \c csvFile | yes      |
		jumpTable    | jump data (backward compatibility mode) | no       |
		reverse      | reverse jump direction  | no | false
		phi          | flux field name         | no          | phi
		rho          | density field name      | no          | rho
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            fan;
		patchType       cyclic;
		fanCurve       csvFile;
		csvFileCoeffs
		{
			hasHeaderLine   1;
			refColumn       0;
			componentColumns 1(1);
			separator       ",";
			file           "$FOAM_CASE/constant/pressureVsQ";
		}
		value           uniform 0;
	}
	\endverbatim

	The above example shows the use of a comma separated (CSV) file to specify
	the jump condition.

Note
	 The underlying \c patchType should be set to \c cyclic

See also
	tnbLib::Function1Types

SourceFiles
	fanPressureJumpFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#include <fixedJumpFvPatchFields.hxx>
#include <Function1.hxx>

#include <Switch.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
				  Class fanPressureJumpFvPatchScalarField Declaration
	\*---------------------------------------------------------------------------*/

	class fanPressureJumpFvPatchScalarField
		:
		public fixedJumpFvPatchScalarField
	{
		// Private Data

			//- Fan curve
		autoPtr<Function1<scalar>> fanCurve_;

		//- Jump table (backward compatibility)
		autoPtr<Function1<scalar>> jumpTable_;

		//- Set true to reverse jump direction
		Switch reverse_;

		//- Name of the flux transporting the field
		word phiName_;

		//- Name of the density field used to normalise the mass flux
		//  if necessary
		word rhoName_;


		// Private Member Functions

			//- Calculate the fan pressure jump
		FoamFiniteVolume_EXPORT void calcFanJump();


	public:

		//- Runtime type information
		//TypeName("fanPressureJump");
		static const char* typeName_() { return "fanPressureJump"; }
		static FoamFiniteVolume_EXPORT const ::tnbLib::word typeName;
		static FoamFiniteVolume_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		FoamFiniteVolume_EXPORT fanPressureJumpFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamFiniteVolume_EXPORT fanPressureJumpFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patch onto a new patch
		FoamFiniteVolume_EXPORT fanPressureJumpFvPatchScalarField
		(
			const fanPressureJumpFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		FoamFiniteVolume_EXPORT fanPressureJumpFvPatchScalarField
		(
			const fanPressureJumpFvPatchScalarField&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchScalarField> clone() const
		{
			return tmp<fvPatchScalarField>
				(
					new fanPressureJumpFvPatchScalarField(*this)
					);
		}

		//- Copy constructor setting internal field reference
		FoamFiniteVolume_EXPORT fanPressureJumpFvPatchScalarField
		(
			const fanPressureJumpFvPatchScalarField&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual tmp<fvPatchField<scalar>> clone
		(
			const DimensionedField<scalar, volMesh>& iF
		) const
		{
			return tmp<fvPatchField<scalar>>
				(
					new fanPressureJumpFvPatchScalarField(*this, iF)
					);
		}


		// Member Functions

			//- Update the coefficients associated with the patch field
		FoamFiniteVolume_EXPORT virtual void updateCoeffs();

		//- Write
		FoamFiniteVolume_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_fanPressureJumpFvPatchScalarField_Header
