//
//  Grid.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#include "Grid.h"

#include "GameObject/Grid/InfoGridTileData.h"

Grid* Grid::create()
{
    Grid *pRet = new(std::nothrow) Grid();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


Grid::Grid()
{
    _gridCriteriaCornerLocation = _gridCenterLocation = Vec2::ZERO;
    
    _gridTileSize = Vec2(86.f, 86.f);
    _gridTileCount = IntPoint(9, 9);
    _activateTileNum = 0;
}
Grid::~Grid()
{
    for(auto data : _gridTiles)
    {
        data.second->release();
    }
    _gridTiles.clear();
}

bool Grid::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    spawnGrid(getPosition(), _gridTileSize, _gridTileCount);
    return true;
}
void Grid::onEnter()
{
    
}

void Grid::update(float dt)
{
    Node::update(dt);
}

void Grid::spawnGrid(const Vec2 centerLocation, const Vec2 tileSize, const IntPoint tileCount, bool isCriteriaLeftBottom)
{
    IntPoint index = IntPoint(0);
    
    _gridCenterLocation = centerLocation;
    _gridTileSize = tileSize;
    _gridTileCount = tileCount;
    
    destroyGrid();
    
    if(isCriteriaLeftBottom)
        calculateCenterAndBottomLeft();
    else
        calculateCenterAndTopRight();

    const int firstIndex = 0;
    const int lastIndex = _gridTileCount.Y;

    for(int X = 0; X < _gridTileCount.X; ++X)
    {
        for(int Y = firstIndex; Y < lastIndex; ++Y)
        {
            index = IntPoint(X, Y);
            
            Vec2 location = getTileLocationFromGridIndex(index);

            InfoGridTileData* data = InfoGridTileData::create();
            data->setIndex(index);
            data->setLocation(location);
            data->retain();
            addGridTile(data);
            
        }
    }
    //showTileVisual();
    _onGridGenerated_.Invoke();
}

void Grid::addGridTile(InfoGridTileData* data)
{
    _gridTiles.insert(std::pair(data->getIndex(), data));

    _onTileDataUpdated_.Invoke(data->getIndex());
}

void Grid::removeGridTile(const IntPoint& index)
{
    if(!_gridTiles.contains(index))
        return;

    const auto data = getGridTileData(index);
    data->resetData();
    data->setIndex(index);
    
    _onTileDataUpdated_.Invoke(data->getIndex());
}

void Grid::destroyGrid()
{
    for(auto data : _gridTiles)
    {
        data.second->release();
    }
    _gridTiles.clear();
    
    _onGridDestroyed_.Invoke();
}
void Grid::calculateCenterAndBottomLeft()
{
    //_gridCenterLocation = MafMath::SnapVectorToVector(_gridCenterLocation, _gridTileSize);
    Vec2 tmp = Vec2(_gridTileCount.X, _gridTileCount.Y);
    
    tmp.x -= MafMath::FloatSelect(MafMath::IsIntEven(_gridTileCount.X), 0.f, 1.f);
    tmp.y -= MafMath::FloatSelect(MafMath::IsIntEven(_gridTileCount.Y), 0.f, 1.f);
    tmp /= 2;
    tmp *= _gridTileSize;
    _gridCriteriaCornerLocation = _gridCenterLocation - tmp;
}

void Grid::calculateCenterAndTopRight()
{
    //_gridCenterLocation = MafMath::SnapVectorToVector(_gridCenterLocation, _gridTileSize);
    Vec2 tmp = Vec2(_gridTileCount.X, _gridTileCount.Y);
    
    tmp.x -= MafMath::FloatSelect(MafMath::IsIntEven(_gridTileCount.X), 0.f, 1.f);
    tmp.y -= MafMath::FloatSelect(MafMath::IsIntEven(_gridTileCount.Y), 0.f, 1.f);
    tmp /= 2;
    tmp *= _gridTileSize;
    _gridCriteriaCornerLocation = _gridCenterLocation + tmp;
}

Vec2 Grid::getTileLocationFromGridIndex(const IntPoint index, bool isCriteriaLeftBottom) const
{
    if(isCriteriaLeftBottom)
        return _gridCriteriaCornerLocation + _gridTileSize * Vec2(index.X, index.Y);
    else
        return _gridCriteriaCornerLocation - _gridTileSize * Vec2(index.X, index.Y);
}

bool Grid::isIndexValid(const IntPoint index) const
{
    return _gridTiles.contains(index);
}

void Grid::showTileVisual()
{
    removeAllChildren();
    for(auto data : _gridTiles)
    {
        auto tile = ui::Scale9Sprite::create("Assets/ui/defense/white_line.png", Rect::ZERO, Rect(2,2,1,1));
        tile->setContentSize(Size(getGridTileSize()));
        tile->setPosition(data.second->getLocation());
        tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(tile);
        
        auto label = Label::createWithTTF(data.second->getIndex().ToString(), GAME_FONT, 20);
        label->setPosition(tile->getContentSize()/2);
        tile->addChild(label);
        
        
    }
}
