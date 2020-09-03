#pragma once
#ifndef _PairCollisionRecord_Header
#define _PairCollisionRecord_Header

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
	tnbLib::PairCollisionRecord

Description

	Record of a collision between the particle holding the record and
	the particle with the stored id.

	The access status of the record is to be coded in the
	origProcOfOther data member.  The actual processor is offset by
	+1.  A negative value means that the record has not been accessed,
	positive means that it has.

SourceFiles
	PairCollisionRecordI.H
	PairCollisionRecord.C
	PairCollisionRecordIO.C

\*---------------------------------------------------------------------------*/

#include <label.hxx>
#include <vector.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators
	template<class Type>
	class PairCollisionRecord;

	template<class Type>
	inline bool operator==
		(
			const PairCollisionRecord<Type>&,
			const PairCollisionRecord<Type>&
			);

	template<class Type>
	inline bool operator!=
		(
			const PairCollisionRecord<Type>&,
			const PairCollisionRecord<Type>&
			);

	template<class Type>
	Istream& operator>>(Istream&, PairCollisionRecord<Type>&);

	template<class Type>
	Ostream& operator<<(Ostream&, const PairCollisionRecord<Type>&);


	/*---------------------------------------------------------------------------*\
							 Class PairCollisionRecord Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class PairCollisionRecord
	{
		// Private Data

			//- Originating processor id of other collision partner
		label origProcOfOther_;

		//- Local particle id on originating processor of other
		//  collision partner
		label origIdOfOther_;

		//- Collision data, stored as if the storing particle was the
		//  first particle (particle A) in the collision.
		Type data_;


	public:

		// Constructors

			//- Construct null
		PairCollisionRecord();

		//- Construct from components
		PairCollisionRecord
		(
			bool accessed,
			label origProcOfOther,
			label origIdOfOther,
			const Type& data = pTraits<Type>::zero
		);

		//- Construct from Istream
		PairCollisionRecord(Istream&);

		//- Copy constructor
		PairCollisionRecord(const PairCollisionRecord&);


		//- Destructor
		~PairCollisionRecord();


		// Member Functions


			// Access

				//- Return the origProcOfOther data
		inline label origProcOfOther() const;

		//- Return the origIdOfOther data
		inline label origIdOfOther() const;

		//- Return access to the collision data
		inline const Type& collisionData() const;

		//- Return access to the collision data
		inline Type& collisionData();


		// Check

		inline bool match
		(
			label queryOrigProcOfOther,
			label queryOrigIdOfOther
		) const;

		//- Return the accessed status of the record
		inline bool accessed() const;


		// Edit

			//- Set the accessed property of the record to accessed
		inline void setAccessed();

		//- Set the accessed property of the record to unaccessed
		inline void setUnaccessed();


		// Member Operators

		void operator=(const PairCollisionRecord&);


		// Friend Operators

		friend bool operator== <Type>
			(
				const PairCollisionRecord<Type>&,
				const PairCollisionRecord<Type>&
				);

		friend bool operator!= <Type>
			(
				const PairCollisionRecord<Type>&,
				const PairCollisionRecord<Type>&
				);


		// IOstream Operators

		friend Istream& operator>> <Type>
			(
				Istream&,
				PairCollisionRecord<Type>&
				);

		friend Ostream& operator<< <Type>
			(
				Ostream&,
				const PairCollisionRecord<Type>&
				);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <PairCollisionRecordI.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <PairCollisionRecord.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_PairCollisionRecord_Header