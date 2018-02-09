//
//  VoteOperate.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#ifndef VoteOperate_hpp
#define VoteOperate_hpp

#include "LQComponent.h"
#include "StudioLayer.h"
#include "ImageLabel.h"
class VoteOperate: public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(VoteOperate);
public:
    VoteOperate();
    virtual bool init();
    virtual void onAdd();
    void touchBtn(ui::Widget*);
    
    void showLovers();
    void setVotePath(const vector<string>&);
private:
    Vector<Node*> vote_texs;
    Vector<Sprite*> voteLayers;
    vector<string> votePaths;
    vector<Vec2> voteLayerPos;
    int curentNum;
    int endNum;
    int winIndex;
};

#endif /* VoteOperate_hpp */
