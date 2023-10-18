//
//  TimeTravelManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "TimeTravelManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/MonsterManager.h"

USING_NS_CC;

TimeTravelManager::TimeTravelManager(void) :
_nTimeTravelCount(-1),
_nTimeTravelTime(-1),

_strFloor(""),
_strFloorCount(""),

_strRewardTime(""),
_strRewardFloor(""),
_strRewardGem(""),
_strRewardPoint(""),
_strRewardKey(""),
_strRewardGold(""),

_strRewardMonsterSoulCount(""),

_onCallbackCalculation(nullptr)
{
    
}

TimeTravelManager::~TimeTravelManager(void)
{
    
}

bool TimeTravelManager::init()
{
    for ( int i = 0; i < sizeof(_listItemLevel) / sizeof(TInteger); i++ )
    {
        _listItemLevel[i] = 0;
    }
    
    return true;
}

void TimeTravelManager::saveData()
{
    std::stringstream str;
    
    str.str("");
    E_TIME_TRAVEL listType[] = {E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD};
    for ( int i = 0; i < sizeof(listType) / sizeof(E_TIME_TRAVEL); i++ )
    {
        E_TIME_TRAVEL type = listType[i];
        str << _listItemLevel[type] << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_TIME_TRAVEL_LEVEL, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    str << MafUtils::toString(_nTimeTravelCount.value());
    UserDefault::getInstance()->setStringForKey(KEY_TIME_TRAVEL_COUNT, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    str << MafUtils::toString(_nTimeTravelTime.value());
    UserDefault::getInstance()->setStringForKey(KEY_TIME_TRAVEL_TIME, MafAes256::Encrypt(str.str()));
}

void TimeTravelManager::loadData()
{
    std::string str = "";
    
    str = UserDefault::getInstance()->getStringForKey(KEY_TIME_TRAVEL_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        E_TIME_TRAVEL listType[] = {E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD};
        for ( int i = 0; i < sizeof(listType) / sizeof(E_TIME_TRAVEL); i++ )
        {
            E_TIME_TRAVEL type = listType[i];
            
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setItemLevel(type, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setItemLevel(type, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TIME_TRAVEL_TIME);
    if ( !str.empty() )
    {
        setTimeTravelChangeTime(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    else
    {
        setTimeTravelChangeTime(-1);
    }
    
    // time 다음 count 가져오기.!
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TIME_TRAVEL_COUNT);
    if ( !str.empty() )
    {
        if ( _nTimeTravelTime == -1 )
        {
            setTimeTravelCount(-1);
        }
        else
        {
            setTimeTravelCount(std::atoi(MafAes256::Decrypt(str).c_str()));
        }
    }
    else
    {
        setTimeTravelCount(-1);
    }
}

#pragma mark - set, get item
double TimeTravelManager::getItemEffectSub(E_TIME_TRAVEL type, int level)
{
    double value = 0;
    
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:
            value = (level)*20;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:
            value = (level)*40;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:
            value = level;
            break;
            
        default:
            break;
    }
    
    return  value;
}

double TimeTravelManager::getItemEffect(E_TIME_TRAVEL type, int level)
{
    int value = 0;
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:       value = 600; break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:     value = 4800; break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:      value = 3; break;
        default:
            break;
    }
    
    if ( type == E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES )
    {
        value -= getItemEffectSub(type, level);
    }
    else
    {
        value += getItemEffectSub(type, level);
    }
    
    return value;
}

int TimeTravelManager::getItemPrice(E_TIME_TRAVEL type, int level)
{
    int value = 0;
    
    level += 1;
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:
            value = 700 * std::pow(level + 9 * (int)(0.2f * level), 0.4f);
            value = ceil(value / 100.0f) * 100;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:
            value = 400 * std::pow(level + 9 * (int)(0.2f * level), 0.4f);
            value = ceil(value / 100.0f) * 100;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:      value = 5000 * level; break;
        default:
            break;
    }
         
    return value;
}

int TimeTravelManager::getItemMaxLevelCondition()
{
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    int condition = 0;
    
    
    if ( floorHigh <= 500 )
    {
        condition = 500;
    }
    else if ( floorHigh <= 2000 )
    {
        condition = 2000;
    }
    else if ( floorHigh <= 3500 )
    {
        condition = 3500;
    }

    return condition;
}

int TimeTravelManager::getItemMaxLevel(E_TIME_TRAVEL type)
{
    int max = 0;
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:
            if ( floorHigh > 3500 )         max = 30;
            else if ( floorHigh > 2000 )    max = 20;
            else if ( floorHigh > 500 )     max = 10;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:
            if ( floorHigh > 3500 )         max = 30;
            else if ( floorHigh > 2000 )    max = 20;
            else if ( floorHigh > 500 )     max = 10;
            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:
            if ( floorHigh > 3500 )         max = 6;
            else if ( floorHigh > 2000 )    max = 4;
            else if ( floorHigh > 500 )     max = 2;
            break;
        default:
            break;
    }
    
    return max;
}

int TimeTravelManager::getItemLevel(E_TIME_TRAVEL type)
{
    int level = _listItemLevel[type].valueInt();
    int levelMax = getItemMaxLevel(type);
    
    if ( level > levelMax )
    {
        _listItemLevel[type] = levelMax;
        level = levelMax;
    }
    
    return level;
}

void TimeTravelManager::setItemLevel(E_TIME_TRAVEL type, int level)
{
    _listItemLevel[type] = level;
}

std::string TimeTravelManager::getItemNameKey(E_TIME_TRAVEL type)
{
    std::string strKey = "";
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:       strKey = "t_ui_time_jump_skill_timeup_title";            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:     strKey = "t_ui_time_jump_skill_chargetime_title";            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:      strKey = "t_ui_time_jump_skill_maxcount_title";            break;
        default:
            break;
    }
    
    return strKey;
}

std::string TimeTravelManager::getItemOptionKey(E_TIME_TRAVEL type)
{
    std::string strKey = "";
    switch (type) {
        case E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD:       strKey = "t_ui_time_jump_skill_timeup_mag";            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES:     strKey = "t_ui_time_jump_skill_chargetime_mag";            break;
        case E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD:      strKey = "t_ui_time_jump_skill_maxcount_mag";            break;
        default:
            break;
    }
    
    return strKey;
}
 
#pragma mark - set, get time travel
int TimeTravelManager::getTimeTravelChangeTime()
{
    int count = getTimeTravelCount();
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    int timeMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES));
    
    if ( count >= countMax )
    {
        return timeMax;
    }
    else
    {
        int64_t timeNow = UtilsDate::getInstance()->getTime();
        int64_t timePrev = _nTimeTravelTime;
        int64_t timeDiff = timeNow - timePrev;
        
        return timeMax - (timeDiff % timeMax);
    }
}

