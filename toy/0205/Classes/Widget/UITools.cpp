//
//  UITools.cpp
//
//  Created by luoxp on 14-9-22.
//
//

#include "UICPP.h"
#include "UITools.h"
#include "GUI/CCEditBox/CCEditBox.h"
#include <random>

Button * quickButton(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
#if COCOS2D_DEBUG >= 1
    Button *btn = dynamic_cast<Button*>(Helper::seekWidgetByName(root, name));
    CCASSERT(btn != nullptr, "btn is nullptr.");
#else
    Button *btn = (Button*)Helper::seekWidgetByName(root, name);
#endif
    btn->setPressedActionEnabled(true);
    btn->setTouchEnabled(true);
    btn->addTouchEventListener(callback);
    
    return btn;
}

ImageView * quickImageView(const string& name, Widget* root)
{
    ImageView *image = (ImageView*)Helper::seekWidgetByName(root, name);
    return image;
}

ImageView * quickImageView(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    ImageView *image = (ImageView*)Helper::seekWidgetByName(root, name);
    image->setTouchEnabled(true);
    image->addTouchEventListener(callback);
    return image;
}

CheckBox * quickCheckBox(const string& name, Widget* root, const CheckBox::ccCheckBoxCallback &callback)
{
    CheckBox * check = (CheckBox*)Helper::seekWidgetByName(root, name);
    check->setTouchEnabled(true);
    check->addEventListener(callback);
    return check;
}

Layout * quickRoot(const string& filename, Layer *layer)
{
    Node* ui = NodeReader::getInstance()->createNode(filename);
    layer->addChild(ui);
    
    Layout* root = (Layout*)(ui->getChildren().at(0));
    Size size = Director::getInstance()->getVisibleSize();
    root->setContentSize(size);
    
    return root;
}


Layout * quickRoot(const string& filename, Layer *layer, Node **ui)
{
    *ui = NodeReader::getInstance()->createNode(filename);
    layer->addChild(*ui);
    
    Layout* root = (Layout*)((*ui)->getChildren().at(0));
    Size size = Director::getInstance()->getVisibleSize();
    root->setContentSize(size);
    
    return root;
}


Layout * quickRootForSmallLayer(const string& filename, Layer *layer)
{
    Node* ui = NodeReader::getInstance()->createNode(filename);
    layer->addChild(ui);
    
    Layout* root = (Layout*)(ui->getChildren().at(0));
    return root;
}


Layout * quickLayout(int iTag, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    Layout *layout = (Layout*)Helper::seekWidgetByTag(root, iTag);
    layout->addTouchEventListener(callback);

    return layout;
}

Layout * quickLayout(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    Layout *layout = (Layout*)Helper::seekWidgetByName(root, name);
    layout->addTouchEventListener(callback);
    
    return layout;
}

void quickSetString(int iTag, Widget *itemRoot, const string &text)
{
    Text * ui = (Text*)Helper::seekWidgetByTag(itemRoot, iTag);
    ui->setString(text);
}

void quickSetString(const string &itemName, Widget *itemRoot, const string &text)
{
    Text * ui = (Text*)Helper::seekWidgetByName(itemRoot, itemName);
    ui->setString(text);
}

void quickSetImage(int iTag, Widget *itemRoot, const string &filename)
{
    ImageView * ui = (ImageView*)Helper::seekWidgetByTag(itemRoot, iTag);
    ui->loadTexture(filename);
}

void quickSetImage(const string &itemName, Widget *itemRoot, const string &filename)
{
	ImageView * ui = (ImageView*)Helper::seekWidgetByName(itemRoot, itemName);
    ui->loadTexture(filename);
}

void quickSetVisible(int iTag, Widget *itemRoot, bool bVisible)
{
	Widget * ui = Helper::seekWidgetByTag(itemRoot, iTag);
    ui->setVisible(bVisible);
}

void quickSetVisible(const string& name , Widget *itemRoot, bool bVisible)
{
    Widget * ui = Helper::seekWidgetByName(itemRoot, name);
    ui->setVisible(bVisible);
}

