/*
    使用说明
 本地化支持的语言需用plist/xml文件配置（如localization.plist） 用void setup(std::string type,std::string languagePath);加载该配置文件
 加载本地化语言的字符串文件 void loadFile(std::string type,const std::string filePath,...);
 */
#ifndef __LocalizationHelp__
#define __LocalizationHelp__


#include "SingleTon.h"
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

#define LocalizationString(str) LocalizationHelp::getInstance()->getString(str)


class LocalizationHelp : public SingleTon<LocalizationHelp>
{
public:
    LocalizationHelp();
    ~LocalizationHelp();
    
//    加载要本地化语言的的plist/xml文件路径 type（xml/plist） languagePath（路径名前缀）
    void setup(std::string type,std::string languagePath);
    void checkLanguage();
    
//    加载要本地化的plist/xml文件路径
    void loadFile(std::string type,const std::string filePath,...);
    
//    获取关键字对应的键值
    std::string getString(const std::string name);
    
private:
    void _loadFile(std::string type,const std::string filePath,ValueMap* map);
    void _setLanguageCode();
protected:
    std::vector<std::string> m_filePath;
    ValueMap m_Map;
    ValueMap m_languageMap;
    
//    语言代号后缀
    std::string m_sSXLanguage;
    
    LanguageType m_eLanguage;
};

#endif