void TimeTravelManager::setTimeTravelChangeTime(int64_t time)
{
    _nTimeTravelTime = time;
}

int TimeTravelManager::getTimeTravelCount()
{
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    if ( _nTimeTravelCount > countMax )
    {
        _nTimeTravelCount = countMax;
    }
    
    int count = _nTimeTravelCount.valueInt();
    
    int64_t timeNow = UtilsDate::getInstance()->getTime();
    int64_t timePrev = _nTimeTravelTime;
    int timeDiff = (int)(timeNow - timePrev);
    if ( timePrev != -1 && timeDiff > 0 )
    {
        int timeMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES));
        
        count += timeDiff / timeMax;
        if ( count > countMax )
        {
            count = countMax;
        }
    }
    
    return count;
}

int TimeTravelManager::getTimeTravelCountMax()
{
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    
    return countMax;
}

void TimeTravelManager::setTimeTravelCount(int64_t count)
{
    _nTimeTravelCount = count;
    if ( _nTimeTravelCount == -1 )
    {
        _nTimeTravelCount = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    }
    
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    if ( _nTimeTravelCount > countMax )
    {
        _nTimeTravelCount = countMax;
    }
}

void TimeTravelManager::addTimeTravelCount()
{
    _nTimeTravelCount += 1;
    
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    if ( _nTimeTravelCount > countMax )
    {
        _nTimeTravelCount = countMax;
    }
    
    //
    saveData();
}

