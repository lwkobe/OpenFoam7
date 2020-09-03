#pragma once
#ifndef _inverseVolumeDiffusivity_Header
#define _inverseVolumeDiffusivity_Header

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
	tnbLib::inverseVolumeDiffusivity

Description
	Inverse cell-volume motion diffusivity.

SourceFiles
	inverseVolumeDiffusivity.C

\*---------------------------------------------------------------------------*/

#include <motionDiffusivity.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class inverseVolumeDiffusivity Declaration
	\*---------------------------------------------------------------------------*/

	class inverseVolumeDiffusivity
		:
		public motionDiffusivity
	{
	public:

		//- Runtime type information
		TypeName("inverseVolume");


		// Constructors

			//- Construct for the given fvMesh and data Istream
		inverseVolumeDiffusivity(const fvMesh& mesh, Istream& mdData);

		//- Disallow default bitwise copy construction
		inverseVolumeDiffusivity(const inverseVolumeDiffusivity&) = delete;


		//- Destructor
		virtual ~inverseVolumeDiffusivity();


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const inverseVolumeDiffusivity&) = delete;

		//- Return diffusivity field
		virtual tmp<surfaceScalarField> operator()() const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_inverseVolumeDiffusivity_Header