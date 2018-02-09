#include "LYString.h"
#include "platform/CCFileUtils.h"
#include "base/ccMacros.h"
#include <stdlib.h>
#include <stdio.h>

NS_CC_MAX_BEGIN

#define kMaxStringLen (1024*100)

LYString::LYString()
:m_sString("")
{}

LYString::LYString(const char * str)
:m_sString(str)
{}

LYString::LYString(const std::string& str)
:m_sString(str)
{}

LYString::LYString(const LYString& str)
:m_sString(str.getCString())
{}

LYString::~LYString()
{
    m_sString.clear();
}

LYString& LYString::operator= (const LYString& other)
{
    m_sString = other.m_sString;
    return *this;
}

bool LYString::initWithFormatAndValist(const char* format, va_list ap)
{
    bool bRet = false;
    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString = pBuf;
        free(pBuf);
        bRet = true;
    }
    return bRet;
}

bool LYString::initWithFormat(const char* format, ...)
{
    bool bRet = false;
    m_sString.clear();
    
    va_list ap;
    va_start(ap, format);
    
    bRet = initWithFormatAndValist(format, ap);
    
    va_end(ap);
    
    return bRet;
}

LYString* LYString::substringFromIndex(unsigned int from) {
    if (from > m_sString.length()) {
        return LYString::create("");
    }
    return LYString::create(m_sString.substr(from, m_sString.length() - from));
}
LYString* LYString::substringToIndex(unsigned int to) {
    if (to > m_sString.length()) {
        return LYString::create(m_sString.c_str());
    }
    return LYString::create(m_sString.substr(0, to));
}
LYString* LYString::substringWithRange(CCRange range) {
    return LYString::create(m_sString.substr(range.location, range.length));
}

unsigned int LYString::findFirstOf(const char* str) {
    return m_sString.find_first_of(str);
}
unsigned int LYString::findLastOf(const char* str) {
    return m_sString.find_last_of(str);
}

CCArray* LYString::componentsSeparatedByString(const char* separator) {
    
    CCArray* array = CCArray::create();
    
    std::string str = m_sString;
    std::string sep = separator;
    
    while (true) {
        unsigned int index = str.find_first_of(sep.c_str());
        if (index >= str.length()) {
            std::string sub_str = str.substr(0, index);
            array->addObject(LYString::create(sub_str.c_str()));
            break;
        } else {
            std::string sub_str = str.substr(0, index);
            array->addObject(LYString::create(sub_str.c_str()));
            str = str.substr(index + sep.length(), str.length() - index);
        }
    }
    
    return array;
    
}

//LYString* LYString::uppercaseString() {
//    
//}
//LYString* LYString::lowercaseString() {
//    
//}
//LYString* LYString::capitalizedString() {
//    
//}

int LYString::intValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return atoi(m_sString.c_str());
}

unsigned int LYString::uintValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int)atoi(m_sString.c_str());
}

float LYString::floatValue() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return (float)atof(m_sString.c_str());
}

double LYString::doubleValue() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return atof(m_sString.c_str());
}

bool LYString::boolValue() const
{
    if (length() == 0)
    {
        return false;
    }
    
    if (0 == strcmp(m_sString.c_str(), "0") || 0 == strcmp(m_sString.c_str(), "false"))
    {
        return false;
    }
    return true;
}

const char* LYString::getCString() const
{
    return m_sString.c_str();
}

unsigned int LYString::length() const
{
    return m_sString.length();
}

int LYString::compare(const char * pStr) const
{
    return strcmp(getCString(), pStr);
}

Ref* LYString::copyWithZone(CCZone* pZone)
{
    CCAssert(pZone == NULL, "LYString should not be inherited.");
    LYString* pStr = new LYString(m_sString.c_str());
    return pStr;
}

bool LYString::isEqual(const Ref* pObject)
{
    bool bRet = false;
    const LYString* pStr = dynamic_cast<const LYString*>(pObject);
    if (pStr != NULL)
    {
        if (0 == m_sString.compare(pStr->m_sString))
        {
            bRet = true;
        }
    }
    return bRet;
}

LYString* LYString::create(const std::string& str)
{
    LYString* pRet = new LYString(str);
    pRet->autorelease();
    return pRet;
}

LYString* LYString::createWithData(const unsigned char* pData, unsigned long nLen)
{
    LYString* pRet = NULL;
    if (pData != NULL)
    {
        char* pStr = (char*)malloc(nLen+1);
        if (pStr != NULL)
        {
            pStr[nLen] = '\0';
            if (nLen > 0)
            {
                memcpy(pStr, pData, nLen);
            }
            
            pRet = LYString::create(pStr);
            free(pStr);
        }
    }
    return pRet;
}

LYString* LYString::createWithFormat(const char* format, ...)
{
    LYString* pRet = LYString::create("");
    va_list ap;
    va_start(ap, format);
    pRet->initWithFormatAndValist(format, ap);
    va_end(ap);
    
    return pRet;
}

LYString* LYString::createWithContentsOfFile(const char* pszFileName)
{
    ssize_t size = 0;
    unsigned char* pData = 0;
    LYString* pRet = NULL;
    pData = FileUtils::getInstance()->getFileData(pszFileName, "rb", &size);
    pRet = LYString::createWithData(pData, size);
    CC_SAFE_DELETE_ARRAY(pData);
    return pRet;
}

NS_CC_MAX_END
