#define FoamRegisterDebugSwitch_EXPORT_DEFINE
#include <fixedBlended.hxx>

#include <fvMesh.hxx>
#include <volMesh.hxx>  // added by amir
#include <fvPatchFields.hxx>  // added by amir

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	makeSurfaceInterpolationScheme(fixedBlended);
}

// ************************************************************************* //