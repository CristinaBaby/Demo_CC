//
//  AsyncManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__AsyncManager__
#define __ColorBook__AsyncManager__

#include "UIHeader.h"
#include "json/rapidjson.h"
#include "json/document.h"

#define xAsync  (AsyncManager::getInstance())

class AsyncManager : public Node
{
public:
    static AsyncManager* getInstance();
    void destroyInstance();
    
    void add(const string &fileName);
    void addImages(const vector<string> &filesName);
    int  getFileCount() { return _fileCount; };
    void update(float dt);
    
protected:
    AsyncManager();
    void addResource(const string &fileName);
    void addGUI(const rapidjson::Value &widgetTree);
    void addFinished(Texture2D* texture);
    void addArmatureFinished(float dt);
    void widgetFromJsonDictionary(const rapidjson::Value& value);
    void widgetFromJsonDictionaryOriginal(const rapidjson::Value& value);           //以后备用

protected:
    deque<string>       _filesname;
    vector<string>      _filesnameGUI;
    int                 _fileCount = 0;
};

#endif /* defined(__ColorBook__AsyncManager__) */
