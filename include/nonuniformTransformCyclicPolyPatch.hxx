#pragma once
#ifndef _nonuniformTransformCyclicPolyPatch_Header
#define _nonuniformTransformCyclicPolyPatch_Header

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
	tnbLib::nonuniformTransformCyclicPolyPatch

Description
	Transform boundary used in extruded regions. Allows non-uniform transforms.
	Wip.

SourceFiles
	nonuniformTransformCyclicPolyPatch.C

\*---------------------------------------------------------------------------*/

#include <cyclicPolyPatch.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
				 Class nonuniformTransformCyclicPolyPatch Declaration
	\*---------------------------------------------------------------------------*/

	class nonuniformTransformCyclicPolyPatch
		:
		public cyclicPolyPatch
	{

	public:

		//- Runtime type information
		//TypeName("nonuniformTransformCyclic");
		static const char* typeName_() { return "nonuniformTransformCyclic"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from components
		nonuniformTransformCyclicPolyPatch
		(
			const word& name,
			const label size,
			const label start,
			const label index,
			const polyBoundaryMesh& bm,
			const word& patchType,
			const transformType transform = UNKNOWN
		)
			:
			cyclicPolyPatch(name, size, start, index, bm, patchType, transform)
		{}

		//- Construct from dictionary
		nonuniformTransformCyclicPolyPatch
		(
			const word& name,
			const dictionary& dict,
			const label index,
			const polyBoundaryMesh& bm,
			const word& patchType
		)
			:
			cyclicPolyPatch(name, dict, index, bm, patchType)
		{}

		//- Construct as copy, resetting the boundary mesh
		nonuniformTransformCyclicPolyPatch
		(
			const nonuniformTransformCyclicPolyPatch& pp,
			const polyBoundaryMesh& bm
		)
			:
			cyclicPolyPatch(pp, bm)
		{}

		//- Construct given the original patch and resetting the
		//  face list and boundary mesh information
		nonuniformTransformCyclicPolyPatch
		(
			const nonuniformTransformCyclicPolyPatch& pp,
			const polyBoundaryMesh& bm,
			const label index,
			const label newSize,
			const label newStart,
			const word& neighbPatchName
		)
			:
			cyclicPolyPatch(pp, bm, index, newSize, newStart, neighbPatchName)
		{}

		//- Construct given the original patch and a map
		nonuniformTransformCyclicPolyPatch
		(
			const nonuniformTransformCyclicPolyPatch& pp,
			const polyBoundaryMesh& bm,
			const label index,
			const labelUList& mapAddressing,
			const label newStart
		)
			:
			cyclicPolyPatch(pp, bm, index, mapAddressing, newStart)
		{}

		//- Construct and return a clone, resetting the boundary mesh
		virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
		{
			return autoPtr<polyPatch>
				(
					new nonuniformTransformCyclicPolyPatch(*this, bm)
					);
		}

		//- Construct and return a clone, resetting the face list
		//  and boundary mesh
		virtual autoPtr<polyPatch> clone
		(
			const polyBoundaryMesh& bm,
			const label index,
			const label newSize,
			const label newStart
		) const
		{
			return autoPtr<polyPatch>
				(
					new nonuniformTransformCyclicPolyPatch
					(
						*this,
						bm,
						index,
						newSize,
						newStart,
						neighbPatchName()
					)
					);
		}

		//- Construct and return a clone, resetting the face list
		//  and boundary mesh
		virtual autoPtr<polyPatch> clone
		(
			const polyBoundaryMesh& bm,
			const label index,
			const labelUList& mapAddressing,
			const label newStart
		) const
		{
			return autoPtr<polyPatch>
				(
					new nonuniformTransformCyclicPolyPatch
					(
						*this,
						bm,
						index,
						mapAddressing,
						newStart
					)
					);
		}


		// Destructor

		virtual ~nonuniformTransformCyclicPolyPatch()
		{}


	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_nonuniformTransformCyclicPolyPatch_Header
