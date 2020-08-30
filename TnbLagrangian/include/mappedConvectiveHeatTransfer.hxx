#pragma once
#ifndef _mappedConvectiveHeatTransfer_Header
#define _mappedConvectiveHeatTransfer_Header

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
	tnbLib::regionModels::surfaceFilmModels::mappedConvectiveHeatTransfer

Description
	Convective heat transfer model based on a re-working of a Nusselt number
	correlation

SourceFiles
	mappedConvectiveHeatTransfer.C

\*---------------------------------------------------------------------------*/

#include <heatTransferModel.hxx>
#include <volFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace regionModels
	{
		namespace surfaceFilmModels
		{

			/*---------------------------------------------------------------------------*\
							Class mappedConvectiveHeatTransfer Declaration
			\*---------------------------------------------------------------------------*/

			class mappedConvectiveHeatTransfer
				:
				public heatTransferModel
			{
				// Private Data

					//- Heat transfer coefficient - primary region [W/m^2/K]
				volScalarField htcConvPrimary_;

				//- Heat transfer coefficient - film region [W/m^2/K]
				//  Assumes that the primary regtion to film region boundaries are
				//  described as mappedPushed types
				volScalarField htcConvFilm_;


			public:

				//- Runtime type information
				TypeName("mappedConvectiveHeatTransfer");


				// Constructors

					//- Construct from surface film model and dictionary
				mappedConvectiveHeatTransfer
				(
					surfaceFilmRegionModel& film,
					const dictionary& dict
				);

				//- Disallow default bitwise copy construction
				mappedConvectiveHeatTransfer
				(
					const mappedConvectiveHeatTransfer&
				) = delete;


				//- Destructor
				virtual ~mappedConvectiveHeatTransfer();


				// Member Functions

					// Evolution

						//- Correct
				virtual void correct();

				//- Return the heat transfer coefficient [W/m^2/K]
				virtual tmp<volScalarField> h() const;


				// Member Operators

					//- Disallow default bitwise assignment
				void operator=(const mappedConvectiveHeatTransfer&) = delete;
			};


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace surfaceFilmModels
	} // End namespace regionModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_mappedConvectiveHeatTransfer_Header
