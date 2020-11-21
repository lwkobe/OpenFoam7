#pragma once
#ifndef _memInfo_Header
#define _memInfo_Header

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
	tnbLib::memInfo

Description
	Memory usage information for the process running this object.

Note
	Uses the information from /proc/\<pid\>/status

SourceFiles
	memInfo.C

\*---------------------------------------------------------------------------*/

#include <OSspecific.hxx>
#include <IFstream.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators

	class memInfo;

	FoamBase_EXPORT Istream& operator>>(Istream&, memInfo&);
	FoamBase_EXPORT Ostream& operator<<(Ostream&, const memInfo&);


	/*---------------------------------------------------------------------------*\
							   Class memInfo Declaration
	\*---------------------------------------------------------------------------*/

	class memInfo
	{
		// Private Data

			//- Peak memory used by the process (VmPeak in /proc/\<pid\>/status)
		int peak_;

		//- Memory used by the process (VmSize in /proc/\<pid\>/status)
		int size_;

		//- Resident set size of the process (VmRSS in /proc/\<pid\>/status)
		int rss_;


	public:

		// Constructors

			//- Construct null
		FoamBase_EXPORT memInfo();


		//- Destructor
		FoamBase_EXPORT ~memInfo();


		// Member Functions

			//- Parse /proc/\<pid\>/status
		FoamBase_EXPORT const memInfo& update();

		// Access

			//- Access the stored peak memory (VmPeak in /proc/\<pid\>/status)
			//  The value is stored from the previous update()
		int peak() const
		{
			return peak_;
		}

		//- Access the stored memory size (VmSize in /proc/\<pid\>/status)
		//  The value is stored from the previous update()
		int size() const
		{
			return size_;
		}

		//- Access the stored rss value (VmRSS in /proc/\<pid\>/status)
		//  The value is stored from the previous update()
		int rss() const
		{
			return rss_;
		}

		//- True if the memory information appears valid
		FoamBase_EXPORT bool valid() const;


		// IOstream Operators

			//- Read peak/size/rss from stream
		friend FoamBase_EXPORT Istream& operator>>(Istream&, memInfo&);

		//- Write peak/size/rss to stream
		friend FoamBase_EXPORT Ostream& operator<<(Ostream&, const memInfo&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_memInfo_Header
