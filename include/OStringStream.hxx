#pragma once
#ifndef _OStringStream_Header
#define _OStringStream_Header

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
	tnbLib::OStringStream

Description
	Output to memory buffer stream.

SourceFiles
	StringStreamsPrint.C

\*---------------------------------------------------------------------------*/

#include <OSstream.hxx>

#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class OStringStream Declaration
	\*---------------------------------------------------------------------------*/

	class OStringStream
		:
		public OSstream
	{

	public:

		// Constructors

			//- Construct and set stream status
		OStringStream
		(
			streamFormat format = ASCII,
			versionNumber version = currentVersion
		)
			:
			OSstream
			(
				*(new std::ostringstream()),
				"OStringStream.sinkFile",
				format,
				version
			)
		{}

		//- Copy constructor
		OStringStream(const OStringStream& oss)
			:
			OSstream
			(
				*(
					new std::ostringstream
					(
						dynamic_cast<const std::ostringstream&>
						(
							oss.stdStream()
							).str()
					)
					),
				oss.name(),
				oss.format(),
				oss.version()
			)
		{}


		//- Destructor
		~OStringStream()
		{
			delete &dynamic_cast<std::ostringstream&>(stdStream());
		}


		// Member Functions

			// Access

				//- Return the string
		string str() const
		{
			return dynamic_cast<const std::ostringstream&>
				(
					stdStream()
					).str();
		}


		// Edit

			//- Rewind the OStringStream
		void rewind()
		{
			stdStream().rdbuf()->pubseekpos(0);
		}


		// Print

			//- Print description to Ostream
		FoamBase_EXPORT void print(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_OStringStream_Header
