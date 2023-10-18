//
//  InfoDefenseManaAmpSton.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#ifndef InfoDefenseManaAmpStone_hpp
#define InfoDefenseManaAmpStone_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseManaAmpStone : public cocos2d::Ref
{
public:
    static InfoDefenseManaAmpStone* create();
    
    InfoDefenseManaAmpStone();
    virtual ~InfoDefenseManaAmpStone();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int value);
    
    E_DF_AMP_STONE_Type getType();
    
    int getBonusSuccessRate() const;
    void setBonusSuccessRate(const int value);
    
    int getSuccessIncreaseRate() const;
    void setSuccessIncreaseRate(const int value);
    
    int getSuccessDecreaseRate() const;
    void setSuccessDecreaseRate(const int value);
    
    int getFailReplaceIndex() const;
    void setFailReplaceIndex(const int value);
    
    float getOutputQuantity() const;
    void setOutputQuantity(const float value);
private:
    int _index;
    int _bonusSuccessRate;
    int _successIncreaseRate;
    int _successDecreaseRate;
    int _failReplaceIdx;
    float _outputQuantity;
};

#endif /* InfoDefenseManaAmpSton_hpp */
