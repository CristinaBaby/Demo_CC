//
//  EncryptDataHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "EncryptDataHelper.h"
#include "../AppGlobal.h"
#include "../utilities/STUserDefault.h"
#include "../utilities/base64.h"

using namespaceST::STUserDefault;
using std::string;
USING_NS_CC;

#define USERDEFAULT     UserDefault::getInstance()


int EncryptDataHelper::updateHighScore(const std::string &key, int scores)
{
    Value highScore = Value::Null;
    
    // first load value from file if exists.
    string  temphighScoreString = STLoadStringFromUD(key.c_str(), "0");
    if (temphighScoreString == "0")
    {
        // not exsit this value
        highScore = Value(temphighScoreString);
    }
    else
    {
        // decode from base64
        highScore = Value(base64_decode(temphighScoreString));
    }
    
    // have a new record
    if (scores > highScore.asInt())
    {
        highScore = Value(scores);
        CCLOG(" new record is %s", highScore.asString().c_str());
        
        // save the highest score to STUserDefault, and encode with base64
        string encoded = base64_encode(reinterpret_cast<const unsigned char*>(highScore.asString().c_str()),
                                       highScore.asString().length());
        
        STSaveStringToUD(key.c_str(), encoded);
    }
    
    return highScore.asInt();
}

bool EncryptDataHelper::getPurchaseFlag(const string &key)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    // because this flag no need to delete, so we store it in CCUserDefault
    return USERDEFAULT->getBoolForKey(encodeKey.c_str(), false);
}

void EncryptDataHelper::setPurchaseFlag(const std::string &key, bool value)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    USERDEFAULT->setBoolForKey(encodeKey.c_str(), value);
    USERDEFAULT->flush();
}

long EncryptDataHelper::getRateUsTimestamp(const std::string &key)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    string temp = USERDEFAULT->getStringForKey(encodeKey.c_str());
    
    string tv = base64_decode(temp);
    
    if (!tv.empty())
    {
        return atol(tv.c_str());
    }
    else
    {
        return 0;
    }
}

void EncryptDataHelper::setRateUsTimestamp(const string &key, long value)
{
    // check key
    if (key.empty())
        return;
    
    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%ld", value);
    
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    string encodeValue = base64_encode(reinterpret_cast<const unsigned char*>(tmp), strlen(tmp));
    
    USERDEFAULT->setStringForKey(encodeKey.c_str(), encodeValue);
    USERDEFAULT->flush();
}

void EncryptDataHelper::setPorpsQuantity(const std::string &key, unsigned quantity)
{
    // save the highest score to STUserDefault, and encode with base64
    string encodedKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    USERDEFAULT->setIntegerForKey(encodedKey.c_str(), quantity);
    USERDEFAULT->flush();
}

int EncryptDataHelper::getPorpsQuantity(const std::string &key, unsigned defVal)
{
    // encode the key value
    string encodedKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    // because this flag no need to delete, so we store it in CCUserDefault
    return USERDEFAULT->getIntegerForKey(encodedKey.c_str(), defVal);
}

bool EncryptDataHelper:: isLevelModeLocked(const string &key)
{
    string temp = STLoadStringFromUD(key.c_str(), "");
    return temp.empty();
}

void EncryptDataHelper:: unlockLevelMode(const std::string &key)
{
    // save the highest score to STUserDefault, and encode with base64
    string encoded = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()),
                                   key.length());
    
    STSaveStringToUD(key.c_str(), encoded);
}


Value EncryptDataHelper:: getValueFromST(const std::string &key, const Value &defVal)
{
    string temp = STLoadStringFromUD(key.c_str(), "");
    
    if (temp.empty()) {
        return defVal;
    }
    
    return Value( base64_decode(temp) );
}

void EncryptDataHelper:: setValeInST(const std::string &key, const Value &val)
{
    string value = val.asString();
    string encoded = base64_encode(reinterpret_cast<const unsigned char*>(value.c_str()),
                                   value.length());
    
    STSaveStringToUD(key.c_str(), encoded);
}




