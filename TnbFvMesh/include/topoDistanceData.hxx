#pragma once
#ifndef _topoDistanceData_Header
#define _topoDistanceData_Header

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
	tnbLib::topoDistanceData

Description
	For use with FaceCellWave. Determines topological distance to starting faces

SourceFiles
	topoDistanceDataI.H
	topoDistanceData.C

\*---------------------------------------------------------------------------*/

#include <point.hxx>
#include <tensor.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class polyPatch;
	class polyMesh;


	// Forward declaration of friend functions and operators

	class topoDistanceData;

	FoamFvMesh_EXPORT Istream& operator>>(Istream&, topoDistanceData&);
	FoamFvMesh_EXPORT Ostream& operator<<(Ostream&, const topoDistanceData&);


	/*---------------------------------------------------------------------------*\
							   Class topoDistanceData Declaration
	\*---------------------------------------------------------------------------*/

	class topoDistanceData
	{
		// Private Data

			//- Starting data
		label data_;

		//- Distance
		label distance_;


	public:

		// Constructors

			//- Construct null
		inline topoDistanceData();

		//- Construct from count
		inline topoDistanceData
		(
			const label data,
			const label distance
		);


		// Member Functions

			// Access


		inline label data() const
		{
			return data_;
		}
		inline label distance() const
		{
			return distance_;
		}


		// Needed by FaceCellWave

			//- Check whether origin has been changed at all or
			//  still contains original (invalid) value.
		template<class TrackingData>
		inline bool valid(TrackingData& td) const;

		//- Check for identical geometrical data. Used for cyclics checking.
		template<class TrackingData>
		inline bool sameGeometry
		(
			const polyMesh&,
			const topoDistanceData&,
			const scalar,
			TrackingData& td
		) const;

		//- Convert any absolute coordinates into relative to (patch)face
		//  centre
		template<class TrackingData>
		inline void leaveDomain
		(
			const polyMesh&,
			const polyPatch&,
			const label patchFacei,
			const point& faceCentre,
			TrackingData& td
		);

		//- Reverse of leaveDomain
		template<class TrackingData>
		inline void enterDomain
		(
			const polyMesh&,
			const polyPatch&,
			const label patchFacei,
			const point& faceCentre,
			TrackingData& td
		);

		//- Apply rotation matrix to any coordinates
		template<class TrackingData>
		inline void transform
		(
			const polyMesh&,
			const tensor&,
			TrackingData& td
		);

		//- Influence of neighbouring face.
		template<class TrackingData>
		inline bool updateCell
		(
			const polyMesh&,
			const label thisCelli,
			const label neighbourFacei,
			const topoDistanceData& neighbourInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of neighbouring cell.
		template<class TrackingData>
		inline bool updateFace
		(
			const polyMesh&,
			const label thisFacei,
			const label neighbourCelli,
			const topoDistanceData& neighbourInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of different value on same face.
		template<class TrackingData>
		inline bool updateFace
		(
			const polyMesh&,
			const label thisFacei,
			const topoDistanceData& neighbourInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Same (like operator==)
		template<class TrackingData>
		inline bool equal(const topoDistanceData&, TrackingData& td) const;

		// Member Operators

			// Needed for List IO
		inline bool operator==(const topoDistanceData&) const;

		inline bool operator!=(const topoDistanceData&) const;


		// IOstream Operators

		friend FoamFvMesh_EXPORT Ostream& operator<<(Ostream&, const topoDistanceData&);
		friend FoamFvMesh_EXPORT Istream& operator>>(Istream&, topoDistanceData&);
	};


	//- Data associated with topoDistanceData type are contiguous
	template<>
	inline bool contiguous<topoDistanceData>()
	{
		return true;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#include <topoDistanceDataI.hxx>

#endif // !_topoDistanceData_Header
