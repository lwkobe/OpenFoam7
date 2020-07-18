#pragma once
#ifndef _ILList_Header
#define _ILList_Header

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
	tnbLib::ILList

Description
	Template class for intrusive linked lists.

SourceFiles
	ILList.C
	ILListIO.C

\*---------------------------------------------------------------------------*/

#include <UILList.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class Istream;
	class Ostream;

	// Forward declaration of friend functions and operators

	template<class LListBase, class T> class ILList;

	template<class LListBase, class T> Istream& operator>>
		(
			Istream&,
			ILList<LListBase, T>&
			);


	/*---------------------------------------------------------------------------*\
							   Class ILList Declaration
	\*---------------------------------------------------------------------------*/

	template<class LListBase, class T>
	class ILList
		:
		public UILList<LListBase, T>
	{
		// Private Member Functions

			//- Read from Istream using given Istream constructor class
		template<class INew>
		void read(Istream&, const INew&);


	public:

		// Constructors

			//- Null construct
		ILList()
		{}

		//- Construct given initial T
		ILList(T* a)
			:
			UILList<LListBase, T>(a)
		{}

		//- Construct from Istream
		ILList(Istream&);

		//- Copy constructor
		ILList(const ILList<LListBase, T>&);

		//- Move constructor
		ILList(ILList<LListBase, T>&&);

		//- Copy constructor with additional argument for clone
		template<class CloneArg>
		ILList(const ILList<LListBase, T>& lst, const CloneArg& cloneArg);

		//- Construct from Istream using given Istream constructor class
		template<class INew>
		ILList(Istream&, const INew&);


		//- Destructor
		~ILList();


		// Member Functions

			// Edit

				//- Remove the head element specified from the list and delete it
		bool eraseHead();

		//- Remove the specified element from the list and delete it
		bool erase(T* p);

		//- Clear the contents of the list
		void clear();

		//- Transfer the contents of the argument into this List
		//  and annul the argument list.
		void transfer(ILList<LListBase, T>&);


		// Member Operators

			//- Assignment operator
		void operator=(const ILList<LListBase, T>&);

		//- Move assignment operator
		void operator=(ILList<LListBase, T>&&);


		// Istream operator

			//- Read List from Istream, discarding contents of existing List.
		friend Istream& operator>> <LListBase, T>
			(
				Istream&,
				ILList<LListBase, T>&
				);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <ILList.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_ILList_Header