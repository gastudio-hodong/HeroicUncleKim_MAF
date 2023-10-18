//
//  InfoEventVoteCandidateReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/09.
//

#ifndef InfoEventVoteCandidateReward_hpp
#define InfoEventVoteCandidateReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventVoteCandidateReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventVoteCandidateReward);
    
    InfoEventVoteCandidateReward();
    virtual ~InfoEventVoteCandidateReward();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
    
    InfoItems* getReward(const int idx);
    void insertReward(const int idx, const std::string value);
    
private:
    int _idx;
    std::map<int, std::string> _mapReward;
};


#endif /* InfoEventVoteCandidateReward_hpp */
