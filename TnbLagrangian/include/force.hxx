#pragma once
#ifndef _force_Header
#define _force_Header

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
	tnbLib::regionModels::surfaceFilmModels::force

Description
	Base class for film (stress-based) force models

SourceFiles
	force.C
	forceNew.C

\*---------------------------------------------------------------------------*/

#include <filmSubModelBase.hxx>
#include <runTimeSelectionTables.hxx>
#include <fvMatrices.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace regionModels
	{
		namespace surfaceFilmModels
		{

			/*---------------------------------------------------------------------------*\
									  Class force Declaration
			\*---------------------------------------------------------------------------*/

			class force
				:
				public filmSubModelBase
			{
			public:

				//- Runtime type information
				TypeName("force");


				// Declare runtime constructor selection table

				declareRunTimeSelectionTable
				(
					autoPtr,
					force,
					dictionary,
					(
						surfaceFilmRegionModel& film,
						const dictionary& dict
						),
						(film, dict)
				);

				// Constructors

					//- Construct null
				force(surfaceFilmRegionModel& film);

				//- Construct from type name, dictionary and surface film model
				force
				(
					const word& modelType,
					surfaceFilmRegionModel& film,
					const dictionary& dict
				);

				//- Disallow default bitwise copy construction
				force(const force&) = delete;


				// Selectors

					//- Return a reference to the selected force model
				static autoPtr<force> New
				(
					surfaceFilmRegionModel& film,
					const dictionary& dict,
					const word& modelType
				);


				//- Destructor
				virtual ~force();


				// Member Functions

					// Evolution

						//- Correct
				virtual tmp<fvVectorMatrix> correct(volVectorField& U) = 0;


				// Member Operators

					//- Disallow default bitwise assignment
				void operator=(const force&) = delete;
			};


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace surfaceFilmModels
	} // End namespace regionModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif