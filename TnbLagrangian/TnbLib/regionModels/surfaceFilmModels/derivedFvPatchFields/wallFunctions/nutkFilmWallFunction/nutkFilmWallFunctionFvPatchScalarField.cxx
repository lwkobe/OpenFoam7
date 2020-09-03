#include <nutkFilmWallFunctionFvPatchScalarField.hxx>

#include <fvPatchFieldMapper.hxx>
#include <volFields.hxx>
#include <turbulentFluidThermoModel.hxx>
#include <addToRunTimeSelectionTable.hxx>
#include <surfaceFilmRegionModel.hxx>
#include <mappedWallPointPatch.hxx>
#include <mapDistribute.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{
		namespace RASModels
		{

			// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

			tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::calcUTau
			(
				const scalarField& magGradU
			) const
			{
				tmp<scalarField> tuTau(new scalarField(patch().size(), 0.0));
				scalarField& uTau = tuTau.ref();

				typedef regionModels::surfaceFilmModels::surfaceFilmRegionModel modelType;

				bool foundFilm =
					db().time().foundObject<modelType>("surfaceFilmProperties");

				if (!foundFilm)
				{
					// Do nothing on construction - film model doesn't exist yet
					return tuTau;
				}

				const label patchi = patch().index();

				// Retrieve phase change mass from surface film model
				const modelType& filmModel =
					db().time().lookupObject<modelType>("surfaceFilmProperties");

				const label filmPatchi = filmModel.regionPatchID(patchi);

				tmp<volScalarField> mDotFilm(filmModel.primaryMassTrans());
				scalarField mDotFilmp = mDotFilm().boundaryField()[filmPatchi];
				filmModel.toPrimary(filmPatchi, mDotFilmp);


				// Retrieve RAS turbulence model
				const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
					(
						IOobject::groupName
						(
							turbulenceModel::propertiesName,
							internalField().group()
						)
						);

				const scalarField& y = turbModel.y()[patchi];
				const tmp<volScalarField> tk = turbModel.k();
				const volScalarField& k = tk();
				const tmp<scalarField> tnuw = turbModel.nu(patchi);
				const scalarField& nuw = tnuw();

				const scalar Cmu25 = pow(Cmu_, 0.25);

				forAll(uTau, facei)
				{
					label faceCelli = patch().faceCells()[facei];

					scalar ut = Cmu25 * sqrt(k[faceCelli]);

					scalar yPlus = y[facei] * ut / nuw[facei];

					scalar mStar = mDotFilmp[facei] / (y[facei] * ut);

					scalar factor = 0.0;
					if (yPlus > yPlusCrit_)
					{
						scalar expTerm = exp(min(50.0, B_*mStar));
						scalar powTerm = pow(yPlus, mStar / kappa_);
						factor = mStar / (expTerm*powTerm - 1.0 + rootVSmall);
					}
					else
					{
						scalar expTerm = exp(min(50.0, mStar));
						factor = mStar / (expTerm*yPlus - 1.0 + rootVSmall);
					}

					uTau[facei] = sqrt(max(0, magGradU[facei] * ut*factor));
				}

				return tuTau;
			}


			tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::nut() const
			{
				const label patchi = patch().index();

				const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
					(
						IOobject::groupName
						(
							turbulenceModel::propertiesName,
							internalField().group()
						)
						);

				const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
				const scalarField magGradU(mag(Uw.snGrad()));
				const tmp<scalarField> tnuw = turbModel.nu(patchi);
				const scalarField& nuw = tnuw();

				return max
				(
					scalar(0),
					sqr(calcUTau(magGradU)) / (magGradU + rootVSmall) - nuw
				);
			}


			// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

			nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF
			)
				:
				nutkWallFunctionFvPatchScalarField(p, iF),
				B_(5.5),
				yPlusCrit_(11.05)
			{}


			nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
			(
				const nutkFilmWallFunctionFvPatchScalarField& ptf,
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const fvPatchFieldMapper& mapper
			)
				:
				nutkWallFunctionFvPatchScalarField(ptf, p, iF, mapper),
				B_(5.5),
				yPlusCrit_(11.05)
			{}


			nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const dictionary& dict
			)
				:
				nutkWallFunctionFvPatchScalarField(p, iF, dict),
				B_(dict.lookupOrDefault("B", 5.5)),
				yPlusCrit_(dict.lookupOrDefault("yPlusCrit", 11.05))
			{}


			nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
			(
				const nutkFilmWallFunctionFvPatchScalarField& wfpsf
			)
				:
				nutkWallFunctionFvPatchScalarField(wfpsf),
				B_(wfpsf.B_),
				yPlusCrit_(wfpsf.yPlusCrit_)
			{}


			nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
			(
				const nutkFilmWallFunctionFvPatchScalarField& wfpsf,
				const DimensionedField<scalar, volMesh>& iF
			)
				:
				nutkWallFunctionFvPatchScalarField(wfpsf, iF),
				B_(wfpsf.B_),
				yPlusCrit_(wfpsf.yPlusCrit_)
			{}


			// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

			tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::yPlus() const
			{
				const label patchi = patch().index();

				const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
					(
						IOobject::groupName
						(
							turbulenceModel::propertiesName,
							internalField().group()
						)
						);

				const scalarField& y = turbModel.y()[patchi];
				const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
				const tmp<scalarField> tnuw = turbModel.nu(patchi);
				const scalarField& nuw = tnuw();

				return y * calcUTau(mag(Uw.snGrad())) / nuw;
			}


			void nutkFilmWallFunctionFvPatchScalarField::write(Ostream& os) const
			{
				fvPatchField<scalar>::write(os);
				writeLocalEntries(os);
				writeEntry(os, "B", B_);
				writeEntry(os, "yPlusCrit", yPlusCrit_);
				writeEntry(os, "value", *this);
			}


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

			makePatchTypeField(fvPatchScalarField, nutkFilmWallFunctionFvPatchScalarField);

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace RASModels
	} // End namespace compressible
} // End namespace tnbLib

// ************************************************************************* //