void TimeTravelManager::subTimeTravelCount()
{
    _nTimeTravelCount -= 1;
    if ( _nTimeTravelCount <= 0 )
    {
        _nTimeTravelCount = 0;
    }
}

#pragma mark - get, set other
void TimeTravelManager::setCallbackCalculation(const std::function<void(void)>& callback)
{
    _onCallbackCalculation = callback;
}


#pragma mark - reward
bool TimeTravelManager::OnItemLevel(E_TIME_TRAVEL type)
{
    int level = getItemLevel(type);
    int levelMax = getItemMaxLevel(type);
    if ( level >= levelMax )
    {
        return false;
    }
    
    auto price = getItemPrice(type, level);
    if ( ItemsMoneyManager::getInstance()->getGem() < price )
    {
        return false;
    }
    
    //
    ItemsMoneyManager::getInstance()->setGem(-price);
    
    //
    _listItemLevel[type] += 1;
    
    if ( type == E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD )
    {
        addTimeTravelCount();
    }
    
    //
    saveData();
    
    return true;
}

bool TimeTravelManager::OnTimeTravel()
{
    int timeMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES));
    int countMax = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    
    int64_t timePrev = _nTimeTravelTime;
    int64_t timeNow = UtilsDate::getInstance()->getTime();
    int timeDiff = (int)(timeNow - timePrev);
    if ( timePrev != -1 && timeDiff > 0 )
    {
        int countPlus = timeDiff / timeMax;
        
        _nTimeTravelTime += countPlus * timeMax;
        _nTimeTravelCount += countPlus;
        if ( _nTimeTravelCount > countMax )
        {
            _nTimeTravelCount = countMax;
        }
    }
    
    if ( _nTimeTravelCount <= 0 )
    {
        return false;
    }
    
    //
    if ( _nTimeTravelCount >= countMax )
    {
        setTimeTravelChangeTime(timeNow);
    }
    
    //
    subTimeTravelCount();
    
    //
    saveData();
    
    //
    MafAnalyticsManager::LogEvent(kAnalTimeTravelUse, kRepeatTrue);
    if (timePrev == -1 )
    {
        MafAnalyticsManager::LogEvent(kAnalTimeTravelFirst, kRepeatFalse);
    }
        
    return true;
}

