#pragma once
#ifndef _snapParameters_Header
#define _snapParameters_Header

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
	tnbLib::snapParameters

Description
	Simple container to keep together snap specific information.

SourceFiles
	snapParameters.C

\*---------------------------------------------------------------------------*/

#include <dictionary.hxx>
#include <scalar.hxx>
#include <Switch.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Class forward declarations

	/*---------------------------------------------------------------------------*\
							   Class snapParameters Declaration
	\*---------------------------------------------------------------------------*/

	class snapParameters
	{
		// Private Data

		const label nSmoothPatch_;

		const scalar snapTol_;

		const label nSmoothDispl_;

		const label nSnap_;

		const label nFeatureSnap_;

		const Switch explicitFeatureSnap_;

		const Switch implicitFeatureSnap_;

		const Switch multiRegionFeatureSnap_;

		const Switch detectNearSurfacesSnap_;


	public:

		// Constructors

			//- Construct from dictionary
		FoamSnappyHexMesh_EXPORT snapParameters(const dictionary& dict);

		//- Disallow default bitwise copy construction
		FoamSnappyHexMesh_EXPORT snapParameters(const snapParameters&) = delete;


		// Member Functions

			// Access

				//- Number of patch smoothing iterations before finding
				//  correspondence to surface
		label nSmoothPatch() const
		{
			return nSmoothPatch_;
		}

		//- Relative distance for points to be attracted by surface
		//  feature point
		//  or edge. True distance is this factor times local
		//  maximum edge length.
		scalar snapTol() const
		{
			return snapTol_;
		}

		//- Number of mesh displacement smoothing iterations.
		label nSmoothDispl() const
		{
			return nSmoothDispl_;
		}

		//- Maximum number of snapping relaxation iterations. Should stop
		//  before upon reaching a correct mesh.
		label nSnap() const
		{
			return nSnap_;
		}

		label nFeatureSnap() const
		{
			return nFeatureSnap_;
		}

		Switch explicitFeatureSnap() const
		{
			return explicitFeatureSnap_;
		}

		Switch implicitFeatureSnap() const
		{
			return implicitFeatureSnap_;
		}

		Switch multiRegionFeatureSnap() const
		{
			return multiRegionFeatureSnap_;
		}

		Switch detectNearSurfacesSnap() const
		{
			return detectNearSurfacesSnap_;
		}


		// Member Operators

			//- Disallow default bitwise assignment
		FoamSnappyHexMesh_EXPORT void operator=(const snapParameters&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_snapParameters_Header
