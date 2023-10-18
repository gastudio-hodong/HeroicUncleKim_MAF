//
//  InfoAdventureStage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#ifndef InfoAdventureStage_hpp
#define InfoAdventureStage_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoAdventureStage : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureStage);
    InfoAdventureStage();
    virtual ~InfoAdventureStage();
    virtual bool init();
    
public:
    const int getArea() const;
    void setArea(const int value);
    
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getRequireLv() const;
    void setRequireLv(const int value);
    
    const int getTime() const;
    void setTime(const int value);
    
    const int getEndTime() const;
    void setEndTime(const int value);
    
    const int getExp() const;
    void setExp(const int value);
    
    const bool isRunning() const;
    void setIsRunning(const bool value);
    
    const int getPartyIdx() const;
    void setPartyIdx(const int value);
    
    const bool isComplete() const;
    void setIsComplete(const bool value);
    
    cocos2d::Vector<InfoItems*> getListReward();
    void setReward(std::string reward);
    
    cocos2d::Vector<InfoItems*> getListRewardBossPoint();
    void setRewardBossPoint(std::string reward);
    
    
    const bool isOpen() const;
private:
    bool _isRunning;
    bool _isComplete;
    int _nIdx;
    int _nArea;
    int _nRequireLv;
    int _nTime;
    int _nEndTime;
    int _nExp;
    int _nPartyIdx;
    
    std::string _reward;
    std::string _rewardBossPoint;
};

#endif /* InfoAdventureStage_hpp */
