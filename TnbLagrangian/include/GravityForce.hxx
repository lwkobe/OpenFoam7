#pragma once
#ifndef _GravityForce_Header
#define _GravityForce_Header

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
	tnbLib::GravityForce

Description
	Calculates particle gravity force

SourceFiles
	GravityForceI.H
	GravityForce.C

\*---------------------------------------------------------------------------*/

#include <ParticleForce.hxx>

#ifdef FoamLagrangian_EXPORT_DEFINE
#define FoamGravityForce_EXPORT __declspec(dllexport)
#else
#ifdef FoamGravityForce_EXPORT_DEFINE
#define FoamGravityForce_EXPORT __declspec(dllexport)
#else
#define FoamGravityForce_EXPORT __declspec(dllimport)
#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class fvMesh;

	/*---------------------------------------------------------------------------*\
						   Class GravityForce Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class GravityForce
		:
		public ParticleForce<CloudType>
	{
		// Private Data

			//- Reference to the acceleration due to gravity
		const vector& g_;


	public:

		//- Runtime type information
		//TypeName("gravity");
		static const char* typeName_() { return "gravity"; }
		static FoamGravityForce_EXPORT const ::tnbLib::word typeName;
		static FoamGravityForce_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from mesh
		GravityForce
		(
			CloudType& owner,
			const fvMesh& mesh,
			const dictionary& dict
		);

		//- Construct copy
		GravityForce(const GravityForce& gf);

		//- Construct and return a clone
		virtual autoPtr<ParticleForce<CloudType>> clone() const
		{
			return autoPtr<ParticleForce<CloudType>>
				(
					new GravityForce<CloudType>(*this)
					);
		}


		//- Destructor
		virtual ~GravityForce();


		// Member Functions

			// Access

				//- Return the acceleration due to gravity
		inline const vector& g() const;


		// Evaluation

			//- Calculate the non-coupled force
		virtual forceSuSp calcNonCoupled
		(
			const typename CloudType::parcelType& p,
			const typename CloudType::parcelType::trackingData& td,
			const scalar dt,
			const scalar mass,
			const scalar Re,
			const scalar muc
		) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <GravityForceI.hxx>
#include <GravityForce_Imp.hxx>

//#ifdef NoRepository
//#include <GravityForce.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_GravityForce_Header
