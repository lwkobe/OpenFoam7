#pragma once
#ifndef _TimeScaleModel_Header
#define _TimeScaleModel_Header

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
	tnbLib::TimeScaleModel

Description
	Base class for time scale models.

SourceFiles
	TimeScaleModel.C

\*---------------------------------------------------------------------------*/

#include <constants.hxx>
#include <Field.hxx>
#include <FieldField.hxx>
#include <dictionary.hxx>
#include <runTimeSelectionTables.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class TimeScaleModel Declaration
	\*---------------------------------------------------------------------------*/

	class TimeScaleModel
	{
		// Private member functions

			//- Disallow default bitwise assignment
		void operator=(const TimeScaleModel&) = delete;


	protected:

		// Protected data

			//- Close pack volume fraction
		scalar alphaPacked_;

		//- Coefficient of restitution
		scalar e_;


	public:

		//- Runtime type information
		TypeName("timeScaleModel");

		//- Declare runtime constructor selection table
		declareRunTimeSelectionTable
		(
			autoPtr,
			TimeScaleModel,
			dictionary,
			(const dictionary& dict),
			(dict)
		);


		//- Constructors

			//- Construct from components
		TimeScaleModel(const dictionary& dict);

		//- Construct a copy
		TimeScaleModel(const TimeScaleModel& sm);

		//- Construct and return a clone
		virtual autoPtr<TimeScaleModel> clone() const = 0;


		//- Selector
		static autoPtr<TimeScaleModel> New
		(
			const dictionary& dict
		);


		//- Destructor
		virtual ~TimeScaleModel();


		//- Member Functions

			//- Time scale
		virtual tmp<FieldField<Field, scalar>> oneByTau
		(
			const FieldField<Field, scalar>& alpha,
			const FieldField<Field, scalar>& r32,
			const FieldField<Field, scalar>& uSqr,
			const FieldField<Field, scalar>& f
		) const = 0;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_TimeScaleModel_Header
