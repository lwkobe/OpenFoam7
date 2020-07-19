#pragma once
#ifndef _gradingDescriptors_Header
#define _gradingDescriptors_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2015-2019 OpenFOAM Foundation
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
	tnbLib::gradingDescriptors

Description
	List of gradingDescriptor for the sections of a block with additional IO
	functionality

SourceFiles
	gradingDescriptors.C

\*---------------------------------------------------------------------------*/

#include <gradingDescriptor.hxx>
#include <List.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	class Istream;
	class Ostream;

	// Forward declaration of friend functions and operators
	class gradingDescriptors;
	Istream& operator>>(Istream&, gradingDescriptors&);

	/*---------------------------------------------------------------------------*\
						 Class gradingDescriptors Declaration
	\*---------------------------------------------------------------------------*/

	class gradingDescriptors
		:
		public List<gradingDescriptor>
	{

	public:

		// Constructors

			//- Default constructor
		gradingDescriptors();

		//- Construct from a gradingDescriptor
		gradingDescriptors(const gradingDescriptor& gd);


		// Member Functions

			//- Return the inverse gradingDescriptors with 1/expansionRatio
		gradingDescriptors inv() const;


		// IOstream Operators

		friend Istream& operator>>(Istream&, gradingDescriptors&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_gradingDescriptors_Header
