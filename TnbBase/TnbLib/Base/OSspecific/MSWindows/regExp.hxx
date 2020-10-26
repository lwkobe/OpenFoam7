#pragma once
#ifndef _regExp_Header
#define _regExp_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
	\\  /    A nd           | Web:         http://www.foam-extend.org
	 \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
	This file is part of foam-extend.

	foam-extend is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation, either version 3 of the License, or (at your
	option) any later version.

	foam-extend is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
	tnbLib::regExp

Description
	Wrapper around POSIX extended regular expressions.

SeeAlso
	The manpage regex(7) for more information about POSIX regular expressions.
	These differ somewhat from @c Perl and @c sed regular expressions.

SourceFiles
	regExp.C

\*---------------------------------------------------------------------------*/

#include <Base_Module.hxx>

#include <string>

template <class T> class CRegexpT;

namespace tnbLib
{

	// Forward declaration of classes
	class string;
	template<class T> class List;

	/*---------------------------------------------------------------------------*\
							   Class regExp Declaration
	\*---------------------------------------------------------------------------*/

	class regExp
	{
		// Private data

			//- Precompiled regular expression
		mutable CRegexpT<char>* preg_;

		// Private member functions

			//- Disallow default bitwise copy construct
		FoamBase_EXPORT regExp(const regExp&);

		//- Disallow default bitwise assignment
		FoamBase_EXPORT void operator=(const regExp&);

	public:

		//- Is character a regular expression meta-character?
		//  any character: '.' \n
		//  quantifiers: '*', '+', '?' \n
		//  grouping: '(', '|', ')' \n
		//  range: '[', ']' \n
		//
		//  Don't bother checking for '{digit}' bounds
		inline static bool meta(char c)
		{
			return
				(
				(c == '.')                           // any character
					|| (c == '*' || c == '+' || c == '?')   // quantifiers
					|| (c == '(' || c == ')' || c == '|')   // grouping/branching
					|| (c == '[' || c == ']')               // range
					);
		}


		// Constructors

			//- Construct null
		FoamBase_EXPORT regExp();

		//- Construct from character array, optionally ignoring case
		FoamBase_EXPORT regExp(const char*, const bool ignoreCase = false);

		//- Construct from std::string (or string), optionally ignoring case
		FoamBase_EXPORT regExp(const std::string&, const bool ignoreCase = false);

		// Destructor

		FoamBase_EXPORT ~regExp();


		// Member functions

		//- Access

			//- Return true if a precompiled expression does not exist
		inline bool empty() const
		{
			return !preg_;
		}

		//- Does a precompiled expression exist?
		inline bool exists() const
		{
			return preg_ ? true : false;
		}


		//- Editing

			//- Compile pattern into a regular expression, optionally ignoring case
		FoamBase_EXPORT void set(const char*, const bool ignoreCase = false) const;

		//- Compile pattern into a regular expression, optionally ignoring case
		FoamBase_EXPORT void set(const std::string&, const bool ignoreCase = false) const;


		//- Release precompiled expression.
		//  Returns true if precompiled expression existed before clear
		FoamBase_EXPORT 	bool clear() const;


		//- Searching

			//- Find position within string.
			//  Returns the index where it begins or string::npos if not found
		FoamBase_EXPORT std::string::size_type find(const std::string& str) const;

		//- Return true if it matches the entire string
		//  The begin-of-line (^) and end-of-line ($) anchors are implicit
		FoamBase_EXPORT bool match(const std::string&) const;

		//- Return true if it matches and sets the sub-groups matched
		//  The begin-of-line (^) and end-of-line ($) anchors are implicit
		FoamBase_EXPORT bool match(const string&, List<string>& groups) const;

		//- Return true if the regex was found in within string
		bool search(const std::string& str) const
		{
			return std::string::npos != find(str);
		}


		// Member Operators

			//- Assign and compile pattern from a character array
			//  Always case sensitive
		FoamBase_EXPORT void operator=(const char*);

		//- Assign and compile pattern from string
		//  Always case sensitive
		FoamBase_EXPORT void operator=(const std::string&);

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
}

#endif // !_regExp_Header
