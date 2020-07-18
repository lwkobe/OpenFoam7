#pragma once
#include <polyMesh.hxx>
#include <transform.hxx>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::update
(
	const point& pt,
	const pointEdgePoint& w2,
	const scalar tol,
	TrackingData& td
)
{
	scalar dist2 = magSqr(pt - w2.origin());

	if (!valid(td))
	{
		// current not yet set so use any value
		distSqr_ = dist2;
		origin_ = w2.origin();

		return true;
	}

	scalar diff = distSqr_ - dist2;

	if (diff < 0)
	{
		// already nearer to pt
		return false;
	}

	if ((diff < small) || ((distSqr_ > small) && (diff / distSqr_ < tol)))
	{
		// don't propagate small changes
		return false;
	}
	else
	{
		// update with new values
		distSqr_ = dist2;
		origin_ = w2.origin();

		return true;
	}
}


// Update this with w2 (information on same point)
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::update
(
	const pointEdgePoint& w2,
	const scalar tol,
	TrackingData& td
)
{
	if (!valid(td))
	{
		// current not yet set so use any value
		distSqr_ = w2.distSqr();
		origin_ = w2.origin();

		return true;
	}

	scalar diff = distSqr_ - w2.distSqr();

	if (diff < 0)
	{
		// already nearer to pt
		return false;
	}

	if ((diff < small) || ((distSqr_ > small) && (diff / distSqr_ < tol)))
	{
		// don't propagate small changes
		return false;
	}
	else
	{
		// update with new values
		distSqr_ = w2.distSqr();
		origin_ = w2.origin();

		return true;
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline tnbLib::pointEdgePoint::pointEdgePoint()
	:
	origin_(point::max),
	distSqr_(great)
{}


inline tnbLib::pointEdgePoint::pointEdgePoint
(
	const point& origin,
	const scalar distSqr
)
	:
	origin_(origin),
	distSqr_(distSqr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const tnbLib::point& tnbLib::pointEdgePoint::origin() const
{
	return origin_;
}


inline tnbLib::scalar tnbLib::pointEdgePoint::distSqr() const
{
	return distSqr_;
}


template<class TrackingData>
inline bool tnbLib::pointEdgePoint::valid(TrackingData& td) const
{
	return origin_ != point::max;
}


// Checks for cyclic points
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::sameGeometry
(
	const pointEdgePoint& w2,
	const scalar tol,
	TrackingData& td
) const
{
	scalar diff = tnbLib::mag(distSqr() - w2.distSqr());

	if (diff < small)
	{
		return true;
	}
	else
	{
		if ((distSqr() > small) && ((diff / distSqr()) < tol))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}


template<class TrackingData>
inline void tnbLib::pointEdgePoint::leaveDomain
(
	const polyPatch& patch,
	const label patchPointi,
	const point& coord,
	TrackingData& td
)
{
	origin_ -= coord;
}


template<class TrackingData>
inline void tnbLib::pointEdgePoint::transform
(
	const tensor& rotTensor,
	TrackingData& td
)
{
	origin_ = tnbLib::transform(rotTensor, origin_);
}


// Update absolute geometric quantities. Note that distance (distSqr_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void tnbLib::pointEdgePoint::enterDomain
(
	const polyPatch& patch,
	const label patchPointi,
	const point& coord,
	TrackingData& td
)
{
	// back to absolute form
	origin_ += coord;
}


// Update this with information from connected edge
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::updatePoint
(
	const polyMesh& mesh,
	const label pointi,
	const label edgeI,
	const pointEdgePoint& edgeInfo,
	const scalar tol,
	TrackingData& td
)
{
	return update(mesh.points()[pointi], edgeInfo, tol, td);
}


// Update this with new information on same point
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::updatePoint
(
	const polyMesh& mesh,
	const label pointi,
	const pointEdgePoint& newPointInfo,
	const scalar tol,
	TrackingData& td
)
{
	return update(mesh.points()[pointi], newPointInfo, tol, td);
}


// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::updatePoint
(
	const pointEdgePoint& newPointInfo,
	const scalar tol,
	TrackingData& td
)
{
	return update(newPointInfo, tol, td);
}


// Update this with information from connected point
template<class TrackingData>
inline bool tnbLib::pointEdgePoint::updateEdge
(
	const polyMesh& mesh,
	const label edgeI,
	const label pointi,
	const pointEdgePoint& pointInfo,
	const scalar tol,
	TrackingData& td
)
{
	const edge& e = mesh.edges()[edgeI];
	return update(e.centre(mesh.points()), pointInfo, tol, td);
}


template<class TrackingData>
inline bool tnbLib::pointEdgePoint::equal
(
	const pointEdgePoint& rhs,
	TrackingData& td
) const
{
	return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool tnbLib::pointEdgePoint::operator==(const tnbLib::pointEdgePoint& rhs)
const
{
	return (origin() == rhs.origin()) && (distSqr() == rhs.distSqr());
}


inline bool tnbLib::pointEdgePoint::operator!=(const tnbLib::pointEdgePoint& rhs)
const
{
	return !(*this == rhs);
}


// ************************************************************************* //