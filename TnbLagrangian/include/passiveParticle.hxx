#pragma once
#ifndef _passiveParticle_Header
#define _passiveParticle_Header

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
	tnbLib::passiveParticle

Description
	Copy of base particle

SourceFiles
	passiveParticle.H

\*---------------------------------------------------------------------------*/

#include <particle.hxx>
#include <IOstream.hxx>
#include <autoPtr.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						  Class passiveParticle Declaration
	\*---------------------------------------------------------------------------*/

	class passiveParticle
		:
		public particle
	{

	public:

		// Constructors

			//- Construct from components
		passiveParticle
		(
			const polyMesh& mesh,
			const barycentric& coordinates,
			const label celli,
			const label tetFacei,
			const label tetPti
		)
			:
			particle(mesh, coordinates, celli, tetFacei, tetPti)
		{}


		//- Construct from a position and a cell, searching for the rest of the
		//  required topology
		passiveParticle
		(
			const polyMesh& mesh,
			const vector& position,
			const label celli
		)
			:
			particle(mesh, position, celli)
		{}

		//- Construct from Istream
		passiveParticle
		(
			const polyMesh& mesh,
			Istream& is,
			bool readFields = true
		)
			:
			particle(mesh, is, readFields)
		{}

		//- Copy constructor
		passiveParticle(const passiveParticle& p)
			:
			particle(p)
		{}

		//- Construct and return a clone
		virtual autoPtr<particle> clone() const
		{
			return autoPtr<particle>(new passiveParticle(*this));
		}
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_passiveParticle_Header
