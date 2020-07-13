#pragma once
#ifndef _faceZoneToCell_Header
#define _faceZoneToCell_Header

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
	tnbLib::faceZoneToCell

Description
	A topoSetSource to select cells based on side of faceZone.

SourceFiles
	faceZoneToCell.C

\*---------------------------------------------------------------------------*/

#include <topoSetSource.hxx>
#include <wordRe.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class faceZoneToCell Declaration
	\*---------------------------------------------------------------------------*/

	class faceZoneToCell
		:
		public topoSetSource
	{
	public:
		//- Enumeration defining the valid options
		enum faceAction
		{
			MASTER,
			SLAVE
		};

	private:

		// Private Data

		static const NamedEnum<faceAction, 2> faceActionNames_;

		//- Add usage string
		static addToUsageTable usage_;

		//- Name/regular expression of faceZone
		wordRe zoneName_;

		//- Option
		faceAction option_;


		// Private Member Functions

		void combine(topoSet& set, const bool add) const;


	public:

		//- Runtime type information
		TypeName("faceZoneToCell");

		// Constructors

			//- Construct from components
		faceZoneToCell
		(
			const polyMesh& mesh,
			const word& zoneName,
			const faceAction option
		);

		//- Construct from dictionary
		faceZoneToCell
		(
			const polyMesh& mesh,
			const dictionary& dict
		);

		//- Construct from Istream
		faceZoneToCell
		(
			const polyMesh& mesh,
			Istream&
		);


		//- Destructor
		virtual ~faceZoneToCell();


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

#endif // !_faceZoneToCell_Header
