//
//  InfoBoard.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/01.
//

#ifndef InfoBoard_h
#define InfoBoard_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoBoard : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoBoard);
    
    InfoBoard();
    virtual ~InfoBoard();
    virtual bool init();

public:
    int getIdx();
    void setIdx(int nIdx);
    
    InfoItems* getItems();
    void setItems(int nIdx, int nTypeParams, int nCount);
    
    int getTilePosX();
    int getTilePosY();
    void setTilePos(std::string strPos);
    
    cocos2d::Sprite* getTile();
    void setTile(cocos2d::Sprite* spriteTile);
    
private:
    TInteger _nIdx;
    
    TInteger _nItemIdx;
    TInteger _nItemTypeParams;
    TInteger _nItemCount;
    
    int _nTilePosX;
    int _nTilePosY;
    
    cocos2d::Sprite* _spriteTile;
};
 

#endif /* InfoBoard_h */
