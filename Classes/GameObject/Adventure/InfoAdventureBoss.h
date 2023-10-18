//
//  InfoAdventureBossAbility.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#ifndef InfoAdventureBoss_hpp
#define InfoAdventureBoss_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
 
class InfoItems;
class InfoAdventureBoss : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureBoss);
    
    InfoAdventureBoss();
    virtual ~InfoAdventureBoss() {};
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    bool isHard();
    void setHard(int value);
    
    std::string getName();
    void setNameKey(std::string value);
    
    std::string getIconBannerPath();
    void setIconBannerPath(std::string value);
    
    std::string getIconBattlePath();
    void setIconBattlePath(std::string value);
    
    std::string getIconBattleBGPath();
    void setIconBattleBGPath(std::string value);
    
    int getHpNow();
    void setHpNow(int value);
    
    int getHpMax();
    void setHpMax(int value);
    
    int getWeakness1();
    void setWeakness1(int value);
    
    int getWeakness1CountNow();
    void setWeakness1CountNow(int value);
    
    int getWeakness1CountMax();
    void setWeakness1CountMax(int value);
    
    int getWeakness2();
    void setWeakness2(int value);
    
    int getWeakness2CountNow();
    void setWeakness2CountNow(int value);
    
    int getWeakness2CountMax();
    void setWeakness2CountMax(int value);
    
    cocos2d::Vector<InfoItems*> getListReward();
    void setReward(std::string value);
    
    cocos2d::Vector<InfoItems*> getListRewardBonus();
    void setRewardBonus(std::string value);
    
private:
    TInteger _idx;
    
    TInteger _bHard;
    
    std::string _nameKey;
    std::string _iconBannerPath;
    std::string _iconBattlePath;
    std::string _iconBattleBGPath;
    
    TInteger _hpNow;
    TInteger _hpMax;
    
    TInteger _weakness1;
    TInteger _weakness1CountNow;
    TInteger _weakness1CountMax;
    TInteger _weakness2;
    TInteger _weakness2CountNow;
    TInteger _weakness2CountMax;
    
    std::string _reward;
    std::string _rewardBonus;
};
#endif /* InfoAdventureBoss_hpp */
