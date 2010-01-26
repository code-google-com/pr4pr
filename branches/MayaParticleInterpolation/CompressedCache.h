#pragma once

class CompressedCache
{
public:
	CompressedCache(const size_t CacheSize = 32<<20);
	~CompressedCache();
	bool Open(const char* Path);
	size_t Write(void* Data, const size_t Size);
	void Close();
private:
	size_t mCacheSize;
	size_t mUsedSize;
	char* mBuffer;
	void* mGZFile;
};
