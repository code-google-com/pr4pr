#include "CompressedCache.h"

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <zlib.h>

#pragma comment(lib,"zlib32.lib")

CompressedCache::CompressedCache(const size_t CacheSize) : mCacheSize(CacheSize), mUsedSize(0), mBuffer(NULL), mGZFile(NULL)
{
}

CompressedCache::~CompressedCache()
{
	Close();
}

bool CompressedCache::Open(const char* Path)
{
	Close();

	mGZFile = gzopen(Path,"wb");
	if( mGZFile == NULL )
		return false;

	mBuffer = (char*)_aligned_malloc(mCacheSize,16);
	if( mBuffer == NULL )
		return false;

	return true;
}

size_t CompressedCache::Write(void* Data, const size_t Size)
{
	if( mBuffer == NULL || mGZFile == NULL )
		return 0;

	size_t ResidualSize = mCacheSize - mUsedSize;

	if( Size > ResidualSize )
	{
		if( mUsedSize )
			gzwrite(mGZFile,mBuffer,mUsedSize);
		gzwrite(mGZFile,mBuffer,Size);
		gzflush(mGZFile,Z_FULL_FLUSH);
	}else
	{
		memcpy(mBuffer+mUsedSize,Data,Size);
		mUsedSize += Size;
	}

	return Size;
}

void CompressedCache::Close()
{
	if( mGZFile )
	{
		if( mUsedSize )
		{
			gzwrite(mGZFile,mBuffer,mUsedSize);
			gzflush(mGZFile,Z_FULL_FLUSH);
		}
		gzclose(mGZFile);
	}
	if( mBuffer )
		_aligned_free(mBuffer);

	mGZFile = NULL;
	mBuffer = NULL;
}

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	FILE* FP = fopen("E:/dualscattering.pdf","rb");

	CompressedCache Cache(32<<20);
	Cache.Open("001.bin");

	const size_t Size = 2293396;
	char* InBuffer = new char[Size];

	fread(InBuffer,Size,1,FP);
	Cache.Write(InBuffer,Size);

	return 0;
}
