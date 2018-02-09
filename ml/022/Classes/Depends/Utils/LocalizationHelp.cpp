
#include "LocalizationHelp.h"
#include "tinyxml2/tinyxml2.h"
#include "StringHelp.h"

using namespace tinyxml2;


LocalizationHelp::LocalizationHelp()
{
    
}

LocalizationHelp::~LocalizationHelp()
{
    
}
void LocalizationHelp::setup(std::string type,std::string languagePath)
{
    _loadFile(type,languagePath,&m_languageMap);
    m_eLanguage = Application::getInstance()->getCurrentLanguage();
    _setLanguageCode();
}

void LocalizationHelp::checkLanguage()
{
    LanguageType languageType = Application::getInstance()->getCurrentLanguage();
    std::string languageCode = Application::getInstance()->getCurrentLanguageCode();
    if (languageType==m_eLanguage) {
        return;
    }
    auto it = m_languageMap.find(languageCode);
    if (it == m_Map.end()) {
        return;
    }
//    不支持本地话当前语言，设置为默认语言（默认语言为第一个）
    if (!it->second.asBool()) {
        m_eLanguage = LanguageType::ENGLISH;
        _setLanguageCode();
    }else{
        m_eLanguage = Application::getInstance()->getCurrentLanguage();
        _setLanguageCode();
    }

    std::vector<std::string>::iterator iter;
    
    for_each(m_filePath.begin(),m_filePath.end(),[&](std::string pathStr){
        std::vector<std::string> strVector = StringHelp::split(pathStr,".");
        std::string type = strVector.at(1);
        std::string path = strVector.at(0)+"_"+ m_sSXLanguage;
        _loadFile(type,path,&m_Map);
    });

}

void LocalizationHelp::loadFile(std::string type,const std::string filePath, ...)
{
    _loadFile(type,filePath+"_"+m_sSXLanguage,&m_Map);
    m_filePath.push_back(std::string(filePath).append(".").append(type));
    va_list args;
    va_start(args,filePath);
    while (1) {
        const char* path = va_arg(args, char*);
        if (!path) {
            break;
        }
        m_filePath.push_back(std::string(path).append(".").append(type));
        _loadFile(type,std::string(path)+"_"+m_sSXLanguage,&m_Map);
    }
    va_end(args);
}

std::string LocalizationHelp::getString(const std::string name)
{
    for (auto it = m_Map.begin(); it != m_Map.end();++it)
    {
        log("key is %s,value is %s",it->first.c_str(),it->second.asString().c_str());
    }
    auto it = m_Map.find(name);
    if (it == m_Map.end()) {
        log("LocalizationHelp: %s has not be localized",name.c_str());
        return name;
    }
    return it->second.asString();
}


#pragma mark=======private function=====
void LocalizationHelp::_loadFile(std::string type,const std::string filePath,ValueMap* map)
{
    FileUtils *fu= FileUtils::getInstance();
    std::string path = fu->fullPathForFilename(std::string(filePath).append(".").append(type));
    log("=========%s",path.c_str());
    ValueMap vm;
    if (std::strcmp(type.c_str(), "xml")==0) {
        std::string fullPath = FileUtils::getInstance()->getWritablePath() + path;
        
        XMLDocument *pDoc = new XMLDocument();
        XMLError errorId = pDoc->LoadFile(path.c_str());
        
        if (errorId != 0) {
            //xml格式错误
            return;
        }
        //得到跟节点
        XMLElement *rootEle = pDoc->RootElement();
        
        //跟节点的第一个字节点 dict
        XMLElement *dictEle = rootEle->FirstChildElement();
        //dict下面的子节点 key
        XMLElement *keyEle = dictEle->FirstChildElement();
        
        for (auto e=keyEle; e; e=e->NextSiblingElement()){
            //打印key节点的值
            log("%s,%s",e->Name(),e->GetText());
            
            e = e->NextSiblingElement();
            log("%s,%s",e->Name(),e->GetText());
            
            //string节点的子节点
            XMLElement *nulXmlEle = e->FirstChildElement();
            if (NULL == nulXmlEle) {
                log("string下面没有子点了");
            }
        }
    }else{
        vm = fu->getValueMapFromFile(path);//dictionary,如果是vector,使用fu->getValueVectorFromFile(const std::string &filename)
    }
    for (auto it = vm.begin(); it != vm.end();++it)
    {
        map->insert(ValueMap::value_type(it->first,Value(it->second.asString())));
    }
}

void LocalizationHelp::_setLanguageCode()
{
    switch (m_eLanguage) {
        case LanguageType::CHINESE:
            m_sSXLanguage = "zh";
            break;
        case LanguageType::FRENCH:
            m_sSXLanguage = "fr";
            break;
        case LanguageType::ITALIAN:
            m_sSXLanguage = "it";
            break;
        case LanguageType::GERMAN:
            m_sSXLanguage = "de";
            break;
        case LanguageType::SPANISH:
            m_sSXLanguage = "es";
            break;
        case LanguageType::DUTCH:
            m_sSXLanguage = "nl";
            break;
        case LanguageType::RUSSIAN:
            m_sSXLanguage = "ru";
            break;
        case LanguageType::KOREAN:
            m_sSXLanguage = "ko";
            break;
        case LanguageType::JAPANESE:
            m_sSXLanguage = "ja";
            break;
        case LanguageType::HUNGARIAN:
            m_sSXLanguage = "hu";
            break;
        case LanguageType::PORTUGUESE:
            m_sSXLanguage = "pt";
            break;
        case LanguageType::ARABIC:
            m_sSXLanguage = "ar";
            break;
        case LanguageType::NORWEGIAN:
            m_sSXLanguage = "nb";
            break;
        case LanguageType::POLISH:
            m_sSXLanguage = "pl";
            break;
        default:
            m_sSXLanguage = "en";
            break;
    }
}