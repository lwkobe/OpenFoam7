#pragma once
#ifndef _laminarTemplate_Header
#define _laminarTemplate_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2013-2019 OpenFOAM Foundation
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
	tnbLib::combustionModels::laminar

Description
	Laminar combustion model.

SourceFiles
	laminar.C

\*---------------------------------------------------------------------------*/

#include <ChemistryCombustionTemplate.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef FoamCombustionModels_EXPORT_DEFINE
#define FoamLaminarCombModel_EXPORT __declspec(dllexport)
#else
#ifdef FoamLaminarCombModel_EXPORT_DEFINE
#define FoamLaminarCombModel_EXPORT __declspec(dllexport)
#else
#define FoamLaminarCombModel_EXPORT __declspec(dllimport)
#endif
#endif

namespace tnbLib
{
	namespace combustionModels
	{

		/*---------------------------------------------------------------------------*\
									Class laminar Declaration
		\*---------------------------------------------------------------------------*/

		template<class ReactionThermo>
		class laminar
			:
			public ChemistryCombustion<ReactionThermo>
		{
			// Private Data

				//- Integrate reaction rate over the time-step
				//  using the selected ODE solver
			bool integrateReactionRate_;


		protected:

			// Protected Member Functions

				//- Return the chemical time scale
			tmp<volScalarField> tc() const;


		public:

			//- Runtime type information
			//TypeName("laminar");
			static const char* typeName_() { return "laminar"; }
			static FoamLaminarCombModel_EXPORT const ::tnbLib::word typeName;
			static FoamLaminarCombModel_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			laminar
			(
				const word& modelType,
				ReactionThermo& thermo,
				const compressibleTurbulenceModel& turb,
				const word& combustionProperties
			);

			//- Disallow default bitwise copy construction
			laminar(const laminar&);


			//- Destructor
			virtual ~laminar();


			// Member Functions

				//- Correct combustion rate
			virtual void correct();

			//- Fuel consumption rate matrix.
			virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

			//- Heat release rate [kg/m/s^3]
			virtual tmp<volScalarField> Qdot() const;

			//- Update properties from given dictionary
			virtual bool read();


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const laminar&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace combustionModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <laminarTemplateI.hxx>

//#ifdef NoRepository
//#include "laminarTemplate.cxx"
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_laminarTemplate_Header
