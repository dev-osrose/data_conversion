#ifndef FLAT_FILE_SYSTEM_H
#define FLAT_FILE_SYSTEM_H

#include "filesystem.hpp"
#include "flatfile.hpp"

class FlatFileSystem : public FileSystem {
public:
	FlatFileSystem(){}
	FlatFileSystem(const char* baseDir){
		SetBaseDirectory(baseDir);
	}

	virtual ~FlatFileSystem(){}

	virtual File* OpenFile(const char* path, const char* mode, bool dataFile = true, bool showError = true){
		std::string realpath = path;
		if(dataFile) GetFullPath(realpath);

		FlatFile* file = new FlatFile();
		if(!file->Open(realpath.c_str(), mode)){
			if(showError) LOG("Error: FlatFileSystem could not open file: '%s' [%s]\n", path, mode);
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
		//mBaseDirectory.replace("\"", "");
		//mBaseDirectory.replace("/", "\\");
		//char end = mBaseDirectory.at(mBaseDirectory.length() - 1);
		//if(end != '\\') mBaseDirectory += "\\";
		LOG("FlatFileSystem Base Directory: '%s'\n", mBaseDirectory.c_str());
	}

	const char* GetBaseDirectory(){
		return mBaseDirectory.c_str();
	}

private:
	std::string mBaseDirectory;
};

#endif