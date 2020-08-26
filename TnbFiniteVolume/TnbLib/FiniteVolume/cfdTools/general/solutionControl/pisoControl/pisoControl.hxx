#pragma once
#ifndef _pisoControl_Header
#define _pisoControl_Header

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
	tnbLib::pisoControl

Description
	Piso control class. Provides time-loop and piso-loop control methods. No
	convergence checking is done.

SourceFiles
	pisoControlI.H
	pisoControl.C

\*---------------------------------------------------------------------------*/

#include <fluidSolutionControl.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							 Class pisoControl Declaration
	\*---------------------------------------------------------------------------*/

	class pisoControl
		:
		public fluidSolutionControl
	{
	protected:

		// Protected data

			//- Maximum number of piso correctors
		label nCorrPiso_;

		//- Current piso corrector
		label corrPiso_;


	public:

		// Static Data Members

			//- Run-time type information
		TypeName("pisoControl");


		// Constructors

			//- Construct from a mesh and the name of the algorithm
		pisoControl(fvMesh& mesh, const word& algorithmName = "PISO");


		//- Destructor
		virtual ~pisoControl();


		// Member Functions

			// IO

				//- Read controls
		virtual bool read();


		// Access

			//- Maximum number of piso correctors
		inline label nCorrPiso() const;

		//- Flat to indicate any piso iteration
		inline bool anyPisoIter() const;

		//- Flag to indicate the first piso iteration
		inline bool firstPisoIter() const;

		//- Flag to indicate the last piso iteration
		inline bool finalPisoIter() const;

		//- Flag to indicate the last inner iteration (last piso and last
		//  non-orthogonal)
		inline bool finalInnerIter() const;

		//- Flag to indicate whether in final state
		virtual bool isFinal() const;


		// Evolution

			//- Piso loop
		bool correct();

		//- Time run loop
		bool run(Time& time);

		//- Time loop loop
		bool loop(Time& time);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <pisoControlI.hxx>

#endif // !_pisoControl_Header
