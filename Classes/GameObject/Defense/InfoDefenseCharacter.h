//
//  InfoDefenseCharacter.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#ifndef InfoDefenseCharacter_hpp
#define InfoDefenseCharacter_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseCharacter : public cocos2d::Ref
{
public:
    static InfoDefenseCharacter* create();
    
    InfoDefenseCharacter();
    virtual ~InfoDefenseCharacter();
    virtual bool init();
    
public:
    bool isHave() const;
    void setIsHave(const bool value);
    
    int getIndex() const;
    void setIndex(const int value);
    
    int getGrade() const;
    void setGrade(const int value);
    
    int getMaxLv();
    void setMaxLv(const int value);
    
    double getDFAttackIncrease();
    void setDFAttackIncrease(const double value);
    
    int getLvUpCost() const;
    void setLvUpCost(const int value);
    
    int getMaxSmelting() const;
    void setMaxSmelting(const int value);
    
    double getSmeltingBSAttack();
    void setSmeltingBSAttack(const double value);
    
    int getSmeltingCost();
    void setSmeltingCost(const int value);
    
    double getBaseDFAttack();
    void setBaseDFAttack(const double value);
    
    double getBaseDFAttackSpeed();
    void setBaseDFAttackSpeed(const double value);
    
    double getBaseBSAttak();
    void setBaseBSAttak(const double value);
    
    double getBSAttack(int nSmelting = -1);
    double getDFAttack(int nLevel = -1);
    
    int getCurrentCount() ;
    void setCurrentCount(const int value);
    
    int getCurrentLv() ;
    void setCurrentLv(const int value);
    
    int getCurrentSmelting() ;
    void setCurrentSmelting(const int value);
    
    std::string getResourcePath() const;
    std::string getName() const;
    
    bool isReddotCondition();
    void completeFirstReddotCondition();
private:
    bool _isHave;
    int _index;
    int _grade;
    TInteger _maxLv;
    TDouble _lvDFAttackIncrease;
    int _lvUpCost;
    int _maxSmelting;
    TDouble _smeltingBSAttack;
    int _smeltingCost;
    TDouble _baseDFAttack;
    TDouble _baseDFfAttackSpeed;
    TDouble _baseBSAttak;
    TInteger _currentCount;
    TInteger _currentLv;
    TInteger _currentSmelting;
};


#endif /* InfoDefenseCharacter_hpp */
