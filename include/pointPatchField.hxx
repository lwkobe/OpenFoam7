#pragma once
#ifndef _pointPatchField_Header
#define _pointPatchField_Header

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
	tnbLib::pointPatchField

Description
	Abstract base class for point-mesh patch fields.

	The base-field does not store values as they are part of the
	"internal field".  There are derived classes to store constraint values
	e.g. fixedValuePointPatchField derived from the generic
	valuePointPatchField which ensures the values in the "internal field"
	are reset to the fixed-values by applying the stored values.

SourceFiles
	pointPatchField.C
	newPointPatchField.C

\*---------------------------------------------------------------------------*/

#include <pointPatch.hxx>
#include <DimensionedField.hxx>
#include <autoPtr.hxx>

#include <Pstream.hxx>  // added by amir
#include <runTimeSelectionTables.hxx> // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// Forward declaration of classes

	class objectRegistry;
	class dictionary;
	class pointPatchFieldMapper;
	class pointMesh;

	// Forward declaration of friend functions and operators

	template<class Type>
	class pointPatchField;

	template<class Type>
	class calculatedPointPatchField;

	template<class Type>
	Ostream& operator<<
		(
			Ostream&,
			const pointPatchField<Type>&
			);


	/*---------------------------------------------------------------------------*\
						   Class pointPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class pointPatchField
	{
		// Private Data

			//- Reference to patch
		const pointPatch& patch_;

		//- Reference to internal field
		const DimensionedField<Type, pointMesh>& internalField_;

		//- Update index used so that updateCoeffs is called only once during
		//  the construction of the matrix
		bool updated_;

		//- Optional patch type, used to allow specified boundary conditions
		//  to be applied to constraint patches by providing the constraint
		//  patch type as 'patchType'
		word patchType_;


	public:

		typedef pointPatch Patch;
		typedef calculatedPointPatchField<Type> Calculated;


		//- Runtime type information
		//TypeName("pointPatchField");
		static const char* typeName_() { return "pointPatchField"; }
		static FoamBase_EXPORT const ::tnbLib::word typeName;
		static FoamBase_EXPORT int debug;
		virtual const word& type() const { return typeName; };

		//- Debug switch to disallow the use of genericPointPatchField
		static int disallowGenericPointPatchField;


		// Declare run-time constructor selection tables

		/*declareRunTimeSelectionTable
		(
			autoPtr,
			pointPatchField,
			pointPatch,
			(
				const pointPatch& p,
				const DimensionedField<Type, pointMesh>& iF
				),
				(p, iF)
		);*/

		typedef autoPtr<pointPatchField> (*pointPatchConstructorPtr)(const pointPatch& p,
		                                                             const DimensionedField<Type, pointMesh>& iF);
		typedef HashTable<pointPatchConstructorPtr, word, string::hash> pointPatchConstructorTable;
		static FoamBase_EXPORT pointPatchConstructorTable* pointPatchConstructorTablePtr_;
		static FoamBase_EXPORT void constructpointPatchConstructorTables();
		static FoamBase_EXPORT void destroypointPatchConstructorTables();

		template <class pointPatchFieldType>
		class addpointPatchConstructorToTable
		{
		public:
			static autoPtr<pointPatchField> New(const pointPatch& p, const DimensionedField<Type, pointMesh>& iF)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(p, iF));
			}

			addpointPatchConstructorToTable(const word& lookup = pointPatchFieldType::typeName)
			{
				constructpointPatchConstructorTables();
				if (!pointPatchConstructorTablePtr_->insert(lookup, New))
				{
					std::cerr << "Duplicate entry " << lookup << " in runtime selection table " << "pointPatchField" << std::endl;
					error::safePrintStack(std::cerr);
				}
			}

			~addpointPatchConstructorToTable() { destroypointPatchConstructorTables(); }
		};

		template <class pointPatchFieldType>
		class addRemovablepointPatchConstructorToTable
		{
			const word& lookup_;
		public:
			static autoPtr<pointPatchField> New(const pointPatch& p, const DimensionedField<Type, pointMesh>& iF)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(p, iF));
			}

			addRemovablepointPatchConstructorToTable(const word& lookup = pointPatchFieldType::typeName) : lookup_(lookup)
			{
				constructpointPatchConstructorTables();
				pointPatchConstructorTablePtr_->set(lookup, New);
			}

			~addRemovablepointPatchConstructorToTable()
			{
				if (pointPatchConstructorTablePtr_) { pointPatchConstructorTablePtr_->erase(lookup_); }
			}
		};;

		/*declareRunTimeSelectionTable
		(
			autoPtr,
			pointPatchField,
			patchMapper,
			(
				const pointPatchField<Type>& ptf,
				const pointPatch& p,
				const DimensionedField<Type, pointMesh>& iF,
				const pointPatchFieldMapper& m
				),
				(dynamic_cast<const pointPatchFieldType&>(ptf), p, iF, m)
		);*/

		typedef autoPtr<pointPatchField> (*patchMapperConstructorPtr)(const pointPatchField<Type>& ptf, const pointPatch& p,
		                                                              const DimensionedField<Type, pointMesh>& iF,
		                                                              const pointPatchFieldMapper& m);
		typedef HashTable<patchMapperConstructorPtr, word, string::hash> patchMapperConstructorTable;
		static FoamBase_EXPORT patchMapperConstructorTable* patchMapperConstructorTablePtr_;
		static FoamBase_EXPORT void constructpatchMapperConstructorTables();
		static FoamBase_EXPORT void destroypatchMapperConstructorTables();

		template <class pointPatchFieldType>
		class addpatchMapperConstructorToTable
		{
		public:
			static autoPtr<pointPatchField> New(const pointPatchField<Type>& ptf, const pointPatch& p,
			                                    const DimensionedField<Type, pointMesh>& iF, const pointPatchFieldMapper& m)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(dynamic_cast<const pointPatchFieldType&>(ptf), p, iF, m));
			}

			addpatchMapperConstructorToTable(const word& lookup = pointPatchFieldType::typeName)
			{
				constructpatchMapperConstructorTables();
				if (!patchMapperConstructorTablePtr_->insert(lookup, New))
				{
					std::cerr << "Duplicate entry " << lookup << " in runtime selection table " << "pointPatchField" << std::endl;
					error::safePrintStack(std::cerr);
				}
			}

			~addpatchMapperConstructorToTable() { destroypatchMapperConstructorTables(); }
		};

		template <class pointPatchFieldType>
		class addRemovablepatchMapperConstructorToTable
		{
			const word& lookup_;
		public:
			static autoPtr<pointPatchField> New(const pointPatchField<Type>& ptf, const pointPatch& p,
			                                    const DimensionedField<Type, pointMesh>& iF, const pointPatchFieldMapper& m)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(dynamic_cast<const pointPatchFieldType&>(ptf), p, iF, m));
			}

			addRemovablepatchMapperConstructorToTable(const word& lookup = pointPatchFieldType::typeName) : lookup_(lookup)
			{
				constructpatchMapperConstructorTables();
				patchMapperConstructorTablePtr_->set(lookup, New);
			}

			~addRemovablepatchMapperConstructorToTable()
			{
				if (patchMapperConstructorTablePtr_) { patchMapperConstructorTablePtr_->erase(lookup_); }
			}
		};;

		/*declareRunTimeSelectionTable
		(
			autoPtr,
			pointPatchField,
			dictionary,
			(
				const pointPatch& p,
				const DimensionedField<Type, pointMesh>& iF,
				const dictionary& dict
				),
				(p, iF, dict)
		);*/

		typedef autoPtr<pointPatchField> (*dictionaryConstructorPtr)(const pointPatch& p,
		                                                             const DimensionedField<Type, pointMesh>& iF,
		                                                             const dictionary& dict);
		typedef HashTable<dictionaryConstructorPtr, word, string::hash> dictionaryConstructorTable;
		static FoamBase_EXPORT dictionaryConstructorTable* dictionaryConstructorTablePtr_;
		static FoamBase_EXPORT void constructdictionaryConstructorTables();
		static FoamBase_EXPORT void destroydictionaryConstructorTables();

		template <class pointPatchFieldType>
		class adddictionaryConstructorToTable
		{
		public:
			static autoPtr<pointPatchField> New(const pointPatch& p, const DimensionedField<Type, pointMesh>& iF,
			                                    const dictionary& dict)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(p, iF, dict));
			}

			adddictionaryConstructorToTable(const word& lookup = pointPatchFieldType::typeName)
			{
				constructdictionaryConstructorTables();
				if (!dictionaryConstructorTablePtr_->insert(lookup, New))
				{
					std::cerr << "Duplicate entry " << lookup << " in runtime selection table " << "pointPatchField" << std::endl;
					error::safePrintStack(std::cerr);
				}
			}

			~adddictionaryConstructorToTable() { destroydictionaryConstructorTables(); }
		};

		template <class pointPatchFieldType>
		class addRemovabledictionaryConstructorToTable
		{
			const word& lookup_;
		public:
			static autoPtr<pointPatchField> New(const pointPatch& p, const DimensionedField<Type, pointMesh>& iF,
			                                    const dictionary& dict)
			{
				return autoPtr<pointPatchField>(new pointPatchFieldType(p, iF, dict));
			}

			addRemovabledictionaryConstructorToTable(const word& lookup = pointPatchFieldType::typeName) : lookup_(lookup)
			{
				constructdictionaryConstructorTables();
				dictionaryConstructorTablePtr_->set(lookup, New);
			}

			~addRemovabledictionaryConstructorToTable()
			{
				if (dictionaryConstructorTablePtr_) { dictionaryConstructorTablePtr_->erase(lookup_); }
			}
		};;


		// Constructors

			//- Construct from patch and internal field
		pointPatchField
		(
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Construct from patch, internal field and dictionary
		pointPatchField
		(
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patchField<Type> onto a new patch
		pointPatchField
		(
			const pointPatchField<Type>&,
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const pointPatchFieldMapper&
		);

		//- Copy constructor
		pointPatchField(const pointPatchField<Type>&);

		//- Construct and return a clone
		virtual autoPtr<pointPatchField<Type>> clone() const = 0;

		//- Construct as copy setting internal field reference
		pointPatchField
		(
			const pointPatchField<Type>&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual autoPtr<pointPatchField<Type>> clone
		(
			const DimensionedField<Type, pointMesh>& iF
		) const = 0;


		// Selectors

			//- Return a pointer to a new patchField created on freestore given
			//  patch and internal field
			//  (does not set the patch field values)
		static autoPtr<pointPatchField<Type>> New
		(
			const word&,
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Return a pointer to a new patchField created on freestore given
		//  patch and internal field
		//  (does not set the patch field values).
		//  Allows override of constraint type
		static autoPtr<pointPatchField<Type>> New
		(
			const word&,
			const word& actualPatchType,
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&
		);

		//- Return a pointer to a new patchField created on freestore from
		//  a given pointPatchField mapped onto a new patch
		static autoPtr<pointPatchField<Type>> New
		(
			const pointPatchField<Type>&,
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const pointPatchFieldMapper&
		);

		//- Return a pointer to a new patchField created on freestore
		//  from dictionary
		static autoPtr<pointPatchField<Type>> New
		(
			const pointPatch&,
			const DimensionedField<Type, pointMesh>&,
			const dictionary&
		);

		//- Return a pointer to a new calculatedPointPatchField created on
		//  freestore without setting patchField values
		template<class Type2>
		static autoPtr<pointPatchField<Type>>
			NewCalculatedType
			(
				const pointPatchField<Type2>&
			);


		//- Destructor
		virtual ~pointPatchField<Type>()
		{}


		// Member Functions

			// Access

				//- Return local objectRegistry
		const objectRegistry& db() const;

		//- Return size
		label size() const
		{
			return patch().size();
		}

		//- Return patch
		const pointPatch& patch() const
		{
			return patch_;
		}

		//- Return dimensioned internal field reference
		const DimensionedField<Type, pointMesh>&
			internalField() const
		{
			return internalField_;
		}

		//- Return internal field reference
		const Field<Type>& primitiveField() const
		{
			return internalField_;
		}

		//- Optional patch type
		const word& patchType() const
		{
			return patchType_;
		}

		//- Optional patch type
		word& patchType()
		{
			return patchType_;
		}

		//- Return true if this patch field fixes a value
		virtual bool fixesValue() const
		{
			return false;
		}

		//- Return true if this patch field is coupled
		virtual bool coupled() const
		{
			return false;
		}

		//- Return true if the boundary condition has already been updated
		bool updated() const
		{
			return updated_;
		}

		//- Return field created from appropriate internal field values
		tmp<Field<Type>> patchInternalField() const;

		//- Return field created from appropriate internal field values
		//  given internal field reference
		template<class Type1>
		tmp<Field<Type1>> patchInternalField
		(
			const Field<Type1>& iF
		) const;

		//- Return field created from selected internal field values
		//  given internal field reference
		template<class Type1>
		tmp<Field<Type1>> patchInternalField
		(
			const Field<Type1>& iF,
			const labelList& meshPoints
		) const;

		//- Given the internal field and a patch field,
		//  add the patch field to the internal field
		template<class Type1>
		void addToInternalField
		(
			Field<Type1>& iF,
			const Field<Type1>& pF
		) const;

		//- Given the internal field and a patch field,
		//  add selected elements of the patch field to the internal field
		template<class Type1>
		void addToInternalField
		(
			Field<Type1>& iF,
			const Field<Type1>& pF,
			const labelList& points
		) const;

		//- Given the internal field and a patch field,
		//  set the patch field in the internal field
		template<class Type1>
		void setInInternalField
		(
			Field<Type1>& iF,
			const Field<Type1>& pF,
			const labelList& meshPoints
		) const;

		//- Given the internal field and a patch field,
		//  set the patch field in the internal field
		template<class Type1>
		void setInInternalField
		(
			Field<Type1>& iF,
			const Field<Type1>& pF
		) const;

		//- Return the type of the calculated form of pointPatchField
		static const word& calculatedType();

		//- Return the constraint type this pointPatchField implements.
		virtual const word& constraintType() const
		{
			return word::null;
		}


		// Mapping functions

			//- Map (and resize as needed) from self given a mapping object
			//  Used to update fields following mesh topology change
		virtual void autoMap(const pointPatchFieldMapper&)
		{}

		//- Reverse map the given pointPatchField onto this pointPatchField
		//  Used to reconstruct fields
		virtual void rmap(const pointPatchField<Type>&, const labelList&)
		{}


		// Evaluation functions

			//- Update the coefficients associated with the patch field
			//  Sets Updated to true
		virtual void updateCoeffs()
		{
			updated_ = true;
		}

		//- Initialise evaluation of the patch field (do nothing)
		virtual void initEvaluate
		(
			const Pstream::commsTypes commsType =
			Pstream::commsTypes::blocking
		)
		{}

		//- Evaluate the patch field
		virtual void evaluate
		(
			const Pstream::commsTypes commsType =
			Pstream::commsTypes::blocking
		);


		// I-O

			//- Write
		virtual void write(Ostream&) const;


		// Member Operators

		virtual void operator=(const pointPatchField<Type>&) {}
		virtual void operator=(pointPatchField<Type>&&) {}
		virtual void operator+=(const pointPatchField<Type>&) {}
		virtual void operator-=(const pointPatchField<Type>&) {}
		virtual void operator*=(const pointPatchField<scalar>&) {}
		virtual void operator/=(const pointPatchField<scalar>&) {}

		virtual void operator=(const Field<Type>&) {}
		virtual void operator+=(const Field<Type>&) {}
		virtual void operator-=(const Field<Type>&) {}

		virtual void operator*=(const Field<scalar>&) {}
		virtual void operator/=(const Field<scalar>&) {}

		virtual void operator=(const Type&) {}
		virtual void operator+=(const Type&) {}
		virtual void operator-=(const Type&) {}
		virtual void operator*=(const scalar) {}
		virtual void operator/=(const scalar) {}


		// Force an assignment irrespective of form of patch
		// By generic these do nothing unless the patch actually has boundary
		// values

		virtual void operator==(const pointPatchField<Type>&) {}
		virtual void operator==(const Field<Type>&) {}
		virtual void operator==(const Type&) {}


		// Ostream operator

		friend Ostream& operator<< <Type>
			(
				Ostream&,
				const pointPatchField<Type>&
				);
	};


	// This function is added as a hack to enable simple backward compatibility
	// with versions using referenceLevel in GeometicField
	template<class Type>
	const pointPatchField<Type>& operator+
		(
			const pointPatchField<Type>& ppf,
			const Type&
			)
	{
		return ppf;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <pointPatchFieldFunctions.hxx>
#include <pointPatchFieldI.hxx>
#include <pointPatchFieldNew_Imp.hxx>

//#ifdef NoRepository
//#include <pointPatchField.cxx>
//#include <calculatedPointPatchField.hxx>
//#endif


#define addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField) \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        pointPatch                                                             \
    );                                                                         \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        patchMapper                                                            \
    );                                                                         \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        dictionary                                                             \
    );


// for non-templated patch fields
#define makePointPatchTypeField(PatchTypeField,typePatchTypeField)             \
    defineTypeNameAndDebug(typePatchTypeField, 0);                             \
    addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


// for templated patch fields
#define makeTemplatePointPatchTypeField(PatchTypeField, typePatchTypeField)    \
    defineNamedTemplateTypeNameAndDebug(typePatchTypeField, 0);                \
    addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


#define makePointPatchFields(type)                                             \
    makeTemplatePointPatchTypeField                                            \
    (                                                                          \
        pointPatchScalarField,                                                 \
        type##PointPatchScalarField                                            \
    );                                                                         \
    makeTemplatePointPatchTypeField                                            \
    (                                                                          \
        pointPatchVectorField,                                                 \
        type##PointPatchVectorField                                            \
    );                                                                         \
    makeTemplatePointPatchTypeField                                            \
    (                                                                          \
        pointPatchSphericalTensorField,                                        \
        type##PointPatchSphericalTensorField                                   \
    );                                                                         \
    makeTemplatePointPatchTypeField                                            \
    (                                                                          \
        pointPatchSymmTensorField,                                             \
        type##PointPatchSymmTensorField                                        \
    );                                                                         \
    makeTemplatePointPatchTypeField                                            \
    (                                                                          \
        pointPatchTensorField,                                                 \
        type##PointPatchTensorField                                            \
);


#define makePointPatchFieldsTypeName(type)                                     \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchScalarField, 0);       \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchVectorField, 0);       \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        type##PointPatchSphericalTensorField, 0                                \
    );                                                                         \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchSymmTensorField, 0);   \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchTensorField, 0)


#define makePointPatchFieldTypedefs(type)                                      \
    typedef type##PointPatchField<scalar> type##PointPatchScalarField;         \
    typedef type##PointPatchField<vector> type##PointPatchVectorField;         \
    typedef type##PointPatchField<sphericalTensor>                             \
        type##PointPatchSphericalTensorField;                                  \
    typedef type##PointPatchField<symmTensor> type##PointPatchSymmTensorField;\
    typedef type##PointPatchField<tensor> type##PointPatchTensorField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_pointPatchField_Header
