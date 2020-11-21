#pragma once
#ifndef _uniform_Header
#define _uniform_Header

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
	tnbLib::distributionModels::uniform

Description
	Uniform/equally-weighted distribution model

SourceFiles
	uniform.C

\*---------------------------------------------------------------------------*/

#include <distributionModel.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace distributionModels
	{

		/*---------------------------------------------------------------------------*\
								   Class uniform Declaration
		\*---------------------------------------------------------------------------*/

		class uniform
			:
			public distributionModel
		{
			// Private Data

				//- Distribution minimum
			scalar minValue_;

			//- Distribution maximum
			scalar maxValue_;


		public:

			//- Runtime type information
			//TypeName("uniform");
			static const char* typeName_() { return "uniform"; }
			static FoamLagrangian_EXPORT const ::tnbLib::word typeName;
			static FoamLagrangian_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			FoamLagrangian_EXPORT uniform(const dictionary& dict, Random& rndGen);

			//- Construct copy
			FoamLagrangian_EXPORT uniform(const uniform& p);

			//- Construct and return a clone
			virtual autoPtr<distributionModel> clone() const
			{
				return autoPtr<distributionModel>(new uniform(*this));
			}


			//- Destructor
			FoamLagrangian_EXPORT virtual ~uniform();


			// Member Functions

				//- Sample the distributionModel
			FoamLagrangian_EXPORT virtual scalar sample() const;

			//- Return the minimum value
			FoamLagrangian_EXPORT virtual scalar minValue() const;

			//- Return the maximum value
			FoamLagrangian_EXPORT virtual scalar maxValue() const;

			//- Return the mean value
			FoamLagrangian_EXPORT virtual scalar meanValue() const;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace distributionModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif // !_uniform_Header
