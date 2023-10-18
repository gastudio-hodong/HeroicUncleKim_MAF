//
//  InfoMineralRefineUpgrade.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#ifndef InfoMineralRefineUpgrade_h
#define InfoMineralRefineUpgrade_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoMineralRefineUpgrade : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoMineralRefineUpgrade);
    
    InfoMineralRefineUpgrade();
    virtual ~InfoMineralRefineUpgrade();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
    
    std::string getName();
    std::string getDesc();
    std::string getIconPath();
    
    double getEffect();
    void setEffect(double value);
    
    int getLevelMax();
    void setLevelMax(int value);
    
    InfoItems* getCost();
    cocos2d::Vector<InfoItems*> getCostMine();
    void setCost(std::string value);
    
    double getCostIncrease();
    void setCostIncrease(double value);
    
    
    
private:
    int _idx;
    
    double _effect;
    
    int _levelMax;
    
    double _costIncrease;
    
    cocos2d::Vector<InfoItems*> _listCost;
    cocos2d::Vector<InfoItems*> _listCostMine;
};

#endif /* InfoMineralRefineUpgrade_hpp */
