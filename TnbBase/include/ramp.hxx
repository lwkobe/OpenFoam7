#pragma once
#ifndef _ramp_Header
#define _ramp_Header

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
	tnbLib::Function1Types::ramp

Description
	Ramp function base class for the set of scalar functions starting from 0 and
	increasing monotonically to 1 from \c start over the \c duration and
	remaining at 1 thereafter.

	Usage:
	\verbatim
		<entryName> <rampFunction>;
		<entryName>Coeffs
		{
			start     10;
			duration  20;
		}
	\endverbatim
	or
	\verbatim
		<entryName>
		{
			type      <rampFunction>;
			start     10;
			duration  20;
		}
	\endverbatim

	Where:
	\table
		Property | Description  | Required | Default value
		start    | Start time   | no       | 0
		duration | Duration     | yes      |
	\endtable

See also
	tnbLib::Function1

SourceFiles
	ramp.C

\*---------------------------------------------------------------------------*/

#include <Function1.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace Function1Types
	{

		/*---------------------------------------------------------------------------*\
								   Class ramp Declaration
		\*---------------------------------------------------------------------------*/

		class ramp
			:
			public Function1<scalar>
		{
		protected:

			// Protected data

				//- Start-time of the ramp function
			scalar start_;

			//- Duration of the ramp function
			scalar duration_;

			//- Simple linear ramp function
			//  which form the basis of many more complex ramp functions
			inline scalar linearRamp(const scalar t) const
			{
				return max(min((t - start_) / duration_, 1), 0);
			}


		private:

			// Private Member Functions

				//- Read the coefficients from the given dictionary
			FoamBase_EXPORT void read(const dictionary& coeffs);


		public:

			// Constructors

				//- Construct from entry name and dictionary
			FoamBase_EXPORT ramp
			(
				const word& entryName,
				const dictionary& dict
			);


			//- Destructor
			FoamBase_EXPORT virtual ~ramp();


			// Member Functions

				//- Return value for time t
			virtual scalar value(const scalar t) const = 0;

			//- Write in dictionary format
			FoamBase_EXPORT virtual void writeData(Ostream& os) const;


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const ramp&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace Function1Types
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_ramp_Header
