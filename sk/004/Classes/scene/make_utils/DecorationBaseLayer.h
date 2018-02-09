//
//  DecorationBaseLayer.h
//  BHUG1064
//
//  Created by maxiang on 6/6/16.
//
//

#ifndef DecorationBaseLayer_h
#define DecorationBaseLayer_h

#include "MakeBaseScene.h"
#include "scene/shop/ShopScene.h"
#include "ESScrollView.h"
#include "ESUnitAdapter.h"

#define DecorationFrameFile     "ui/frame.png"
#define DecorationZoomFile      "ui/btn_scale.png"
#define DecorationDeleteFile    "ui/btn_delete.png"
#define DecorationBgFile        "ui/dec/bg.png"
#define SelectBgFile            "ui/dec/bg_chick.png"
#define ScribblePatterBrushFile ""
#define ScribblePatterMask      ""

//可以拖拽的工具, 如果没有, 设置为空
#define ScribbleDragTool        ""

//显示或隐藏装饰条的button, 如果没有, 设置为空
#define HideOrShowButtonDown    ""
#define HideOrShowButtonUp      ""
using namespace std;
typedef struct _DecorationConfig
{
public:
    _DecorationConfig()
    {};
    
    _DecorationConfig(const std::string _name,
                      const std::string _typeSelected,
                      const std::string _typeUnselected,
                      const std::string _iconFormat,
                      const std::string _fileFormat,
                      int _begin,
                      int _end,
                      int _zorder = 10)
    {
        name = _name;
        typeSelected = _typeSelected;
        typeUnselected = _typeUnselected;
        iconFormat = _iconFormat;
        fileFormat = _fileFormat;
        begin = _begin;
        end = _end;
        zorder = _zorder;
    };
    
    /* 这个装饰的名字，对应IAP和Reward配置文件 */
    std::string name;
    
    /* icon 路径格式 */
    std::string iconFormat;//
    
    /* icon对应大图路径格式 */
    std::string fileFormat;
    
    /* 类别对应选中路径格式 */
    std::string typeSelected;
    
    /* 类别对应未选中路径格式 */
    std::string typeUnselected;

    
    /* 图片起始索引 */
    int begin;
    
    /* 图片结束索引 */
    int end;
    
    /* 装饰物品的zorder, 默认为10 */
    int zorder = 10;
}DecorationConfig;

class ComponentCycleScroll;
class ComponentDecoration;
class ComponentScribblePattern;
class ComponentTouchMove;
class ESScrollView;
class DecorationBaseLayer : public MakeBaseScene
{
public:
    CREATE_FUNC(DecorationBaseLayer);
    virtual bool init() override;
    
#pragma mark- Decoration API
    /* @brief 放大缩小旋转类装饰
       @param config 配置
     */
    void showRotateAndZoomDecoration(DecorationConfig config);
    
    /* @brief scribble pattern涂抹类装饰
       @param config 配置
       @param scribbleParent scribble添加到哪个父节点上
       @param finishCallback 涂抹完成回调
       @param getMaskCall 获得遮罩的回调，因为有些涂抹图片需要不同的遮罩, 默认为宏定义
       @param isShowPlaceHolder 是否在涂抹位置显示半透明纹理
     */
    void showScribblePatternDecoration(DecorationConfig config,
                                       Node *scribbleParent,
                                       const std::function<void()>& finishCallback = nullptr,
                                       const std::function<std::string(int)>& getMaskCall = nullptr,
                                       bool isShowPlaceHolder = false);
    
    /* @brief 替换纹理类装饰
       @param config 配置
       @param replaceSprite 需要替换纹理的的精灵
       @param callback 替换完后回调，用于记录替换的纹理，一般记录item的Tag值
       @param moveAble 是否可以移动，默认为false
     */
    void showReplaceTextureDecoration(DecorationConfig config,
                                      Sprite *replaceSprite,
                                      const std::function<void(Widget *item)> callback = nullptr,
                                      bool moveAble = false);
    
    /* @brief 点缀类装饰，在一个食物上裁剪
       @param config 配置
       @param clippingParent 添加到的父节点，一般为食物本身
       @param mask 点缀的遮罩，一般为食物形状的图片
     */
    void showClippingTextureDecoration(DecorationConfig config,
                                       Node *clippingParent,
                                       const std::string& mask);
  
    /* @brief 保存图片, 整个Layer
       @param name 保存的图片名字
       @param callback 保存完回调
      */
    void saveEatSprite(const std::string& name,
                       const std::function<void(RenderTexture*, const std::string&)>& callback = nullptr);
    
