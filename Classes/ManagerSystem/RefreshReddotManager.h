#ifndef RefreshReddotManager_h
#define RefreshReddotManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigEnum.h"

class RefreshReddotManager : public MafSingleton<RefreshReddotManager>
{
public:
    RefreshReddotManager(void);
    virtual ~RefreshReddotManager(void);
    virtual bool init();
    
public:
    void addUI(E_REFRESH_REDDOT eType, cocos2d::Node* node);
    void refreshUI(E_REFRESH_REDDOT eType);
    
    void setValue(E_REFRESH_REDDOT eType, bool bValue);
    
    bool isReddotVisible(E_REFRESH_REDDOT eType);
    bool checkEquipCraft(E_PLACE ePlace);
    bool checkPetReinforce(E_PLACE ePlace, E_PET ePet);
    bool checkPetReinforceAll();
    bool checkPetEvolution(E_PLACE ePlace, E_PET ePet);
    bool checkPetEvolutionAll();
    bool checkPetEquip(E_PLACE ePlace, E_PET ePet);
    bool checkPetEquipAll();
    
    bool checkDungeonPassReward();
    bool checkPetEquipBox();

private:
    
};

#endif /* RefreshReddotManager_h */
