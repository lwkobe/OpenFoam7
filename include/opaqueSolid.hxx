#pragma once
#ifndef _opaqueSolid_Header
#define _opaqueSolid_Header

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
	tnbLib::radiationModels::opaqueSolid

Description
	Radiation for solid opaque solids - does nothing to energy equation source
	terms (returns zeros) but creates absorptionEmissionModel and
	scatterModel.

SourceFiles
	opaqueSolid.C

\*---------------------------------------------------------------------------*/

#include <radiationModel.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace radiationModels
	{

		/*---------------------------------------------------------------------------*\
								 Class opaqueSolid Declaration
		\*---------------------------------------------------------------------------*/

		class opaqueSolid
			:
			public radiationModel
		{
		public:

			//- Runtime type information
			TypeName("opaqueSolid");


			// Constructors

				//- Construct from components
			opaqueSolid(const volScalarField& T);

			//- Construct from components
			opaqueSolid(const dictionary& dict, const volScalarField& T);

			//- Disallow default bitwise copy construction
			opaqueSolid(const opaqueSolid&) = delete;


			//- Destructor
			virtual ~opaqueSolid();


			// Member Functions

				// Edit

					//- Solve radiation equation(s)
			//- Edited by amir
			void calculate() override;
			//void calculate();

			//- Read radiationProperties dictionary
			//- Edited by amir
			bool read() override;
			//bool read();

			//- Source term component (for power of T^4)
			//- Edited by amir
			tmp<volScalarField> Rp() const override;
			//tmp<volScalarField> Rp() const;

			//- Source term component (constant)
			//- Edited by amir
			tmp<volScalarField::Internal> Ru() const override;
			//tmp<volScalarField::Internal> Ru() const;


			// Member Operators

				//- Disallow default bitwise assignment
			void operator=(const opaqueSolid&) = delete;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace radiationModels
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_opaqueSolid_Header
