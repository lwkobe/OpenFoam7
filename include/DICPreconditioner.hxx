#pragma once
#ifndef _DICPreconditioner_Header
#define _DICPreconditioner_Header

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
	tnbLib::DICPreconditioner

Description
	Simplified diagonal-based incomplete Cholesky preconditioner for symmetric
	matrices (symmetric equivalent of DILU).  The reciprocal of the
	preconditioned diagonal is calculated and stored.

SourceFiles
	DICPreconditioner.C

\*---------------------------------------------------------------------------*/

#include <lduMatrix.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class DICPreconditioner Declaration
	\*---------------------------------------------------------------------------*/

	class DICPreconditioner
		:
		public lduMatrix::preconditioner
	{
		// Private Data

			//- The reciprocal preconditioned diagonal
		scalarField rD_;


	public:

		//- Runtime type information
		//TypeName("DIC");
		static const char* typeName_() { return "DIC"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from matrix components and preconditioner solver controls
		FoamBase_EXPORT DICPreconditioner
		(
			const lduMatrix::solver&,
			const dictionary& solverControlsUnused
		);


		//- Destructor
		virtual ~DICPreconditioner()
		{}


		// Member Functions

			//- Calculate the reciprocal of the preconditioned diagonal
		static FoamBase_EXPORT void calcReciprocalD(scalarField& rD, const lduMatrix& matrix);

		//- Return wA the preconditioned form of residual rA
		FoamBase_EXPORT virtual void precondition
		(
			scalarField& wA,
			const scalarField& rA,
			const direction cmpt = 0
		) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_DICPreconditioner_Header
