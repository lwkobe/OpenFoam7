#pragma once
#ifndef _zoneCombustion_Header
#define _zoneCombustion_Header

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
	tnbLib::combustionModels::zoneCombustion

Description
	Zone-filtered combustion model.

	Enable the reactions within the specified list of cell-zones and set
	to zero elsewhere.

SourceFiles
	zoneCombustion.C

\*---------------------------------------------------------------------------*/

#include <CombustionModelTemplate.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef FoamCombustionModels_EXPORT_DEFINE
#define FoamZoneCombustion_EXPORT __declspec(dllexport)
#else
#ifdef FoamZoneCombustion_EXPORT_DEFINE
#define FoamZoneCombustion_EXPORT __declspec(dllexport)
#else
#define FoamZoneCombustion_EXPORT __declspec(dllimport)
#endif
#endif

namespace tnbLib
{
	namespace combustionModels
	{

		/*---------------------------------------------------------------------------*\
									Class zoneCombustion Declaration
		\*---------------------------------------------------------------------------*/

		template<class ReactionThermo>
		class zoneCombustion
			:
			public CombustionModel<ReactionThermo>
		{
			// Private Data

				//- The combustion model to be zone-filtered
			autoPtr<CombustionModel<ReactionThermo>> combustionModelPtr_;

			//- List of zone names in which the reactions are active
			wordList zoneNames_;


			// Private Member Functions

				//- Filter the reaction-rate matrix on the cellZones
			tmp<fvScalarMatrix> filter(const tmp<fvScalarMatrix>& tR) const;

			//- Filter the given field on the cellZones
			tmp<volScalarField> filter(const tmp<volScalarField>& tS) const;


		public:

			//- Runtime type information
			//TypeName("zoneCombustion");
			static const char* typeName_() { return "zoneCombustion"; }
			static FoamZoneCombustion_EXPORT const ::tnbLib::word typeName;
			static FoamZoneCombustion_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			zoneCombustion
			(
				const word& modelType,
				ReactionThermo& thermo,
				const compressibleTurbulenceModel& turb,
				const word& combustionProperties
			);

			//- Disallow default bitwise copy construction
			zoneCombustion(const zoneCombustion&);


			//- Destructor
			virtual ~zoneCombustion();


			// Member Functions

				//- Return access to the thermo package
			virtual ReactionThermo& thermo();

			//- Return const access to the thermo package
			virtual const ReactionThermo& thermo() const;

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
			void operator=(const zoneCombustion&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace combustionModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <zoneCombustionI.hxx>

//#ifdef NoRepository
//#include "zoneCombustion.cxx"
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_zoneCombustion_Header
