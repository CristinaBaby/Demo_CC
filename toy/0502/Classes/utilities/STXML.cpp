//
//  STXML.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-24.
//
//

#include "STXML.h"
#include <fstream>

USING_NS_CC;
USING_NS_ST;
using namespace tinyxml2;
using namespace std;


bool STXML::createXMLFile(const char* filepath, XMLDocument* pXmlDoc)
{
    CCAssert(pXmlDoc, "XMLDocument point is NULL!");

    bool bRet = XML_SUCCESS == pXmlDoc->SaveFile(filepath);
    
    return bRet;
}


XMLDocument* STXML::loadXMLFile(const char* filepath)
{
    XMLDocument* xmlDoc = new XMLDocument();
    
    do {
        // on android OS, "LoadFile" method can't load the file under assets directory correctly.
        if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            
            //CCFileData data(STUserDefault::sharedUserDefault()->getXMLFilePath().c_str(),"rt");
            ssize_t nSize;
            const char* pXmlBuffer = (const char*)CCFileUtils::getInstance()->getFileData(filepath, "rb", &nSize);
            //const char* pXmlBuffer = (const char*)data.getBuffer();
            if(NULL == pXmlBuffer)
            {
                CCLOG("can not read xml file");
                break;
            }
            xmlDoc->Parse(pXmlBuffer);
            delete[] pXmlBuffer;
        }
        else
        {
            xmlDoc->LoadFile(filepath);
        }
        
    } while (0);
    
    return xmlDoc;
}

bool STXML::removeXMLFile(const char *filepath)
{
    bool bRet = false;
    
    if( remove(filepath) != 0 )
    {
        bRet = false;
        CCLOGERROR("Error deleting file");
    }
    else
    {
        bRet = true;
    }
    
    return bRet;
}

// Determine if xml exists.
bool STXML::isXMLFileExist(const char* filepath)
{
    FILE *fp = fopen(filepath, "r");
    
    bool bRet = false;
    if (fp)
    {
        bRet = true;
        
        fclose(fp);
    }
    
    return bRet;
}

bool STXML::backupXMLFile(const char* sourcepath, const char* backuppath)
{
    bool bRet = false;
    
    if (isXMLFileExist(sourcepath))
    {
        if (copyFile(sourcepath, backuppath))
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
        
    }
    else
    {
        bRet = false;
        CCLOGERROR("the source file is NOT exist!");
    }
    
    return bRet;
}

bool STXML::copyFile(const char *pszFile, const char *newFile)
{
    ifstream input;
    ofstream output;
    
    input.open(pszFile, ios::binary);
    
    if (input.fail())
    {
        input.close();
        output.close();
        
        return false;
    }
    
    output.open(newFile, ios::binary);
    
    if (output.fail())
    {
        input.close();
        output.close();
        
        return false;
    }
    else
    {
        output << input.rdbuf();
        
        output.close();
        input.close();
        
        return true;
    }
    
}

void STXML::addDefaultHeader(XMLDocument *doc)
{
    if(doc)
    {
        // add a declaration in the head of the file
        XMLDeclaration *pDeclaration = doc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
        if(!pDeclaration)
            return;
        doc->LinkEndChild(pDeclaration);
    }
}

tinyxml2::XMLElement* STXML::getXMLNodeForKey(const char* filepath, const char* pKey,
                                              tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = NULL;
    
    // check the key value
    if (! pKey)
    {
        return NULL;
    }
    
    do
    {
        // load xml file
 		tinyxml2::XMLDocument* xmlDoc = loadXMLFile(filepath);
		*doc = xmlDoc;
        
		// get root node
		*rootNode = xmlDoc->RootElement();
		if (NULL == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
        
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (NULL != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}
            
			curNode = curNode->NextSiblingElement();
		}
	} while (0);
    
	return curNode;
}

void STXML::setValueForKey(const char* filepath, const char* pKey, const char* pValue)
{
 	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
    
	// check the params
	if (! pKey || ! pValue)
	{
		return;
	}
    
	// find the node
	node = getXMLNodeForKey(filepath, pKey, &rootNode, &doc);
    
	// if node exist, change the content
	if (node)
	{
        if (node->FirstChild())
        {
            node->FirstChild()->SetValue(pValue);
        }
        else
        {
            tinyxml2::XMLText* content = doc->NewText(pValue);
            node->LinkEndChild(content);
        }
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey);//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue);//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}
	}
    
    // save file and free doc
	if (doc)
	{
		doc->SaveFile(filepath);
		delete doc;
	}
}
