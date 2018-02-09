//
//  STUserDefault.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-29.
//
//

#include "STUserDefault.h"


// root name of xml
#define USERDEFAULT_ROOT_NAME    "STUserDefaultRoot"

#define XML_FILE_NAME "STUserDefault.xml"

using namespace std;
USING_NS_CC;
USING_NS_ST;

/**
 * implements of STUserDefault
 */

STUserDefault* STUserDefault::m_spUserDefault = 0;
string STUserDefault::m_sFilePath = string("");
bool STUserDefault::m_sbIsFilePathInitialized = false;

/**
 * If the user invoke delete STUserDefault::sharedUserDefault(), should set m_spUserDefault
 * to null to avoid error when he invoke STUserDefault::sharedUserDefault() later.
 */
STUserDefault::~STUserDefault()
{
	CC_SAFE_DELETE(m_spUserDefault);
    //    m_spUserDefault = NULL;
}

STUserDefault::STUserDefault()
{
	m_spUserDefault = NULL;
}

void STUserDefault::destroy()
{
    m_spUserDefault = NULL;
}

bool STUserDefault::getBoolForKey(const char* pKey, bool defaultValue/* = false */)
{
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node = STXML::getXMLNodeForKey(m_sFilePath.c_str(), pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}
    
	bool ret = defaultValue;
    
	if (value)
	{
		ret = (! strcmp(value, "true"));
	}
    
    if (doc) delete doc;
    
	return ret;
}

int STUserDefault::getIntegerForKey(const char* pKey, int defaultValue/* = 0 */)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  STXML::getXMLNodeForKey(m_sFilePath.c_str(), pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}
    
	int ret = defaultValue;
    
	if (value)
	{
		ret = atoi(value);
	}
    
	if(doc)
	{
		delete doc;
	}
    
	return ret;
}

float STUserDefault::getFloatForKey(const char* pKey, float defaultValue/* = 0.0f */)
{
    float ret = (float)getDoubleForKey(pKey, (double)defaultValue);
    
    return ret;
}

double STUserDefault::getDoubleForKey(const char* pKey, double defaultValue/* = 0.0 */)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  STXML::getXMLNodeForKey(m_sFilePath.c_str(), pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}
    
	double ret = defaultValue;
    
	if (value)
	{
		ret = atof(value);
	}
    
    if (doc) delete doc;
    
	return ret;
}

string STUserDefault::getStringForKey(const char* pKey, const std::string & defaultValue/* = "" */)
{
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  STXML::getXMLNodeForKey(m_sFilePath.c_str(), pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}
    
	string ret = defaultValue;
    
	if (value)
	{
		ret = string(value);
	}
    
    if (doc) delete doc;
    
	return ret;
}

void STUserDefault::setBoolForKey(const char* pKey, bool value)
{
    // save bool value as string
    
    if (true == value)
    {
        setStringForKey(pKey, "true");
    }
    else
    {
        setStringForKey(pKey, "false");
    }
}

void STUserDefault::setIntegerForKey(const char* pKey, int value)
{
    // check key
    if (! pKey)
    {
        return;
    }
    
    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%d", value);
    
    STXML::setValueForKey(m_sFilePath.c_str(), pKey, tmp);
}

void STUserDefault::setFloatForKey(const char* pKey, float value)
{
    setDoubleForKey(pKey, value);
}

void STUserDefault::setDoubleForKey(const char* pKey, double value)
{
    // check key
    if (! pKey)
    {
        return;
    }
    
    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%f", value);
    
    STXML::setValueForKey(m_sFilePath.c_str(), pKey, tmp);
}

void STUserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    // check key
    if (! pKey)
    {
        return;
    }
    
    STXML::setValueForKey(m_sFilePath.c_str(), pKey, value.c_str());
}

STUserDefault* STUserDefault::getInstance()
{
    initXMLFilePath();
    
    // only create xml file one time
    // the file exists after the program exit
    if ((! isXMLFileExist()) && (! createXMLFile()))
    {
        return NULL;
    }
    
    if (! m_spUserDefault)
    {
        m_spUserDefault = new STUserDefault();
    }
    
    return m_spUserDefault;
}

bool STUserDefault::isXMLFileExist()
{
    return STXML::isXMLFileExist(m_sFilePath.c_str());
}

void STUserDefault::initXMLFilePath()
{
    if (! m_sbIsFilePathInitialized)
    {
        m_sFilePath += CCFileUtils::getInstance()->getWritablePath() + XML_FILE_NAME;
        m_sbIsFilePathInitialized = true;
    }
}

// create new xml file
bool STUserDefault::createXMLFile()
{
	bool bRet = false;
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    if (NULL==pDoc)
    {
        return false;
    }
    
    // add default xml header
    STXML::addDefaultHeader(pDoc);
    
    // add root node
	tinyxml2::XMLElement *pRootEle = pDoc->NewElement(USERDEFAULT_ROOT_NAME);
	if (NULL==pRootEle)
	{
		return false;
	}
	pDoc->LinkEndChild(pRootEle);
    
    bRet = STXML::createXMLFile(m_sFilePath.c_str(), pDoc);
    
	if(pDoc)
	{
		delete pDoc;
	}
    
	return bRet;
}

const string& STUserDefault::getXMLFilePath()
{
    return m_sFilePath;
}

void STUserDefault::reset()
{
    STXML::removeXMLFile(m_sFilePath.c_str());
}
