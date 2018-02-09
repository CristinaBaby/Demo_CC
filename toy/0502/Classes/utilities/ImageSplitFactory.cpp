//
//  ImageSplitFactory.cpp
//  cocosTest
//
//  Created by zhangguangzong1 on 2/12/14.
//
//

#include "ImageSplitFactory.h"
#include "CSVParse.h"
#include "../gameconfig/GameConfig.h"
#include "AppGlobal.h"
#include "AppConfig.h"
#include "../utilities/STVisibleRect.h"


ImageSplitFactory* ImageSplitFactory::instance = NULL;

ImageSplitFactory::ImageSplitFactory():mTileConfigs(NULL),mScale(1.0f),pictureX(300) , pictureY(1536)
{
    pictureX = GET_GAME_MAP_LEFTTOP.x;
    pictureY = GET_GAME_MAP_LEFTTOP.y;
}

ImageSplitFactory::~ImageSplitFactory()
{
    if (mTileConfigs)
    {
        mTileConfigs->release();
    }
}

ImageSplitFactory* ImageSplitFactory::sharedImageSplitFactory()
{
    if (!instance)
    {
        instance = new ImageSplitFactory();
    }
    return instance;
}


void ImageSplitFactory::destoryInstance()
{
    CC_SAFE_DELETE(instance);
}

void ImageSplitFactory::loadTileConfigs(const char* csvPath)
{
    mTileConfigs = CCArray::create();
    mTileConfigs->retain();
    
    /*拼图块类型信息*/
    CSVParse* config_csv = CSVParse::create(csvPath);
    const unsigned rown = config_csv->getRows();
    
    for (int r = 1; r < rown; r++)
    {
        CCDictionary* eachTile = CCDictionary::create();
        
        CCString* strOffsetRight = CCString::create(config_csv->getDatas(r, "offsetRight"));
        eachTile->setObject(strOffsetRight, "offsetRight");
        
        CCString* strOffsetBottom = CCString::create(config_csv->getDatas(r, "offsetBottom"));
        eachTile->setObject(strOffsetBottom, "offsetBottom");
        
        CCString* strWidth = CCString::create(config_csv->getDatas(r, "width"));
        eachTile->setObject(strWidth    , "width");
        
        CCString* strHeight = CCString::create(config_csv->getDatas(r, "height"));
        eachTile->setObject(strHeight, "height");
        
        
        mTileConfigs->addObject(eachTile);
    }
}



void ImageSplitFactory::loadSplitByCalculate(int size)
{
    if (!mTileConfigs)
    {
//        CCLOGERROR("you must be load tile configs first!");
        return;
    }
    
    for (vector<vector<AreaInfo> >::iterator it = mAreaInfos.begin(); it!=mAreaInfos.end();it++)
    {
        (*it).clear();
    }
}

void ImageSplitFactory::calculateScale(int column)
{
    if (column == 4  || column == 3)
    {
        mScale = 1024/column/baseTileSize;
        mScale = UI_SCALE*mScale*0.99;
        return;
    }else if(column == 2){
        mScale = 1024/column/baseTileSize;
        mScale = UI_SCALE*mScale*0.965;
        
    }else
    {
        mScale = 1024/column/baseTileSize;
        mScale = UI_SCALE*mScale;
    }
    
    
}



