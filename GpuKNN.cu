#include "PR.h"

#include "GpuKNN.h"

#include <thrust/transform.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sort.h>
#include <thrust/replace.h>
#include <thrust/functional.h>
#include <iostream>

NAMESPACE_PR_BEGIN

struct SqrtDist
{
	float3 Center;
	
	SqrtDist(const float3& P)
	{
		Center.x = P.x;
		Center.y = P.y;
		Center.z = P.z;
	}
    __host__ __device__ float operator()(const float3& P) const
    {
        float a = P.x - Center.x;
        float b = P.y - Center.y;
        float c = P.z - Center.z;
        return a*a + b*b + c*c;
    }
};

GpuKNN::GpuKNN(const size_t Count, const Float3* Points)
{
	mPoints.resize(Count);
	mIndices.resize(Count);
	const float3* p = (const float3*)(Points);
	thrust::copy(p,p+Count,mPoints.begin());
	thrust::sequence(mIndices.begin(), mIndices.end());
}

void GpuKNN::Lookup(const Float3& P, const float SqrtR, const size_t Count, int* NNIndices)
{
	thrust::device_vector<float> Dist(mPoints.size());
	float3 P2;
	P2.x = P.x;
	P2.y = P.y;
	P2.z = P.z;
	thrust::transform(mPoints.begin(), mPoints.end(),  Dist.begin(), SqrtDist(P2));
	thrust::sort_by_key(Dist.begin(),Dist.end(), mIndices.begin());
	thrust::copy(mIndices.begin(), mIndices.begin()+Count, NNIndices);
}

NAMESPACE_PR_END
