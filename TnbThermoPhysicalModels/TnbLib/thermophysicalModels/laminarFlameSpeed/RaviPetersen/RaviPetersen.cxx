#include <RaviPetersen.hxx>

#include <addToRunTimeSelectionTable.hxx>
#include <Time.hxx>  // added by amir

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace laminarFlameSpeedModels
	{
		defineTypeNameAndDebug(RaviPetersen, 0);

		addToRunTimeSelectionTable
		(
			laminarFlameSpeed,
			RaviPetersen,
			dictionary
		);
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::laminarFlameSpeedModels::RaviPetersen::RaviPetersen
(
	const dictionary& dict,
	const psiuReactionThermo& ct
)
	:
	laminarFlameSpeed(dict, ct),
	coeffsDict_(dict.optionalSubDict(typeName + "Coeffs").subDict(fuel_)),
	pPoints_(coeffsDict_.lookup("pPoints")),
	EqRPoints_(coeffsDict_.lookup("EqRPoints")),
	alpha_(coeffsDict_.lookup("alpha")),
	beta_(coeffsDict_.lookup("beta")),
	TRef_(readScalar(coeffsDict_.lookup("TRef")))
{
	checkPointsMonotonicity("equivalenceRatio", EqRPoints_);
	checkPointsMonotonicity("pressure", pPoints_);
	checkCoefficientArrayShape("alpha", alpha_);
	checkCoefficientArrayShape("beta", beta_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tnbLib::laminarFlameSpeedModels::RaviPetersen::~RaviPetersen()
{}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void tnbLib::laminarFlameSpeedModels::RaviPetersen::checkPointsMonotonicity
(
	const word& name,
	const List<scalar>& x
) const
{
	for (label i = 1; i < x.size(); i++)
	{
		if (x[i] <= x[i - 1])
		{
			FatalIOErrorInFunction
			(
				coeffsDict_
			) << "Data points for the " << name
				<< " do not increase monotonically" << endl
				<< exit(FatalIOError);
		}
	}
}


void tnbLib::laminarFlameSpeedModels::RaviPetersen::checkCoefficientArrayShape
(
	const word& name,
	const List<List<List<scalar>>>& x
) const
{
	bool ok = true;

	ok &= x.size() == EqRPoints_.size() - 1;

	forAll(x, i)
	{
		ok &= x[i].size() == pPoints_.size();

		forAll(x[i], j)
		{
			ok &= x[i][j].size() == x[i][0].size();
		}
	}

	if (!ok)
	{
		FatalIOErrorInFunction
		(
			coeffsDict_
		) << "Inconsistent size of " << name << " coefficients array" << endl
			<< exit(FatalIOError);
	}
}


inline bool tnbLib::laminarFlameSpeedModels::RaviPetersen::interval
(
	const List<scalar>& xPoints,
	const scalar x,
	label& xIndex,
	scalar& xXi,
	scalar& xLim
) const
{
	if (x < xPoints.first())
	{
		xIndex = 0;
		xXi = 0.0;
		xLim = xPoints.first();
		return false;
	}

	else if (x > xPoints.last())
	{
		xIndex = xPoints.size() - 2;
		xXi = 1.0;
		xLim = xPoints.last();
		return false;
	}

	for (xIndex = 0; x > xPoints[xIndex + 1]; xIndex++)
	{
		// increment xIndex until xPoints[xIndex] < x < xPoints[xIndex+1]
	}

	xXi = (x - xPoints[xIndex]) / (xPoints[xIndex + 1] - xPoints[xIndex]);
	xLim = x;

	return true;
}


inline tnbLib::scalar tnbLib::laminarFlameSpeedModels::RaviPetersen::polynomial
(
	const List<scalar>& coeffs,
	const scalar x
) const
{
	scalar xPow = 1.0;
	scalar y = 0.0;
	forAll(coeffs, i)
	{
		y += coeffs[i] * xPow;
		xPow *= x;
	}
	return y;
}


inline tnbLib::scalar tnbLib::laminarFlameSpeedModels::RaviPetersen::dPolynomial
(
	const List<scalar>& coeffs,
	const scalar x
) const
{
	scalar xPow = 1.0;
	scalar y = 0.0;
	for (label i = 1; i < coeffs.size(); i++)
	{
		y += i * coeffs[i] * xPow;
		xPow *= x;
	}
	return y;
}


inline tnbLib::scalar tnbLib::laminarFlameSpeedModels::RaviPetersen::THatPowB
(
	const label EqRIndex,
	const label pIndex,
	const scalar EqR,
	const scalar Tu
) const
{
	return pow
	(
		Tu / TRef_,
		polynomial(beta_[EqRIndex][pIndex], EqR)
	);
}


inline tnbLib::scalar
tnbLib::laminarFlameSpeedModels::RaviPetersen::correlationInRange
(
	const label EqRIndex,
	const label pIndex,
	const scalar EqR,
	const scalar Tu
) const
{
	// standard correlation
	return
		polynomial(alpha_[EqRIndex][pIndex], EqR)
		*THatPowB(EqRIndex, pIndex, EqR, Tu);
}


inline tnbLib::scalar
tnbLib::laminarFlameSpeedModels::RaviPetersen::correlationOutOfRange
(
	const label EqRIndex,
	const label pIndex,
	const scalar EqR,
	const scalar EqRLim,
	const scalar Tu
) const
{
	scalar A = polynomial(alpha_[EqRIndex][pIndex], EqRLim);
	scalar dA = dPolynomial(alpha_[EqRIndex][pIndex], EqRLim);
	scalar dB = dPolynomial(beta_[EqRIndex][pIndex], EqRLim);
	scalar TB = THatPowB(EqRIndex, pIndex, EqRLim, Tu);

	// linear extrapolation from the bounds of the correlation
	return max(TB*(A + (dA + A * log(Tu / TRef_)*dB)*(EqR - EqRLim)), 0.0);
}


inline tnbLib::scalar tnbLib::laminarFlameSpeedModels::RaviPetersen::speed
(
	const scalar EqR,
	const scalar p,
	const scalar Tu
) const
{
	scalar Su = 0, s;

	label EqRIndex, pIndex;
	scalar EqRXi, pXi;
	scalar EqRLim, pLim;
	bool EqRInRange;

	EqRInRange = interval(EqRPoints_, EqR, EqRIndex, EqRXi, EqRLim);

	interval(pPoints_, p, pIndex, pXi, pLim);

	for (label pI = 0; pI < 2; pI++)
	{
		if (EqRInRange)
		{
			s = correlationInRange(EqRIndex, pIndex + pI, EqR, Tu);
		}
		else
		{
			s = correlationOutOfRange(EqRIndex, pIndex + pI, EqR, EqRLim, Tu);
		}

		Su += (1 - pXi)*s;
		pXi = 1 - pXi;
	}

	return Su;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

tnbLib::tmp<tnbLib::volScalarField>
tnbLib::laminarFlameSpeedModels::RaviPetersen::operator()() const
{
	const volScalarField& p = psiuReactionThermo_.p();
	const volScalarField& Tu = psiuReactionThermo_.Tu();

	volScalarField EqR
	(
		IOobject
		(
			"EqR",
			p.time().timeName(),
			p.db(),
			IOobject::NO_READ,
			IOobject::NO_WRITE,
			false
		),
		p.mesh(),
		dimensionedScalar(dimless, 0)
	);

	if (psiuReactionThermo_.composition().contains("ft"))
	{
		const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");

		EqR =
			dimensionedScalar
			(
				psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
			)*ft / max(1 - ft, small);
	}
	else
	{
		EqR = equivalenceRatio_;
	}

	tmp<volScalarField> tSu0
	(
		volScalarField::New
		(
			"Su0",
			p.mesh(),
			dimensionedScalar(dimVelocity, 0)
		)
	);

	volScalarField& Su0 = tSu0.ref();

	forAll(Su0, celli)
	{
		Su0[celli] = speed(EqR[celli], p[celli], Tu[celli]);
	}

	return tSu0;
}


// ************************************************************************* //