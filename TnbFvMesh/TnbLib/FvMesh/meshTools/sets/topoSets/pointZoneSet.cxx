#include <pointZoneSet.hxx>

#include <mapPolyMesh.hxx>
#include <polyMesh.hxx>
#include <processorPolyPatch.hxx>
#include <cyclicPolyPatch.hxx>

#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

	defineTypeNameAndDebug(pointZoneSet, 0);

	addToRunTimeSelectionTable(topoSet, pointZoneSet, word);
	addToRunTimeSelectionTable(topoSet, pointZoneSet, size);
	addToRunTimeSelectionTable(topoSet, pointZoneSet, set);


	// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

	void pointZoneSet::updateSet()
	{
		labelList order;
		sortedOrder(addressing_, order);
		inplaceReorder(order, addressing_);

		pointSet::clearStorage();
		pointSet::resize(2 * addressing_.size());
		forAll(addressing_, i)
		{
			pointSet::insert(addressing_[i]);
		}
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	pointZoneSet::pointZoneSet
	(
		const polyMesh& mesh,
		const word& name,
		readOption r,
		writeOption w
	)
		:
		pointSet(mesh, name, 1000),  // do not read pointSet
		mesh_(mesh),
		addressing_(0)
	{
		const pointZoneMesh& pointZones = mesh.pointZones();
		label zoneID = pointZones.findZoneID(name);

		if
			(
				r == IOobject::MUST_READ
				|| r == IOobject::MUST_READ_IF_MODIFIED
				|| (r == IOobject::READ_IF_PRESENT && zoneID != -1)
				)
		{
			const pointZone& fz = pointZones[zoneID];
			addressing_ = fz;
		}

		updateSet();

		check(mesh.nPoints());
	}


	pointZoneSet::pointZoneSet
	(
		const polyMesh& mesh,
		const word& name,
		const label size,
		writeOption w
	)
		:
		pointSet(mesh, name, size, w),
		mesh_(mesh),
		addressing_(0)
	{
		updateSet();
	}


	pointZoneSet::pointZoneSet
	(
		const polyMesh& mesh,
		const word& name,
		const topoSet& set,
		writeOption w
	)
		:
		pointSet(mesh, name, set.size(), w),
		mesh_(mesh),
		addressing_(refCast<const pointZoneSet>(set).addressing())
	{
		updateSet();
	}


	// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

	pointZoneSet::~pointZoneSet()
	{}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	void pointZoneSet::invert(const label maxLen)
	{
		// Count
		label n = 0;

		for (label pointi = 0; pointi < maxLen; pointi++)
		{
			if (!found(pointi))
			{
				n++;
			}
		}

		// Fill
		addressing_.setSize(n);
		n = 0;

		for (label pointi = 0; pointi < maxLen; pointi++)
		{
			if (!found(pointi))
			{
				addressing_[n] = pointi;
				n++;
			}
		}
		updateSet();
	}


	void pointZoneSet::subset(const topoSet& set)
	{
		DynamicList<label> newAddressing(addressing_.size());

		const pointZoneSet& fSet = refCast<const pointZoneSet>(set);

		forAll(fSet.addressing(), i)
		{
			label pointi = fSet.addressing()[i];

			if (found(pointi))
			{
				newAddressing.append(pointi);
			}
		}

		addressing_.transfer(newAddressing);
		updateSet();
	}


	void pointZoneSet::addSet(const topoSet& set)
	{
		DynamicList<label> newAddressing(addressing_);

		const pointZoneSet& fSet = refCast<const pointZoneSet>(set);

		forAll(fSet.addressing(), i)
		{
			label pointi = fSet.addressing()[i];

			if (!found(pointi))
			{
				newAddressing.append(pointi);
			}
		}

		addressing_.transfer(newAddressing);
		updateSet();
	}


	void pointZoneSet::deleteSet(const topoSet& set)
	{
		DynamicList<label> newAddressing(addressing_.size());

		const pointZoneSet& fSet = refCast<const pointZoneSet>(set);

		forAll(addressing_, i)
		{
			label pointi = addressing_[i];

			if (!fSet.found(pointi))
			{
				// Not found in fSet so add
				newAddressing.append(pointi);
			}
		}

		addressing_.transfer(newAddressing);
		updateSet();
	}


	void pointZoneSet::sync(const polyMesh& mesh)
	{
		pointSet::sync(mesh);

		// Take over contents of pointSet into addressing.
		addressing_ = sortedToc();
		updateSet();
	}


	label pointZoneSet::maxSize(const polyMesh& mesh) const
	{
		return mesh.nPoints();
	}


	bool pointZoneSet::writeObject
	(
		IOstream::streamFormat s,
		IOstream::versionNumber v,
		IOstream::compressionType c,
		const bool write
	) const
	{
		// Write shadow pointSet
		word oldTypeName = typeName;
		const_cast<word&>(type()) = pointSet::typeName;
		bool ok = pointSet::writeObject(s, v, c, write);
		const_cast<word&>(type()) = oldTypeName;

		// Modify pointZone
		pointZoneMesh& pointZones = const_cast<polyMesh&>(mesh_).pointZones();
		label zoneID = pointZones.findZoneID(name());

		if (zoneID == -1)
		{
			zoneID = pointZones.size();

			pointZones.setSize(zoneID + 1);
			pointZones.set
			(
				zoneID,
				new pointZone
				(
					name(),
					addressing_,
					zoneID,
					pointZones
				)
			);
		}
		else
		{
			pointZones[zoneID] = addressing_;
		}
		pointZones.clearAddressing();

		return ok && pointZones.write(write);
	}


	void pointZoneSet::updateMesh(const mapPolyMesh& morphMap)
	{
		// pointZone
		labelList newAddressing(addressing_.size());

		label n = 0;
		forAll(addressing_, i)
		{
			label pointi = addressing_[i];
			label newPointi = morphMap.reversePointMap()[pointi];
			if (newPointi >= 0)
			{
				newAddressing[n] = newPointi;
				n++;
			}
		}
		newAddressing.setSize(n);

		addressing_.transfer(newAddressing);

		updateSet();
	}


	void pointZoneSet::writeDebug
	(
		Ostream& os,
		const primitiveMesh& mesh,
		const label maxLen
	) const
	{
		pointSet::writeDebug(os, mesh, maxLen);
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //