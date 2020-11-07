#pragma once
#ifndef _mappedPatchBase_Header
#define _mappedPatchBase_Header

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
	tnbLib::mappedPatchBase

Description
	Determines a mapping between patch face centres and mesh cell or face
	centres and processors they're on.

	If constructed from dictionary:
	\verbatim
		// Region to sample (default is region0)
		sampleRegion region0;

		// What to sample:
		// - nearestCell         : sample cell containing point
		// - nearestOnlyCell     : nearest sample cell (even if not containing
		//                         point)
		// - nearestPatchFace    : nearest face on selected patch
		// - nearestPatchFaceAMI : nearest face on selected patch
								   - patches need not conform
								   - uses AMI interpolation
		// - nearestFace         : nearest boundary face on any patch
		// - nearestPatchPoint   : nearest patch point (for coupled points
		//                         this might be any of the points so you have
		//                         to guarantee the point data is synchronised
		//                         beforehand)
		sampleMode nearestCell;

		// If sampleMode is nearestPatchFace : patch to find faces of
		samplePatch movingWall;

		// If sampleMode is nearestPatchFace : specify patchgroup to find
		// samplePatch and sampleRegion (if not provided)
		coupleGroup baffleGroup;

		// How to supply offset (w.r.t. my patch face centres):
		// - uniform : single offset vector
		// - nonuniform : per-face offset vector
		// - normal : using supplied distance and face normal
		offsetMode uniform;

		// According to offsetMode (see above) supply one of
		// offset, offsets or distance
		offset  (1 0 0);
	\endverbatim

	Note: if offsetMode is \c normal it uses outwards pointing normals. So
	supply a negative distance if sampling inside the domain.


Note
	Storage is not optimal. It temporary collects all (patch)face centres
	on all processors to keep the addressing calculation simple.

SourceFiles
	mappedPatchBase.C

\*---------------------------------------------------------------------------*/

#include <pointField.hxx>
#include <Tuple2.hxx>
#include <pointIndexHit.hxx>
#include <AMIInterpolation.hxx>
#include <coupleGroupIdentifier.hxx>

