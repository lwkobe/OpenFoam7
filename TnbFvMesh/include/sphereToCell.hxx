#pragma once
#ifndef _sphereToCell_Header
#define _sphereToCell_Header

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
	tnbLib::sphereToCell

Description
	A topoSetSource to select cells based on cell centres inside sphere.

SourceFiles
	sphereToCell.C

\*---------------------------------------------------------------------------*/

#include <topoSetSource.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class sphereToCell Declaration
	\*---------------------------------------------------------------------------*/

	class sphereToCell
		:
		public topoSetSource
	{

		// Private Data

			//- Add usage string
		static addToUsageTable usage_;

		//- Centre
		vector centre_;

		//- Radius
		scalar radius_;


		// Private Member Functions

		void combine(topoSet& set, const bool add) const;


	public:

		//- Runtime type information
		TypeName("sphereToCell");


		// Constructors

			//- Construct from components
		sphereToCell
		(
			const polyMesh& mesh,
			const vector& centre,
			const scalar radius
		);

		//- Construct from dictionary
		sphereToCell
		(
			const polyMesh& mesh,
			const dictionary& dict
		);

		//- Construct from Istream
		sphereToCell
		(
			const polyMesh& mesh,
			Istream&
		);


		//- Destructor
		virtual ~sphereToCell();


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

#endif // !_sphereToCell_Header
