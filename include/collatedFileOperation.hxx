#pragma once
#ifndef _collatedFileOperation_Header
#define _collatedFileOperation_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2017-2019 OpenFOAM Foundation
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
	tnbLib::fileOperations::collatedFileOperation

Description
	Version of masterUncollatedFileOperation that collates regIOobjects
	into a container in the processors/ subdirectory.

	Uses threading if maxThreadFileBufferSize > 0.

See also
	masterUncollatedFileOperation

SourceFiles
	collatedFileOperation.C

\*---------------------------------------------------------------------------*/

#include <masterUncollatedFileOperation.hxx>
#include <OFstreamCollator.hxx>
#include <fileOperationInitialise.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace fileOperations
	{

		/*---------------------------------------------------------------------------*\
							Class collatedFileOperation Declaration
		\*---------------------------------------------------------------------------*/

		class collatedFileOperation
			:
			public masterUncollatedFileOperation
		{
		protected:

			// Protected data

				//- Any communicator allocated by me
			const label myComm_;

			//- Threaded writer
			mutable OFstreamCollator writer_;

			// For non-parallel operation

				//- Number of processors (overall)
			label nProcs_;

			//- Ranks of IO handlers
			const labelList ioRanks_;


			// Private Member Functions

			static FoamBase_EXPORT labelList ioRanks();

			//- Is proci master of communicator (in parallel) or master of
			//  the io ranks (non-parallel)
			FoamBase_EXPORT bool isMasterRank(const label proci) const;

			//- Append to processors/ file
			FoamBase_EXPORT bool appendObject
			(
				const regIOobject& io,
				const fileName& pathName,
				IOstream::streamFormat fmt,
				IOstream::versionNumber ver,
				IOstream::compressionType cmp
			) const;


		public:

			//- Runtime type information
			//TypeName("collated");
			static const char* typeName_() { return "collated"; }
			static FoamBase_EXPORT const ::tnbLib::word typeName;
			static FoamBase_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Static data

				//- Max size of thread buffer size. This is the overall size of
				//  all files to be written. Starts blocking if not enough size.
				//  Read as float to enable easy specification of large sizes.
			static FoamBase_EXPORT float maxThreadFileBufferSize;


			// Constructors

				//- Construct null
			FoamBase_EXPORT collatedFileOperation(const bool verbose);

			//- Construct from user communicator
			FoamBase_EXPORT collatedFileOperation
			(
				const label comm,
				const labelList& ioRanks,
				const word& typeName,
				const bool verbose
			);


			//- Destructor
			FoamBase_EXPORT virtual ~collatedFileOperation();


			// Member Functions

				// (reg)IOobject functionality

					//- Generate disk file name for object. Opposite of filePath.
			FoamBase_EXPORT virtual fileName objectPath
			(
				const IOobject& io,
				const word& typeName
			) const;

			//- Writes a regIOobject (so header, contents and divider).
			//  Returns success state.
			FoamBase_EXPORT virtual bool writeObject
			(
				const regIOobject&,
				IOstream::streamFormat format = IOstream::ASCII,
				IOstream::versionNumber version = IOstream::currentVersion,
				IOstream::compressionType compression = IOstream::UNCOMPRESSED,
				const bool write = true
			) const;

			// Other

				//- Forcibly wait until all output done. Flush any cached data
			FoamBase_EXPORT virtual void flush() const;

			//- Actual name of processors dir
			FoamBase_EXPORT virtual word processorsDir(const IOobject&) const;

			//- Actual name of processors dir
			FoamBase_EXPORT virtual word processorsDir(const fileName&) const;

			//- Set number of processor directories/results. Only used in
			//  decomposePar
			FoamBase_EXPORT virtual void setNProcs(const label nProcs);
		};


		/*---------------------------------------------------------------------------*\
					   Class collatedFileOperationInitialise Declaration
		\*---------------------------------------------------------------------------*/

		class collatedFileOperationInitialise
			:
			public masterUncollatedFileOperationInitialise
		{
		public:

			// Constructors

				//- Construct from components
			collatedFileOperationInitialise(int& argc, char**& argv)
				:
				masterUncollatedFileOperationInitialise(argc, argv)
			{}


			//- Destructor
			virtual ~collatedFileOperationInitialise()
			{}


			// Member Functions

				//- Needs threading
			virtual bool needsThreading() const
			{
				return
					collatedFileOperation::maxThreadFileBufferSize
			  > 0;
			}
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace fileOperations
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_collatedFileOperation_Header
