//
//  JigsawTileSprite.h
//  BedtimeStory
//
//  Created by maxiang on 9/11/15.
//
//

#ifndef __BedtimeStory__JigsawTileSprite__
#define __BedtimeStory__JigsawTileSprite__

#include "cocos2d.h"
#include "AppConfigs.h"

class JigsawTileSprite : public cocos2d::Sprite
{
public:
    static JigsawTileSprite* createWithTexture(cocos2d::Texture2D *texture);

    void setTileParent(JigsawTileSprite *parent){_tileParent = parent;};
    JigsawTileSprite *getTileParent(){return _tileParent;};
    
    void addTileChild(JigsawTileSprite *child){_tileChildrenVector.push_back(child);};
    void removeTileChild(JigsawTileSprite *child);
    std::vector<JigsawTileSprite*>& getTileChildren(){return _tileChildrenVector;};
    
    void addLinkedTileChild(JigsawTileSprite *child){_linkedTileSpriteVector.push_back(child);};
    std::vector<JigsawTileSprite*>& getLinkedTileChildren(){return _linkedTileSpriteVector;};

    CC_SYNTHESIZE(bool, _isFixed, IsFixed);
    CC_SYNTHESIZE(bool, _isCheckedLink, IsCheckedLink);

protected:
    JigsawTileSprite():
    _isFixed(false),
    _tileParent(nullptr),
    _isCheckedLink(false)
    {
    };
    
    ~JigsawTileSprite();
    
    JigsawTileSprite *_tileParent;
    std::vector<JigsawTileSprite*> _tileChildrenVector;
    std::vector<JigsawTileSprite*> _linkedTileSpriteVector;
};


#endif /* defined(__BedtimeStory__JigsawTileSprite__) */