    /* @brief 保存图片, 指定大小
       @param name 保存的图片名字
       @param area 指定的范围大小
       @param callback 保存完回调
     */
    void saveEatSprite(const std::string& name,
                       const Rect& area,
                       const std::function<void(RenderTexture*, const std::string&)>& callback= nullptr);
    

    void ignorAllTouch(bool ignor);
#pragma mark- Config API
    
    /* @brief 是否显示装饰条隐藏button，默认为false */
    CC_SYNTHESIZE(bool, _showDecorationBarButton, ShowDecorationBarButton);
    
    /* @brief 设置装饰条的可见性
       @param visible 是否可见
     */
    void setDecorationBarVisible(bool visible);
    
    /* @brief 设置装饰背景条, 默认为宏定义
       @param bgFile 图片文件
     */
    void setDecorationBg(const std::string& bgFile);
    
    /* @brief 设置装饰品放大缩小的ui, 默认为宏定义
       @param zoomFile 图片文件
     */
    void setZoomUI(const std::string& zoomFile);
    
    /* @brief 设置装饰品删除的ui, 默认为宏定义
       @param deleteFile 图片文件
     */
    void setDeleteUI(const std::string& deleteFile);
    
    /* @brief 设置装饰品选中时的背景, 默认为宏定义
       @param bgFile 图片文件
     */
    void setItemSelectBg(const std::string& bgFile);
    
    /* @brief 设置scribble pattern涂抹的笔刷, 默认为宏定义
       @param brushFile 图片文件
     */
    void setScribblePatternBrush(const std::string& brushFile);
    
    /* @brief 设置scribble pattern涂抹的遮罩, 默认为宏定义
       @param maskFile 图片文件
     */
    void setScribblePatternMask(const std::string& maskFile);
    
    /* @brief 设置scribble pattern跟着手指涂抹的工具, 默认为宏定义
       @param toolFile 图片文件
     */
    void setScribblePatternTool(const std::string& toolFile);
    
    /* @brief 设置隐藏装饰条向下的button, 默认为宏定义
       @param buttonFile 图片文件
     */
    void setHideOrShowDecroationBarButtonDown(const std::string& buttonFile);
    
    /* @brief 设置隐藏装饰条向上的button, 默认为宏定义
       @param buttonFile 图片文件
     */
    void setHideOrShowDecroationBarButtonUp(const std::string& buttonFile);
    
    
    void showDecorationParticle(Vec2 pos,int zOder = 20);
    
    void initTypeButton();

#pragma mark- Inner
public:
    DecorationBaseLayer():
    _decorationContainerLayer(nullptr),
    _module("\0"),
    _iapIdentifier("\0"),
    _selectBg(nullptr),
    _selectWidget(nullptr),
    _cycleScrollBg(nullptr),
    _cycleScrollComp(nullptr),
    _clippingTouchComp(nullptr),
    _scribblePatternComp(nullptr),
    _patternScribbleParentNode(nullptr),
    _scribbleTool(nullptr),
    _hideOrShowButton(nullptr),
    _isShowDecorationBar(false),
    _showDecorationBarButton(false),
    _curBtn(nullptr),
    _scrollView(nullptr)
    {};
    virtual ~DecorationBaseLayer();
    
    ESScrollView* _scrollView;

protected:
    void setupDecorationWithConfig(DecorationConfig config,
                                   const std::function<void(Widget *item)>& itemClickCallback);
    
    void removeCycleScroll(bool animation, const std::function<void()>& callback);
    void createCycleScroll();
    void showCycleScroll();
    void markItemSelect(Widget *item);
    
    void clearPreviousDecoration();
    
    void hideOrShowButtonAction();
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    std::function<void(Widget *item)> _typeBtnClicked = nullptr;
    

protected:
    std::string _module;//模块名
    std::string _iapIdentifier;//这些装饰从属的iap标识
    
    Widget *_selectWidget;
    
    std::vector<ComponentDecoration*> _decorationsVector;

    Sprite *_selectBg;
    
    Button *_hideOrShowButton;
    bool _isShowDecorationBar;

    Layer *_decorationContainerLayer;
    Sprite *_cycleScrollBg;
    ComponentCycleScroll *_cycleScrollComp;
    
    ComponentTouchMove *_clippingTouchComp;
    ComponentScribblePattern *_scribblePatternComp;
    Node *_patternScribbleParentNode;
    Sprite *_scribbleTool;
    vector<DecorationConfig> _configVec;
    
    Button* _curBtn;
    int _curTag = 0;
    vector<Button*> _typeBtnVec;

};

#endif /* DecorationBaseLayer_h */
