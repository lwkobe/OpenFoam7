#pragma once
#ifndef _nearestToCell_Header
#define _nearestToCell_Header

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
	tnbLib::nearestToCell

Description
	A topoSetSource to select cells nearest to points.

SourceFiles
	nearestToCell.C

\*---------------------------------------------------------------------------*/

#include <topoSetSource.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class nearestToCell Declaration
	\*---------------------------------------------------------------------------*/

	class nearestToCell
		:
		public topoSetSource
	{

		// Private Data

			//- Add usage string
		static addToUsageTable usage_;

		//- Points to select nearest to
		pointField points_;


		// Private Member Functions

		void combine(topoSet& set, const bool add) const;


	public:

		//- Runtime type information
		TypeName("nearestToCell");

		// Constructors

			//- Construct from components
		nearestToCell
		(
			const polyMesh& mesh,
			const pointField& points
		);

		//- Construct from dictionary
		nearestToCell
		(
			const polyMesh& mesh,
			const dictionary& dict
		);

		//- Construct from Istream
		nearestToCell
		(
			const polyMesh& mesh,
			Istream&
		);


		//- Destructor
		virtual ~nearestToCell();


		// Member Functions

		virtual sourceType setType() const
		{
			return CELLSETSOURCE;
		}

		virtual void applyToSet
		(
			const topoSetSource::setAction action,
			topoSet&
		) const;

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_nearestToCell_Header
