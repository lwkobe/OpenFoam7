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

Application
	mshToFoam

Description
	Converts .msh file generated by the Adventure system.

	Note: the .msh format does not contain any boundary information. It is
	purely a description of the internal mesh.

	Can read both linear-tet format (i.e. 4 verts per tet) and linear-hex
	format (8 verts per hex) (if provided with the -hex (at your option)
	(Note: will bomb out if not supplied with the correct option for the
	 file format)

	Not extensively tested.

\*---------------------------------------------------------------------------*/

#include <argList.hxx>
#include <Time.hxx>
#include <polyMesh.hxx>
#include <IFstream.hxx>
#include <polyPatch.hxx>
#include <ListOps.hxx>
#include <cellModeller.hxx>

#include <fstream>

using namespace tnbLib;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


int main(int argc, char *argv[])
{
	argList::noParallel();
	argList::validArgs.append(".msh file");
	argList::addBoolOption
	(
		"hex",
		"treat input as containing hex instead of tet cells"
	);

#include <setRootCase.lxx>
#include <createTime.lxx>

	const bool readHex = args.optionFound("hex");
	IFstream mshStream(args[1]);

	label nCells;
	mshStream >> nCells;

	if (readHex)
	{
		Info << "Trying to read " << nCells << " hexes." << nl << endl;
	}
	else
	{
		Info << "Trying to read " << nCells << " tets." << nl << endl;
	}

	cellShapeList cells(nCells);

	const cellModel& tet = *(cellModeller::lookup("tet"));
	const cellModel& hex = *(cellModeller::lookup("hex"));

	labelList tetPoints(4);
	labelList hexPoints(8);

	if (readHex)
	{
		for (label celli = 0; celli < nCells; celli++)
		{
			for (label cp = 0; cp < 8; cp++)
			{
				mshStream >> hexPoints[cp];
			}
			cells[celli] = cellShape(hex, hexPoints);
		}
	}
	else
	{
		for (label celli = 0; celli < nCells; celli++)
		{
			for (label cp = 0; cp < 4; cp++)
			{
				mshStream >> tetPoints[cp];
			}
			cells[celli] = cellShape(tet, tetPoints);
		}
	}


	label nPoints;

	mshStream >> nPoints;

	Info << "Trying to read " << nPoints << " points." << endl << endl;

	pointField points(nPoints);


	for (label pointi = 0; pointi < nPoints; pointi++)
	{
		scalar x, y, z;

		mshStream >> x >> y >> z;

		points[pointi] = point(x, y, z);
	}


	polyMesh mesh
	(
		IOobject
		(
			polyMesh::defaultRegion,
			runTime.constant(),
			runTime
		),
		move(points),
		cells,
		faceListList(0),
		wordList(0),
		wordList(0),
		"defaultFaces",
		polyPatch::typeName,
		wordList(0)
	);

	Info << "Writing mesh ..." << endl;

	mesh.write();


	Info << "End\n" << endl;

	return 0;
}


// ************************************************************************* //