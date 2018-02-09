
#include "FileUtility.h"
#include "CFSystemFunction.h" //这个即将被SSC整理，头文件包含方式暂时不变

bool FileUtility::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mode_t processMask = umask(0);
	int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (ret != 0 && (errno != EEXIST))
	{
		return false;
	}

	return true;
#else
	BOOL ret = CreateDirectoryA(path, nullptr);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
	return true;
#endif
}

std::string FileUtility::getFileDirectory(const std::string& fileName)
{
	if(fileName.size() < 1) return fileName;

	long index = fileName.find_last_of("/");
	if(index > 1 && index <= fileName.size())
	{
		return fileName.substr(0, index);
	}
	else
	{
		return fileName;
	}
}

bool FileUtility::isFileExist(const std::string& filePath)
{
	if(filePath.size() < 1) return false;

	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

bool FileUtility::createFile(const std::string& filePath)
{
	if(filePath.size() < 1) return false;
	FILE* fp = fopen(filePath.c_str(), "w");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

std::string FileUtility::getStoragePath()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return FileUtils::getInstance()->getWritablePath();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CFSystemFunction sys;
	return sys.getSDCardPath();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return FileUtils::getInstance()->getWritablePath();
#endif
}

bool FileUtility::removeDir(const std::string& path )
{
	#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *dir;
	dirent *dir_info;
	char file_path[PATH_MAX];
	if(isFile(path.c_str()))
	{
		remove(path.c_str());
		return true;
	}
	if(isDir(path.c_str()))
	{
		if((dir = opendir(path.c_str())) == nullptr)
			return false;
		while((dir_info = readdir(dir)) != nullptr)
		{
			getFilePath(path.c_str(), dir_info->d_name, file_path);
			if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
				continue;
			if(isDir(file_path))
			{
				removeDir(file_path);
				rmdir(file_path);
			}
			else if(isFile(file_path))
				remove(file_path);

		}
	}
	#else
		
	
	#endif
	return true;
}

void  FileUtility::getFilePath(const char *path, const char *file_name,  char *file_path)
{
	strcpy(file_path, path);
	if(file_path[strlen(path) - 1] != '/')
		strcat(file_path, "/");
	strcat(file_path, file_name);
}

//
bool FileUtility::isDir(const char *path)
{
	#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)//
	{
		return S_ISDIR(statbuf.st_mode) != 0;//
	}
	return false;
#else

	return false;
#endif
	
}

//
bool FileUtility::isFile(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)
		return S_ISREG(statbuf.st_mode) != 0;//
	return false;
#else

	return false;
#endif
}

int FileUtility::getImgFileCount(const string& path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    int fileCount = 0;
    string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    if(FileUtility::isDir(fullPath.c_str()))
    {
        if((dir = opendir(fullPath.c_str())) == nullptr)
        {
            log("invalid path");
            return 0;
        }else
        {
            while((dir_info = readdir(dir)) != nullptr)
            {
                FileUtility::getFilePath(fullPath.c_str(), dir_info->d_name, file_path);
                if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
                    continue;
                if(FileUtility::isDir(file_path))
                {
                    //不做处理
                    continue;
                }else if(FileUtility::isFile(file_path))
                {
                    string filePath(file_path);
                    if (filePath.substr(filePath.size() - 4) == ".png" ||
                        filePath.substr(filePath.size() - 4) == ".jpg")
                    {
                        fileCount++;
                    }
                }
            }
        }
    }
    return fileCount;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    STSystemFunction sys;
//    return sys.listAssetFiles(path);
#else
    return 0;
#endif
}

