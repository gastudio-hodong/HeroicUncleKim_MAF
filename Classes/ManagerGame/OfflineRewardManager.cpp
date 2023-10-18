//
//  OfflineRewardManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "OfflineRewardManager.h"

#include "Common/ConfigKey.h"

#include "GameObject/Defense/InfoDefenseFloor.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/MonsterManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"


USING_NS_CC;

OfflineRewardManager::OfflineRewardManager(void) :
_strFloor(""),
_strFloorCount(""),
_strTime(""),

_strRewardTime(""),
_strRewardFloor(""),
_strRewardGem(""),
_strRewardPoint(""),
_strRewardKey(""),
_strRewardGold(""),
_strRewardMine1(""),
_strRewardMine2(""),
_strRewardMine3(""),
_strRewardMine4(""),
_strRewardMine5(""),
_strRewardMine6(""),
_strRewardMineSpecial(""),
_strRewardMonsterSoulCount(""),
_strRewardDefenseCoinCount(""),

_onCallbackCalculation(nullptr)
{
    
}

OfflineRewardManager::~OfflineRewardManager(void)
{
    
}

bool OfflineRewardManager::init()
{
    
    return true;
}

#pragma mark - set, get
void OfflineRewardManager::setOfflineLastTime()
{
    time_t nTime = UtilsDate::getInstance()->getTime();
    if ( nTime <= 0 )
    {
        UtilsDate::getInstance()->getTime();
        return;
    }
    
    std::string offlineTime = MafUtils::toString(nTime);
    std::string offlineTimeEncrypt = MafAes256::Encrypt(offlineTime);
    
    UserDefault::getInstance()->setStringForKey(KEY_OFFLINE_TIME, offlineTimeEncrypt);
}

time_t OfflineRewardManager::getOfflineLastTimeDiff()
{
    std::string offlineTimeDecrypt = UserDefault::getInstance()->getStringForKey(KEY_OFFLINE_TIME, "");
    if ( offlineTimeDecrypt.length() == 0 )
    {
        return 0;
    }
    
    offlineTimeDecrypt = MafAes256::Decrypt(offlineTimeDecrypt);
    time_t nTime = atol(offlineTimeDecrypt.c_str());
    if ( nTime <= 0 )
    {
        return 0;
    }
    
    time_t nTimeDiff = UtilsDate::getInstance()->getTime() - nTime;
    if ( nTimeDiff < 600 )
    {
        // nTime : 오프라인 시간이 0일때
        // nTime : 오프라인 시간 차이가 10분 이하일때
        nTimeDiff = 0 ;
    }
    
    if ( nTimeDiff > 86400 )
    {
        // nTimeDiff : 오프라인 시간 차이가 24시간 이상일때
        nTimeDiff = 86400;
    }
    return nTimeDiff;
}

