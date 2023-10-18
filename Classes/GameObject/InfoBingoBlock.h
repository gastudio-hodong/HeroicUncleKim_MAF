//
//  InfoBingoBlock.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/23.
//

#ifndef InfoBingoBlock_h
#define InfoBingoBlock_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoBingoBlock : public cocos2d::Ref
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        BOARD_BLOCK,
        LINE_REWARD_BLOCK,
        QUEST_REWARD_BLOCK
    };
    
    static InfoBingoBlock* create();
    
    InfoBingoBlock();
    virtual ~InfoBingoBlock();
    virtual bool init();
    
    
public:
    
    InfoBingoBlock::E_TYPE getType();
    void setType(InfoBingoBlock::E_TYPE value);
    
    int getIdx();
    void setIdx(int value);
    
    int getNum();
    void setNum(int value);
    
    int getQuestCount();
    void setQuestCount(int value);
    
    InfoItems* getItem();
    void setItem(InfoItems* value);
    
    bool isReward();
    void setIsReward(bool value);
    //
    
private:
    bool _isReward;
    E_TYPE _eType;
    int _nIdx;
    int _nNum;
    int _nQuestCount;
    InfoItems* _item;
};

#endif /* InfoBingoBlock_hpp */
