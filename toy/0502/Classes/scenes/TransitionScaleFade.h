//
//  RecordScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

/** @brief TransitionScaleFade:
 Fade out the outgoing scene and then fade in the incoming scene.'''
 */
class CC_DLL TransitionScaleFade : public TransitionFade
{
public:
    /** creates the transition with a duration and with an RGB color
     * Example: FadeTransition::create(2, scene, Color3B(255,0,0); // red color
     */
    static TransitionScaleFade* create(float duration, Scene* scene, const Color3B& color);
    static TransitionScaleFade* create(float duration, Scene* scene);
    
    /**
     * @js NA
     * @lua NA
     */
    virtual void onEnter() override;
};
