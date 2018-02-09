
#ifndef __ItemScrollView__
#define __ItemScrollView__


#include "cocos2d.h"
#include "Heads.h"

USING_NS_CC;

class ItemCell:public Node {
    
    
public:
    static ItemCell* create(const std::string bgPath,const std::string bgHighlight,const std::string frontHighlight,const std::string btn,const std::string lock,int lockType);
    bool init(const std::string bgPath,const std::string bgHighlight,const std::string frontHighlight,const std::string btn,const std::string lock,int lockType);
    
    void setSelected(bool selected);
    int lockType; //0 lock 1 reward
    int decType; 
    std::function<void(ItemCell*)> onItemCellSelected;
private:
    Sprite* m_pBg;
    Sprite* m_pBgHeighlight;
    Sprite* m_pFrontHeighlight;
    Button* m_pBtn;
    Sprite* m_pLock;
    std::string m_sBgPath;
    std::string m_sBgHighlightPath;
    std::string m_sFrontHighlightPath;
};

class ItemScrollView : public Layer,public TableViewDataSource,public TableViewDelegate

{
public:
    ItemScrollView();
    ~ItemScrollView();
    
    enum class Direction{
        eDirectionV,
        eDirectionH,
    };
    static ItemScrollView* createWithSize(Size size,bool useTableView = true);
    virtual bool init(Size size,bool useTableView);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) {
        return cellSizeForTable(table);
    };
    virtual Size cellSizeForTable(TableView *table) {
        return Size(640,130);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void showItemAnimation(){
        if(m_pScrollView){
            Vector<Node*> pChildren = m_pScrollView->getInnerContainer()->getChildren();
            int count = pChildren.size();
            int index = 0;
            for_each(pChildren.begin(), pChildren.end(), [=,&index,&count](Node* pNode){
                Vec2 pos = pNode->getPosition();
                pNode->setPosition(Vec2(pos.x,m_pScrollView->getInnerContainer()->getContentSize().height+100));
                pNode->runAction(Sequence::create(DelayTime::create(0.1+0.1*(count-index)),
                                                  EaseSineOut::create(MoveTo::create(0.3+0.3*index,pos)), NULL));
            });
        }
    }
    std::function<void(int index,int type,DecorateConfigData Data)> onItemCellSelected;
    std::function<void()> onOKClicked;
    
    void setMargin(float margin){
        m_nMargin = margin;
    }
    void setDirection(Direction dir){
        m_eDirection = dir;
        if (m_pTableView) {
            m_pTableView->setDirection(dir==Direction::eDirectionH?cocos2d::extension::ScrollView::Direction::HORIZONTAL:cocos2d::extension::ScrollView::Direction::VERTICAL);
        }
        if (m_pScrollView) {
            m_pScrollView->setDirection(dir==Direction::eDirectionH?cocos2d::ui::ScrollView::Direction::HORIZONTAL:cocos2d::ui::ScrollView::Direction::VERTICAL);
        }
    };
//    void setUseTableScrollView(bool use){
//        m_bUseTableView = use;
//    }
    void reloadData(bool second = true);
    
    void insertOthers(DecorateConfigData decData,bool second = true,bool front = false);
    
    void scrollToNext();
    
    void scrollToPre();
    
    int levelFreeCount;
    int lockFreeCount;
    int getPercent();

    void setSelected(bool selected = true){
        if(m_pPreCell){
            m_pPreCell->setSelected(selected);
        }
    }
    std::string bgPath;
    std::string bgHighlightPath;
    std::string frontHighlightPath;
    std::string btnPathEX;
    std::string boxPathEX;
    DecorateConfigData decorationData;
    
    Vec2 getWorldItemPos(){
        if(m_pPreCell){
            return m_pPreCell->getParent()->convertToWorldSpace(m_pPreCell->getPosition());
        }
        return Vec2::ZERO;
    }
    int indexOffset; //偏移量（有的可能从0开始有的可能从1开始）
    
    TableViewCell* m_pCurCell;
protected:
    void _relayout();
    void _itemSelected(ItemCell*);
    void _onButtonCallback(Ref *ref,Widget::TouchEventType);
protected:
    int colNum;
    TableView* m_pTableView;
    ExpandScrollView* m_pScrollView;
    bool m_bUseTableView;
    int m_nPercent;
    Direction m_eDirection;
    Sprite* m_pBoard;
    float m_nMargin;
    ItemCell* m_pPreCell;
    std::vector<DecorateConfigData> insertDecData;
    std::vector<ItemCell*> m_ItemVector;
};

#endif