#pragma once
#ifndef _pressureControl_Header
#define _pressureControl_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2017-2019 OpenFOAM Foundation
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
	tnbLib::pressureControl

Description
	Provides controls for the pressure reference is closed-volume simulations
	and a general method for limiting the pressure during the startup of
	steady-state simulations.

SourceFiles
	pressureControlI.H
	pressureControl.C

\*---------------------------------------------------------------------------*/

#include <dimensionedScalar.hxx>
#include <volFieldsFwd.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							 Class pressureControl Declaration
	\*---------------------------------------------------------------------------*/

	class pressureControl
	{
		// Private Data

			//- Optional cell in which the reference pressure is set
		label refCell_;

		//- Optional pressure reference level
		scalar refValue_;

		//- Pressure lower-limit
		dimensionedScalar pMax_;

		//- Pressure upper-limit
		dimensionedScalar pMin_;

		//- Pressure lower-limit
		bool limitMaxP_;

		//- Pressure upper-limit
		bool limitMinP_;


	public:

		// Constructors

			//- Construct from the simple/pimple sub-dictionary
		FoamFiniteVolume_EXPORT pressureControl
		(
			const volScalarField& p,
			const volScalarField& rho,
			const dictionary& dict,
			const bool pRefRequired = true
		);


		// Member Functions

			//- Return the cell in which the reference pressure is set
		inline label refCell() const;

		//- Return the pressure reference level
		inline scalar refValue() const;

		//- Limit the pressure if necessary and return true if so
		FoamFiniteVolume_EXPORT bool limit(volScalarField& p) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <pressureControlI.hxx>

#endif // !_pressureControl_Header