//切图时  通过移动背景来切歌
void ImageSplitFactory::splitImage(const char* imagePath,vector<TileSprite*>& v,bool isRetain)
{
    if (!mAreaInfos.size())
    {
//        CCLOGERROR("you must be load splite info first!");
        return;
    }
    v.clear();
//    log("imagePath=%s" , imagePath);
//    Sprite* pSprite = Sprite::create(imagePath);
//    pSprite->setAnchorPoint(Vec2(0, 1));
//    pSprite->setPosition(Vec2(0,0));
//    CCLOG("pSprite-width = %f  pSprite-height = %f" , pSprite->getContentSize().width ,pSprite->getContentSize().height);

    //pSprite->setScale(UI_SCALE);
    
    

    vector<float> lasty;
    //地图 行列数是相同的  mAreaInfos.size()表示 多少行  这里表示列数
    for (int i = 0 ; i< mAreaInfos.size() ; i++)
    {
        lasty.push_back(0);
    }

    int index = 0;
    
    TextureCache::getInstance()->addImage(imagePath);
    
    auto texture = TextureCache::getInstance()->getTextureForKey(imagePath);
    for (vector<vector<AreaInfo> >::iterator it = mAreaInfos.begin(); it!=mAreaInfos.end();it++)
    {
        int count = 0;
        float lastX =0;
        
        for (vector<AreaInfo>::iterator it_ = (*it).begin(); it_ != (*it).end(); it_++)
        {
            //后面看下内存, 优化成只创建一个Texture
//            Sprite* pSprite = Sprite::create(imagePath);
            Sprite* pSprite = Sprite::createWithTexture(texture);
            pSprite->setAnchorPoint(Vec2(0, 1));
            pSprite->setPosition(Vec2(0,0));
            ccBlendFunc bSrc = {GL_DST_ALPHA,GL_ONE_MINUS_DST_COLOR};
            pSprite->setScale(UI_SCALE);
            pSprite->setBlendFunc(bSrc);
            
            
            const char* name ;
            if(column%2==0)
                name =   CCString::createWithFormat("tileset/tile%d.png",(*it_).mType)->getCString();
            else
             name =   CCString::createWithFormat("tileset1/tile%d.png",(*it_).mType)->getCString();
         
         
         Sprite* mask = Sprite::create(name);
         mask->setAnchorPoint(Vec2(0, 0));
         mask->setPosition(Vec2(0,0));
         mask->setScale(mScale);
//            mask->setBlendFunc(bDst);
         
         pSprite->setPosition(Vec2((*it_).mLTPoint.x,(*it_).mLTPoint.y + mask->getContentSize().height * mScale));

         
         CCRenderTexture* rt = CCRenderTexture::create((int)(mask->getContentSize().width * mScale),
          (int)(mask->getContentSize().height* mScale));
         rt->begin();
         mask->visit();
         pSprite->visit();
         rt->end();
         
         
            //TileSprite 直接由切割好的Texture创建
//            log("width = %f  height =%f" , rt->getSprite()->getContentSize().width , rt->getSprite()->getContentSize().height);
         TileSprite* tile = TileSprite::create(rt->getSprite()->getTexture());
//            tile->initWithTexture(rt->getSprite()->getTexture());
         tile->setFlipY(true);
         tile->mType = (*it_).mType;
         tile->set_m_Index(index);
         if (isRetain)
            tile->retain();
        tile->setAnchorPoint(Vec2(0, 1));
        tile->cocos2d::Node::setPosition(pictureX + lastX, pictureY + lasty[count]);
        
        
        log("%f %f", tile->getPositionX(), tile->getPositionY());
        tile->setOriginalPoint(tile->getPosition());
        
        v.push_back(tile);
        
        lastX = lastX +  baseTileSize * mScale -  (*it_).mOffsetRight*mScale;
        lasty[count] = lasty[count] - baseTileSize * mScale - (*it_).mOffsetBottom * mScale;
        
        
        count++;
        index++;
        
        Director::getInstance()->getTextureCache()->removeTexture(pSprite->getTexture());
            //break;
        
    }
    
}


lasty.clear();
//    pSprite->release();

}


void ImageSplitFactory::loadRenderSplitByConfigs(const char* csvPath)
{
    if (!mTileConfigs)
    {
        CCLOGERROR("you must be load tile configs first!");
        return;
    }
    
    for (vector<vector<AreaInfo> >::iterator it = mAreaInfos.begin(); it!=mAreaInfos.end();it++)
    {
        (*it).clear();
    }
    
    /*图片切分信息*/
    CSVParse* desc_csv = CSVParse::create(csvPath);
    //获取行列个数
    CCString* strW = CCString::create(desc_csv->getData(0, 0));
    row = strW->intValue();
    CCString* strH = CCString::create(desc_csv->getData(0, 1));
    column= strH->intValue();
    
    this->calculateScale(row);
    
    float offsetX = 0;
    vector<float> offsetYs;
    for (int i = 0 ; i< row ; i++)
    {
        offsetYs.push_back(0);
    }
    
    for (int i = 0; i< row ; i++)
    {
        vector<AreaInfo> vAreas;
        for (int j = 0; j < column; j++)
        {
            CCString* strType = CCString::create(desc_csv->getData(i+1, j));
            CCDictionary* eachTile = (CCDictionary*)mTileConfigs->objectAtIndex(strType->intValue() - 1);
            AreaInfo info;
            info.mType = strType->intValue();
            info.mOffsetRight = ((CCString*)eachTile->objectForKey("offsetRight"))->floatValue();
            info.mOffsetBottom = ((CCString*)eachTile->objectForKey("offsetBottom"))->floatValue();
            
            
            info.mLTPoint = Vec2(offsetX ,offsetYs[j]);
            info.mOffset = Vec2(-info.mLTPoint.x, -info.mLTPoint.y);
            offsetX =  offsetX - baseTileSize * mScale + info.mOffsetRight * mScale;
            offsetYs[j] = offsetYs[j] + baseTileSize * mScale + info.mOffsetBottom * mScale;
            
            vAreas.push_back(info);
            
            mNewAreaInfos.push_back(info);
        }
        offsetX = 0;
        mAreaInfos.push_back(vAreas);
    }
}

void ImageSplitFactory::recycle()
{
    mAreaInfos.clear();
    mTileConfigs->removeAllObjects();
    
}

void ImageSplitFactory::resetOriginalPosition(vector<TileSprite*>& sprites, Vec2 &topleft)
{
    for (int i = 0; i < sprites.size(); i++) {
        
        float scale = 0.468;
        sprites.at(i)->setScale(scale);
        
        Vec2 pos = V::getRealPositionForDesignPosition(topleft + mNewAreaInfos.at(i).mOffset * scale);
        
        Size contentSize = sprites.at(i)->getContentSize() * scale;
        pos.x += contentSize.width / 2;
        pos.y -= contentSize.height / 2;
        
        sprites.at(i)->setOriginalPoint(pos);
    }
}

