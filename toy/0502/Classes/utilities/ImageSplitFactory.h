//
//  ImageSplitFactory.h
//  cocosTest
//
//  Created by zhangguangzong1 on 2/12/14.
//
//

#ifndef __cocosTest__ImageSplitFactory__
#define __cocosTest__ImageSplitFactory__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "../Jigsaw/AreaInfo.h"
#include "../widgets/TileSprite.h"

USING_NS_CC;
using namespace std;


 static float baseTileSize = 96.4;
/*
 按照切图顺序我们只考虑右偏移和下偏移，在这种前提下，准确的理解偏移，应该这样说：右偏移：
 从图片最左边开始，超过拼图块单位长度（在给你的程序中，单位长度就是104x104就是黑色部分）的部分是偏移部分。

 每个方块
 */
class ImageSplitFactory {
    
protected:
    ImageSplitFactory();
    virtual ~ImageSplitFactory();
public:
    static ImageSplitFactory* instance;
    static ImageSplitFactory* sharedImageSplitFactory();
    static void destoryInstance();
   
public:
    float mScale;
public:
    /*拼图块的配置信息*/
    CCArray* mTileConfigs;
    /*图片切分后的各区域信息*/
    vector<vector<AreaInfo> > mAreaInfos;
    vector<AreaInfo> mNewAreaInfos;
public:
    void resetOriginalPosition(vector<TileSprite*>& sprites, Vec2 &topleft);
    
    /*加载拼图块信息*/
    void loadTileConfigs(const char* csvPath);
    
    /**
     * @brief 程序内部计算图片切分数据
     * @param size 待切分的尺寸大小 如：3->3x3,4->4x4
     */
    void loadSplitByCalculate(int size);
    
//    void addClippingSprite(const char* imagePath,vector<ClippingSprite*>& v , Node *parent);
    
    void recycle();
    
    /**
     * @brief 切分图片
     * @param imagePath 待切分的图片路径
     * @param v         切分后的拼图块精灵存放在此二维向量中
     * @param isRetain  是否retain向量中的精灵
     */
    void splitImage(const char* imagePath,vector<TileSprite*>& v,bool isRetain = false);
    
    
    void loadRenderSplitByConfigs(const char* csvPath);
    /*计算缩放值*/
    void calculateScale(int size);
    
    //图片的位置
    int pictureX;
    int pictureY;
   
    int row ;
    int column;
};

#endif /* defined(__cocosTest__ImageSplitFactory__) */
