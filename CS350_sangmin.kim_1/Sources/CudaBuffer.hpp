//#pragma once
////#include <cuda_runtime_api.h>
//
//template <typename T>
//class CudaBuffer
//{
//public:
//	CudaBuffer(int memorySize);
//	~CudaBuffer();
//
//	T* GetDeviceMemoryPtr();
//	T* GetHostMemoryPtr();
//	void CopyDeviceToHost();
//	void CopyHostToDevice();
//	void Realloc(int num);
//	int GetLastIndexBeforeRealloc();
//	int GetMemorySize();
//	int GetIndicesNum();
//private:
//	T* dDataPtr;
//	T* dataPtr;
//	int memSize;
//	int indicesNum;
//	int lastIndexBeforeRealloc;
//};
//
//template <typename T>
//CudaBuffer<T>::CudaBuffer(int memorySize)
//{
//	memSize = memorySize;
//	indicesNum = memSize / sizeof(T);
//	dataPtr = new T[indicesNum];
//	memset(dataPtr, 0, memSize);
//	cudaMalloc(reinterpret_cast<void**>(&dDataPtr), memSize);
//	CopyHostToDevice();
//}
//
//template <typename T>
//T* CudaBuffer<T>::GetDeviceMemoryPtr()
//{
//	return dDataPtr;
//}
//
//template <typename T>
//T* CudaBuffer<T>::GetHostMemoryPtr()
//{
//	return dataPtr;
//}
//
//template <typename T>
//void CudaBuffer<T>::CopyDeviceToHost()
//{
//	cudaMemcpy(dataPtr, dDataPtr, memSize, cudaMemcpyDeviceToHost);
//}
//
//template <typename T>
//void CudaBuffer<T>::CopyHostToDevice()
//{
//	cudaMemcpy(dDataPtr, dataPtr, memSize, cudaMemcpyHostToDevice);
//}
//
//template <typename T>
//void CudaBuffer<T>::Realloc(int num)
//{
//	CopyDeviceToHost();
//	cudaFree(dDataPtr);
//	//void* dataToVoid = static_cast<void*>(dDataPtr);
//
//	lastIndexBeforeRealloc = indicesNum;
//
//	indicesNum += num;
//	memSize = sizeof(T) * indicesNum;
//
//	dataPtr = static_cast<T*>(realloc(dataPtr, memSize));
//
//	cudaMalloc(reinterpret_cast<void**>(&dDataPtr), memSize);
//	CopyHostToDevice();
//}
//
//template <typename T>
//int CudaBuffer<T>::GetLastIndexBeforeRealloc()
//{
//	return lastIndexBeforeRealloc;
//}
//
//template <typename T>
//int CudaBuffer<T>::GetMemorySize()
//{
//	return memSize;
//}
//
//template <typename T>
//int CudaBuffer<T>::GetIndicesNum()
//{
//	return indicesNum;
//}
//
//template <typename T>
//CudaBuffer<T>::~CudaBuffer()
//{
//	cudaFree(dDataPtr);
//	delete[] dataPtr;
//}
