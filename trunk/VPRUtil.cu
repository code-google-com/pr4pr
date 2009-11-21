#include "VPRUtil.h"

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>

#include <boost/progress.hpp>
#include <boost/scoped_array.hpp>


struct TentKernel
{
	TentKernel(const float3& TheSP)
	{
		SP.x = TheSP.x;
		SP.y = TheSP.y;
		SP.z = TheSP.z;
	}
	
	__host__ __device__
	float operator()(const float3 P, const float Sum)
	{
		return Sum + (1.0f-abs(P.x-SP.x))*(1.0f-abs(P.y-SP.y))*(1.0-abs(P.z-SP.z));
	}
	
	float3 SP;
};

bool MakeTent(const size_t Len, const float3* Points, const int Res[3], const float3& Min, const float3& Max, float* Data)
{
	try
	{
		float3 Extent;
		Extent.x = Max.x - Min.x;
		Extent.y = Max.y - Min.y;
		Extent.z = Max.z - Min.z;
		
		float3 Delta;
		Delta.x = Extent.x / (float)Res[0];
		Delta.y = Extent.y / (float)Res[1];
		Delta.z = Extent.z / (float)Res[2];
		
		std::cout<<"MakeTent "<<Res[0]<<'\t'<<Res[1]<<'\t'<<Res[2]<<std::endl;
		const int TotalCount = Res[0]*Res[1]*Res[2];
		
		boost::scoped_array<float3> GridPosHst(new float3[TotalCount]);
		thrust::device_vector<float3> GridPosDev(TotalCount);
		thrust::device_vector<float> TentFuncDev(TotalCount,0.0f);
		
		for( int z=0; z<Res[2]; ++z )
		{
			int ZOffset = z*Res[0]*Res[1];
			for( int y=0; y<Res[1]; ++y )
			{
				int YOffset = y*Res[0];
				for( int x=0; x<Res[0]; ++x )
				{
					int XOffset = x;
					float3 P;
					P.x = ( (float)x+0.5f )*Delta.x + Min.x;
					P.y = ( (float)y+0.5f )*Delta.y + Min.y;
					P.z = ( (float)z+0.5f )*Delta.z + Min.z;
					GridPosHst[ZOffset+YOffset+XOffset] = P;
				}
			}
		}
		thrust::copy(GridPosHst.get(),GridPosHst.get()+TotalCount,GridPosDev.begin());
		GridPosHst.reset();

		boost::progress_display Progress(Len);
		boost::progress_timer Timer;
		
		std::cout<<"ParticleResolver : MakeTent Start to Calculate the Tent Function"<<std::endl;
		for( size_t i=0; i<Len; ++i )
		{
			thrust::transform(GridPosDev.begin(),GridPosDev.end(),TentFuncDev.begin(),TentFuncDev.end(),TentKernel(Points[i]));
			++Progress;
		}
		thrust::copy(TentFuncDev.begin(),TentFuncDev.end(),Data);

	}catch(...)
	{
		return false;
	}
	
	
	return true;
}
