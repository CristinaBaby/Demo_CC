//
//  MiniGameManager.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "MiniGameManager.h"
#include "CSVParse.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace extension;

static MiniGameManager *s_SingletonParfaitMakerManager = nullptr;

MiniGameManager::MiniGameManager()
{}

MiniGameManager::~MiniGameManager()
{
    destroySpotGameData();
    destroyJigsawGameData();
}

MiniGameManager* MiniGameManager::getInstance()
{
    if (!s_SingletonParfaitMakerManager) {
        s_SingletonParfaitMakerManager = new (std::nothrow)(MiniGameManager);
    }
    
    return s_SingletonParfaitMakerManager;
}

#pragma mark- Destroy Game Data

void MiniGameManager::destroySpotGameData()
{
    for (auto data : _spotDataVector)
    {
        delete data;
    }
    _spotDataVector.clear();
}

void MiniGameManager::destroyJigsawGameData()
{
    for (auto data : _jigsawDataVector)
    {
        delete data;
    }
    _jigsawDataVector.clear();
}

void MiniGameManager::destroyColorBookGameData()
{
    for (auto data : _colorBookDataVector)
    {
        delete data;
    }
    _colorBookDataVector.clear();
}

void MiniGameManager::destroyRecordDataVector()
{
    for (auto data : _recordDataVector)
    {
        delete data;
    }
    _recordDataVector.clear();
}

#pragma mark- Spot

void MiniGameManager::initSpotData()
{
    destroySpotGameData();
    
    //inner init, spot game has 30 pictures
    for (int i = 1; i <= 30; ++i)
    {
        auto spotData = new SpotGameData();
        
        //format res path with index
        std::string indexString = i < 10? "0" + std::to_string(i) : std::to_string(i);
        
        spotData->index = i;
        spotData->resPath = "Spot/L" + indexString + "/";
        spotData->tmxFileName = "L" + indexString + ".tmx";
        spotData->textureFileName =  "L" + indexString + ".png";
        spotData->textureFilePlist =  "L" + indexString + ".plist";
        spotData->logo = std::to_string(i) + ".png";

        _spotDataVector.push_back(spotData);
    }
}

#pragma mark- Jigsaw

void MiniGameManager::initJigsawData()
{
    destroyJigsawGameData();
    
    //inner init, jigsaw game has 30 pictures
    for (int i = 1; i <= 30; ++i)
    {
        auto jigsawData = new JigsawGameData();

        //format res path with index
        std::string indexString = i < 10? "0" + std::to_string(i) : std::to_string(i);
        
        jigsawData->index = i;
        jigsawData->resPath = "res/Jigsaw/";
        jigsawData->textureFileName = "res/Jigsaw/L" + indexString + ".jpg";
        jigsawData->logo = std::to_string(i) + ".png";
        
        //use 4x4 jigsaw tile
        auto jigsawConfig = new JigsawConfig();
        jigsawConfig->init("config/TileConfig4x4.csv");
        jigsawData->config = jigsawConfig;

        _jigsawDataVector.push_back(jigsawData);
    }
}

void JigsawConfig::init(const std::string &csvfile)
{
    auto parser = CSVParse::create(csvfile.c_str());
    int rows = parser->getRows() - 1;
    if (rows == 0) {
        return;
    }
    
    int matrix = (int)sqrt(rows);
    for (int row = 1; row <= rows; ++row)
    {
        JigsawTileInfo tileInfo;
        
        int tileIndex = atoi(parser->getData(row, 1));
        tileInfo.offsetx = atof(parser->getData(row, 2));
        tileInfo.offsety = atof(parser->getData(row, 3));
        tileInfo.width = atof(parser->getData(row, 4));
        tileInfo.height = atof(parser->getData(row, 5));
        tileInfo.tilePath = "res/Jigsaw/tileset/tile" + std::to_string(tileIndex) + ".png";

        if (row == matrix)
        {
            this->totalWidth = float(tileInfo.offsetx + tileInfo.width/2);
        }
        
        if (row == rows)
        {
            this->totalHeight = tileInfo.offsety + tileInfo.height/2;
        }
        
        jigsawTilesVector.push_back(tileInfo);
    }
    
    this->rows = matrix;
    this->columns = matrix;
}

