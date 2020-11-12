#pragma once
#ifndef _incompressibleTurbulenceModel_Header
#define _incompressibleTurbulenceModel_Header

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
	tnbLib::incompressibleTurbulenceModel

Description
	Abstract base class for turbulence models (RAS, LES and laminar).

SourceFiles
	incompressibleTurbulenceModel.C

\*---------------------------------------------------------------------------*/

#include <turbulenceModel.hxx>
#include <geometricOneField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declarations
	class fvMesh;

	/*---------------------------------------------------------------------------*\
					Class incompressibleTurbulenceModel Declaration
	\*---------------------------------------------------------------------------*/

	class incompressibleTurbulenceModel
		:
		public turbulenceModel
	{
	protected:

		// Protected data

		geometricOneField rho_;


		// Protected member functions

			//- ***HGW Temporary function to be removed when the run-time selectable
			//  thermal transport layer is complete
		virtual void correctNut()
		{}


	public:

		//- Runtime type information
		//TypeName("incompressibleTurbulenceModel");
		static const char* typeName_() { return "incompressibleTurbulenceModel"; }
		static FoamTurbulence_EXPORT const ::tnbLib::word typeName;
		static FoamTurbulence_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from components
		FoamTurbulence_EXPORT incompressibleTurbulenceModel
		(
			const geometricOneField& rho,
			const volVectorField& U,
			const surfaceScalarField& alphaRhoPhi,
			const surfaceScalarField& phi,
			const word& propertiesName
		);

		//- Disallow default bitwise copy construction
		FoamTurbulence_EXPORT incompressibleTurbulenceModel
		(
			const incompressibleTurbulenceModel&
		) = delete;


		//- Destructor
		virtual ~incompressibleTurbulenceModel()
		{}


		// Member Functions

			//- Return the laminar dynamic viscosity
		FoamTurbulence_EXPORT virtual tmp<volScalarField> mu() const;

		//- Return the laminar dynamic viscosity on patch
		FoamTurbulence_EXPORT virtual tmp<scalarField> mu(const label patchi) const;

		//- Return the turbulence dynamic viscosity
		FoamTurbulence_EXPORT virtual tmp<volScalarField> mut() const;

		//- Return the turbulence dynamic viscosity on patch
		FoamTurbulence_EXPORT virtual tmp<scalarField> mut(const label patchi) const;

		//- Return the effective dynamic viscosity
		FoamTurbulence_EXPORT virtual tmp<volScalarField> muEff() const;

		//- Return the effective dynamic viscosity on patch
		FoamTurbulence_EXPORT virtual tmp<scalarField> muEff(const label patchi) const;

		//- Return the effective stress tensor including the laminar stress
		FoamTurbulence_EXPORT virtual tmp<volSymmTensorField> devReff() const = 0;

		//- Return the source term for the momentum equation
		FoamTurbulence_EXPORT virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const = 0;


		// Member Operators

			//- Disallow default bitwise assignment
		FoamTurbulence_EXPORT void operator=(const incompressibleTurbulenceModel&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_incompressibleTurbulenceModel_Header
