#pragma once
#include <IOmanip.hxx>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline tnbLib::species::thermo<Thermo, Type>::thermo
(
	const Thermo& sp
)
	:
	Thermo(sp)
{}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::T
(
	const scalar f,
	const scalar p,
	const scalar T0,
	scalar(thermo<Thermo, Type>::*F)(const scalar, const scalar) const,
	scalar(thermo<Thermo, Type>::*dFdT)(const scalar, const scalar)
	const,
	scalar(thermo<Thermo, Type>::*limit)(const scalar) const,
	const bool diagnostics
) const
{
	if (T0 < 0)
	{
		FatalErrorInFunction
			<< "Negative initial temperature T0: " << T0
			<< abort(FatalError);
	}

	scalar Test = T0;
	scalar Tnew = T0;
	scalar Ttol = T0 * tol_;
	int    iter = 0;

	if (diagnostics)
	{
		const unsigned int width = IOstream::defaultPrecision() + 8;

		InfoInFunction
			<< "Energy -> temperature conversion failed to converge:" << endl;
		Pout << setw(width) << "iter"
			<< setw(width) << "Test"
			<< setw(width) << "e/h"
			<< setw(width) << "Cv/p"
			<< setw(width) << "Tnew"
			<< endl;
	}

	do
	{
		Test = Tnew;
		Tnew =
			(this->*limit)
			(Test - ((this->*F)(p, Test) - f) / (this->*dFdT)(p, Test));

		if (diagnostics)
		{
			const unsigned int width = IOstream::defaultPrecision() + 8;

			Pout << setw(width) << iter
				<< setw(width) << Test
				<< setw(width) << ((this->*F)(p, Test))
				<< setw(width) << ((this->*dFdT)(p, Test))
				<< setw(width) << Tnew
				<< endl;
		}

		if (iter++ > maxIter_)
		{
			if (!diagnostics)
			{
				T(f, p, T0, F, dFdT, limit, true);
			}

			FatalErrorInFunction
				<< "Maximum number of iterations exceeded: " << maxIter_
				<< abort(FatalError);
		}

	} while (mag(Tnew - Test) > Ttol);

	return Tnew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline tnbLib::species::thermo<Thermo, Type>::thermo
(
	const word& name,
	const thermo& st
)
	:
	Thermo(name, st)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline tnbLib::word
tnbLib::species::thermo<Thermo, Type>::heName()
{
	return Type<thermo<Thermo, Type>>::energyName();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::Cpv(const scalar p, const scalar T) const
{
	return Type<thermo<Thermo, Type>>::Cpv(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::gamma(const scalar p, const scalar T) const
{
	const scalar Cp = this->Cp(p, T);
	return Cp / (Cp - this->CpMCv(p, T));
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::CpByCpv
(
	const scalar p,
	const scalar T
) const
{
	return Type<thermo<Thermo, Type>>::CpByCpv(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::HE(const scalar p, const scalar T) const
{
	return Type<thermo<Thermo, Type>>::HE(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::G(const scalar p, const scalar T) const
{
	return this->Ha(p, T) - T * this->S(p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::A(const scalar p, const scalar T) const
{
	return this->Ea(p, T) - T * this->S(p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::cp(const scalar p, const scalar T) const
{
	return this->Cp(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::ha(const scalar p, const scalar T) const
{
	return this->Ha(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::hs(const scalar p, const scalar T) const
{
	return this->Hs(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::hc() const
{
	return this->Hc()*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::s(const scalar p, const scalar T) const
{
	return this->S(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::he(const scalar p, const scalar T) const
{
	return this->HE(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::cv(const scalar p, const scalar T) const
{
	return this->Cv(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::es(const scalar p, const scalar T) const
{
	return this->Es(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::ea(const scalar p, const scalar T) const
{
	return this->Ea(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::g(const scalar p, const scalar T) const
{
	return this->G(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::a(const scalar p, const scalar T) const
{
	return this->A(p, T)*this->W();
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::K(const scalar p, const scalar T) const
{
	scalar arg = -this->Y()*this->G(Pstd, T) / (RR*T);

	if (arg < 600)
	{
		return exp(arg);
	}
	else
	{
		return rootVGreat;
	}
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::Kp(const scalar p, const scalar T) const
{
	return K(p, T);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::Kc(const scalar p, const scalar T) const
{
	const scalar nm = this->Y() / this->W();

	if (equal(nm, small))
	{
		return Kp(p, T);
	}
	else
	{
		return Kp(p, T)*pow(Pstd / (RR*T), nm);
	}
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::Kx
(
	const scalar p,
	const scalar T
) const
{
	const scalar nm = this->Y() / this->W();

	if (equal(nm, small))
	{
		return Kp(p, T);
	}
	else
	{
		return Kp(p, T)*pow(Pstd / p, nm);
	}
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::Kn
(
	const scalar p,
	const scalar T,
	const scalar n
) const
{
	const scalar nm = this->Y() / this->W();

	if (equal(nm, small))
	{
		return Kp(p, T);
	}
	else
	{
		return Kp(p, T)*pow(n*Pstd / p, nm);
	}
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::THE
(
	const scalar he,
	const scalar p,
	const scalar T0
) const
{
	return Type<thermo<Thermo, Type>>::THE(*this, he, p, T0);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::THs
(
	const scalar hs,
	const scalar p,
	const scalar T0
) const
{
	return T
	(
		hs,
		p,
		T0,
		&thermo<Thermo, Type>::Hs,
		&thermo<Thermo, Type>::Cp,
		&thermo<Thermo, Type>::limit
	);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::THa
(
	const scalar ha,
	const scalar p,
	const scalar T0
) const
{
	return T
	(
		ha,
		p,
		T0,
		&thermo<Thermo, Type>::Ha,
		&thermo<Thermo, Type>::Cp,
		&thermo<Thermo, Type>::limit
	);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::TEs
(
	const scalar es,
	const scalar p,
	const scalar T0
) const
{
	return T
	(
		es,
		p,
		T0,
		&thermo<Thermo, Type>::Es,
		&thermo<Thermo, Type>::Cv,
		&thermo<Thermo, Type>::limit
	);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar tnbLib::species::thermo<Thermo, Type>::TEa
(
	const scalar ea,
	const scalar p,
	const scalar T0
) const
{
	return T
	(
		ea,
		p,
		T0,
		&thermo<Thermo, Type>::Ea,
		&thermo<Thermo, Type>::Cv,
		&thermo<Thermo, Type>::limit
	);
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::dKcdTbyKc
(
	const scalar p,
	const scalar T
) const
{
	const scalar nm = this->Y() / this->W();

	if (equal(nm, small))
	{
		return -this->dGdT(Pstd, T)*this->Y() / RR;
	}
	else
	{
		return -(nm / T + this->dGdT(Pstd, T)*this->Y() / RR);
	}
}


template<class Thermo, template<class> class Type>
inline tnbLib::scalar
tnbLib::species::thermo<Thermo, Type>::dcpdT(const scalar p, const scalar T) const
{
	return this->dCpdT(p, T)*this->W();;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline void tnbLib::species::thermo<Thermo, Type>::operator+=
(
	const thermo<Thermo, Type>& st
	)
{
	Thermo::operator+=(st);
}


template<class Thermo, template<class> class Type>
inline void tnbLib::species::thermo<Thermo, Type>::operator*=(const scalar s)
{
	Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline tnbLib::species::thermo<Thermo, Type> tnbLib::species::operator+
(
	const thermo<Thermo, Type>& st1,
	const thermo<Thermo, Type>& st2
	)
{
	return thermo<Thermo, Type>
		(
			static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
			);
}


template<class Thermo, template<class> class Type>
inline tnbLib::species::thermo<Thermo, Type> tnbLib::species::operator*
(
	const scalar s,
	const thermo<Thermo, Type>& st
	)
{
	return thermo<Thermo, Type>
		(
			s*static_cast<const Thermo&>(st)
			);
}


template<class Thermo, template<class> class Type>
inline tnbLib::species::thermo<Thermo, Type> tnbLib::species::operator==
(
	const thermo<Thermo, Type>& st1,
	const thermo<Thermo, Type>& st2
	)
{
	return thermo<Thermo, Type>
		(
			static_cast<const Thermo&>(st1) == static_cast<const Thermo&>(st2)
			);
}


// ************************************************************************* //