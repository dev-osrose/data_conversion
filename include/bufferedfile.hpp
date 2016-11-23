#ifndef BUFFERED_FILE_H
#define BUFFERED_FILE_H

#include "file.hpp"
#include "safedelete.hpp"
#include <stdio.h>


#ifndef _WIN32

void memcpy_s( void* dest, size_t destsz,const void* src, size_t count ) {
  char *csrc = (char *)src;
  char *cdest = (char *)dest;
  
  if(cdest == nullptr || destsz <= 0 || count <= 0) return;
  if(count > destsz) count = destsz;

  for(int i = 0; i < count; ++i) {
    cdest[i] = csrc[i];
  }
}

#endif

class BufferedFile : public File {
public:
	BufferedFile() : mBuffer(0), mPosition(0), mSize(0) {}
	BufferedFile(const char* path, const char* method)
		: mBuffer(0), mPosition(0), mSize(0)
	{
		Open(path, method);
	}

	~BufferedFile(){
		Close();
	}

	bool Open(const char* path, const char* method){
		FILE* fh;
		fh = fopen(path, method);
		if(!fh) return false;
		fseek(fh, 0, SEEK_END);

		mPosition = 0;
		mSize = ftell(fh);
		mBuffer = new unsigned char[mSize];

		fseek(fh, 0, SEEK_SET);
		fread(mBuffer, 1, mSize, fh);
		fclose(fh);

		return true;
	}

	bool IsOpen(){
		return (mBuffer != NULL);
	}

	void Close(){
		SAFE_DELETE_ARRAY(mBuffer);
	}

	int ReadData(void* data, int size){
		if(mPosition + size > mSize) size = mSize - mPosition;
		memcpy_s(data, size, mBuffer + mPosition, size);
		mPosition += size;
		return size;
	}

	int WriteData(void* data, int size){
		memcpy_s(mBuffer + mPosition, mSize - mPosition, data, size);
		return size;
	}

	int WriteData(void* data, int size, int pos){
		memcpy_s(mBuffer + pos, mSize - pos, data, size);
		return size;
	}

	void Skip(int bytes){
		mPosition += bytes;
	}

	void Seek(int position){
		mPosition = position;
	}

	long Position(){
		return mPosition;
	}

	long Size(){
		return mSize;
	}

	bool IsEOF(){
		return mPosition >= mSize;
	}

private:
	long mSize;
	long mPosition;
	unsigned char* mBuffer;
};

#endif