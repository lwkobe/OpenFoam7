#include <cuttingPlane.hxx>

#include <primitiveMesh.hxx>
#include <linePointRef.hxx>
#include <meshTools.hxx>

#include <PstreamReduceOps.hxx>  // added by amir

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Set values for what is close to zero and what is considered to
// be positive (and not just rounding noise)
//! \cond localScope
const tnbLib::scalar zeroish = tnbLib::small;
const tnbLib::scalar positive = tnbLib::small * 1E3;
//! \endcond

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void tnbLib::cuttingPlane::calcCutCells
(
	const primitiveMesh& mesh,
	const scalarField& dotProducts,
	const labelUList& cellIdLabels
)
{
	const labelListList& cellEdges = mesh.cellEdges();
	const edgeList& edges = mesh.edges();

	label listSize = cellEdges.size();
	if (notNull(cellIdLabels))
	{
		listSize = cellIdLabels.size();
	}

	cutCells_.setSize(listSize);
	label cutcelli(0);

	// fp: check if the list (cellZone) is not empty.
	const bool isZoneEmpty
	(
		(returnReduce(cellIdLabels.size(), sumOp<label>()) > 0) ? false : true
	);

	// Find the cut cells by detecting any cell that uses points with
	// opposing dotProducts.
	for (label listI = 0; listI < listSize; ++listI)
	{
		label celli = listI;

		if (notNull(cellIdLabels))
		{
			celli = cellIdLabels[listI];
		}
		else
		{
			// fp: in parallel computation, if the cellZone exists globally
			// but not locally, the postprocessing must be still be limited to
			// the crossing plane.
			if (!isZoneEmpty)
			{
				cutCells_.setSize(0);
				return;
			}
		}
		const labelList& cEdges = cellEdges[celli];

		label nCutEdges = 0;

		forAll(cEdges, i)
		{
			const edge& e = edges[cEdges[i]];

			if
				(
				(dotProducts[e[0]] < zeroish && dotProducts[e[1]] > positive)
					|| (dotProducts[e[1]] < zeroish && dotProducts[e[0]] > positive)
					)
			{
				nCutEdges++;

				if (nCutEdges > 2)
				{
					cutCells_[cutcelli++] = celli;

					break;
				}
			}
		}
	}

	// Set correct list size
	cutCells_.setSize(cutcelli);
}


void tnbLib::cuttingPlane::intersectEdges
(
	const primitiveMesh& mesh,
	const scalarField& dotProducts,
	List<label>& edgePoint
)
{
	// Use the dotProducts to find out the cut edges.
	const edgeList& edges = mesh.edges();
	const pointField& points = mesh.points();

	// Per edge -1 or the label of the intersection point
	edgePoint.setSize(edges.size());

	DynamicList<point> dynCuttingPoints(4 * cutCells_.size());

	forAll(edges, edgeI)
	{
		const edge& e = edges[edgeI];

		if
			(
			(dotProducts[e[0]] < zeroish && dotProducts[e[1]] > positive)
				|| (dotProducts[e[1]] < zeroish && dotProducts[e[0]] > positive)
				)
		{
			// Edge is cut
			edgePoint[edgeI] = dynCuttingPoints.size();

			const point& p0 = points[e[0]];
			const point& p1 = points[e[1]];

			scalar alpha = lineIntersect(linePointRef(p0, p1));

			if (alpha < zeroish)
			{
				dynCuttingPoints.append(p0);
			}
			else if (alpha >= 1.0)
			{
				dynCuttingPoints.append(p1);
			}
			else
			{
				dynCuttingPoints.append((1 - alpha)*p0 + alpha * p1);
			}
		}
		else
		{
			edgePoint[edgeI] = -1;
		}
	}

	this->storedPoints().transfer(dynCuttingPoints);
}


