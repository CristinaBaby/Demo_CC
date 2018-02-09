//
//  MoreGameLoader.cpp
//  BBQParty
//
//  Created by liuwei on 14-8-1.
//
//

#include "MoreGameLoader.h"
#include "ExtensionFunc.h"
#include "libjson.h"
#include "JSONNode.h"

#define JSON_APP "apps"
#define JSON_ICON "icon"

MoreGameLoader::MoreGameLoader(){
}

MoreGameLoader::~MoreGameLoader(){
}

void MoreGameLoader::loadIcon(){
    ExtensionFunc sys;
    string url = sys.getMoregameUrl();
    
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
        
        NotificationCenter::getInstance()->postNotification(kMoreGameLoadSucceed, img);
        img->autorelease();
    }
}

string MoreGameLoader::getResultbyJsonKey(const char *jsonData, const char *IDS){
    string result = "";
    vector<string> icons;
    if (json_is_valid(jsonData)) {
        JSONNode* mainNode = (JSONNode*)json_parse(jsonData);
        if(!mainNode) return "";
        
        JSONNode::json_iterator itorMain = mainNode->begin();
        while(itorMain != mainNode->end())
        {
            JSONNode::json_iterator itorArray = (*itorMain)->begin();
            while (itorArray != (*itorMain)->end()){
                JSONNode::json_iterator itorLevel = (*itorArray)->begin();
                while(itorLevel != (*itorArray)->end())
                {
                    log("the json level %s", (*itorLevel)->name().c_str());
                    if (strcmp((*itorLevel)->name().c_str(), IDS) == 0) {
                        stringstream stream;
                        stream<<(*itorLevel)->as_string().c_str();
                        result = stream.str();
                        icons.push_back(result);
                        break;
                    }
                     ++itorLevel;
                }
                ++itorArray;
            }
            ++itorMain;
        }
        
        JSONNode::deleteJSONNode(mainNode);
        
        int num = 0;
        int count = icons.size();
        if (count > 1) {
            num = arc4random() % count;
        }
        return icons.at(num);
    }else {
        return "";
    }
}
