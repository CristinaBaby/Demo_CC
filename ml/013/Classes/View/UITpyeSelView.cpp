//
//  UITpyeSelView.cpp
//  Make
//
//  Created by liji on 15/8/11.
//
//

#include "UITpyeSelView.h"
#include "KSVisibleRect.h"

const int typeCount = 6;
#define path "type/"

std::string typeIconName[typeCount] =
{
    "btn_candy.png",
    "btn_flavor.png",
    "btn_fruit.png",
    "btn_spoons.png",
    "btn_umbrella.png",
    "btn_background.png",
};

std::string typeName[typeCount] =
{
    "candy",
    "syrup",
    "fruit",
    "spoons",
    "extras",
    "background",
};


bool UITpyeSelView::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto bg = Sprite::create("button/banner1.png");
    bg -> setPosition(320,80);
    this -> addChild(bg);
    
    _typeScrollView = ui::ScrollView::create();
    _typeScrollView -> setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _typeScrollView -> setPosition(Vec2(20,0));
    _typeScrollView -> setContentSize(Size(600,180));
    _typeScrollView -> setInnerContainerSize(Size((148+50)*typeCount,180));
    this -> addChild(_typeScrollView);

    for (int i = 0; i<typeCount; i++)
    {
        auto fullPath = path + typeIconName[i];
        auto fullSelectedPath = path + getSelectedName(typeIconName[i]);
        
        ui::CheckBox* btn = ui::CheckBox::create(fullPath, fullPath, fullSelectedPath, fullPath, fullPath);
        btn -> setPosition(Vec2(110+(148+50)*i, 90));
        btn -> addEventListener(CC_CALLBACK_2(UITpyeSelView::touchEvent, this));
        _typeScrollView -> getInnerContainer() -> addChild(btn,10,i);
    }

    
    return true;
}

void UITpyeSelView::loadTypeListFromTo(int idxFrom,int idxTo)
{
    if(_typeScrollView)
        _typeScrollView ->removeFromParent();_typeScrollView = nullptr;
    
    int delta = idxTo - idxFrom;
    
    _typeScrollView = ui::ScrollView::create();
    _typeScrollView -> setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _typeScrollView -> setPosition(Vec2(20,0));
    _typeScrollView -> setContentSize(Size(600,180));
    _typeScrollView -> setInnerContainerSize(Size((148+50)*delta,180));
    this -> addChild(_typeScrollView);
    
    for (int i = idxFrom; i<idxTo; i++)
    {
        auto fullPath = path + typeIconName[i];
        auto fullSelectedPath = path + getSelectedName(typeIconName[i]);
        
        ui::CheckBox* btn = ui::CheckBox::create(fullPath, fullPath, fullSelectedPath, fullPath, fullPath);
        btn -> setPosition(Vec2(110+(148+50)*(i-idxFrom), 90));
        btn -> addEventListener(CC_CALLBACK_2(UITpyeSelView::touchEvent, this));
        _typeScrollView -> getInnerContainer() -> addChild(btn,10,i);
    }

}


std::string UITpyeSelView::getSelectedName(std::string pName)
{
    int a =  (int)pName.find(".");
    pName.replace(a, 20, "_h.png");
    return pName;
}

void UITpyeSelView::touchEvent(Ref* obj,ui::CheckBox::EventType type)
{

    
    
    
    ui::CheckBox *btn = dynamic_cast<ui::CheckBox *>(obj);
    switch (type)
    {
        case ui::CheckBox::EventType::SELECTED:
            
//
        
            if (_selectedBtn)
            {
                log("%d,tag",_selectedBtn -> getTag());
                _selectedBtn -> setSelectedState(false);
            }
            _selectedBtn = btn;
            if(typeSelected)
            {

                this -> typeSelected(typeName[btn -> getTag()]);
                this -> typeSelected(typeName[btn -> getTag()]);//不刷新一次会造成点击事件被屏蔽的问题，先用这个方法治标
            }
            break;
        case ui::CheckBox::EventType::UNSELECTED:
            _selectedBtn = nullptr;
            if(typeUnSelected)
            {
                this -> typeUnSelected();
            }

            break;
        default:
            break;
            
    }
    
    
}

void UITpyeSelView::setSelectNone()
{
    if (_selectedBtn)
    {
        _selectedBtn -> setSelectedState(false);
    }
    _selectedBtn = nullptr;
}

void UITpyeSelView::enableTouch(bool enable)
{
    if(_typeScrollView)
    {
        _typeScrollView -> setEnabled(enable);
    }
}



