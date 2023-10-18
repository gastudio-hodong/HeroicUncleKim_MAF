/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Libs/maf/MafGridView.h"
#include "extensions/GUI/CCScrollView/CCTableViewCell.h"

NS_CC_EXT_BEGIN

MafGridView* MafGridView::create()
{
    return MafGridView::create(nullptr, Size::ZERO);
}

MafGridView* MafGridView::create(MafGridViewDataSource* dataSource, Size size, int column)
{
    return MafGridView::create(dataSource, size, column, NULL);
}

MafGridView* MafGridView::create(MafGridViewDataSource* dataSource, Size size, int column, Node *container)
{
    MafGridView *table = new MafGridView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->setColumn(column);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool MafGridView::initWithViewSize(Size size, Node* container/* = NULL*/)
{
    if (ScrollView::initWithViewSize(size,container))
    {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        _vordering      = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);
        
        ScrollView::setDelegate(this);
        return true;
    }
    return false;
}

MafGridView::MafGridView()
: _touchedCell(nullptr)
, _indices(nullptr)
, _dataSource(nullptr)
, _gridViewDelegate(nullptr)
, _oldDirection(Direction::NONE)
, _isUsedCellsDirty(false)
, _column(1)
{
    
}

MafGridView::~MafGridView()
{
    CC_SAFE_DELETE(_indices);
}

void MafGridView::setVerticalFillOrder(VerticalFillOrder fillOrder)
{
    if (_vordering != fillOrder)
    {
        _vordering = fillOrder;
        if (!_cellsUsed.empty())
        {
            this->reloadData();
        }
    }
}

MafGridView::VerticalFillOrder MafGridView::getVerticalFillOrder()
{
    return _vordering;
}

void MafGridView::reloadData()
{
    _oldDirection = Direction::NONE;
    
    for(const auto &cell : _cellsUsed) {
        if(_gridViewDelegate != NULL) {
            _gridViewDelegate->tableCellWillRecycle(this, cell);
        }
        
        _cellsFreed.pushBack(cell);
        
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }
    
    _indices->clear();
    _cellsUsed.clear();
    
    this->_updateCellPositions();
    this->_updateContentSize();
    if (_dataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}

TableViewCell *MafGridView::cellAtIndex(ssize_t idx)
{
    if (_indices->find(idx) != _indices->end())
    {
        for (const auto& cell : _cellsUsed)
        {
            if (cell->getIdx() == idx)
            {
                return cell;
            }
        }
    }
    
    return nullptr;
}

void MafGridView::updateCell()
{
    for (extension::TableViewCell* cell : _cellsUsed)
    {
        updateCellAtIndex(cell->getIdx());
    }
}
void MafGridView::updateCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    extension::TableViewCell* cell = this->cellAtIndex(idx);
    if (cell)
    {
        this->_moveCellOutOfSight(cell);
    }
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void MafGridView::insertCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    long newIdx = 0;
    
    auto cell = cellAtIndex(idx);
    if (cell)
    {
        newIdx = _cellsUsed.getIndex(cell);
        // Move all cells behind the inserted position
        for (long i = newIdx; i < _cellsUsed.size(); i++)
        {
            cell = _cellsUsed.at(i);
            this->_setIndexForCell(cell->getIdx()+1, cell);
        }
    }
    
    //insert a new cell
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
    
    this->_updateCellPositions();
    this->_updateContentSize();
}

void MafGridView::removeCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    long uCountOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    ssize_t newIdx = 0;
    
    extension::TableViewCell* cell = this->cellAtIndex(idx);
    if (!cell)
    {
        return;
    }
    
    newIdx = _cellsUsed.getIndex(cell);
    
    //remove first
    this->_moveCellOutOfSight(cell);
    
    _indices->erase(idx);
    this->_updateCellPositions();
    
    for (ssize_t i = _cellsUsed.size()-1; i > newIdx; i--)
    {
        cell = _cellsUsed.at(i);
        this->_setIndexForCell(cell->getIdx()-1, cell);
    }
}

