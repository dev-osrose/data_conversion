#ifndef BUFFERED_FILE_SYSTEM_H
#define BUFFERED_FILE_SYSTEM_H

#include "filesystem.hpp"
#include "bufferedfile.hpp"

class BufferedFileSystem : public FileSystem {
public:
	BufferedFileSystem(){}
	BufferedFileSystem(const char* baseDir){
		SetBaseDirectory(baseDir);
	}

	virtual ~BufferedFileSystem(){}

	virtual File* OpenFile(const char* path, const char* mode, bool dataFile = true, bool showError = true){
		std::string modeStr = mode;
		if(modeStr.find("w") != std::string::npos){
			if(showError) LOG("Error: BufferedFileSystem does not support file writing! ('{}' [{}])", path, mode);
			return NULL;
		}

		std::string realpath = path;
		if(dataFile) GetFullPath(realpath);

		BufferedFile* file = new BufferedFile();
		if(!file->Open(realpath.c_str(), mode)){
			if(showError) LOG("Error: BufferedFileSystem could not open file: '{}' [{}]", path, mode);
			delete file;
			return NULL;
		}

		return file;
	}

	virtual bool FileExists(const char* path){
		File* fh = OpenFile(path, "rb", true, false);
		if(!fh) return false;
		delete fh;
		return true;
	}

	virtual long FileSize(const char* path){
		File* fh = OpenFile(path, "rb");
		if(!fh) return 0;
		long size = fh->Size();
		delete fh;
		return size;
	}

	virtual void GetFullPath(std::string& path){
		path = mBaseDirectory + path;
	}

	void SetBaseDirectory(const char* dir){
		mBaseDirectory = dir;
		//mBaseDirectory.ReplaceAll("\"", "");
		//mBaseDirectory.ReplaceAll("/", "\\");
		//char end = mBaseDirectory.At(mBaseDirectory.Length() - 1);
		//if(end != '\\') mBaseDirectory += "\\";
		LOG("BufferedFileSystem Base Directory: '{}'", mBaseDirectory);
	}

	const char* GetBaseDirectory(){
		return mBaseDirectory.c_str();
	}

private:
	std::string mBaseDirectory;
};

#endif