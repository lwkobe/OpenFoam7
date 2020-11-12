#pragma once
#ifndef _standardPhaseChange_Header
#define _standardPhaseChange_Header

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
	tnbLib::regionModels::surfaceFilmModels::standardPhaseChange

Description
	Standard phase change model with modification for boiling

SourceFiles
	standardPhaseChange.C

\*---------------------------------------------------------------------------*/

#include <phaseChangeModel.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace regionModels
	{
		namespace surfaceFilmModels
		{

			/*---------------------------------------------------------------------------*\
								Class standardPhaseChange Declaration
			\*---------------------------------------------------------------------------*/

			class standardPhaseChange
				:
				public phaseChangeModel
			{
			protected:

				// Protected data

					//- Minimum film height for model to be active
				const scalar deltaMin_;

				//- Length scale / [m]
				const scalar L_;

				//- Boiling temperature factor / []
				//  Used to set max limit on temperature to Tb*TbFactor
				const scalar TbFactor_;

				//- Switch to treat YInf as zero
				Switch YInfZero_;


				// Protected member functions

					//- Return Sherwood number as a function of Reynolds and Schmidt numbers
				FoamLagrangian_EXPORT scalar Sh(const scalar Re, const scalar Sc) const;

				template<class YInfType>
				void correctModel
				(
					const scalar dt,
					scalarField& availableMass,
					scalarField& dMass,
					scalarField& dEnergy,
					YInfType YInf
				);


			public:

				//- Runtime type information
				//TypeName("standardPhaseChange");
				static const char* typeName_() { return "standardPhaseChange"; }
				static FoamLagrangian_EXPORT const ::tnbLib::word typeName;
				static FoamLagrangian_EXPORT int debug;
				virtual const word& type() const { return typeName; };


				// Constructors

					//- Construct from surface film model
				FoamLagrangian_EXPORT standardPhaseChange
				(
					surfaceFilmRegionModel& film,
					const dictionary& dict
				);

				//- Disallow default bitwise copy construction
				FoamLagrangian_EXPORT standardPhaseChange(const standardPhaseChange&) = delete;


				//- Destructor
				FoamLagrangian_EXPORT virtual ~standardPhaseChange();


				// Member Functions

					// Evolution

						//- Correct
				FoamLagrangian_EXPORT virtual void correctModel
				(
					const scalar dt,
					scalarField& availableMass,
					scalarField& dMass,
					scalarField& dEnergy
				);


				// Member Operators

					//- Disallow default bitwise assignment
				FoamLagrangian_EXPORT void operator=(const standardPhaseChange&) = delete;
			};


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace surfaceFilmModels
	} // End namespace regionModels
} // End namespace tnbLib

#include <standardPhaseChangeI.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_standardPhaseChange_Header
