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

#ifndef __GRIDVIEW_H__
#define __GRIDVIEW_H__


#include "extensions/GUI/CCScrollView/CCScrollView.h"
#include "extensions/GUI/CCScrollView/CCTableViewCell.h"

#include <set>
#include <vector>

NS_CC_EXT_BEGIN

class MafGridView;
class MafGridViewDelegate : public ScrollViewDelegate
{
public:
    virtual void tableCellTouched(MafGridView* table, extension::TableViewCell* cell) = 0;
    virtual void tableCellHighlight(MafGridView* table, extension::TableViewCell* cell){};
    virtual void tableCellUnhighlight(MafGridView* table, extension::TableViewCell* cell){};
    virtual void tableCellWillRecycle(MafGridView* table, extension::TableViewCell* cell){};
    
};


/**
 * Data source that governs table backend data.
 */
class MafGridViewDataSource
{
public:
    virtual ~MafGridViewDataSource() {}
    virtual Size tableCellSizeForIndex(MafGridView *table, ssize_t idx) {
        return cellSizeForTable(table);
    };
    virtual Size cellSizeForTable(MafGridView *table) {
        return Size::ZERO;
    };
    virtual TableViewCell* tableCellAtIndex(MafGridView *table, ssize_t idx) = 0;
    virtual ssize_t numberOfCellsInTableView(MafGridView *table) = 0;
};


class MafGridView : public ScrollView, public ScrollViewDelegate
{
public:
    
    enum class VerticalFillOrder
    {
        NONE = 0,
        
        TOP_DOWN,
        BOTTOM_UP
    };
    
    static MafGridView* create();
    static MafGridView* create(MafGridViewDataSource* dataSource, Size size, int column = 1);
    static MafGridView* create(MafGridViewDataSource* dataSource, Size size, int column, Node *container);

    MafGridView();
    virtual ~MafGridView();
    
    bool initWithViewSize(Size size, Node* container = NULL);
    MafGridViewDataSource* getDataSource() { return _dataSource; }
    
    void setDataSource(MafGridViewDataSource* source) { _dataSource = source; }
    MafGridViewDelegate* getDelegate() { return _gridViewDelegate; }
    
    void setDelegate(MafGridViewDelegate* pDelegate) { _gridViewDelegate = pDelegate; }
    void setVerticalFillOrder(VerticalFillOrder order);
    VerticalFillOrder getVerticalFillOrder();
    
    void updateCell();
    void updateCellAtIndex(ssize_t idx);
    void insertCellAtIndex(ssize_t idx);
    
    void removeCellAtIndex(ssize_t idx);
    void reloadData();
    
    TableViewCell *dequeueCell();
    TableViewCell *cellAtIndex(ssize_t idx);
    
    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view)  override {}
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;

    void setColumn(int column);

protected:
    long __indexFromOffset(Vec2 offset);
    long _indexFromOffset(Vec2 offset);
    Vec2 __offsetFromIndex(ssize_t index);
    Vec2 _offsetFromIndex(ssize_t index);

    void _moveCellOutOfSight(extension::TableViewCell *cell);
    void _setIndexForCell(ssize_t index, extension::TableViewCell *cell);
    void _addCellIfNecessary(extension::TableViewCell * cell);

    void _updateCellPositions();



    int _column;

    extension::TableViewCell *_touchedCell;
    
    VerticalFillOrder _vordering;
    std::set<ssize_t>* _indices;

    std::vector<float> _vCellsPositions;
    std::vector<float> _hCellsPositions;

    Vector<TableViewCell*> _cellsUsed;
    Vector<TableViewCell*> _cellsFreed;

    MafGridViewDataSource* _dataSource;
    MafGridViewDelegate* _gridViewDelegate;

    Direction _oldDirection;

    bool _isUsedCellsDirty;

public:
    void _updateContentSize();

};


NS_CC_EXT_END

#endif /* __CCTABLEVIEW_H__ */
