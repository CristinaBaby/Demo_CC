//
//  ScribbleArmatureNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 14-10-29.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleArmatureNode__
#define __ScribbleDemo_3_2__ScribbleArmatureNode__

#include <stdio.h>
#include "ScribbleTouchNode.h"
#include "cocostudio/CCBone.h"
using namespace cocostudio;
class ScribbleArmatureNode : public ScribbleTouchNode {
public:
    bool init(Size pCanvasSize);
    static ScribbleArmatureNode *create(Size pCanvasSize);
public:
    ScribbleArmatureNode();
    inline void setArmaturePosition(Point pPoint){_armaturePosition = pPoint;}
    inline Point positionInArmature(Point pPoint){
        pPoint.subtract(_armaturePosition);
        return pPoint;
    }
    void paint(Point pWorldPosition) override;
    void paint(Point pStartWorldPosition, Point pEndWorldPosition) override;
    void paint(vector<Point> pWorldPositions) override;
    
public://method for armature animation
    void setAdditionalTransform(Mat4* additionalTransform);
    void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void updateArmatureTransform();
    void updateTransform() override;
    virtual void setSkinData(const BaseData &data);
    virtual const BaseData &getSkinData() const;
    virtual void setBone(Bone *bone);
    virtual Bone *getBone() const;
    
protected:
    Point _armaturePosition;
    BaseData _skinData;
    Bone *_bone;
//    Armature *_armature;
    cocos2d::Mat4 _skinTransform;
};
#endif /* defined(__ScribbleDemo_3_2__ScribbleArmatureNode__) */
