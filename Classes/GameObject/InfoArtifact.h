//
//  InfoArtifact.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoArtifact_h
#define InfoArtifact_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoArtifact : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoArtifact);
    
    InfoArtifact();
    virtual ~InfoArtifact();
    virtual bool init();
    
public:
    E_ARTIFACT getIdx();
    void setIdx(E_ARTIFACT value);
    
    E_ARTIFACT_TYPE getType();
    void setType(E_ARTIFACT_TYPE value);
    
    std::string getTitle();
    void setTitleKey(std::string value);
    
    std::string getDesc();
    void setDescKey(std::string value);
    
    std::string getIconPath();
    void setIconPath(std::string value);
    
    int getGeneration();
    void setGeneration(int value);
    
    int getCondition();
    void setCondition(int value);
    
    int getLevelMax();
    void setLevelMax(int value);
    
    int getLevelReinforceMax();
    void setLevelReinforceMax(int value);
    
    E_ITEMS getPriceType();
    void setPriceType(E_ITEMS value);
    
    int getPricePoint();
    void setPricePoint(int value);
    
    int getPrevArtifact();
    void setPrevArtifact(int value);
    
    bool isCalculator();
    void setCalculator(bool value);
    
    bool isReinforce();
    void setReinforce(bool value);
    
private:
    E_ARTIFACT _idx;
    E_ARTIFACT_TYPE _type;
    
    std::string _title;
    std::string _desc;
    std::string _iconPath;
    
    int _generation;
    int _condition;
    int _levelMax;
    int _levelReinforceMax;
    
    E_ITEMS _price_type;
    int _price_point;
    
    int _prev_artifact;
    
    bool _is_calculator;
    bool _is_reinforce;
    
};
#endif /* InfoArtifact_h */
