#include <processorCyclicFvPatchField.hxx>

#include <processorCyclicFvPatch.hxx>
#include <demandDrivenData.hxx>
#include <transformField.hxx>

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const fvPatch& p,
	const DimensionedField<Type, volMesh>& iF
)
	:
	processorFvPatchField<Type>(p, iF),
	procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const fvPatch& p,
	const DimensionedField<Type, volMesh>& iF,
	const dictionary& dict
)
	:
	processorFvPatchField<Type>(p, iF, dict),
	procPatch_(refCast<const processorCyclicFvPatch>(p))
{
	if (!isType<processorCyclicFvPatch>(p))
	{
		FatalIOErrorInFunction
		(
			dict
		) << "\n    patch type '" << p.type()
			<< "' not constraint type '" << typeName << "'"
			<< "\n    for patch " << p.name()
			<< " of field " << this->internalField().name()
			<< " in file " << this->internalField().objectPath()
			<< exit(FatalIOError);
	}

	if (Pstream::defaultCommsType == Pstream::commsTypes::scheduled)
	{
		WarningInFunction
			<< "Scheduled communication with split cyclics not supported."
			<< endl;
	}
}


template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const fvPatch& p,
	const DimensionedField<Type, volMesh>& iF,
	const Field<Type>& f
)
	:
	processorFvPatchField<Type>(p, iF, f),
	procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const processorCyclicFvPatchField<Type>& ptf,
	const fvPatch& p,
	const DimensionedField<Type, volMesh>& iF,
	const fvPatchFieldMapper& mapper
)
	:
	processorFvPatchField<Type>(ptf, p, iF, mapper),
	procPatch_(refCast<const processorCyclicFvPatch>(p))
{
	if (!isType<processorCyclicFvPatch>(this->patch()))
	{
		FatalErrorInFunction
			<< "' not constraint type '" << typeName << "'"
			<< "\n    for patch " << p.name()
			<< " of field " << this->internalField().name()
			<< " in file " << this->internalField().objectPath()
			<< exit(FatalIOError);
	}
}


template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const processorCyclicFvPatchField<Type>& ptf
)
	:
	processorFvPatchField<Type>(ptf),
	procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
	const processorCyclicFvPatchField<Type>& ptf,
	const DimensionedField<Type, volMesh>& iF
)
	:
	processorFvPatchField<Type>(ptf, iF),
	procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
tnbLib::processorCyclicFvPatchField<Type>::~processorCyclicFvPatchField()
{}


// ************************************************************************* //