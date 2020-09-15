#pragma once
#ifndef _PatchInteractionModel_Header
#define _PatchInteractionModel_Header

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
	tnbLib::PatchInteractionModel

Description
	Templated patch interaction model class

SourceFiles
	PatchInteractionModel.C
	PatchInteractionModelNew.C

\*---------------------------------------------------------------------------*/

#include <IOdictionary.hxx>
#include <autoPtr.hxx>
#include <runTimeSelectionTables.hxx>
#include <polyPatch.hxx>
#include <wallPolyPatch.hxx>
#include <tetIndices.hxx>
#include <CloudSubModelBase.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					   Class PatchInteractionModel Declaration
	\*---------------------------------------------------------------------------*/

	template<class CloudType>
	class PatchInteractionModel
		:
		public CloudSubModelBase<CloudType>
	{
	public:

		// Public enumerations

			// Interaction types
		enum interactionType
		{
			itNone,
			itRebound,
			itStick,
			itEscape,
			itOther
		};

		static wordList interactionTypeNames_;


	private:

		// Private Data

			//- Name of velocity field - default = "U"
		const word UName_;


	public:

		//- Runtime type information
		TypeName("patchInteractionModel");

		//- Declare runtime constructor selection table
		declareRunTimeSelectionTable
		(
			autoPtr,
			PatchInteractionModel,
			dictionary,
			(
				const dictionary& dict,
				CloudType& owner
				),
				(dict, owner)
		);


		// Constructors

			//- Construct null from owner
		PatchInteractionModel(CloudType& owner);

		//- Construct from components
		PatchInteractionModel
		(
			const dictionary& dict,
			CloudType& owner,
			const word& type
		);

		//- Construct copy
		PatchInteractionModel(const PatchInteractionModel<CloudType>& pim);

		//- Construct and return a clone
		virtual autoPtr<PatchInteractionModel<CloudType>> clone() const = 0;


		//- Destructor
		virtual ~PatchInteractionModel();


		//- Selector
		static autoPtr<PatchInteractionModel<CloudType>> New
		(
			const dictionary& dict,
			CloudType& owner
		);


		// Access

			//- Return name of velocity field
		const word& UName() const;


		// Member Functions

			//- Convert interaction result to word
		static word interactionTypeToWord(const interactionType& itEnum);

		//- Convert word to interaction result
		static interactionType wordToInteractionType(const word& itWord);

		//- Apply velocity correction
		//  Returns true if particle remains in same cell
		virtual bool correct
		(
			typename CloudType::parcelType& p,
			const polyPatch& pp,
			bool& keepParticle
		) = 0;


		// I-O

			//- Write patch interaction info to stream
		virtual void info(Ostream& os);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makePatchInteractionModel(CloudType)                                   \
                                                                               \
    typedef tnbLib::CloudType::kinematicCloudType kinematicCloudType;            \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        tnbLib::PatchInteractionModel<kinematicCloudType>,                       \
        0                                                                      \
    );                                                                         \
                                                                               \
    namespace tnbLib                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            PatchInteractionModel<kinematicCloudType>,                         \
            dictionary                                                         \
        );                                                                     \
    }


#define makePatchInteractionModelType(SS, CloudType)                           \
                                                                               \
    typedef tnbLib::CloudType::kinematicCloudType kinematicCloudType;            \
    defineNamedTemplateTypeNameAndDebug(tnbLib::SS<kinematicCloudType>, 0);      \
                                                                               \
    tnbLib::PatchInteractionModel<kinematicCloudType>::                          \
        adddictionaryConstructorToTable<tnbLib::SS<kinematicCloudType>>          \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <PatchInteractionModelI.hxx>
#include <PatchInteractionModelNew_Imp.hxx>

//#ifdef NoRepository
//#include <PatchInteractionModel.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_PatchInteractionModel_Header