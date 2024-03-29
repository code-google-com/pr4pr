﻿/*
* Copyright (c) 1998, Bo Zhou <Bo.Schwarzstein@gmail.com>
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Bo Zhou, nor the names of its contributors
*       may be used to endorse or promote products derived from this
*       software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <memory>

#include "PR.h"
#include "OPR.h"
#include "DPR.h"
#include "EPR.h"
#include "VPR.h"
#include "KPR.h"

using namespace std;
using namespace PR;

#define PRTYPE "PRType"
#define NCOPIES "NCopies"
#define RANDPATTERN "RandPattern"
#define FALLOFF "Falloff"
#define PATH "Path"
#define PARAMETERS "Parameters"
#define SEED "Seed"
#define BOUND "Bound"
#define VOLRES "VolRes"

///////////////////////////////////////////////////////////////////////////////////////////////////////

DLLEXPORT
RifPlugin* RifPluginManufacture(int argc, char **argv)
{
	return new ParticleResolverPlugin(argc,argv);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

auto_ptr<ParticleResolver> ParticleResolverPlugin::Instance;
ParticleResolver::Type ParticleResolverPlugin::CurrentPR;

RtVoid ParticleResolverPlugin::AttributeV(RtToken Name, RtInt N, RtToken Tokens[], RtPointer Data[])
{
	if( strcmp("user",Name) == 0 )
	{
		for( int i=0; i<N; ++i )
		{
			if( strstr(Tokens[i],PRTYPE) )
			{
				int* p = (int*)Data[0];
				switch( *p )
				{
				case 0:
					cout<<"ParticleResolverPlugin : Using OriginParticleResolver"<<endl;
					Instance.reset( new OriginParticleResolver );
					CurrentPR = ParticleResolver::OPR;
					break;
				case 1:
					cout<<"ParticleResolverPlugin : Using DiffusionParticleResolver"<<endl;
					Instance.reset( new DiffusionParticleResolver );
					CurrentPR = ParticleResolver::DPR;
					break;
				case 2:
					cout<<"ParticleResolverPlugin : Using ExternalParticleResolver"<<endl;
					Instance.reset( new ExternalParticleResolver );
					CurrentPR = ParticleResolver::EPR;
					break;
				case 3:
					cout<<"ParticleResolverPlugin : Using KrakatoaParticleResolver"<<endl;
					Instance.reset( new KrakatoaParticleResolver );
					CurrentPR = ParticleResolver::KPR;
					break;
				case 4:
					cout<<"ParticleResolverPlugin : Using VolumetricParticleSolver"<<endl;
					Instance.reset( new VolumetricParticleSolver );
					CurrentPR = ParticleResolver::VPR;
					break;
				}

				continue;
			}else if( strstr(Tokens[i],NCOPIES) )
			{
				if( CurrentPR == ParticleResolver::DPR )
				{
					int NCopies = *(int*)Data[0];
					DiffusionParticleResolver* p = dynamic_cast<DiffusionParticleResolver*>( Instance.get() );
					p->SetNCopies(NCopies);
				}else if( CurrentPR == ParticleResolver::VPR )
				{
					int NCopies = *(int*)Data[0];
					VolumetricParticleSolver* p = dynamic_cast<VolumetricParticleSolver*>( Instance.get() );
					p->SetNCopies(NCopies);
				}else
				{
					cerr<<"ParticleResolverPlugin : Only DiffusionParticleResolver Or VolumetricParticleSolver Use Attribute ["<<NCOPIES<<"]"<<endl;
				}

				continue;
			}else if( strstr(Tokens[i],RANDPATTERN) )
			{
				int RandPattern = *(int*)Data[0];
				DiffusionParticleResolver* p = dynamic_cast<DiffusionParticleResolver*>( Instance.get() );
				if( p )
				{
					p->SetRandPattern(RandPattern);
					cout<<"ParticleResolverPlugin : RandPattern ["<< RandPattern <<"]"<<endl;
				}else
				{
					cerr<<"ParticleResolverPlugin : Only DiffusionParticleResolver Use Attribute ["<<RANDPATTERN<<"]"<<endl;
				}

				continue;
			}else if( strstr(Tokens[i],FALLOFF) )
			{
				float Falloff = *(float*)Data[0];
				DiffusionParticleResolver* p = dynamic_cast<DiffusionParticleResolver*>( Instance.get() );
				if( p )
				{
					p->SetFalloff(Falloff);
					cout<<fixed<<"ParticleResolverPlugin : Falloff Coefficient ["<< Falloff <<"]"<<endl;
				}else
				{
					cerr<<"ParticleResolverPlugin : Only DiffusionParticleResolver Use Attribute ["<<FALLOFF<<"]"<<endl;
				}

				continue;
			}else if( strstr(Tokens[i],SEED) )
			{
				int Seed = *(int*)Data[0];
				DiffusionParticleResolver* p = dynamic_cast<DiffusionParticleResolver*>( Instance.get() );
				if( p )
				{
					p->SetSeed(Seed);
					cout<<fixed<<"ParticleResolverPlugin : Random Seed ["<< Seed <<"]"<<endl;
				}else
				{
					cerr<<"ParticleResolverPlugin : Only DiffusionParticleResolver Use Attribute ["<<SEED<<"]"<<endl;
				}

				continue;
			}else if( strstr(Tokens[i],BOUND) )
			{
				// \todo We have to deal with array.
				float* Bound = (float*)Data;
				RtBound B;
				B[0] = Bound[0];
				B[1] = Bound[1];
				B[2] = Bound[2];
				B[3] = Bound[3];
				B[4] = Bound[4];
				B[5] = Bound[5];
				ExternalParticleResolver* p = dynamic_cast<ExternalParticleResolver*>( Instance.get() );
				if( p )
				{
					p->SetBound(B);
					cout<<fixed<<"ParticleResolverPlugin : ExternalParticleResolver Use Bound ["<<B[0]<<' '<<B[1]<<' '<<B[2]<<' '<<B[3]<<' '<<B[4]<<' '<<B[5]<<"]"<<endl;
				}else
				{
					cerr<<"ParticleResolverPlugin : Only ExternalParticleResolver Use Attribute ["<<BOUND<<"]"<<endl;
				}

				continue;
			}else if( strstr(Tokens[i],VOLRES) )
			{
				int** Res = (int**)Data;
				VolumetricParticleSolver* p = dynamic_cast<VolumetricParticleSolver*>( Instance.get() );
				int X = *Res[0];
				int Y = *Res[1];
				int Z = *Res[2];
				p->SetRes(X,Y,Z);
				continue;
			}else if( strstr(Tokens[i],PATH) )
			{
				if( CurrentPR == ParticleResolver::VPR )
				{
					char* Path = * (char**)Data[0];
					VolumetricParticleSolver* p = dynamic_cast<VolumetricParticleSolver*>( Instance.get() );
					p->SetBkmPath( Path );
				}else if( CurrentPR == ParticleResolver::KPR )
				{
					char* Path = * (char**)Data[0];
					KrakatoaParticleResolver* p = dynamic_cast<KrakatoaParticleResolver*>( Instance.get() );
					p->SetPath(Path);
				}
			}
		}
	}
	RiAttributeV(Name,N,Tokens,Data);
}

RtVoid ParticleResolverPlugin::PointsV(RtInt NVerts, RtInt N, RtToken Tokens[], RtPointer Data[])
{
	cout<<"ParticleResolverPlugin : Found ["<<NVerts<<"] And ["<<N<<"] Tokens"<<endl;
	Instance->DoIt(NVerts,N,Tokens,Data);
	Instance.reset( new OriginParticleResolver );
	CurrentPR = ParticleResolver::OPR;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ParticleResolverPlugin::ParticleResolverPlugin(int argc, char** argv)
{
	mRF.AttributeV = ParticleResolverPlugin::AttributeV;
	mRF.PointsV = ParticleResolverPlugin::PointsV;

	Instance.reset( new OriginParticleResolver );
	CurrentPR = ParticleResolver::OPR;
}

ParticleResolverPlugin::~ParticleResolverPlugin()
{
	Instance.reset();
}

RifFilter& ParticleResolverPlugin::GetFilter()
{
	return mRF;
}
