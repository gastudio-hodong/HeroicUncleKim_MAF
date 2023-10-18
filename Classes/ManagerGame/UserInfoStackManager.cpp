//
//  UserInfoStackManager.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#include "UserInfoStackManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

static UserInfoStackManager *s_Instance = nullptr;
UserInfoStackManager* UserInfoStackManager::getInstance(){
    if(s_Instance==nullptr){
        s_Instance = new UserInfoStackManager();
    }
    return s_Instance;
}

void UserInfoStackManager::destroyInstance()
{
    if ( s_Instance != nullptr )
    {
        delete s_Instance;
        s_Instance = nullptr;
    }
}

UserInfoStackManager::UserInfoStackManager()
{
    _uDragonSkill = 0;
    
    _uRevivalAttackBuffCount = 0;
    _uRevivalQuestBuffCount = 0;
    _uRevivalKeyBuffCount1 = 0;
    _uRevivalKeyBuffCount2 = 0;
    
    _uERevivalAttackBuffCount = 0;
    _uDragonBuffCount = 0;
    _uHellBuffCount = 0;
    _uHitBuffCount = 0;
    _uQuest25Count = 0;
    _uReviveSpecial = 0;
    _uReviveKeyBooster = 0;
    _iHeroHitCount = 0;
    _iTotalAttackCount = 0;
    _iTotalAttackChickBuffTime = 0;
}
UserInfoStackManager::~UserInfoStackManager(){
    s_Instance = nullptr;
}

void UserInfoStackManager::loadLocalData(){
    
    _bPrincessEvolutionActive = UserDefault::getInstance()->getBoolForKey("princessEvolutionActive", true);
}

void UserInfoStackManager::setPrincessEvolutionActive(E_PLACE ePlace, bool value)
{
    UserDefault::getInstance()->setBoolForKey("princessEvolutionActive", value);
    _bPrincessEvolutionActive = value;
}

bool UserInfoStackManager::getPrincessEvolutionActive(E_PLACE ePlace)
{
    bool bResult = false;
    if ( PetNewManager::getInstance()->getPetEvolution(E_PET::PRINCESS_4) >= 1 )
    {
        bResult = _bPrincessEvolutionActive;
    }
    
    return bResult;
}

void UserInfoStackManager::setDragonSkillCheck(int value, bool bSave){
    _uDragonSkill =value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_DRAGON_SKILLCHECK, MafAes256::Encrypt(MafUtils::toString(_uDragonSkill)));
    }
}
int UserInfoStackManager::getDragonSkillCheck(){
    return _uDragonSkill;
}

#pragma mark-

void UserInfoStackManager::setRevivalAttackBuff(int value,bool bSave){
    if (value > 15) {
        value = 1;
    }
    _uRevivalAttackBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVAL_ATK_BUFF, MafAes256::Encrypt(MafUtils::toString(_uRevivalAttackBuffCount)));
    }
}
int UserInfoStackManager::getRevivalAttackBuff(){
    if (_uRevivalAttackBuffCount > 15) {
        _uRevivalAttackBuffCount = 15;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uRevivalAttackBuffCount = 15;
    }
    return _uRevivalAttackBuffCount;
}
void UserInfoStackManager::setRevivalQuestBuff(int value,bool bSave){
    if (value > 30) {
        value = 1;
    }
    _uRevivalQuestBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVAL_QUEST_BUFF, MafAes256::Encrypt(MafUtils::toString(_uRevivalQuestBuffCount)));
    }
}
int UserInfoStackManager::getRevivalQuestBuff(){
    if (_uRevivalQuestBuffCount > 30) {
        _uRevivalQuestBuffCount = 30;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uRevivalQuestBuffCount = 30;
    }
    return _uRevivalQuestBuffCount;
}
void UserInfoStackManager::setRevivalKeyBuff1(int value,bool bSave){
    if (value > 10) {
        value = 1;
    }
    _uRevivalKeyBuffCount1 = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVAL_KEY_BUFF1, MafAes256::Encrypt(MafUtils::toString(_uRevivalKeyBuffCount1)));
    }
}
int UserInfoStackManager::getRevivalKeyBuff1(){
    if (_uRevivalKeyBuffCount1 > 10) {
        _uRevivalKeyBuffCount1 = 10;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uRevivalKeyBuffCount1 = 10;
    }
    return _uRevivalKeyBuffCount1;
}
void UserInfoStackManager::setRevivalKeyBuff2(int value,bool bSave){
    if (value > 80) {
        value = 80;
    }
    _uRevivalKeyBuffCount2 = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVAL_KEY_BUFF2, MafAes256::Encrypt(MafUtils::toString(_uRevivalKeyBuffCount2)));
    }
}
int UserInfoStackManager::getRevivalKeyBuff2(){
    if (_uRevivalKeyBuffCount2 > 80) {
        _uRevivalKeyBuffCount2 = 80;
    }
    return _uRevivalKeyBuffCount2;
}

