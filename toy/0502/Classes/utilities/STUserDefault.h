//
//  STUserDefault.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-29.
//
//

#ifndef __MysteryGuardians__STUserDefault__
#define __MysteryGuardians__STUserDefault__

#include "STXML.h"
#include "STMacro.h"

// help to user UserDefault
#define STSaveStringToUD                        namespaceST::STUserDefault::getInstance()->setStringForKey
#define STSaveIntegerToUD                       namespaceST::STUserDefault::getInstance()->setIntegerForKey
#define STSaveBooleanToUD                       namespaceST::STUserDefault::getInstance()->setBoolForKey
#define STSaveFloatToUD                         namespaceST::STUserDefault::getInstance()->setFloatForKey
#define STLoadStringFromUD                      namespaceST::STUserDefault::getInstance()->getStringForKey
#define STLoadIntegerFromUD                     namespaceST::STUserDefault::getInstance()->getIntegerForKey
#define STLoadBooleanFromUD                     namespaceST::STUserDefault::getInstance()->getBoolForKey
#define STLoadFloatFromUD                       namespaceST::STUserDefault::getInstance()->getFloatForKey

NS_ST_BEGIN

class STUserDefault : protected STXML
{
public:
    ~STUserDefault();
    
    // get value methods
    
    /**
     @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
     */
    bool    getBoolForKey(const char* pKey, bool defaultValue = false);
    /**
     @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
     */
    int     getIntegerForKey(const char* pKey, int defaultValue = 0);
    /**
     @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
     */
    float    getFloatForKey(const char* pKey, float defaultValue = 0.0f);
    /**
     @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
     */
    double  getDoubleForKey(const char* pKey, double defaultValue = 0.0);
    /**
     @brief Get string value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is "".
     */
    std::string getStringForKey(const char* pKey, const std::string & defaultValue = "");
    
    // set value methods
    
    /**
     @brief Set bool value by key.
     */
    void    setBoolForKey(const char* pKey, bool value);
    /**
     @brief Set integer value by key.
     */
    void    setIntegerForKey(const char* pKey, int value);
    /**
     @brief Set float value by key.
     */
    void    setFloatForKey(const char* pKey, float value);
    /**
     @brief Set double value by key.
     */
    void    setDoubleForKey(const char* pKey, double value);
    /**
     @brief Set string value by key.
     */
    void    setStringForKey(const char* pKey, const std::string & value);
    /**
     @brief reset xml file contents
     */
    void    reset();
    
    static STUserDefault* getInstance();
    static void destroy();
    const static std::string& getXMLFilePath();
    static bool isXMLFileExist();
    
private:
    STUserDefault();
    static bool createXMLFile();
    static void initXMLFilePath();
    
    static STUserDefault* m_spUserDefault;
    static std::string m_sFilePath;
    static bool m_sbIsFilePathInitialized;
};

NS_ST_END

#endif /* defined(__MysteryGuardians__STUserDefault__) */
