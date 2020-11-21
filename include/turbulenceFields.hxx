#pragma once
#ifndef _turbulenceFields_Header
#define _turbulenceFields_Header

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
	tnbLib::functionObjects::turbulenceFields

Description
	Stores turbulence fields on the mesh database for further manipulation.

	Fields are stored as copies of the original, with the prefix
	"tubulenceModel:", e.g.:

	\verbatim
	turbulenceModel:R
	\endverbatim

	Example of function object specification:
	\verbatim
	turbulenceFields1
	{
		type        turbulenceFields;
		libs        ("libfieldFunctionObjects.so");
		...
		fields
		(
			R
			devRhoReff
		);
	}
	\endverbatim

Usage
	\table
		Property     | Description                 | Required | Default value
		type         | Type name: processorField   | yes      |
		fields       | Fields to store (see below) | yes      |
		prefix       | If true prefix fields       | no       | no
	\endtable

	Where \c fields can include:
	\plaintable
		k           | turbulence kinetic energy
		epsilon     | turbulence kinetic energy dissipation rate
		omega       | turbulence specific dissipation rate
		nut         | turbulence viscosity (incompressible)
		nuEff       | effective turbulence viscosity (incompressible)
		mut         | turbulence viscosity (compressible)
		muEff       | effective turbulence viscosity (compressible)
		alphat      | turbulence thermal diffusivity (compressible)
		alphaEff    | effective turbulence thermal diffusivity (compressible)
		R           | Reynolds stress tensor
		devReff     | Deviatoric part of the effective Reynolds stress
		devRhoReff  | Divergence of the Reynolds stress
	\endplaintable

See also
	tnbLib::functionObjects::fvMeshFunctionObject
	tnbLib::functionObjects::timeControl

SourceFiles
	turbulenceFields.C

\*---------------------------------------------------------------------------*/

#include <fvMeshFunctionObject.hxx>
#include <HashSet.hxx>
#include <NamedEnum.hxx>
#include <volFieldsFwd.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace functionObjects
	{

		/*---------------------------------------------------------------------------*\
							  Class turbulenceFields Declaration
		\*---------------------------------------------------------------------------*/

		class turbulenceFields
			:
			public fvMeshFunctionObject
		{
		public:

			enum class compressibleField
			{
				k,
				epsilon,
				omega,
				mut,
				muEff,
				alphat,
				alphaEff,
				R,
				devRhoReff
			};
			static FoamFunctionObjects_EXPORT const NamedEnum<compressibleField, 9> compressibleFieldNames_;

			enum class incompressibleField
			{
				k,
				epsilon,
				omega,
				nut,
				nuEff,
				R,
				devReff
			};
			static FoamFunctionObjects_EXPORT const NamedEnum<incompressibleField, 7> incompressibleFieldNames_;


		protected:

			// Protected data

				//- Fields to load
			wordHashSet fieldSet_;

			//- Optional field prefix to avoid name clashes
			//  Defaults to null
			word prefix_;


			// Protected Member Functions

			static FoamFunctionObjects_EXPORT const word& modelName();

			//- Process the turbulence field
			template<class Type>
			void processField
			(
				const word& fieldName,
				const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvalue
			);

			//- Return omega calculated from k and epsilon
			template<class Model>
			tmp<volScalarField> omega(const Model& model) const;


		public:

			//- Runtime type information
			//TypeName("turbulenceFields");
			static const char* typeName_() { return "turbulenceFields"; }
			static FoamFunctionObjects_EXPORT const ::tnbLib::word typeName;
			static FoamFunctionObjects_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from Time and dictionary
			FoamFunctionObjects_EXPORT turbulenceFields
			(
				const word& name,
				const Time& runTime,
				const dictionary& dict
			);

			//- Disallow default bitwise copy construction
			FoamFunctionObjects_EXPORT turbulenceFields(const turbulenceFields&) = delete;


			//- Destructor
			FoamFunctionObjects_EXPORT virtual ~turbulenceFields();


			// Member Functions

				//- Read the controls
			FoamFunctionObjects_EXPORT virtual bool read(const dictionary&);

			//- Calculate turbulence fields
			FoamFunctionObjects_EXPORT virtual bool execute();

			//- Do nothing.
			//  The turbulence fields are registered and written automatically
			FoamFunctionObjects_EXPORT virtual bool write();


			// Member Operators

				//- Disallow default bitwise assignment
			FoamFunctionObjects_EXPORT void operator=(const turbulenceFields&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace functionObjects
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
//#include <turbulenceFieldsTemplates.cxx>
#endif

#include <turbulenceFieldsTemplates.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_turbulenceFields_Header
