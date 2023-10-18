//
//  InfoEventVoteAll.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef InfoEventVoteAll_hpp
#define InfoEventVoteAll_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventVoteAll : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventVoteAll);
    
    InfoEventVoteAll();
    virtual ~InfoEventVoteAll();
    virtual bool init();
        
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    
    const int getCount() const;
    void setCount(const int value);
    
    cocos2d::Vector<InfoItems*> getReward();
    void setReward(const std::string value);
private:
    int _idx;
    int _count;
    
    std::string _reward;
    
};

#endif /* InfoEventVoteAll_hpp */
