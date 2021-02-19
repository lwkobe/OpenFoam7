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
    tnbLib::lagrangianFieldDecomposer

Description
    Lagrangian field decomposer.

SourceFiles
    lagrangianFieldDecomposer.C
    lagrangianFieldDecomposerDecomposeFields.C

\*---------------------------------------------------------------------------*/

#ifndef lagrangianFieldDecomposer_H
#define lagrangianFieldDecomposer_H

#include <includeAllModules.hxx>

#include <Cloud.hxx>
#include <CompactIOField.hxx>
#include <indexedParticle.hxx>
#include <passiveParticle.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class lagrangianFieldDecomposer Declaration
\*---------------------------------------------------------------------------*/

class lagrangianFieldDecomposer
{
    // Private Data

        //- Reference to processor mesh
        const polyMesh& procMesh_;

        //- Lagrangian positions for this processor
        Cloud<passiveParticle> positions_;

        //- The indices of the particles on this processor
        labelList particleIndices_;


public:

    // Constructors

        //- Construct from components
        lagrangianFieldDecomposer
        (
            const polyMesh& mesh,
            const polyMesh& procMesh,
            const labelList& faceProcAddressing,
            const labelList& cellProcAddressing,
            const word& cloudName,
            const Cloud<indexedParticle>& lagrangianPositions,
            const List<SLList<indexedParticle*>*>& cellParticles
        );

        //- Disallow default bitwise copy construction
        lagrangianFieldDecomposer(const lagrangianFieldDecomposer&) = delete;


    // Member Functions

        // Read the fields and hold on the pointer list
        template<class Type>
        static void readFields
        (
            const label cloudI,
            const IOobjectList& lagrangianObjects,
            PtrList<PtrList<IOField<Type>>>& lagrangianFields
        );

        template<class Type>
        static void readFieldFields
        (
            const label cloudI,
            const IOobjectList& lagrangianObjects,
            PtrList
            <
                PtrList<CompactIOField<Field<Type>, Type>>
            >& lagrangianFields
        );


        //- Decompose volume field
        template<class Type>
        tmp<IOField<Type>> decomposeField
        (
            const word& cloudName,
            const IOField<Type>& field
        ) const;

        template<class Type>
        tmp<CompactIOField<Field<Type>, Type>> decomposeFieldField
        (
            const word& cloudName,
            const CompactIOField<Field<Type>, Type>& field
        ) const;


        template<class GeoField>
        void decomposeFields
        (
            const word& cloudName,
            const PtrList<GeoField>& fields
        ) const;

        template<class GeoField>
        void decomposeFieldFields
        (
            const word& cloudName,
            const PtrList<GeoField>& fields
        ) const;


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const lagrangianFieldDecomposer&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    //#include "lagrangianFieldDecomposerDecomposeFields.C"
#endif

#include "lagrangianFieldDecomposerDecomposeFieldsI.hxx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //