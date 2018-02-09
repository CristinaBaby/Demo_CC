//
//  DataManager.cpp
//
//  Created by tanshoumei on 3/10/16.
//
//

#include "depends/component/datamanager/DataManager.h"

void DataManager::insertDataOverrideOriginData(string module, string keyInModule, const cocos2d::Value& value)
{
    string&& key = formatFinalKey(module, keyInModule);
    bool isExist = isDataExist(module, keyInModule);
    if(isExist)
    {
        _dataMap[key] = value;
        return;
    }
    _dataMap.insert(make_pair(key, value));
}

void DataManager::insertDataNotOverrideOriginData(string module, string keyInModule, const cocos2d::Value& value)
{
    string&& key = formatFinalKey(module, keyInModule);
    bool isExist = isDataExist(module, keyInModule);
    if(isExist){
        __CCLOGWITHFUNCTION("warning, data already exist for key:%s, you choose not override, so insert failed", key.c_str());
        return;
    }
    _dataMap.insert(make_pair(key, value));
}

bool DataManager::isDataExist(string module, string keyInModule)
{
    string&& key = formatFinalKey(module, keyInModule);
    if(_dataMap.find(key) != _dataMap.end())
        return true;
    return false;
}

Value DataManager::getData(string module, string keyInModule)
{
    string&& key = formatFinalKey(module, keyInModule);
    if(_dataMap.find(key) != _dataMap.end())
    {
        return _dataMap.at(key);
    }
    else
    {
        __CCLOGWITHFUNCTION("warning, data not found for the key:%s", key.c_str());
        return Value();
    }
}

string DataManager::formatFinalKey(string module, string keyInModule)
{
    return module + "_" + keyInModule;
}
