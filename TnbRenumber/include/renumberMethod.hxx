#pragma once
#ifndef _renumberMethod_Header
#define _renumberMethod_Header

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
	tnbLib::renumberMethod

Description
	Abstract base class for renumbering

SourceFiles
	renumberMethod.C

\*---------------------------------------------------------------------------*/

#include <polyMesh.hxx>
#include <pointField.hxx>
#include <CompactListList.hxx>

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class renumberMethod Declaration
	\*---------------------------------------------------------------------------*/

	class renumberMethod
	{

	protected:

		// Protected data

		const dictionary& renumberDict_;


	public:

		//- Runtime type information
		TypeName("renumberMethod");


		// Declare run-time constructor selection tables

		declareRunTimeSelectionTable
		(
			autoPtr,
			renumberMethod,
			dictionary,
			(
				const dictionary& renumberDict
				),
				(renumberDict)
		);


		// Constructors

			//- Construct given the renumber dictionary
		renumberMethod(const dictionary& renumberDict)
			:
			renumberDict_(renumberDict)
		{}

		//- Disallow default bitwise copy construction
		renumberMethod(const renumberMethod&) = delete;


		// Selectors

			//- Return a reference to the selected renumbering method
		static autoPtr<renumberMethod> New
		(
			const dictionary& renumberDict
		);


		//- Destructor
		virtual ~renumberMethod()
		{}


		// Member Functions

			//- Return the order in which cells need to be visited, i.e.
			//  from ordered back to original cell label.
			//  This is only defined for geometric renumberMethods.
		virtual labelList renumber(const pointField&) const
		{
			NotImplemented;
			return labelList(0);
		}

		//- Return the order in which cells need to be visited, i.e.
		//  from ordered back to original cell label.
		//  Use the mesh connectivity (if needed)
		virtual labelList renumber(const polyMesh&, const pointField&) const;

		//- Return the order in which cells need to be visited, i.e.
		//  from ordered back to original cell label.
		//  Addressing in losort addressing (= neighbour + offsets into
		//  neighbour)
		virtual labelList renumber
		(
			const labelList& cellCells,
			const labelList& offsets,
			const pointField&
		) const;

		//- Return the order in which cells need to be visited, i.e.
		//  from ordered back to original cell label.
		//  Gets passed agglomeration map (from fine to coarse cells) and coarse
		//  cell
		//  location. Can be overridden by renumberMethods that provide this
		//  functionality natively. Coarse cells are local to the processor
		//  (if in parallel). If you want to have coarse cells spanning
		//  processors use the globalCellCells instead.
		virtual labelList renumber
		(
			const polyMesh& mesh,
			const labelList& cellToRegion,
			const pointField& regionPoints
		) const;

		//- Return the order in which cells need to be visited, i.e.
		//  from ordered back to original cell label.
		//  The connectivity is equal to mesh.cellCells() except
		//  - the connections are across coupled patches
		virtual labelList renumber
		(
			const labelListList& cellCells,
			const pointField& cc
		) const = 0;


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const renumberMethod&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_renumberMethod_Header
