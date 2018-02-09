//
//  DialogLayer.hpp
//
//
//  Created by maxiang on 2/23/16.
//
//

#ifndef DialogLayer_hpp
#define DialogLayer_hpp

#include "cocos2d.h"
#include "ui/cocosGUI.h"

class DialogLayer : public cocos2d::Layer
{
public:
    static DialogLayer* showWithMessage(const std::string& message,
                                const std::function<void()>& okCallback,
                                const std::function<void()>& noCallback,
                                cocos2d::Node *container,
                                const float fontSize = 40.0,
                                bool dismissAnimation = true,
                                bool confirmOnly = false);
    
    void updateBg(const std::string& file);

protected:
    //    CREATE_FUNC(DialogLayer);
    
    static DialogLayer* create(bool confirmOnly = false);
    virtual bool init(bool confirmOnly = false);
    
    DialogLayer():
    _messageLabel(nullptr),
    _dialogBg(nullptr)
    {};
    virtual ~DialogLayer();
    
    //    virtual bool init() override;
    
    void setupWithMessage(const std::string& message,
                          const std::function<void()>& okCallback,
                          const std::function<void()>& noCallback);
    
    void show();
    
protected:
    std::function<void()> _okCallback;
    std::function<void()> _noCallback;
    
    cocos2d::Label *_messageLabel;
    cocos2d::Sprite *_dialogBg;
    bool _dismissAnimation;
};

#endif /* DialogLayer_hpp */
