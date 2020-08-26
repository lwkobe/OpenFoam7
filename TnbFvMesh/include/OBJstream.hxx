#pragma once
#ifndef _OBJstream_Header
#define _OBJstream_Header

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
	tnbLib::OBJstream

Description
	OFstream which keeps track of vertices

SourceFiles
	OBJstream.C

\*---------------------------------------------------------------------------*/

#include <OFstream.hxx>
#include <point.hxx>
#include <edge.hxx>
#include <face.hxx>
#include <triPointRef.hxx>
#include <linePointRef.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							  Class OBJstream Declaration
	\*---------------------------------------------------------------------------*/

	class OBJstream
		:
		public OFstream
	{
		// Private Data

		bool startOfLine_;

		label nVertices_;


		// Private Member Functions

		void writeAndCheck(const char);

	public:

		// Declare name of the class and its debug switch
		ClassName("OBJstream");


		// Constructors

			//- Construct from pathname
		OBJstream
		(
			const fileName& pathname,
			streamFormat format = ASCII,
			versionNumber version = currentVersion,
			compressionType compression = UNCOMPRESSED
		);


		//- Destructor
		~OBJstream();


		// Member Functions

			// Access

				//- Return the number of vertices written
		label nVertices() const
		{
			return nVertices_;
		}


		// Ostream implementation

			//- Inherit write from Ostream
		using Ostream::write;

		//- Write character
		virtual Ostream& write(const char);

		//- Write character string
		virtual Ostream& write(const char*);

		//- Write word
		virtual Ostream& write(const word&);

		virtual Ostream& write(const string&);

		//- Write std::string surrounded by quotes.
		//  Optional write without quotes.
		virtual Ostream& writeQuoted
		(
			const std::string&,
			const bool quoted = true
		);


		// Direct write functionality

			//- Write point
		Ostream& write(const point&);

		//- Write point and vector normal ('vn')
		Ostream& write(const point&, const vector&);

		//- Write edge as points with line
		Ostream& write(const edge&, const UList<point>&);

		//- Write line
		Ostream& write(const linePointRef&);

		//- Write line with points and vector normals ('vn')
		Ostream& write
		(
			const linePointRef&,
			const vector& n0,
			const vector& n1
		);

		//- Write triangle as points with lines or filled polygon
		Ostream& write(const triPointRef&, const bool lines = true);

		//- Write face as points with lines or filled polygon
		Ostream& write
		(
			const face&,
			const UList<point>&,
			const bool lines = true
		);

		//- Write patch as points and faces with lines or filled polygons
		Ostream& write
		(
			const faceList&,
			const pointField&,
			const bool lines = true
		);

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_OBJstream_Header