#pragma mark - reward
std::string TimeTravelManager::getMonsterHp(int nFloor, int nFloorCount)
{
    std::string strHpMax = DataManager::getInstance()->GetMonsterHp(E_PLACE::DG_NORMAL, nFloor);

    UnitBehaviorMonster::E_BOSS eType = UnitBehaviorMonster::E_BOSS::NORMAL;
    if ( nFloorCount == C_MAX_FLOOR_COUNT)
    {
        if ( nFloor % 100 == 0 )
        {
            if ( nFloor >= 9001 )       eType = UnitBehaviorMonster::E_BOSS::BOSS_9000;
            else if ( nFloor >= 8001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_8000;
            else if ( nFloor >= 7001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_7000;
            else if ( nFloor >= 6001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_6000;
            else if ( nFloor >= 5001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_AFTERLIFE3;
            else if ( nFloor >= 4001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_AFTERLIFE2;
            else if ( nFloor >= 3001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_AFTERLIFE1;
            else if ( nFloor >= 2001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_HEAVEN;
            else if ( nFloor >= 1001 )  eType = UnitBehaviorMonster::E_BOSS::BOSS_HELL;
            else                        eType = UnitBehaviorMonster::E_BOSS::BOSS_DRAGON;
        }
        else if ( nFloor % 10 == 0 )
            eType = UnitBehaviorMonster::E_BOSS::BOSS_10;
        else
            eType = UnitBehaviorMonster::E_BOSS::BOSS;
    }
    
    if ( eType == UnitBehaviorMonster::E_BOSS::BOSS )
        strHpMax = MafUtils::bigMulNum(strHpMax, MafUtils::toString(C_BOSS_HP));
    else if ( eType == UnitBehaviorMonster::E_BOSS::BOSS_10 )
        strHpMax.append("0");
    else if ( eType >= UnitBehaviorMonster::E_BOSS::BOSS_DRAGON && eType <= UnitBehaviorMonster::E_BOSS::BOSS_9000 )
        strHpMax = MafUtils::bigMulNum(strHpMax, MafUtils::toString(C_BOSS_D_HP));

    if ( PetNewManager::getInstance()->getPet(E_PET::HANDSOME_8) )
    {
        double nEffect =  PetNewManager::getInstance()->getPetEffect(E_PET::HANDSOME_8, PetNewManager::getInstance()->getPetLevel(E_PET::HANDSOME_8));
        nEffect = 100 - nEffect;

        double nEffectReinforce = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::HANDSOME_8, PetNewManager::getInstance()->getPetReinforce(E_PET::HANDSOME_8));
        if ( nEffectReinforce > 0 )
        {
            nEffect = nEffect * nEffectReinforce;
        }

        strHpMax = MafUtils::bigMulNum(strHpMax, MafUtils::toString((int)nEffect));
        if ( strHpMax.length() > 2 )
        {
            strHpMax.pop_back();
            strHpMax.pop_back();
        }
    }

    if ( PetNewManager::getInstance()->getPet(E_PET::DRAGON_3) )
    {
        double nEffect = PetNewManager::getInstance()->getPetEffect(E_PET::DRAGON_3, PetNewManager::getInstance()->getPetLevel(E_PET::DRAGON_3));
        double nEffectMin = (PetNewManager::getInstance()->getPetLevel(E_PET::DRAGON_3)-1)*4;

        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_38) )
        {
            nEffectMin += 10;
        }

        double nEffectReinforceEvolution = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::DRAGON_3, PetNewManager::getInstance()->getPetEvolution(E_PET::DRAGON_3));
        if ( nEffectReinforceEvolution > 0)
        {
            nEffectMin += nEffectReinforceEvolution;
        }

        double nEffectReinforce = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::DRAGON_3, PetNewManager::getInstance()->getPetReinforce(E_PET::DRAGON_3));
        if (nEffectReinforce > 0)
        {
            nEffect += nEffectReinforce;
        }
        nEffectMin += PetNewManager::getInstance()->getPetEquipEffect(302, 0);
        nEffectMin *= 10;
        nEffect *= 10;
        if ( nEffect < nEffectMin )
        {
            nEffectMin = nEffect;
        }

        std::string strHpTemp = strHpMax;
        strHpTemp = MafUtils::bigMulNum(strHpTemp, MafUtils::toString((int)nEffectMin));
        if (strHpTemp.length() > 3)
        {
            strHpTemp.pop_back();
            strHpTemp.pop_back();
            strHpTemp.pop_back();
            
            strHpMax = MafUtils::bigSubNum(strHpMax, strHpTemp);
        }
    }
    
    return strHpMax;
}
void TimeTravelManager::OnRewardCalculation()
{
    double nTime = getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD));
    
    // 봉식이 공격력
    std::string strAtkHero = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_HERO, false, true);
    std::string strAtkCriHero = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_HERO, true, true);
    int nCriPercentHero = UserInfoManager::getInstance()->getCriticalRatePercent(E_PLACE::DG_NORMAL, true, true);
    if ( nCriPercentHero >= 100 )
    {
        nCriPercentHero = 100;
    }

    // 크르르 공격력
    std::string strAtkChick = "0";
    std::string strAtkCriChick = "0";
    int nCriPercentChick = 0;
    if ( PetNewManager::getInstance()->getPet(E_PET::CHICK_1) )
    {
        strAtkChick = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_PET_CHICK, false, true);
        strAtkCriChick = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_PET_CHICK, true, true);
        nCriPercentChick = UserInfoManager::getInstance()->getCriticalRatePercent(E_PLACE::DG_NORMAL, false, true);
        if ( nCriPercentChick >= 100 )
        {
            nCriPercentChick = 100;
        }
    }


    int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    int nFloorPrev = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloor = nFloorPrev;
    int nFloorCount = UserInfoManager::getInstance()->getFloorCount(E_PLACE::DG_NORMAL);
    double nSpeedMove = UserInfoManager::getInstance()->getUserSpeed(E_PLACE::DG_NORMAL, true);
    double nSpeedAttack = UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_NORMAL);
    
    {
        double nEffect = ParameterManager::getInstance()->getParameterInteger("timejump_speed");
        nEffect = 1.0f / (nEffect / 100.0f + 1.0f);
        
        nSpeedMove *= nEffect;
    }
    
    if ( nSpeedMove > 1 || nSpeedMove <= 0 )
    {
        nSpeedMove = 0.125f;
    }

    //
    int rewardTime = nTime;
    int rewardFloor = 0;
    int rewardGem = 0;
    int rewardPoint = 0;
    int rewardMonsterDiaCount = 0;
    int rewardMonsterSoulCount = 0;
    int rewardKey = 0;
    std::string rewardGold = "0";
    std::string rewardKeyString = "0";
    
    int rewardKeyBonus = 1;
    int rewardGemBonus = 0;
    int rewardPointBonus = 0;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_5) )
    {
        rewardKeyBonus = 2;
    }

    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_39) )
    {
        rewardGemBonus = 1;
    }

    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_27) )
    {
        rewardPointBonus = 1;
    }

    int BONUS_100F_KEY = MafRemoteConfig::GetRemoteValueInt("BONUS_100F_KEY");
    int BONUS_10F_KEY = MafRemoteConfig::GetRemoteValueInt("BONUS_10F_KEY");
    int BONUS_1F_KEY = MafRemoteConfig::GetRemoteValueInt("BONUS_1F_KEY");
    int BONUS_100F_GEM = MafRemoteConfig::GetRemoteValueInt("BONUS_100F_GEM");
    int BONUS_10F_GEM = MafRemoteConfig::GetRemoteValueInt("BONUS_10F_GEM");

    std::string strHp = getMonsterHp(nFloor, nFloorCount);
    int nAttackCount = 0;
    while ( nTime >= 0 )
    {
        if ( nAttackCount > 5000 )
        {// 같은 자리에서 1000번 이상 공격시. break 주석
            break;
        }

        int randCriHero = random(1, 100);
        int randCriChick = random(1, 100);
        if ( MafUtils::compareStr(strAtkHero, strHp) == true )
        {//히로 일반데미지 원킬
            nAttackCount++;
            strHp = "0";
        }
        else if ( nCriPercentHero >= randCriHero && MafUtils::compareStr(strAtkCriHero, strHp) == true )
        {//히로 크리데미지 원킬
            nAttackCount++;
            strHp = "0";
        }
        else if ( MafUtils::compareStr(strAtkChick, strHp) == true )
        {//크르르 일반데미지 원킬
            nAttackCount++;
            strHp = "0";
        }
        else if ( nCriPercentChick >= randCriChick && MafUtils::compareStr(strAtkCriChick, strHp) == true )
        {//크르르 크리데미지 원킬
            nAttackCount++;
            strHp = "0";
        }
        else
        {
            //
            nAttackCount++;

            // 히로 데미지 감소
            if ( nCriPercentHero >= randCriHero && strHp.length() < strAtkCriHero.length() + 7 )
            {
                strHp = MafUtils::bigSubNum(strHp, strAtkCriHero);
            }
            else if ( strHp.length() < strAtkHero.length() + 7 )
            {
                strHp = MafUtils::bigSubNum(strHp, strAtkHero);
            }

            if ( strHp.compare("0") != 0 )
            {// 크르르 데미지 감소
                if ( nCriPercentChick >= randCriChick && strHp.length() < strAtkCriChick.length() + 7 )
                {
                    strHp = MafUtils::bigSubNum(strHp, strAtkCriChick);
                }
                else if ( strHp.length() < strAtkChick.length() + 7 )
                {
                    strHp = MafUtils::bigSubNum(strHp, strAtkChick);
                }
            }
        }

        bool bMonsterDia = false;
        if ( strHp.compare("0") == 0 )
        {
            //
            bMonsterDia = true;

            //
            strHp = getMonsterHp(nFloor, nFloorCount);
            nAttackCount = 0;
        }

        // time
        nTime = nTime - nSpeedAttack;
        if ( bMonsterDia == true )
        {
            if ( nFloorCount == C_MAX_FLOOR_COUNT )
            {
                nTime = nTime - (nSpeedMove * 4 * 4);
                nTime = nTime - (nSpeedMove * 4 * 2);
            }
            else
            {
                nTime = nTime - (nSpeedMove * 4 * 2);
            }
        }

        // reward
        if ( bMonsterDia == true )
        {
            // reward
            rewardMonsterDiaCount++;
            if ( nFloorCount == C_MAX_FLOOR_COUNT )
            {
                rewardFloor++;
                rewardMonsterSoulCount++;
                if ( nFloor % 100 == 0 )
                {
                    rewardKey += BONUS_100F_KEY * rewardKeyBonus;
                    rewardGem += BONUS_100F_GEM + rewardGemBonus;
                }
                else if ( nFloor % 10 == 0 )
                {
                    rewardKey += BONUS_10F_KEY * rewardKeyBonus;
                    rewardGem += BONUS_10F_GEM;
                }
                else
                {
                    rewardKey += BONUS_1F_KEY * rewardKeyBonus;
                }

                if ( nFloor % 1000 == 0 )
                {
                    rewardPoint += 2 + rewardPointBonus;
                }
            }
        }

        // floor
        if ( bMonsterDia == true )
        {
            nFloorCount++;
            if ( nFloorCount > C_MAX_FLOOR_COUNT )
            {
                nFloor++;
                nFloorCount = 1;
            }
            
            if ( nFloor >= nFloorHigh )
            {
                break;;
            }
        }
    }


    // 적 처치 골드 보상
    if ( rewardMonsterDiaCount > 0 )
    {
        std::string strGoldEarn = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::MON_KILL, true);
        strGoldEarn = MafUtils::bigMulNum(strGoldEarn, MafUtils::toString(rewardMonsterDiaCount));

        rewardGold = MafUtils::bigAddNum(rewardGold, strGoldEarn);
    }

    // 퀘스트 골드 보상
    if ( rewardTime > 0 )
    {
        std::string strGoldEarn = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::SECOND_DEFAULT);
        strGoldEarn = MafUtils::bigMulNum(strGoldEarn, MafUtils::toString(rewardTime));

        rewardGold = MafUtils::bigAddNum(rewardGold, strGoldEarn);
    }


    // 뿌리링 열쇠 보상
    if ( PetNewManager::getInstance()->getPet(E_PET::PURIRING_5) )
    {
        int nLevelPet = PetNewManager::getInstance()->getPetLevel(E_PET::PURIRING_5);
        for ( int i = nFloorPrev; i < nFloor; i++ )
        {
            std::string strTemp = PetNewManager::getInstance()->getPetEffectPuriringKey(nLevelPet, i, 5);
            strTemp = MafUtils::bigMulNum(strTemp, "5");
            rewardKeyString = MafUtils::bigAddNum(rewardKeyString, strTemp);
        }

        //
        if ( rewardKey != 0 )
        {
            rewardKeyString = MafUtils::bigAddNum(rewardKeyString, MafUtils::toString(rewardKey));
        }
    }
    
    // 보석 10%만 지급.
    rewardGem = rewardGem * 0.1f;

    if ( rewardFloor <= 1 )
    {
        rewardFloor = 0;
    }
    
    // 보상 기록 저장
    _strFloor = MafAes256::Encrypt(MafUtils::toString(nFloor));
    _strFloorCount = MafAes256::Encrypt(MafUtils::toString(nFloorCount));
    _strRewardTime = MafAes256::Encrypt(MafUtils::toString(rewardTime));
    _strRewardFloor = MafAes256::Encrypt(MafUtils::toString(rewardFloor));
    _strRewardGem = MafAes256::Encrypt(MafUtils::toString(rewardGem));
    _strRewardPoint = MafAes256::Encrypt(MafUtils::toString(rewardPoint));
    _strRewardKey = MafAes256::Encrypt(rewardKeyString);
    _strRewardGold = MafAes256::Encrypt(rewardGold);

    _strRewardMonsterSoulCount = MafAes256::Encrypt(MafUtils::toString(rewardMonsterSoulCount));

    //
    if ( _onCallbackCalculation != nullptr )
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread(_onCallbackCalculation);
    }
}

