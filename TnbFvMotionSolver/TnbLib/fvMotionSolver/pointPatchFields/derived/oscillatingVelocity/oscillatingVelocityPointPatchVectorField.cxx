#include <oscillatingVelocityPointPatchVectorField.hxx>

#include <pointPatchFields.hxx>
#include <addToRunTimeSelectionTable.hxx>
#include <Time.hxx>
#include <polyMesh.hxx>

#include <pointPatchFieldMapper.hxx>  // added by amir
#include <pointMesh.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

	oscillatingVelocityPointPatchVectorField::
		oscillatingVelocityPointPatchVectorField
		(
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF
		)
		:
		fixedValuePointPatchField<vector>(p, iF),
		amplitude_(Zero),
		omega_(0.0),
		p0_(p.localPoints())
	{}


	oscillatingVelocityPointPatchVectorField::
		oscillatingVelocityPointPatchVectorField
		(
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF,
			const dictionary& dict
		)
		:
		fixedValuePointPatchField<vector>(p, iF, dict),
		amplitude_(dict.lookup("amplitude")),
		omega_(readScalar(dict.lookup("omega")))
	{
		if (!dict.found("value"))
		{
			updateCoeffs();
		}

		if (dict.found("p0"))
		{
			p0_ = vectorField("p0", dict, p.size());
		}
		else
		{
			p0_ = p.localPoints();
		}
	}


	oscillatingVelocityPointPatchVectorField::
		oscillatingVelocityPointPatchVectorField
		(
			const oscillatingVelocityPointPatchVectorField& ptf,
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF,
			const pointPatchFieldMapper& mapper
		)
		:
		fixedValuePointPatchField<vector>(ptf, p, iF, mapper),
		amplitude_(ptf.amplitude_),
		omega_(ptf.omega_),
		p0_(mapper(ptf.p0_))
	{}


	oscillatingVelocityPointPatchVectorField::
		oscillatingVelocityPointPatchVectorField
		(
			const oscillatingVelocityPointPatchVectorField& ptf,
			const DimensionedField<vector, pointMesh>& iF
		)
		:
		fixedValuePointPatchField<vector>(ptf, iF),
		amplitude_(ptf.amplitude_),
		omega_(ptf.omega_),
		p0_(ptf.p0_)
	{}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	void oscillatingVelocityPointPatchVectorField::autoMap
	(
		const pointPatchFieldMapper& m
	)
	{
		fixedValuePointPatchField<vector>::autoMap(m);

		m(p0_, p0_);
	}


	void oscillatingVelocityPointPatchVectorField::rmap
	(
		const pointPatchField<vector>& ptf,
		const labelList& addr
	)
	{
		const oscillatingVelocityPointPatchVectorField& oVptf =
			refCast<const oscillatingVelocityPointPatchVectorField>(ptf);

		fixedValuePointPatchField<vector>::rmap(oVptf, addr);

		p0_.rmap(oVptf.p0_, addr);
	}


	void oscillatingVelocityPointPatchVectorField::updateCoeffs()
	{
		if (this->updated())
		{
			return;
		}

		const polyMesh& mesh = this->internalField().mesh()();
		const Time& t = mesh.time();
		const pointPatch& p = this->patch();

		Field<vector>::operator=
			(
			(p0_ + amplitude_ * sin(omega_*t.value()) - p.localPoints())
				/ t.deltaTValue()
				);

		fixedValuePointPatchField<vector>::updateCoeffs();
	}


	void oscillatingVelocityPointPatchVectorField::write(Ostream& os) const
	{
		pointPatchField<vector>::write(os);
		writeEntry(os, "amplitude", amplitude_);
		writeEntry(os, "omega", omega_);
		writeEntry(os, "p0", p0_);
		writeEntry(os, "value", *this);
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	makePointPatchTypeField
	(
		pointPatchVectorField,
		oscillatingVelocityPointPatchVectorField
	);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //
