//
//  PicHelp.cpp
//  IceCreamSundae
//
//  Created by luotianqiang on 16/5/29.
//
//

#include "PicHelp.h"


void PicHelp::caluteImage(RenderTexture* _render,vector<int>& maxYs,vector<int> &minYs){
    maxYs.clear();
    minYs.clear();
//    auto pos = node->getPosition();
//    auto size = node->getContentSize()+Size(0,80);
//    auto _render = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
//    _render->begin();
//    node->setPosition(Vec2::ZERO);
//    node->visit();
//    _render->end();
//    Director::getInstance()->getRenderer()->render();
//    node->setPosition(pos);
    
    auto pImage = _render->newImage(true);
     auto *lData = pImage->getData();
  
    int H = pImage->getHeight();
    int W = pImage->getWidth();
    
    for (int i = 0; i < W; ++i) {
        int maxY = 0;
        int minY = W;
        for (int j = 0; j < H; ++j) {
            auto lPixcelIndex = j*W+i;
            auto lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 150) {
                if(minY == W)
                    minY = j;
                if(maxY<j)
                    maxY = j;
            }
        }
        if(maxY<minY){
            minYs.push_back(0);
            maxYs.push_back(0);
        }else {
        minYs.push_back(H-maxY);
        maxYs.push_back(H-minY);
        }
    }
    pImage->release();
}