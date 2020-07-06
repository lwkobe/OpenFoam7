#include <uniformInterpolationTable.hxx>

#include <Time.hxx>
#include <IOdictionary.hxx>

// * * * * * * * * * * * *  Private Member Functions * * * * * * * * * * * * //

template<class Type>
void tnbLib::uniformInterpolationTable<Type>::checkTable() const
{
	if (size() < 2)
	{
		FatalErrorInFunction
			<< "Table " << name() << ": must have at least 2 values." << nl
			<< "Table size = " << size() << nl
			<< "    min, interval width = " << x0_ << ", " << dx_ << nl
			<< exit(FatalError);
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
tnbLib::uniformInterpolationTable<Type>::uniformInterpolationTable
(
	const IOobject& io,
	bool readFields
)
	:
	IOobject(io),
	List<scalar>(2, 0.0),
	x0_(0.0),
	dx_(1.0),
	log10_(false),
	bound_(false)
{
	if (readFields)
	{
		IOdictionary dict(io);

		dict.lookup("data") >> *this;
		dict.lookup("x0") >> x0_;
		dict.lookup("dx") >> dx_;
		dict.readIfPresent("log10", log10_);
		dict.readIfPresent("bound", bound_);
	}

	checkTable();
}


template<class Type>
tnbLib::uniformInterpolationTable<Type>::uniformInterpolationTable
(
	const word& tableName,
	const objectRegistry& db,
	const dictionary& dict,
	const bool initialiseOnly
)
	:
	IOobject
	(
		tableName,
		db.time().constant(),
		db,
		IOobject::NO_READ,
		IOobject::NO_WRITE,
		false // if used in BCs, could be used by multiple patches
	),
	List<scalar>(2, 0.0),
	x0_(readScalar(dict.lookup("x0"))),
	dx_(readScalar(dict.lookup("dx"))),
	log10_(dict.lookupOrDefault<Switch>("log10", false)),
	bound_(dict.lookupOrDefault<Switch>("bound", false))
{
	if (initialiseOnly)
	{
		const scalar xMax = readScalar(dict.lookup("xMax"));
		const label nIntervals = static_cast<label>(xMax - x0_) / dx_ + 1;
		this->setSize(nIntervals);
	}
	else
	{
		dict.lookup("data") >> *this;
	}

	checkTable();
}


template<class Type>
tnbLib::uniformInterpolationTable<Type>::uniformInterpolationTable
(
	const uniformInterpolationTable& uit
)
	:
	IOobject(uit),
	List<scalar>(uit),
	x0_(uit.x0_),
	dx_(uit.dx_),
	log10_(uit.log10_),
	bound_(uit.bound_)
{
	checkTable();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
tnbLib::uniformInterpolationTable<Type>::~uniformInterpolationTable()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
Type tnbLib::uniformInterpolationTable<Type>::interpolate(scalar x) const
{
	if (bound_)
	{
		x = max(min(xMax() - small * dx_, x), x0_);
	}
	else
	{
		if (x < x0_)
		{
			FatalErrorInFunction
				<< "Supplied value is less than minimum table value:" << nl
				<< "xMin=" << x0_ << ", xMax=" << xMax() << ", x=" << x << nl
				<< exit(FatalError);
		}

		if (x > xMax())
		{
			FatalErrorInFunction
				<< "Supplied value is greater than maximum table value:" << nl
				<< "xMin=" << x0_ << ", xMax=" << xMax() << ", x=" << x << nl
				<< exit(FatalError);
		}
	}

	const label i = static_cast<label>((x - x0_) / dx_);

	const scalar xLo = x0_ + i * dx_;

	Type fx = (x - xLo) / dx_ * (operator[](i + 1) - operator[](i)) + operator[](i);

	if (debug)
	{
		Info << "Table: " << name() << ", x=" << x
			<< ", x_lo=" << xLo << ", x_hi=" << xLo + dx_
			<< ", f(x_lo)=" << operator[](i) << ", f(x_hi)=" << operator[](i + 1)
			<< ", f(x)=" << fx << endl;
	}

	return fx;
}


template<class Type>
Type tnbLib::uniformInterpolationTable<Type>::interpolateLog10
(
	scalar x
) const
{
	if (log10_)
	{
		if (x > 0)
		{
			x = ::log10(x);
		}
		else if (bound_ && (x <= 0))
		{
			x = x0_;
		}
		else
		{
			FatalErrorInFunction
				<< "Table " << name() << nl
				<< "Supplied value must be greater than 0 when in log10 mode"
				<< nl << "x=" << x << nl << exit(FatalError);
		}
	}

	return interpolate(x);
}


template<class Type>
void tnbLib::uniformInterpolationTable<Type>::write() const
{
	IOdictionary dict(*this);

	dict.add("data", static_cast<const List<scalar>&>(*this));
	dict.add("x0", x0_);
	dict.add("dx", dx_);
	if (log10_)
	{
		dict.add("log10", log10_);
	}
	if (bound_)
	{
		dict.add("bound", bound_);
	}

	dict.regIOobject::write();
}


// ************************************************************************* //