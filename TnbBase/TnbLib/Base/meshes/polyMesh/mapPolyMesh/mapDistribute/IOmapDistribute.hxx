#pragma once
#ifndef _IOmapDistribute_Header
#define _IOmapDistribute_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2014-2019 OpenFOAM Foundation
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
	tnbLib::IOmapDistribute

Description
	IOmapDistribute is derived from mapDistribute and
	IOobject to give the mapDistribute
	automatic IO functionality via the objectRegistry.

SourceFiles
	IOmapDistribute.C

\*---------------------------------------------------------------------------*/

#include <mapDistribute.hxx>
#include <regIOobject.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class IOmapDistribute Declaration
	\*---------------------------------------------------------------------------*/

	class IOmapDistribute
		:
		public regIOobject,
		public mapDistribute
	{

	public:

		//- Runtime type information
		//TypeName("mapDistribute");
		static const char* typeName_() { return "mapDistribute"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };

		// Constructors

			//- Construct given an IOobject
		FoamBase_EXPORT IOmapDistribute(const IOobject&);

		//- Construct given an IOobject and mapDistribute
		FoamBase_EXPORT IOmapDistribute(const IOobject&, const mapDistribute&);

		//- Move constructor transferring the mapDistribute contents
		FoamBase_EXPORT IOmapDistribute(const IOobject&, mapDistribute&&);


		//- Destructor
		FoamBase_EXPORT virtual ~IOmapDistribute();


		// Member Functions

			//- ReadData function required for regIOobject read operation
		FoamBase_EXPORT virtual bool readData(Istream&);

		//- WriteData function required for regIOobject write operation
		FoamBase_EXPORT virtual bool writeData(Ostream&) const;

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_IOmapDistribute_Header
