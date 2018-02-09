//
//  CocosHelper.h
//
//  Created by tanshoumei on 14-9-12.
//
//

#ifndef __CocosHelper__
#define __CocosHelper__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
//#include "cocostudio/Cocostudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class CocosHelper
{
public:
    /**
     *@brief 设置 parent 结点下 Button 类的回调
     *@note 该方法只会向下寻找一层
     */
    static void addTouchEventToChildButtons(Node* parent, Widget::ccWidgetTouchCallback callback);
    
    /**
     *@brief 从Aniamte中获取Sprite
     *@param frameIndex: 帧索引，返回相应的帧生成Sprite。
     */
    static Sprite* getSpriteFromAnimate(const Animate* animate, const int frameIndex = 0);
    
    /**
     *@brief 实例化 Sprite 对象，不添加纹理缓存
     */
    static Sprite* createWithNoCache(const string &name);
    
    /**
     *@brief 根据 fileName 添加精灵到指定结点 (不添加纹理缓存)
     */
    static Sprite* addSpriteNoCache(const string &fileName, Node* parentNode, const int zOrder, const int tag);
    
    /**
     *@brief 根据sprite file name添加精灵到指定位置
     */
    static Sprite* AddSprite(const string &fileName, Node* parentNode, const int zOrder, const int tag);
    
    /**
     *@brief 根据sprite frame name添加精灵到指定位置
     */
    static Sprite* AddSpriteByFrame(const string &fileName, Node* parentNode, const int zOrder, const int tag);
    
    /**
     *@brief 根据一张图，生成一个按钮item,其选中效果为变暗，和UIButton差不多
     */
    static MenuItemSprite* menuItemSprite(const string& fileName, ccMenuCallback pCallback = nullptr);
    static MenuItemSprite* menuItemSprite(const string& norFile, const string& selFile, ccMenuCallback pCallback = nullptr);
    
    /**
     *@brief 将一个node从当前父节点中移除，并加入到新的父节点中，保持界面上同一位置
     */
    static void convertToNewParent(Node* node, Node* parentNode);
    
    /**
     *@brief 通过plist和对应的png创建帧动画，frameName从1开始
     *@param fameName 文件中每一帧的名字前缀
     *@param num 图片数
     */
    static Animate* createAnimate(string frameName, int num, float delay = 0.2, unsigned int loops = 1);
    
    static ParticleSystemQuad* showParticle(std::string path,Vec2 location=Vec2::ZERO);
public:
    /**
     *@brief 判断点是否在 node 之内，可用于touch判断
     */
    static bool isPointInNode(Node* node, const Vec2& worldPoint);
    
    /**
     *@brief 获取 node 的Rect
     */
    static Rect getNodeRectInWorld(Node* node);
    
    /**
     *@brief 获取contentSize * 0.5
     */
    static Vec2 getNodeCenterLocal(Node* node);
    
public:
    /**
     *@brief 当前系统时间(毫秒)
     */
    static long milliSecondNow();
    
    /**
     *@brief 将指定字符串(str)，按分隔符(默认为"-")进行分割，分隔的字符串放于vOut中
     */
    static void splitString(const std::string str, vector<std::string>& vOut, const char* splitChar = "-");
    
    /**
     *@brief Vec2 to string
     */
    static string pointToString(const Vec2& point);
    
    /**
     *@brief Rect to string
     */
    static string rectToString(const Rect& rect);
    
    /**
     *@brief 将字符串转换成Rect，可设置数字间分隔符，默认为","
     */
    static Rect stringToRect(std::string str, const char* sp = ",");
    
    /**
     *@brief 将字符串转换成Vec2，可设置数字间分隔符，默认为","
     */
    static Vec2 stringToPoint(std::string str, const char* sp = ",");
    
    /**
     *@brief 将字符串转换成ccColor3B，可设置数字间分隔符，默认为","
     */
    static Color3B stringToColor3B(std::string str, const char* sp = ",");
};

#endif /* defined(__CocosHelper__) */
