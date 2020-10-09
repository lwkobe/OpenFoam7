#include <nearWallFields.hxx>

#include <wordReList.hxx>
#include <findCellParticle.hxx>
#include <mappedPatchBase.hxx>
#include <OBJstream.hxx>
#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace functionObjects
	{
		defineTypeNameAndDebug(nearWallFields, 0);
		addToRunTimeSelectionTable(functionObject, nearWallFields, dictionary);
	}
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void tnbLib::functionObjects::nearWallFields::calcAddressing()
{
	// Count number of faces
	label nPatchFaces = 0;
	forAllConstIter(labelHashSet, patchSet_, iter)
	{
		label patchi = iter.key();
		nPatchFaces += mesh_.boundary()[patchi].size();
	}

	// Global indexing
	globalIndex globalWalls(nPatchFaces);

	DebugInFunction << "nPatchFaces: " << globalWalls.size() << endl;

	// Construct cloud
	Cloud<findCellParticle> cloud
	(
		mesh_,
		cloud::defaultName,
		IDLList<findCellParticle>()
	);

	// Add particles to track to sample locations
	nPatchFaces = 0;

	forAllConstIter(labelHashSet, patchSet_, iter)
	{
		label patchi = iter.key();
		const fvPatch& patch = mesh_.boundary()[patchi];

		vectorField nf(patch.nf());

		forAll(patch, patchFacei)
		{
			cloud.addParticle
			(
				new findCellParticle
				(
					mesh_,
					patch.Cf()[patchFacei],
					patch.faceCells()[patchFacei],
					-distance_ * nf[patchFacei],
					globalWalls.toGlobal(nPatchFaces) // passive data
				)
			);

			nPatchFaces++;
		}
	}



	if (debug)
	{
		// Dump particles
		OBJstream str
		(
			mesh_.time().path()
			/ "wantedTracks_" + mesh_.time().timeName() + ".obj"
		);
		InfoInFunction << "Dumping tracks to " << str.name() << endl;

		forAllConstIter(Cloud<findCellParticle>, cloud, iter)
		{
			const vector p = iter().position();
			str.write(linePointRef(p, p + iter().displacement()));
		}
	}



	// Per cell: empty or global wall index and end location
	cellToWalls_.setSize(mesh_.nCells());
	cellToSamples_.setSize(mesh_.nCells());

	// Database to pass into findCellParticle::move
	findCellParticle::trackingData td(cloud, cellToWalls_, cellToSamples_);

	// Track all particles to their end position.
	scalar maxTrackLen = 2.0*mesh_.bounds().mag();


	// Debug: collect start points
	pointField start;
	if (debug)
	{
		start.setSize(nPatchFaces);
		nPatchFaces = 0;
		forAllConstIter(Cloud<findCellParticle>, cloud, iter)
		{
			const findCellParticle& tp = iter();
			start[nPatchFaces++] = tp.position();
		}
	}


	cloud.move(cloud, td, maxTrackLen);


	// Rework cell-to-globalpatchface into a map
	List<Map<label>> compactMap;
	getPatchDataMapPtr_.reset
	(
		new mapDistribute
		(
			globalWalls,
			cellToWalls_,
			compactMap
		)
	);


	// Debug: dump resulting tracks
	if (debug)
	{
		getPatchDataMapPtr_().distribute(start);
		{
			OBJstream str
			(
				mesh_.time().path()
				/ "obtainedTracks_" + mesh_.time().timeName() + ".obj"
			);
			InfoInFunction << "Dumping obtained to " << str.name() << endl;

			forAll(cellToWalls_, celli)
			{
				const List<point>& ends = cellToSamples_[celli];
				const labelList& cData = cellToWalls_[celli];
				forAll(cData, i)
				{
					str.write(linePointRef(ends[i], start[cData[i]]));
				}
			}
		}
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::functionObjects::nearWallFields::nearWallFields
(
	const word& name,
	const Time& runTime,
	const dictionary& dict
)
	:
	fvMeshFunctionObject(name, runTime, dict),
	fieldSet_()
{
	read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

tnbLib::functionObjects::nearWallFields::~nearWallFields()
{
	DebugInFunction << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool tnbLib::functionObjects::nearWallFields::read(const dictionary& dict)
{
	fvMeshFunctionObject::read(dict);

	dict.lookup("fields") >> fieldSet_;
	patchSet_ =
		mesh_.boundaryMesh().patchSet(wordReList(dict.lookup("patches")));
	distance_ = readScalar(dict.lookup("distance"));


	// Clear out any previously loaded fields
	vsf_.clear();
	vvf_.clear();
	vSpheretf_.clear();
	vSymmtf_.clear();
	vtf_.clear();
	fieldMap_.clear();
	reverseFieldMap_.clear();


	// Generate fields with mappedField boundary condition

	// Convert field to map
	fieldMap_.resize(2 * fieldSet_.size());
	reverseFieldMap_.resize(2 * fieldSet_.size());
	forAll(fieldSet_, setI)
	{
		const word& fldName = fieldSet_[setI].first();
		const word& sampleFldName = fieldSet_[setI].second();

		fieldMap_.insert(fldName, sampleFldName);
		reverseFieldMap_.insert(sampleFldName, fldName);
	}

	TnbLog << type() << " " << name()
		<< ": Sampling " << fieldMap_.size() << " fields" << endl;

	// Do analysis
	calcAddressing();

	return true;
}


bool tnbLib::functionObjects::nearWallFields::execute()
{
	DebugInFunction << endl;

	if
		(
			fieldMap_.size()
			&& vsf_.empty()
			&& vvf_.empty()
			&& vSpheretf_.empty()
			&& vSymmtf_.empty()
			&& vtf_.empty()
			)
	{
		TnbLog << type() << " " << name()
			<< ": Creating " << fieldMap_.size() << " fields" << endl;

		createFields(vsf_);
		createFields(vvf_);
		createFields(vSpheretf_);
		createFields(vSymmtf_);
		createFields(vtf_);

		TnbLog << endl;
	}

	TnbLog << type() << " " << name()
		<< " write:" << nl
		<< "    Sampling fields to " << time_.timeName()
		<< endl;

	sampleFields(vsf_);
	sampleFields(vvf_);
	sampleFields(vSpheretf_);
	sampleFields(vSymmtf_);
	sampleFields(vtf_);

	return true;
}


bool tnbLib::functionObjects::nearWallFields::write()
{
	DebugInFunction << endl;

	TnbLog << "    Writing sampled fields to " << time_.timeName()
		<< endl;

	forAll(vsf_, i)
	{
		vsf_[i].write();
	}
	forAll(vvf_, i)
	{
		vvf_[i].write();
	}
	forAll(vSpheretf_, i)
	{
		vSpheretf_[i].write();
	}
	forAll(vSymmtf_, i)
	{
		vSymmtf_[i].write();
	}
	forAll(vtf_, i)
	{
		vtf_[i].write();
	}

	return true;
}


// ************************************************************************* //