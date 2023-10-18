//
//  InfoFrostFloor.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#ifndef InfoFrostFloor_h
#define InfoFrostFloor_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoFrostFloor : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoFrostFloor);
    
    InfoFrostFloor();
    virtual ~InfoFrostFloor();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
    
    bool isBoss();
    void setBoss(int value);
    
    InfoItems* getCost();
    void setCost(std::string value);
    
    std::string getReward();
    void setReward(std::string value);
    
    std::string getHP();
    void setHPControl(int value);
   
    
private:
    int _idx;
    
    bool _bBoss;
    
    std::string _cost;
    std::string _reward;
    
    int _hpControl;
    
};

#endif /* InfoFrostFloor_hpp */
