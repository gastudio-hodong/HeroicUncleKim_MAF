//
//  InfoEventSunfish.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef InfoEventSunfish_hpp
#define InfoEventSunfish_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventSunfish : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventSunfish);
    
    InfoEventSunfish();
    virtual ~InfoEventSunfish();
    virtual bool init();
        
public:
    int getType();
    void setType(int value);
    
    int getLevel();
    void setLevel(int value);
    
    double getWeight();
    void setWeight(double value);
    
    int getSurvivalRate();
    void setSurvivalRate(int value);
    
    int getBonusRate();
    void setBonusRate(int value);
    
    int getBonusWeight();
    void setBonusWeight(int value);
    
    int getCost();
    void setCost(int value);
    
    int getPoint();
    void setPoint(int value);
    
    std::string getName();
    void setNameKey(std::string key);
    
    std::string getSkinPath();
    std::string getIconPath();
    
    std::string getDieDialogue();
    void setDieDialogueKey(std::string key);
    
    std::string getDieText();
    void setDieTextKey(std::string key);
    
    std::string getIllustratedTitleText();
    void setIllustratedTitleTextKey(std::string key);
    
    std::string getIllustratedText();
    void setIllustratedTextKey(std::string key);
    
    std::string getSurvivalText();
    void setSurvivalTextKey(std::string key);
    
    cocos2d::Vector<InfoItems*> getReward();
    void setReward(const std::string value);
    
    cocos2d::Vector<InfoItems*> getRewardDie();
    void setRewardDie(const std::string value);
    
private:
    TInteger _type;
    TInteger _level;
    
    double _weight;
    TInteger _survival_rate;
    TInteger _bonus_rate;
    TInteger _bonus_weight;
    
    TInteger _cost;
    TInteger _point;
    
    std::string _nameKey;
    std::string _die_dialogue;
    std::string _die_text;
    std::string _illustrated_title_text;
    std::string _illustrated_text;
    std::string _survival_text;
    
    std::string _reward;
    std::string _reward_die;
    
};

#endif /* InfoEventSunfish_hpp */
