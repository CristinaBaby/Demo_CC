 //
//  ImageDownload.cpp
//  puzzle
//
//  Created by liyang1 on 5/12/14.
//
//

#include "ImageDownload.h"
#include "curl/curl.h"
#include "Utils.h"
#include "MyZipUtils.h"
#include "../AppGlobal.h"
#include "extensions/ExtensionMacros.h"
#include "../modules/STSystemFunction.h"
#include "../gameconfig/GameConfig.h"
#include <chrono>
#include <thread>

using std::map;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
#include <sys/xattr.h>
#endif

ImageDownloader::ImageDownloader(){}

ImageDownloader::~ImageDownloader(){};

int request_tag;
void ImageDownloader::SendHttpRequest(const char* url, Sprite* layer, const char* filename)
{
    request_tag = GameConfig::sharedGameConfig()->current_request_tag;
    log("URL=%s",url);
    HttpRequest* request = new HttpRequest();
    this->container = layer;
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(ImageDownloader::HttpRequestComplete));
    request->setTag(filename);
    HttpClient::getInstance()->send(request);
    request->release();
}

void ImageDownloader::updateContainer(Sprite* layer)
{
    this->container = layer;

}


//下载完成, 刷新需要显示的图片
void ImageDownloader::HttpRequestComplete(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        //((ShowSprite*)container)->refreshNetSprite(false);
//         log("response failed   获取网络图片失败");
        return;
    }

    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
//        log("%s completed", response->getHttpRequest()->getTag());
    }
    
//    int statusCode = response->getResponseCode();
//    char statusString[64] = {};
//    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
//    log("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
//        log("response failed   获取网络图片失败");
//        log("error buffer: %s", response->getErrorBuffer());
        //((ShowSprite*)container)->refreshNetSprite(false);
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string path = GET_PLATFORM_SAVE_PATH;
    std::string bufffff(buffer->begin(),buffer->end());

    //保存到本地文件
    path.append(response->getHttpRequest()->getTag());
//    CCLOG("path: %s",path.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    FILE *fp1 = fopen(path.c_str(), "wb+");
    fclose(fp1);
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(path.c_str(), attrName, &attrValue, sizeof(attrValue), 0, 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android

#endif
   
    
    

    
    FILE *fp = fopen(path.c_str(), "wb+");
    fwrite(bufffff.c_str(), 1,buffer->size(), fp);
    fclose(fp);
    
    GameConfig::sharedGameConfig()->map_more_pictrue_download.erase(response->getHttpRequest()->getTag());
}

void ImageDownloader::SendHttpRequestForFile(const char *url, const char *filename)
{
    log("url=%s" , url);
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(ImageDownloader::HttpRequestForFileComplete));
    request->setTag(filename);
    HttpClient::getInstance()->send(request);
    request->release();
}


void ImageDownloader::HttpRequestForFileComplete(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
//        log("response failed   获取文件失败");
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
//        log("%s completed", response->getHttpRequest()->getTag());
    }

    if (!response->isSucceed())
    {
//        log("response failed   获取文件失败");
//        log("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string path = GET_PLATFORM_SAVE_PATH;
    std::string bufffff(buffer->begin(),buffer->end());
    
    if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
        MyZipUtils *zipUtil = new MyZipUtils;
        zipUtil-> createDirectory(path.c_str());
    }
    //保存到本地文件
    path.append(response->getHttpRequest()->getTag());
    CCLOG("path: %s",path.c_str());
    FILE *fp = fopen(path.c_str(), "wb+");
    fwrite(bufffff.c_str(), 1,buffer->size(), fp);
    fclose(fp);
    
}

long downloadFileLenth = 0;
double localFileLenth = 0;

int ImageDownloader::sendCurlRequestFroFile(const char *url, const char *filename)
{
    int res = 0;
//    CCLOG("url=%s  filename=%s" ,url,filename);
    CURL *handler = curl_easy_init();
    curl_easy_setopt(handler, CURLOPT_URL, url);
    
    
    std::string path;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    STSystemFunction *function = new STSystemFunction;
    path = function->getPlatformTempPath().append(filename);
    log("path=%s" , path.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    path = GameConfig::sharedGameConfig()->getPlatformWritePath().append(filename);
#endif
    
    FILE *fp = fopen(path.c_str(), "wb"); //创建方式打开
    
    curl_easy_setopt(handler, CURLOPT_TIMEOUT, 500);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, downLoadPackage);   //写文件回调方法
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(handler, CURLOPT_NOPROGRESS, 0); //表示从第0个字节开始下载
    curl_easy_setopt(handler, CURLOPT_PROGRESSFUNCTION, getProgressValue );  //设置回调的进度函数
    curl_easy_setopt(handler, CURLOPT_PROGRESSDATA, filename);

    res = curl_easy_perform(handler);
    
//    curl_easy_cleanup(handler);
    fclose(fp);
//    CCLOG("res=%d" , res);
    if (res!=0) {
//        CCLOG("failed");
    } else {
//        CCLOG("success");
    }
    return res;
    
}
string m_url ;

//控制方法
void ImageDownloader::downloadControler(string url,std::string filename)
{
    pthread_t th_socket;
//    log(" downloadControler getpid=%d  filename=%s" ,getpid() , filename.c_str());
    m_url = url;
    m_filename = filename;
    int errCode=0;
    do {
        pthread_attr_t tAttr;
        errCode=pthread_attr_init(&tAttr);
        CC_BREAK_IF(errCode!=0);
        errCode=pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
        if(errCode!=0)
        {
            pthread_attr_destroy(&tAttr);
            break;
        }
       errCode = pthread_create(&th_socket, &tAttr, thread_funcation, &m_filename);
    
        
    } while (0);
    
}


void*  ImageDownloader::thread_funcation(void *arg)
{
    
    string chuan_filename = *(string*)arg;
    int count = 0;
    bool  ret = false;
    int DOWNLOAD_TIMES = 1;
    while (count++ < DOWNLOAD_TIMES){
        ret = sendCurlRequestFroFile(m_url.c_str(),chuan_filename.c_str()); //直接下载
        if (ret == 0){ //下载完成
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
};


void ImageDownloader::saveUserDownloadTask(const char *url, std::string filename)
{
    m_url = url;
    m_filename = filename.c_str();
}

void ImageDownloader::beginUserDownloadTask()
{
    downloadControler(m_url, m_filename);
}


//long ImageDownloader::getDownloadFileLenth(const char *url){
//	long fileLenth = 0;
//    m_url = url;
//	CURL *handle = curl_easy_init();
//	curl_easy_setopt(handle, CURLOPT_URL, url);
//	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头
//	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body
//    
//	if (curl_easy_perform(handle) == CURLE_OK) {
//		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileLenth);
//	} else {
//		fileLenth = -1;
//	}
//    CCLOG("fileLenth=%ld",fileLenth);
//	return fileLenth;
//}


int ImageDownloader::getProgressValue(const char* flag, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded){
    int download_percent =(int)(nowDownloaded / totalToDownload *100);
    map<std::string, int>::iterator mapData = GameConfig::sharedGameConfig()->map_package_download.find(flag);
    (*mapData).second = download_percent;
//    CCLOG("进度等于%d" , download_percent);
    return 0;
};

size_t ImageDownloader::downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

