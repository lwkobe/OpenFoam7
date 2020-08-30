#pragma once
#ifndef _BreakupModel_Header
#define _BreakupModel_Header

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
	tnbLib::BreakupModel

Description
	Templated break-up model class

SourceFiles
	BreakupModel.C
	BreakupModelNew.C

\*---------------------------------------------------------------------------*/

#include <IOdictionary.hxx>
#include <autoPtr.hxx>
#include <runTimeSelectionTables.hxx>

#include <CloudSubModelBase.hxx>  // added by amir
#include <Switch.hxx>  // added by amir
#include <vector.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class BreakupModel Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class BreakupModel
		:
		public CloudSubModelBase<CloudType>
	{

	protected:

		// Protected data

		Switch solveOscillationEq_;

		scalar y0_;
		scalar yDot0_;

		scalar TABComega_;
		scalar TABCmu_;
		scalar TABtwoWeCrit_;


	public:

		//- Runtime type information
		TypeName("breakupModel");

		//- Declare runtime constructor selection table
		declareRunTimeSelectionTable
		(
			autoPtr,
			BreakupModel,
			dictionary,
			(
				const dictionary& dict,
				CloudType& owner
				),
				(dict, owner)
		);


		// Constructors

			//- Construct null from owner
		BreakupModel(CloudType& owner);

		//- Construct from dictionary
		BreakupModel
		(
			const dictionary& dict,
			CloudType& owner,
			const word& type,
			bool solveOscillationEq = false
		);

		//- Construct copy
		BreakupModel(const BreakupModel<CloudType>& bum);

		//- Construct and return a clone
		virtual autoPtr<BreakupModel<CloudType>> clone() const = 0;


		//- Destructor
		virtual ~BreakupModel();


		//- Selector
		static autoPtr<BreakupModel<CloudType>> New
		(
			const dictionary& dict,
			CloudType& owner
		);


		// Access

		inline const Switch& solveOscillationEq() const
		{
			return solveOscillationEq_;
		}

		inline const scalar& y0() const
		{
			return y0_;
		}

		inline const scalar& yDot0() const
		{
			return yDot0_;
		}

		inline const scalar& TABComega() const
		{
			return TABComega_;
		}

		inline const scalar& TABCmu() const
		{
			return TABCmu_;
		}

		inline const scalar& TABtwoWeCrit() const
		{
			return TABtwoWeCrit_;
		}


		// Member Functions

			//- Update the parcel properties and return true if a child parcel
			//  should be added
		virtual bool update
		(
			const scalar dt,
			const vector& g,
			scalar& d,
			scalar& tc,
			scalar& ms,
			scalar& nParticle,
			scalar& KHindex,
			scalar& y,
			scalar& yDot,
			const scalar d0,
			const scalar rho,
			const scalar mu,
			const scalar sigma,
			const vector& U,
			const scalar rhoc,
			const scalar muc,
			const vector& Urel,
			const scalar Urmag,
			const scalar tMom,
			scalar& dChild,
			scalar& massChild
		) = 0;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBreakupModel(CloudType)                                            \
                                                                               \
    typedef tnbLib::CloudType::sprayCloudType sprayCloudType;                    \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        tnbLib::BreakupModel<sprayCloudType>,                                    \
        0                                                                      \
    );                                                                         \
                                                                               \
    namespace tnbLib                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            BreakupModel<sprayCloudType>,                                      \
            dictionary                                                         \
        );                                                                     \
    }


#define makeBreakupModelType(SS, CloudType)                                    \
                                                                               \
    typedef tnbLib::CloudType::sprayCloudType sprayCloudType;                    \
    defineNamedTemplateTypeNameAndDebug(tnbLib::SS<sprayCloudType>, 0);          \
                                                                               \
    tnbLib::BreakupModel<sprayCloudType>::                                       \
        adddictionaryConstructorToTable<tnbLib::SS<sprayCloudType>>              \
            add##SS##CloudType##sprayCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <BreakupModel.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_BreakupModel_Header
