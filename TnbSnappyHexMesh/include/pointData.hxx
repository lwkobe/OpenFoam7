#pragma once
#ifndef _pointData_Header
#define _pointData_Header

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
	tnbLib::pointData

Description
	Variant of pointEdgePoint with some transported additional data.
	WIP - should be templated on data like wallDistData.
	Passive vector v_ is not a coordinate (so no enterDomain/leaveDomain
	transformation needed)

SourceFiles
	pointDataI.H
	pointData.C

\*---------------------------------------------------------------------------*/

#include <pointEdgePoint.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators

	class pointData;

	FoamSnappyHexMesh_EXPORT Istream& operator>>(Istream&, pointData&);
	FoamSnappyHexMesh_EXPORT Ostream& operator<<(Ostream&, const pointData&);


	/*---------------------------------------------------------------------------*\
							   Class pointData Declaration
	\*---------------------------------------------------------------------------*/

	class pointData
		:
		public pointEdgePoint
	{
		// Private Data

			//- Additional information.
		scalar s_;

		//- Additional information.
		vector v_;

	public:

		// Constructors

			//- Construct null
		inline pointData();

		//- Construct from origin, distance
		inline pointData
		(
			const point& origin,
			const scalar distSqr,
			const scalar s,
			const vector& v
		);


		// Member Functions

			// Access

		inline scalar s() const;

		inline const vector& v() const;


		// Needed by meshWave

			//- Apply rotation matrix to origin
		template<class TrackingData>
		inline void transform
		(
			const tensor& rotTensor,
			TrackingData& td
		);

		//- Influence of edge on point
		template<class TrackingData>
		inline bool updatePoint
		(
			const polyMesh& mesh,
			const label pointi,
			const label edgeI,
			const pointData& edgeInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of different value on same point.
		//  Merge new and old info.
		template<class TrackingData>
		inline bool updatePoint
		(
			const polyMesh& mesh,
			const label pointi,
			const pointData& newPointInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of different value on same point.
		//  No information about current position whatsoever.
		template<class TrackingData>
		inline bool updatePoint
		(
			const pointData& newPointInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of point on edge.
		template<class TrackingData>
		inline bool updateEdge
		(
			const polyMesh& mesh,
			const label edgeI,
			const label pointi,
			const pointData& pointInfo,
			const scalar tol,
			TrackingData& td
		);

		// Member Operators

			// Needed for List IO
		inline bool operator==(const pointData&) const;
		inline bool operator!=(const pointData&) const;


		// IOstream Operators

		friend FoamSnappyHexMesh_EXPORT Ostream& operator<<(Ostream&, const pointData&);
		friend FoamSnappyHexMesh_EXPORT Istream& operator>>(Istream&, pointData&);
	};


	//- Data associated with pointData as contiguous as pointEdgePoint
	template<>
	inline bool contiguous<pointData>()
	{
		return contiguous<pointEdgePoint>();
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <pointDataI.hxx>

#endif // !_pointData_Header
