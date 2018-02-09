//
//  ScribbleSpineNode.cpp
//  TestGame
//
//  Created by renhong on 16/1/8.
//
//

#include "ScribbleSpineNode.h"
#ifdef SCRIBBLE_USE_SPINE

Map<string, ScribbleSpineNode *> ScribbleSpineNode::_scribbleSpineNodes;

void ScribbleSpineNode::removeScribbleNodeInCache(string pName){
    _scribbleSpineNodes.erase(pName);
}

void ScribbleSpineNode::clearScribbleNodeInCache(string pName){
    _scribbleSpineNodes.clear();
}

ScribbleSpineNode *ScribbleSpineNode::getScribbleNodeIncache(string pName){
    ScribbleSpineNode *lNode = nullptr;
    if (_scribbleSpineNodes.find(pName) != _scribbleSpineNodes.end()) {
        lNode = _scribbleSpineNodes.find(pName)->second;
    }
    return lNode;
}

ScribbleSpineNode *ScribbleSpineNode::create(Size pCanvasSize){
    ScribbleSpineNode *ret = new ScribbleSpineNode();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleSpineNode *ScribbleSpineNode::create(Size pCanvasSize, string pName){
    ScribbleSpineNode *lNode = nullptr;
    if (_scribbleSpineNodes.find(pName) != _scribbleSpineNodes.end()) {
        lNode = _scribbleSpineNodes.find(pName)->second;
    }
    if (nullptr == lNode) {
        lNode = ScribbleSpineNode::create(pCanvasSize);
        if (nullptr != lNode) {
            _scribbleSpineNodes.insert(pName, lNode);
        }
    }
    return lNode;
}

ScribbleSpineNode::ScribbleSpineNode(){
    _worldVertices = nullptr;
    _skeletonAnimation = nullptr;
    _slot = nullptr;
}

ScribbleSpineNode::~ScribbleSpineNode(){
    free(_worldVertices);
    this->setSkeletonAnimation(nullptr);
    this->setSlot(nullptr);
}

bool ScribbleSpineNode::init(Size pCanvasSize){
    if ( !ScribbleTouchNode::init(pCanvasSize) ){
        return false;
    }
    _worldVertices = (float *)malloc(1000 * sizeof(float)); // Max number of vertices per mesh.
    memset(_worldVertices, 0, 1000 * sizeof(float));
    _touchListener->setSwallowTouches(false);
    this->scheduleUpdate();
    return true;
}

void ScribbleSpineNode::displaySkin(){
    switch (this->getSlot()->attachment->type) {
        case SP_ATTACHMENT_REGION:{
            spRegionAttachment* attachment = (spRegionAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            spRegionAttachment_setUVs(attachment, 0, 1, 1, 0, 0);
            break;
        }
        case SP_ATTACHMENT_MESH:{
            spMeshAttachment* attachment = (spMeshAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            attachment->regionU = 0;
            attachment->regionV = 1;
            attachment->regionU2 = 1;
            attachment->regionV2 = 0;
            
            spMeshAttachment_updateUVs(attachment);
            break;
        }
        case SP_ATTACHMENT_SKINNED_MESH:{
            spSkinnedMeshAttachment *attachment = (spSkinnedMeshAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            attachment->regionU = 0;
            attachment->regionV = 1;
            attachment->regionU2 = 1;
            attachment->regionV2 = 0;
            attachment->regionRotate = 0;
            
            spSkinnedMeshAttachment_updateUVs(attachment);
            break;
        }
        default:
            break;
    }
}

spAtlasPage* ScribbleSpineNode::createScribbleAtlasPage(){
    spAtlas* atlas = this->getSkeletonAnimation()->getAtlas();
    Texture2D *imgTex = this->getCanvas()->getSprite()->getTexture();
    
    Size size = imgTex->getContentSize();
    spAtlasPage* page =  atlas->pages;
    spAtlasPage* newPage = spAtlasPage_create(atlas , ScribbleSpineAtlasPage);
    newPage->format = page->format;
    newPage->magFilter = page->magFilter;
    newPage->minFilter = page->minFilter;
    newPage->rendererObject = imgTex;
    newPage->width = size.width;
    newPage->height = size.height;
    newPage->uWrap = page->uWrap;
    newPage->vWrap = page->vWrap;
    
    return newPage;
}

void ScribbleSpineNode::displayScribbleAtlasPage(spAtlasPage* atlasPage, spAtlasRegion* region){
    /*自己替换的图片只有一张*/
    region->height= atlasPage->height;
    region->width = atlasPage->width;
    region->offsetX = 0;
    region->offsetY = 0;
    region->originalHeight = atlasPage->height;
    region->originalWidth = atlasPage->width;
    region->rotate = 0;
    region->u = 0;
    region->v = 0;
    region->u2 = 1;
    region->v2 = 1;
    
    /*自己创建的Page自己释放一下,默认创建的，在骨骼动画释放时会释放*/
    if (region->page &&
        strcmp(region->page->name, ScribbleSpineAtlasPage) == 0){
        FREE(region->page->name);
        FREE(region->page);
    }
    region->page = atlasPage;
}

void ScribbleSpineNode::update(float dt){
    if (nullptr == this->getSlot()) {
        return;
    }
    if (nullptr == this->getSkeletonAnimation()) {
        return;
    }
    switch (this->getSlot()->attachment->type) {
        case SP_ATTACHMENT_REGION:{
            spRegionAttachment* attachment = (spRegionAttachment*)this->getSlot()->attachment;
            spRegionAttachment_computeWorldVertices(attachment, this->getSlot()->bone, _worldVertices);
            float lAnimationX = this->getSkeletonAnimation()->getPositionX();
            float lAnimationY = this->getSkeletonAnimation()->getPositionY();
            //左下角
            float lbX = _worldVertices[SP_VERTEX_X1] + lAnimationX;
            float lbY = _worldVertices[SP_VERTEX_Y1] + lAnimationY;
            //左上角
            float ltX = _worldVertices[SP_VERTEX_X2] + lAnimationX;
            float ltY = _worldVertices[SP_VERTEX_Y2] + lAnimationY;
            //右上角
            float rtX = _worldVertices[SP_VERTEX_X3] + lAnimationX;
            float rtY = _worldVertices[SP_VERTEX_Y3] + lAnimationY;
            //右下角
            float rbX = _worldVertices[SP_VERTEX_X4] + lAnimationX;
            float rbY = _worldVertices[SP_VERTEX_Y4] + lAnimationY;
            
            
            Vec2 lWorldPos((ltX + rbX) / 2.0, (ltY + rbY) / 2.0);
            this->setPosition(this->getParent()->convertToNodeSpace(lWorldPos));
            this->setScale(this->getSlot()->bone->scaleX);
            float lDeltaY = rtY - rbY;
            float lDeltaX = rbX - rtX;
            float lDeltaAngle = 0;
            if (lDeltaY > 0) {//一二象限
                if (lDeltaX > 0) {//一象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//二象限
                    lDeltaAngle = 90;
                }
                else{//+Y轴
                    lDeltaAngle = -90;
                }
            }
            else if (lDeltaY < 0){//三四象限
                if (lDeltaX > 0) {//四象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//三象限
                    lDeltaAngle = 90;
                }
                else{//-Y轴
                    lDeltaAngle = 90;//没有测试数据，暂未验证此数据的准确性。
                }
            }
            else{//X轴
                //lDeltaAngle = 0;//没有测试数据，暂未验证此数据的准确性。
            }
            
            Vec2 lX(1, 0);
            Vec2 lV(lDeltaX, lDeltaY);//(slot->rtX - slot->rbX,slot->rtY - slot->rbY);
            /*
             //尝试使用向量计算来求cos角度
             float lCosTheta = lX.dot(lV) / (sqrt(lX.x * lX.x + lX.y * lX.y) * sqrt(lV.x * lV.x + lV.y * lV.y));
             float lR2 = CC_RADIANS_TO_DEGREES(acos(lCosTheta));
             */
            float lR = CC_RADIANS_TO_DEGREES(atan(lDeltaY / lDeltaX)) +  lDeltaAngle;
            
            //    CCLOG("s->bone->rotation %f %f %f\n", lR, lR2, lCosTheta);
            this->setRotation(lR);//lR2 + 90  lR2+90+180
            
            break;
        }
        case SP_ATTACHMENT_MESH:{
            float lAnimationX = this->getSkeletonAnimation()->getPositionX();
            float lAnimationY = this->getSkeletonAnimation()->getPositionY();
            
            spMeshAttachment* attachment = (spMeshAttachment*)this->getSlot()->attachment;
            spMeshAttachment_computeWorldVertices(attachment, this->getSlot(), _worldVertices);
            
            
            int verticesCount = attachment->verticesCount;
            
            float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
            float scaleX = this->getSkeletonAnimation()->getScaleX(), scaleY = this->getSkeletonAnimation()->getScaleY();
            
            for (int ii = 0; ii < verticesCount; ii += 2) {
                float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
                minX = min(minX, x);
                minY = min(minY, y);
                maxX = max(maxX, x);
                maxY = max(maxY, y);
            }
            
            Vec2 position = Vec2(lAnimationX - this->getContentSize().width / 2, lAnimationY);
            Rect lBondingBox = Rect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
            float lOffsetX = lBondingBox.size.width / 2;
            
            //左下角
            float lbX = lBondingBox.origin.x;
            float lbY = lBondingBox.origin.y;
            
            //            _lb->setPosition(Vec2(lbX + lOffsetX, lbY));
            
            //左上角
            float ltX = lBondingBox.origin.x;
            float ltY = lBondingBox.origin.y + lBondingBox.size.height;
            
            //            _lt->setPosition(Vec2(ltX + lOffsetX, ltY));//Vec2(ltX, ltY)
            
            //右上角
            float rtX = lBondingBox.origin.x + lBondingBox.size.width;
            float rtY = lBondingBox.origin.y + lBondingBox.size.height;
            //            _rt->setPosition(Vec2(rtX + lOffsetX, rtY));
            
            //右下角
            float rbX = lBondingBox.origin.x + lBondingBox.size.width;
            float rbY = lBondingBox.origin.y;
            //            _rb->setPosition(Vec2(rbX + lOffsetX, rbY));
            
            Vec2 lWorldPos((ltX + rbX) / 2.0 + this->getContentSize().width / 2, (ltY + rbY) / 2.0);
            this->setPosition(this->getParent()->convertToNodeSpace(lWorldPos));
            
            this->setScaleX(this->getSlot()->bone->scaleX);
            this->setScaleY(this->getSlot()->bone->scaleY);
            float lDeltaY = rtY - rbY;
            float lDeltaX = rbX - rtX;
            float lDeltaAngle = 0;
            if (lDeltaY > 0) {//一二象限
                if (lDeltaX > 0) {//一象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//二象限
                    lDeltaAngle = 90;
                }
                else{//+Y轴
                    lDeltaAngle = -90;
                }
            }
            else if (lDeltaY < 0){//三四象限
                if (lDeltaX > 0) {//四象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//三象限
                    lDeltaAngle = 90;
                }
                else{//-Y轴
                    lDeltaAngle = 90;//没有测试数据，暂未验证此数据的准确性。
                }
            }
            else{//X轴
                //lDeltaAngle = 0;//没有测试数据，暂未验证此数据的准确性。
            }
            
            Vec2 lX(1, 0);
            Vec2 lV(lDeltaX, lDeltaY);//(slot->rtX - slot->rbX,slot->rtY - slot->rbY);
            
            float lR = CC_RADIANS_TO_DEGREES(atan(lDeltaY / lDeltaX)) +  lDeltaAngle;
            
//            CCLOG("s->bone->rotation %f\n", lR);
            this->setRotation(lR);//lR2 + 90  lR2+90+180
            
            break;
        }
        case SP_ATTACHMENT_SKINNED_MESH:{
            
            float lAnimationX = this->getSkeletonAnimation()->getPositionX();
            float lAnimationY = this->getSkeletonAnimation()->getPositionY();
            
            spSlot *slot = this->getSlot();
            spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)slot->attachment;
            spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
            
            int verticesCount = mesh->uvsCount;
            
            float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
            float scaleX = this->getSkeletonAnimation()->getScaleX(), scaleY = this->getSkeletonAnimation()->getScaleY();
            
            for (int ii = 0; ii < verticesCount; ii += 2) {
                float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
                minX = min(minX, x);
                minY = min(minY, y);
                maxX = max(maxX, x);
                maxY = max(maxY, y);
            }
            
            Vec2 position = Vec2(lAnimationX - this->getContentSize().width / 2, lAnimationY);
            Rect lBondingBox = Rect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
            float lOffsetX = lBondingBox.size.width / 2;
            
            //左下角
            float lbX = lBondingBox.origin.x;
            float lbY = lBondingBox.origin.y;
            
//            _lb->setPosition(Vec2(lbX + lOffsetX, lbY));
            
            //左上角
            float ltX = lBondingBox.origin.x;
            float ltY = lBondingBox.origin.y + lBondingBox.size.height;
            
//            _lt->setPosition(Vec2(ltX + lOffsetX, ltY));//Vec2(ltX, ltY)
            
            //右上角
            float rtX = lBondingBox.origin.x + lBondingBox.size.width;
            float rtY = lBondingBox.origin.y + lBondingBox.size.height;
//            _rt->setPosition(Vec2(rtX + lOffsetX, rtY));
            
            //右下角
            float rbX = lBondingBox.origin.x + lBondingBox.size.width;
            float rbY = lBondingBox.origin.y;
//            _rb->setPosition(Vec2(rbX + lOffsetX, rbY));
            
            Vec2 lWorldPos((ltX + rbX) / 2.0 + this->getContentSize().width / 2, (ltY + rbY) / 2.0);
            this->setPosition(this->getParent()->convertToNodeSpace(lWorldPos));
            
            this->setScaleX(this->getSlot()->bone->scaleX);
            this->setScaleY(this->getSlot()->bone->scaleY);
            float lDeltaY = rtY - rbY;
            float lDeltaX = rbX - rtX;
            float lDeltaAngle = 0;
            if (lDeltaY > 0) {//一二象限
                if (lDeltaX > 0) {//一象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//二象限
                    lDeltaAngle = 90;
                }
                else{//+Y轴
                    lDeltaAngle = -90;
                }
            }
            else if (lDeltaY < 0){//三四象限
                if (lDeltaX > 0) {//四象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//三象限
                    lDeltaAngle = 90;
                }
                else{//-Y轴
                    lDeltaAngle = 90;//没有测试数据，暂未验证此数据的准确性。
                }
            }
            else{//X轴
                //lDeltaAngle = 0;//没有测试数据，暂未验证此数据的准确性。
            }
            
            Vec2 lX(1, 0);
            Vec2 lV(lDeltaX, lDeltaY);//(slot->rtX - slot->rbX,slot->rtY - slot->rbY);
            
            float lR = CC_RADIANS_TO_DEGREES(atan(lDeltaY / lDeltaX)) +  lDeltaAngle;
            
//            CCLOG("s->bone->rotation %f\n", lR);
            this->setRotation(lR);//lR2 + 90  lR2+90+180
            
            break;
        }
    }
}
#endif