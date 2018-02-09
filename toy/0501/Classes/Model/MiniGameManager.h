//
//  MiniGameManager.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__MiniGameManager__
#define __BedtimeStory__MiniGameManager__

#include "cocos2d.h"
#include "AppConfigs.h"

class BaseGameData
{
public:
    BaseGameData() :
    index(-1),
    resPath(""),
    logo("")
    {};
    
public:

    int index;
    std::string resPath;
    std::string logo;
};

#pragma mark- Spot Game Data

class SpotGameData : public BaseGameData
{
public:
    SpotGameData() :
    tmxFileName(""),
    textureFileName(""),
    textureFilePlist("")
    {};
    
public:
  
    std::string tmxFileName;
    std::string textureFileName;
    std::string textureFilePlist;
};

#pragma mark- Jigsaw Game Data

class JigsawConfig
{
public:
    JigsawConfig():
    rows(0),
    columns(0),
    totalWidth(0.0),
    totalHeight(0.0)
    {};
    
    struct JigsawTileInfo
    {
        float offsetx;
        float offsety;
        float width;
        float height;
        
        std::string tilePath;
    };
  
public:
    int rows;
    int columns;
    float totalWidth;
    float totalHeight;

    void init(const std::string& csvfile);
    std::vector<JigsawTileInfo> jigsawTilesVector;
};

class JigsawGameData : public BaseGameData
{
public:
    JigsawGameData() :
    textureFileName(""),
    config(nullptr)
    {};
    
    ~JigsawGameData()
    {
        if (config != nullptr)
        {
            delete config;
        }
    };
    
public:

    JigsawConfig *config;
    std::string textureFileName;
};

#pragma mark- ColorBook Game Data

class ColorBookGameData : public BaseGameData
{
public:
    ColorBookGameData() :
    pictureFileName(""),
    maskFileName("")
    {};
    
public:
    std::string     pictureFileName;
    std::string     maskFileName;
};

struct Pen
{
    std::string penLogo;
    GLuint R;
    GLuint G;
    GLuint B;
    GLuint A;
    int index;
};

struct Pattern
{
    std::string patternLogo;
    std::string pattern;
    int index;
};

enum class PensType
{
    PensType_None = 0,
    PensType_Pen1,
    PensType_Pen2,
    PensType_Pattern,
};

#pragma mark- Record Game Data

class RecordGameData : public BaseGameData //this data struct used to show logo
{
public:
    RecordGameData():
    leftStoryImageFile(""),
    rightStoryImageFile("")
    {};
    
    ~RecordGameData()
    {
        
    };
    
public:
    std::string leftStoryImageFile;
    std::string rightStoryImageFile;

};

struct RecordInfo //this data struct used to store record info
{
    int index;
    int recordTime;
    bool isSelected;
    std::string recordFilePath;
    std::string recordDate;
};

#pragma mark- Game Manager

enum class GameType
{
    None = 0,
    Spot,
    Jigsaw,
    ColorBook,
    Story,
    Record
};

class MiniGameManager
{
public:
    static MiniGameManager* getInstance();
    MiniGameManager();
    ~MiniGameManager();
    
    void initSpotData();
    void initJigsawData();
    
    //for color book game
    void initColorBookData();
    void setupPens(const std::string& path);
    void setupPatterns(const std::string& path);
    std::vector<Pen> *getPens(){return &_pensVector;};
    std::vector<Pattern> *getPatterns(){return &_patternsVector;};
    CC_SYNTHESIZE(PensType, _currentPensType, CurrentPensType);
    
    //for record data
    void initRecordData();
    void saveRecordInfo();
    
    void addRecordInfo(const int storyIndex, RecordInfo *info);
    void deleteRecordInfo(const int storyIndex, RecordInfo *info);
    std::vector<RecordInfo*>* getRecordInfoVector(const int storyIndex);

    void destroySpotGameData();
    void destroyJigsawGameData();
    void destroyColorBookGameData();
    void destroyRecordDataVector();

    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<SpotGameData*>, _spotDataVector, SpotDataVector);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<JigsawGameData*>, _jigsawDataVector, JigsawDataVector);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<ColorBookGameData*>, _colorBookDataVector, ColorBookDataVector);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<RecordGameData*>, _recordDataVector, RecordDataVector);
    
protected:
    std::vector<Pen> _pensVector;
    std::vector<Pattern> _patternsVector;
    
    //key->story index, value->RecordInfo sturct, used to store records
    std::unordered_map<std::string, std::vector<RecordInfo*>> _recordInfoMap;

    cocos2d::ValueVector _recordInfoV;
};

#endif /* defined(__BedtimeStory__MiniGameManager__) */
