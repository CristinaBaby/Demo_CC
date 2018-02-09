//
//  CocosHelper.cpp
//  LibDoctor
//
//  Created by tanshoumei on 13-10-16.
//
//

#include "CocosHelper.h"
//#include "CCImageColorSpace.h"

Sprite* CocosHelper::spriteCreateWithNoCache(const char* pName)
{
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(pName);
    if (fullpath.size() == 0)
            return nullptr;
    Sprite* pSpr = nullptr;
    Image* pimg = nullptr;
    Texture2D* pTex  = nullptr;
    do {
        pimg = new Image;
        CC_BREAK_IF(pimg == nullptr);
        bool b = pimg->initWithImageFile(pName);
        CC_BREAK_IF(!b);
        pTex = new Texture2D;
        CC_BREAK_IF(pTex == nullptr);
        b = pTex->initWithImage(pimg);
        CC_BREAK_IF(!b);
        pSpr = Sprite::createWithTexture(pTex);
    } while (false);
    CC_SAFE_RELEASE_NULL(pimg);
    CC_SAFE_RELEASE_NULL(pTex);
    return pSpr;
}

//void CocosHelper::AddNode(Node* apNode, Node* apParent, const Point& aPt, const int aZorder, const int aTag)
//{
//    if(apNode && apParent)
//    {
//        apParent->addChild(apNode, aZorder, aTag);
//        apNode->setPosition(aPt);
//    }
//}

void CocosHelper::AddNodeAutoAdapte(Node* apNode, Node* apParent, const Point& aPt, const int aZorder, const int aTag)
{
//    if(apNode && apParent)
//    {
//        apParent->addChild(apNode, aZorder, aTag);
//        CMVisibleRect::setPosition(apNode, aPt.x, aPt.y);
//    }
}

//根据sprite file name添加精灵到指定位置
Sprite* CocosHelper::AddSprite( const char* apFileName, Node* apParent, const int aZorder, const int aTag)
{
    Sprite *spr = Sprite::create(apFileName);
    apParent->addChild(spr, aZorder, aTag);
    return spr;
}

//根据sprite file name添加精灵到指定位置.（不会将texture缓存）
Sprite* CocosHelper::AddSpriteNoCache( const char* apFileName, Node* apParent, const int aZorder, const int aTag)
{
    Sprite *spr = spriteCreateWithNoCache(apFileName);
    apParent->addChild(spr, aZorder, aTag);
    return spr;
}

//根据sprite frame name添加精灵到指定位置
Sprite* CocosHelper::AddSpriteByFrame( const char* apFileName, Node* apParent, const int aZorder, const int aTag)
{
    SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(apFileName);
    if(pFrame)
    {
        Sprite* spr = Sprite::createWithSpriteFrame(pFrame);
        apParent->addChild(spr, aZorder, aTag);
        return spr;
    }
    return NULL;
}

//将字符串转换成Rect，可设置数字间分隔符，默认为","
Rect CocosHelper::stringToRect(std::string aStr, const char* sp /*= ","*/)
{
      string _newString(aStr.c_str());
    int nums[4] = {0};
    const char *tokenPtr = strtok((char*)_newString.c_str(), sp);
    int inx = 0;
    while (NULL != tokenPtr && inx < 4) {
        nums[inx++] = atoi(tokenPtr);
        tokenPtr = strtok(NULL, sp);
    }
    return Rect(nums[0], nums[1], nums[2], nums[3]);
}

//将字符串转换成Point，可设置数字间分隔符，默认为","
Point CocosHelper::stringToPoint(std::string aStr, const char* sp /*= ","*/)
{
          string _newString(aStr.c_str());
    float nums[2] = {0};
    const char *tokenPtr = strtok((char*)_newString.c_str(), sp);
    int inx = 0;
    while (NULL != tokenPtr && inx < 2) {
        nums[inx++] = atof(tokenPtr);
        tokenPtr = strtok(NULL, sp);
    }
    return Point(nums[0], nums[1]);
}

 vector<int> CocosHelper::stringToInt(std::string aStr,const char* sp) {
     vector<int>indexs;
     string _newString(aStr.c_str());
     const char* tokenPtr = strtok((char*)_newString.c_str(), sp);
     while (nullptr != tokenPtr) {
         indexs.push_back(atoi(tokenPtr));
         tokenPtr = strtok(nullptr, sp);
     }
     return indexs;
}

