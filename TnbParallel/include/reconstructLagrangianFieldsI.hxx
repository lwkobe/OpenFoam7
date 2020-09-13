#pragma once
#include <IOField.hxx>
#include <CompactIOField.hxx>
#include <Time.hxx>

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
tnbLib::tmp<tnbLib::IOField<Type>> tnbLib::reconstructLagrangianField
(
	const word& cloudName,
	const polyMesh& mesh,
	const PtrList<fvMesh>& meshes,
	const word& fieldName
)
{
	// Construct empty field on mesh
	tmp<IOField<Type>> tfield
	(
		new IOField<Type>
		(
			IOobject
			(
				fieldName,
				mesh.time().timeName(),
				cloud::prefix / cloudName,
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			Field<Type>(0)
			)
	);
	Field<Type>& field = tfield.ref();

	forAll(meshes, i)
	{
		// Check object on local mesh
		IOobject localIOobject
		(
			fieldName,
			meshes[i].time().timeName(),
			cloud::prefix / cloudName,
			meshes[i],
			IOobject::MUST_READ,
			IOobject::NO_WRITE
		);

		if (localIOobject.typeHeaderOk<IOField<Type>>(true))
		{
			IOField<Type> fieldi(localIOobject);

			label offset = field.size();
			field.setSize(offset + fieldi.size());

			forAll(fieldi, j)
			{
				field[offset + j] = fieldi[j];
			}
		}
	}

	return tfield;
}


template<class Type>
tnbLib::tmp<tnbLib::CompactIOField<tnbLib::Field<Type>, Type>>
tnbLib::reconstructLagrangianFieldField
(
	const word& cloudName,
	const polyMesh& mesh,
	const PtrList<fvMesh>& meshes,
	const word& fieldName
)
{
	// Construct empty field on mesh
	tmp<CompactIOField<Field<Type>, Type >> tfield
	(
		new CompactIOField<Field<Type>, Type>
		(
			IOobject
			(
				fieldName,
				mesh.time().timeName(),
				cloud::prefix / cloudName,
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			Field<Field<Type>>(0)
			)
	);
	Field<Field<Type>>& field = tfield.ref();

	forAll(meshes, i)
	{
		// Check object on local mesh
		IOobject localIOobject
		(
			fieldName,
			meshes[i].time().timeName(),
			cloud::prefix / cloudName,
			meshes[i],
			IOobject::MUST_READ,
			IOobject::NO_WRITE
		);

		if
			(
				localIOobject.typeHeaderOk<CompactIOField<Field<Type>, Type>>
				(
					false
					)
				|| localIOobject.typeHeaderOk<IOField<Field<Type>>>(false)
				)
		{
			CompactIOField<Field<Type>, Type> fieldi(localIOobject);

			label offset = field.size();
			field.setSize(offset + fieldi.size());

			forAll(fieldi, j)
			{
				field[offset + j] = fieldi[j];
			}
		}
	}

	return tfield;
}



template<class Type>
void tnbLib::reconstructLagrangianFields
(
	const word& cloudName,
	const polyMesh& mesh,
	const PtrList<fvMesh>& meshes,
	const IOobjectList& objects,
	const HashSet<word>& selectedFields
)
{
	const word fieldClassName(IOField<Type>::typeName);

	IOobjectList fields = objects.lookupClass(fieldClassName);

	if (fields.size())
	{
		Info << "    Reconstructing lagrangian "
			<< fieldClassName << "s\n" << endl;

		forAllConstIter(IOobjectList, fields, fieldIter)
		{
			if
				(
					selectedFields.empty()
					|| selectedFields.found(fieldIter()->name())
					)
			{
				Info << "        " << fieldIter()->name() << endl;
				reconstructLagrangianField<Type>
					(
						cloudName,
						mesh,
						meshes,
						fieldIter()->name()
						)().write();
			}
		}

		Info << endl;
	}
}


template<class Type>
void tnbLib::reconstructLagrangianFieldFields
(
	const word& cloudName,
	const polyMesh& mesh,
	const PtrList<fvMesh>& meshes,
	const IOobjectList& objects,
	const HashSet<word>& selectedFields
)
{
	{
		const word fieldClassName(CompactIOField<Field<Type>, Type>::typeName);

		IOobjectList fields = objects.lookupClass(fieldClassName);

		if (fields.size())
		{
			Info << "    Reconstructing lagrangian "
				<< fieldClassName << "s\n" << endl;

			forAllConstIter(IOobjectList, fields, fieldIter)
			{
				if
					(
						selectedFields.empty()
						|| selectedFields.found(fieldIter()->name())
						)
				{
					Info << "        " << fieldIter()->name() << endl;
					reconstructLagrangianFieldField<Type>
						(
							cloudName,
							mesh,
							meshes,
							fieldIter()->name()
							)().write();
				}
			}

			Info << endl;
		}
	}

	{
		const word fieldClassName(IOField<Field<Type>>::typeName);

		IOobjectList fields = objects.lookupClass(fieldClassName);

		if (fields.size())
		{
			Info << "    Reconstructing lagrangian "
				<< fieldClassName << "s\n" << endl;

			forAllConstIter(IOobjectList, fields, fieldIter)
			{
				if
					(
						selectedFields.empty()
						|| selectedFields.found(fieldIter()->name())
						)
				{
					Info << "        " << fieldIter()->name() << endl;
					reconstructLagrangianFieldField<Type>
						(
							cloudName,
							mesh,
							meshes,
							fieldIter()->name()
							)().write();
				}
			}

			Info << endl;
		}
	}
}


// ************************************************************************* //