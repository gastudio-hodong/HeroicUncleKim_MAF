//
//  InfoDefenseMonster.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#ifndef InfoDefenseMonster_hpp
#define InfoDefenseMonster_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseMonster : public cocos2d::Ref
{
public:
    static InfoDefenseMonster* create();
    
    InfoDefenseMonster();
    virtual ~InfoDefenseMonster();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int value);
    
    E_DF_MonsterType getType() const;
    void setType(const E_DF_MonsterType value);
    
    double getHpCoefficient()const;
    void setHpCoefficient(const double value);
    
    double getDefensive()const;
    void setDefensive(const double value);
    
    int getSpeed()const;
    void setSpeed(const int value);
    
private:
    int _index;
    E_DF_MonsterType _type;
    double _hpCoefficient;
    double _defensive;
    int _speed;
};


#endif /* InfoDefenseMonster_hpp */
