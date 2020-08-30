#pragma once
#ifndef _VariableHardSphere_Header
#define _VariableHardSphere_Header

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
	tnbLib::VariableHardSphere

Description
	Variable Hard Sphere BinaryCollision Model

\*---------------------------------------------------------------------------*/

#include <BinaryCollisionModel.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	/*---------------------------------------------------------------------------*\
						  Class VariableHardSphere Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class VariableHardSphere
		:
		public BinaryCollisionModel<CloudType>
	{
		// Private Data

			//- Reference temperature
		const scalar Tref_;


	public:

		//- Runtime type information
		TypeName("VariableHardSphere");


		// Constructors

			//- Construct from dictionary
		VariableHardSphere
		(
			const dictionary& dict,
			CloudType& cloud
		);


		//- Destructor
		virtual ~VariableHardSphere();


		// Member Functions

			//- Flag to indicate whether model activates collision model
		virtual bool active() const;

		//- Return the collision cross section * relative velocity product
		virtual scalar sigmaTcR
		(
			const typename CloudType::parcelType& pP,
			const typename CloudType::parcelType& pQ
		) const;

		//- Apply collision
		virtual void collide
		(
			typename CloudType::parcelType& pP,
			typename CloudType::parcelType& pQ
		);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <VariableHardSphere.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_VariableHardSphere_Header
