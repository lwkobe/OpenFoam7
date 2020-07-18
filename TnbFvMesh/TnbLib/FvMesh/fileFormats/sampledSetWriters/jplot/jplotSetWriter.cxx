#include <jplotSetWriter.hxx>

#include <clock.hxx>
#include <coordSet.hxx>
#include <fileName.hxx>
#include <OFstream.hxx>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
tnbLib::Ostream& tnbLib::jplotSetWriter<Type>::writeHeader(Ostream& os) const
{
	return os
		<< "# JPlot input file" << nl
		<< "#" << nl << nl
		<< "# Generated by sample on " << clock::date().c_str() << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
tnbLib::jplotSetWriter<Type>::jplotSetWriter()
	:
	writer<Type>()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
tnbLib::jplotSetWriter<Type>::~jplotSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tnbLib::fileName tnbLib::jplotSetWriter<Type>::getFileName
(
	const coordSet& points,
	const wordList& valueSetNames
) const
{
	return this->getBaseName(points, valueSetNames) + ".dat";
}


template<class Type>
void tnbLib::jplotSetWriter<Type>::write
(
	const coordSet& points,
	const wordList& valueSetNames,
	const List<const Field<Type>*>& valueSets,
	Ostream& os
) const
{
	os << "# JPlot file" << nl
		<< "# column 1: " << points.name() << nl;

	forAll(valueSets, i)
	{
		os << "# column " << i + 2 << ": " << valueSetNames[i] << nl;
	}

	// Collect sets into columns
	List<const List<Type>*> columns(valueSets.size());

	forAll(valueSets, i)
	{
		columns[i] = valueSets[i];
	}

	this->writeTable(points, columns, os);
}


// ************************************************************************* //