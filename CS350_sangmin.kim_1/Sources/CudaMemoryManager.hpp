#pragma once
#include <cuda_runtime_api.h>

class CudaMemoryManager 
{
public:
	template <typename T>
	CudaMemoryManager(int memorySize);
	~CudaMemoryManager();

	template <typename T>
	T* GetDataPtr();
private:
	void* dataPtr;
};

template <typename T>
inline CudaMemoryManager::CudaMemoryManager(int memorySize)
{
	T* data;
	
	cudaMalloc(&data, memorySize);
	
	dataPtr = reinterpret_cast<void*>(data);
}

template <typename T>
T* CudaMemoryManager::GetDataPtr()
{
	return reinterpret_cast<T*>(dataPtr);
}

inline CudaMemoryManager::~CudaMemoryManager()
{
	cudaFree(dataPtr);
}
