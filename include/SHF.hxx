#pragma once
#ifndef _SHF_Header
#define _SHF_Header

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
	tnbLib::SHF

Description
	Secondary Breakup Model to take account of the different breakup regimes,
	bag, molutimode, shear....

	Accurate description in
	@verbatim
	R. Schmehl, G. Maier, S. Witting
	"CFD Analysis of Fuel Atomization, Secondary Droplet Breakup and Spray
	Dispersion in the Premix Duct of a LPP Combustor".
	Eight International Conference on Liquid Atomization and Spray Systems, 2000
	@endverbatim

\*---------------------------------------------------------------------------*/

#include <BreakupModel.hxx>

#include <vector.hxx>  // added by amir

#ifdef FoamLagrangian_EXPORT_DEFINE
#define FoamSHF_EXPORT __declspec(dllexport)
#else
#ifdef FoamSHF_EXPORT_DEFINE
#define FoamSHF_EXPORT __declspec(dllexport)
#else
#define FoamSHF_EXPORT __declspec(dllimport)
#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	/*---------------------------------------------------------------------------*\
								Class SHF Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class SHF
		:
		public BreakupModel<CloudType>
	{
		// Private Data

			// Model constants

		scalar weCorrCoeff_;

		scalar weBuCrit_;
		scalar weBuBag_;
		scalar weBuMM_;

		scalar ohnCoeffCrit_;
		scalar ohnCoeffBag_;
		scalar ohnCoeffMM_;

		scalar ohnExpCrit_;
		scalar ohnExpBag_;
		scalar ohnExpMM_;

		scalar cInit_;

		scalar c1_;
		scalar c2_;
		scalar c3_;

		scalar cExp1_;
		scalar cExp2_;
		scalar cExp3_;

		scalar weConst_;
		scalar weCrit1_;
		scalar weCrit2_;

		scalar coeffD_;
		scalar onExpD_;
		scalar weExpD_;

		scalar mu_;
		scalar sigma_;

		scalar d32Coeff_;
		scalar cDmaxBM_;
		scalar cDmaxS_;

		scalar corePerc_;


	public:

		//- Runtime type information
		//TypeName("SHF");
		static const char* typeName_() { return "SHF"; }
		static FoamSHF_EXPORT const ::tnbLib::word typeName;
		static FoamSHF_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from dictionary
		SHF(const dictionary&, CloudType&);

		//- Construct copy
		SHF(const SHF<CloudType>& bum);

		//- Construct and return a clone
		virtual autoPtr<BreakupModel<CloudType>> clone() const
		{
			return autoPtr<BreakupModel<CloudType>>
				(
					new SHF<CloudType>(*this)
					);
		}


		//- Destructor
		virtual ~SHF();


		// Member Functions

			//- Update the parcel properties
		virtual bool update
		(
			const scalar dt,
			const vector& g,
			scalar& d,
			scalar& tc,
			scalar& ms,
			scalar& nParticle,
			scalar& KHindex,
			scalar& y,
			scalar& yDot,
			const scalar d0,
			const scalar rho,
			const scalar mu,
			const scalar sigma,
			const vector& U,
			const scalar rhoc,
			const scalar muc,
			const vector& Urel,
			const scalar Urmag,
			const scalar tMom,
			scalar& dChild,
			scalar& massChild
		);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <SHFI.hxx>

//#ifdef NoRepository
//#include <SHF.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_SHF_Header
