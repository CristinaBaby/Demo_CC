//
//  MoreGameLoader.cpp
//  BBQParty
//
//  Created by liuwei on 14-8-1.
//
//

#include "MoreGameLoader.h"
#include "MoreGameUrl.h"
#include  "cocostudio/CocoStudio.h"

#define JSON_APP "apps"
#define JSON_ICON "icon"

MoreGameLoader::MoreGameLoader(){
}

MoreGameLoader::~MoreGameLoader(){
}

void MoreGameLoader::loadIcon(){
    MoreGameUrl _more;
    string url = _more.getMoreGameUrl();
    
    log("url :: %s", url.c_str());
    
    HttpClient* httpClient = HttpClient::getInstance();
    HttpRequest* request = new HttpRequest();
    request->setTag("Get_MoreGame");
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(MoreGameLoader::httpRequestFinish, this));
    
    httpClient->setTimeoutForConnect(30);
    httpClient->send(request);
    request->release();
    
}

void MoreGameLoader::httpRequestFinish(HttpClient* client, HttpResponse* response) {
    
    if(!response->isSucceed())
    {
        log("error is here %s",response->getErrorBuffer());
        return;
    }
    
    const char* tag = response->getHttpRequest()->getTag();
    if (0 == strcmp(tag, "Get_MoreGame")) {
        std::vector<char> *data = response->getResponseData();
        int vectorLength = data->size();
        if(vectorLength <= 0)
        {
            log("ResponseData is null");
            return;
        }
        
        std::string recieveData;
        for(int i=0; i<vectorLength; i++)
        {
            recieveData += (*data)[i];
        }
        recieveData += '\0';
        
        log("recieve data is %s",recieveData.c_str());
        string iconUrl = getResultbyJsonKey(recieveData.c_str(), JSON_ICON);
     
        if (iconUrl.length() > 0) {
            log("icon url is %s", iconUrl.c_str());
            HttpClient* currentClient = HttpClient::getInstance();
            
            HttpRequest* currentRequest = new HttpRequest();
            currentRequest->setTag("Get_CCImage");
            currentRequest->setUrl(iconUrl.c_str());
            currentRequest->setResponseCallback(CC_CALLBACK_2(MoreGameLoader::httpRequestFinish, this));
            currentRequest->setRequestType(HttpRequest::Type::GET);
            
            currentClient->setTimeoutForConnect(30);
            currentClient->send(currentRequest);
            currentRequest->release();
            
        }
        
    }
    if (0 == strcmp(tag, "Get_CCImage")) {
        std::vector<char>*buffer = response->getResponseData();
        int vectorLength = buffer->size();
        if(vectorLength <= 0)
        {
            log("ResponseData is null");
            return;
        }
        
        Image * img=new Image();
        unsigned char tempSS[buffer->size()];
        for(unsigned int i=0;i < buffer->size();i++){
            tempSS[i] = (unsigned char)buffer->at(i);
        }
        img->initWithImageData(tempSS, buffer->size());
        
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kMoreGameLoadSucceed,img);
        img->autorelease();
    }
}

string MoreGameLoader::getResultbyJsonKey(const char *jsonData, const char *IDS){
    rapidjson::Document _doc;
    string result = "";
    vector<string> icons;
    do {
        CC_BREAK_IF(jsonData == nullptr || strcmp(jsonData, "") == 0);
        std::string loadString(jsonData);
       // CC_SAFE_DELETE_ARRAY(jsonData);
        _doc.Parse<0>(loadString.c_str());
        CC_BREAK_IF(_doc.HasParseError()||!_doc.IsObject() || !_doc.HasMember(JSON_APP));
        const rapidjson::Value &apps = _doc[JSON_APP];
        CC_BREAK_IF(!apps.IsArray()||apps.Size()==0);
        const rapidjson::Value & _iconValue = apps[rand()%apps.Size()];
        CC_BREAK_IF(!_iconValue.HasMember(JSON_ICON));
        result = _iconValue[JSON_ICON].GetString();
    }while (0);
    return result;
}
