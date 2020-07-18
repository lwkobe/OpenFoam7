#pragma once
#ifndef _dimensionedType_Header
#define _dimensionedType_Header

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
	tnbLib::dimensioned

Description
	Generic dimensioned Type class

SourceFiles
	dimensionedType.C

\*---------------------------------------------------------------------------*/


#include <word.hxx>
#include <direction.hxx>
#include <dimensionSet.hxx>
#include <VectorSpace.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of friend functions and operators

	template<class Type> class dimensioned;

	template<class Type>
	Istream& operator>>(Istream&, dimensioned<Type>&);

	template<class Type>
	Ostream& operator<<(Ostream&, const dimensioned<Type>&);

	class dictionary;

	/*---------------------------------------------------------------------------*\
							   Class dimensioned Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class dimensioned
	{
		// Private Data

			//- Variable name
		word name_;

		//- The dimension set
		dimensionSet dimensions_;

		//- The data value
		Type value_;


		// Private Member Functions

			//- Initialize from Istream
			//  Helper-function for constructors
		void initialize(Istream& is);


	public:

		//- Component type
		typedef typename pTraits<Type>::cmptType cmptType;


		// Constructors

			//- Construct given name, dimensions and value
		dimensioned(const word&, const dimensionSet&, const Type&);

		//- Construct given dimensions and value
		//  with the name obtained from the value
		dimensioned(const dimensionSet&, const Type&);

		//- Construct dimensionless given value only
		//  with a name obtained from the value
		dimensioned(const Type&);

		//- Construct from a dimensioned<Type> changing the name.
		dimensioned(const word&, const dimensioned<Type>&);

		//- Construct from Istream.
		dimensioned(Istream&);

		//- Construct from an Istream with a given name
		dimensioned(const word&, Istream&);

		//- Construct from an Istream with a given name and dimensions
		dimensioned(const word&, const dimensionSet&, Istream&);

		//- Construct from dictionary lookup with a given name and dimensions
		dimensioned(const word&, const dimensionSet&, const dictionary&);

		//- Null constructor
		dimensioned();


		// Static member functions

			//- Construct from dictionary, with default dimensions and value.
		static dimensioned<Type> lookupOrDefault
		(
			const word&,
			const dictionary&,
			const dimensionSet& dims = dimless,
			const Type& defaultValue = pTraits<Type>::zero
		);

		//- Construct from dictionary dimensionless with value.
		static dimensioned<Type> lookupOrDefault
		(
			const word&,
			const dictionary&,
			const Type& defaultValue = pTraits<Type>::zero
		);

		//- Construct from dictionary, with default value.
		//  If the value is not found, it is added into the dictionary.
		static dimensioned<Type> lookupOrAddToDict
		(
			const word&,
			dictionary&,
			const dimensionSet& dims = dimless,
			const Type& defaultValue = pTraits<Type>::zero
		);

		//- Construct from dictionary, dimensionless with default value.
		//  If the value is not found, it is added into the dictionary.
		static dimensioned<Type> lookupOrAddToDict
		(
			const word&,
			dictionary&,
			const Type& defaultValue = pTraits<Type>::zero
		);


		// Member Functions

			//- Return const reference to name.
		const word& name() const;

		//- Return non-const reference to name.
		word& name();

		//- Return const reference to dimensions.
		const dimensionSet& dimensions() const;

		//- Return non-const reference to dimensions.
		dimensionSet& dimensions();

		//- Return const reference to value.
		const Type& value() const;

		//- Return non-const reference to value.
		Type& value();

		//- Return a component as a dimensioned<cmptType>
		dimensioned<cmptType> component(const direction) const;

		//- Return a component with a dimensioned<cmptType>
		void replace(const direction, const dimensioned<cmptType>&);

		//- Return transpose.
		dimensioned<Type> T() const;

		//- Update the value of dimensioned<Type>
		void read(const dictionary&);

		//- Update the value of dimensioned<Type> if found in the dictionary.
		bool readIfPresent(const dictionary&);


		// I/O

			//- Read value from stream and units from dictionary
		Istream& read(Istream& is, const dictionary&);

		//- Read value from stream and units from table
		Istream& read(Istream& is, const HashTable<dimensionedScalar>&);

		//- Read value from stream and units from system table
		Istream& read(Istream& is);


		// Member Operators

			//- Return a component as a dimensioned<cmptType>
		dimensioned<cmptType> operator[](const direction) const;

		void operator+=(const dimensioned<Type>&);
		void operator-=(const dimensioned<Type>&);
		void operator*=(const scalar);
		void operator/=(const scalar);


		// IOstream Operators

		friend Istream& operator>> <Type>
			(Istream&, dimensioned<Type>&);

		friend Ostream& operator<< <Type>
			(Ostream&, const dimensioned<Type>&);
	};


	template<class Type>
	void writeEntry(Ostream& os, const dimensioned<Type>& dt);


	// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

	template<class Type, direction r>
	dimensioned<typename powProduct<Type, r>::type>
		pow
		(
			const dimensioned<Type>&,
			typename powProduct<Type, r>::type
			= pTraits<typename powProduct<Type, r>::type>::zero
		);

	template<class Type>
	dimensioned<typename outerProduct<Type, Type>::type>
		sqr(const dimensioned<Type>&);

	template<class Type>
	dimensioned<scalar> magSqr(const dimensioned<Type>&);

	template<class Type>
	dimensioned<scalar> mag(const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> cmptMultiply
	(
		const dimensioned<Type>&,
		const dimensioned<Type>&
	);

	template<class Type>
	dimensioned<Type> cmptDivide
	(
		const dimensioned<Type>&,
		const dimensioned<Type>&
	);

	template<class Type>
	dimensioned<Type> max(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> min(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	bool operator>(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	bool operator<(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> operator+(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> operator-(const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> operator-(const dimensioned<Type>&, const dimensioned<Type>&);

	template<class Type>
	dimensioned<Type> operator*
		(
			const dimensioned<scalar>&,
			const dimensioned<Type>&
			);

	template<class Type>
	dimensioned<Type> operator/
		(
			const dimensioned<Type>&,
			const dimensioned<scalar>&
			);


#define PRODUCT_OPERATOR(product, op, opFunc)                                  \
                                                                               \
template<class Type1, class Type2>                                             \
dimensioned<typename product<Type1, Type2>::type>                              \
operator op(const dimensioned<Type1>&, const dimensioned<Type2>&);             \
                                                                               \
template<class Type, class Form, class Cmpt, direction nCmpt>                  \
dimensioned<typename product<Type, Form>::type>                                \
operator op                                                                    \
(                                                                              \
    const dimensioned<Type>&,                                                  \
    const VectorSpace<Form,Cmpt,nCmpt>&                                        \
);                                                                             \
                                                                               \
template<class Type, class Form, class Cmpt, direction nCmpt>                  \
dimensioned<typename product<Form, Type>::type>                                \
operator op                                                                    \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>&,                                       \
    const dimensioned<Type>&                                                   \
);

	template <class Type1, class Type2>
	dimensioned<typename outerProduct<Type1, Type2>::type> operator
	*(const dimensioned<Type1>&, const dimensioned<Type2>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename outerProduct<Type, Form>::type> operator *(const dimensioned<Type>&,
	                                                                const VectorSpace<Form, Cmpt, nCmpt>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename outerProduct<Form, Type>::type> operator *(const VectorSpace<Form, Cmpt, nCmpt>&,
	                                                                const dimensioned<Type>&);
		
	template <class Type1, class Type2>
	dimensioned<typename crossProduct<Type1, Type2>::type> operator
	^(const dimensioned<Type1>&, const dimensioned<Type2>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename crossProduct<Type, Form>::type> operator ^(const dimensioned<Type>&,
	                                                                const VectorSpace<Form, Cmpt, nCmpt>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename crossProduct<Form, Type>::type> operator ^(const VectorSpace<Form, Cmpt, nCmpt>&,
	                                                                const dimensioned<Type>&);
		
	template <class Type1, class Type2>
	dimensioned<typename innerProduct<Type1, Type2>::type> operator
	&(const dimensioned<Type1>&, const dimensioned<Type2>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename innerProduct<Type, Form>::type> operator &(const dimensioned<Type>&,
	                                                                const VectorSpace<Form, Cmpt, nCmpt>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename innerProduct<Form, Type>::type> operator &(const VectorSpace<Form, Cmpt, nCmpt>&,
	                                                                const dimensioned<Type>&);
		
	template <class Type1, class Type2>
	dimensioned<typename scalarProduct<Type1, Type2>::type> operator &&(const dimensioned<Type1>&,
	                                                                    const dimensioned<Type2>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename scalarProduct<Type, Form>::type> operator &&(const dimensioned<Type>&,
	                                                                  const VectorSpace<Form, Cmpt, nCmpt>&);
	template <class Type, class Form, class Cmpt, direction nCmpt>
	dimensioned<typename scalarProduct<Form, Type>::type> operator &&(const VectorSpace<Form, Cmpt, nCmpt>&,
	                                                                  const dimensioned<Type>&);

#undef PRODUCT_OPERATOR


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include <dimensionedType.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_dimensionedType_Header