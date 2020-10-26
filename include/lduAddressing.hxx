#pragma once
#ifndef _lduAddressing_Header
#define _lduAddressing_Header

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
	tnbLib::lduAddressing

Description
	The class contains the addressing required by the lduMatrix: upper, lower
	and losort.

	The addressing can be created in two ways: either with references to
	upper and lower in which case it stores references or from labelLists,
	in which case it stores the addressing itself. Additionally, the losort
	addressing belongs to the class is as on lazy evaluation.

	The ordering of owner addresses is such that the labels are in
	increasing order, with groups of identical labels for edges "owned" by
	the same point. The neighbour labels are also ordered in ascending
	order but only for groups of edges belonging to each point. An example
	is given below:
	\verbatim
		owner     neighbour
		0         1
		0         20
		1         2
		1         21
		2         3
		2         22
		3         4
		3         23
		4         5
		4         24
		5         6
		5         25
		6         7
		6         26
		7         8
		7         27
		8         9
		8         28
		9         10
		9         29
	\endverbatim

	There exists an alternative way of addressing the owner
	list: instead of repeating the same label in the owner list, it is
	possible to address the start of each point neighbours in the
	neighbour list. This reduces the size of owner addressing from a list
	over all edges to a list over all points + 1:

	\verbatim
		Owner start list: 0 2 4 6 8 10 12 14 16 18
	\endverbatim

	We shall use the second form of the addressing for fast lookup
	of edge label from the known owner and neighbour, using the following
	algorithm:
	-# take the owner label and position the start of lookup
	   using the owner start list
	-# loop through all neighbours of this owner (ending at the start of
	  lookup of owner + 1) until the match with current neighbour is found.
	  The index used on the neighbour list for the match is the edge index.

	While owner start addressing allows us to find the edge owned by the
	points, it is also necessary to find the edges for which the point is
	a neighbour. Losort addressing lists the edges neighboured by the
	point and we shall use the same trick as above to address into this
	list. Thus, for every point the losort start gives the address of the
	first face to neighbour this point.

SourceFiles
	lduAddressing.C

\*---------------------------------------------------------------------------*/

#include <labelList.hxx>
#include <lduSchedule.hxx>
#include <Tuple2.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class lduAddressing Declaration
	\*---------------------------------------------------------------------------*/

	class lduAddressing
	{
		// Private Data

			//- Number of equations
		label size_;


		//- Demand-driven data

			//- Losort addressing
		mutable labelList* losortPtr_;

		//- Owner start addressing
		mutable labelList* ownerStartPtr_;

		//- Losort start addressing
		mutable labelList* losortStartPtr_;


		// Private Member Functions

			//- Calculate losort
		FoamBase_EXPORT void calcLosort() const;

		//- Calculate owner start
		FoamBase_EXPORT void calcOwnerStart() const;

		//- Calculate losort start
		FoamBase_EXPORT void calcLosortStart() const;


	public:

		// Constructors

		lduAddressing(const label nEqns)
			:
			size_(nEqns),
			losortPtr_(nullptr),
			ownerStartPtr_(nullptr),
			losortStartPtr_(nullptr)
		{}

		//- Disallow default bitwise copy construction
		FoamBase_EXPORT lduAddressing(const lduAddressing&) = delete;


		//- Destructor
		FoamBase_EXPORT virtual ~lduAddressing();


		// Member Functions

			//- Return number of equations
		label size() const
		{
			return size_;
		}

		//- Return lower addressing
		FoamBase_EXPORT virtual const labelUList& lowerAddr() const = 0;

		//- Return upper addressing
		FoamBase_EXPORT virtual const labelUList& upperAddr() const = 0;

		//- Return patch to internal addressing given patch number
		virtual const labelUList& patchAddr
		(
			const label patchNo
		) const = 0;

		// Return patch field evaluation schedule
		FoamBase_EXPORT virtual const lduSchedule& patchSchedule() const = 0;

		//- Return losort addressing
		FoamBase_EXPORT const labelUList& losortAddr() const;

		//- Return owner start addressing
		FoamBase_EXPORT const labelUList& ownerStartAddr() const;

		//- Return losort start addressing
		FoamBase_EXPORT const labelUList& losortStartAddr() const;

		//- Return off-diagonal index given owner and neighbour label
		FoamBase_EXPORT label triIndex(const label a, const label b) const;

		//- Calculate bandwidth and profile of addressing
		FoamBase_EXPORT Tuple2<label, scalar> band() const;


		// Member Operators

			//- Disallow default bitwise assignment
		FoamBase_EXPORT void operator=(const lduAddressing&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_lduAddressing_Header
