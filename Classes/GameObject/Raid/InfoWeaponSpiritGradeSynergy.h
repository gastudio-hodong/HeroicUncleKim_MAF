//
//  InfoWeaponSpiritGradeSynergy.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/25.
//

#ifndef InfoWeaponSpiritGradeSynergy_hpp
#define InfoWeaponSpiritGradeSynergy_hpp

#include "Common/ConfigDefault.h"

class InfoWeaponSpiritGradeSynergy : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoWeaponSpiritGradeSynergy);
    
    InfoWeaponSpiritGradeSynergy();
    virtual ~InfoWeaponSpiritGradeSynergy();
    virtual bool init();
    
    
public:
    int getGrade();
    void setGrade(int value);
    
    double getIncreasesLv();
    void setIncreasesLv(double value);
    
    double getIncreasesSmelting();
    void setIncreasesSmelting(double value);
    
private:
    int _nGrade;
    TDouble _nIncreasesLv;
    TDouble _nIncreasesSmelting;
};

#endif /* InfoWeaponSpiritGradeSynergy_hpp */
