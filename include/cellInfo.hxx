#pragma once
#ifndef _cellInfo_Header
#define _cellInfo_Header

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
	tnbLib::cellInfo

Description
	Holds information regarding type of cell. Used in inside/outside
	determination in cellClassification.

SourceFiles
	cellInfoI.H
	cellInfo.C

\*---------------------------------------------------------------------------*/

#include <point.hxx>
#include <label.hxx>
#include <tensor.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	class polyPatch;
	class polyMesh;


	// Forward declaration of friend functions and operators

	class cellInfo;

	FoamFvMesh_EXPORT Istream& operator>>(Istream&, cellInfo&);
	FoamFvMesh_EXPORT Ostream& operator<<(Ostream&, const cellInfo&);


	/*---------------------------------------------------------------------------*\
							   Class cellInfo Declaration
	\*---------------------------------------------------------------------------*/

	class cellInfo
	{
		// Private Data

		label type_;

		// Private Member Functions

			//- Update current cell/face type with neighbouring
			//  type. Return true if information needs to propagate,
			//  false otherwise.
		template<class TrackingData>
		inline bool update
		(
			const cellInfo& w2,
			const label thisFacei,
			const label thisCelli,
			const label neighbourFacei,
			const label neighbourCelli,
			TrackingData& td
		);

	public:

		// Constructors

			//- Construct null
		inline cellInfo();

		//- Construct from cType
		inline cellInfo(const label);


		// Member Functions

			// Access

		inline label type() const
		{
			return type_;
		}


		// Needed by meshWave

			//- Check whether origin has been changed at all or
			//  still contains original (invalid) value.
		template<class TrackingData>
		inline bool valid(TrackingData& td) const;

		//- Check for identical geometrical data. Used for cyclics checking.
		template<class TrackingData>
		inline bool sameGeometry
		(
			const polyMesh&,
			const cellInfo&,
			const scalar,
			TrackingData& td
		) const;

		//- Convert any absolute coordinates into relative to (patch)face
		//  centre
		template<class TrackingData>
		inline void leaveDomain
		(
			const polyMesh&,
			const polyPatch& patch,
			const label patchFacei,
			const point& faceCentre,
			TrackingData& td
		);

		//- Reverse of leaveDomain
		template<class TrackingData>
		inline void enterDomain
		(
			const polyMesh&,
			const polyPatch& patch,
			const label patchFacei,
			const point& faceCentre,
			TrackingData& td
		);

		//- Apply rotation matrix to any coordinates
		template<class TrackingData>
		inline void transform
		(
			const polyMesh&,
			const tensor& rotTensor,
			TrackingData& td
		);

		//- Influence of neighbouring face.
		template<class TrackingData>
		inline bool updateCell
		(
			const polyMesh&,
			const label thisCelli,
			const label neighbourFacei,
			const cellInfo& neighbourInfo,
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
			const cellInfo& neighbourInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Influence of different value on same face.
		template<class TrackingData>
		inline bool updateFace
		(
			const polyMesh&,
			const label thisFacei,
			const cellInfo& neighbourInfo,
			const scalar tol,
			TrackingData& td
		);

		//- Same (like operator==)
		template<class TrackingData>
		inline bool equal(const cellInfo&, TrackingData& td) const;

		// Member Operators

			// Note: Used to determine whether to call update.
		inline bool operator==(const cellInfo&) const;

		inline bool operator!=(const cellInfo&) const;


		// IOstream Operators

		friend FoamFvMesh_EXPORT Ostream& operator<<(Ostream&, const cellInfo&);
		friend FoamFvMesh_EXPORT Istream& operator>>(Istream&, cellInfo&);
	};


	//- Data associated with cellInfo type are contiguous
	template<>
	inline bool contiguous<cellInfo>()
	{
		return true;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <cellInfoI.hxx>

#endif // !_cellInfo_Header