#pragma mark- ColorBook

void MiniGameManager::initColorBookData()
{
    destroyColorBookGameData();
    
    //inner init, colobook game has 30 pictures
    for (int i = 1; i <= 30; ++i)
    {
        auto jigsawData = new ColorBookGameData();
        
        //format res path with index
        std::string indexString = i < 10? "0" + std::to_string(i) : std::to_string(i);
        
        jigsawData->index = i;
        jigsawData->resPath = "res/Colorbook/";
        jigsawData->pictureFileName = "res/Colorbook/L" + indexString + "_picture.png";
        jigsawData->maskFileName = "res/Colorbook/L" + indexString + "_mask.png";
        jigsawData->logo = std::to_string(i) + ".png";

        _colorBookDataVector.push_back(jigsawData);
    }
}

void MiniGameManager::setupPens(const std::string& path)
{
    auto parser = CSVParse::create(path.c_str());
    int rows = parser->getRows();
    if (rows == 0) {
        return;
    }

    //clear old data
    _pensVector.clear();
    
    //init new data
    for (int row = 1; row <= rows - 1; ++row)
    {
        Pen pen;
        pen.index = atoi(parser->getData(row, 0));
        pen.penLogo = "ui/pens/" + std::string(parser->getData(row, 1));
        pen.R = atoi(parser->getData(row, 2));
        pen.G = atoi(parser->getData(row, 3));
        pen.B = atoi(parser->getData(row, 4));
        pen.A = 255.0;

        _pensVector.push_back(pen);
    }
}

void MiniGameManager::setupPatterns(const std::string& path)
{
    auto parser = CSVParse::create(path.c_str());
    int rows = parser->getRows();
    if (rows == 0) {
        return;
    }
    
    //clear old data
    _patternsVector.clear();
    
    //init new data
    for (int row = 1; row <= rows - 1; ++row)
    {
        Pattern pattern;
        pattern.index = atoi(parser->getData(row, 0));
        pattern.patternLogo = "ui/pens/" + std::string(parser->getData(row, 1));
        pattern.pattern = "ui/patterns/" + std::string(parser->getData(row, 2));

        _patternsVector.push_back(pattern);
    }
}

#pragma mark- Record

void MiniGameManager::initRecordData()
{
    destroyRecordDataVector();
    
    //
    for (int i = 1; i <= 30; ++i)
    {
        auto recordData = new RecordGameData();
        
        //format res path with index
        std::string indexString = i < 10? "0" + std::to_string(i) : std::to_string(i);
        
        std::string prefix = i < 10 ? "0" : "";
        std::string left = prefix + std::to_string(i) + "_1.png";
        std::string right = prefix + std::to_string(i) + "_2.png";

        recordData->index = i;
        recordData->logo = std::to_string(i) + ".png";
        recordData->leftStoryImageFile = left;
        recordData->rightStoryImageFile = right;

        _recordDataVector.push_back(recordData);
    }
    
    std::string recordInfoPath = FileUtils::getInstance()->getWritablePath() + RECORD_FILE;
    auto mapInfo = FileUtils::getInstance()->getValueMapFromFile(recordInfoPath);
    if (mapInfo.size() == 0)
        return;
    
    for (auto iterator : mapInfo)
    {
        auto key = iterator.first;
        auto vector = iterator.second.asValueVector();
        
        std::vector<RecordInfo*> infoVector;
        for (auto dataInfo : vector)
        {
            auto info = new RecordInfo();
            info->index = dataInfo.asValueMap().at("index").asInt();
            info->recordTime = dataInfo.asValueMap().at("recordTime").asInt();
            info->recordFilePath = dataInfo.asValueMap().at("recordFilePath").asString();
            info->recordDate = dataInfo.asValueMap().at("recordDate").asString();
            info->isSelected = dataInfo.asValueMap().at("isSelected").asBool();
            
            infoVector.push_back(info);
        }
        
        _recordInfoMap.insert(std::make_pair(key, infoVector));
    }
}

