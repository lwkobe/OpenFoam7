#pragma once
#ifndef _specieCoeffs_Header
#define _specieCoeffs_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2019 OpenFOAM Foundation
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
	tnbLib::specieCoeffs

Description
	Class to hold the specie index and its coefficients in the
	reaction rate expression.

SourceFiles
	specieCoeffs.C

\*---------------------------------------------------------------------------*/

#include <speciesTable.hxx>
#include <token.hxx>  // added by amir
#include <OSstream.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators
	class specieCoeffs;
	Ostream& operator<<(Ostream&, const specieCoeffs&);


	/*---------------------------------------------------------------------------*\
							 Class specieCoeffs Declaration
	\*---------------------------------------------------------------------------*/

	class specieCoeffs
	{
	public:

		label index;
		scalar stoichCoeff;
		scalar exponent;

		specieCoeffs()
			:
			index(-1),
			stoichCoeff(0),
			exponent(1)
		{}

		FoamThermophysicalModels_EXPORT specieCoeffs(const speciesTable& species, Istream& is);

		bool operator==(const specieCoeffs& sc) const
		{
			return index == sc.index;
		}

		bool operator!=(const specieCoeffs& sc) const
		{
			return index != sc.index;
		}

		friend Ostream& operator<<(Ostream& os, const specieCoeffs& sc)
		{
			os << sc.index << token::SPACE
				<< sc.stoichCoeff << token::SPACE
				<< sc.exponent;
			return os;
		}

		//- Construct the left- and right-hand-side reaction coefficients
		static FoamThermophysicalModels_EXPORT void setLRhs
		(
			Istream&,
			const speciesTable&,
			List<specieCoeffs>& lhs,
			List<specieCoeffs>& rhs
		);

		//- Write the string representation of the specieCoeffs list
		static FoamThermophysicalModels_EXPORT void reactionStr
		(
			OStringStream& reaction,
			const speciesTable&,
			const List<specieCoeffs>& scs
		);

		//- Return string representation of reaction
		static FoamThermophysicalModels_EXPORT string reactionStr
		(
			OStringStream& reaction,
			const speciesTable&,
			const List<specieCoeffs>& lhs,
			const List<specieCoeffs>& rhs
		);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_specieCoeffs_Header
