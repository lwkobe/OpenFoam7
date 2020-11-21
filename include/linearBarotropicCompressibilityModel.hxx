#pragma once
#ifndef _linearBarotropicCompressibilityModel_Header
#define _linearBarotropicCompressibilityModel_Header

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
	tnbLib::compressibilityModels::linear

Description
	linear compressibility model.

SourceFiles
	linear.C

\*---------------------------------------------------------------------------*/

#include <barotropicCompressibilityModel.hxx>
#include <dimensionedScalar.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressibilityModels
	{

		/*---------------------------------------------------------------------------*\
								   Class linear Declaration
		\*---------------------------------------------------------------------------*/

		class linear
			:
			public barotropicCompressibilityModel
		{
			// Private Data

			dimensionedScalar psiv_;
			dimensionedScalar psil_;


		public:

			//- Runtime type information
			//TypeName("linear");
			static const char* typeName_() { return "linear"; }
			static FoamThermophysicalModels_EXPORT const ::tnbLib::word typeName;
			static FoamThermophysicalModels_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			FoamThermophysicalModels_EXPORT linear
			(
				const dictionary& compressibilityProperties,
				const volScalarField& gamma,
				const word& psiName = "psi"
			);


			//- Destructor
			~linear()
			{}


			// Member Functions

				//- Correct the linear compressibility
			FoamThermophysicalModels_EXPORT void correct();

			//- Read transportProperties dictionary
			FoamThermophysicalModels_EXPORT bool read(const dictionary& compressibilityProperties);
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace compressibilityModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_linearBarotropicCompressibilityModel_Header
