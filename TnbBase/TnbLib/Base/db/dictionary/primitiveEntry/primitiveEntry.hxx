#pragma once
#ifndef _primitiveEntry_Header
#define _primitiveEntry_Header

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
	tnbLib::primitiveEntry

Description
	A keyword and a list of tokens is a 'primitiveEntry'.
	An primitiveEntry can be read, written and printed, and the types and
	values of its tokens analysed.

	A primitiveEntry is a high-level building block for data description. It
	is a front-end for the token parser. A list of entries can be used as a
	set of keyword syntax elements, for example.

SourceFiles
	primitiveEntry.C
	primitiveEntryIO.C

\*---------------------------------------------------------------------------*/

#include <IStringStream.hxx>
#include <OStringStream.hxx>

#include <entry.hxx>
#include <ITstream.hxx>
#include <InfoProxy.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class dictionary;

	/*---------------------------------------------------------------------------*\
						   Class primitiveEntry Declaration
	\*---------------------------------------------------------------------------*/

	class primitiveEntry
		:
		public entry,
		public ITstream
	{
		// Private Member Functions

			//- Append the given tokens starting at the current tokenIndex
		FoamBase_EXPORT void append(const UList<token>&);

		//- Append the given token to this entry
		FoamBase_EXPORT void append
		(
			const token& currToken,
			const dictionary&,
			Istream&
		);

		//- Expand the given variable (keyword starts with $)
		FoamBase_EXPORT bool expandVariable(const string&, const dictionary&);

		//- Expand the given function (keyword starts with #)
		FoamBase_EXPORT bool expandFunction
		(
			const word&,
			const dictionary&,
			Istream&
		);

		//- Read the complete entry from the given stream
		FoamBase_EXPORT void readEntry(const dictionary&, Istream&);


	public:

		// Constructors

			//- Construct from keyword and a Istream
		FoamBase_EXPORT primitiveEntry(const keyType&, Istream&);

		//- Construct from keyword, parent dictionary and Istream
		FoamBase_EXPORT primitiveEntry(const keyType&, const dictionary& parentDict, Istream&);

		//- Construct from keyword and a ITstream
		FoamBase_EXPORT primitiveEntry(const keyType&, const ITstream&);

		//- Construct from keyword and a single token
		FoamBase_EXPORT primitiveEntry(const keyType&, const token&);

		//- Construct from keyword and a list of tokens
		FoamBase_EXPORT primitiveEntry(const keyType&, const UList<token>&);

		//- Move construct from keyword and by transferring a list of tokens
		FoamBase_EXPORT primitiveEntry(const keyType&, List<token>&&);

		//- Construct from keyword and a T
		template<class T>
		primitiveEntry(const keyType&, const T&);

		autoPtr<entry> clone(const dictionary&) const
		{
			return autoPtr<entry>(new primitiveEntry(*this));
		}


		// Member Functions

			//- Inherit read from ITstream
		using ITstream::read;

		//- Return the dictionary name
		const fileName& name() const
		{
			return ITstream::name();
		}

		//- Return the dictionary name
		fileName& name()
		{
			return ITstream::name();
		}

		//- Return line number of first token in dictionary
		FoamBase_EXPORT label startLineNumber() const;

		//- Return line number of last token in dictionary
		FoamBase_EXPORT label endLineNumber() const;

		//- Return true because this entry is a stream
		bool isStream() const
		{
			return true;
		}

		//- Return token stream if this entry is a primitive entry
		FoamBase_EXPORT ITstream& stream() const;

		//- This entry is not a dictionary,
		//  calling this function generates a FatalError
		FoamBase_EXPORT const dictionary& dict() const;

		//- This entry is not a dictionary,
		//  calling this function generates a FatalError
		FoamBase_EXPORT dictionary& dict();

		//- Read tokens from the given stream
		FoamBase_EXPORT virtual bool read(const dictionary&, Istream&);

		//- Write
		FoamBase_EXPORT void write(Ostream&) const;

		//- Write, optionally with contents only (no keyword, etc)
		FoamBase_EXPORT void write(Ostream&, const bool contentsOnly) const;

		//- Return info proxy.
		//  Used to print token information to a stream
		InfoProxy<primitiveEntry> info() const
		{
			return *this;
		}
	};


	template<>
	Ostream& operator<<(Ostream&, const InfoProxy<primitiveEntry>&);


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <primitiveEntryTemplatesI.hxx>

//#ifdef NoRepository
//#include <primitiveEntryTemplates.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_primitiveEntry_Header