vector<Vec2> CocosHelper::stringToPoints(std::string aStr) {
     string _newString(aStr.c_str());
    vector<Vec2> points;
    char * buf = (char*)_newString.c_str();
    char *outer_ptr = nullptr;
    char *inner_ptr = nullptr;
    char *p = strtok_r(buf,";",&outer_ptr);
    while(p!= nullptr) {
        buf=p;
        char *q = strtok_r(buf,",",&inner_ptr);
         float nums[2] = {0};
         int  inx = 0;
        while(q!=nullptr) {
            nums[inx++] = atof(q);
            buf=nullptr;
            q = strtok_r(buf,",",&inner_ptr);
        }
        points.push_back(Vec2(nums[0],nums[1]));
        buf=nullptr;
        p = strtok_r(buf,";",&outer_ptr);
    }
    return points;
}

//判断点是否在node之内，可用于touch判断
bool CocosHelper::isPointInNode(Node* apNode, const Point& aPtWorld)
{
    if(apNode)
    {
        auto p = apNode->convertToNodeSpace(aPtWorld);
        Rect rect;
        rect.origin = Vec2::ZERO;
        rect.size = apNode->getContentSize();
        if(rect.containsPoint(p))
            return true;
    }
    return false;
}

Rect CocosHelper::getNodeRectInWorld(Node* apNode)
{
    Rect rect;
    rect.origin = apNode->convertToWorldSpace(Point::ZERO);
    rect.size = apNode->getContentSize();
    return rect;
}

//node content rect center
Point CocosHelper::getNodeCenterLocal(Node* apNode)
{
    Point pt;
    if(apNode){
        pt.x = apNode->getContentSize().width * .5;
        pt.y = apNode->getContentSize().height * .5;
    }
    return pt;
}

 MenuItemSprite* CocosHelper::menuItemScaleSprite(const char* aName) {
     Sprite* normalSpr = Sprite::create(aName);
     Sprite* selectSpr = Sprite::create(aName);
     selectSpr->setScale(1.1f);
     MenuItemSprite* item = MenuItemSprite::create(normalSpr, selectSpr);
     return item;
}

//根据一张图，生成一个按钮item,其选中效果为变暗，和UIButton差不多
MenuItemSprite* CocosHelper::menuItemSprite(const char* aName)
{
    Sprite* normalSpr = Sprite::create(aName);
    Sprite* selectSpr = Sprite::create(aName);
    selectSpr->setColor(Color3B::GRAY);
    MenuItemSprite* item = MenuItemSprite::create(normalSpr, selectSpr);
    return item;
}

void CocosHelper::AddNode(Node* apNode, Node* apParent, const Point& aPt, const int aZorder, const int aTag)
{
    if(apNode && apParent)
    {
        apParent->addChild(apNode, aZorder, aTag);
        apNode->setPosition(aPt);
    }
}


Point CocosHelper::change(Node *apNode, Point p)
{
       return p -apNode->getPosition() + Point(apNode->getContentSize().width/2.0f, apNode->getContentSize().height/2.0f);

}

Sprite* CocosHelper::createByhsv(Image* pImage, int h,int s,int v){
//    CCImageColorSpace cspace;
//    auto newImage =  cspace.imageWithHSB(pImage, h, s,v);
//    auto tex = new Texture2D();
//    tex->initWithImage(newImage);
//    auto pSprite = Sprite::createWithTexture(tex);
//    CC_SAFE_RELEASE_NULL(tex);
//    return pSprite;
    return nullptr;
}

int CocosHelper::getPixelAt(const Point p,Node* node)
{
    int o = 0;
    auto ornA = node->getAnchorPoint();
    auto po = node->getPosition();
    
    node->setPosition(-p+Vec2(1,1));
    auto render = RenderTexture::create(2,2,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    render->begin();
    node->visit();
    render->end();
    Director::getInstance()->getRenderer()->render();
    node->setPosition(po);
    node->setAnchorPoint(ornA);
    auto image = render->newImage();
    unsigned char * data = image->getData();
    
    for(int i =0;i<sizeof(data)/sizeof(char);i+=4) {
        if(data[i+3]>0) {
            o = data[i+3];
            break;
        }
    }
    
    CC_SAFE_RELEASE_NULL(image);
    log("float o== %d",o);
    return o;
}

void CocosHelper::reAddNode(Node* _node,Node*_newParent){
    if(_node != nullptr && _newParent!= nullptr){
        Vec2 _newPoint;
        if(_node->isIgnoreAnchorPointForPosition())
            _node->convertToWorldSpace(Vec2::ZERO);
         else
            _newPoint = _node->convertToWorldSpace(_node->getAnchorPointInPoints());
        _newPoint = _newParent->convertToNodeSpace(_newPoint);
        _node->retain();
        _node->removeFromParentAndCleanup(false);
        _node->setPosition(_newPoint);
        _newParent->addChild(_node);
        _node->release();
    }
}

void CocosHelper::visitAllNode( Node* _root ,std::function<void( Node*)> _callBack){
    if(_root != nullptr && _callBack != nullptr){
        _callBack(_root);
        for(const auto& _child:_root->getChildren())
            visitAllNode(_child,_callBack);
    }
}


