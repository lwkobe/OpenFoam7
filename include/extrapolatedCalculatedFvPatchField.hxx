#pragma once
#ifndef _extrapolatedCalculatedFvPatchField_Header
#define _extrapolatedCalculatedFvPatchField_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2016-2019 OpenFOAM Foundation
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
	tnbLib::extrapolatedCalculatedFvPatchField

Description
	This boundary condition applies a zero-gradient condition from the patch
	internal field onto the patch faces when \c evaluated but may also be
	assigned.  \c snGrad returns the patch gradient evaluated from the current
	internal and patch field values rather than returning zero.

Usage
	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            extrapolatedCalculated;
	}
	\endverbatim

SourceFiles
	extrapolatedCalculatedFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <calculatedFvPatchField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					   Class extrapolatedCalculatedFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class extrapolatedCalculatedFvPatchField
		:
		public calculatedFvPatchField<Type>
	{

	public:

		//- Runtime type information
		TypeName("extrapolatedCalculated");


		// Constructors

			//- Construct from patch and internal field
		extrapolatedCalculatedFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		extrapolatedCalculatedFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patchField<Type> onto a new patch
		extrapolatedCalculatedFvPatchField
		(
			const extrapolatedCalculatedFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		extrapolatedCalculatedFvPatchField
		(
			const extrapolatedCalculatedFvPatchField<Type>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type>> clone() const
		{
			return tmp<fvPatchField<Type>>
				(
					new extrapolatedCalculatedFvPatchField<Type>(*this)
					);
		}

		//- Copy constructor setting internal field reference
		extrapolatedCalculatedFvPatchField
		(
			const extrapolatedCalculatedFvPatchField<Type>&,
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
					new extrapolatedCalculatedFvPatchField<Type>(*this, iF)
					);
		}


		// Member Functions

			//- Evaluate the patch field
		virtual void evaluate
		(
			const Pstream::commsTypes commsType = Pstream::commsTypes::blocking
		);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <extrapolatedCalculatedFvPatchField.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_extrapolatedCalculatedFvPatchField_Header