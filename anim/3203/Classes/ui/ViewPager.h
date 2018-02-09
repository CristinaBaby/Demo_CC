//
// ViewPager.h
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#ifndef __ANIM3203__ViewPager__
#define __ANIM3203__ViewPager__

#include "cocos2d.h"

USING_NS_CC;

class ViewPager : public Node
{
public:
    ViewPager();
    ~ViewPager();

    static ViewPager* create(const std::string &aNormal, const std::string &aActive);
    virtual bool init(const std::string &aNormal, const std::string &aActive);
    
    void setDotCount(size_t aCount);
    
    void setCurrentDot(size_t aIndex);

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables

    size_t m_nCount;
    
    std::string m_sNormalFilePath;
    
    Sprite *m_pActiveDot;
    
private://method
    std::vector<Sprite *> m_vDots;
};

#endif /* defined(__ANIM3203__ViewPager__) */