#ifndef GPUKNN
#define GPUKNN

#include <thrust/device_vector>

class GpuKNN
{
public:
	GpuKNN(const size_t Count, const Float3* Points);
	void Lookup(const Float3& P, const float SqrtR, const size_t Count, int* NNIndices);
private:
	thrust::device_vector<float3> mPoints;
	thrust::device_vector<int> mIndices;
};

#endif
