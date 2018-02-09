//
//  MoreGameLoader.h
//  BBQParty
//
//  Created by liuwei on 14-8-1.
//
//

#ifndef __BBQParty__MoreGameLoader__
#define __BBQParty__MoreGameLoader__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <string>
#include "network/HttpClient.h"
#include "SingleTon.h"
#define kMoreGameLoadSucceed "kMoreGameLoadSucceed"

using namespace cocos2d::network;

using namespace cocos2d;
USING_NS_CC_EXT;
using namespace std;


class MoreGameLoader : public SingleTon<MoreGameLoader>{
    friend class SingleTon<MoreGameLoader>;
public:
    MoreGameLoader();
    ~MoreGameLoader();
    
    void loadIcon();
    
    void httpRequestFinish(HttpClient* client, HttpResponse* response);
    
    string getResultbyJsonKey(const char* jsonData, const char* IDS);
};

#endif /* defined(__BBQParty__MoreGameLoader__) */