bool tnbLib::cuttingPlane::walkCell
(
	const primitiveMesh& mesh,
	const labelUList& edgePoint,
	const label celli,
	const label startEdgeI,
	DynamicList<label>& faceVerts
)
{
	label facei = -1;
	label edgeI = startEdgeI;

	label nIter = 0;

	faceVerts.clear();
	do
	{
		faceVerts.append(edgePoint[edgeI]);

		// Cross edge to other face
		facei = meshTools::otherFace(mesh, celli, facei, edgeI);

		// Find next cut edge on face.
		const labelList& fEdges = mesh.faceEdges()[facei];

		label nextEdgeI = -1;

		// Note: here is where we should check for whether there are more
		// than 2 intersections with the face (warped/non-convex face).
		// If so should e.g. decompose the cells on both faces and redo
		// the calculation.

		forAll(fEdges, i)
		{
			label edge2I = fEdges[i];

			if (edge2I != edgeI && edgePoint[edge2I] != -1)
			{
				nextEdgeI = edge2I;
				break;
			}
		}

		if (nextEdgeI == -1)
		{
			// Did not find another cut edge on facei. Do what?
			WarningInFunction
				<< "Did not find closed walk along surface of cell " << celli
				<< " starting from edge " << startEdgeI
				<< " in " << nIter << " iterations." << nl
				<< "Collected cutPoints so far:" << faceVerts
				<< endl;

			return false;
		}

		edgeI = nextEdgeI;

		nIter++;

		if (nIter > 1000)
		{
			WarningInFunction
				<< "Did not find closed walk along surface of cell " << celli
				<< " starting from edge " << startEdgeI
				<< " in " << nIter << " iterations." << nl
				<< "Collected cutPoints so far:" << faceVerts
				<< endl;
			return false;
		}

	} while (edgeI != startEdgeI);


	if (faceVerts.size() >= 3)
	{
		return true;
	}
	else
	{
		WarningInFunction
			<< "Did not find closed walk along surface of cell " << celli
			<< " starting from edge " << startEdgeI << nl
			<< "Collected cutPoints so far:" << faceVerts
			<< endl;

		return false;
	}
}


void tnbLib::cuttingPlane::walkCellCuts
(
	const primitiveMesh& mesh,
	const bool triangulate,
	const labelUList& edgePoint
)
{
	const pointField& cutPoints = this->points();

	// use dynamic lists to handle triangulation and/or missed cuts
	DynamicList<face>  dynCutFaces(cutCells_.size());
	DynamicList<label> dynCutCells(cutCells_.size());

	// scratch space for calculating the face vertices
	DynamicList<label> faceVerts(10);

	forAll(cutCells_, i)
	{
		label celli = cutCells_[i];

		// Find the starting edge to walk from.
		const labelList& cEdges = mesh.cellEdges()[celli];

		label startEdgeI = -1;

		forAll(cEdges, cEdgeI)
		{
			label edgeI = cEdges[cEdgeI];

			if (edgePoint[edgeI] != -1)
			{
				startEdgeI = edgeI;
				break;
			}
		}

		// Check for the unexpected ...
		if (startEdgeI == -1)
		{
			FatalErrorInFunction
				<< "Cannot find cut edge for cut cell " << celli
				<< abort(FatalError);
		}

		// Walk from starting edge around the circumference of the cell.
		bool okCut = walkCell
		(
			mesh,
			edgePoint,
			celli,
			startEdgeI,
			faceVerts
		);

		if (okCut)
		{
			face f(faceVerts);

			// Orient face to point in the same direction as the plane normal
			if ((f.area(cutPoints) & normal()) < 0)
			{
				f.flip();
			}

			// the cut faces are usually quite ugly, so optionally triangulate
			if (triangulate)
			{
				label nTri = f.triangles(cutPoints, dynCutFaces);
				while (nTri--)
				{
					dynCutCells.append(celli);
				}
			}
			else
			{
				dynCutFaces.append(f);
				dynCutCells.append(celli);
			}
		}
	}

	this->storedFaces().transfer(dynCutFaces);
	cutCells_.transfer(dynCutCells);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void tnbLib::cuttingPlane::reCut
(
	const primitiveMesh& mesh,
	const bool triangulate,
	const labelUList& cellIdLabels
)
{
	MeshStorage::clear();
	cutCells_.clear();

	const scalarField dotProducts((mesh.points() - refPoint()) & normal());

	// Determine cells that are (probably) cut.
	calcCutCells(mesh, dotProducts, cellIdLabels);

	// Determine cutPoints and return list of edge cuts.
	// per edge -1 or the label of the intersection point
	labelList edgePoint;
	intersectEdges(mesh, dotProducts, edgePoint);

	// Do topological walk around cell to find closed loop.
	walkCellCuts(mesh, triangulate, edgePoint);
}


void tnbLib::cuttingPlane::remapFaces
(
	const labelUList& faceMap
)
{
	// recalculate the cells cut
	if (notNull(faceMap) && faceMap.size())
	{
		MeshStorage::remapFaces(faceMap);

		List<label> newCutCells(faceMap.size());
		forAll(faceMap, facei)
		{
			newCutCells[facei] = cutCells_[faceMap[facei]];
		}
		cutCells_.transfer(newCutCells);
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::cuttingPlane::cuttingPlane(const plane& pln)
	:
	plane(pln)
{}


tnbLib::cuttingPlane::cuttingPlane
(
	const plane& pln,
	const primitiveMesh& mesh,
	const bool triangulate,
	const labelUList& cellIdLabels
)
	:
	plane(pln)
{
	reCut(mesh, triangulate, cellIdLabels);
}


// ************************************************************************* //