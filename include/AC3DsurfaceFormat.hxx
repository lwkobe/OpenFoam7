#pragma once
#ifndef _AC3DsurfaceFormat_Header
#define _AC3DsurfaceFormat_Header

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
	tnbLib::fileFormats::AC3DsurfaceFormat

Description
	Provide a means of reading/writing AC3D format.

	http://www.inivis.com/ac3d/man/ac3dfileformat.html

Note
	On input, the faces are already organized as zones.
	The output is always sorted by zones.
	In the absence of zones, a single zone will be assigned.

SourceFiles
	AC3DsurfaceFormat.C

\*---------------------------------------------------------------------------*/

#include <MeshedSurface.hxx>
#include <MeshedSurfaceProxy.hxx>
#include <UnsortedMeshedSurface.hxx>
#include <AC3DsurfaceFormatCore.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace fileFormats
	{

		/*---------------------------------------------------------------------------*\
							  Class AC3DsurfaceFormat Declaration
		\*---------------------------------------------------------------------------*/

		template<class Face>
		class AC3DsurfaceFormat
			:
			public MeshedSurface<Face>,
			public AC3DsurfaceFormatCore
		{
		public:

			// Constructors

				//- Construct from file name
			AC3DsurfaceFormat(const fileName&);

			//- Disallow default bitwise copy construction
			AC3DsurfaceFormat(const AC3DsurfaceFormat<Face>&) = delete;


			// Selectors

				//- Read file and return surface
			static autoPtr<MeshedSurface<Face>> New(const fileName& name)
			{
				return autoPtr<MeshedSurface<Face>>
					(
						new AC3DsurfaceFormat<Face>(name)
						);
			}


			//- Destructor
			virtual ~AC3DsurfaceFormat()
			{}


			// Member Functions

				//- Write surface mesh components by proxy
			static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

			//- Write UnsortedMeshedSurface, the output is always sorted by zones.
			static void write(const fileName&, const UnsortedMeshedSurface<Face>&);

			//- Read from file
			virtual bool read(const fileName&);

			//- Write object
			virtual void write(const fileName& name) const
			{
				write(name, MeshedSurfaceProxy<Face>(*this));
			}


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const AC3DsurfaceFormat<Face>&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace fileFormats
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <AC3DsurfaceFormat.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_AC3DsurfaceFormat_Header
