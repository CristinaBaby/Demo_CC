/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __LYSTRING_H__
#define __LYSTRING_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <stdarg.h>
#include <string>
#include <functional>
#include "cocos2d.h"


#include "AppGlobal.h"

NS_CC_MAX_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */
typedef struct _CCRange {
    unsigned int location;
    unsigned int length;
} CCRange;

class CC_DLL LYString : public Ref
{
public:
    LYString();
    LYString(const char* str);
    LYString(const std::string& str);
    LYString(const LYString& str);
    
    virtual ~LYString();
    
    /* override assignment operator */
    LYString& operator= (const LYString& other);
    
    /** init a string with format, it's similar with the c function 'sprintf' */
    bool initWithFormat(const char* format, ...);
    
    LYString* substringFromIndex(unsigned int from);
    LYString* substringToIndex(unsigned int to);
    LYString* substringWithRange(CCRange range);    // Hint: Use with rangeOfComposedCharacterSequencesForRange: to avoid breaking up composed characters
    
    unsigned int findFirstOf(const char* str);
    unsigned int findLastOf(const char* str);
    
    CCArray* componentsSeparatedByString(const char* separator);
    
    /* The following three case methods perform the canonical (non-localized) mappings. They are suitable for programming operations that require stable results not depending on the user's locale preference.  For localized case mapping for strings presented to users, use their corresponding methods with locale argument below.
     */
//    LYString* uppercaseString();
//    LYString* lowercaseString();
//    LYString* capitalizedString();
    
    /** convert to int value */
    int intValue() const;
    
    /** convert to unsigned int value */
    unsigned int uintValue() const;
    
    /** convert to float value */
    float floatValue() const;
    
    /** convert to double value */
    double doubleValue() const;
    
    /** convert to bool value */
    bool boolValue() const;
    
    /** get the C string */
    const char* getCString() const;
    
    /** get the length of string */
    unsigned int length() const;
    
    /** compare to a c string */
    int compare(const char *) const;
    
    /* override functions */
    virtual Ref* copyWithZone(CCZone* pZone);
    virtual bool isEqual(const Ref* pObject);
    
    /** create a string with std string, you can also pass a c string pointer because the default constructor of std::string can access a c string pointer.
     *  @return A LYString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static LYString* create(const std::string& str);
    
    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in LYString.cpp file.
     *  @return A LYString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static LYString* createWithFormat(const char* format, ...);
    
    /** create a string with binary data
     *  @return A LYString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static LYString* createWithData(const unsigned char* pData, unsigned long nLen);
    
    /** create a string with a file,
     *  @return A LYString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static LYString* createWithContentsOfFile(const char* pszFileName);
    
private:
    
    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap);
    
public:
    std::string m_sString;
};

//struct LYStringCompare : public std::binary_function<LYString *, LYString *, bool> {
//public:
//    bool operator() (LYString * a, LYString * b) const {
//        return strcmp(a->getCString(), b->getCString()) < 0;
//    }
//};

#define LYStringMake(str) LYString::create(str)
#define LYs               LYStringMake

// end of data_structure group
/// @}

NS_CC_MAX_END

#endif //__LYString_H__
