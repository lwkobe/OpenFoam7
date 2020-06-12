#pragma once
#ifndef _regIOobject_Header
#define _regIOobject_Header

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
	tnbLib::regIOobject

Description
	regIOobject is an abstract class derived from IOobject to handle
	automatic object registration with the objectRegistry.

SourceFiles
	regIOobject.C
	regIOobjectRead.C
	regIOobjectWrite.C

\*---------------------------------------------------------------------------*/

#include <IOobject.hxx>
#include <typeInfo.hxx>
#include <OSspecific.hxx>
#include <NamedEnum.hxx>
#include <labelList.hxx> // added by amir
#include <ISstream.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							 Class regIOobject Declaration
	\*---------------------------------------------------------------------------*/

	class regIOobject
		:
		public IOobject
	{

	protected:

		//- Helper: check readOpt flags and read if necessary
		bool readHeaderOk
		(
			const IOstream::streamFormat PstreamFormat,
			const word& typeName
		);


	private:

		// Private Data

			//- Is this object registered with the registry
		bool registered_;

		//- Is this object owned by the registry
		bool ownedByRegistry_;

		//- List of modification watch indices
		mutable labelList watchIndices_;

		//- eventNo of last update
		label eventNo_;

		//- Istream for reading
		autoPtr<ISstream> isPtr_;


		// Private Member Functions

			//- Return Istream
		Istream& readStream(const bool read = true);

		//- Dissallow assignment
		void operator=(const regIOobject&);


	public:

		// Static data

			//- Runtime type information
		TypeName("regIOobject");

		static float fileModificationSkew;


		// Constructors

			//- Construct from IOobject. Optional flag for if IOobject is the
			//  top level regIOobject.
		regIOobject(const IOobject&, const bool isTime = false);

		//- Copy constructor
		regIOobject(const regIOobject&);

		//- Copy constructor, transferring registry registration to copy
		//  if registerCopy is true
		regIOobject(const regIOobject&, bool registerCopy);

		//- Copy constructor with new name, transferring registry registration
		//  to copy as specified
		regIOobject(const word& newName, const regIOobject&, bool registerCopy);

		//- Copy constructor with new IO parameters
		regIOobject(const IOobject&, const regIOobject&);


		//- Destructor
		virtual ~regIOobject();


		// Member Functions

			// Registration

				//- Add object to registry
		bool checkIn();

		//- Remove object from registry
		bool checkOut();

		//- Add file watch on object (if registered and READ_IF_MODIFIED)
		virtual void addWatch();

		//- Is this object owned by the registry?
		inline bool ownedByRegistry() const;

		//- Transfer ownership of this object to its registry
		inline void store();

		//- Transfer ownership of the given object pointer to its registry
		//  and return reference to object.
		template<class Type>
		inline static Type& store(Type*);

		//- Transfer ownership of the given object pointer to its registry
		//  and return reference to object.
		template<class Type>
		inline static Type& store(autoPtr<Type>&);

		//- Release ownership of this object from its registry
		inline void release();


		// Dependency checking

			//- Event number at last update.
		inline label eventNo() const;

		//- Event number at last update.
		inline label& eventNo();

		//- Return true if up-to-date with respect to given object
		//  otherwise false
		bool upToDate(const regIOobject&) const;

		//- Return true if up-to-date with respect to given objects
		//  otherwise false
		bool upToDate
		(
			const regIOobject&,
			const regIOobject&
		) const;

		//- Return true if up-to-date with respect to given objects
		//  otherwise false
		bool upToDate
		(
			const regIOobject&,
			const regIOobject&,
			const regIOobject&
		) const;

		//- Return true if up-to-date with respect to given objects
		//  otherwise false
		bool upToDate
		(
			const regIOobject&,
			const regIOobject&,
			const regIOobject&,
			const regIOobject&
		) const;

		//- Set up to date (obviously)
		void setUpToDate();


		// Edit

			//- Rename
		virtual void rename(const word& newName);


		// Reading

			//- Return complete path + object name if the file exists
			//  in the case directory otherwise null. Does not search
			//  up if parallel. Can be overridden to provide this functionality
			//  (e.g. IOdictionary)
		virtual fileName filePath() const;

		//- Read and check header info
		bool headerOk();

		//- Return Istream and check object type against that given
		Istream& readStream(const word&, const bool read = true);

		//- Close Istream
		void close();

		//- Virtual readData function.
		//  Must be defined in derived types for which
		//  re-reading is required
		virtual bool readData(Istream&);

		//- Read object
		virtual bool read();

		//- Add file watch for fileName on object if not yet watched. Return
		//  index of watch
		virtual label addWatch(const fileName&);

		//- Return file-monitoring handles
		inline const labelList& watchIndices() const;

		//- Return file-monitoring handles
		inline labelList& watchIndices();

		//- Return true if the object's file (or files for objectRegistry)
		//  have been modified. (modified state is cached by Time)
		virtual bool modified() const;

		//- Read object if modified (as set by call to modified)
		virtual bool readIfModified();

		//- Is object same for all processors
		//  Defaults to false, must be overridden by global IO classes
		virtual bool global() const;


		// Writing

			//- Pure virtual writaData function.
			//  Must be defined in derived types
		virtual bool writeData(Ostream&) const = 0;

		//- Write using given format, version and compression
		virtual bool writeObject
		(
			IOstream::streamFormat,
			IOstream::versionNumber,
			IOstream::compressionType,
			const bool write
		) const;

		//- Write using setting from DB
		virtual bool write(const bool write = true) const;


		// Member Operators

		void operator=(const IOobject&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <regIOobjectI.hxx>

#endif // !_regIOobject_Header
