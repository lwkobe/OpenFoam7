#pragma once
#ifndef _noChemistryTabulation_Header
#define _noChemistryTabulation_Header

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
	tnbLib::chemistryTabulationMethods::none

Description

\*---------------------------------------------------------------------------*/

#include <chemistryTabulationMethod.hxx>

#ifdef FoamThermophysicalModels_EXPORT_DEFINE
#define FoamnoChemistryTabulationMethod_EXPORT __declspec(dllexport)
#else
#ifdef FoamnoChemistryTabulationMethod_EXPORT_DEFINE
#define FoamnoChemistryTabulationMethod_EXPORT __declspec(dllexport)
#else
#define FoamnoChemistryTabulationMethod_EXPORT __declspec(dllimport)
#endif
#endif

namespace tnbLib
{
	namespace chemistryTabulationMethods
	{

		/*---------------------------------------------------------------------------*\
								   Class none Declaration
		\*---------------------------------------------------------------------------*/

		template<class CompType, class ThermoType>
		class none
			:
			public chemistryTabulationMethod<CompType, ThermoType>
		{
		public:

			//- Runtime type information
			//TypeName("none");
			static const char* typeName_() { return "none"; }
			static FoamnoChemistryTabulationMethod_EXPORT const ::tnbLib::word typeName;
			static FoamnoChemistryTabulationMethod_EXPORT int debug;
			virtual const word& type() const { return typeName; };

			// Constructors

				//- Construct from dictionary
			none
			(
				const dictionary& chemistryProperties,
				TDACChemistryModel<CompType, ThermoType>& chemistry
			);

			//- Disallow default bitwise copy construction
			none(const none&) = delete;


			// Destructor
			virtual ~none();


			// Member Functions

				//- Return the size of the binary tree
			virtual label size()
			{
				NotImplemented;
				return 0;
			}

			virtual void writePerformance()
			{
				NotImplemented;
			}

			//- Find the closest stored leaf of phiQ and store the result in
			//  RphiQ or return false.
			virtual bool retrieve
			(
				const tnbLib::scalarField& phiq,
				scalarField& Rphiq
			)
			{
				NotImplemented;
				return false;
			}

			// Add information to the tabulation.This function can grow an
			// existing point or add a new leaf to the binary tree Input : phiq
			// the new composition to store Rphiq the mapping of the new
			// composition point
			virtual label add
			(
				const scalarField& phiq,
				const scalarField& Rphiq,
				const scalar rho,
				const scalar deltaT
			)
			{
				NotImplemented;
				return false;
			}

			virtual bool update()
			{
				NotImplemented;
				return false;
			}
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace chemistryTabulationMethods
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <noChemistryTabulationI.hxx>

//#ifdef NoRepository
//#include <noChemistryTabulation.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_noChemistryTabulation_Header