void TimeTravelManager::OnReward()
{
    //
    int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    int nFloorPrev = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloor = nFloorPrev;
    int nFloorCount = UserInfoManager::getInstance()->getFloorCount(E_PLACE::DG_NORMAL);

    //
    int rewardFloor = getRewardFloor();

    //
    if ( _strFloor.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strFloor);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nFloor = nTemp;
        }
    }

    if ( _strFloorCount.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strFloorCount);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nFloorCount = nTemp;
        }
    }

    //
    if ( _strRewardGem.length() != 0 )
    {
        std::string strGold = getRewardGold();
        ItemsMoneyManager::getInstance()->setGold(strGold);
    }

    if ( _strRewardKey.length() != 0 )
    {
        std::string strKey = getRewardKey();
        ItemsMoneyManager::getInstance()->setKey(strKey);
    }

    if ( _strRewardGem.length() != 0 )
    {
        int nGem = getRewardGem();
        ItemsMoneyManager::getInstance()->setGem(nGem);
    }

    if ( _strRewardPoint.length() != 0 )
    {
        int nPoint = getRewardPoint();
        ItemsMoneyManager::getInstance()->setPoint(nPoint);
    }

    // 층수 적용
    UserInfoManager::getInstance()->setFloor(E_PLACE::DG_NORMAL, nFloor);
    UserInfoManager::getInstance()->setFloorCount(E_PLACE::DG_NORMAL, nFloorCount);
   
    // 누적 층수
    int nFloorTotal = UserInfoManager::getInstance()->getTotalFloor() + rewardFloor;
    UserInfoManager::getInstance()->setTotalFloor(nFloorTotal);


    //펫 진화 체크
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        E_PET ePet = (E_PET)i;
        
        PetNewManager::getInstance()->onPetEvolution(ePet);
    }

    //
    if ( _strRewardMonsterSoulCount.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strRewardMonsterSoulCount);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            int nMonsterSoulRate = ParameterManager::getInstance()->getParameterInteger("monster_soul_rate");
            for ( int i = 0; i < nTemp; i++ )
            {
                if ( random(1, 100) <= nMonsterSoulRate )
                {
                    int nMonsterSoulIdx = random(1, C_COUNT_MONSTER);
                    MonsterManager::getInstance()->addSoulCount(nMonsterSoulIdx, 1);
                }
            }
            MonsterManager::getInstance()->saveData();
            
            //
            RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, MonsterManager::getInstance()->isSoulLevelComplete());
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP);
        }
    }



    //
    _strFloor = "";
    _strFloorCount = "";

    _strRewardTime = "";
    _strRewardFloor = "";
    _strRewardGem = "";
    _strRewardPoint = "";
    _strRewardKey = "";
    _strRewardGold = "";

    _strRewardMonsterSoulCount = "";
}


int TimeTravelManager::getRewardFloor()
{
    int nResult = 0;
    if ( _strRewardFloor.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strRewardFloor);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

int TimeTravelManager::getRewardGem()
{
    int nResult = 0;
    if ( _strRewardGem.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strRewardGem);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

int TimeTravelManager::getRewardPoint()
{
    int nResult = 0;
    if ( _strRewardPoint.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strRewardPoint);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

std::string TimeTravelManager::getRewardKey()
{
    std::string strResult = "0";
    if ( _strRewardKey.length() != 0 )
    {
        strResult = MafAes256::Decrypt(_strRewardKey);
    }
    
    return strResult;
}

std::string TimeTravelManager::getRewardGold()
{
    std::string strResult = "0";
    if ( _strRewardGold.length() != 0 )
    {
        strResult = MafAes256::Decrypt(_strRewardGold);
    }
    
    return strResult;
}
