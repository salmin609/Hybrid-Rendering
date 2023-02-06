#pragma once
#include <GL/glew.h>

template <typename T>
class Buffer
{
public:
	Buffer(GLenum type, unsigned size, GLenum usage, void* data);
	void Bind(unsigned uniformBufferSlot = 0);
	void BindStorage(int index);
	void BindStorage();
	void BindStorageBuffer(int storageIndex, unsigned size);
	unsigned GetId();

	template <typename T>
	void Check();

	~Buffer();

private:
	unsigned bufferId;
	GLenum type;
	int storageIndex;
	int size;
	T type;

};

template <typename T>
void Buffer::Check()
{
	if (type == GL_SHADER_STORAGE_BUFFER)
	{
		BindStorage();
	}
	else if (type == GL_ARRAY_BUFFER)
	{
		Bind();
	}

	T* checkVal = static_cast<T*>(glMapBufferRange(type, 0, size,
		GL_MAP_READ_BIT));
	std::vector<T> check;
	int sizeCheck = size / sizeof(T);
	check.reserve(size);
	for (int i = 0; i < sizeCheck; ++i)
	{
		check.push_back(checkVal[i]);
	}
	glUnmapBuffer(type);
}

inline Buffer::Buffer(GLenum type, unsigned sizeVal, GLenum usage, void* data) : type(type)
{
	storageIndex = 0;
	size = sizeVal;

	glGenBuffers(1, &bufferId);
	glBindBuffer(type, bufferId);
	glBufferData(type, sizeVal, data, usage);
}

inline void Buffer::Bind(unsigned uniformBufferSlot)
{
	switch (type)
	{
	case GL_UNIFORM_BUFFER:
		glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferSlot, bufferId);
		break;
	case GL_ARRAY_BUFFER:
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		break;
	case GL_SHADER_STORAGE_BUFFER:
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		break;
	default:;
	}
}

inline void Buffer::BindStorage(int index)
{
	storageIndex = index;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, bufferId);
}

inline void Buffer::BindStorage()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storageIndex, bufferId);
}

inline void Buffer::BindStorageBuffer(int storageIndexVal, unsigned sizeVal)
{
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, storageIndexVal, bufferId, 0, sizeVal);
}

inline unsigned Buffer::GetId()
{
	return bufferId;
}

inline Buffer::~Buffer()
{
	glDeleteBuffers(1, &bufferId);
}
