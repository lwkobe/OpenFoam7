#pragma once
#ifndef _pointPatchMapper_Header
#define _pointPatchMapper_Header

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
	tnbLib::pointPatchMapper

Description
	Mapping class for a pointPatchField.

SourceFiles
	pointPatchMapper.C

\*---------------------------------------------------------------------------*/

#include <pointMapper.hxx>
#include <pointPatchFieldMapper.hxx>
#include <pointPatch.hxx>
#include <primitiveFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of classes
	class pointPatch;
	class mapPolyMesh;

	/*---------------------------------------------------------------------------*\
						   Class pointPatchMapper Declaration
	\*---------------------------------------------------------------------------*/

	class pointPatchMapper
		:
		public generalPointPatchFieldMapper
	{
		// Private Data

			//- Reference to patch
		const pointPatch& patch_;

		//- Reference to point mapper for internal field
		const morphFieldMapper& pointMapper_;

		//- Reference to mapping data
		const mapPolyMesh& mpm_;

		//- Size before mapping
		const label sizeBeforeMapping_;


		// Demand-driven private data

		mutable bool hasUnmapped_;

		//- Direct addressing (only one for of addressing is used)
		mutable labelList* directAddrPtr_;

		//- Interpolated addressing (only one for of addressing is used)
		mutable labelListList* interpolationAddrPtr_;

		//- Interpolation weights
		mutable scalarListList* weightsPtr_;


		// Private Member Functions

			//- Calculate addressing for mapping with inserted cells
		FoamBase_EXPORT void calcAddressing() const;

		//- Clear out local storage
		FoamBase_EXPORT void clearOut();


	public:

		// Constructors

			//- Construct from mappers
		FoamBase_EXPORT pointPatchMapper
		(
			const pointPatch& patch,
			const pointMapper& pointMap,
			const mapPolyMesh& mpm
		);

		//- Disallow default bitwise copy construction
		FoamBase_EXPORT pointPatchMapper
		(
			const pointPatchMapper&
		);


		//- Destructor
		FoamBase_EXPORT virtual ~pointPatchMapper();


		// Member Functions

		virtual bool hasUnmapped() const
		{
			return hasUnmapped_;
		}

		//- Return size of field before mapping
		virtual label sizeBeforeMapping() const
		{
			return sizeBeforeMapping_;
		}

		//- Is the mapping direct
		virtual bool direct() const
		{
			return pointMapper_.direct();
		}

		//- Return direct addressing
		FoamBase_EXPORT virtual const labelUList& directAddressing() const;

		//- Return interpolated addressing
		FoamBase_EXPORT virtual const labelListList& addressing() const;

		//- Return interpolaion weights
		FoamBase_EXPORT virtual const scalarListList& weights() const;


		// Member Operators

			//- Disallow default bitwise assignment
		FoamBase_EXPORT void operator=(const pointPatchMapper&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_pointPatchMapper_Header
