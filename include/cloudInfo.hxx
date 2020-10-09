#pragma once
#ifndef _cloudInfo_Header
#define _cloudInfo_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2012-2019 OpenFOAM Foundation
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
	tnbLib::functionObjects::cloudInfo

Description
	Outputs Lagrangian cloud information to a file.

	The current outputs include:
	- total current number of parcels
	- total current mass of parcels

	Example of function object specification:
	\verbatim
	cloudInfo1
	{
		type        cloudInfo;
		libs        ("libcloudFunctionObjects.so");
		...
		clouds
		(
			kinematicCloud1
			thermoCloud1
		);
	}
	\endverbatim


Usage
	\table
		Property     | Description             | Required    | Default value
		type         | type name: cloudInfo    | yes         |
		clouds       | list of clouds names to process |yes  |
	\endtable

	The output data of each cloud is written to a file named \<cloudName\>.dat

See also
	tnbLib::functionObject
	tnbLib::functionObjects::regionFunctionObject
	tnbLib::functionObjects::logFiles

SourceFiles
	cloudInfo.C

\*---------------------------------------------------------------------------*/

#include <regionFunctionObject.hxx>
#include <logFiles.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace functionObjects
	{

		/*---------------------------------------------------------------------------*\
								 Class cloudInfo Declaration
		\*---------------------------------------------------------------------------*/

		class cloudInfo
			:
			public regionFunctionObject,
			public logFiles
		{
		protected:

			// Protected Member Functions

				//- File header information
			virtual void writeFileHeader(const label i);


		public:

			//- Runtime type information
			TypeName("cloudInfo");


			// Constructors

				//- Construct from Time and dictionary
			cloudInfo
			(
				const word& name,
				const Time& runTime,
				const dictionary&
			);

			//- Disallow default bitwise copy construction
			cloudInfo(const cloudInfo&) = delete;


			//- Destructor
			virtual ~cloudInfo();


			// Member Functions

				//- Read the controls
			virtual bool read(const dictionary&);

			//- Execute, currently does nothing
			virtual bool execute();

			//- Write
			virtual bool write();


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const cloudInfo&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace functionObjects
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_cloudInfo_Header
