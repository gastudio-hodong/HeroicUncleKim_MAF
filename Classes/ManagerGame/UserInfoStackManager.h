//
//  UserInfoStackManager.hpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#ifndef UserInfoStackManager_hpp
#define UserInfoStackManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UserInfoStackManager
{
public:
    static UserInfoStackManager *getInstance();
    static void destroyInstance();

    UserInfoStackManager();
    virtual ~UserInfoStackManager();

private:    
    bool _bPrincessEvolutionActive;
    int _uDragonSkill;
    
    int _uRevivalAttackBuffCount;
    int _uRevivalQuestBuffCount;
    int _uRevivalKeyBuffCount1;
    int _uRevivalKeyBuffCount2;
    int _uERevivalAttackBuffCount;
    int _uDragonBuffCount;
    int _uHellBuffCount;
    
    int _uHitBuffCount;
    int _uQuest25Count;
    int _uReviveSpecial;
    int _uReviveKeyBooster;
    
    int _iHeroHitCount;
    int _uSetItemEffectIdx25Count;
    int _iTotalAttackCount;
    
    CC_SYNTHESIZE(int, _iTotalAttackChickBuffTime, TotalAttackChickBuffTime);
    
public:
    
    void loadLocalData();
    
    int getTotalAttackCount();
    void setTotalAttackCount(int value);
    
    void setPrincessEvolutionActive(E_PLACE ePlace, bool value);
    bool getPrincessEvolutionActive(E_PLACE ePlace);
    void setDragonSkillCheck(int value, bool save=true);
    int getDragonSkillCheck();
    
    void setRevivalAttackBuff(int value,bool bSave=true);
    int getRevivalAttackBuff();
    void setRevivalQuestBuff(int value,bool bSave=true);
    int getRevivalQuestBuff();
    void setRevivalKeyBuff1(int value,bool bSave=true);
    int getRevivalKeyBuff1();
    void setRevivalKeyBuff2(int value,bool bSave=true);
    int getRevivalKeyBuff2();
    
    void setEnchantRevivalAttackBuff(int value,bool bSave=true);
    int getEnchantRevivalAttackBuff();
    void setDragonBuff(int value,bool bSave=true);
    int getDragonBuff();
    void setHellBuff(int value,bool bSave=true);
    int getHellBuff();
    void setHitBuff(int value,bool bSave=true);
    int getHitBuff();
    void setQuest25Buff(int value,bool bSave=true);
    int getQuest25Buff();
    void setReviveSpecial(int value,bool bSave=true);
    int getReviveSpecial();
    void setReviveKeyBooster(int value,bool bSave=true);
    int getReviveKeyBooster();    
    void setSetItemEffectIdx25Count(int value,bool bSave=true);
    int getSetItemEffectIdx25Count();
    
    void addHeroHitCount();
    
};


#endif /* UserInfoStackManager_hpp */
