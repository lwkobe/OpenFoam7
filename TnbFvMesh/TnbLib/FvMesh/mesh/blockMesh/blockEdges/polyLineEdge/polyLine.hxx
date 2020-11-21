#pragma once
#ifndef _polyLine_Header
#define _polyLine_Header

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
	tnbLib::polyLine

Description
	A series of straight line segments, which can also be interpreted as
	a series of control points for splines, etc.

	A future implementation could also handle a closed polyLine.

SourceFiles
	polyLine.C

\*---------------------------------------------------------------------------*/

#include <pointField.hxx>
#include <scalarList.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							  Class polyLine Declaration
	\*---------------------------------------------------------------------------*/


	class polyLine
	{
	protected:

		// Protected data

			//- The control points or ends of each segments
		pointField points_;

		//- The real line length
		scalar lineLength_;

		//- The rational (0-1) cumulative parameter value for each point
		scalarList param_;


		// Protected Member Functions

			//- Precalculate the rational cumulative parameter value
			//  and the line-length
		FoamFvMesh_EXPORT void calcParam();

		//- Return the line segment and the local parameter [0..1]
		//  corresponding to the global lambda [0..1]
		FoamFvMesh_EXPORT label localParameter(scalar& lambda) const;


	public:

		// Constructors

			//- Construct from components
		FoamFvMesh_EXPORT polyLine
		(
			const pointField&,
			const bool notImplementedClosed = false
		);

		//- Disallow default bitwise copy construction
		FoamFvMesh_EXPORT polyLine(const polyLine&) = delete;


		// Member Functions

			//- Return const-access to the control-points
		FoamFvMesh_EXPORT const pointField& points() const;

		//- Return the number of line segments
		FoamFvMesh_EXPORT label nSegments() const;

		//- Return the point position corresponding to the curve parameter
		//  0 <= lambda <= 1
		FoamFvMesh_EXPORT point position(const scalar) const;

		//- Return the point position corresponding to the local parameter
		//  0 <= lambda <= 1 on the given segment
		FoamFvMesh_EXPORT point position(const label segment, const scalar) const;

		//- Return the length of the curve
		FoamFvMesh_EXPORT scalar length() const;


		// Member Operators

			//- Disallow default bitwise assignment
		FoamFvMesh_EXPORT void operator=(const polyLine&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_polyLine_Header
