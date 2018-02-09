//
//  CBColouringManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "CBColouringManager.h"
USING_NS_CC;
CB_BEGIN_NAMESPACE

static ColouringManager *s_SingletonColouringManager = nullptr;

ColouringManager* ColouringManager::getInstance()
{
    if (!s_SingletonColouringManager)
    {
        s_SingletonColouringManager = new (std::nothrow)(ColouringManager);
        CCASSERT(s_SingletonColouringManager, "ABORT: create PackManager failed");
    }
    
    return s_SingletonColouringManager;
}

ColouringManager::ColouringManager():
_pensType(PensType::PensType_None)
{
    
}

ColouringManager::~ColouringManager()
{
    
}

void ColouringManager::setupPens(const std::string& path, std::vector<Pen> *pens)
{
    ValueMap map = FileUtils::getInstance()->getValueMapFromFile(path);
    if (map.empty()) {
        return;
    }
    
    ValueVector items = map["Pens"].asValueVector();
    if (items.empty()) {
        return;
    }
    
    //clear old data
    pens->clear();
    
    //init new data
    int index = 0;
    for (auto itor = items.begin(); itor != items.end(); )
    {
        auto dic = itor->asValueMap();
        Pen pen;
        pen.penLogo = dic["Logo"].asString();
        pen.R = dic["R"].asInt();
        pen.G = dic["G"].asInt();
        pen.B = dic["B"].asInt();
        pen.A = dic["A"].asInt();
        pen.index = index++;
        
        pens->push_back(pen);
        ++itor;
    }
}

void ColouringManager::setupPens1(const std::string& path)
{
    setupPens(path, &_pens1);
}

void ColouringManager::setupPens2(const std::string& path)
{
    setupPens(path, &_pens2);
}

void ColouringManager::setupPatterns(const std::string& path)
{
    ValueMap map = FileUtils::getInstance()->getValueMapFromFile(path);
    if (map.empty()) {
        return;
    }
    
    ValueVector items = map["Pattern"].asValueVector();
    if (items.empty()) {
        return;
    }
    
    //clear old data
    _patterns.clear();
    
    //init new data
    int index = 0;
    for (auto itor = items.begin(); itor != items.end(); )
    {
        auto dic = itor->asValueMap();
        Pattern pattern;
        pattern.patternLogo = dic["Logo"].asString();
        pattern.pattern = dic["Pattern"].asString();
        pattern.index = index++;
        
        _patterns.push_back(pattern);
        ++itor;
    }
    
    //default is true
    _hasUnlockedPatternPen = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_PatternPenHasUnlocked, false);
}

void ColouringManager::unlockPatternPen()
{
    _hasUnlockedPatternPen = true;
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_PatternPenHasUnlocked, true);
    UserDefault::getInstance()->flush();
}


CB_END_NAMESPACE