#pragma once
#ifndef _directAMI_Header
#define _directAMI_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2013-2019 OpenFOAM Foundation
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
	tnbLib::directAMI

Description
	Direct mapped Arbitrary Mesh Interface (AMI) method

SourceFiles
	directAMI.C

\*---------------------------------------------------------------------------*/

#include <AMIMethod.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							  Class directAMI Declaration
	\*---------------------------------------------------------------------------*/

	class directAMI
		:
		public AMIMethod
	{
		// Private Member Functions

			// Marching front

				//- Append to list of src face seed indices
		void appendToDirectSeeds
		(
			labelList& mapFlag,
			labelList& srcTgtSeed,
			DynamicList<label>& srcSeeds,
			DynamicList<label>& nonOverlapFaces,
			label& srcFacei,
			label& tgtFacei
		) const;

		//- Restart the advancing front - typically happens for
		//  disconnected regions
		void restartAdvancingFront
		(
			labelList& mapFlag,
			DynamicList<label>& nonOverlapFaces,
			label& srcFacei,
			label& tgtFacei
		) const;


		// Evaluation

			//- Area of intersection between source and target faces
		scalar interArea
		(
			const label srcFacei,
			const label tgtFacei
		) const;


	public:

		//- Runtime type information
		TypeName("directAMI");


		// Constructors

			//- Construct from components
		directAMI
		(
			const primitivePatch& srcPatch,
			const primitivePatch& tgtPatch,
			const scalarField& srcMagSf,
			const scalarField& tgtMagSf,
			const faceAreaIntersect::triangulationMode& triMode,
			const bool reverseTarget = false,
			const bool requireMatch = true
		);

		//- Disallow default bitwise copy construction
		directAMI(const directAMI&) = delete;


		//- Destructor
		virtual ~directAMI();


		// Member Functions

			// Manipulation

				//- Update addressing and weights
		virtual void calculate
		(
			labelListList& srcAddress,
			scalarListList& srcWeights,
			labelListList& tgtAddress,
			scalarListList& tgtWeights,
			label srcFacei = -1,
			label tgtFacei = -1
		);


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const directAMI&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_directAMI_Header
