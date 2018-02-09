//
//  STResManager.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__STResManager__
#define __LVUP002__STResManager__

#include "STMacro.h"
#include "cocos2d.h"

NS_ST_BEGIN

class STResManager
{
public:
    static STResManager* getInstance();
    static void destroy();
    
    STResManager(){}
    virtual ~STResManager(){}
    
public:
    /**
     * create a search path by deveice's language and resolution
     */
    std::vector<std::string> getFileSearchPath();
    
    /**
     * get codes for the representation of names of device's language
     */
    const char* getCurrentLanguageSuffix();
};

NS_ST_END

#endif /* defined(__LVUP002__STResManager__) */
