//
//  ComponetFade.h
//  ccats1061
//
//  Created by liji on 16/8/8.
//
//

#ifndef ComponetFade_h
#define ComponetFade_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

class ComponetFade : public ComponentBase
{
public:
    
    static ComponetFade *create(string pathBefore,string pathAfter);
    bool init(string pathBefore,string pathAfter);
    int getFadeDuration(){return _fadeDuaration;};
    void setFadeDuration(float fadeDuaration)
    {
        _fadeDuaration = fadeDuaration;
    };
    
    bool getIsFadeOut()
    {
        return _isFadeOut;
    };
    
    void onAdd();
    
    virtual void start();
    
    virtual void pause();
    
    virtual void resume();
    
    virtual void stop();

    function<void()> _fadeDoneCallback = nullptr;
    
private:
    Sprite* _owner = nullptr;
    Sprite* _imageBefore = nullptr;
    int _fadeDuaration = 3.f;
    bool _isFadeOut = false;
    string _pathBefore;
    string _pathAfter;
    
};

#endif /* ComponetFade_h */
