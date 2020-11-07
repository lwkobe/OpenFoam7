#pragma once
#ifndef _cartesianCS_Header
#define _cartesianCS_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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
	tnbLib::cartesianCS

Description
	Cylindrical coordinate system

SourceFiles
	cartesianCS.C

\*---------------------------------------------------------------------------*/

#include <coordinateSystem.hxx>
#include <typeInfo.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class cartesianCS Declaration
	\*---------------------------------------------------------------------------*/

	class cartesianCS
		:
		public coordinateSystem
	{
	protected:

		// Protected Member Functions


			//- Convert from local coordinate system to the global Cartesian system
			//  with optional translation for the origin
		FoamFvMesh_EXPORT virtual vector localToGlobal(const vector&, bool translate) const;

		//- Convert from local coordinate system to the global Cartesian system
		//  with optional translation for the origin
		FoamFvMesh_EXPORT virtual tmp<vectorField> localToGlobal
		(
			const vectorField&,
			bool translate
		) const;

		//- Convert from global Cartesian system to the local coordinate system
		//  with optional translation for the origin
		FoamFvMesh_EXPORT virtual vector globalToLocal(const vector&, bool translate) const;

		//- Convert from global Cartesian system to the local coordinate system
		//  with optional translation for the origin
		FoamFvMesh_EXPORT virtual tmp<vectorField> globalToLocal
		(
			const vectorField&,
			bool translate
		) const;


	public:

		//- Runtime type information
		/*TypeName("cartesian");*/
		static const char* typeName_() { return "cartesian"; }
		static FoamFvMesh_EXPORT const ::tnbLib::word typeName;
		static FoamFvMesh_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct null
		FoamFvMesh_EXPORT cartesianCS();

		//- Construct copy
		FoamFvMesh_EXPORT cartesianCS
		(
			const coordinateSystem&
		);

		//- Construct copy with a different name
		FoamFvMesh_EXPORT cartesianCS
		(
			const word& name,
			const coordinateSystem&
		);

		//- Construct from origin and rotation
		FoamFvMesh_EXPORT cartesianCS
		(
			const word& name,
			const point& origin,
			const coordinateRotation&
		);

		//- Construct from origin and 2 axes
		FoamFvMesh_EXPORT cartesianCS
		(
			const word& name,
			const point& origin,
			const vector& axis,
			const vector& dirn
		);

		//- Construct from dictionary
		FoamFvMesh_EXPORT cartesianCS(const word&, const dictionary&);


		//- Construct from dictionary and objectRegistry
		FoamFvMesh_EXPORT cartesianCS(const objectRegistry&, const dictionary&);


		//- Destructor
		FoamFvMesh_EXPORT virtual ~cartesianCS();

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_cartesianCS_Header
