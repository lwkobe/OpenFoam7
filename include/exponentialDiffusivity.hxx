#pragma once
#ifndef _exponentialDiffusivity_Header
#define _exponentialDiffusivity_Header

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
	tnbLib::exponentialDiffusivity

Description
	Mesh motion diffusivity maipulator which returns the exp(-alpha/D)
	of the given diffusivity D.

SourceFiles
	exponentialDiffusivity.C

\*---------------------------------------------------------------------------*/

#include <motionDiffusivity.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class exponentialDiffusivity Declaration
	\*---------------------------------------------------------------------------*/

	class exponentialDiffusivity
		:
		public motionDiffusivity
	{
		// Private Data

		scalar alpha_;
		autoPtr<motionDiffusivity> basicDiffusivityPtr_;


	public:

		//- Runtime type information
		TypeName("exponential");


		// Constructors

			//- Construct for the given fvMesh and data Istream
		exponentialDiffusivity(const fvMesh& mesh, Istream& mdData);

		//- Disallow default bitwise copy construction
		exponentialDiffusivity(const exponentialDiffusivity&) = delete;


		//- Destructor
		virtual ~exponentialDiffusivity();


		// Member Functions

			//- Correct the motion diffusivity
		virtual void correct();


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const exponentialDiffusivity&) = delete;

		//- Return diffusivity field
		virtual tmp<surfaceScalarField> operator()() const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_exponentialDiffusivity_Header