void UserInfoStackManager::setEnchantRevivalAttackBuff(int value,bool bSave){
    if (value > 10) {
        value = 1;
    }
    _uERevivalAttackBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_E_REVIVAL_ATK_BUFF, MafAes256::Encrypt(MafUtils::toString(_uERevivalAttackBuffCount)));
    }
}
int UserInfoStackManager::getEnchantRevivalAttackBuff(){
    if (_uERevivalAttackBuffCount > 10) {
        _uERevivalAttackBuffCount = 10;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uERevivalAttackBuffCount = 10;
    }
    return _uERevivalAttackBuffCount;
}
void UserInfoStackManager::setHitBuff(int value,bool bSave){
    if (value > 40) {
        value = 1;
    }
    _uHitBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_HIT_BUFF, MafAes256::Encrypt(MafUtils::toString(_uHitBuffCount)));
    }
}
int UserInfoStackManager::getHitBuff(){
    if (_uHitBuffCount > 40) {
        _uHitBuffCount = 40;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uHitBuffCount = 40;
    }
    return _uHitBuffCount;
}
void UserInfoStackManager::setDragonBuff(int value,bool bSave){
    if (value > 30) {
        value = 1;
    }
    _uDragonBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_DRAGON_BUFF, MafAes256::Encrypt(MafUtils::toString(_uDragonBuffCount)));
    }
}
int UserInfoStackManager::getDragonBuff(){
    if (_uDragonBuffCount > 30) {
        _uDragonBuffCount = 30;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uDragonBuffCount = 30;
    }
    return _uDragonBuffCount;
}

void UserInfoStackManager::setHellBuff(int value,bool bSave){
    if (value > 30) {
        value = 1;
    }
    _uHellBuffCount = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_HELL_BUFF, MafAes256::Encrypt(MafUtils::toString(_uHellBuffCount)));
    }
}
int UserInfoStackManager::getHellBuff(){
    if (_uHellBuffCount > 30) {
        _uHellBuffCount = 30;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uHellBuffCount = 30;
    }
    return _uHellBuffCount;
}

void UserInfoStackManager::setQuest25Buff(int value,bool bSave){
    if (value > 300) {
        value = 300;
    }
    _uQuest25Count = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_QUEST_25_BUFF, MafAes256::Encrypt(MafUtils::toString(_uQuest25Count)));
    }
}
int UserInfoStackManager::getQuest25Buff(){
    if (_uQuest25Count > 300) {
        _uQuest25Count = 300;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_11)) {
        _uQuest25Count = 300;
    }
    return _uQuest25Count;
}

void UserInfoStackManager::setReviveSpecial(int value,bool bSave){
    _uReviveSpecial = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVE_SPECIAL, MafAes256::Encrypt(MafUtils::toString(_uReviveSpecial)));
    }
}
int UserInfoStackManager::getReviveSpecial(){
    return _uReviveSpecial;
}

void UserInfoStackManager::setReviveKeyBooster(int value,bool bSave){
    _uReviveKeyBooster = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_REVIVE_KEYBOOSTER, MafAes256::Encrypt(MafUtils::toString(_uReviveKeyBooster)));
    }
}
int UserInfoStackManager::getReviveKeyBooster(){
    return _uReviveKeyBooster;
}


void UserInfoStackManager::setSetItemEffectIdx25Count(int value,bool bSave){
    _uSetItemEffectIdx25Count = value;
    if(_uSetItemEffectIdx25Count>100){
        _uSetItemEffectIdx25Count = 100;
    }
}
int UserInfoStackManager::getSetItemEffectIdx25Count(){
    return _uSetItemEffectIdx25Count;
}

void UserInfoStackManager::addHeroHitCount(){
    _iHeroHitCount++;
    if (_iHeroHitCount > 50) {
        _iHeroHitCount = 1;
        setHitBuff(getHitBuff()+1);
    }
}

int UserInfoStackManager::getTotalAttackCount()
{
    return _iTotalAttackCount;
}

void UserInfoStackManager::setTotalAttackCount(int value)
{
    if (value > 120000) {
        value = 1;
    }    
    if (value % 1200 == 0) {
        if (CostumeManager::getInstance()->isEquip(HEAD,IDX_38)) {
            _iTotalAttackChickBuffTime = 30;
        }
    }
    if (value % 30000 == 0)
    {
        if (CostumeManager::getInstance()->isEquip(HAND,IDX_36))
        {
            UserInfoManager::getInstance()->addHighArtifact(E_PLACE::DG_NORMAL, E_ARTIFACT_TYPE::ARTI_TYPE_KILL, 1);
        }
    }
    _iTotalAttackCount = value;
}
