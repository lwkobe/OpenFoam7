#pragma once
#ifndef _patchIdentifier_Header
#define _patchIdentifier_Header

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
	tnbLib::patchIdentifier

Description
	Identifies patch by name, patch index and physical type

SourceFiles
	patchIdentifier.C

\*---------------------------------------------------------------------------*/

#include <wordList.hxx>
#include <label.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class dictionary;

	// Forward declaration of friend functions and operators

	class patchIdentifier;
	FoamBase_EXPORT Ostream& operator<<(Ostream&, const patchIdentifier&);


	/*---------------------------------------------------------------------------*\
							   Class patchIdentifier Declaration
	\*---------------------------------------------------------------------------*/

	class patchIdentifier
	{
		// Private Data

			//- Name of patch
		word name_;

		//- Index of patch in boundary
		label index_;

		//- Optional physical type
		mutable word physicalType_;

		//- Optional groups patch belongs to
		wordList inGroups_;


	public:

		// Constructors

			//- Construct from components
		FoamBase_EXPORT patchIdentifier
		(
			const word& name,
			const label index,
			const word& physicalType = word::null,
			const wordList& inGroups = wordList()
		);

		//- Construct from dictionary
		FoamBase_EXPORT patchIdentifier
		(
			const word& name,
			const dictionary&,
			const label index
		);

		//- Construct from geometric patch, resetting the index
		FoamBase_EXPORT patchIdentifier
		(
			const patchIdentifier&,
			const label index
		);


		//- Destructor
		FoamBase_EXPORT virtual ~patchIdentifier();


		// Member Functions

			//- Return name
		const word& name() const
		{
			return name_;
		}

		//- Return name for modification
		word& name()
		{
			return name_;
		}

		//- Return the optional physical type of the patch
		const word& physicalType() const
		{
			return physicalType_;
		}

		//- Return the optional physical type of the patch for modification
		word& physicalType()
		{
			return physicalType_;
		}

		//- Return the index of this patch in the boundaryMesh
		label index() const
		{
			return index_;
		}

		//- Return the index of this patch in the boundaryMesh for modification
		label& index()
		{
			return index_;
		}

		//- Return the optional groups patch belongs to
		const wordList& inGroups() const
		{
			return inGroups_;
		}

		//- Return the optional groups patch belongs to for modification
		wordList& inGroups()
		{
			return inGroups_;
		}

		//- Test if in group
		FoamBase_EXPORT bool inGroup(const word&) const;

		//- Write patchIdentifier as a dictionary
		FoamBase_EXPORT void write(Ostream&) const;


		// Ostream Operator

		friend Ostream& operator<<(Ostream&, const patchIdentifier&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_patchIdentifier_Header
