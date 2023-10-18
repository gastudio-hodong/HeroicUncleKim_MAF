//
//  CListView.h
//

#ifndef CListView_h
#define CListView_h

#include "cocos2d.h"
#include "ui/UIListView.h"

NS_CC_BEGIN

namespace ui{

class CListView;

class DelegateListView
{
public:
    virtual void listviewScrollView(CListView* listview, cocos2d::ui::ScrollView::EventType type) = 0;
    virtual Size listviewCellSize(CListView* listview, ssize_t idx) = 0;
    virtual void listviewCell(CListView *listview, Widget* childview, ssize_t idx) = 0;
    virtual int64_t listviewNumberOfCells(CListView *listview) = 0;
};

class CListView : public cocos2d::ui::ListView
{
public:
    CListView();
    virtual ~CListView();
    
    /**
     * @brief create
     */
    static CListView* create(DelegateListView* delegate, Size size);
    static CListView* create(DelegateListView* delegate, Size size, Direction dir);
    
    /**
     * @brief init
     */
    virtual bool init(Size size, Direction dir);
    
public:
    /**
     * @brief set delegate
     * @param target target delegate
     */
    void setDelegate(DelegateListView* target) { _delegate = target; };
    
    /**
     * @brief Jump to specific item
     * @param itemIndex Specifies the item's index
     * @param positionRatioInView Specifies the position with ratio in list view's content size.
     * @param itemAnchorPoint Specifies an anchor point of each item for position to calculate distance.
     */
    void jumpToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint);
    
    /**
     * @brief Scroll to specific item
     * @param positionRatioInView Specifies the position with ratio in list view's content size.
     * @param itemAnchorPoint Specifies an anchor point of each item for position to calculate distance.
     * @param timeInSec Scroll time
     */
    void scrollToItem(ssize_t itemIndex, const cocos2d::Vec2& positionRatioInView, const cocos2d::Vec2& itemAnchorPoint);
    void scrollToItem(ssize_t itemIndex, const cocos2d::Vec2& positionRatioInView, const cocos2d::Vec2& itemAnchorPoint, float timeInSec);
    
    /**
     * @brief get Container position
     */
    cocos2d::Vec2 getContainerOffset();
    void setContainerOffset(cocos2d::Vec2 offset);
    
    /**
     * @brief reload cell
     */
    void reloadData();
    
protected:
    /**
     * @brief cell position set
     */
    void updateCellPositions();
    
    /**
     * @brief set Container size
     */
    void updateContainerSize();
    
    /**
     * @brief get offset to item index
     */
    int64_t indexFromOffset(cocos2d::Vec2 offset);
    int64_t indexFromOffsetSecond(cocos2d::Vec2 offset);
    
    /**
     * @brief get item index to offset
     */
    cocos2d::Vec2 offsetFromIndex(int64_t nIdx);
    cocos2d::Vec2 offsetFromIndexSecond(int64_t nIdx);
    
    /**
     * Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
     */
    cocos2d::Vec2 minContainerOffset();
    /**
     * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
     */
    cocos2d::Vec2 maxContainerOffset();
    
    /**
     * @brief get item position
     */
    cocos2d::Vec2 calculateItemDestination(int64_t nIdx, cocos2d::Size sizeCell, const cocos2d::Vec2& positionRatioInView, const cocos2d::Vec2& itemAnchorPoint);
    cocos2d::Vec2 calculateItemPositionWithAnchor(int64_t nIdx, cocos2d::Size sizeCell, const cocos2d::Vec2& itemAnchorPoint);
    
    /**
     * @brief get item
     */
    cocos2d::ui::Widget* getItemsTag(int64_t nIdx);
    
    /**
     * @brief scrollview event
     */
    void eventScrollView(cocos2d::Ref* sender, cocos2d::ui::ScrollView::EventType eType);
    
protected:
    /* delegate */
    DelegateListView* _delegate;
    
    /* cell count*/
    int64_t _nCountCellTotal;
    
    /* cell count min*/
    int64_t _nCountCellMinimum;
    
    /* cell size min */
    double _sizeCellMin;
    
    /* content size & container size is set */
    bool _bContentSize;
    
    /* container last position */
    double _nContainerLastPos;
    
    /* container all position */
    std::vector<cocos2d::Vec2> _listPositions;
    
};

}

NS_CC_END

#endif /* CListView_h */
