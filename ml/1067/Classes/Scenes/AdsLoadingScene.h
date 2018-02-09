
#ifndef __AdsLoadingScene__
#define __AdsLoadingScene__

#include "AdLoadingLayerBase.h"

class AdsLoadingScene : public AdLoadingLayerBase {
public:
    
    CREATE_FUNC(AdsLoadingScene);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    void loading();
private:
};

#endif /* defined(__AdsLoadingScene__) */
