//
//  UserInfoConfigManager.hpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#ifndef UserInfoConfigManager_hpp
#define UserInfoConfigManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

enum class E_FUNCTION_ACTIVE
{
    SPEECH,
    BATTERY_AUTO,
    DAMAGE,
    ATTACK_SKIN,
    ATTACK_WEAPON,
    OFFLINE
};

enum class E_ALARM_ACTIVE
{
    ALL,
    PRISON_ENTER,
    JELLY,
    BONGGIOK,
    RAID,
};

class UserInfoConfigManager
{
public:
    static UserInfoConfigManager *getInstance();
    static void destroyInstance();
    
    UserInfoConfigManager();
    virtual ~UserInfoConfigManager();
    virtual bool init();
    
public:
    // function
    bool isFunction(E_FUNCTION_ACTIVE eType);
    void setFunction(E_FUNCTION_ACTIVE eType, bool bActive);
    
    // alarm
    bool isAlarm(E_ALARM_ACTIVE eType);
    bool isAlarmOriginal(E_ALARM_ACTIVE eType);
    void setAlarm(E_ALARM_ACTIVE eType, bool bActive);
    
    // other
    time_t getSaveTime();
    int getSaveTimeDiff();
    void setSaveTime(time_t sec);
    
    E_NEWBIE_STEP getNewbieStep();
    void setNewbieStep(E_NEWBIE_STEP step);
    

private:
    // function 말풍선
    bool _bFunctionSpeech;
    // function 절전 모드
    bool _bFunctionBattery;
    // function 데미지 표시
    bool _bFunctionDamage;
    // function 스킨 공격 이팩트
    bool _bFunctionAttackSkin;
    // function 타격 이팩트
    bool _bFunctionAttackWeapon;
    // alarm 오프라인
    bool _bFunctionOffline;
    
    // alarm 전체
    bool _bAlarmAll;
    // alarm 지하감옥 입장
    bool _bAlarmPrisonEnter;
    // alarm 지하감옥 젤리
    bool _bAlarmJelly;
    // alarm 레이드 봉기옥
    bool _bAlarmBonggiok;
    // alarm 레이드 처치
    bool _bAlarmRaid;
    
    
    E_NEWBIE_STEP _eNewbie;
};


#endif /* UserInfoConfigManager_hpp */
