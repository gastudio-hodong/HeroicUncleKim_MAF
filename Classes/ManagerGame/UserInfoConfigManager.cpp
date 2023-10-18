//
//  UserInfoConfigManager.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#include "UserInfoConfigManager.h"

#include "Common/ConfigKey.h"

USING_NS_CC;

static UserInfoConfigManager *s_SharedUserInfoConfig = nullptr;
UserInfoConfigManager* UserInfoConfigManager::getInstance()
{
    if (!s_SharedUserInfoConfig)
    {
        s_SharedUserInfoConfig = new (std::nothrow) UserInfoConfigManager();
        s_SharedUserInfoConfig->init();
    }
    
    return s_SharedUserInfoConfig;
}

void UserInfoConfigManager::destroyInstance()
{
    if ( s_SharedUserInfoConfig != nullptr )
    {
        delete s_SharedUserInfoConfig;
        s_SharedUserInfoConfig = nullptr;
    }
}

UserInfoConfigManager::UserInfoConfigManager()
{
    
}

UserInfoConfigManager::~UserInfoConfigManager()
{

}

bool UserInfoConfigManager::init()
{
    // 기능 on / off
    {
        // function 말풍선
        _bFunctionSpeech = UserDefault::getInstance()->getBoolForKey("speech_bubble", true);
        
        // function 절전 모드
        _bFunctionBattery = UserDefault::getInstance()->getBoolForKey("BATTERY_AUTO", true);
        
        // function 데미지 표시
        _bFunctionDamage = UserDefault::getInstance()->getBoolForKey("DAMAGE", true);
        
        // function 스킨 공격 이팩트
        _bFunctionAttackSkin = UserDefault::getInstance()->getBoolForKey("SKIN_ATTACK_EFFECT", true);
        
        // function 타격 이팩트
        _bFunctionAttackWeapon = UserDefault::getInstance()->getBoolForKey("WEAPON_ATTACK_EFFECT", true);
        
        // alarm 오프라인
        _bFunctionOffline = UserDefault::getInstance()->getBoolForKey("OFFLINE", true);
    }
    
    // 알람 on /off
    {
        _bAlarmAll = UserDefault::getInstance()->getBoolForKey("PUSH_ALL", true);
        // alarm 지하감옥 입장
        _bAlarmPrisonEnter = UserDefault::getInstance()->getBoolForKey("PUSH_PRISON_ENTER", true);
        // alarm 지하감옥 젤리
        _bAlarmJelly = UserDefault::getInstance()->getBoolForKey("PUSH_PRISON_JELLY", true);
        // alarm 레이드 봉기옥
        _bAlarmBonggiok = UserDefault::getInstance()->getBoolForKey("PUSH_RAID_BONGGIOK", false);
        // alarm 레이드 처치
        _bAlarmRaid = UserDefault::getInstance()->getBoolForKey("PUSH_RAID", false);
    }
    
    
    return true;
}

#pragma mark - function
bool UserInfoConfigManager::isFunction(E_FUNCTION_ACTIVE eType)
{
    bool bResult = false;
    switch (eType) {
        case E_FUNCTION_ACTIVE::SPEECH:         bResult = _bFunctionSpeech;             break;
        case E_FUNCTION_ACTIVE::BATTERY_AUTO:   bResult = _bFunctionBattery;            break;
        case E_FUNCTION_ACTIVE::DAMAGE:         bResult = _bFunctionDamage;             break;
        case E_FUNCTION_ACTIVE::ATTACK_SKIN:    bResult = _bFunctionAttackSkin;         break;
        case E_FUNCTION_ACTIVE::ATTACK_WEAPON:  bResult = _bFunctionAttackWeapon;       break;
        case E_FUNCTION_ACTIVE::OFFLINE:        bResult = _bFunctionOffline;            break;
            
        default:
            break;
    }
    
    return bResult;
}

