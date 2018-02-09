//
//  UITools.cpp
//
//  Created by luoxp on 14-9-22.
//
//

#include "UICPP.h"
#include "UITools.h"
#include "GUI/CCEditBox/CCEditBox.h"

Button * quickButton(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
#if COCOS2D_DEBUG >= 1
    Button *btn = dynamic_cast<Button*>(Helper::seekWidgetByName(root, name));
    CCASSERT(btn != nullptr, "btn is nullptr.");
#else
    Button *btn = (Button*)Helper::seekWidgetByName(root, name);
#endif
    btn->setPressedActionEnabled(true);
    btn->setTouchEnabled(true);               //这个函数有问题.
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

EditBox * quickEditBox(int iParentTag, Layout *root, const Color3B& color, EditBoxDelegate * delegate)
{
    return quickEditBox(iParentTag, root, color, 24, delegate);
}

EditBox * quickEditBox(int iParentTag, Layout *root, const Color3B& color, int fontSize, EditBoxDelegate * delegate)
{
    return quickEditBox(iParentTag, root, color, fontSize, delegate, (int)EditBox::KeyboardReturnType::DONE);
}

EditBox * quickEditBox(int iParentTag, Layout *root, const Color3B& color, int fontSize, EditBoxDelegate * delegate, int returnType)
{
    Layout *parent = (Layout*)Helper::seekWidgetByTag(root, iParentTag);
    EditBox * edit = EditBox::create(parent->getContentSize(), cocos2d::extension::Scale9Sprite::create());
    
    edit->setFont("Arial", fontSize);
    edit->setFontColor(color);
    edit->setPosition(Vec2(parent->getContentSize().width * 0.5, parent->getContentSize().height * 0.5));
    
    edit->setDelegate(delegate);
    edit->setReturnType((EditBox::KeyboardReturnType)returnType);
    parent->addChild(edit);
    
    return edit;
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