bool OfflineRewardManager::isOfflineTime()
{
    bool bResult = false;
    if ( getOfflineTime() > 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

int OfflineRewardManager::getOfflineTime()
{
    int nResult = 0;
    if ( _strTime.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strTime);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

#pragma mark - set, get reward
int OfflineRewardManager::getRewardTime()
{
    int nResult = 0;
    if ( _strRewardTime.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(_strRewardTime);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

int OfflineRewardManager::getRewardFloor()
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

int OfflineRewardManager::getRewardGem()
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

int OfflineRewardManager::getRewardPoint()
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

std::string OfflineRewardManager::getRewardKey()
{
    std::string strResult = "0";
    if ( _strRewardKey.length() != 0 )
    {
        strResult = MafAes256::Decrypt(_strRewardKey);
    }
    
    return strResult;
}

std::string OfflineRewardManager::getRewardGold()
{
    std::string strResult = "0";
    if ( _strRewardGold.length() != 0 )
    {
        strResult = MafAes256::Decrypt(_strRewardGold);
    }
    
    return strResult;
}

int OfflineRewardManager::getRewardMine(E_MINE eType)
{
    int nResult = 0;
    std::string strEncrypt = "";
    
    switch (eType) {
        case E_MINE::MINE_1:    strEncrypt = _strRewardMine1;       break;
        case E_MINE::MINE_2:    strEncrypt = _strRewardMine2;       break;
        case E_MINE::MINE_3:    strEncrypt = _strRewardMine3;       break;
        case E_MINE::MINE_4:    strEncrypt = _strRewardMine4;       break;
        case E_MINE::MINE_5:    strEncrypt = _strRewardMine5;       break;
        case E_MINE::MINE_6:    strEncrypt = _strRewardMine6;       break;
        case E_MINE::MINE_7:    strEncrypt = _strRewardMineSpecial; break;
        default:
            break;
    }
    
    if ( strEncrypt.length() != 0 )
    {
        std::string strDecrypt = MafAes256::Decrypt(strEncrypt);
        int nTemp = atoi(strDecrypt.c_str());
        if ( nTemp > 0 )
        {
            nResult = nTemp;
        }
    }
    
    return nResult;
}

bool OfflineRewardManager::isRewardPetEvolution(E_PET ePet)
{
    bool bResult = false;
    if ( PetNewManager::getInstance()->getPet(ePet) == false )
    {
        return bResult;
    }
    
    int nFloorPrev = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloor = nFloorPrev;
    
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
    
    //펫 진화 체크
    for ( int i = nFloorPrev; i <= nFloor; i++ )
    {
        int nLevelEvolution = PetNewManager::getInstance()->getPetEvolution(ePet);
        if ( i + 1 == PetNewManager::getInstance()->getPetEvolutionFloor(ePet, nLevelEvolution) )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool OfflineRewardManager::isRewardStory()
{
    bool bResult = false;
    
    int nFloorPrev = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloor = nFloorPrev;
    
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
    
    //오픈 체크
    auto list = StoryManager::getInstance()->getStoryAll();
    for ( auto &obj : list)
    {
        if ( obj->getConditionType().compare("FLOOR") != 0 )
        {
            continue;
        }
        
        if ( obj->getConditionParams() + 1 > nFloorPrev && obj->getConditionParams() < nFloor )
        {
            if ( StoryManager::getInstance()->isStoryOpen(obj->getIdx()) == false )
            {
                bResult = true;
                break;
            }
        }
    }
    
    return bResult;
}
std::string OfflineRewardManager::getRewardDefenseCoin()
{
    std::string strResult = "0";
    if ( _strRewardDefenseCoinCount.length() != 0 )
    {
        strResult = MafAes256::Decrypt(_strRewardDefenseCoinCount);
    }
    
    return strResult;
}

#pragma mark - get monster
std::string OfflineRewardManager::getMonsterHp(int nFloor, int nFloorCount)
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
        double nEffect = PetNewManager::getInstance()->getPetEffect(E_PET::HANDSOME_8, PetNewManager::getInstance()->getPetLevel(E_PET::HANDSOME_8));
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

#pragma mark - get, set other
void OfflineRewardManager::setCallbackCalculation(const std::function<void(void)>& callback)
{
    _onCallbackCalculation = callback;
}

#pragma mark - reward
void OfflineRewardManager::rewardCalculation()
{
    double nRealTime = getOfflineLastTimeDiff();
    if ( nRealTime <= 0 )
    {
        if ( _onCallbackCalculation != nullptr )
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread(_onCallbackCalculation);
        }
        return;
    }
    
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
    
    
    int nFloorPrev = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloor = nFloorPrev;
    int nFloorCount = UserInfoManager::getInstance()->getFloorCount(E_PLACE::DG_NORMAL);
    double nSpeedMove = UserInfoManager::getInstance()->getUserSpeed(E_PLACE::DG_NORMAL, true);
    double nSpeedAttack = UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_NORMAL);
    
    int nLevelArtifact_106 = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_106);
    if ( nLevelArtifact_106 > 0 )
    {
        double nEffect = DataManager::getInstance()->GetArtifactEffect(E_ARTIFACT::ARTI_106, nLevelArtifact_106, 0);
        nEffect = 1.0f / (nEffect / 100.0f + 1.0f);
        
        nSpeedMove *= nEffect;
    }
    
    if ( nSpeedMove > 1 || nSpeedMove <= 0 )
    {
        nSpeedMove = 0.125f;
    }
    
    // 암호화
    {
        _strTime = MafAes256::Encrypt(MafUtils::toString(nRealTime));
    }
    
    // 보상 50% 적용
    double nTime = nRealTime / 2;
    
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
    int rewardMine1 = 0;
    int rewardMine2 = 0;
    int rewardMine3 = 0;
    int rewardMine4 = 0;
    int rewardMine5 = 0;
    int rewardMine6 = 0;
    int rewardMineSpecial = 0;
    
    int rewardKeyBonus = 1;
    int rewardGemBonus = 0;
    int rewardPointBonus = 0;
    
    std::string rewardDefenseCoin = "0";
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
    
    // 광물 보상
    if ( rewardTime > 0 && WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
    {
        double nEffectMineDefault = MineralManager::getInstance()->getPickaxEffect();
        for ( int i = 1; i <= C_COUNT_MINE; i++ )
        {
            E_MINE eMine = (E_MINE)i;
            E_MINERAL eMineral = (E_MINERAL)i;
            if ( eMine == E_MINE::MINE_7 )
            {
                std::string strMineSpecial = UserDefault::getInstance()->getStringForKey(KEY_MINE_SPECIAL);
                if ( !strMineSpecial.empty() )
                {
                    strMineSpecial = MafAes256::Decrypt(strMineSpecial);
                    eMineral = (E_MINERAL)atoi(strMineSpecial.c_str());
                }
            }
            
            double nTime = DataManager::GetMineSpendTime(eMineral);
            double nEffect = nEffectMineDefault;
            int nCount = 0;
            if ( eMineral < E_MINERAL::MINERAL_7_SAPPHIRE )
            {
                nEffect *= PetNewManager::getInstance()->getPetEquipEffect(701, 1);
            }
            else
            {
                nEffect *= PetNewManager::getInstance()->getPetEquipEffect(702, 1);
            }
            
            //
            nTime = (int)(nTime/nEffect);
            nCount = (int)(rewardTime/nTime);
            switch (eMineral) {
                case E_MINERAL::MINERAL_1:  rewardMine1 = nCount;   break;
                case E_MINERAL::MINERAL_2:  rewardMine2 = nCount;   break;
                case E_MINERAL::MINERAL_3:  rewardMine3 = nCount;   break;
                case E_MINERAL::MINERAL_4:  rewardMine4 = nCount;   break;
                case E_MINERAL::MINERAL_5:  rewardMine5 = nCount;   break;
                case E_MINERAL::MINERAL_6:  rewardMine6 = nCount;   break;
                    
                default:
                    rewardMineSpecial = nCount;
                    break;
            }
        }
    }
    
    //디펜스 코인 보상,  디펜스만 100% 오프라인 시간 계산
    if(rewardTime > 0 && DefenseManager::getInstance()->isFirstGift())
    {
        int m_15 = nRealTime / 900;
        if(auto data = DefenseManager::getInstance()->getFloorData((int)DefenseManager::getInstance()->getHighFloor()))
        {
            auto list = ItemsManager::getInstance()->getConvertArray(data->getRewardRepeat());
            if(list.size() > 0)
            {
                auto items = list.at(0);
                rewardDefenseCoin = items->getCount();
            }
        }
        rewardDefenseCoin = MafUtils::bigMulNum(rewardDefenseCoin, MafUtils::toString(m_15));
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

    _strRewardMine1 = MafAes256::Encrypt(MafUtils::toString(rewardMine1));
    _strRewardMine2 = MafAes256::Encrypt(MafUtils::toString(rewardMine2));
    _strRewardMine3 = MafAes256::Encrypt(MafUtils::toString(rewardMine3));
    _strRewardMine4 = MafAes256::Encrypt(MafUtils::toString(rewardMine4));
    _strRewardMine5 = MafAes256::Encrypt(MafUtils::toString(rewardMine5));
    _strRewardMine6 = MafAes256::Encrypt(MafUtils::toString(rewardMine6));
    _strRewardMineSpecial = MafAes256::Encrypt(MafUtils::toString(rewardMineSpecial));
    
    _strRewardMonsterSoulCount = MafAes256::Encrypt(MafUtils::toString(rewardMonsterSoulCount));
    _strRewardDefenseCoinCount = MafAes256::Encrypt(MafUtils::toString(rewardDefenseCoin));
    
    
    //
    if ( _onCallbackCalculation != nullptr )
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread(_onCallbackCalculation);
    }
}

void OfflineRewardManager::reward(bool bAds)
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
        if ( bAds == true )
        {
            strGold = MafUtils::bigMulNum(strGold, "200");
            if ( strGold.length() > 2 )
            {
                strGold.pop_back();
                strGold.pop_back();
            }
            else
            {
                strGold = "0";
            }
        }
        ItemsMoneyManager::getInstance()->setGold(strGold);
    }
    
    if ( _strRewardKey.length() != 0 )
    {
        std::string strKey = getRewardKey();
        if ( bAds == true )
        {
            strKey = MafUtils::bigMulNum(strKey, "200");
            if ( strKey.length() > 2 )
            {
                strKey.pop_back();
                strKey.pop_back();
            }
            else
            {
                strKey = "0";
            }
        }
        ItemsMoneyManager::getInstance()->setKey(strKey);
    }
    
    if ( _strRewardGem.length() != 0 )
    {
        int nGem = getRewardGem();
        if ( bAds == true )
        {
            nGem *= 2;
        }
        ItemsMoneyManager::getInstance()->setGem(nGem);
    }
    
    if ( _strRewardPoint.length() != 0 )
    {
        int nPoint = getRewardPoint();
        if ( bAds == true )
        {
            nPoint *= 2;
        }
        ItemsMoneyManager::getInstance()->setPoint(nPoint);
    }
    
    // 광물
    for ( int i = 1; i <= C_COUNT_MINE; i++ )
    {
        E_MINE eMine = (E_MINE)i;
        E_MINERAL eMineral = (E_MINERAL)i;
        if ( eMine == E_MINE::MINE_7 )
        {
            std::string strMineSpecial = UserDefault::getInstance()->getStringForKey(KEY_MINE_SPECIAL);
            if ( !strMineSpecial.empty() )
            {
                strMineSpecial = MafAes256::Decrypt(strMineSpecial);
                eMineral = (E_MINERAL)atoi(strMineSpecial.c_str());
            }
        }
        
        int nCount = getRewardMine(eMine);
        if ( nCount > 0 )
        {
            if ( bAds == true )
            {
                nCount *= 2;
            }
            MineralManager::getInstance()->setCount(eMineral, MineralManager::getInstance()->getCount(eMineral)+nCount);
        }
    }
    MineralManager::getInstance()->saveData();
    
    
    // 층수 적용
    UserInfoManager::getInstance()->setFloor(E_PLACE::DG_NORMAL, nFloor);
    UserInfoManager::getInstance()->setFloorCount(E_PLACE::DG_NORMAL, nFloorCount);
    if ( nFloorHigh < nFloor )
    {
        //타이밍 패키지
        TimingPackageManager::getInstance()->checkOfflineRewardInfo(nFloor);
        TimingPackageManager::getInstance()->checkOfflineStartInfo(nFloor);
        
        // 최고층수 제한 해야 됨.
        UserInfoManager::getInstance()->setHighFloor(E_PLACE::DG_NORMAL,nFloor);
        MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_HIGH, nFloor);
    }
    
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
    
    //디펜스 코인
    if ( _strRewardDefenseCoinCount.length() != 0 )
    {
        int64_t defenseCoin = std::atoll(getRewardDefenseCoin().c_str());
        if ( bAds == true )
        {
            defenseCoin *= 2;
        }
        
        DefenseManager::getInstance()->setDefenseCoin(defenseCoin);
    }
    
    //
    _strFloor = "";
    _strFloorCount = "";
    _strTime = "";
    
    _strRewardTime = "";
    _strRewardFloor = "";
    _strRewardGem = "";
    _strRewardPoint = "";
    _strRewardKey = "";
    _strRewardGold = "";
    _strRewardMine1 = "";
    _strRewardMine2 = "";
    _strRewardMine3 = "";
    _strRewardMine4 = "";
    _strRewardMine5 = "";
    _strRewardMine6 = "";
    _strRewardMineSpecial = "";
    _strRewardMonsterSoulCount = "";
    _strRewardDefenseCoinCount = "";
    
    //
    setOfflineLastTime();
}
