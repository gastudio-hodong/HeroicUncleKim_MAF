//
//  PetNewDevilManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef PetNewDevilManager_h
#define PetNewDevilManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameDevil.h"

class PetNewDevilManager : public MafSingleton<PetNewDevilManager>
{
public:
    PetNewDevilManager(void);
    virtual ~PetNewDevilManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    
    // info
    
    // get, set : pet
    std::string getPetName(E_PET ePet); //
    std::string getPetExplain(E_PET ePet); //
    std::string getPetUpgradeExplain(E_PET ePet); //
    std::string getPetImagePath(E_PET ePet);
    
    double getPetCoolTime(E_PET ePet);
    double getPetEffect(E_PET ePet, int level);
    
    // get, set : pet level
    int getPetLevel(E_PET ePet);
    int getPetLevelMax(E_PET ePet, int grade);
    void setPetLevel(E_PET ePet, int level);
    
    bool getPet(E_PET ePet);
    int getPetAcquireFloor(E_PET ePet);
    int getPetUpgradePrice(E_PET ePet, int level, int grade);
    
    // get, set : pet grade
    int getPetGrade(E_PET ePet);
    void setPetGrade(E_PET ePet, int grade);
    
    int getPetGradeUpgradePrice(E_PET ePet, int grade);
    
    // event
    int onPetLevelUP(E_PET ePet);
    
    // network
    
    
protected:
    // network
    
    
private:
    E_PLACE _ePlace;
    
    // pet level
    TInteger _uPetLevelDevil[C_COUNT_PET_DEVIL+1];
    TInteger _uPetGradeDevil[C_COUNT_PET_DEVIL+1];
    
    
    // callback
    
};


#endif /* PetNewDevilManager_hpp */
