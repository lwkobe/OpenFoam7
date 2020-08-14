#pragma once
#ifndef _energyJumpAMIFvPatchScalarField_Header
#define _energyJumpAMIFvPatchScalarField_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2012-2019 OpenFOAM Foundation
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
	tnbLib::energyJumpAMIFvPatchScalarField

Description
	This boundary condition provides an energy jump condition across a pair
	of coupled patches with an arbitrary mesh interface (AMI).  It is not
	applied directly, but is employed on-the-fly when converting temperature
	boundary conditions into energy.

See also
	tnbLib::fixedJumpAMIFvPatchField

SourceFiles
	energyJumpAMIFvPatchScalarField.C

\*---------------------------------------------------------------------------*/


#include <fixedJumpAMIFvPatchField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
				  Class energyJumpAMIFvPatchScalarField Declaration
	\*---------------------------------------------------------------------------*/

	class energyJumpAMIFvPatchScalarField
		:
		public fixedJumpAMIFvPatchField<scalar>
	{

	public:

		//- Runtime type information
		TypeName("energyJumpAMI");

		// Constructors

			//- Construct from patch and internal field
		energyJumpAMIFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		energyJumpAMIFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given energyJumpAMIFvPatchScalarField onto a
		//  new patch
		energyJumpAMIFvPatchScalarField
		(
			const energyJumpAMIFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		energyJumpAMIFvPatchScalarField
		(
			const energyJumpAMIFvPatchScalarField&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<scalar>> clone() const
		{
			return tmp<fvPatchField<scalar>>
				(
					new energyJumpAMIFvPatchScalarField(*this)
					);
		}

		//- Copy constructor setting internal field reference
		energyJumpAMIFvPatchScalarField
		(
			const energyJumpAMIFvPatchScalarField&,
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
					new energyJumpAMIFvPatchScalarField(*this, iF)
					);
		}


		// Member Functions

			// Evaluation functions

				//- Update the coefficients
		virtual void updateCoeffs();


		//- Write
		virtual void write(Ostream&) const;
	};

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_energyJumpAMIFvPatchScalarField_Header