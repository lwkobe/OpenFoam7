#include <turbulentTemperatureRadCoupledMixedFvPatchScalarField.hxx>

#include <addToRunTimeSelectionTable.hxx>
#include <fvPatchFieldMapper.hxx>
#include <volFields.hxx>
#include <mappedPatchBase.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{

		// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

		turbulentTemperatureRadCoupledMixedFvPatchScalarField::
			turbulentTemperatureRadCoupledMixedFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF
			)
			:
			mixedFvPatchScalarField(p, iF),
			temperatureCoupledBase(patch(), "undefined", "undefined", "undefined-K"),
			TnbrName_("undefined-Tnbr"),
			qrNbrName_("undefined-qrNbr"),
			qrName_("undefined-qr"),
			thicknessLayers_(0),
			kappaLayers_(0),
			contactRes_(0)
		{
			this->refValue() = 0.0;
			this->refGrad() = 0.0;
			this->valueFraction() = 1.0;
		}


		turbulentTemperatureRadCoupledMixedFvPatchScalarField::
			turbulentTemperatureRadCoupledMixedFvPatchScalarField
			(
				const turbulentTemperatureRadCoupledMixedFvPatchScalarField& psf,
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const fvPatchFieldMapper& mapper
			)
			:
			mixedFvPatchScalarField(psf, p, iF, mapper),
			temperatureCoupledBase(patch(), psf),
			TnbrName_(psf.TnbrName_),
			qrNbrName_(psf.qrNbrName_),
			qrName_(psf.qrName_),
			thicknessLayers_(psf.thicknessLayers_),
			kappaLayers_(psf.kappaLayers_),
			contactRes_(psf.contactRes_)
		{}


		turbulentTemperatureRadCoupledMixedFvPatchScalarField::
			turbulentTemperatureRadCoupledMixedFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const dictionary& dict
			)
			:
			mixedFvPatchScalarField(p, iF),
			temperatureCoupledBase(patch(), dict),
			TnbrName_(dict.lookupOrDefault<word>("Tnbr", "T")),
			qrNbrName_(dict.lookupOrDefault<word>("qrNbr", "none")),
			qrName_(dict.lookupOrDefault<word>("qr", "none")),
			thicknessLayers_(0),
			kappaLayers_(0),
			contactRes_(0.0)
		{
			if (!isA<mappedPatchBase>(this->patch().patch()))
			{
				FatalErrorInFunction
					<< "' not type '" << mappedPatchBase::typeName << "'"
					<< "\n    for patch " << p.name()
					<< " of field " << internalField().name()
					<< " in file " << internalField().objectPath()
					<< exit(FatalError);
			}

			if (dict.found("thicknessLayers"))
			{
				dict.lookup("thicknessLayers") >> thicknessLayers_;
				dict.lookup("kappaLayers") >> kappaLayers_;

				if (thicknessLayers_.size() > 0)
				{
					// Calculate effective thermal resistance by harmonic averaging
					forAll(thicknessLayers_, iLayer)
					{
						contactRes_ += thicknessLayers_[iLayer] / kappaLayers_[iLayer];
					}
					contactRes_ = 1.0 / contactRes_;
				}
			}

			fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

			if (dict.found("refValue"))
			{
				// Full restart
				refValue() = scalarField("refValue", dict, p.size());
				refGrad() = scalarField("refGradient", dict, p.size());
				valueFraction() = scalarField("valueFraction", dict, p.size());
			}
			else
			{
				// Start from user entered data. Assume fixedValue.
				refValue() = *this;
				refGrad() = 0.0;
				valueFraction() = 1.0;
			}
		}


		turbulentTemperatureRadCoupledMixedFvPatchScalarField::
			turbulentTemperatureRadCoupledMixedFvPatchScalarField
			(
				const turbulentTemperatureRadCoupledMixedFvPatchScalarField& psf,
				const DimensionedField<scalar, volMesh>& iF
			)
			:
			mixedFvPatchScalarField(psf, iF),
			temperatureCoupledBase(patch(), psf),
			TnbrName_(psf.TnbrName_),
			qrNbrName_(psf.qrNbrName_),
			qrName_(psf.qrName_),
			thicknessLayers_(psf.thicknessLayers_),
			kappaLayers_(psf.kappaLayers_),
			contactRes_(psf.contactRes_)
		{}


		// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

		void turbulentTemperatureRadCoupledMixedFvPatchScalarField::updateCoeffs()
		{
			if (updated())
			{
				return;
			}

			// Since we're inside initEvaluate/evaluate there might be processor
			// comms underway. Change the tag we use.
			int oldTag = UPstream::msgType();
			UPstream::msgType() = oldTag + 1;

			// Get the coupling information from the mappedPatchBase
			const mappedPatchBase& mpp =
				refCast<const mappedPatchBase>(patch().patch());
			const polyMesh& nbrMesh = mpp.sampleMesh();
			const label samplePatchi = mpp.samplePolyPatch().index();
			const fvPatch& nbrPatch =
				refCast<const fvMesh>(nbrMesh).boundary()[samplePatchi];


			scalarField Tc(patchInternalField());
			scalarField& Tp = *this;

			typedef turbulentTemperatureRadCoupledMixedFvPatchScalarField thisType;

			const fvPatchScalarField& nbrTp =
				nbrPatch.lookupPatchField<volScalarField, scalar>(TnbrName_);

			if (!isA<thisType>(nbrTp))
			{
				FatalErrorInFunction
					<< "Patch field for " << internalField().name() << " on "
					<< patch().name() << " is of type " << thisType::typeName
					<< endl << "The neighbouring patch field " << TnbrName_ << " on "
					<< nbrPatch.name() << " is required to be the same, but is "
					<< "currently of type " << nbrTp.type() << exit(FatalError);
			}

			const thisType& nbrField = refCast<const thisType>(nbrTp);

			// Swap to obtain full local values of neighbour internal field
			scalarField TcNbr(nbrField.patchInternalField());
			mpp.distribute(TcNbr);


			// Swap to obtain full local values of neighbour K*delta
			scalarField KDeltaNbr;
			if (contactRes_ == 0.0)
			{
				KDeltaNbr = nbrField.kappa(nbrField)*nbrPatch.deltaCoeffs();
			}
			else
			{
				KDeltaNbr.setSize(nbrField.size(), contactRes_);
			}
			mpp.distribute(KDeltaNbr);

			scalarField KDelta(kappa(Tp)*patch().deltaCoeffs());

			scalarField qr(Tp.size(), 0.0);
			if (qrName_ != "none")
			{
				qr = patch().lookupPatchField<volScalarField, scalar>(qrName_);
			}

			scalarField qrNbr(Tp.size(), 0.0);
			if (qrNbrName_ != "none")
			{
				qrNbr = nbrPatch.lookupPatchField<volScalarField, scalar>(qrNbrName_);
				mpp.distribute(qrNbr);
			}

			valueFraction() = KDeltaNbr / (KDeltaNbr + KDelta);
			refValue() = TcNbr;
			refGrad() = (qr + qrNbr) / kappa(Tp);

			mixedFvPatchScalarField::updateCoeffs();

			if (debug)
			{
				scalar Q = gSum(kappa(Tp)*patch().magSf()*snGrad());

				Info << patch().boundaryMesh().mesh().name() << ':'
					<< patch().name() << ':'
					<< this->internalField().name() << " <- "
					<< nbrMesh.name() << ':'
					<< nbrPatch.name() << ':'
					<< this->internalField().name() << " :"
					<< " heat transfer rate:" << Q
					<< " walltemperature "
					<< " min:" << gMin(Tp)
					<< " max:" << gMax(Tp)
					<< " avg:" << gAverage(Tp)
					<< endl;
			}

			// Restore tag
			UPstream::msgType() = oldTag;
		}


		void turbulentTemperatureRadCoupledMixedFvPatchScalarField::write
		(
			Ostream& os
		) const
		{
			mixedFvPatchScalarField::write(os);
			writeEntry(os, "Tnbr", TnbrName_);
			writeEntry(os, "qrNbr", qrNbrName_);
			writeEntry(os, "qr", qrName_);
			writeEntry(os, "thicknessLayers", thicknessLayers_);
			writeEntry(os, "kappaLayers", kappaLayers_);

			temperatureCoupledBase::write(os);
		}


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		makePatchTypeField
		(
			fvPatchScalarField,
			turbulentTemperatureRadCoupledMixedFvPatchScalarField
		);


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace compressible
} // End namespace tnbLib


// ************************************************************************* //