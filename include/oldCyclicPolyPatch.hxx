#pragma once
#ifndef _oldCyclicPolyPatch_Header
#define _oldCyclicPolyPatch_Header

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
	tnbLib::oldCyclicPolyPatch

Description
	'old' style cyclic polyPatch with all faces in single patch. Does ordering
	but cannot be used to run. Writes 'type cyclic' so foamUpgradeCyclics
	can be run afterwards.
	Used to get cyclics from mesh converters that assume cyclics in single
	patch (e.g. fluent3DMeshToFoam)

SourceFiles
	oldCyclicPolyPatch.C

\*---------------------------------------------------------------------------*/

#include <coupledPolyPatch.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						  Class oldCyclicPolyPatch Declaration
	\*---------------------------------------------------------------------------*/

	class oldCyclicPolyPatch
		:
		public coupledPolyPatch
	{
		// Private Data

			//- Morph:angle between normals of neighbouring faces.
			//  Used to split cyclic into halves.
		scalar featureCos_;


		// For rotation

			//- Axis of rotation for rotational cyclics
		vector rotationAxis_;

		//- Point on axis of rotation for rotational cyclics
		point rotationCentre_;

		// For translation

			//- Translation vector
		vector separationVector_;


		// Private Member Functions

			//- Find amongst selected faces the one with the largest area
		static FoamBase_EXPORT label findMaxArea(const pointField&, const faceList&);

		FoamBase_EXPORT void calcTransforms();

		//- Calculate face centres
		static FoamBase_EXPORT pointField calcFaceCentres
		(
			const UList<face>&,
			const pointField&
		);

		//- Get f[0] for all faces
		static FoamBase_EXPORT pointField getAnchorPoints
		(
			const UList<face>&,
			const pointField&
		);

		// Face ordering

			//- Find the two parts of the faces of pp using feature edges.
			//  Returns true if successful.
		FoamBase_EXPORT bool getGeometricHalves
		(
			const primitivePatch&,
			labelList&,
			labelList&
		) const;

		//- Calculate geometric factors of the two halves.
		FoamBase_EXPORT void getCentresAndAnchors
		(
			const primitivePatch&,
			const faceList& half0Faces,
			const faceList& half1Faces,

			pointField& ppPoints,
			pointField& half0Ctrs,
			pointField& half1Ctrs,
			pointField& anchors0,
			scalarField& tols
		) const;

		//- Given matched faces matches the anchor point. Sets faceMap,
		//  rotation. Returns true if all matched.
		FoamBase_EXPORT bool matchAnchors
		(
			const bool report,
			const primitivePatch&,
			const labelList&,
			const pointField&,
			const labelList&,
			const faceList&,
			const labelList&,
			const scalarField&,

			labelList& faceMap,
			labelList& rotation
		) const;

		//- For rotational cases, try to find a unique face on each side
		//  of the cyclic.
		FoamBase_EXPORT label getConsistentRotationFace
		(
			const pointField& faceCentres
		) const;


	protected:

		// Protected Member functions

			//- Initialise the calculation of the patch geometry
		FoamBase_EXPORT virtual void initGeometry(PstreamBuffers&);

		//- Calculate the patch geometry
		FoamBase_EXPORT virtual void calcGeometry(PstreamBuffers&);

		//- Initialise the patches for moving points
		FoamBase_EXPORT virtual void initMovePoints(PstreamBuffers&, const pointField&);

		//- Correct patches after moving points
		FoamBase_EXPORT virtual void movePoints(PstreamBuffers&, const pointField&);

		//- Initialise the update of the patch topology
		FoamBase_EXPORT virtual void initUpdateMesh(PstreamBuffers&);

		//- Update of the patch topology
		FoamBase_EXPORT virtual void updateMesh(PstreamBuffers&);

	public:

		//- Runtime type information
		//TypeName("oldCyclic");
		static const char* typeName_() { return "oldCyclic"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from components
		FoamBase_EXPORT oldCyclicPolyPatch
		(
			const word& name,
			const label size,
			const label start,
			const label index,
			const polyBoundaryMesh& bm,
			const word& patchType,
			const transformType transform = UNKNOWN
		);

		//- Construct from dictionary
		FoamBase_EXPORT oldCyclicPolyPatch
		(
			const word& name,
			const dictionary& dict,
			const label index,
			const polyBoundaryMesh& bm,
			const word& patchType
		);

		//- Construct as copy, resetting the boundary mesh
		FoamBase_EXPORT oldCyclicPolyPatch(const oldCyclicPolyPatch&, const polyBoundaryMesh&);

		//- Construct given the original patch and resetting the
		//  face list and boundary mesh information
		FoamBase_EXPORT oldCyclicPolyPatch
		(
			const oldCyclicPolyPatch& pp,
			const polyBoundaryMesh& bm,
			const label index,
			const label newSize,
			const label newStart
		);

		//- Construct and return a clone, resetting the boundary mesh
		virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
		{
			return autoPtr<polyPatch>(new oldCyclicPolyPatch(*this, bm));
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
					new oldCyclicPolyPatch(*this, bm, index, newSize, newStart)
					);
		}


		// Destructor

		FoamBase_EXPORT virtual ~oldCyclicPolyPatch();


		// Member Functions

			// Access

				//- Does this side own the patch ?
		virtual bool owner() const
		{
			NotImplemented;
			return true;
		}

		//- Transform a patch-based position from other side to this side
		virtual void transformPosition(pointField& l) const
		{
			NotImplemented;
		}

		//- Transform a patch-based position from other side to this side
		virtual void transformPosition(point&, const label facei) const
		{
			NotImplemented;
		}

		//- Calculate the patch geometry
		FoamBase_EXPORT virtual void calcGeometry
		(
			const primitivePatch& referPatch,
			const pointField& thisCtrs,
			const vectorField& thisAreas,
			const pointField& thisCc,
			const pointField& nbrCtrs,
			const vectorField& nbrAreas,
			const pointField& nbrCc
		);

		//- Initialize ordering for primitivePatch. Does not
		//  refer to *this (except for name() and type() etc.)
		FoamBase_EXPORT virtual void initOrder
		(
			PstreamBuffers&,
			const primitivePatch&
		) const;

		//- Return new ordering for primitivePatch.
		//  Ordering is -faceMap: for every face
		//  index of the new face -rotation:for every new face the clockwise
		//  shift of the original face. Return false if nothing changes
		//  (faceMap is identity, rotation is 0), true otherwise.
		FoamBase_EXPORT virtual bool order
		(
			PstreamBuffers&,
			const primitivePatch&,
			labelList& faceMap,
			labelList& rotation
		) const;

		//- Write the polyPatch data as a dictionary
		FoamBase_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_oldCyclicPolyPatch_Header
