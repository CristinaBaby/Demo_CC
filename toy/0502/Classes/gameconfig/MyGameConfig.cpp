#include "MyGameConfig.h"
#include "../utilities/STUserDefault.h"

static MyGameConfig *_sharedMyGameConfig = nullptr;

MyGameConfig* MyGameConfig::getInstance()
{
    if (! _sharedMyGameConfig)
    {
        _sharedMyGameConfig = new MyGameConfig();
    }
    
    return _sharedMyGameConfig;
}

void MyGameConfig::destroyInstance()
{
    CC_SAFE_DELETE(_sharedMyGameConfig);
}

MyGameConfig::MyGameConfig()
{
    _pictureIndex = STLoadIntegerFromUD("pictureIndex", 0);
}

string MyGameConfig::getFullFileName()
{
    string str = getFileName() + ".jpg";
    return std::move(str);
}

void MyGameConfig::setPictureIndex(int pictureIndex)
{
    _pictureIndex = pictureIndex;
    STSaveIntegerToUD("pictureIndex", _pictureIndex);
    
}

int MyGameConfig::getPictureIndex()
{
    return _pictureIndex;
}