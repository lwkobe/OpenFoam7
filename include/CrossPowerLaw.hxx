#pragma once
#ifndef _CrossPowerLaw_Header
#define _CrossPowerLaw_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2018-2019 OpenFOAM Foundation
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
	tnbLib::laminarModels::generalizedNewtonianViscosityModels::CrossPowerLaw

Description
	Cross-Power law generalized Newtonian viscosity model

	The strain rate coefficient can be specified either as the constant \c m or
	the critical stress level at the transition to shear thinning \c
	tauStar if \c tauStar is provided:

	Kinematic viscosity [m^2/s]

	\f[
		\nu = \nu_\infty + \frac{(\nu_0 - \nu_\infty)}{1 + (m\gamma)^n}
	\f]

	or

	\f[
		\nu = \nu_\infty
			+ \frac{(\nu_0 - \nu_\infty)}
				   {1 + \left(\frac{\nu_0\gamma}{\tau^*}\right)^n}
	\f]

	Example specification:
	\verbatim
		 viscosityModel CrossPowerLaw;

		 nuInf      10;
		 m          0.4;
		 n          3;
	\endverbatim

SourceFiles
	CrossPowerLaw.C

\*---------------------------------------------------------------------------*/

#include <generalizedNewtonianViscosityModel.hxx>
#include <dimensionedScalar.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace laminarModels
	{
		namespace generalizedNewtonianViscosityModels
		{

			/*---------------------------------------------------------------------------*\
									   Class CrossPowerLaw Declaration
			\*---------------------------------------------------------------------------*/

			class CrossPowerLaw
				:
				public generalizedNewtonianViscosityModel
			{
				// Private Data

				dimensionedScalar nuInf_;
				dimensionedScalar m_;
				dimensionedScalar tauStar_;
				dimensionedScalar n_;


			public:

				//- Runtime type information
				//TypeName("CrossPowerLaw");
				static const char* typeName_() { return "CrossPowerLaw"; }
				static FoamTurbulence_EXPORT const ::tnbLib::word typeName;
				static FoamTurbulence_EXPORT int debug;
				virtual const word& type() const { return typeName; };


				// Constructors

					//- Construct from components
				FoamTurbulence_EXPORT CrossPowerLaw(const dictionary& viscosityProperties);


				//- Destructor
				virtual ~CrossPowerLaw()
				{}


				// Member Functions

					//- Read transportProperties dictionary
				FoamTurbulence_EXPORT virtual bool read(const dictionary& viscosityProperties);

				//- Return the laminar viscosity
				FoamTurbulence_EXPORT virtual tmp<volScalarField> nu
				(
					const volScalarField& nu0,
					const volScalarField& strainRate
				) const;
			};


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace generalizedNewtonianViscosityModels
	} // End namespace laminarModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_CrossPowerLaw_Header