extension::TableViewCell *MafGridView::dequeueCell()
{
    extension::TableViewCell *cell;
    
    if (_cellsFreed.empty()) {
        cell = NULL;
    } else {
        cell = _cellsFreed.at(0);
        cell->retain();
        _cellsFreed.erase(0);
        cell->autorelease();
    }
    return cell;
}

void MafGridView::_addCellIfNecessary(extension::TableViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    _cellsUsed.pushBack(cell);
    _indices->insert(cell->getIdx());
    _isUsedCellsDirty = true;
}

void MafGridView::_updateContentSize()
{
    Size size = Size::ZERO;
    ssize_t cellsCount = _dataSource->numberOfCellsInTableView(this);
    
    if (cellsCount > 0)
    {
        float maxPosition = _vCellsPositions[cellsCount];
        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
                size = Size(maxPosition, _viewSize.height);
                break;
            default:
                size = Size(_viewSize.width, maxPosition);
                break;
        }
    }
    this->setContentSize(size);
    
	if (_oldDirection != _direction)
	{
		if (_direction == Direction::HORIZONTAL)
		{
			this->setContentOffset(Vec2(0,0));
		}
		else
		{
			this->setContentOffset(Vec2(0,this->minContainerOffset().y));
		}
		_oldDirection = _direction;
	}
    
}

Vec2 MafGridView::_offsetFromIndex(ssize_t index)
{
    Vec2 offset = this->__offsetFromIndex(index);
    
    const Size cellSize = _dataSource->tableCellSizeForIndex(this, index);
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    }
    return offset;
}

Vec2 MafGridView::__offsetFromIndex(ssize_t index)
{
    Vec2 offset;
    Size  cellSize;
    
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            offset = Vec2(_vCellsPositions[index], _hCellsPositions[index]);
            break;
        default:
            offset = Vec2(_hCellsPositions[index], _vCellsPositions[index]);
            //offset = Vec2(0.0f, _vCellsPositions[index]);
            break;
    }
    
    return offset;
}

long MafGridView::_indexFromOffset(Vec2 offset)
{
    long index = 0;
    const long maxIdx = _dataSource->numberOfCellsInTableView(this) - 1;
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y;
    }
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }
    
    return index;
}

long MafGridView::__indexFromOffset(Vec2 searchOffset)
{
    long low = 0;
    long high = _dataSource->numberOfCellsInTableView(this) - 1;

    while (high >= low)
    {
        //long index = low + (high - low) / 2;
        
        float cellStartV,cellEndV,cellStartH,cellEndH;
        
        if (_direction == Direction::HORIZONTAL) {
            cellStartH = _vCellsPositions[low];
            cellEndH = _vCellsPositions[low] + _dataSource->tableCellSizeForIndex(this, low).height;
            
            cellStartV = _hCellsPositions[low];
            cellEndV = _hCellsPositions[low + 1];
        }else{
            cellStartV = _vCellsPositions[low];
            cellEndV = _vCellsPositions[low] + _dataSource->tableCellSizeForIndex(this, low).height;
            //cellEndV = _vCellsPositions[low + _column];
            
            //
            cellStartH = _hCellsPositions[low];
            cellEndH = _hCellsPositions[low + 1];
        }
        
        if (cellEndH <= 0.0f) {
            cellEndH = this->getViewSize().width;
        }
        
        if (searchOffset.y < 0) {
            return 0;
        }
        if (searchOffset.x < 0) {
            return 0;
        }

        if (searchOffset.y >= cellStartV && searchOffset.y < cellEndV)
        {
            if (searchOffset.x >= cellStartH && searchOffset.x < cellEndH){
                return low;
            }
        }
        low++;
    }
    
    return -1;
}

