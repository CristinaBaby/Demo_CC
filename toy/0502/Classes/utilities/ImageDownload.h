//
//  ImageDownload.h
//  puzzle
//
//  Created by liyang1 on 5/12/14.
//
//

#ifndef __puzzle__ImageDownload__
#define __puzzle__ImageDownload__

#include <iostream>
#include "cocos2d.h"
#include <network/HttpClient.h>
#include <network/HttpResponse.h>
#include <network/HttpRequest.h>

USING_NS_CC;
using namespace network;
//USING_NS_EXT;



class ImageDownloader : public cocos2d::Ref 
{
//    DECLARE_SINGLETON_CLASS(ImageDownloader);
public:
    ImageDownloader();
    ~ImageDownloader();
    
    std::string m_filename;
    CCString    observerID;
    Sprite*    container;
    bool        useMask;
//    const char* m_image_url;
//    const char* m_image_filename;
    
    void SendHttpRequest(const char* url, Sprite* layer, const char* filename);
    void HttpRequestComplete(HttpClient *sender, HttpResponse *response);
    
    
    void SendHttpRequestForFile(const char* url , const char* filename );
    void HttpRequestForFileComplete( HttpClient *sender, HttpResponse *response);
    
    static int sendCurlRequestFroFile(const char* url , const char* filename);
//    static long getDownloadFileLenth(const char *url);
    
    static int getProgressValue(const char* flag, double dt, double dn, double ult, double uln);
    static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata);
    
    void downloadControler(std::string url,std::string filename);
    
   static void* thread_funcation(void *arg);
    
    void saveUserDownloadTask(const char *url,std::string filename);
   
    void beginUserDownloadTask();

    
    void updateContainer(Sprite* layer);
};




#endif /* defined(__puzzle__ImageDownload__) */
