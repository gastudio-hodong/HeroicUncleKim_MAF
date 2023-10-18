//
//  InfoEventVoteCandidate.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/05.
//

#ifndef InfoEventVoteCandidate_hpp
#define InfoEventVoteCandidate_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventVoteCandidate : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventVoteCandidate);
    
    InfoEventVoteCandidate();
    virtual ~InfoEventVoteCandidate();
    virtual bool init();
        
public:
    const bool isPrevCandidate();
    void setIsPrevCandidate(const bool value);
    
    const int getIdx();
    void setIdx(const int value);
    
    const int getPersnalVoteAllCount();
    void setPersnalVoteAllCount(const int value);
    
    const int getVoteCount();
    void setVoteCount(const int value);
    
    InfoItems* getReward();
    
private:
    bool _isPrevCandidate;
    int _idx;
    int _nPersnalVoteAllCount;
    int _nVoteCount;
};


#endif /* InfoEventVoteCandidate_hpp */
