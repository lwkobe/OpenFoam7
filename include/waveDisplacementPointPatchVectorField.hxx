#pragma once
#ifndef _waveDisplacementPointPatchVectorField_Header
#define _waveDisplacementPointPatchVectorField_Header

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
	tnbLib::waveDisplacementPointPatchVectorField

Description
	Specified surface wave moving boundary.

	The motion is a cosine wave with specified amplitude, frequency, wave number
	and optional leading and trailing spacial ramps and temporal ramp
	to allow the motion to be applied to an initially flat surface.

Usage
	\table
		Property   | Description                    | Required | Default value
		amplitude  | Amplitude of the cosine wave   | yes      |
		omega      | Frequency of the cosine wave   | yes      |
		waveNumber | Wave number of the cosine wave | no       | (0 0 0)
		startRamp  | Leading ramp                   | no       | 1
		endRamp    | Trailing ramp                  | no       | 1
		timeRamp   | Temporal ramp                  | no       | 1
	\endtable

	Example of the boundary condition specification:
	\verbatim
	<patchName>
	{
		type            waveDisplacement;
		value           uniform (0 0 0);
		amplitude       (0 0.05 0);
		omega           2;
		waveNumber      (5 0 0);
		startRamp
		{
			type            linearRamp;
			duration        1;
			start           4;
		}
		endRamp
		{
			type            linearRamp;
			duration        1;
			start           4;
		}
		timeRamp
		{
			type            linearRamp;
			duration        1;
		}
	}
	\endverbatim

SourceFiles
	waveDisplacementPointPatchVectorField.C

\*---------------------------------------------------------------------------*/

#include <fixedValuePointPatchField.hxx>
#include <Function1.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
			Class waveDisplacementPointPatchVectorField Declaration
	\*---------------------------------------------------------------------------*/

	class waveDisplacementPointPatchVectorField
		:
		public fixedValuePointPatchField<vector>
	{
		// Private Data

		vector amplitude_;
		scalar omega_;
		vector waveNumber_;

		//- Optional leading ramp for the wave
		autoPtr<Function1<scalar>> startRamp_;

		//- Optional trailing ramp for the wave
		autoPtr<Function1<scalar>> endRamp_;

		//- Optional temporal ramp for the wave
		autoPtr<Function1<scalar>> timeRamp_;


	public:

		//- Runtime type information
		//TypeName("waveDisplacement");
		static const char* typeName_() { return "waveDisplacement"; }
		static FoamFvMotionSolver_EXPORT const ::tnbLib::word typeName;
		static FoamFvMotionSolver_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		FoamFvMotionSolver_EXPORT waveDisplacementPointPatchVectorField
		(
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamFvMotionSolver_EXPORT waveDisplacementPointPatchVectorField
		(
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patchField<vector> onto a new patch
		FoamFvMotionSolver_EXPORT waveDisplacementPointPatchVectorField
		(
			const waveDisplacementPointPatchVectorField&,
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&,
			const pointPatchFieldMapper&
		);

		//- Construct and return a clone
		virtual autoPtr<pointPatchField<vector>> clone() const
		{
			return autoPtr<pointPatchField<vector>>
				(
					new waveDisplacementPointPatchVectorField
					(
						*this
					)
					);
		}

		//- Construct as copy setting internal field reference
		FoamFvMotionSolver_EXPORT waveDisplacementPointPatchVectorField
		(
			const waveDisplacementPointPatchVectorField&,
			const DimensionedField<vector, pointMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual autoPtr<pointPatchField<vector>> clone
		(
			const DimensionedField<vector, pointMesh>& iF
		) const
		{
			return autoPtr<pointPatchField<vector>>
				(
					new waveDisplacementPointPatchVectorField
					(
						*this,
						iF
					)
					);
		}


		// Member Functions

			// Evaluation functions

				//- Update the coefficients associated with the patch field
		FoamFvMotionSolver_EXPORT virtual void updateCoeffs();


		//- Write
		FoamFvMotionSolver_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_waveDisplacementPointPatchVectorField_Header
