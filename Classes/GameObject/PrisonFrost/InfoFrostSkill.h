//
//  InfoFrostSkill.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#ifndef InfoFrostSkill_h
#define InfoFrostSkill_h

#include "Common/ConfigDefault.h"

class InfoFrostSkill : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoFrostSkill);
    
    InfoFrostSkill();
    virtual ~InfoFrostSkill();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
     
    int getStep();
    void setStep(int value);
    
    std::string getType();
    void setType(std::string value);
    
    std::string getName();
    void setNameKey(std::string value);
    
    std::string getContents();
    void setContentsKey(std::string value);
    
    std::string getIconPath();
    void setIconPath(std::string value);
    
    int getLevelMax();
    void setLevelMax(int value);
    
    double getValueBasic();
    void setValueBasic(double value);
    
    double getValueLevelPer();
    void setValueLevelPer(double value);
    
    int getCoolTime();
    void setCoolTime(int value);
    
    int getDuration();
    void setDuration(int value);
    
private:
    int _idx;
    int _step;
    
    std::string _type;
    std::string _nameKey;
    std::string _contentsKey;
    std::string _iconPath;
    
    int _levelMax;
    double _valueBasic;
    double _valueLevelPer;
    
    int _coolTime;
    
    int _duration;
    
};

#endif /* InfoFrostSkill_hpp */
