//
//  MyZipUtils.cpp
//  puzzle
//
//  Created by liyang1 on 5/15/14.
//
//

#include "MyZipUtils.h"

//ios back up需要
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
#include <sys/xattr.h>

#endif

//创建文件需要
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#endif
//using namespace cocos2d;
//using namespace std;

USING_NS_CC;
MyZipUtils::MyZipUtils(){};

MyZipUtils::~MyZipUtils(){};

//bool MyZipUtils::uncompress(string _storagePath , string TEMP_PACKAGE_FILE_NAME , string unCompressDirect)
//{
//    // Open the zip file
//    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
////    log("file path = %s" , outFileName.c_str());
//    
//    
//    unzFile zipfile = unzOpen(outFileName.c_str());
//    if (! zipfile)
//    {
//        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
//        return false;
//    }
//    
//    // Get info about the zip file
//    unz_global_info global_info;
//    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
//    {
//        CCLOG("can not read file global info of %s", outFileName.c_str());
//        unzClose(zipfile);
//        return false;
//    }
//    
//    // Buffer to hold data read from the zip file
//    char readBuffer[BUFFER_SIZE];
//    
//    CCLOG("start uncompressing  file number = %ld" ,global_info.number_entry);
//    
//    // Loop to extract all files.
//    uLong i;
//    for (i = 0; i < global_info.number_entry; ++i)
//    {
//        // Get info about current file.
//        unz_file_info fileInfo;
//        char fileName[MAX_FILENAME];
//        if (unzGetCurrentFileInfo(zipfile,
//                                  &fileInfo,
//                                  fileName,
//                                  MAX_FILENAME,
//                                  NULL,
//                                  0,
//                                  NULL,
//                                  0) != UNZ_OK)
//        {
//            CCLOG("can not read file info");
//            unzClose(zipfile);
//            return false;
//        }
//        
//
//        string fullPath = unCompressDirect + fileName;
////        CCLOG("fullPath = %s" ,fullPath.c_str());
//            // Entry is a file, so extract it.
//        const size_t filenameLength = strlen(fileName);
//        if (fileName[filenameLength-1] == '/')
//        {
//            // Entry is a direcotry, so create it.
//            // If the directory exists, it will failed scilently.
//            if (!createDirectory(fullPath.c_str()))
//            {
//                CCLOG("can not create directory %s", fullPath.c_str());
//                unzClose(zipfile);
//                return false;
//            }
//        }
//        else
//        {
//            // Open current file.
//
//        if (unzOpenCurrentFile(zipfile) != UNZ_OK)
//            {
//                CCLOG("can not open file %s", fileName);
//                unzClose(zipfile);
//                return false;
//            }
//            
//            // Create a file to store current file.
//            FILE *out = fopen(fullPath.c_str(), "wb");
//            if (! out)
//            {
//                CCLOG("can not open destination file %s", fullPath.c_str());
//                unzCloseCurrentFile(zipfile);
//                unzClose(zipfile);
//                return false;
//            }
//            
//            // Write current file content to destinate file.
//            int error = UNZ_OK;
//            do
//            {
//                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
//                if (error < 0)
//                {
//                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
//                    unzCloseCurrentFile(zipfile);
//                    unzClose(zipfile);
//                    return false;
//                }
//                
//                if (error > 0)
//                {
//                    fwrite(readBuffer, error, 1, out);
//                }
//            } while(error > 0);
//            
//            fclose(out);
//            
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
//            const char* attrName = "com.apple.MobileBackup";
//            u_int8_t attrValue = 1;
//            setxattr(fullPath.c_str(), attrName, &attrValue, sizeof(attrValue), 0, 0);
//     
//#endif
//            
//           
//
////            sssd
//        }
//        
//        unzCloseCurrentFile(zipfile);
//        
//        // Goto next entry listed in the zip file.
//        if ((i+1) < global_info.number_entry)
//        {
//            if (unzGoToNextFile(zipfile) != UNZ_OK)
//            {
//                CCLOG("can not read next file");
//                unzClose(zipfile);
//                return false;
//            }
//        }
//    }
//    
//    remove(outFileName.c_str());//删除zip文件
//    
//    
//};

bool MyZipUtils::createDirectory(const char *path)
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
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}