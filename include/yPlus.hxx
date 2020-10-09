#pragma once
#ifndef _yPlus_Header
#define _yPlus_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2013-2019 OpenFOAM Foundation
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
	tnbLib::functionObjects::yPlus

Description
	Evaluates and outputs turbulence y+ for models. Values written to
	time directories as field 'yPlus'.

	Example of function object specification:
	\verbatim
	yPlus1
	{
		type        yPlus;
		libs        ("libfieldFunctionObjects.so");
		...
	}
	\endverbatim

Usage
	\table
		Property | Description                | Required   | Default value
		type     | type name: yPlus           | yes        |
	\endtable

Note
	Writing field 'yPlus' is done by default, but it can be overridden by
	defining an empty \c objects list. For details see writeLocalObjects.

See also
	tnbLib::functionObject
	tnbLib::functionObjects::fvMeshFunctionObject
	tnbLib::functionObjects::logFiles
	tnbLib::functionObjects::writeLocalObjects
	tnbLib::functionObjects::timeControl

SourceFiles
	yPlus.C

\*---------------------------------------------------------------------------*/

#include <fvMeshFunctionObject.hxx>
#include <logFiles.hxx>
#include <writeLocalObjects.hxx>
#include <volFieldsFwd.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of classes
	class turbulenceModel;

	namespace functionObjects
	{

		/*---------------------------------------------------------------------------*\
								  Class yPlus Declaration
		\*---------------------------------------------------------------------------*/

		class yPlus
			:
			public fvMeshFunctionObject,
			public logFiles,
			public writeLocalObjects
		{
			// Private Member Functions

				//- File header information
			virtual void writeFileHeader(const label i);

			//- Calculate y+
			tmp<volScalarField> calcYPlus
			(
				const turbulenceModel& turbModel
			);


		public:

			//- Runtime type information
			TypeName("yPlus");


			// Constructors

				//- Construct from Time and dictionary
			yPlus
			(
				const word& name,
				const Time& runTime,
				const dictionary& dict
			);

			//- Disallow default bitwise copy construction
			yPlus(const yPlus&) = delete;


			//- Destructor
			virtual ~yPlus();


			// Member Functions

				//- Read the yPlus data
			virtual bool read(const dictionary&);

			//- Calculate the yPlus field
			virtual bool execute();

			//- Write the yPlus field
			virtual bool write();


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const yPlus&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace functionObjects
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_yPlus_Header