#pragma once
#ifndef _multivariateGaussConvectionScheme_Header
#define _multivariateGaussConvectionScheme_Header

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
	tnbLib::fv::multivariateGaussConvectionScheme

Description
	Basic second-order convection using face-gradients and Gauss' theorem.

SourceFiles
	multivariateGaussConvectionScheme.C

\*---------------------------------------------------------------------------*/

#include <convectionScheme.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	namespace fv
	{

		/*---------------------------------------------------------------------------*\
							   Class multivariateGaussConvectionScheme Declaration
		\*---------------------------------------------------------------------------*/

		template<class Type>
		class multivariateGaussConvectionScheme
			:
			public fv::convectionScheme<Type>
		{
			// Private Data

			tmp<multivariateSurfaceInterpolationScheme<Type>> tinterpScheme_;


		public:

			//- Runtime type information
			TypeName("Gauss");


			// Constructors

				//- Construct from flux and Istream
			multivariateGaussConvectionScheme
			(
				const fvMesh& mesh,
				const typename multivariateSurfaceInterpolationScheme<Type>::
				fieldTable& fields,
				const surfaceScalarField& faceFlux,
				Istream& is
			)
				:
				convectionScheme<Type>(mesh, faceFlux),
				tinterpScheme_
				(
					multivariateSurfaceInterpolationScheme<Type>::New
					(
						mesh, fields, faceFlux, is
					)
				)
			{}


			// Member Functions

			tmp<multivariateSurfaceInterpolationScheme<Type>>
				interpolationScheme() const
			{
				return tinterpScheme_;
			}

			tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> interpolate
			(
				const surfaceScalarField&,
				const GeometricField<Type, fvPatchField, volMesh>&
			) const;

			tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> flux
			(
				const surfaceScalarField&,
				const GeometricField<Type, fvPatchField, volMesh>&
			) const;

			tmp<fvMatrix<Type>> fvmDiv
			(
				const surfaceScalarField&,
				const GeometricField<Type, fvPatchField, volMesh>&
			) const;

			tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDiv
			(
				const surfaceScalarField&,
				const GeometricField<Type, fvPatchField, volMesh>&
			) const;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace fv

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <multivariateGaussConvectionScheme.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_multivariateGaussConvectionScheme_Header