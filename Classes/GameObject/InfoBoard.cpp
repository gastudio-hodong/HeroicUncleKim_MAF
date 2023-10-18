//
//  InfoBoard.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/01.
//

#include "InfoBoard.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

#pragma mark - InfoBoard
InfoBoard::InfoBoard():
_nIdx(0),

_nItemIdx(0),
_nItemTypeParams(0),
_nItemCount(0),

_spriteTile(nullptr)
{
    
}

InfoBoard::~InfoBoard()
{
    
}

bool InfoBoard::init()
{
    return true;
}
 
int InfoBoard::getIdx()
{
    return (int)_nIdx;
}
void InfoBoard::setIdx(int nIdx)
{
    _nIdx = nIdx;
}
 
InfoItems* InfoBoard::getItems()
{
    auto objItem = ItemsManager::getInstance()->getInfoItems(_nItemIdx);
    if ( objItem != nullptr )
    {
        objItem->setTypeParams(_nItemTypeParams);
        objItem->setCount(_nItemCount);
    }
    
    return objItem;
}

void InfoBoard::setItems(int nIdx, int nTypeParams, int nCount)
{
    _nItemIdx = nIdx;
    _nItemTypeParams = nTypeParams;
    _nItemCount = nCount;
}

int InfoBoard::getTilePosX()
{
    return _nTilePosX;
}
int InfoBoard::getTilePosY()
{
    return _nTilePosY;
}
void InfoBoard::setTilePos(std::string strPos)
{
    auto listTemp = MafUtils::split(strPos, ',');
    _nTilePosX = atoi(listTemp.at(0).c_str());
    _nTilePosY = atoi(listTemp.at(1).c_str());
}

cocos2d::Sprite* InfoBoard::getTile()
{
    return _spriteTile;
}
void InfoBoard::setTile(cocos2d::Sprite* spriteTile)
{
    _spriteTile = spriteTile;
}
