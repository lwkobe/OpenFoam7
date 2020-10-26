#pragma once
#ifndef _diagonalPreconditioner_Header
#define _diagonalPreconditioner_Header

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
	tnbLib::diagonalPreconditioner

Description
	Diagonal preconditioner for both symmetric and asymmetric matrices.

	The reciprocal of the diagonal is calculated and stored for reuse
	because on most systems '*' is faster than '/'.

SourceFiles
	diagonalPreconditioner.C

\*---------------------------------------------------------------------------*/

#include <lduMatrix.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class diagonalPreconditioner Declaration
	\*---------------------------------------------------------------------------*/

	class diagonalPreconditioner
		:
		public lduMatrix::preconditioner
	{
		// Private Data

			//- The reciprocal diagonal
		scalarField rD;


	public:

		//- Runtime type information
		//TypeName("diagonal");
		static const char* typeName_() { return "diagonal"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from matrix components and preconditioner solver controls
		FoamBase_EXPORT diagonalPreconditioner
		(
			const lduMatrix::solver&,
			const dictionary& solverControlsUnused
		);

		//- Disallow default bitwise copy construction
		FoamBase_EXPORT diagonalPreconditioner(const diagonalPreconditioner&) = delete;


		//- Destructor
		virtual ~diagonalPreconditioner()
		{}


		// Member Functions

			//- Return wA the preconditioned form of residual rA
		FoamBase_EXPORT virtual void precondition
		(
			scalarField& wA,
			const scalarField& rA,
			const direction cmpt = 0
		) const;

		//- Return wT the transpose-matrix preconditioned form of residual rT.
		virtual void preconditionT
		(
			scalarField& wT,
			const scalarField& rT,
			const direction cmpt = 0
		) const
		{
			return precondition(wT, rT, cmpt);
		}


		// Member Operators

			//- Disallow default bitwise assignment
		FoamBase_EXPORT void operator=(const diagonalPreconditioner&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_diagonalPreconditioner_Header
