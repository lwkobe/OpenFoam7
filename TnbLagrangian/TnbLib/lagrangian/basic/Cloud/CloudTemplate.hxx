#pragma once
#ifndef _CloudTemplate_Header
#define _CloudTemplate_Header

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
	tnbLib::Cloud

Description
	Base cloud calls templated on particle type

SourceFiles
	Cloud.C
	CloudIO.C

\*---------------------------------------------------------------------------*/

#include <cloud.hxx>
#include <IDLList.hxx>
#include <IOField.hxx>
#include <CompactIOField.hxx>
#include <polyMesh.hxx>
#include <PackedBoolList.hxx>

#ifdef FoamLagrangian_EXPORT_DEFINE
#define FoamCloud_EXPORT __declspec(dllexport)
#else
#ifdef FoamCloud_EXPORT_DEFINE
#define FoamCloud_EXPORT __declspec(dllexport)
#else
#define FoamCloud_EXPORT __declspec(dllimport)
#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of functions
	template<class ParticleType>
	class Cloud;

	template<class ParticleType>
	class IOPosition;

	template<class ParticleType>
	Ostream& operator<<
		(
			Ostream&,
			const Cloud<ParticleType>&
			);


	/*---------------------------------------------------------------------------*\
							   Class Cloud Declaration
	\*---------------------------------------------------------------------------*/

	template<class ParticleType>
	class Cloud
		:
		public cloud,
		public IDLList<ParticleType>
	{
		// Private Data

			//- Reference to the mesh
		const polyMesh& polyMesh_;

		//- Temporary storage for the global particle positions
		mutable autoPtr<vectorField> globalPositionsPtr_;


		// Private Member Functions

			//- Check patches
		void checkPatches() const;

		//- Initialise cloud on IO constructor
		void initCloud(const bool checkClass);

		//- Read cloud properties dictionary
		void readCloudUniformProperties();

		//- Write cloud properties dictionary
		void writeCloudUniformProperties() const;


	public:

		friend class particle;
		template<class ParticleT>
		friend class IOPosition;

		typedef ParticleType particleType;

		typedef typename IDLList<ParticleType>::iterator iterator;
		typedef typename IDLList<ParticleType>::const_iterator const_iterator;

		//-Runtime type information
		//TypeName("Cloud");
		static const char* typeName_() { return "Cloud"; }
		static FoamCloud_EXPORT const ::tnbLib::word typeName;
		static FoamCloud_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Static data

			//- Name of cloud properties dictionary
		static word cloudPropertiesName;


		// Constructors

			//- Construct from mesh and a list of particles
		Cloud
		(
			const polyMesh& mesh,
			const word& cloudName,
			const IDLList<ParticleType>& particles
		);

		//- Construct from mesh by reading from file with given cloud instance
		//  Optionally disable checking of class name for post-processing
		Cloud
		(
			const polyMesh& pMesh,
			const word& cloudName,
			const bool checkClass = true
		);


		// Member Functions

			// Access

				//- Return the polyMesh reference
		const polyMesh& pMesh() const
		{
			return polyMesh_;
		}

		//- Return the number of particles in the cloud
		label size() const
		{
			return IDLList<ParticleType>::size();
		};


		// Iterators

		const const_iterator begin() const
		{
			return IDLList<ParticleType>::begin();
		};

		const const_iterator cbegin() const
		{
			return IDLList<ParticleType>::cbegin();
		};

		const const_iterator end() const
		{
			return IDLList<ParticleType>::end();
		};

		const const_iterator cend() const
		{
			return IDLList<ParticleType>::cend();
		};

		iterator begin()
		{
			return IDLList<ParticleType>::begin();
		};

		iterator end()
		{
			return IDLList<ParticleType>::end();
		};


		// Edit

		void clear()
		{
			IDLList<ParticleType>::clear();
		};

		//- Transfer particle to cloud
		void addParticle(ParticleType* pPtr);

		//- Remove particle from cloud and delete
		void deleteParticle(ParticleType&);

		//- Remove lost particles from cloud and delete
		void deleteLostParticles();

		//- Reset the particles
		void cloudReset(const Cloud<ParticleType>& c);

		//- Move the particles
		template<class TrackCloudType>
		void move
		(
			TrackCloudType& cloud,
			typename ParticleType::trackingData& td,
			const scalar trackTime
		);

		//- Remap the cells of particles corresponding to the
		//  mesh topology change
		void autoMap(const mapPolyMesh&);


		// Read

			//- Helper to construct IOobject for field and current time.
		IOobject fieldIOobject
		(
			const word& fieldName,
			const IOobject::readOption r
		) const;

		//- Check lagrangian data field
		template<class DataType>
		void checkFieldIOobject
		(
			const Cloud<ParticleType>& c,
			const IOField<DataType>& data
		) const;

		//- Check lagrangian data fieldfield
		template<class DataType>
		void checkFieldFieldIOobject
		(
			const Cloud<ParticleType>& c,
			const CompactIOField<Field<DataType>, DataType>& data
		) const;


		// Write

			//- Write the field data for the cloud of particles Dummy at
			//  this level.
		virtual void writeFields() const;

		//- Write using given format, version and compression.
		//  Only writes the cloud file if the Cloud isn't empty
		virtual bool writeObject
		(
			IOstream::streamFormat fmt,
			IOstream::versionNumber ver,
			IOstream::compressionType cmp,
			const bool write = true
		) const;

		//- Write positions to \<cloudName\>_positions.obj file
		void writePositions() const;

		//- Call this before a topology change. Stores the particles global
		//  positions in the database for use during mapping.
		void storeGlobalPositions() const;


		// Ostream Operator

		friend Ostream& operator<< <ParticleType>
			(
				Ostream&,
				const Cloud<ParticleType>&
				);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <CloudTemplateI.hxx>
#include <CloudTemplateIO_Imp.hxx>

//#ifdef NoRepository
//#include <CloudTemplate.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif // !_CloudTemplate_Header