void quickSetEnabled(const string& name , Widget *itemRoot, bool bEnabled)
{
    Widget * ui = Helper::seekWidgetByName(itemRoot, name);
    ui->setEnabled(bEnabled);
}

void quickAddEvent(const string& eventName, const function<void(EventCustom*)>& callback, Node *node)
{
    auto event = EventListenerCustom::create(eventName, callback);
    node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event, node);
}

void quickSetButtonTitle(int iTag, Widget *itemRoot, const string &itemName)
{
	Button * ui = (Button*)Helper::seekWidgetByTag(itemRoot, iTag);
    ui->setTitleText(itemName);
}

void quickSetFontName(const string& name, Widget *itemRoot, const string& fontname)
{
    auto text = (Text*)Helper::seekWidgetByName(itemRoot, name);
    text->setFontName(fontname);
}

void quickLogContentSize(Node *node)
{
    Size size = node->getContentSize();
    log("width: %f, height: %f", size.width, size.height);
}

void quickLogPosition(Node *node)
{
    Vec2 position = node->getPosition();
    log("x: %f, y: %f", position.x, position.y);
}

//如果这个函数在init里面, 就不需要getRealPositionForDesignPosition
//初始化完成后, 就需要getRealPositionForDesignPosition
Widget* quickWidgetFromOtherRoot(const string& name, Widget *itemRoot, Node *parent)
{
    auto widget = (Widget*)Helper::seekWidgetByName(itemRoot, name);
    widget->retain();
    widget->removeFromParent();
    Vec2 pos = parent->convertToNodeSpace(V::getRealPositionForDesignPosition(widget->getWorldPosition()));
    widget->setPosition(pos);
    parent->addChild(widget);
    widget->release();
    
    return widget;
}

void quickBackToOriginal(Node * node, Vec2 &original)
{
    FiniteTimeAction* action = EaseElasticOut::create(MoveTo::create(0.5f, original), 0.8f);
    node->runAction(action);
}

bool quickIfAInB(Widget * a, Widget * b)
{
    Vec2 pos = b->getWorldPosition();
    Size contentSize = b->getContentSize();
    
    Rect rect = Rect(pos.x - contentSize.width/2, pos.y - contentSize.height/2, contentSize.width, contentSize.height);
    if (rect.containsPoint(a->getWorldPosition()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void quickSetLocalZOrder(Widget * widget, int localZOrder)
{
    widget->setLocalZOrder(localZOrder);
    widget->getVirtualRenderer()->setLocalZOrder(localZOrder);
}

bool quickIsOutsideOfScreen(Widget * widget)
{
    Size win = Director::getInstance()->getVisibleSize();
    Size size = widget->getContentSize();
    Vec2 pos = widget->getWorldPosition();
    
    //先检查x
    if (pos.x - size.width/2 < 0 || pos.x + size.width/2 > win.width) {
        return true;
    }
    else if (pos.y - size.height/2 < 0 || pos.y + size.height/2 > win.height)
    {
        return true;
    }
    
    return false;
}

Vec2 rotateWithDegree(Vec2 &point, float fDegree)
{
    //角度转弧度
    float angle = M_PI * fDegree /180;
    return point.rotateByAngle(Vec2::ZERO, angle);
}

int cycleNum(int iCount, bool bAdd, int *idx)
{
    if (bAdd == true)
    {
        if (*idx >= iCount - 1)
        {
            *idx = 0;
        }
        else
        {
            (*idx)++;
        }
    }
    else if (bAdd == false)
    {
        if ((*idx) <= 0)
        {
            (*idx) = iCount - 1;
        }
        else
        {
            (*idx)--;
        }
    }
    
    return (*idx);
}

int quickRandom(int min, int max)
{
    std::default_random_engine engine(std::random_device{}());
    std::uniform_int_distribution <> distribution(min, max);
    return distribution(engine);
}

float quickRandomFloat(float min, float max)
{
    std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution <> distribution(min, max);
    return distribution(engine);
}

Armature * quickGetAnimation(int animationIndex, const string &name, const Vec2 &pos)
{
    Armature *armature = Armature::create(name);
    armature->getAnimation()->playWithIndex(animationIndex);
    armature->setPosition(pos);
    return armature;
}
