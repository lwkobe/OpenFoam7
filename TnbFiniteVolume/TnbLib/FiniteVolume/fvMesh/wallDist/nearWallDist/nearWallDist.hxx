#pragma once
#ifndef _nearWallDist_Header
#define _nearWallDist_Header

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
	tnbLib::nearWallDist

Description
	Distance calculation for cells with face on a wall.
	Searches pointNeighbours to find closest.

SourceFiles
	nearWallDist.C

\*---------------------------------------------------------------------------*/

#include <volFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class fvMesh;

	/*---------------------------------------------------------------------------*\
							Class nearWallDist Declaration
	\*---------------------------------------------------------------------------*/

	class nearWallDist
		:
		public volScalarField::Boundary
	{
		// Private Data

			//- Reference to mesh
		const fvMesh& mesh_;


		// Private Member Functions

		//- Do all calculations
		FoamFiniteVolume_EXPORT void calculate();


	public:

		// Constructors

		//- Construct from components
		FoamFiniteVolume_EXPORT nearWallDist(const fvMesh& mesh);

		//- Disallow default bitwise copy construction
		FoamFiniteVolume_EXPORT nearWallDist(const nearWallDist&) = delete;


		//- Destructor
		FoamFiniteVolume_EXPORT virtual ~nearWallDist();


		// Member Functions

		const volScalarField::Boundary& y() const
		{
			return *this;
		}

		//- Correct for mesh geom/topo changes
		FoamFiniteVolume_EXPORT virtual void correct();


		// Member Operators

		//- Disallow default bitwise assignment
		FoamFiniteVolume_EXPORT void operator=(const nearWallDist&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_nearWallDist_Header
