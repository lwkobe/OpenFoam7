#pragma once
#ifndef _faceToCell_Header
#define _faceToCell_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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
	tnbLib::faceToCell

Description
	A topoSetSource to select cells based on usage in faces.

SourceFiles
	faceToCell.C

\*---------------------------------------------------------------------------*/

#include <topoSetSource.hxx>
#include <NamedEnum.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class faceToCell Declaration
	\*---------------------------------------------------------------------------*/

	class faceToCell
		:
		public topoSetSource
	{
	public:
		//- Enumeration defining the valid options
		enum faceAction
		{
			NEIGHBOUR,
			OWNER,
			ANY,
			ALL
		};

	private:

		static FoamFvMesh_EXPORT const NamedEnum<faceAction, 4> faceActionNames_;


		//- Add usage string
		static FoamFvMesh_EXPORT addToUsageTable usage_;

		//- Name of set to use
		word setName_;

		//- Option
		faceAction option_;


		// Private Member Functions

			//- Depending on face to cell option add to or delete from cellSet.
		FoamFvMesh_EXPORT void combine(topoSet& set, const bool add) const;


	public:

		//- Runtime type information
		/*TypeName("faceToCell");*/
		static const char* typeName_() { return "faceToCell"; }
		static FoamFvMesh_EXPORT const ::tnbLib::word typeName;
		static FoamFvMesh_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from components
		FoamFvMesh_EXPORT faceToCell
		(
			const polyMesh& mesh,
			const word& setName,
			const faceAction option
		);

		//- Construct from dictionary
		FoamFvMesh_EXPORT faceToCell
		(
			const polyMesh& mesh,
			const dictionary& dict
		);

		//- Construct from Istream
		FoamFvMesh_EXPORT faceToCell
		(
			const polyMesh& mesh,
			Istream&
		);


		//- Destructor
		FoamFvMesh_EXPORT virtual ~faceToCell();


		// Member Functions

		virtual sourceType setType() const
		{
			return CELLSETSOURCE;
		}

		FoamFvMesh_EXPORT virtual void applyToSet
		(
			const topoSetSource::setAction action,
			topoSet&
		) const;

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_faceToCell_Header
