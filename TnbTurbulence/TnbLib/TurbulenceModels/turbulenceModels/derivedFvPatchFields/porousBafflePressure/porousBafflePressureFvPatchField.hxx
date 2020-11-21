#pragma once
#ifndef _porousBafflePressureFvPatchField_Header
#define _porousBafflePressureFvPatchField_Header

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
	tnbLib::porousBafflePressureFvPatchField

Description
	This boundary condition provides a jump condition, using the \c cyclic
	condition as a base.

	The porous baffle introduces a pressure jump defined by:

		\f[
			\Delta p = -(D \mu U + 0.5 I \rho |U|^2 )L
		\f]

	where

	\vartable
		p      | pressure [Pa]
		\rho   | density [kg/m^3]
		\mu    | laminar viscosity [Pa s]
		D      | Darcy coefficient
		I      | inertial coefficient
		L      | porous media length in the flow direction
	\endvartable


Usage
	\table
		Property     | Description             | Required    | Default value
		patchType    | underlying patch type should be \c cyclic| yes |
		phi          | flux field name         | no          | phi
		rho          | density field name      | no          | rho
		D            | Darcy coefficient       | yes         |
		I            | inertial coefficient    | yes         |
		length       | porous media length in the flow direction | yes |
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            porousBafflePressure;
		patchType       cyclic;
		jump            uniform 0;
		D               0.001;
		I               1000000;
		length          0.1;
		value           uniform 0;
	}
	\endverbatim

Note
	 The underlying \c patchType should be set to \c cyclic

SourceFiles
	porousBafflePressureFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <fixedJumpFvPatchField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					Class porousBafflePressureFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	class porousBafflePressureFvPatchField
		:
		public fixedJumpFvPatchField<scalar>
	{
		// Private Data

			//- Name of flux field (default = phi)
		const word phiName_;

		//- Name of density field (default = rho)
		const word rhoName_;

		//- Darcy pressure loss coefficient
		scalar D_;

		//- Inertia pressure lost coefficient
		scalar I_;

		//- Porous media length
		scalar length_;


	public:

		//- Runtime type information
		//TypeName("porousBafflePressure");
		static const char* typeName_() { return "porousBafflePressure"; }
		static FoamTurbulence_EXPORT const ::tnbLib::word typeName;
		static FoamTurbulence_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		FoamTurbulence_EXPORT porousBafflePressureFvPatchField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamTurbulence_EXPORT porousBafflePressureFvPatchField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given porousBafflePressureFvPatchField
		// onto a new patch
		FoamTurbulence_EXPORT porousBafflePressureFvPatchField
		(
			const porousBafflePressureFvPatchField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		FoamTurbulence_EXPORT porousBafflePressureFvPatchField
		(
			const porousBafflePressureFvPatchField&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<scalar>> clone() const
		{
			return tmp<fvPatchField<scalar>>
				(
					new porousBafflePressureFvPatchField(*this)
					);
		}

		//- Copy constructor setting internal field reference
		FoamTurbulence_EXPORT porousBafflePressureFvPatchField
		(
			const porousBafflePressureFvPatchField&,
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
					new porousBafflePressureFvPatchField(*this, iF)
					);
		}


		// Member Functions


			// Evaluation functions

				//- Update the coefficients associated with the patch field
		FoamTurbulence_EXPORT virtual void updateCoeffs();


		//- Write
		FoamTurbulence_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_porousBafflePressureFvPatchField_Header
