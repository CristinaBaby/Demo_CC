//
//  CocosHelper3.cpp
//  HGirl101_MyNewDress
//
//  Created by tanshoumei on 14-9-12.
//
//

#include "CocosHelper.h"
#include "CMVisibleRect.h"

void CocosHelper::addTouchEventToChildButtons(Node *aParentWidge, Widget::ccWidgetTouchCallback callback)
{
    if(aParentWidge)
    {
        for(auto& child: aParentWidge->getChildren())
        {
            Button* lpButton = dynamic_cast<Button*>(child);
            if(lpButton)
            {
                lpButton->addTouchEventListener(callback);
            }
        }
    }
}

Sprite* CocosHelper::getSpriteFromAnimate( const Animate* apAni, const int aFrameInx /*= 0*/)
{
    Sprite* pRet = NULL;
    if( apAni )
    {
        auto frames = apAni->getAnimation()->getFrames();
        if(frames.size() > aFrameInx)
        {
            pRet = Sprite::createWithSpriteFrame(frames.at(aFrameInx)->getSpriteFrame());
        }
    }
    return pRet;
}

Sprite* CocosHelper::createWithNoCache(const string &name)
{
    Image* pImg = new Image;
    pImg->initWithImageFile(name);
    Texture2D* pTex = new Texture2D;
    pTex->initWithImage(pImg);
    Sprite* pSp = Sprite::createWithTexture(pTex);
    pImg->release();
    pTex->release();
    return pSp;
}

Sprite* CocosHelper::addSpriteNoCache(const string &fileName, cocos2d::Node *parentNode, const int zOrder, const int tag)
{
    Sprite* sp = createWithNoCache(fileName);
    parentNode->addChild(sp, zOrder, tag);
    return sp;
}

Sprite* CocosHelper::AddSprite(const string &fileName, Node* parentNode, const int zOrder, const int tag)
{
    Sprite* sp = Sprite::create(fileName);
    parentNode->addChild(sp, zOrder, tag);
    return sp;
}

Sprite* CocosHelper::AddSpriteByFrame(const string &fileName, Node* parentNode, const int zOrder, const int tag)
{
    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
    if (frame)
    {
        Sprite* sp = Sprite::createWithSpriteFrame(frame);
        parentNode->addChild(sp, zOrder, tag);
        return sp;
    }
    return nullptr;
}

MenuItemSprite* CocosHelper::menuItemSprite(const string& aName, ccMenuCallback pCallback/*= nullptr*/)
{
    Sprite* norSp = Sprite::create(aName);
    Sprite* selSp = Sprite::create(aName);
    Sprite* disSp = Sprite::create(aName);
    selSp->setColor(Color3B::GRAY);
    disSp->setColor(Color3B::GRAY);
    MenuItemSprite* item = MenuItemSprite::create(norSp, selSp, disSp, pCallback);
    return item;
}

MenuItemSprite* CocosHelper::menuItemSprite(const string &norFile, const string &selFile, ccMenuCallback pCallback/*= nullptr*/)
{
    Sprite* norSp = Sprite::create(norFile);
    Sprite* selSp = Sprite::create(selFile);
    MenuItemSprite* pItem = MenuItemSprite::create(norSp, selSp);
    return pItem;
}

void CocosHelper::convertToNewParent(Node *node, Node *parentNode)
{
    if(!node || !(node->getParent()) || !parentNode)
        return;
    node->retain();
    Vec2 ptWorld = node->getParent()->convertToWorldSpace(node->getPosition());
    Vec2 ptLocal = parentNode->convertToNodeSpace(ptWorld);
    node->removeFromParentAndCleanup(false);
    parentNode->addChild(node);
    node->setPosition(ptLocal);
    node->release();
}

Animate* CocosHelper::createAnimate(string frameName, int num, float delay/*=0.2*/, unsigned int loops/*=1*/)
{
    Vector<SpriteFrame*> v_Aniframes;
    char s_frameName[50] = {0};
    for (int i = 1; i <= num; ++i)
    {
        sprintf(s_frameName, "%d.png", i);
        SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName + s_frameName);
        v_Aniframes.pushBack(pFrame);
    }
    Animation* frameAnimation = Animation::createWithSpriteFrames(v_Aniframes, delay, loops);
    return Animate::create(frameAnimation);
}

ParticleSystemQuad* CocosHelper::showParticle(std::string path,Vec2 location)
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create(path);
    pParticle->setPosition(location);
    return pParticle;
    
}
bool CocosHelper::isPointInNode(Node* apNode, const Vec2& aPtWorld)
{
    if(apNode)
    {
        Rect rect;
        rect.origin = apNode->convertToWorldSpace(Point::ZERO);
        rect.size = apNode->getContentSize();
        if(rect.containsPoint(aPtWorld))
            return true;
    }
    return false;
}

Rect CocosHelper::getNodeRectInWorld(Node* node)
{
    Rect rect;
    rect.origin = node->convertToWorldSpace(Vec2::ZERO);
    rect.size = node->getContentSize();
    return rect;
}

Vec2 CocosHelper::getNodeCenterLocal(Node *node)
{
    Vec2 pt;
    if (node)
    {
        pt = node->getContentSize() * 0.5;
    }
    return pt;
}

long CocosHelper::milliSecondNow()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void CocosHelper::splitString(const std::string str, vector<std::string> &vOut, const char* splitChar /*= '-' */)
{
    vOut.clear();
    const char *tokenPtr = strtok((char*)str.c_str(), splitChar);
    while (NULL != tokenPtr)
    {
        vOut.push_back(tokenPtr);
        tokenPtr = strtok(NULL, splitChar);
    }
}

string CocosHelper::pointToString(const Vec2 &point)
{
    char str[20] = {0};
    sprintf(str, "(%.2f, %.2f)", point.x, point.y);
    return str;
}

string CocosHelper::rectToString(const Rect &rect)
{
    char str[30] = {0};
    sprintf(str, "(%.2f, %.2f, %.2f, %.2f)", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    return str;
}

Rect CocosHelper::stringToRect(std::string aStr, const char* sp/*=","*/)
{
    int nums[4] = {0};
    const char *tokenPtr = strtok((char*)aStr.c_str(), sp);
    int inx = 0;
    while (NULL != tokenPtr && inx < 4)
    {
        nums[inx++] = atoi(tokenPtr);
        tokenPtr = strtok(NULL, sp);
    }
    return Rect(nums[0], nums[1], nums[2], nums[3]);
}

Vec2 CocosHelper::stringToPoint(std::string aStr, const char* sp/*=","*/)
{
    int nums[2] = {0};
    const char *tokenPtr = strtok((char*)aStr.c_str(), sp);
    int inx = 0;
    while (NULL != tokenPtr && inx < 2)
    {
        nums[inx++] = atoi(tokenPtr);
        tokenPtr = strtok(NULL, sp);
    }
    return Vec2(nums[0], nums[1]);
}

Color3B CocosHelper::stringToColor3B(std::string aStr, const char* sp /*= ","*/)
{
    int nums[3] = {0};
    const char *tokenPtr = strtok((char*)aStr.c_str(), sp);
    int inx = 0;
    while (NULL != tokenPtr && inx < 3) {
        nums[inx++] = atoi(tokenPtr);
        tokenPtr = strtok(NULL, sp);
    }
    return Color3B(nums[0], nums[1], nums[2]);
}
