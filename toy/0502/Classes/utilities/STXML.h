//
//  STXML.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-24.
//
//

#ifndef __MysteryGuardians__STDataXML__
#define __MysteryGuardians__STDataXML__

#include "cocos2d.h"
#include "STMacro.h"
#include "tinyxml2/tinyxml2.h"

NS_ST_BEGIN

class STXML : public cocos2d::Ref{
    
public:
    
    /**
     * @brief create a xml file with specific name
     * @note you must delete XMLDocument point explicitly.
     *
     * @param full path name
     * @param users defined their own XMLDocument object point
     *
     * @return true if file is created successfully, false otherwise.
     */
    static bool createXMLFile(const char* filepath, tinyxml2::XMLDocument* pXmlDoc);
    
    /**
     * @brief read a exist xml file with specific name. if file not exist nothing to do.
     * @note you must delete XMLDocument point explicitly.
     *
     * @param full path name
     *
     * @return tinyxml2::XMLDocument object point.
     */
    static tinyxml2::XMLDocument* loadXMLFile(const char* filepath);
    
    /**
     * @brief remove a exist file.
     * @param full path name
     */
    static bool removeXMLFile(const char* filepath);
    
protected:
    /**
     * @brief check if this file is existed.
     *
     * @param full path name
     */
    static bool isXMLFileExist(const char* filepath);
    
    /**
     * @brief backup the source file
     *
     * @param socurce file's full path
     * @param backup file's full path
     */
    static bool backupXMLFile(const char* sourcepath, const char* backuppath);
    
    /**
     * @brief create a new file by copy frome a existed file.
     *
     * @param socurce file's full path
     * @param backup file's full path
     */
    static bool copyFile(const char*pszFile, const char* newFile);
    
    static tinyxml2::XMLElement* getXMLNodeForKey(const char* filepath, const char* pKey,
                                                  tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc);
    
    static void setValueForKey(const char* filepath, const char* pKey, const char* pValue);

    /**
     * @brief add default xml header, "<?xml version="1.0" encoding="UTF-8"?>"
     */
    static void addDefaultHeader(tinyxml2::XMLDocument *doc);

};

NS_ST_END

#endif /* defined(__MysteryGuardians__STDataXML__) */