#include <polyMesh.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class polyPatch;
	class polyMesh;
	class mapDistribute;

	/*---------------------------------------------------------------------------*\
						   Class mappedPatchBase Declaration
	\*---------------------------------------------------------------------------*/

	class mappedPatchBase
	{

	public:

		// Type enumerations

			//- Mesh items to sample
		enum sampleMode
		{
			NEARESTCELL,         // nearest cell containing sample
			NEARESTPATCHFACE,    // nearest face on selected patch
			NEARESTPATCHFACEAMI, // nearest patch face + AMI interpolation
			NEARESTPATCHPOINT,   // nearest point on selected patch
			NEARESTFACE,         // nearest face
			NEARESTONLYCELL      // nearest cell (even if not containing cell)
		};

		//- How to project face centres
		enum offsetMode
		{
			UNIFORM,            // single offset vector
			NONUNIFORM,         // per-face offset vector
			NORMAL              // use face normal + distance
		};

		static FoamFvMesh_EXPORT const NamedEnum<sampleMode, 6> sampleModeNames_;

		static FoamFvMesh_EXPORT const NamedEnum<offsetMode, 3> offsetModeNames_;


		//- Helper class for finding nearest
		//  Nearest:
		//  - point+local index
		//  - sqr(distance)
		//  - processor
		typedef Tuple2<pointIndexHit, Tuple2<scalar, label>> nearInfo;

		class nearestEqOp
		{

		public:

			void operator()(nearInfo& x, const nearInfo& y) const
			{
				if (y.first().hit())
				{
					if (!x.first().hit())
					{
						x = y;
					}
					else if (y.second().first() < x.second().first())
					{
						x = y;
					}
				}
			}
		};

		class maxProcEqOp
		{

		public:

			void operator()(nearInfo& x, const nearInfo& y) const
			{
				if (y.first().hit())
				{
					if (!x.first().hit())
					{
						x = y;
					}
					else if (y.second().second() > x.second().second())
					{
						x = y;
					}
				}
			}
		};


	protected:

		// Protected data

			//- Patch to sample
		const polyPatch& patch_;

		//- Region to sample
		mutable word sampleRegion_;

		//- What to sample
		const sampleMode mode_;

		//- Patch (if in sampleMode NEARESTPATCH*)
		mutable word samplePatch_;

		//- PatchGroup (if in sampleMode NEARESTPATCH*)
		const coupleGroupIdentifier coupleGroup_;

		//- How to obtain samples
		offsetMode offsetMode_;

		//- Offset vector (uniform)
		vector offset_;

		//- Offset vector (nonuniform)
		vectorField offsets_;

		//- Offset distance (normal)
		scalar distance_;

		//- Same region
		mutable bool sameRegion_;


		// Derived information

			//- Communication schedule:
			//
			//    - Cells/faces to sample per processor
			//    - Patch faces to receive per processor
			//    - schedule
		mutable autoPtr<mapDistribute> mapPtr_;


		// AMI interpolator (only for NEARESTPATCHFACEAMI)

			//- Pointer to AMI interpolator
		mutable autoPtr<AMIInterpolation> AMIPtr_;

		//- Flag to indicate that slave patch should be reversed for AMI
		const bool AMIReverse_;

		//- Pointer to projection surface employed by AMI interpolator
		mutable autoPtr<searchableSurface> surfPtr_;

		//- Dictionary storing projection surface description
		dictionary surfDict_;


		// Protected Member Functions

			//- Get the points from face-centre-decomposition face centres
			//  and project them onto the face-diagonal-decomposition triangles.
		FoamFvMesh_EXPORT tmp<pointField> facePoints(const polyPatch&) const;

		//- Collect single list of samples and originating processor+face.
		FoamFvMesh_EXPORT void collectSamples
		(
			const pointField& facePoints,
			pointField&,
			labelList& patchFaceProcs,
			labelList& patchFaces,
			pointField& patchFc
		) const;

		//- Find cells/faces containing samples
		FoamFvMesh_EXPORT void findSamples
		(
			const sampleMode mode,      // search mode
			const pointField&,
			labelList& sampleProcs,     // processor containing sample
			labelList& sampleIndices,   // local index of cell/face
			pointField& sampleLocations // actual representative location
		) const;

		//- Get the sample points given the face points
		FoamFvMesh_EXPORT tmp<pointField> samplePoints(const pointField&) const;

		//- Calculate mapping
		FoamFvMesh_EXPORT void calcMapping() const;

		//- Calculate AMI interpolator
		FoamFvMesh_EXPORT void calcAMI() const;

		//- Helper to read field or non-uniform list from dictionary
		static FoamFvMesh_EXPORT tmp<pointField> readListOrField
		(
			const word& keyword,
			const dictionary& dict,
			const label size
		);


	public:

		//- Runtime type information
		/*TypeName("mappedPatchBase");*/
		static const char* typeName_() { return "mappedPatchBase"; }
		static FoamFvMesh_EXPORT const ::tnbLib::word typeName;
		static FoamFvMesh_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch
		FoamFvMesh_EXPORT mappedPatchBase(const polyPatch&);

		//- Construct with offsetMode=non-uniform
		FoamFvMesh_EXPORT mappedPatchBase
		(
			const polyPatch& pp,
			const word& sampleRegion,
			const sampleMode sampleMode,
			const word& samplePatch,
			const vectorField& offsets
		);

		//- Construct from offsetMode=uniform
		FoamFvMesh_EXPORT mappedPatchBase
		(
			const polyPatch& pp,
			const word& sampleRegion,
			const sampleMode sampleMode,
			const word& samplePatch,
			const vector& offset
		);

		//- Construct from offsetMode=normal and distance
		FoamFvMesh_EXPORT mappedPatchBase
		(
			const polyPatch& pp,
			const word& sampleRegion,
			const sampleMode sampleMode,
			const word& samplePatch,
			const scalar distance
		);

		//- Construct from dictionary
		FoamFvMesh_EXPORT mappedPatchBase(const polyPatch&, const dictionary&);

		//- Construct from dictionary and (collocated) sample mode
		//  (only for nearestPatchFace, nearestPatchFaceAMI, nearestPatchPoint)
		//  Assumes zero offset.
		FoamFvMesh_EXPORT mappedPatchBase(const polyPatch&, const sampleMode, const dictionary&);

		//- Construct as copy, resetting patch
		FoamFvMesh_EXPORT mappedPatchBase(const polyPatch&, const mappedPatchBase&);

		//- Construct as copy, resetting patch, map original data
		FoamFvMesh_EXPORT mappedPatchBase
		(
			const polyPatch&,
			const mappedPatchBase&,
			const labelUList& mapAddressing
		);


		//- Destructor
		FoamFvMesh_EXPORT virtual ~mappedPatchBase();


		// Member Functions

		FoamFvMesh_EXPORT void clearOut();

		// Access

			//- What to sample
		inline const sampleMode& mode() const;

		//- Region to sample
		inline const word& sampleRegion() const;

		//- Patch (only if NEARESTPATCHFACE)
		inline const word& samplePatch() const;

		//- PatchGroup (only if NEARESTPATCHFACE)
		inline const word& coupleGroup() const;

		//- Return size of mapped mesh/patch/boundary
		inline label sampleSize() const;

		//- Offset vector (from patch faces to destination mesh objects)
		inline const vector& offset() const;

		//- Offset vector (from patch faces to destination mesh objects)
		inline const vectorField& offsets() const;

		//- Cached sampleRegion != mesh.name()
		inline bool sameRegion() const;

		//- Return reference to the parallel distribution map
		inline const mapDistribute& map() const;

		//- Return reference to the AMI interpolator
		inline const AMIInterpolation& AMI
		(
			const bool forceUpdate = false
		) const;

		//- Return a pointer to the AMI projection surface
		FoamFvMesh_EXPORT const autoPtr<tnbLib::searchableSurface>& surfPtr() const;

		//- Get the region mesh
		FoamFvMesh_EXPORT const polyMesh& sampleMesh() const;

		//- Get the patch on the region
		FoamFvMesh_EXPORT const polyPatch& samplePolyPatch() const;


		// Helpers

			//- Get the sample points
		FoamFvMesh_EXPORT tmp<pointField> samplePoints() const;

		//- Get a point on the face given a face decomposition method:
		//  face-centre-tet : face centre. Returns index of face.
		//  face-planes     : face centre. Returns index of face.
		//  face-diagonal   : intersection of ray from cellcentre to
		//                    facecentre with any of the triangles.
		//                    Returns index (0..size-2) of triangle.
		static FoamFvMesh_EXPORT pointIndexHit facePoint
		(
			const polyMesh&,
			const label facei,
			const polyMesh::cellDecomposition
		);


		// Distribute

			//- Wrapper around map/interpolate data distribution
		template<class Type>
		void distribute(List<Type>& lst) const;

		//- Wrapper around map/interpolate data distribution with operation
		template<class Type, class CombineOp>
		void distribute(List<Type>& lst, const CombineOp& cop) const;

		//- Wrapper around map/interpolate data distribution
		template<class Type>
		void reverseDistribute(List<Type>& lst) const;

		//- Wrapper around map/interpolate data distribution with operation
		template<class Type, class CombineOp>
		void reverseDistribute(List<Type>& lst, const CombineOp& cop) const;


		// I/O

			//- Write as a dictionary
		FoamFvMesh_EXPORT virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <mappedPatchBaseI.hxx>
#include <mappedPatchBaseTemplatesI.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#ifdef NoRepository
//#include <mappedPatchBaseTemplates.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_mappedPatchBase_Header
