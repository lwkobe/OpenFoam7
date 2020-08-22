#include <angularOscillatingDisplacementPointPatchVectorField.hxx>

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

	angularOscillatingDisplacementPointPatchVectorField::
		angularOscillatingDisplacementPointPatchVectorField
		(
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF
		)
		:
		fixedValuePointPatchField<vector>(p, iF),
		axis_(Zero),
		origin_(Zero),
		angle0_(0.0),
		amplitude_(0.0),
		omega_(0.0),
		p0_(p.localPoints())
	{}


	angularOscillatingDisplacementPointPatchVectorField::
		angularOscillatingDisplacementPointPatchVectorField
		(
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF,
			const dictionary& dict
		)
		:
		fixedValuePointPatchField<vector>(p, iF, dict),
		axis_(dict.lookup("axis")),
		origin_(dict.lookup("origin")),
		angle0_(readScalar(dict.lookup("angle0"))),
		amplitude_(readScalar(dict.lookup("amplitude"))),
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


	angularOscillatingDisplacementPointPatchVectorField::
		angularOscillatingDisplacementPointPatchVectorField
		(
			const angularOscillatingDisplacementPointPatchVectorField& ptf,
			const pointPatch& p,
			const DimensionedField<vector, pointMesh>& iF,
			const pointPatchFieldMapper& mapper
		)
		:
		fixedValuePointPatchField<vector>(ptf, p, iF, mapper),
		axis_(ptf.axis_),
		origin_(ptf.origin_),
		angle0_(ptf.angle0_),
		amplitude_(ptf.amplitude_),
		omega_(ptf.omega_),
		p0_(mapper(ptf.p0_))
	{}


	angularOscillatingDisplacementPointPatchVectorField::
		angularOscillatingDisplacementPointPatchVectorField
		(
			const angularOscillatingDisplacementPointPatchVectorField& ptf,
			const DimensionedField<vector, pointMesh>& iF
		)
		:
		fixedValuePointPatchField<vector>(ptf, iF),
		axis_(ptf.axis_),
		origin_(ptf.origin_),
		angle0_(ptf.angle0_),
		amplitude_(ptf.amplitude_),
		omega_(ptf.omega_),
		p0_(ptf.p0_)
	{}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	void angularOscillatingDisplacementPointPatchVectorField::autoMap
	(
		const pointPatchFieldMapper& m
	)
	{
		fixedValuePointPatchField<vector>::autoMap(m);

		m(p0_, p0_);
	}


	void angularOscillatingDisplacementPointPatchVectorField::rmap
	(
		const pointPatchField<vector>& ptf,
		const labelList& addr
	)
	{
		const angularOscillatingDisplacementPointPatchVectorField& aODptf =
			refCast<const angularOscillatingDisplacementPointPatchVectorField>(ptf);

		fixedValuePointPatchField<vector>::rmap(aODptf, addr);

		p0_.rmap(aODptf.p0_, addr);
	}


	void angularOscillatingDisplacementPointPatchVectorField::updateCoeffs()
	{
		if (this->updated())
		{
			return;
		}

		const polyMesh& mesh = this->internalField().mesh()();
		const Time& t = mesh.time();

		scalar angle = angle0_ + amplitude_ * sin(omega_*t.value());
		vector axisHat = axis_ / mag(axis_);
		vectorField p0Rel(p0_ - origin_);

		vectorField::operator=
			(
				p0Rel*(cos(angle) - 1)
				+ (axisHat ^ p0Rel*sin(angle))
				+ (axisHat & p0Rel)*(1 - cos(angle))*axisHat
				);

		fixedValuePointPatchField<vector>::updateCoeffs();
	}


	void angularOscillatingDisplacementPointPatchVectorField::write
	(
		Ostream& os
	) const
	{
		pointPatchField<vector>::write(os);
		writeEntry(os, "axis", axis_);
		writeEntry(os, "origin", origin_);
		writeEntry(os, "angle0", angle0_);
		writeEntry(os, "amplitude", amplitude_);
		writeEntry(os, "omega", omega_);
		writeEntry(os, "p0", p0_);
		writeEntry(os, "value", *this);
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	makePointPatchTypeField
	(
		pointPatchVectorField,
		angularOscillatingDisplacementPointPatchVectorField
	);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //