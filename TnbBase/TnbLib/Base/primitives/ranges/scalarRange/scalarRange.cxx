#include <scalarRange.hxx>

#include <tokenList.hxx>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int tnbLib::scalarRange::debug(::tnbLib::debug::debugSwitch("scalarRange", 0));


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::scalarRange::scalarRange()
	:
	type_(EMPTY),
	value_(0),
	value2_(0)
{}


tnbLib::scalarRange::scalarRange(const scalar lower, const scalar upper)
	:
	type_(RANGE),
	value_(lower),
	value2_(upper)
{
	// mark invalid range as empty
	if (lower > upper)
	{
		type_ = EMPTY;
		value_ = value2_ = 0;
	}
}


tnbLib::scalarRange::scalarRange(Istream& is)
	:
	type_(EXACT),
	value_(0),
	value2_(0)
{
	is >> *this;

	if (scalarRange::debug)
	{
		Info << "constructed scalarRange: " << *this << endl;
	}
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool tnbLib::scalarRange::empty() const
{
	return type_ == EMPTY;
}


bool tnbLib::scalarRange::valid() const
{
	return type_ != EMPTY;
}


bool tnbLib::scalarRange::isExact() const
{
	return type_ == EXACT;
}


tnbLib::scalar tnbLib::scalarRange::value() const
{
	return value_;
}


tnbLib::scalar tnbLib::scalarRange::lower() const
{
	if (type_ == UPPER)
	{
		return -tnbLib::great;
	}
	else
	{
		return value_;
	}
}

tnbLib::scalar tnbLib::scalarRange::upper() const
{
	switch (type_)
	{
	case LOWER:
		return tnbLib::great;
		break;

	case RANGE:
		return value2_;
		break;

	default:
		return value_;
		break;
	}
}


bool tnbLib::scalarRange::selected(const scalar value) const
{
	switch (type_)
	{
	case LOWER:
		return value >= value_;

	case UPPER:
		return value <= value_;

	case RANGE:
		return value >= value_ && value <= value2_;

	case EXACT:
		return value == value_;

	default:
		return false;
	}
}



// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

bool tnbLib::scalarRange::operator==(const scalarRange& range) const
{
	return
		(
			type_ == range.type_
			&& value_ == range.value_
			&& value2_ == range.value2_
			);
}


bool tnbLib::scalarRange::operator!=(const scalarRange& range) const
{
	return !(operator==(range));
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

tnbLib::Istream& tnbLib::operator>>(Istream& is, scalarRange& range)
{
	range.type_ = scalarRange::EXACT;
	range.value_ = 0;
	range.value2_ = 0;

	List<token> toks(4);
	label nTok = 0;

	// skip leading ','
	do
	{
		is.read(toks[nTok]);
		is.check("scalarRange token read");
	} while
		(
			toks[nTok].isPunctuation()
			&& toks[nTok].pToken() == token::COMMA
			);

	++nTok;

	// looks like ':VALUE'
	if
		(
			toks[nTok - 1].isPunctuation()
			&& toks[nTok - 1].pToken() == token::COLON
			)
	{
		range.type_ = scalarRange::UPPER;
		is.read(toks[nTok++]);
		is.check("scalarRange token read");
	}

	// a number is now required
	if (!toks[nTok - 1].isNumber())
	{
		is.setBad();
		range.type_ = scalarRange::EMPTY;
		range.value_ = range.value2_ = 0;
		Info << "rejected ill-formed or empty range:";
		for (label i = 0; i < nTok; ++i)
		{
			Info << " " << toks[i];
		}
		Info << endl;
		return is;
	}

	range.value_ = toks[nTok - 1].number();
	is.read(toks[nTok++]);
	is.check("scalarRange token read");

	if (scalarRange::debug)
	{
		Info << "tokens:";
		for (label i = 0; i < nTok; ++i)
		{
			Info << " " << toks[i];
		}
		Info << endl;
	}

	// could be 'VALUE:' or 'VALUE:VALUE'
	if
		(
			toks[nTok - 1].isPunctuation()
			&& toks[nTok - 1].pToken() == token::COLON
			)
	{
		if (range.type_ == scalarRange::UPPER)
		{
			is.setBad();
			range.type_ = scalarRange::EMPTY;
			range.value_ = range.value2_ = 0;
			Info << "rejected ill-formed range:";
			for (label i = 0; i < nTok; ++i)
			{
				Info << " " << toks[i];
			}
			Info << endl;
			return is;
		}

		is.read(toks[nTok++]);
		is.check("scalarRange token read");

		if (scalarRange::debug)
		{
			Info << "tokens:";
			for (label i = 0; i < nTok; ++i)
			{
				Info << " " << toks[i];
			}
			Info << endl;
		}


		// if there is a number, we have 'VALUE:VALUE' and not simply 'VALUE:'
		if (toks[nTok - 1].isNumber())
		{
			range.type_ = scalarRange::RANGE;
			range.value2_ = toks[nTok - 1].number();
			is.read(toks[nTok++]);
			is.check("scalarRange token read");
		}
		else
		{
			range.type_ = scalarRange::LOWER;
		}
	}

	if (scalarRange::debug)
	{
		Info << "tokens:";
		for (label i = 0; i < nTok; ++i)
		{
			Info << " " << toks[i];
		}
		Info << endl;
	}


	// some remaining tokens, but they are not the next comma
	// - this is a problem!
	if
		(
			toks[nTok - 1].good()
			&&
			(
				!toks[nTok - 1].isPunctuation()
				|| toks[nTok - 1].pToken() != token::COMMA
				)
			)
	{
		is.setBad();
		range.type_ = scalarRange::EMPTY;
		range.value_ = range.value2_ = 0;

		Info << "rejected ill-formed range:";
		for (label i = 0; i < nTok; ++i)
		{
			Info << " " << toks[i];
		}
		Info << endl;
	}

	return is;
}


tnbLib::Ostream& tnbLib::operator<<(Ostream& os, const scalarRange& range)
{
	switch (range.type_)
	{
	case scalarRange::LOWER:
		os << range.value_ << " <=> Inf";
		break;

	case scalarRange::UPPER:
		os << "-Inf <=> " << range.value_;
		break;

	case scalarRange::RANGE:
		os << range.value_ << " <=> " << range.value2_;
		break;

	case scalarRange::EXACT:
		os << range.value_;
		break;

	default:
		os << "empty";
		break;
	}

	return os;
}


// ************************************************************************* //