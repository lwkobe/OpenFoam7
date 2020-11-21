#pragma once
#ifndef _IOOutputFilter_Header
#define _IOOutputFilter_Header

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
	tnbLib::IOOutputFilter

Description
	IOdictionary wrapper around OutputFilter to allow them to read from
	their associated dictionaries.

Note
	The IOobject or the objectRegistry will normally have to be
	derived from a fvMesh for a subsequent cast (within OutputFilter)
	to work correctly.

SourceFiles
	IOOutputFilter.C

\*---------------------------------------------------------------------------*/

#include <IOdictionary.hxx>
#include <pointFieldFwd.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of classes
	class mapPolyMesh;
	class polyMesh;  // added by amir

	/*---------------------------------------------------------------------------*\
						   Class IOOutputFilter Declaration
	\*---------------------------------------------------------------------------*/

	template<class OutputFilter>
	class IOOutputFilter
		:
		public IOdictionary,
		public OutputFilter
	{

	public:

		// Constructors

			//- Construct from an IOobject for IOdictionary
			//  Allow the possibility to load fields from files
		IOOutputFilter
		(
			const word& outputFilterName,
			const IOobject& ioDict,
			const bool loadFromFile = false
		);

		//- Construct for given objectRegistry and dictionary
		//  Allow dictionary to be optional
		//  Allow the possibility to load fields from files
		IOOutputFilter
		(
			const word& outputFilterName,
			const objectRegistry&,
			const word& dictName = OutputFilter::typeName() + "Dict",
			const IOobject::readOption rOpt = IOobject::MUST_READ_IF_MODIFIED,
			const bool loadFromFile = false
		);

		//- Construct for given objectRegistry and dictionary
		//  Dictionary read from full path.
		//  Allow the possibility to load fields from files
		IOOutputFilter
		(
			const word& outputFilterName,
			const objectRegistry&,
			const fileName& dictName,
			const IOobject::readOption rOpt = IOobject::MUST_READ_IF_MODIFIED,
			const bool loadFromFile = false
		);

		//- Disallow default bitwise copy construction
		IOOutputFilter(const IOOutputFilter&) = delete;


		//- Destructor
		virtual ~IOOutputFilter();


		// Member Functions

			//- Return name
		virtual const word& name() const;

		//- Inherit read from OutputFilter
		using OutputFilter::read;

		//- Read output filter properties
		virtual bool read();

		//- Inherit write from regIOobject
		using regIOobject::write;

		//- Sample and write
		virtual bool write(const bool write = true);

		//- Update for changes of mesh
		virtual void updateMesh(const mapPolyMesh& mpm);

		//- Update for changes of mesh
		virtual void movePoints(const polyMesh& mesh);


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const IOOutputFilter&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#ifdef NoRepository
//#include <IOOutputFilter.cxx>
//#endif

#include <IOOutputFilterI.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_IOOutputFilter_Header
