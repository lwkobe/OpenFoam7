#pragma once
#ifndef _InjectionModelTemplate_Header
#define _InjectionModelTemplate_Header

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
	tnbLib::InjectionModel

Description
	Templated injection model class.

	The injection model nominally describes the parcel:
	- position
	- diameter
	- velocity
	In this case, the fullyDescribed() flag should be set to 0 (false). When
	the parcel is then added to the cloud, the remaining properties are
	populated using values supplied in the constant properties.

	If, however, all of a parcel's properties are described in the model, the
	fullDescribed() flag should be set to 1 (true).


SourceFiles
	InjectionModel.C
	InjectionModelNew.C

\*---------------------------------------------------------------------------*/

#include <IOdictionary.hxx>
#include <autoPtr.hxx>
#include <runTimeSelectionTables.hxx>
#include <CloudSubModelBase.hxx>
#include <vector.hxx>
#include <TimeFunction1.hxx>

#ifdef FoamLagrangian_EXPORT_DEFINE
#define FoamInjectionModel_EXPORT __declspec(dllexport)
#else
#ifdef FoamInjectionModel_EXPORT_DEFINE
#define FoamInjectionModel_EXPORT __declspec(dllexport)
#else
#define FoamInjectionModel_EXPORT __declspec(dllimport)
#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class InjectionModel Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class InjectionModel
		:
		public CloudSubModelBase<CloudType>
	{
	public:

		//- Convenience typedef for parcelType
		typedef typename CloudType::parcelType parcelType;


		// Enumerations

			//- Parcel basis representation options
			//  i.e constant number of particles OR constant mass per parcel
		enum parcelBasis
		{
			pbNumber,
			pbMass,
			pbFixed
		};


	protected:

		// Protected data

			// Global injection properties

				//- Start of injection [s]
		scalar SOI_;

		//- Total volume of particles introduced by this injector [m^3]
		//  - scaled to ensure massTotal is achieved
		scalar volumeTotal_;

		//- Total mass to inject [kg]
		scalar massTotal_;

		//- Mass flow rate profile for steady calculations
		TimeFunction1<scalar> massFlowRate_;

		//- Total mass injected to date [kg]
		scalar massInjected_;


		// Counters

			//- Number of injections counter
		label nInjections_;

		//- Running counter of total number of parcels added
		label parcelsAddedTotal_;


		// Injection properties per Lagrangian time step

			//- Parcel basis enumeration
		parcelBasis parcelBasis_;

		//- nParticle to assign to parcels when the 'fixed' basis
		//  is selected
		scalar nParticleFixed_;

		//- Continuous phase time at start of injection time step [s]
		scalar time0_;

		//- Time at start of injection time step [s]
		scalar timeStep0_;


		// Protected Member Functions

			//- Additional flag to identify whether or not injection of parcelI is
			//  permitted
		virtual bool validInjection(const label parcelI) = 0;

		//- Determine properties for next time step/injection interval
		virtual bool prepareForNextTimeStep
		(
			const scalar time,
			label& newParcels,
			scalar& newVolumeFraction
		);

		//- Find the cell that contains the supplied position
		//  Will modify position slightly towards the owner cell centroid to
		//  ensure that it lies in a cell and not edge/face
		virtual bool findCellAtPosition
		(
			label& celli,
			label& tetFacei,
			label& tetPti,
			vector& position,
			bool errorOnNotFound = true
		);

		//- Set number of particles to inject given parcel properties
		virtual scalar setNumberOfParticles
		(
			const label parcels,
			const scalar volumeFraction,
			const scalar diameter,
			const scalar rho
		);

		//- Post injection checks
		virtual void postInjectCheck
		(
			const label parcelsAdded,
			const scalar massAdded
		);


	public:

		//- Runtime type information
		//TypeName("injectionModel");
		static const char* typeName_() { return "injectionModel"; }
		static FoamInjectionModel_EXPORT const ::tnbLib::word typeName;
		static FoamInjectionModel_EXPORT int debug;
		virtual const word& type() const { return typeName; };

		//- Declare runtime constructor selection table
		/*declareRunTimeSelectionTable
		(
			autoPtr,
			InjectionModel,
			dictionary,
			(
				const dictionary& dict,
				CloudType& owner,
				const word& modelType
				),
				(dict, owner, modelType)
		);*/
		
		typedef autoPtr<InjectionModel> (*dictionaryConstructorPtr)(const dictionary& dict, CloudType& owner,
		                                                            const word& modelType);
		typedef HashTable<dictionaryConstructorPtr, word, string::hash> dictionaryConstructorTable;
		static FoamInjectionModel_EXPORT dictionaryConstructorTable* dictionaryConstructorTablePtr_;
		static FoamInjectionModel_EXPORT void constructdictionaryConstructorTables();
		static FoamInjectionModel_EXPORT void destroydictionaryConstructorTables();

		template <class InjectionModelType>
		class adddictionaryConstructorToTable
		{
		public:
			static autoPtr<InjectionModel> New(const dictionary& dict, CloudType& owner, const word& modelType)
			{
				return autoPtr<InjectionModel>(new InjectionModelType(dict, owner, modelType));
			}

			adddictionaryConstructorToTable(const word& lookup = InjectionModelType::typeName)
			{
				constructdictionaryConstructorTables();
				if (!dictionaryConstructorTablePtr_->insert(lookup, New))
				{
					std::cerr << "Duplicate entry " << lookup << " in runtime selection table " << "InjectionModel" <<
						std::endl;
					error::safePrintStack(std::cerr);
				}
			}

			~adddictionaryConstructorToTable() { destroydictionaryConstructorTables(); }
		};

		template <class InjectionModelType>
		class addRemovabledictionaryConstructorToTable
		{
			const word& lookup_;
		public:
			static autoPtr<InjectionModel> New(const dictionary& dict, CloudType& owner, const word& modelType)
			{
				return autoPtr<InjectionModel>(new InjectionModelType(dict, owner, modelType));
			}

			addRemovabledictionaryConstructorToTable(const word& lookup = InjectionModelType::typeName) : lookup_(
				lookup)
			{
				constructdictionaryConstructorTables();
				dictionaryConstructorTablePtr_->set(lookup, New);
			}

			~addRemovabledictionaryConstructorToTable()
			{
				if (dictionaryConstructorTablePtr_) { dictionaryConstructorTablePtr_->erase(lookup_); }
			}
		};


		// Constructors

			//- Construct null from owner
		InjectionModel(CloudType& owner);

		//- Construct from dictionary
		InjectionModel
		(
			const dictionary& dict,
			CloudType& owner,
			const word& modelName,
			const word& modelType
		);

		//- Construct copy
		InjectionModel(const InjectionModel<CloudType>& im);

		//- Construct and return a clone
		virtual autoPtr<InjectionModel<CloudType>> clone() const = 0;


		//- Destructor
		virtual ~InjectionModel();


		// Selectors

			//- Selector with lookup from dictionary
		static autoPtr<InjectionModel<CloudType>> New
		(
			const dictionary& dict,
			CloudType& owner
		);

		//- Selector with name and type
		static autoPtr<InjectionModel<CloudType>> New
		(
			const dictionary& dict,
			const word& modelName,
			const word& modelType,
			CloudType& owner
		);


		// Member Functions

			// Mapping

				//- Update mesh
		virtual void updateMesh();


		// Global information

			//- Return the start-of-injection time
		inline scalar timeStart() const;

		//- Return the total volume to be injected across the event
		inline scalar volumeTotal() const;

		//- Return mass of particles to introduce
		inline scalar massTotal() const;

		//- Return mass of particles injected (cumulative)
		inline scalar massInjected() const;

		//- Return the end-of-injection time
		virtual scalar timeEnd() const = 0;

		//- Number of parcels to introduce relative to SOI
		virtual label parcelsToInject
		(
			const scalar time0,
			const scalar time1
		) = 0;

		//- Volume of parcels to introduce relative to SOI
		virtual scalar volumeToInject
		(
			const scalar time0,
			const scalar time1
		) = 0;

		//- Return the average parcel mass over the injection period
		virtual scalar averageParcelMass();


		// Counters

			//- Return the number of injections
		inline label nInjections() const;

		//- Return the total number parcels added
		inline label parcelsAddedTotal() const;


		// Per-injection event functions

			//- Main injection loop
		template<class TrackCloudType>
		void inject
		(
			TrackCloudType& cloud,
			typename CloudType::parcelType::trackingData& td
		);

		//- Main injection loop - steady-state
		template<class TrackCloudType>
		void injectSteadyState
		(
			TrackCloudType& cloud,
			typename CloudType::parcelType::trackingData& td,
			const scalar trackTime
		);


		// Injection geometry

			//- Set the injection position and owner cell, tetFace and tetPt
		virtual void setPositionAndCell
		(
			const label parcelI,
			const label nParcels,
			const scalar time,
			vector& position,
			label& cellOwner,
			label& tetFacei,
			label& tetPti
		) = 0;

		//- Set the parcel properties
		virtual void setProperties
		(
			const label parcelI,
			const label nParcels,
			const scalar time,
			parcelType& parcel
		) = 0;

		//- Flag to identify whether model fully describes the parcel
		virtual bool fullyDescribed() const = 0;


		// I-O

			//- Write injection info to stream
		virtual void info(Ostream& os);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeInjectionModel(CloudType)                                          \
                                                                               \
    typedef tnbLib::CloudType::kinematicCloudType kinematicCloudType;            \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        tnbLib::InjectionModel<kinematicCloudType>,                              \
        0                                                                      \
    );                                                                         \
                                                                               \
    namespace tnbLib                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            InjectionModel<kinematicCloudType>,                                \
            dictionary                                                         \
        );                                                                     \
    }


#define makeInjectionModelType(SS, CloudType)                                  \
                                                                               \
    typedef tnbLib::CloudType::kinematicCloudType kinematicCloudType;            \
    defineNamedTemplateTypeNameAndDebug(tnbLib::SS<kinematicCloudType>, 0);      \
                                                                               \
    tnbLib::InjectionModel<kinematicCloudType>::                                 \
        adddictionaryConstructorToTable<tnbLib::SS<kinematicCloudType>>          \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <InjectionModelTemplateI.hxx>
#include <InjectionModelTemplate_Imp.hxx>
#include <InjectionModelTemplateNew_Imp.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#ifdef NoRepository
//#include <InjectionModelTemplate.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_InjectionModelTemplate_Header
