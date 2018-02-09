//
//  CategoryReader.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/24.
//
//

#include "CategoryReader.h"
#include "cocos2d.h"
USING_NS_CC;

vector<string> CategoryReader::getIconPath(string categoryName){
    vector<string> tempPaths;
    auto _parentPath = "icon_category/" +categoryName;
    int _num = FilePathUtil::getFileNum(_parentPath);
    stringstream _temp;
    for(int i=0;i<_num;i++){
        _temp.str("");
        _temp.clear();
        _temp<<_parentPath<<"/"<<categoryName<<i<<".png";
        tempPaths.push_back(_temp.str());
    }
    return tempPaths;
}
string CategoryReader::getCategoryPath(string categoryName ,int index){
    stringstream _temp;
    _temp<<"category/"<<categoryName<<"/"<<categoryName<<index<<".png";
    if(FileUtils::getInstance()->isFileExist(_temp.str()))
        return _temp.str();
    else{
        _temp.str("");
        _temp.clear();
        _temp<<"category/"<<categoryName<<"/"<<categoryName<<index<<".jpg";
        if(FileUtils::getInstance()->isFileExist(_temp.str()))
            return _temp.str();
    }
    return _temp.str();
}