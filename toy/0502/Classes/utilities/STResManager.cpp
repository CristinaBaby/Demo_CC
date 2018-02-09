//
//  STResManager.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#include "STResManager.h"
#include "../AppGlobal.h"
#include "STMultiScreenHelper.h"

USING_NS_ST;
USING_NS_CC;
using std::vector;
using std::string;

static STResManager* s_p_STResManager = NULL;

STResManager* STResManager::getInstance()
{
    if (!s_p_STResManager)
    {
        s_p_STResManager = new STResManager();
    }
    
    return s_p_STResManager;
}

void STResManager::destroy()
{
    if (s_p_STResManager)
    {
        delete s_p_STResManager;
        s_p_STResManager = NULL;
    }
}

vector<string> STResManager::getFileSearchPath()
{
    vector<string> searchpath;
    string languageSuffix = getCurrentLanguageSuffix();
    if (!languageSuffix.empty())
    {
        languageSuffix = "/ui-" + languageSuffix;
    }
    
    searchpath.push_back("ui");
    searchpath.push_back("jigsaw");
    searchpath.push_back("fish");
    searchpath.push_back("game");
    searchpath.push_back("colorbook");
    searchpath.push_back("spot");
    searchpath.push_back("fonts");
    searchpath.push_back("colorbook/pattern");
    searchpath.push_back("disclaimer");
    searchpath.push_back("reading");
    searchpath.push_back("sounds");
    searchpath.push_back("ui/ui_adsloading");
    
    return searchpath;
}

const char* STResManager::getCurrentLanguageSuffix()
{
    switch (Application::getInstance()->getCurrentLanguage())
    {
        case LanguageType::CHINESE:
            return "zh";
        case LanguageType::FRENCH:
            return "fr";
        case LanguageType::ITALIAN:
            return "it";
        case LanguageType::GERMAN:
            return "de";
        case LanguageType::SPANISH:
            return "es";
        case LanguageType::RUSSIAN:
            return "ru";
        case LanguageType::KOREAN:
            return "ko";
        case LanguageType::JAPANESE:
            return "ja";
        case LanguageType::HUNGARIAN:
            return "hu";
        case LanguageType::PORTUGUESE:
            return "pt";
        case LanguageType::ARABIC:
            return "ar";
        default:
            // English
            return "en";
    }
}





