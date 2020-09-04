#pragma once
#ifndef _extrudeModel_Header
#define _extrudeModel_Header

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
	tnbLib::extrudeModel

Description
	Top level extrusion model class

SourceFiles
	extrudeModel.C

\*---------------------------------------------------------------------------*/

#include <dictionary.hxx>
#include <point.hxx>
#include <autoPtr.hxx>
#include <runTimeSelectionTables.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class extrudeModel Declaration
	\*---------------------------------------------------------------------------*/

	class extrudeModel
	{
	protected:

		// Protected data

		label nLayers_;

		const scalar expansionRatio_;

		const dictionary& dict_;

		const dictionary& coeffDict_;


	public:

		//- Runtime type information
		TypeName("extrudeModel");

		//- Declare runtime constructor selection table

		declareRunTimeSelectionTable
		(
			autoPtr,
			extrudeModel,
			dictionary,
			(
				const dictionary& dict
				),
				(dict)
		);


		// Constructors

			//- Construct from dictionary
		extrudeModel(const word& modelType, const dictionary&);

		//- Disallow default bitwise copy construction
		extrudeModel(const extrudeModel&) = delete;


		// Selectors

			//- Select null constructed
		static autoPtr<extrudeModel> New(const dictionary&);


		//- Destructor
		virtual ~extrudeModel();


		// Member Functions

			// Access

		label nLayers() const;

		scalar expansionRatio() const;


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const extrudeModel&) = delete;

		//- Helper: calculate cumulative relative thickness for layer.
		//  (layer=0 -> 0; layer=nLayers -> 1)
		scalar sumThickness(const label layer) const;

		virtual point operator()
			(
				const point& surfacePoint,
				const vector& surfaceNormal,
				const label layer
				) const = 0;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_extrudeModel_Header
