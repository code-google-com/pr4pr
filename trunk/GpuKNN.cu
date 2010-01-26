/*
 * Copyright (c) 2010, Bo Zhou <Bo.Schwarzstein@gmail.com>
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
