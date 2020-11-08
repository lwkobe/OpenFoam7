#pragma once
#ifndef _removeCells_Header
#define _removeCells_Header

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
	tnbLib::removeCells

Description
	Given list of cells to remove insert all the topology changes.

	Works in two passes:
	- get faces that will become boundary faces
	- given these faces and the patches they have to go into make the
	  changes.

SourceFiles
	removeCells.C

\*---------------------------------------------------------------------------*/

#include <labelList.hxx>
#include <typeInfo.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of classes
	class polyMesh;
	class polyTopoChange;
	class mapPolyMesh;

	/*---------------------------------------------------------------------------*\
							   Class removeCells Declaration
	\*---------------------------------------------------------------------------*/

	class removeCells
	{
		// Private Data

			//- Reference to mesh
		const polyMesh& mesh_;

		//- Whether or not to synchronize parallel case.
		const bool syncPar_;


		// Private Member Functions

			//- Decrease count of elements of f
		static FoamDynamicMesh_EXPORT void uncount
		(
			const labelList& f,
			labelList& nUsage
		);


	public:

		//- Runtime type information
		//ClassName("removeCells");
		static const char* typeName_() { return "removeCells"; }
		static FoamDynamicMesh_EXPORT const ::tnbLib::word typeName;
		static FoamDynamicMesh_EXPORT int debug;


		// Constructors

			//- Construct from mesh. syncPar: do parallel synchronization.
		FoamDynamicMesh_EXPORT removeCells(const polyMesh& mesh, const bool syncPar = true);


		// Member Functions

			// Topology changes

				//- Get labels of exposed faces.
				//  These are
				//  - internal faces that become boundary faces
				//  - coupled faces that become uncoupled (since one of the sides
				//    gets deleted)
		FoamDynamicMesh_EXPORT labelList getExposedFaces(const labelList& cellsToRemove) const;

		//- Play commands into polyTopoChange to remove cells.
		//  patchIDs is for every element in facesToExpose (see above) the
		//  patch it has to go into. This cannot be a coupled patch!
		FoamDynamicMesh_EXPORT void setRefinement
		(
			const labelList& cellsToRemove,
			const labelList& facesToExpose,
			const labelList& patchIDs,
			polyTopoChange&
		) const;

		//- Force recalculation of locally stored data on topological change
		void updateMesh(const mapPolyMesh&)
		{}
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_removeCells_Header
