#include "VPR.h"
#include "VPRUtil.h"

#ifdef _MSC_VER
	#include <process.h>
	#include <stdio.h>
#else
	#include <stdio.h>
#endif

#include <iostream>
#include <float.h>
#include <time.h>

#include <boost/scoped_array.hpp>

#include <lsm_fast_marching_method.h>

using namespace std;
using namespace PR;

VolumetricParticleSolver::VolumetricParticleSolver() : mNCopies(0)
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
	
	cout<<fixed<<"ParticleResolverPlugin : "<<"VolumetricParticleSolver Bounding Box "<<endl<<"\t["<<BB[0]<<' '<<BB[1]<<"]"<<endl;

	float3 BBCopy[2];
	BBCopy[0].x = BB[0].x;
	BBCopy[0].y = BB[0].y;
	BBCopy[0].z = BB[0].z;
	BBCopy[1].x = BB[1].x;
	BBCopy[1].y = BB[1].y;
	BBCopy[1].z = BB[1].z;

	const size_t TotalCount = mRes[0]*mRes[1]*mRes[2];
	boost::scoped_array<float> TentFunc(new float[TotalCount]);
	boost::scoped_array<float> SDF(new float[TotalCount]);
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

		// Dump for test
		char CharBuf[256];
		sprintf(CharBuf,"%d.raw",time(NULL));
		FILE* FP = fopen(CharBuf,"wb");
		fwrite(SDF.get(),sizeof(float),TotalCount,FP);
		fflush(FP);
		fclose(FP);
	}
//
//	 \todo Write .BKM or .PRT file ?
//	wstring BkmMakerBinPath, CmdArg;
//	intptr_t Ret = -1;
//#ifdef _MSC_VER
//	wchar_t* SysVar = _wgetenv(L"RMANTREE");
//	if( SysVar == NULL )
//	{
//		return;
//	}
//	BkmMakerBinPath.append(L"bin\\brickmake.exe");
//	Ret = _wspawnl(_P_WAIT,BkmMakerBinPath.c_str(),CmdArg.c_str(),NULL);
//#else
//	BkmMakerBinPath = wgetenv(L"RMANTREE");
//	BkmMakerBinPath.append(L"bin/brickmake");
//	Ret = wspawnl(P_WAIT,BkmMakerBinPath.c_str(),CmdArg.c_str(),NULL);
//#endif
//
//	if( Ret != 0 )
//	{
//		cerr<<"ParticleResolverPlugin : "<<"VolumetricParticleSolver Have Not Got Success From brickmake"<<endl;
//	}
}

void VolumetricParticleSolver::SetRes(const RtInt& X, const RtInt& Y, const RtInt& Z)
{
	mRes[0] = X;
	mRes[1] = Y;
	mRes[2] = Z;
}

void VolumetricParticleSolver::SetNCopies(const RtInt& NCopies)
{
	mNCopies = NCopies;
}

void VolumetricParticleSolver::SetBkmPath(const char* Path)
{
	mBkmPath = Path;
}
