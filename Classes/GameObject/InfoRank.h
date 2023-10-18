//
//  InfoRank.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoRank_h
#define InfoRank_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoRank : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRank);
    
    InfoRank();
    virtual ~InfoRank();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int idx);
    
    int getType();
    void setType(int type);
    
    uint64_t getCondition1();
    void setCondition1(uint64_t condition);
    
    uint64_t getCondition2();
    void setCondition2(uint64_t condition);
    
    cocos2d::Vector<InfoItems*> getListReward();
    void setReward(std::string reward);
    
private:
    int _nIdx;
    int _nType;
    
    uint64_t _nCondition1;
    uint64_t _nCondition2;
    
    cocos2d::Vector<InfoItems*> _listReward;
    
};
#endif /* InfoRank_h */
