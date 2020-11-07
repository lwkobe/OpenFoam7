#pragma once
#ifndef _MeshedSurfaceProxy_Header
#define _MeshedSurfaceProxy_Header

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
	tnbLib::MeshedSurfaceProxy

Description
	A proxy for writing MeshedSurface, UnsortedMeshedSurface and surfMesh
	to various file formats.

SourceFiles
	MeshedSurfaceProxy.C
	MeshedSurfaceProxyCore.C

\*---------------------------------------------------------------------------*/

#include <pointField.hxx>
#include <face.hxx>
#include <triFace.hxx>

#include <surfZoneList.hxx>
#include <surfaceFormatsCore.hxx>
#include <runTimeSelectionTables.hxx>
#include <memberFunctionSelectionTables.hxx>
#include <HashSet.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators

	template<class Face> class MeshedSurface;

	/*---------------------------------------------------------------------------*\
						 Class MeshedSurfaceProxy Declaration
	\*---------------------------------------------------------------------------*/

	template<class Face>
	class MeshedSurfaceProxy
		:
		public fileFormats::surfaceFormatsCore
	{
		// Private Data

		const pointField& points_;

		const List<Face>& faces_;

		const List<surfZone>& zones_;

		const List<label>& faceMap_;


	public:

		// Public Typedefs

			//- Face type used
		typedef Face FaceType;


		// Static

			//- Runtime type information
		/*ClassName("MeshedSurfaceProxy");*/
		static const char* typeName_() { return "MeshedSurfaceProxy"; }
		static FoamFvMesh_EXPORT const ::tnbLib::word typeName;
		static FoamFvMesh_EXPORT int debug;

		//- The file format types that can be written via MeshedSurfaceProxy
		static wordHashSet writeTypes();

		//- Can this file format type be written via MeshedSurfaceProxy?
		static bool canWriteType(const word& ext, const bool verbose = false);


		// Constructors

			//- Construct from component references
		MeshedSurfaceProxy
		(
			const pointField&,
			const List<Face>&,
			const List<surfZone>& = List<surfZone>(),
			const List<label>& faceMap = List<label>()
		);


		//- Destructor
		virtual ~MeshedSurfaceProxy();


		// Member Function Selectors

		/*declareMemberFunctionSelectionTable
		(
			void,
			MeshedSurfaceProxy,
			write,
			fileExtension,
			(
				const fileName& name,
				const MeshedSurfaceProxy<Face>& surf
				),
				(name, surf)
		);*/

		typedef void (*writefileExtensionMemberFunctionPtr)(const fileName& name, const MeshedSurfaceProxy<Face>& surf);
		typedef HashTable<writefileExtensionMemberFunctionPtr, word, string::hash> writefileExtensionMemberFunctionTable;
		static FoamFvMesh_EXPORT writefileExtensionMemberFunctionTable* writefileExtensionMemberFunctionTablePtr_;

		template <class MeshedSurfaceProxyType>
		class addwritefileExtensionMemberFunctionToTable
		{
		public:
			addwritefileExtensionMemberFunctionToTable(const word& lookup = MeshedSurfaceProxyType::typeName)
			{
				constructwritefileExtensionMemberFunctionTables();
				writefileExtensionMemberFunctionTablePtr_->insert(lookup, MeshedSurfaceProxyType::write);
			}

			~addwritefileExtensionMemberFunctionToTable() { destroywritefileExtensionMemberFunctionTables(); }
		};

		static FoamFvMesh_EXPORT void constructwritefileExtensionMemberFunctionTables();
		static FoamFvMesh_EXPORT void destroywritefileExtensionMemberFunctionTables();

		//- Write to file
		static void write(const fileName&, const MeshedSurfaceProxy<Face>&);


		// Member Functions

			// Access

				//- Return const access to the points
		inline const pointField& points() const
		{
			return points_;
		}

		//- Return const access to the faces
		inline const List<Face>& faces() const
		{
			return faces_;
		}

		//- Const access to the surface zones.
		//  If zones are defined, they must be contiguous and cover the
		//  entire surface
		inline const List<surfZone>& surfZones() const
		{
			return zones_;
		}

		//- Const access to the faceMap, zero-sized when unused
		inline const List<label>& faceMap() const
		{
			return faceMap_;
		}

		//- Use faceMap?
		inline bool useFaceMap() const
		{
			return faceMap_.size() == faces_.size();
		}

		// Write

			//- Generic write routine. Chooses writer based on extension.
		virtual void write(const fileName& name) const
		{
			write(name, *this);
		}

		//- Write to database
		virtual void write(const Time&, const word& surfName = "") const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <MeshedSurfaceProxyI.hxx>

//#ifdef NoRepository
//#include <MeshedSurfaceProxy.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_MeshedSurfaceProxy_Header
