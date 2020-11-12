#pragma once
#ifndef _Smagorinsky_Header
#define _Smagorinsky_Header

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
	tnbLib::LESModels::Smagorinsky

Description
	The Smagorinsky SGS model.

	Reference:
	\verbatim
		Smagorinsky, J. (1963).
		General circulation experiments with the primitive equations: I.
		The basic experiment*.
		Monthly weather review, 91(3), 99-164.
	\endverbatim

	The form of the Smagorinsky model implemented is obtained from the
	k-equation model assuming local equilibrium which provides estimates of both
	k and epsilon separate from the sub-grid scale viscosity:

	\verbatim
		B = 2/3*k*I - 2*nuSgs*dev(D)

	where

		D = symm(grad(U));
		k from D:B + Ce*k^3/2/delta = 0
		nuSgs = Ck*sqrt(k)*delta
	\endverbatim

	The default model coefficients are
	\verbatim
		SmagorinskyCoeffs
		{
			Ck                  0.094;
			Ce                  1.048;
		}
	\endverbatim

See also
	tnbLib::LESModels::kEqn

SourceFiles
	Smagorinsky.C

\*---------------------------------------------------------------------------*/

#include <LESModel.hxx>
#include <LESeddyViscosity.hxx>

#ifdef FoamTurbulence_EXPORT_DEFINE
#define FoamSmagorinsky_EXPORT __declspec(dllexport)
#else
#ifdef FoamSmagorinsky_EXPORT_DEFINE
#define FoamSmagorinsky_EXPORT __declspec(dllexport)
#else
#define FoamSmagorinsky_EXPORT __declspec(dllimport)
#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace LESModels
	{

		/*---------------------------------------------------------------------------*\
								   Class Smagorinsky Declaration
		\*---------------------------------------------------------------------------*/

		template<class BasicTurbulenceModel>
		class Smagorinsky
			:
			public LESeddyViscosity<BasicTurbulenceModel>
		{
		protected:

			// Protected data

			dimensionedScalar Ck_;


			// Protected Member Functions

				//- Return SGS kinetic energy
				//  calculated from the given velocity gradient
			tmp<volScalarField> k(const tmp<volTensorField>& gradU) const;

			//- Update the SGS eddy viscosity
			virtual void correctNut();


		public:

			typedef typename BasicTurbulenceModel::alphaField alphaField;
			typedef typename BasicTurbulenceModel::rhoField rhoField;
			typedef typename BasicTurbulenceModel::transportModel transportModel;


			//- Runtime type information
			//TypeName("Smagorinsky");
			static const char* typeName_() { return "Smagorinsky"; }
			static FoamSmagorinsky_EXPORT const ::tnbLib::word typeName;
			static FoamSmagorinsky_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			Smagorinsky
			(
				const alphaField& alpha,
				const rhoField& rho,
				const volVectorField& U,
				const surfaceScalarField& alphaRhoPhi,
				const surfaceScalarField& phi,
				const transportModel& transport,
				const word& propertiesName = turbulenceModel::propertiesName,
				const word& type = typeName
			);

			//- Disallow default bitwise copy construction
			Smagorinsky(const Smagorinsky&) = delete;


			//- Destructor
			virtual ~Smagorinsky()
			{}


			// Member Functions

				//- Read model coefficients if they have changed
			virtual bool read();

			//- Return SGS kinetic energy
			virtual tmp<volScalarField> k() const
			{
				return k(fvc::grad(this->U_));
			}

			//- Return sub-grid disipation rate
			virtual tmp<volScalarField> epsilon() const;

			//- Correct Eddy-Viscosity and related properties
			virtual void correct();


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const Smagorinsky&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace LESModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <SmagorinskyI.hxx>

//#ifdef NoRepository
//#include <Smagorinsky.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_Smagorinsky_Header
