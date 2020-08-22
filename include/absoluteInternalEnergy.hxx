#pragma once
#ifndef _absoluteInternalEnergy_Header
#define _absoluteInternalEnergy_Header

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
	tnbLib::absoluteInternalEnergy

Description
	Thermodynamics mapping class to expose the absolute internal energy
	functions.

\*---------------------------------------------------------------------------*/

#include <scalar.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class absoluteInternalEnergy Declaration
	\*---------------------------------------------------------------------------*/

	template<class Thermo>
	class absoluteInternalEnergy
	{

	public:

		// Constructors

			//- Construct
		absoluteInternalEnergy()
		{}


		// Member Functions

			//- Return the instantiated type name
		static word typeName()
		{
			return "absoluteInternalEnergy";
		}

		// Fundamental properties

		static word energyName()
		{
			return "ea";
		}

		// Heat capacity at constant volume [J/kg/K]
		scalar Cpv
		(
			const Thermo& thermo,
			const scalar p,
			const scalar T
		) const
		{
			return thermo.Cv(p, T);
		}

		//- Cp/Cv []
		scalar CpByCpv
		(
			const Thermo& thermo,
			const scalar p,
			const scalar T
		) const
		{
			return thermo.gamma(p, T);
		}

		// Absolute internal energy [J/kg]
		scalar HE
		(
			const Thermo& thermo,
			const scalar p,
			const scalar T
		) const
		{
			return thermo.Ea(p, T);
		}

		//- Temperature from absolute internal energy
		//  given an initial temperature T0
		scalar THE
		(
			const Thermo& thermo,
			const scalar e,
			const scalar p,
			const scalar T0
		) const
		{
			return thermo.TEa(e, p, T0);
		}
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_absoluteInternalEnergy_Header