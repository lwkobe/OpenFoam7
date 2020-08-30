#pragma once
#ifndef _indexedParticle_Header
#define _indexedParticle_Header

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
	tnbLib::indexedParticle

Description
	Adds label index to base particle

SourceFiles
	indexedParticle.H

\*---------------------------------------------------------------------------*/

#include <particle.hxx>
#include <IOstream.hxx>
#include <autoPtr.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						  Class indexedParticle Declaration
	\*---------------------------------------------------------------------------*/

	class indexedParticle
		:
		public particle
	{
		// Private Data

		label index_;


	public:

		// Constructors

			//- Construct from components
		indexedParticle
		(
			const polyMesh& mesh,
			const barycentric& coordinates,
			const label celli,
			const label tetFacei,
			const label tetPti,
			const label index = 0
		)
			:
			particle(mesh, coordinates, celli, tetFacei, tetPti),
			index_(index)
		{}

		//- Construct from Istream
		indexedParticle
		(
			const polyMesh& mesh,
			Istream& is,
			bool readFields = true
		)
			:
			particle(mesh, is, readFields)
		{}

		//- Construct as a copy
		indexedParticle(const indexedParticle& p)
			:
			particle(p)
		{}

		//- Construct and return a clone
		virtual autoPtr<particle> clone() const
		{
			return autoPtr<particle>(new indexedParticle(*this));
		}


		// Member Functions

		label index() const
		{
			return index_;
		}

		label& index()
		{
			return index_;
		}
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_indexedParticle_Header
