//
//  DataManager.h
//
//  Created by tanshoumei on 3/10/16.
//
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include <stdio.h>
#include <map>
#include "cocos2d.h"
#include "depends/component/utils/SingleTon.h"

using namespace std;
using namespace cocos2d;

/// used for module data exchange.
class DataManager: public SingleTon<DataManager>
{
public:
    void insertDataOverrideOriginData(string module, string keyInModule, const Value& value);
    
    void insertDataNotOverrideOriginData(string module, string keyInModule, const Value& value);
    
    bool isDataExist(string module, string keyInModule);
    
    //you should check the returned value is null or not.
    Value getData(string module, string keyInModule);
    
private:
    std::string formatFinalKey(string module, string keyInModule);
    
private:
    map<string ,Value> _dataMap;
};
#endif /* DataManager_hpp */
