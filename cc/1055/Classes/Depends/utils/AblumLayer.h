//
//  AblumLayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#ifndef AblumLayer_hpp
#define AblumLayer_hpp

#include "cocos2d.h"
#include "StudioLayer.h"
#include "ActionHelper.h"
#include "SSCInternalLibManager.h"
#include "RuntimePermission.h"
#include "RuntimePermissionManager.h"
USING_NS_CC;

using namespace std;

class AblumLayer: public Layer{
public:
    CREATE_FUNC(AblumLayer);
    AblumLayer();
    ~AblumLayer();
    void setRender(RenderTexture* _render);
protected:
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    Image* _image;
    string _shareFilePath;
    StudioLayer* _operate;
      virtual void touchEnd(ui::Widget* widget);
private:
    void saveImage();
    void onPermissionGrantedResult(int requestCode,bool bGranted);
};

#endif /* AblumLayer_hpp */