void MiniGameManager::saveRecordInfo()
{
    auto mapInfo = ValueMap();
    for (auto iterator : _recordInfoMap)
    {
        auto key = iterator.first;
        auto vector = iterator.second;
        
        auto recordVector = ValueVector();
        for (auto dataInfo : vector)
        {
            auto recordMap = ValueMap();
            
            recordMap.insert(std::make_pair("index", Value(dataInfo->index)));
            recordMap.insert(std::make_pair("recordTime", Value(dataInfo->recordTime)));
            recordMap.insert(std::make_pair("recordFilePath", Value(dataInfo->recordFilePath)));
            recordMap.insert(std::make_pair("recordDate", Value(dataInfo->recordDate)));
            recordMap.insert(std::make_pair("isSelected", Value(dataInfo->isSelected)));

            recordVector.push_back(Value(recordMap));
        }
        
        mapInfo.insert(std::make_pair(key, Value(recordVector)));
    }
    
    std::string recordInfoPath = FileUtils::getInstance()->getWritablePath() + RECORD_FILE;
    FileUtils::getInstance()->writeToFile(mapInfo, recordInfoPath);
}

void MiniGameManager::addRecordInfo(const int storyIndex, RecordInfo *info)
{
    std::string stringIndex = std::to_string(storyIndex);
    auto iterator = _recordInfoMap.find(stringIndex);
    if (iterator == _recordInfoMap.end())
    {
        std::vector<RecordInfo*> recordInfoVector;
        _recordInfoMap.insert(std::make_pair(stringIndex, recordInfoVector));
    }
    else
    {
        if (_recordInfoMap.at(stringIndex).size() >= 5)
        {
            auto theLastOne = _recordInfoMap.at(stringIndex).begin();
            
            //remove record file
            std::string recordFile = FileUtils::getInstance()->getWritablePath() + (*theLastOne)->recordFilePath;
            if (FileUtils::getInstance()->isFileExist(recordFile))
            {
                if (remove(recordFile.c_str()) != 0)
                {
                    CCLOG("WARNING: remove file :%s faild!", recordFile.c_str());
                }
            }
            
            delete (*theLastOne);
            _recordInfoMap.at(stringIndex).erase(theLastOne);
        }
    }

    _recordInfoMap.at(stringIndex).push_back(info);

    saveRecordInfo();
}

void MiniGameManager::deleteRecordInfo(const int storyIndex, RecordInfo *info)
{
    std::string stringIndex = std::to_string(storyIndex);
    auto iterator = _recordInfoMap.find(stringIndex);
    if (iterator != _recordInfoMap.end())
    {
        auto find = std::find(_recordInfoMap.at(stringIndex).begin(), _recordInfoMap.at(stringIndex).end(), info);
        if (find != _recordInfoMap.at(stringIndex).end())
        {
            //remove record file
            std::string recordFile = FileUtils::getInstance()->getWritablePath() + (*find)->recordFilePath;
            if (FileUtils::getInstance()->isFileExist(recordFile))
            {
                if (remove(recordFile.c_str()) != 0)
                {
                    CCLOG("WARNING: remove file :%s faild!", recordFile.c_str());
                }
            }
            
            delete (*find);
            _recordInfoMap.at(stringIndex).erase(find);
        }
    }
    
    saveRecordInfo();
}

std::vector<RecordInfo*>* MiniGameManager::getRecordInfoVector(const int storyIndex)
{
    std::string stringIndex = std::to_string(storyIndex);
    auto iterator = _recordInfoMap.find(stringIndex);
    if (iterator != _recordInfoMap.end())
    {
        return &_recordInfoMap.at(stringIndex);
    }
    
    return nullptr;
}





