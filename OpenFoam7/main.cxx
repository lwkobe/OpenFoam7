//#include <test.hxx>

#include <jobInfo.hxx>
#include <UniformField.hxx>
#include <vector.hxx>
#include <IOstreams.hxx>
using namespace tnbLib;
//#define FoamBase_STATIC_DEFINE
int main(int argc, char *argv[])
{
	//jobInfo_.perform();

	//tnbLib::Test_UniformField();
	UniformField<scalar> uf1(13.1);
	UniformField<vector> uf2(vector(1, 2, 3));

	Info << "uf1 = " << uf1[22] << "; uf2 = " << uf2[1002] << endl;

	return 0;
}