void MafGridView::_moveCellOutOfSight(extension::TableViewCell *cell)
{
    if(_gridViewDelegate != NULL) {
        _gridViewDelegate->tableCellWillRecycle(this, cell);
    }
    
    _cellsFreed.pushBack(cell);
    _cellsUsed.eraseObject(cell);
    _isUsedCellsDirty = true;
    
    _indices->erase(cell->getIdx());
    cell->reset();
    
    if (cell->getParent() == this->getContainer())
    {
        this->getContainer()->removeChild(cell, true);;
    }
}

void MafGridView::_setIndexForCell(ssize_t index, extension::TableViewCell *cell)
{
    cell->setAnchorPoint(Vec2(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}

void MafGridView::_updateCellPositions()
{
    int columnCount = 0;
    long cellsCount = _dataSource->numberOfCellsInTableView(this);
    _vCellsPositions.resize(cellsCount + 1, 0.0);
    _hCellsPositions.resize(cellsCount + 1, 0.0);
    if (cellsCount > 0)
    {
        float currentPos = 0;
        float currentPosH = 0;
        Size cellSize;
        for (int i=0; i < cellsCount; i++)
        {
            columnCount++;
            
            _vCellsPositions[i] = currentPos;
            _hCellsPositions[i] = currentPosH;
            cellSize = _dataSource->tableCellSizeForIndex(this, i);
            switch (this->getDirection())
            {
                case Direction::HORIZONTAL:
                    if (columnCount == _column) {
                        columnCount = 0;
                        currentPosH = 0;
                        currentPos += cellSize.width;
                    }else{
                        currentPosH += cellSize.height;
                    }
                    break;
                default:
                    if (columnCount == _column) {
                        columnCount = 0;
                        currentPosH = 0;
                        currentPos += cellSize.height;
                    }else{
                        currentPosH += cellSize.width;
                    }
                    break;
            }
        }
        if (columnCount != 0) {
            currentPos += cellSize.height;
        }
        _vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
        _hCellsPositions[cellsCount] = currentPosH;
    }
}
void MafGridView::setColumn(int column){
    _column = column;
}
void MafGridView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems)
    {
        return;
    }
    
    if (_isUsedCellsDirty)
    {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](extension::TableViewCell *a, extension::TableViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_gridViewDelegate != NULL) {
        _gridViewDelegate->scrollViewDidScroll(this);
    }
    
    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    }
    
    startIdx = this->_indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX)
	{
		startIdx = countOfItems - 1;
	}
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += _viewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += _viewSize.width/this->getContainer()->getScaleX();

    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = countOfItems - 1;
	}
#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        extension::TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        i++;
    }
    
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        extension::TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        i++;
    }
#endif
    
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.at(0);
        idx = cell->getIdx();
        
        while(idx < startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.back();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    
    for (long i = startIdx; i <= endIdx; i++)
    {
        if (_indices->find(i) != _indices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}

void MafGridView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    
    if (_touchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);
        
		if (bb.containsPoint(pTouch->getLocation()) && _gridViewDelegate != NULL)
        {
            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
            _gridViewDelegate->tableCellTouched(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool MafGridView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);
    
    if(_touches.size() == 1)
    {
        long index;
        Vec2 point;
        
        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        index = this->_indexFromOffset(point);
        
		if (index == CC_INVALID_INDEX)
		{
			_touchedCell = NULL;
		}
        else
		{
			_touchedCell  = this->cellAtIndex(index);
		}
        
        if (_touchedCell && _gridViewDelegate != NULL)
        {
            _gridViewDelegate->tableCellHighlight(this, _touchedCell);
        }
    }
    else if (_touchedCell)
    {
        if(_gridViewDelegate != NULL)
        {
            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
    return touchResult;
}

void MafGridView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);
    
    if (_touchedCell && isTouchMoved())
    {
        if(_gridViewDelegate != NULL)
        {
            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
}

void MafGridView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);
    
    if (_touchedCell)
    {
        if(_gridViewDelegate != NULL)
        {
            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
}

NS_CC_EXT_END
