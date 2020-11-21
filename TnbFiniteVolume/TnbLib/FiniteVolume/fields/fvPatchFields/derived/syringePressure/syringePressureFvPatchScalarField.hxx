#pragma once
#ifndef _syringePressureFvPatchScalarField_Header
#define _syringePressureFvPatchScalarField_Header

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
	tnbLib::syringePressureFvPatchScalarField

Description
	This boundary condition provides a pressure condition, obtained from a
	zero-D model of the cylinder of a syringe.

	The syringe cylinder is defined by its initial volume, piston area and
	velocity profile specified by regions of constant acceleration, speed
	and deceleration.  The gas in the cylinder is described by its initial
	pressure and compressibility which is assumed constant, i.e. isothermal
	expansion/compression.

Usage
	\table
		Property     | Description             | Required    | Default value
		Ap           | syringe piston area [m^2] | yes        |
		Sp           | syringe piston speed [m/s] | yes      |
		VsI          | initial syringe volume [m^3] | yes     |
		tas          | start of piston acceleration [s] | yes |
		tae          | end of piston acceleration [s] | yes  |
		tds          | start of piston deceleration [s] | yes |
		tde          | end of piston deceleration [s] | yes  |
		psI          | initial syringe pressure [Pa] | yes   |
		psi          | gas compressibility [m2/s^2] | yes     |
		ams          | added (or removed) gas mass [kg] | yes |
	\endtable

	Example of the BC specification:
	\verbatim
	<patchName>
	{
		type            syringePressure;
		Ap              1.388e-6;
		Sp              0.01;
		VsI             1.388e-8;
		tas             0.001;
		tae             0.002;
		tds             0.005;
		tde             0.006;
		psI             1e5;
		psi             1e-5;
		ams             0;
		value           uniform 0;
	}
	\endverbatim

See also
	tnbLib::fixedValueFvPatchField

SourceFiles
	syringePressureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#include <fixedValueFvPatchFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
				 Class syringePressureFvPatchScalarField Declaration
	\*---------------------------------------------------------------------------*/

	class syringePressureFvPatchScalarField
		:
		public fixedValueFvPatchScalarField
	{
		// Private Data

			//- Syringe piston area
		scalar Ap_;

		//- Syringe piston speed
		scalar Sp_;

		//- Initial syringe volume
		scalar VsI_;

		//- Start of piston acceleration
		scalar tas_;

		//- End of piston acceleration
		scalar tae_;

		//- Start of piston deceleration
		scalar tds_;

		//- End of piston deceleration
		scalar tde_;

		//- Initial syringe pressure
		scalar psI_;

		//- Gas compressibility
		scalar psi_;

		//- Added gas mass
		scalar ams_;

		//- Added gas mass at previous time step
		scalar ams0_;

		//- Name of the flux transporting the field
		word phiName_;

		//- Current time index used to store ms0_
		label curTimeIndex_;


		// Private Functions

			//- Return the volume of the syringe at time t
		scalar Vs(const scalar t) const;


	public:

		//- Runtime type information
		/*TypeName("syringePressure");*/
		static const char* typeName_() { return "syringePressure"; }
		static FoamFiniteVolume_EXPORT const ::tnbLib::word typeName;
		static FoamFiniteVolume_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

		//- Construct from patch and internal field
		FoamFiniteVolume_EXPORT syringePressureFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamFiniteVolume_EXPORT syringePressureFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given syringePressureFvPatchScalarField
		//  onto a new patch
		FoamFiniteVolume_EXPORT syringePressureFvPatchScalarField
		(
			const syringePressureFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchScalarField> clone() const
		{
			return tmp<fvPatchScalarField>
				(
					new syringePressureFvPatchScalarField(*this)
					);
		}

		//- Copy constructor
		FoamFiniteVolume_EXPORT syringePressureFvPatchScalarField
		(
			const syringePressureFvPatchScalarField&
		);

		//- Copy constructor setting internal field reference
		FoamFiniteVolume_EXPORT syringePressureFvPatchScalarField
		(
			const syringePressureFvPatchScalarField&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual tmp<fvPatchScalarField> clone
		(
			const DimensionedField<scalar, volMesh>& iF
		) const
		{
			return tmp<fvPatchScalarField>
				(
					new syringePressureFvPatchScalarField(*this, iF)
					);
		}


		// Member Functions

		// Evaluation functions

		//- Update the coefficients associated with the patch field
		FoamFiniteVolume_EXPORT virtual void updateCoeffs();


		//- Write
		FoamFiniteVolume_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_syringePressureFvPatchScalarField_Header
