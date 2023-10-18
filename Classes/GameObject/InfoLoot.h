//
//  InfoLoot.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#ifndef InfoLoot_hpp
#define InfoLoot_hpp

#include "Common/ConfigDefault.h"

class InfoLoot : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoLoot);

    InfoLoot();
    virtual ~InfoLoot();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
    
    int getGrade();
    void setGrade(int value);
    
    std::string getName();
    void setNameKey(std::string value);
    
    std::string getDesc();
    void setDescKey(std::string value);
    
    double getEffectDefault();
    void setEffectDefault(double value);
    
    double getEffectIncreasing();
    void setEffectIncreasing(double value);
    
    int getLevelMax();
    void setLevelMax(int level);
    
    
private:
    int _idx;
    int _grade;
    
    std::string _nameKey;
    std::string _descKey;
    
    double _effectDefault;
    double _effectIncreasing;
    
    int _levelMax;
    
    
};

#endif /* InfoLoot_hpp */
