#pragma once
#ifndef _variableHeightFlowRateFvPatchField_Header
#define _variableHeightFlowRateFvPatchField_Header

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
	tnbLib::variableHeightFlowRateFvPatchScalarField

Description
	This boundary condition provides a phase fraction condition based on the
	local flow conditions, whereby the values are constrained to lay between
	user-specified upper and lower bounds.  The behaviour is described by:

	if alpha > upperBound:
	- apply a fixed value condition, with a uniform level of the upper bound

	if lower bound <= alpha <= upper bound:
	- apply a  zero-gradient condition

	if alpha < lowerBound:
	- apply a fixed value condition, with a uniform level of the lower bound

Usage
	\table
		Property     | Description             | Required    | Default value
		phi          | flux field name         | no          | phi
		lowerBound   | lower bound for clipping | yes        |
		upperBound   | upper bound for clipping | yes        |
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            variableHeightFlowRate;
		lowerBound      0.0;
		upperBound      0.9;
		value           uniform 0;
	}
	\endverbatim

SourceFiles
	variableHeightFlowRateFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#include <mixedFvPatchFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
			  Class variableHeightFlowRateFvPatchScalarField Declaration
	\*---------------------------------------------------------------------------*/

	class variableHeightFlowRateFvPatchScalarField
		:
		public mixedFvPatchScalarField
	{

	protected:

		// Protected data

			//- Name of flux field
		word phiName_;

		//- Lower bound for alpha1
		scalar lowerBound_;

		//- Upper bound for alpha1
		scalar upperBound_;


	public:

		//- Runtime scalar information
		//TypeName("variableHeightFlowRate");
		static const char* typeName_() { return "variableHeightFlowRate"; }
		static FoamFiniteVolume_EXPORT const ::tnbLib::word typeName;
		static FoamFiniteVolume_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		FoamFiniteVolume_EXPORT variableHeightFlowRateFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamFiniteVolume_EXPORT variableHeightFlowRateFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given
		//  variableHeightFlowRateFvPatchScalarField onto a new patch
		FoamFiniteVolume_EXPORT variableHeightFlowRateFvPatchScalarField
		(
			const variableHeightFlowRateFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Copy constructor
		FoamFiniteVolume_EXPORT variableHeightFlowRateFvPatchScalarField
		(
			const variableHeightFlowRateFvPatchScalarField&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<scalar>> clone() const
		{
			return tmp<fvPatchField<scalar>>
				(
					new variableHeightFlowRateFvPatchScalarField(*this)
					);
		}

		//- Copy constructor setting internal field reference
		FoamFiniteVolume_EXPORT variableHeightFlowRateFvPatchScalarField
		(
			const variableHeightFlowRateFvPatchScalarField&,
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
					new variableHeightFlowRateFvPatchScalarField(*this, iF)
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

#endif // !_variableHeightFlowRateFvPatchField_Header
