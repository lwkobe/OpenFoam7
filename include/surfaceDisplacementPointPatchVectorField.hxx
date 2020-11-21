#pragma once
#ifndef _surfaceDisplacementPointPatchVectorField_Header
#define _surfaceDisplacementPointPatchVectorField_Header

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
	tnbLib::surfaceDisplacementPointPatchVectorField

Description
	Displacement fixed by projection onto triSurface.
	Use in a displacementMotionSolver
	as a bc on the pointDisplacement field.

	Calculates the projection onto the surface according
	to the projectMode
	- NEAREST : nearest
	- POINTNORMAL : intersection with point normal
	- FIXEDNORMAL : intersection with fixed vector

	This displacement is then clipped with the specified velocity * deltaT.

	Optionally (intersection only) removes a component ("wedgePlane") to
	stay in 2D.

	Needs:
	- geometry : dictionary with searchableSurfaces. (usually
	  triSurfaceMeshes in constant/triSurface)
	- projectMode : see above
	- projectDirection : if projectMode = fixedNormal
	- wedgePlane : -1 or component to knock out of intersection normal
	- frozenPointsZone : empty or name of pointZone containing points
						 that do not move

SourceFiles
	surfaceDisplacementPointPatchVectorField.C

\*---------------------------------------------------------------------------*/

#include <pointPatchFields.hxx>
#include <fixedValuePointPatchFields.hxx>
#include <searchableSurfaces.hxx>
#include <Switch.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
			  Class surfaceDisplacementPointPatchVectorField Declaration
	\*---------------------------------------------------------------------------*/

	class surfaceDisplacementPointPatchVectorField
		:
		public fixedValuePointPatchVectorField
	{
	public:

		// Public data types

		enum projectMode
		{
			NEAREST,
			POINTNORMAL,
			FIXEDNORMAL
		};

	private:

		// Private Data

			//- Project mode names
		static FoamFvMotionSolver_EXPORT const NamedEnum<projectMode, 3> projectModeNames_;

		//- Maximum velocity
		const vector velocity_;

		//- Names of surfaces
		const dictionary surfacesDict_;

		//- How to project/project onto surface
		const projectMode projectMode_;

		//- Direction to project
		const vector projectDir_;

		//- Plane for 2D wedge case or -1.
		const label wedgePlane_;

		//- pointZone with frozen points
		const word frozenPointsZone_;

		//- Demand driven: surface to project
		mutable autoPtr<searchableSurfaces> surfacesPtr_;


		// Private Member Functions

			//- Calculate displacement (w.r.t. points0()) to project onto surface
		FoamFvMotionSolver_EXPORT void calcProjection(vectorField& displacement) const;


	public:

		//- Runtime type information
		//TypeName("surfaceDisplacement");
		static const char* typeName_() { return "surfaceDisplacement"; }
		static FoamFvMotionSolver_EXPORT const ::tnbLib::word typeName;
		static FoamFvMotionSolver_EXPORT int debug;
		virtual const word& type() const { return typeName; };


		// Constructors

			//- Construct from patch and internal field
		FoamFvMotionSolver_EXPORT surfaceDisplacementPointPatchVectorField
		(
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&
		);

		//- Construct from patch, internal field and dictionary
		FoamFvMotionSolver_EXPORT surfaceDisplacementPointPatchVectorField
		(
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&,
			const dictionary&
		);

		//- Construct by mapping given patchField<vector> onto a new patch
		FoamFvMotionSolver_EXPORT surfaceDisplacementPointPatchVectorField
		(
			const surfaceDisplacementPointPatchVectorField&,
			const pointPatch&,
			const DimensionedField<vector, pointMesh>&,
			const pointPatchFieldMapper&
		);

		//- Copy constructor
		FoamFvMotionSolver_EXPORT surfaceDisplacementPointPatchVectorField
		(
			const surfaceDisplacementPointPatchVectorField&
		);

		//- Construct and return a clone
		virtual autoPtr<pointPatchVectorField> clone() const
		{
			return autoPtr<pointPatchVectorField>
				(
					new surfaceDisplacementPointPatchVectorField
					(
						*this
					)
					);
		}

		//- Copy constructor setting internal field reference
		FoamFvMotionSolver_EXPORT surfaceDisplacementPointPatchVectorField
		(
			const surfaceDisplacementPointPatchVectorField&,
			const DimensionedField<vector, pointMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual autoPtr<pointPatchVectorField> clone
		(
			const DimensionedField<vector, pointMesh>& iF
		) const
		{
			return autoPtr<pointPatchVectorField>
				(
					new surfaceDisplacementPointPatchVectorField
					(
						*this,
						iF
					)
					);
		}

		// Member Functions

			//- Surface to follow. Demand loads surfaceNames.
		FoamFvMotionSolver_EXPORT const searchableSurfaces& surfaces() const;

		//- Update the coefficients associated with the patch field
		FoamFvMotionSolver_EXPORT virtual void updateCoeffs();

		//- Write
		FoamFvMotionSolver_EXPORT virtual void write(Ostream&) const;


		// Member Operators

			//- Disallow default bitwise assignment
		FoamFvMotionSolver_EXPORT void operator=
			(
				const surfaceDisplacementPointPatchVectorField&
				) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_surfaceDisplacementPointPatchVectorField_Header
