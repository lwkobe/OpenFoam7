#pragma once
#ifndef _fixedMeanOutletInletFvPatchField_Header
#define _fixedMeanOutletInletFvPatchField_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2018-2019 OpenFOAM Foundation
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
	tnbLib::fixedMeanOutletInletFvPatchField

Description
	This boundary condition extrapolates field to the patch using the near-cell
	values and adjusts the distribution to match the specified, optionally
	time-varying, mean value.  This extrapolated field is applied as a
	fixedValue for outflow faces but zeroGradient is applied to inflow faces.

	This boundary condition can be applied to pressure when inletOutlet is
	applied to the velocity so that a zeroGradient condition is applied to the
	pressure at inflow faces where the velocity is specified to avoid an
	unphysical over-specification of the set of boundary conditions.

Usage
	\table
		Property     | Description             | Required    | Default value
		meanValue    | mean value Function1    | yes         |
		phi          | Flux field name         | no          | phi
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            fixedMeanOutletInlet;
		meanValue       1.0;
	}
	\endverbatim

See also
	tnbLib::fixedMeanFvPatchField
	tnbLib::outletInletFvPatchField
	tnbLib::Function1Types

SourceFiles
	fixedMeanOutletInletFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <outletInletFvPatchFields.hxx>
#include <Function1.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					  Class fixedMeanOutletInletFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class fixedMeanOutletInletFvPatchField
		:
		public outletInletFvPatchField<Type>
	{
		// Private Data

			//- MeanValue value the field is adjusted to maintain
		autoPtr<Function1<Type>> meanValue_;


	public:

		//- Runtime type information
		TypeName("fixedMeanOutletInlet");


		// Constructors

			//- Construct from patch and internal field
		fixedMeanOutletInletFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		fixedMeanOutletInletFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given fixedMeanOutletInletFvPatchField
		//  onto a new patch
		fixedMeanOutletInletFvPatchField
		(
			const fixedMeanOutletInletFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		fixedMeanOutletInletFvPatchField
		(
			const fixedMeanOutletInletFvPatchField<Type>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type>> clone() const
		{
			return tmp<fvPatchField<Type>>
				(
					new fixedMeanOutletInletFvPatchField<Type>(*this)
					);
		}

		//- Copy constructor setting internal field reference
		fixedMeanOutletInletFvPatchField
		(
			const fixedMeanOutletInletFvPatchField<Type>&,
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
					new fixedMeanOutletInletFvPatchField<Type>(*this, iF)
					);
		}


		// Member Functions

			//- Update the coefficients associated with the patch field
		virtual void updateCoeffs();

		//- Write
		virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <fixedMeanOutletInletFvPatchFieldI.hxx>

//#ifdef NoRepository
//#include <fixedMeanOutletInletFvPatchField.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_fixedMeanOutletInletFvPatchField_Header
