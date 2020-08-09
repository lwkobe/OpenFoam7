#include <fWallFunctionFvPatchScalarField.hxx>

#include <turbulenceModel.hxx>
#include <nutWallFunctionFvPatchScalarField.hxx>
#include <v2f.hxx>
#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace RASModels
	{

		// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

		fWallFunctionFvPatchScalarField::fWallFunctionFvPatchScalarField
		(
			const fvPatch& p,
			const DimensionedField<scalar, volMesh>& iF
		)
			:
			fixedValueFvPatchField<scalar>(p, iF)
		{}


		fWallFunctionFvPatchScalarField::fWallFunctionFvPatchScalarField
		(
			const fWallFunctionFvPatchScalarField& ptf,
			const fvPatch& p,
			const DimensionedField<scalar, volMesh>& iF,
			const fvPatchFieldMapper& mapper
		)
			:
			fixedValueFvPatchField<scalar>(ptf, p, iF, mapper)
		{}


		fWallFunctionFvPatchScalarField::fWallFunctionFvPatchScalarField
		(
			const fvPatch& p,
			const DimensionedField<scalar, volMesh>& iF,
			const dictionary& dict
		)
			:
			fixedValueFvPatchField<scalar>(p, iF, dict)
		{}


		fWallFunctionFvPatchScalarField::fWallFunctionFvPatchScalarField
		(
			const fWallFunctionFvPatchScalarField& v2wfpsf
		)
			:
			fixedValueFvPatchField<scalar>(v2wfpsf)
		{}


		fWallFunctionFvPatchScalarField::fWallFunctionFvPatchScalarField
		(
			const fWallFunctionFvPatchScalarField& v2wfpsf,
			const DimensionedField<scalar, volMesh>& iF
		)
			:
			fixedValueFvPatchField<scalar>(v2wfpsf, iF)
		{}


		// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

		void fWallFunctionFvPatchScalarField::updateCoeffs()
		{
			if (updated())
			{
				return;
			}

			const label patchi = patch().index();

			const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
				(
					IOobject::groupName
					(
						turbulenceModel::propertiesName,
						internalField().group()
					)
					);
			const v2fBase& v2fModel = refCast<const v2fBase>(turbModel);

			const nutWallFunctionFvPatchScalarField& nutw =
				nutWallFunctionFvPatchScalarField::nutw(turbModel, patchi);

			const scalarField& y = turbModel.y()[patchi];

			const tmp<volScalarField> tk = turbModel.k();
			const volScalarField& k = tk();

			const tmp<volScalarField> tepsilon = turbModel.epsilon();
			const volScalarField& epsilon = tepsilon();

			const tmp<volScalarField> tv2 = v2fModel.v2();
			const volScalarField& v2 = tv2();

			const tmp<scalarField> tnuw = turbModel.nu(patchi);
			const scalarField& nuw = tnuw();

			const scalar Cmu25 = pow025(nutw.Cmu());

			scalarField& f = *this;

			// Set f wall values
			forAll(f, facei)
			{
				label celli = patch().faceCells()[facei];

				scalar uTau = Cmu25 * sqrt(k[celli]);

				scalar yPlus = uTau * y[facei] / nuw[facei];

				if (yPlus > nutw.yPlusLam())
				{
					scalar N = 6.0;
					scalar v2c = v2[celli];
					scalar epsc = epsilon[celli];
					scalar kc = k[celli];

					f[facei] = N * v2c*epsc / (sqr(kc) + rootVSmall);
					f[facei] /= sqr(uTau) + rootVSmall;
				}
				else
				{
					f[facei] = 0.0;
				}
			}

			fixedValueFvPatchField<scalar>::updateCoeffs();

			// TODO: perform averaging for cells sharing more than one boundary face
		}


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		makePatchTypeField
		(
			fvPatchScalarField,
			fWallFunctionFvPatchScalarField
		);

		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace RASModels
} // End namespace tnbLib

// ************************************************************************* //