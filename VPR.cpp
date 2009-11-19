#include "VPR.h"
#include "VPRUtil.h"

#include <iostream>
#include <float.h>

#include <boost/scoped_array.hpp>

#include <lsm_fast_marching_method.h>

using namespace std;
using namespace PR;

VolumetricParticleSolver::VolumetricParticleSolver()
{
	mRes[0] = mRes[1] = mRes[2] = 2;
}

VolumetricParticleSolver::~VolumetricParticleSolver()
{
}

RtVoid VolumetricParticleSolver::DoIt(RtInt NVert, RtInt N, RtToken Tokens[], RtPointer Data[])
{
	// Calculate BB
	RtPoint* PosPointer = NULL;
	for( int i=0; i<N; ++i )
	{
		if( strcmp(Tokens[i],"P") == 0 )
		{
			PosPointer = (RtPoint*)Data[i];
			break;
		}
	}

	Float3 BB[2];// Min & Max
	BB[0].x = BB[0].y = BB[0].z = FLT_MAX;
	BB[1].x = BB[1].y = BB[1].z = - FLT_MAX;
	for( int i=0; i<NVert; ++i )
	{
		BB[0].x = min( BB[0].x, PosPointer[i][0] );
		BB[0].y = min( BB[0].y, PosPointer[i][1] );
		BB[0].z = min( BB[0].z, PosPointer[i][2] );

		BB[1].x = max( BB[1].x, PosPointer[i][0] );
		BB[1].y = max( BB[1].y, PosPointer[i][1] );
		BB[1].z = max( BB[1].z, PosPointer[i][2] );
	}
	Float3 Size;
	Size.x = BB[1].x - BB[0].x;
	Size.y = BB[1].y - BB[0].y;
	Size.z = BB[1].z - BB[0].z;
	
	cout<<fixed<<"ParticleResolverPlugin : "<<"VolumetricParticleSolver Bounding Box "<<"["<<BB[0]<<' '<<BB[1]<<"]"<<endl;

	float3 BBCopy[2];
	BBCopy[0].x = BB[0].x;
	BBCopy[0].y = BB[0].y;
	BBCopy[0].z = BB[0].z;
	BBCopy[1].x = BB[1].x;
	BBCopy[1].y = BB[1].y;
	BBCopy[1].z = BB[1].z;

	boost::scoped_array<float> TentFunc(new float[mRes[0]*mRes[1]*mRes[2]]);
	boost::scoped_array<float> SDF(new float[mRes[0]*mRes[1]*mRes[2]]);
	bool Ok = MakeTent(NVert,(const float3*)PosPointer,mRes,BBCopy[0],BBCopy[1],TentFunc.get());
	if(Ok)
	{
		float DxDyDz[3];
		DxDyDz[0] = Size.x / (float)mRes[0];
		DxDyDz[1] = Size.y / (float)mRes[1];
		DxDyDz[2] = Size.z / (float)mRes[2];
		int LSMOk = computeDistanceFunction3d(SDF.get(),TentFunc.get(),NULL,1,mRes,DxDyDz);
		if( LSMOk != 0 )
		{
			cerr<<"ParticleResolverPlugin : "<<"VolumetricParticleSolver Can't Solve SDF."<<endl;
			return;
		}
	}

	/// \todo Write .BKM or .PRT file ?
}

void VolumetricParticleSolver::SetRes(const RtInt& X, const RtInt& Y, const RtInt& Z)
{
}
