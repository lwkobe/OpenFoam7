#pragma once
#ifndef _sampledTriSurfaceMesh_Header
#define _sampledTriSurfaceMesh_Header

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
	tnbLib::sampledSurfaces::triSurfaceMesh

Description
	A sampledSurface from a triSurfaceMesh. It samples on the points/triangles
	of the triSurface.

	- it either samples cells or (non-coupled) boundary faces

	- 6 different modes:
		- source=cells, interpolate=false:
			finds per triangle centre the nearest cell centre and uses its value
		- source=cells, interpolate=true
			finds per triangle centre the nearest cell centre.
			Per surface point checks if this nearest cell is the one containing
			point; otherwise projects the point onto the nearest point on
			the boundary of the cell (to make sure interpolateCellPoint
			gets a valid location)

		- source=insideCells, interpolate=false:
			finds per triangle centre the cell containing it and uses its value.
			Trims triangles outside mesh.
		- source=insideCells, interpolate=true
			Per surface point interpolate cell containing it.

		- source=boundaryFaces, interpolate=false:
			finds per triangle centre the nearest point on the boundary
			(uncoupled faces only) and uses the value (or 0 if the nearest
			is on an empty boundary)
		- source=boundaryFaces, interpolate=true:
			finds per triangle centre the nearest point on the boundary
			(uncoupled faces only).
			Per surface point projects the point onto this boundary face
			(to make sure interpolateCellPoint gets a valid location)

	- since it finds a nearest per triangle each triangle is guaranteed
	to be on one processor only. So after stitching (by sampledSurfaces)
	the original surface should be complete.

SourceFiles
	sampledTriSurfaceMesh.C

\*---------------------------------------------------------------------------*/

#include <sampledSurface.hxx>
#include <triSurfaceMesh.hxx>
#include <MeshedSurface.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class treeDataFace;
	class meshSearch;

	namespace sampledSurfaces
	{

		/*---------------------------------------------------------------------------*\
							   Class triSurfaceMesh Declaration
		\*---------------------------------------------------------------------------*/

		class triSurfaceMesh
			:
			public sampledSurface,
			public MeshedSurface<face>
		{
		public:
			//- Types of communications
			enum samplingSource
			{
				cells,
				insideCells,
				boundaryFaces,
			};

		private:

			// Private Typedefs

			typedef MeshedSurface<face> MeshStorage;


			// Private Data

			static FoamSampling_EXPORT const NamedEnum<samplingSource, 3> samplingSourceNames_;

			//- Surface to sample on
			const tnbLib::triSurfaceMesh surface_;

			//- Whether to sample internal cell values or boundary values
			const samplingSource sampleSource_;

			//- Track if the surface needs an update
			mutable bool needsUpdate_;

			//- Search tree for all non-coupled boundary faces
			mutable autoPtr<indexedOctree<treeDataFace>> boundaryTreePtr_;

			//- From local surface triangle to mesh cell/face.
			labelList sampleElements_;

			//- Local points to sample per point
			pointField samplePoints_;


			// Private Member Functions

				//- Get tree of all non-coupled boundary faces
			FoamSampling_EXPORT const indexedOctree<treeDataFace>& nonCoupledboundaryTree() const;

			//- Sample field on faces
			template<class Type>
			tmp<Field<Type>> sampleField
			(
				const GeometricField<Type, fvPatchField, volMesh>& vField
			) const;


			template<class Type>
			tmp<Field<Type>>
				interpolateField(const interpolation<Type>&) const;

			FoamSampling_EXPORT bool update(const meshSearch& meshSearcher);

		public:

			//- Runtime type information
			//TypeName("triSurfaceMesh");
			static const char* typeName_() { return "triSurfaceMesh"; }
			static FoamSampling_EXPORT const ::tnbLib::word typeName;
			static FoamSampling_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			FoamSampling_EXPORT triSurfaceMesh
			(
				const word& name,
				const polyMesh& mesh,
				const word& surfaceName,
				const samplingSource sampleSource
			);

			//- Construct from dictionary
			FoamSampling_EXPORT triSurfaceMesh
			(
				const word& name,
				const polyMesh& mesh,
				const dictionary& dict
			);

			//- Construct from triSurface
			FoamSampling_EXPORT triSurfaceMesh
			(
				const word& name,
				const polyMesh& mesh,
				const triSurface& surface,
				const word& sampleSourceName
			);


			//- Destructor
			FoamSampling_EXPORT virtual ~triSurfaceMesh();


			// Member Functions

				//- Does the surface need an update?
			FoamSampling_EXPORT virtual bool needsUpdate() const;

			//- Mark the surface as needing an update.
			//  May also free up unneeded data.
			//  Return false if surface was already marked as expired.
			FoamSampling_EXPORT virtual bool expire();

			//- Update the surface as required.
			//  Do nothing (and return false) if no update was needed
			FoamSampling_EXPORT virtual bool update();

			//- Update the surface using a bound box to limit the searching.
			//  For direct use, i.e. not through sample.
			//  Do nothing (and return false) if no update was needed
			FoamSampling_EXPORT bool update(const treeBoundBox&);

			//- Points of surface
			virtual const pointField& points() const
			{
				return MeshStorage::points();
			}

			//- Faces of surface
			virtual const faceList& faces() const
			{
				return MeshStorage::faces();
			}


			//- Sample field on surface
			FoamSampling_EXPORT virtual tmp<scalarField> sample
			(
				const volScalarField&
			) const;

			//- Sample field on surface
			FoamSampling_EXPORT virtual tmp<vectorField> sample
			(
				const volVectorField&
			) const;

			//- Sample field on surface
			FoamSampling_EXPORT virtual tmp<sphericalTensorField> sample
			(
				const volSphericalTensorField&
			) const;

			//- Sample field on surface
			FoamSampling_EXPORT virtual tmp<symmTensorField> sample
			(
				const volSymmTensorField&
			) const;

			//- Sample field on surface
			FoamSampling_EXPORT virtual tmp<tensorField> sample
			(
				const volTensorField&
			) const;


			//- Interpolate field on surface
			FoamSampling_EXPORT virtual tmp<scalarField> interpolate
			(
				const interpolation<scalar>&
			) const;


			//- Interpolate field on surface
			FoamSampling_EXPORT virtual tmp<vectorField> interpolate
			(
				const interpolation<vector>&
			) const;

			//- Interpolate field on surface
			FoamSampling_EXPORT virtual tmp<sphericalTensorField> interpolate
			(
				const interpolation<sphericalTensor>&
			) const;

			//- Interpolate field on surface
			FoamSampling_EXPORT virtual tmp<symmTensorField> interpolate
			(
				const interpolation<symmTensor>&
			) const;

			//- Interpolate field on surface
			FoamSampling_EXPORT virtual tmp<tensorField> interpolate
			(
				const interpolation<tensor>&
			) const;

			//- Write
			FoamSampling_EXPORT virtual void print(Ostream&) const;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace sampledSurfaces
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <sampledTriSurfaceMeshI.hxx>

//#ifdef NoRepository
//#include <sampledTriSurfaceMeshTemplates.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_sampledTriSurfaceMesh_Header
