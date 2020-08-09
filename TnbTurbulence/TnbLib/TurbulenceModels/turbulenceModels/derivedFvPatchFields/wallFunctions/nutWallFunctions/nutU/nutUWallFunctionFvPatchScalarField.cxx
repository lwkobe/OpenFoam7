#include <nutUWallFunctionFvPatchScalarField.hxx>

#include <turbulenceModel.hxx>
#include <fvPatchFieldMapper.hxx>
#include <volFields.hxx>
#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

	tmp<scalarField> nutUWallFunctionFvPatchScalarField::nut() const
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
		const tmp<scalarField> tnuw = turbModel.nu(patchi);
		const scalarField& nuw = tnuw();

		const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
		const scalarField magUp(mag(Uw.patchInternalField() - Uw));

		const scalarField yPlus(this->yPlus(magUp));

		tmp<scalarField> tnutw(new scalarField(patch().size(), 0.0));
		scalarField& nutw = tnutw.ref();

		forAll(yPlus, facei)
		{
			if (yPlus[facei] > yPlusLam_)
			{
				nutw[facei] =
					nuw[facei] * (yPlus[facei] * kappa_ / log(E_*yPlus[facei]) - 1);
			}
		}

		return tnutw;
	}


	tmp<scalarField> nutUWallFunctionFvPatchScalarField::yPlus
	(
		const scalarField& magUp
	) const
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
		const tmp<scalarField> tnuw = turbModel.nu(patchi);
		const scalarField& nuw = tnuw();

		tmp<scalarField> tyPlus(new scalarField(patch().size(), 0.0));
		scalarField& yPlus = tyPlus.ref();

		forAll(yPlus, facei)
		{
			const scalar Re = magUp[facei] * y[facei] / nuw[facei];
			const scalar ryPlusLam = 1 / yPlusLam_;

			int iter = 0;
			scalar yp = yPlusLam_;
			scalar yPlusLast = yp;

			do
			{
				yPlusLast = yp;
				if (yp > yPlusLam_)
				{
					yp = (kappa_*Re + yp) / (1 + log(E_*yp));
				}
				else
				{
					yp = sqrt(Re);
				}
			} while (mag(ryPlusLam*(yp - yPlusLast)) > 0.0001 && ++iter < 20);

			yPlus[facei] = yp;
		}

		return tyPlus;
	}


	// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

	nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
	(
		const fvPatch& p,
		const DimensionedField<scalar, volMesh>& iF
	)
		:
		nutWallFunctionFvPatchScalarField(p, iF)
	{}


	nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
	(
		const nutUWallFunctionFvPatchScalarField& ptf,
		const fvPatch& p,
		const DimensionedField<scalar, volMesh>& iF,
		const fvPatchFieldMapper& mapper
	)
		:
		nutWallFunctionFvPatchScalarField(ptf, p, iF, mapper)
	{}


	nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
	(
		const fvPatch& p,
		const DimensionedField<scalar, volMesh>& iF,
		const dictionary& dict
	)
		:
		nutWallFunctionFvPatchScalarField(p, iF, dict)
	{}


	nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
	(
		const nutUWallFunctionFvPatchScalarField& sawfpsf
	)
		:
		nutWallFunctionFvPatchScalarField(sawfpsf)
	{}


	nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
	(
		const nutUWallFunctionFvPatchScalarField& sawfpsf,
		const DimensionedField<scalar, volMesh>& iF
	)
		:
		nutWallFunctionFvPatchScalarField(sawfpsf, iF)
	{}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	tmp<scalarField> nutUWallFunctionFvPatchScalarField::yPlus() const
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
		const scalarField magUp(mag(Uw.patchInternalField() - Uw));

		return yPlus(magUp);
	}


	void nutUWallFunctionFvPatchScalarField::write(Ostream& os) const
	{
		fvPatchField<scalar>::write(os);
		writeLocalEntries(os);
		writeEntry(os, "value", *this);
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	makePatchTypeField
	(
		fvPatchScalarField,
		nutUWallFunctionFvPatchScalarField
	);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //