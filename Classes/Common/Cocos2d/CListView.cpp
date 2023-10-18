//
//  CListView.cpp
//
//

#include "Common/Cocos2d/CListView.h"
#include "ui/UIButton.h"

NS_CC_BEGIN

namespace ui {

CListView::CListView() :
_delegate(nullptr),
_nCountCellTotal(0),
_nCountCellMinimum(0),
_sizeCellMin(0),
_bContentSize(false),
_nContainerLastPos(0)
{
    
}

CListView::~CListView()
{
    
}
 

CListView* CListView::create(DelegateListView* delegate, Size size)
{
    return create(delegate, size, Direction::VERTICAL);
}

CListView* CListView::create(DelegateListView* delegate, Size size, Direction dir)
{
    CListView* widget = new (std::nothrow) CListView();
    if (widget && widget->init(size, dir))
    {
        widget->autorelease();
        widget->setDelegate(delegate);
        widget->updateCellPositions();
        widget->updateContainerSize();
        
        return widget;
    }
    
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool CListView::init(Size size, Direction dir)
{
    if ( ListView::init() == false )
    {
        return false;
    }
    
    //
    setName("CListView");
    
    //
    setContentSize(size);
    setDirection(dir);
    
    // item default
    auto itemDefault = Layout::create();
    itemDefault->setContentSize(Size(10, 10));
    itemDefault->setName("CListView::CELL");
    setItemModel(itemDefault);
    
    // event add scroll move
    addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(CListView::eventScrollView,this));
    
    return true;
}

void CListView::jumpToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint)
{
    if ( itemIndex < 0 )
    {
        itemIndex = 0;
    }
    
    if ( itemIndex >= _nCountCellTotal )
    {
        itemIndex = _nCountCellTotal - 1;
    }
    
    doLayout();

    Size sizeCell = _delegate->listviewCellSize(this, itemIndex);
    Vec2 destination = calculateItemDestination(itemIndex, sizeCell, positionRatioInView, itemAnchorPoint);

    {
        Vec2 delta = destination - getInnerContainerPosition();
        Vec2 outOfBoundary = getHowMuchOutOfBoundary(delta);
        destination += outOfBoundary;
    }
    
    if ( getContainerOffset() != destination )
    {
        jumpToDestination(destination);
    }
}

void CListView::scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint)
{
    scrollToItem(itemIndex, positionRatioInView, itemAnchorPoint, _scrollTime);
}

void CListView::scrollToItem(ssize_t itemIndex, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint, float timeInSec)
{
    if ( itemIndex < 0 )
    {
        itemIndex = 0;
    }
    
    if ( itemIndex >= _nCountCellTotal )
    {
        itemIndex = _nCountCellTotal - 1;
    }
    
    Size sizeCell = _delegate->listviewCellSize(this, itemIndex);
    Vec2 destination = calculateItemDestination(itemIndex, sizeCell, positionRatioInView, itemAnchorPoint);
    
    {
        Vec2 delta = destination - getInnerContainerPosition();
        Vec2 outOfBoundary = getHowMuchOutOfBoundary(delta);
        destination += outOfBoundary;
    }
    
    if ( getContainerOffset() != destination )
    {
        startAutoScrollToDestination(destination, timeInSec, true);
    }
}

Vec2 CListView::getContainerOffset()
{
    return getInnerContainerPosition();
}

void CListView::setContainerOffset(cocos2d::Vec2 offset)
{
    const Vec2 minOffset = this->minContainerOffset();
    const Vec2 maxOffset = this->maxContainerOffset();
    
    offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
    offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
    
    setInnerContainerPosition(offset);
}

void CListView::reloadData()
{
    stopOverallScroll();
    
    //
    updateCellPositions();
    updateContainerSize();
    
    //
    eventScrollView(this, cocos2d::ui::ScrollView::EventType::CONTAINER_MOVED);
}

void CListView::updateCellPositions()
{
    if ( _delegate == nullptr )
    {
        return;
    }
    
    _nCountCellTotal = _delegate->listviewNumberOfCells(this);
    _listPositions.resize(_nCountCellTotal + 1, Vec2::ZERO);
    _sizeCellMin = 0;
    
    //
    if ( _nCountCellTotal <= 0 )
    {
        return;
    }
    
    //
    double nMargin = getItemsMargin();
    
    Size viewSize = getContentSize();
    Vec2 posCurrent = Vec2::ZERO;
    Vec2 posMargen = Vec2(0, nMargin);
    if ( getDirection() == Direction::HORIZONTAL )
    {
        posMargen = Vec2(nMargin, 0);
    }
    
    for ( int i = 0; i < _nCountCellTotal; i++ )
    {
        Size cellSize = _delegate->listviewCellSize(this, i);
        
        if ( getDirection() == Direction::HORIZONTAL )
        {
            posCurrent.y = viewSize.height / 2 - cellSize.height / 2;
        }
        
        //
        if ( i != 0 )
        {
            posCurrent += posMargen;
        }
        _listPositions[i] = posCurrent;
        
        //
        
        switch (getDirection())
        {
            case Direction::HORIZONTAL:
                posCurrent.x += cellSize.width;
                if ( _sizeCellMin == 0 || _sizeCellMin > cellSize.width )
                {
                    _sizeCellMin = cellSize.width;
                }
                break;
            default:
                posCurrent.y += cellSize.height;
                if ( _sizeCellMin == 0 || _sizeCellMin > cellSize.height )
                {
                    _sizeCellMin = cellSize.height;
                }
                break;
        }
        
        
    }
    _listPositions[_nCountCellTotal] = posCurrent;
}

void CListView::updateContainerSize()
{
    if ( _delegate == nullptr )
    {
        return;
    }
    
    _bContentSize = false;
    
    //
    removeAllItems();
    
    //
    if ( _nCountCellTotal <= 0 )
    {
        return;
    }
    
    Size sizeContainer = Size::ZERO;
    Size sizeView = getContentSize();
    Vec2 position = _listPositions[_nCountCellTotal];
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            sizeContainer = Size(position.x, sizeView.height);
            
            _nCountCellMinimum = sizeView.width / _sizeCellMin * 2;
            break;
        }
            
        default:
        {
            sizeContainer = Size(sizeView.width, position.y);
            
            _nCountCellMinimum = sizeView.height / _sizeCellMin * 2;
            break;
        }
    }
    
    
    for ( int i = 0; i < _nCountCellMinimum && i < _nCountCellTotal; i++ )
    {
        Size size = _delegate->listviewCellSize(this, i);
        
        auto item = _model->clone();
        item->setContentSize(size);
        item->setTag(i);
        pushBackCustomItem(item);
        
        _delegate->listviewCell(this, item, i);
    }
    
    // layout & containerSize
    forceDoLayout();
    setInnerContainerSize(sizeContainer);
    getInnerContainer()->forceDoLayout();
    _innerContainer->setBackGroundColor(Color3B::YELLOW);
    
    //
    for ( auto obj : _items )
    {
        int nIdx = obj->getTag();
        
        Vec2 positionCell = offsetFromIndex(nIdx);
        if ( getDirection() == Direction::HORIZONTAL )
        {
            positionCell = Vec2(positionCell.x, obj->getPositionY());
        }
        else
        {
            positionCell = Vec2(obj->getPositionX(), positionCell.y);
        }
        obj->setPosition(positionCell);
    }
    
    //
    if ( getDirection() == Direction::VERTICAL )
    {
        _nContainerLastPos = getInnerContainer()->getPosition().y;
    }
    else if ( getDirection() == Direction::HORIZONTAL )
    {
        _nContainerLastPos = getInnerContainer()->getPosition().x;
    }
    
    //
    _bContentSize = true;
}

int64_t CListView::indexFromOffset(Vec2 offset)
{
    if ( getDirection() == Direction::HORIZONTAL )
    {
        offset.x = offset.x;
    }
    else
    {
        offset.y = _innerContainer->getContentSize().height - offset.y;
    }
    
    
    int64_t nIdx = indexFromOffsetSecond(offset);
    if ( nIdx != -1 )
    {
        nIdx = MAX(0, nIdx);
        if ( nIdx > _nCountCellTotal - 1 )
        {
            nIdx = -1;
        }
    }

    return nIdx;
}

int64_t CListView::indexFromOffsetSecond(cocos2d::Vec2 offset)
{
    long low = 0;
    long high = _nCountCellTotal - 1;
    
    double search;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            search = offset.x;
            break;
        default:
            search = offset.y;
            break;
    }

    while (high >= low)
    {
        long index = low + (high - low) / 2;
        Vec2 posStart = _listPositions[index];
        Vec2 posEnd = _listPositions[index + 1];
        
        float cellStart = posStart.y;
        float cellEnd = posEnd.y;
        if ( getDirection() == Direction::HORIZONTAL )
        {
            cellStart = posStart.x;
            cellEnd = posEnd.x;
        }
            
        if (search >= cellStart && search <= cellEnd)
        {
            return index;
        }
        else if (search < cellStart)
        {
            high = index - 1;
        }
        else
        {
            low = index + 1;
        }
    }

    if ( low <= 0 )
    {
        return 0;
    }

    return -1;
}

Vec2 CListView::offsetFromIndex(int64_t nIdx)
{
    const Size cellSize = _delegate->listviewCellSize(this, nIdx);
    
    Vec2 offset = offsetFromIndexSecond(nIdx);
    offset.y = _innerContainer->getContentSize().height - offset.y - cellSize.height;
    return offset;
}

Vec2 CListView::offsetFromIndexSecond(int64_t nIdx)
{
    Vec2 offset = Vec2::ZERO;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            offset.set(_listPositions[nIdx]);
            break;
        default:
            offset.set(_listPositions[nIdx]);
            break;
    }

    return offset;
}

Vec2 CListView::maxContainerOffset()
{
    Vec2 anchorPoint = _innerContainer->isIgnoreAnchorPointForPosition() ? Vec2::ZERO : _innerContainer->getAnchorPoint();
    float contW       = _innerContainer->getContentSize().width * _innerContainer->getScaleX();
    float contH       = _innerContainer->getContentSize().height * _innerContainer->getScaleY();
    
    return Vec2(anchorPoint.x * contW, anchorPoint.y * contH);
}

Vec2 CListView::minContainerOffset()
{
    Vec2 anchorPoint = _innerContainer->isIgnoreAnchorPointForPosition() ? Vec2::ZERO : _innerContainer->getAnchorPoint();
    float contW       = _innerContainer->getContentSize().width * _innerContainer->getScaleX();
    float contH       = _innerContainer->getContentSize().height * _innerContainer->getScaleY();
    
    Size sizeView = getContentSize();
    return Vec2(sizeView.width - (1 - anchorPoint.x) * contW, sizeView.height - (1 - anchorPoint.y) * contH);
}

Vec2 CListView::calculateItemDestination(int64_t nIdx, Size sizeCell, const Vec2& positionRatioInView, const Vec2& itemAnchorPoint)
{
    const Size& contentSize = getContentSize();
    Vec2 positionInView = Vec2::ZERO;
    positionInView.x += contentSize.width * positionRatioInView.x;
    positionInView.y += contentSize.height * positionRatioInView.y;

    Vec2 itemPosition = calculateItemPositionWithAnchor(nIdx, sizeCell, itemAnchorPoint);
    return -(itemPosition - positionInView);
}

Vec2 CListView::calculateItemPositionWithAnchor(int64_t nIdx, Size sizeCell, const Vec2& itemAnchorPoint)
{
    Vec2 origin = offsetFromIndex(nIdx);
    return origin + Vec2(sizeCell.width * itemAnchorPoint.x, sizeCell.height * itemAnchorPoint.y);
}

Widget* CListView::getItemsTag(int64_t nIdx)
{
    Widget* objResult = nullptr;
    for (auto& obj : _items)
    {
        if ( obj->getTag() == nIdx )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

#pragma mark - event
void CListView::eventScrollView(cocos2d::Ref* sender, cocos2d::ui::ScrollView::EventType eType)
{
    //
    auto listView = dynamic_cast<CListView*>(sender);
    if ( listView == nullptr || _nCountCellTotal <= 0 || _bContentSize == false )
    {
        return;
    }
    
    //
    if ( eType != ScrollView::EventType::CONTAINER_MOVED )
    {
        return;
    }
    
    // container last pos diff
    double nContainerLastPosDiff = 0;
    if ( getDirection() == Direction::VERTICAL )
    {
        nContainerLastPosDiff = _nContainerLastPos - getInnerContainerPosition().y;
    }
    else if ( getDirection() == Direction::HORIZONTAL )
    {
        nContainerLastPosDiff = _nContainerLastPos - getInnerContainerPosition().x;
    }
    
    if ( fabs(nContainerLastPosDiff) < 2 )
    {
        return;
    }
    
    //
    _delegate->listviewScrollView(listView, eType);
    
    int64_t nIdxStart = 0, nIdxEnd = 0;
    Size sizeView = getContentSize();
    Vec2 offset = getInnerContainerPosition() * -1;
    
    //
    if ( getDirection() == Direction::HORIZONTAL )
    {
        offset.x = offset.x;
    }
    else
    {
        offset.y = offset.y + sizeView.height/getInnerContainer()->getScaleY();
    }
    nIdxStart = indexFromOffset(offset);
    if (nIdxStart == CC_INVALID_INDEX)
    {
        nIdxStart = _nCountCellTotal - 1;
    }
    
    //
    if ( getDirection() == Direction::HORIZONTAL )
    {
        offset.x += sizeView.width/getInnerContainer()->getScaleX();
    }
    else
    {
        offset.y -= sizeView.height/getInnerContainer()->getScaleY();
    }
    nIdxEnd = indexFromOffset(offset);
    if ( nIdxEnd == CC_INVALID_INDEX)
    {
        nIdxEnd = _nCountCellTotal - 1;
    }

    for (auto & item : _items)
    {
        if ( item->getTag() < nIdxStart || item->getTag() > nIdxEnd )
        {
            item->setTag(-1);
        }
    }
    
    std::sort(std::begin(_items), std::end(_items), [=](Node* n1, Node* n2) {
        return n1->getTag() < n2->getTag();
    });
    
    for (long i = nIdxStart; i <= nIdxEnd; i++)
    {
        int64_t nIdx = i;

        auto item = getItemsTag(nIdx);
        if ( item == nullptr )
        {
            item = _items.front();

            _items.pushBack(item);
            _items.erase(0);
            
            
            Size sizeCell = _delegate->listviewCellSize(this, nIdx);
            item->setContentSize(sizeCell);
            item->setTag((int)nIdx);
            item->setLocalZOrder((int)nIdx);

            //
            _delegate->listviewCell(this, item, nIdx);
        }
        
        // position
        Vec2 positionCell = offsetFromIndex(nIdx);
        item->setPosition(positionCell);
    }
    
    // update container last pos
    if ( getDirection() == Direction::VERTICAL )
    {
        _nContainerLastPos = getInnerContainerPosition().y;
    }
    else if ( getDirection() == Direction::HORIZONTAL )
    {
        _nContainerLastPos = getInnerContainerPosition().x;
    }
}

}

NS_CC_END
