//
//  AsyncManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "AsyncManager.h"
#include "UICpp.h"


static AsyncManager *_sharedAsync = nullptr;

AsyncManager* AsyncManager::getInstance()
{
    if (! _sharedAsync)
    {
        _sharedAsync = new AsyncManager();
    }
    
    return _sharedAsync;
}

void AsyncManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedAsync);
}

AsyncManager::AsyncManager()
{
    //xScheduler->scheduleUpdate(this, 0, false);
}

void AsyncManager::add(const string &fileName)
{
    addResource(fileName);
//    _filesname.push_back(fileName);
}

void AsyncManager::update(float dt)
{
    if (!_filesname.empty()) {
        addResource(_filesname.front());
        _filesname.pop_front();
    }
}

void AsyncManager::addResource(const string &fileName)
{
    size_t dot = fileName.find_last_of(".");
    string suffix = fileName.substr(dot + 1, fileName.length() - (dot + 1)) ;
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), (int (*)(int))tolower);
    
    if(suffix == "json" || suffix == "exportjson")
    {
        rapidjson::Document jsonDict;
        string contentStr = FileUtils::getInstance()->getStringFromFile(fileName);
        jsonDict.Parse<0>(contentStr.c_str());
        if (!jsonDict.HasParseError())
        {
            const rapidjson::Value &widgetTree = jsonDict["widgetTree"];
            if(!widgetTree.IsNull())
            {
                addGUI(widgetTree);
            }
            else
            {
                _fileCount++;
                ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(fileName, this, schedule_selector(AsyncManager::addArmatureFinished));
            }
        }
    }
    else if(suffix == "png" || suffix == "jpg")
    {
        _fileCount++;
        Director::getInstance()->getTextureCache()->addImageAsync(fileName, CC_CALLBACK_1(AsyncManager::addFinished, this));
    }
}

void AsyncManager::addImages(const vector<string> &filesName)
{
    for (auto file : filesName) {
        _fileCount++;
        Director::getInstance()->getTextureCache()->addImageAsync(file, CC_CALLBACK_1(AsyncManager::addFinished, this));
    }
}

void AsyncManager::addFinished(Texture2D* texture)
{
    _fileCount--;
}

void AsyncManager::addArmatureFinished(float dt)
{
    _fileCount--;
}

void AsyncManager::addGUI(const rapidjson::Value &widgetTree)
{
    widgetFromJsonDictionary(widgetTree);
    
    addImages(_filesnameGUI);
    _filesnameGUI.clear();
}

void AsyncManager::widgetFromJsonDictionary(const rapidjson::Value& value)
{
    string classname = value["classname"].GetString();  //e.g. ImageView
    const rapidjson::Value& options = value["options"]; //参数, 包括fileNameData
    
    if (classname.compare("ImageView") == 0)
    {
        const rapidjson::Value& fileNameData = options["fileNameData"];
        auto path = fileNameData["path"].GetString();
        _filesnameGUI.push_back(path);
    }
    else if (classname.compare("Button") == 0)
    {
        const rapidjson::Value& normalData = options["normalData"];
        auto path = normalData["path"].GetString();
        _filesnameGUI.push_back(path);
        
        const rapidjson::Value& pressedData = options["pressedData"];
        if (pressedData["path"].IsString()) {
            path = pressedData["path"].GetString();
            _filesnameGUI.push_back(path);
        }

        const rapidjson::Value& disabledData = options["disabledData"];
        if (disabledData["path"].IsString()) {
            path = disabledData["path"].GetString();
            _filesnameGUI.push_back(path);
        }
    }
    else if (classname.compare("Panel") == 0)
    {
        const rapidjson::Value& backGroundImageData = options["backGroundImageData"];
        auto path = backGroundImageData["path"].GetString();
        _filesnameGUI.push_back(path);
    }
    
    int childrenCount = value["children"].Size();
    for (int i = 0; i < childrenCount; i++)
    {
        const rapidjson::Value& children = value["children"][i];
        //递归调用自己解析
        widgetFromJsonDictionary(children);
    }
}

void AsyncManager::widgetFromJsonDictionaryOriginal(const rapidjson::Value& value)
{
    string classname = DICTOOL->getStringValue_json(value, "classname");    //e.g. ImageView
    const rapidjson::Value& options = DICTOOL->getSubDictionary_json(value, "options");    //参数, 包括fileNameData
    
    if (classname.compare("ImageView") == 0)
    {
        const rapidjson::Value& fileNameData = DICTOOL->getSubDictionary_json(options, "fileNameData");
        auto path = DICTOOL->getStringValue_json(fileNameData, "path");
        _filesnameGUI.push_back(path);
    }
    else if (classname.compare("Button") == 0)
    {
        const rapidjson::Value& normalData = DICTOOL->getSubDictionary_json(options, "normalData");
        auto path = DICTOOL->getStringValue_json(normalData, "path");
        _filesnameGUI.push_back(path);
        
        const rapidjson::Value& pressedData = DICTOOL->getSubDictionary_json(options, "pressedData");
        path = DICTOOL->getStringValue_json(pressedData, "path");
        if (path) {
            _filesnameGUI.push_back(path);
        }
        
        const rapidjson::Value& disabledData = DICTOOL->getSubDictionary_json(options, "disabledData");
        path = DICTOOL->getStringValue_json(disabledData, "path");
        if (path) {
            _filesnameGUI.push_back(path);
        }
    }
    else if (classname.compare("Panel") == 0)
    {
        const rapidjson::Value& backGroundImageData = DICTOOL->getSubDictionary_json(options, "backGroundImageData");
        auto path = DICTOOL->getStringValue_json(backGroundImageData, "path");
        _filesnameGUI.push_back(path);
    }
    
    int childrenCount = DICTOOL->getArrayCount_json(value, "children");
    for (int i = 0; i < childrenCount; i++)
    {
        const rapidjson::Value& children = DICTOOL->getDictionaryFromArray_json(value, "children", i);
        //递归调用自己解析
        widgetFromJsonDictionary(children);
    }
}
