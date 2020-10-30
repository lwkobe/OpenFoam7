#pragma once
#ifndef _patchPatchDist_Header
#define _patchPatchDist_Header

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
	tnbLib::patchPatchDist

Description
	Like wallDist but calculates on a patch the distance to nearest neighbouring
	patches. Uses PatchEdgeFaceWave to do actual calculation.

SourceFiles
	patchPatchDist.C

\*---------------------------------------------------------------------------*/


#include <scalarField.hxx>
#include <HashSet.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	//class polyMesh;
	class polyPatch;

	/*---------------------------------------------------------------------------*\
							 Class patchPatchDist Declaration
	\*---------------------------------------------------------------------------*/

	class patchPatchDist
		:
		public scalarField
	{
		// Private Member Data

			//- Patch to operate on
		const polyPatch& patch_;

		//- Patches to determine the distance to
		const labelHashSet nbrPatchIDs_;

		//- Number of unset faces.
		label nUnset_;

	public:

		// Constructors

			//- Construct from patch and neighbour patches.
		FoamFvMesh_EXPORT patchPatchDist
		(
			const polyPatch& pp,
			const labelHashSet& nbrPatchIDs
		);


		//- Destructor
		FoamFvMesh_EXPORT virtual ~patchPatchDist();


		// Member Functions

		const scalarField& y() const
		{
			return *this;
		}

		label nUnset() const
		{
			return nUnset_;
		}

		//- Correct for mesh geom/topo changes
		FoamFvMesh_EXPORT virtual void correct();
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_patchPatchDist_Header