void UserInfoConfigManager::setFunction(E_FUNCTION_ACTIVE eType, bool bActive)
{
    switch (eType) {
        case E_FUNCTION_ACTIVE::SPEECH:
            _bFunctionSpeech = bActive;
            UserDefault::getInstance()->setBoolForKey("speech_bubble", bActive);
            break;
        case E_FUNCTION_ACTIVE::BATTERY_AUTO:
            _bFunctionBattery = bActive;
            UserDefault::getInstance()->setBoolForKey("BATTERY_AUTO", bActive);
            break;
        case E_FUNCTION_ACTIVE::DAMAGE:
            _bFunctionDamage = bActive;
            UserDefault::getInstance()->setBoolForKey("DAMAGE", bActive);
            break;
        case E_FUNCTION_ACTIVE::ATTACK_SKIN:
            _bFunctionAttackSkin = bActive;
            UserDefault::getInstance()->setBoolForKey("SKIN_ATTACK_EFFECT", bActive);
            break;
        case E_FUNCTION_ACTIVE::ATTACK_WEAPON:
            _bFunctionAttackWeapon = bActive;
            UserDefault::getInstance()->setBoolForKey("WEAPON_ATTACK_EFFECT", bActive);
            break;
        case E_FUNCTION_ACTIVE::OFFLINE:
            _bFunctionOffline = bActive;
            UserDefault::getInstance()->setBoolForKey("OFFLINE", bActive);
            break;
            
        default:
            break;
    }
}


#pragma mark - alarm
bool UserInfoConfigManager::isAlarm(E_ALARM_ACTIVE eType)
{
    bool bResult = _bAlarmAll;
    
    if ( bResult == false )
    {
        return bResult;
    }
    
    bResult = isAlarmOriginal(eType);
    return bResult;
}

bool UserInfoConfigManager::isAlarmOriginal(E_ALARM_ACTIVE eType)
{
    bool bResult = false;
    switch (eType) {
        case E_ALARM_ACTIVE::ALL:                   bResult = _bAlarmAll;             break;
        case E_ALARM_ACTIVE::PRISON_ENTER:          bResult = _bAlarmPrisonEnter;             break;
        case E_ALARM_ACTIVE::JELLY:                 bResult = _bAlarmJelly;            break;
        case E_ALARM_ACTIVE::BONGGIOK:              bResult = _bAlarmBonggiok;             break;
        case E_ALARM_ACTIVE::RAID:                  bResult = _bAlarmRaid;         break;
            
        default:
            break;
    }
    
    return bResult;
}

void UserInfoConfigManager::setAlarm(E_ALARM_ACTIVE eType, bool bActive)
{
    switch (eType) {
        case E_ALARM_ACTIVE::ALL:
            _bAlarmAll = bActive;
            UserDefault::getInstance()->setBoolForKey("PUSH_ALL", bActive);
            break;
        case E_ALARM_ACTIVE::PRISON_ENTER:
            _bAlarmPrisonEnter = bActive;
            UserDefault::getInstance()->setBoolForKey("PUSH_PRISON_ENTER", bActive);
            break;
        case E_ALARM_ACTIVE::JELLY:
            _bAlarmJelly = bActive;
            UserDefault::getInstance()->setBoolForKey("PUSH_PRISON_JELLY", bActive);
            break;
        case E_ALARM_ACTIVE::BONGGIOK:
            _bAlarmBonggiok = bActive;
            UserDefault::getInstance()->setBoolForKey("PUSH_RAID_BONGGIOK", bActive);
            break;
        case E_ALARM_ACTIVE::RAID:
            _bAlarmRaid = bActive;
            UserDefault::getInstance()->setBoolForKey("PUSH_RAID", bActive);
            break;
            
        default:
            break;
    }
}



#pragma mark - other
time_t UserInfoConfigManager::getSaveTime()
{
    std::string strSec = UserDefault::getInstance()->getStringForKey(KEY_SAVE_TIME, "0");
    time_t sec = atol(strSec.c_str());
    
    if ( sec == 0 )
    {
        sec = UtilsDate::getInstance()->getTime();
        setSaveTime(sec);
    }
    
    return sec;
}

int UserInfoConfigManager::getSaveTimeDiff()
{
    time_t secNow = UtilsDate::getInstance()->getTime();
    time_t secSave = getSaveTime();
    
    int diff = difftime(secNow, secSave) / 86400;
    
    return diff;
}

void UserInfoConfigManager::setSaveTime(time_t sec)
{
    std::string strSec = MafUtils::format("%ld", sec);
    UserDefault::getInstance()->setStringForKey(KEY_SAVE_TIME, strSec);
}

E_NEWBIE_STEP UserInfoConfigManager::getNewbieStep()
{
    return _eNewbie;
}

void UserInfoConfigManager::setNewbieStep(E_NEWBIE_STEP step)
{
    _eNewbie = step;
}

 
