#pragma once
#ifndef _actuationDiskSource_Header
#define _actuationDiskSource_Header

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
	tnbLib::fv::actuationDiskSource

Description
	Actuation disk source

	Constant values for momentum source for actuation disk
	\f[
		T = 2 \rho A U_{o}^2 a (1-a)
	\f]
	and
	\f[
		U_1 = (1 - a)U_{o}
	\f]

	where:
	\vartable
		A   | disk area
		U_o | upstream velocity
		a   | 1 - Cp/Ct
		U_1 | velocity at the disk
	\endvartable

Usage
	Example usage:
	\verbatim
	fields          (U);        // names of fields to apply source
	diskDir         (-1 0 0);   // disk direction
	Cp              0.1;        // power coefficient
	Ct              0.5;        // thrust coefficient
	diskArea        5.0;        // disk area
	upstreamPoint   (0 0 0);    // upstream point
	\endverbatim


SourceFiles
	actuationDiskSource.C
	actuationDiskSourceTemplates.C

\*---------------------------------------------------------------------------*/

#include <cellSetOption.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace fv
	{

		/*---------------------------------------------------------------------------*\
							 Class actuationDiskSource Declaration
		\*---------------------------------------------------------------------------*/

		class actuationDiskSource
			:
			public cellSetOption
		{

		protected:

			// Protected data

				//- Disk area normal
			vector diskDir_;

			//- Power coefficient
			scalar Cp_;

			//- Thrust coefficient
			scalar Ct_;

			//- Disk area
			scalar diskArea_;

			//- Upstream point sample
			point upstreamPoint_;

			//- Upstream cell ID
			label upstreamCellId_;


		private:

			// Private Member Functions

				//- Check data
			FoamFvOptions_EXPORT void checkData() const;

			//- Add resistance to the UEqn
			template<class RhoFieldType>
			void addActuationDiskAxialInertialResistance
			(
				vectorField& Usource,
				const labelList& cells,
				const scalarField& V,
				const RhoFieldType& rho,
				const vectorField& U
			) const;


		public:

			//- Runtime type information
			//TypeName("actuationDiskSource");
			static const char* typeName_() { return "actuationDiskSource"; }
			static FoamFvOptions_EXPORT const ::tnbLib::word typeName;
			static FoamFvOptions_EXPORT int debug;
			virtual const word& type() const { return typeName; };


			// Constructors

				//- Construct from components
			FoamFvOptions_EXPORT actuationDiskSource
			(
				const word& name,
				const word& modelType,
				const dictionary& dict,
				const fvMesh& mesh
			);

			//- Disallow default bitwise copy construction
			actuationDiskSource(const actuationDiskSource&) = delete;


			//- Destructor
			virtual ~actuationDiskSource()
			{}


			// Member Functions

				// Access

					//- Return Cp
			scalar Cp() const
			{
				return Cp_;
			}

			//- Return Ct
			scalar Ct() const
			{
				return Ct_;
			}

			//- Normal disk direction
			const vector& diskDir() const
			{
				return diskDir_;
			}

			//- Disk area
			scalar diskArea() const
			{
				return diskArea_;
			}


			// Add explicit and implicit contributions

			   //- Source term to momentum equation
			FoamFvOptions_EXPORT virtual void addSup
			(
				fvMatrix<vector>& eqn,
				const label fieldi
			);

			//- Source term to compressible momentum equation
			FoamFvOptions_EXPORT virtual void addSup
			(
				const volScalarField& rho,
				fvMatrix<vector>& eqn,
				const label fieldi
			);


			// IO

				//- Read dictionary
			FoamFvOptions_EXPORT virtual bool read(const dictionary& dict);


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const actuationDiskSource&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace fv
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#ifdef NoRepository
//#include <actuationDiskSourceTemplates.cxx>
//#endif

#include <actuationDiskSourceTemplatesI.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_actuationDiskSource_Header
