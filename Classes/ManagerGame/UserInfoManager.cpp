//
//  ModelUserInfo.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 8. 17..
//
//

#include "UserInfoManager.h"

#include "GameObject/InfoPetEquip.h"
#include "GameObject/InfoArtifact.h"

#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/FriendManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/LootsManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/QuestManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/MonsterManager.h"
#include "ManagerGame/StoneManager.h"

#pragma mark-

static UserInfoManager *s_UserInfo = nullptr;
UserInfoManager* UserInfoManager::getInstance()
{
    if (!s_UserInfo)
    {
        s_UserInfo = new (std::nothrow) UserInfoManager();
        s_UserInfo->init();
    }
    
    return s_UserInfo;
}

void UserInfoManager::destroyInstance()
{
    if ( s_UserInfo->_uiBettle != nullptr )
    {
        s_UserInfo->_uiBettle = nullptr;
    }
    
    if ( s_UserInfo != nullptr )
    {
        delete s_UserInfo;
        s_UserInfo = nullptr;
    }
}

UserInfoManager::UserInfoManager() :

//
_eCurrentPlace(E_PLACE::EMPTY),

//
_uFloor(0),
_uFloorCount(0),
_uFloorTotal(0),
_uFloorHigh(0),

_uFloorDevil(0),
_uFloorDevilCount(0),
_uFloorDevilHigh(0),

//
_uRevival(0),
_uRevivalDevil(0),

//
_uiBettle(nullptr),

_bOneKill(false),
_bOneKillPrison(false),

_hitCount(0),
_uItemFeverTiem(0),
_uItemFeverCount(0),
_hitCountDungeon(0),
_honghitCount(0),
_honghitCountDungeon(0),

_hitCountDevil(0),
_hitCountDevilHedgehog(0),
_hitCountDevilFlash(0),
_nCheatSensorFloorIntervalCount(0)
{
    
}

UserInfoManager::~UserInfoManager()
{
    
}

bool UserInfoManager::init()
{
    _fHeightIphoneX = 0.0f;
    if ( MafNative::IsDeviceiPhoneX() )
    {
        Size size = UtilsRect::getVisibleSize();
        Rect safeArea = Director::getInstance()->getSafeAreaRect();
        
        _fHeightIphoneX = size.height - safeArea.origin.y - safeArea.size.height;
    }
    
    //
    _eCurrentPlace = E_PLACE::EMPTY;
    
    //
    _uFloor = 1;
    _uFloorCount = 1;
    _uFloorTotal = 0;
    _uFloorHigh = 1;
    
    _uFloorDevil = 1;
    _uFloorDevilCount = 1;
    _uFloorDevilHigh = 1;
    
    //
    _uRevival = 1;
    _uRevivalDevil = 1;
    
    //
    _onModifyFloor = nullptr;
    _onModifyEquip = nullptr;
    
    _uAttendVip = 0;
    _uAttendDay = 0;
    
    // 피버타임
    _uFeverTiem = 0.0f;
    
    _uGiftHave = false;
    _uAttendHave = false;
    
    loadLocalData();
    resetEveryThing();
    
    
    if ( _uiBettle != nullptr )
    {
        _uiBettle = nullptr;
    }
    
    return true;
}

void UserInfoManager::resetEveryThing(){
    
    for ( int i = 0; i < sizeof(_uTimeBuff)/sizeof(int); i++ )
    {
        _uTimeBuff[i] = 0;
    }
    for ( int i = 0; i < sizeof(_uTimeAds)/sizeof(int); i++ )
    {
        _uTimeAds[i] = 0;
    }
    for(int i = 0; i < C_MAX_MYHOME; i++)
    {
        _uTimeMyhomeAds[i] = 0;
    }

    for(int i=0; i <= C_COUNT_ARTIFACT; i++)
    {
        _uArtifactLevel[i] = 0;
    }
    _uArtifactLevel[E_ARTIFACT::ARTI_32_PRISON_WEAPON] = 1;
    _uArtifactLevel[E_ARTIFACT::ARTI_34_PICKAX] = 1;
    _listArtifactReinforce.clear();


    //
    for(int i=0; i <= C_COUNT_RELIC; i++)
    {
        _uRelicLevel[i] = 0;
    }
    
    for(int i=E_HONOR::HONOR_1; i <= E_HONOR::HONOR_3; i++){
        setHonorLevel((E_HONOR)i, 0, false);
    }
    
    _uDailyQuestTime = 0;
    //미션 최초 초기화
    //resetDailyQuest();
    
    //devil
    for(int i=0; i<C_COUNT_EQUIP_TYPE_DEVIL;i++){
        _uDevilEquip[i] = E_EQUIP_IDX::E_D_IDX_1;
        _uDevilEquipReinforce[i] = 0;
    }
    
    for(int i=0; i <= C_COUNT_ARTIFACT_DEVIL; i++){ //
        _bArtifactDevil[i] = false;
    }
    _bArtifactDevil[0]=true;
    
    for(int i=0;i < C_MAX_MYHOME;i++)
    {
        _uMyhomeLevelList[i] = 0;
        _uMyhomeWatchcountList[i] = 0;
    }
    
    
    resetNormal(-1);
    resetDevil();
}

void UserInfoManager::resetNormal(int floor)
{
    _uFloor = MAX(floor, 1);
    _uFloorCount = 1;
    
    ItemsMoneyManager::getInstance()->resetNormal();
    WeaponManager::getInstance()->onReset(floor);
    QuestManager::getInstance()->onReset(floor);
}

void UserInfoManager::resetDevil()
{
    _uFloorDevil = 1;
    _uFloorDevilCount = 1;
    
    ItemsMoneyManager::getInstance()->resetDevil();
    
    for(int i=0; i <= C_COUNT_QUEST_DEVIL; i++){ //
        _uQuestLevelDevil[i] = 0;
        _uQuestTimeDevil[i] = DataManager::GetQuestSpendTime(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i);
    }
    _uQuestLevelDevil[1] = 1;
    
    for(int i=0; i<C_COUNT_EQUIP_TYPE_DEVIL;i++){
        _uDevilEquipLv[i] = 0;
    }
}

#pragma mark -
E_PLACE UserInfoManager::getCurrentPlace()
{
    return _eCurrentPlace;
}

void UserInfoManager::setCurrentPlace(E_PLACE eplace)
{
    _eCurrentPlace = eplace;

    UserDefault::getInstance()->setIntegerForKey(KEY_PLACE, (int)_eCurrentPlace);
}

#pragma mark - battle
Node* UserInfoManager::getBattleUI()
{
    return _uiBettle;
}

void UserInfoManager::setBattleUI(Node* node)
{
    if ( _uiBettle != nullptr )
    {
        _uiBettle = nullptr;
    }
    
    _uiBettle = node;
}

#pragma mark - battle : floor
int UserInfoManager::getFloor(E_PLACE ePlace)
{
    int nFloor = 1;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        nFloor = _uFloor.valueInt();
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nFloor = _uFloorDevil.valueInt();
    }
    
    return nFloor;
}

void UserInfoManager::setFloor(E_PLACE ePlace, int nFloor, bool bSave)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        _uFloor = nFloor;
        
        if ( bSave == true )
        {
            UserDefault::getInstance()->setStringForKey(KEY_FLOOR, MafAes256::Encrypt(MafUtils::toString(nFloor)));
        }
    }
    else if( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uFloorDevil = nFloor;
        
        if ( bSave == true )
        {
            UserDefault::getInstance()->setStringForKey(KEY_D_FLOOR, MafAes256::Encrypt(MafUtils::toString(nFloor)));
        }
    }
}

int UserInfoManager::getFloorCount(E_PLACE ePlace)
{
    int nFloorCount = 1;
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nFloorCount = _uFloorDevilCount.valueInt();
    }
    else
    {
        nFloorCount = _uFloorCount.valueInt();
    }
    
    return nFloorCount;
}

void UserInfoManager::setFloorCount(E_PLACE ePlace, int nFloorCount, bool bDirect, bool bSave)
{
    if( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uFloorDevilCount = nFloorCount;
        
        if ( bSave == true )
        {
            UserDefault::getInstance()->setStringForKey(KEY_D_FLOOR_COUNT, MafAes256::Encrypt(MafUtils::toString(nFloorCount)));
        }
    }
    else
    {
        _uFloorCount = nFloorCount;
        
        if ( bSave == true )
        {
            UserDefault::getInstance()->setStringForKey(KEY_FLOOR_COUNT, MafAes256::Encrypt(MafUtils::toString(nFloorCount)));
        }
    }
    
    if ( _onModifyFloor!= nullptr && bDirect )
    {
        _onModifyFloor();
    }
}

int UserInfoManager::getHighFloor(E_PLACE ePlace)
{
    int nFloor = 1;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        nFloor = _uFloorHigh.valueInt();
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nFloor = _uFloorDevilHigh.valueInt();
    }
    
    return nFloor;
}

void UserInfoManager::setHighFloor(E_PLACE ePlace, int nFloor)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        _uFloorHigh = nFloor;
         
        UserDefault::getInstance()->setStringForKey(DATA_HIGH_FLOOR, MafAes256::Encrypt(MafUtils::toString(nFloor)));
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uFloorDevilHigh = nFloor;
        
        UserDefault::getInstance()->setStringForKey(DATA_D_HIGH_FLOOR, MafAes256::Encrypt(MafUtils::toString(nFloor)));
    }
}

void UserInfoManager::setTotalFloor(int floor)
{
    _uFloorTotal = floor;
    
    UserDefault::getInstance()->setStringForKey(DATA_TOTAL_FLOOR, MafAes256::Encrypt(MafUtils::toString(floor)));
}

int UserInfoManager::getTotalFloor()
{
    return _uFloorTotal.valueInt();
}

bool UserInfoManager::isFloorCheck(int floor, bool bUp)
{
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_13))
    {
        return true;
    }
    
    if (bUp)
    {
        if (getFloor(E_PLACE::DG_NORMAL) >= floor)
        {
            return true;
        }
    }
    else
    {
        if (getFloor(E_PLACE::DG_NORMAL) <= floor)
        {
            return true;
        }
    }
    return false;
}

bool UserInfoManager::isMeterCheck(int meter)
{
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_39)) {
        return true;
    }
    
    if ( PrisonManager::getInstance()->getDungeonKill() <= meter )
    {
        return true;
    }
    
    return false;
}

#pragma mark - battle : revival
int UserInfoManager::getRevival(E_PLACE ePlace)
{
    int nRevival = 1;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        nRevival = _uRevival.valueInt();
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nRevival = _uRevivalDevil.valueInt();
    }
 
    nRevival = MAX(nRevival, 1);
    
    return nRevival;
}

void UserInfoManager::setRevival(E_PLACE ePlace, int nRevival)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        _uRevival = nRevival;
        
        UserDefault::getInstance()->setStringForKey(DATA_REVIVAL, MafAes256::Encrypt(MafUtils::toString(nRevival)));
    }
    else if( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uRevivalDevil = nRevival;
        
        UserDefault::getInstance()->setStringForKey(KEY_D_REVIVAL, MafAes256::Encrypt(MafUtils::toString(nRevival)));
    }
}

int UserInfoManager::getRevivalAds(E_PLACE ePlace)
{
    int nResult = 0;
    
    std::string strEncrypt = "";
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        strEncrypt = UserDefault::getInstance()->getStringForKey(DATA_REVIVAL_ADS);
    }
    
    if ( !strEncrypt.empty() )
    {
        std::string strDecrypt = MafAes256::Decrypt(strEncrypt);
        nResult = std::atoi(strDecrypt.c_str());
    }
    
    return nResult;
}

void UserInfoManager::setRevivalAds(E_PLACE ePlace, int nAdsCount)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        UserDefault::getInstance()->setStringForKey(DATA_REVIVAL_ADS, MafAes256::Encrypt(MafUtils::toString(nAdsCount)));
    }
}



#pragma mark - cheat : sensor

bool UserInfoManager::isCheatSensorOn()
{
    bool result = false;
    
    if(_nCheatSensorFloorIntervalCount > ParameterManager::getInstance()->getParameterInteger("cheat_sensor_interval"))
        result = true;
    
    return result;
}

void UserInfoManager::setCheatSensorCount(int value)
{
    _nCheatSensorFloorIntervalCount = value;
}

int UserInfoManager::getCheatSensorCount()
{
    return _nCheatSensorFloorIntervalCount;
}

void UserInfoManager::checkCheating()
{
    if(ParameterManager::getInstance()->getParameterInteger("cheat_sensor_switch") == 0)
        return;
    
    std::string strLog = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strLog = "IOS_" + std::to_string(AccountManager::getInstance()->getUserIdx());
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strLog = "AOS_" + std::to_string(AccountManager::getInstance()->getUserIdx());;
#endif
    
    const E_STONE listStone[] = {E_STONE::ATTACK, E_STONE::GOLD, E_STONE::KEY, E_STONE::FORCE_ATTACK};
    for ( const auto& eStone : listStone )
    {
        auto count = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::LOCAL, eStone);
        if(count > 2000)
        {
            MafAnalyticsManager::LogEvent(kCheatStone, "userIdx", strLog.c_str(), kRepeatFalse);
            break;
        }
    }
    
    _nCheatSensorFloorIntervalCount = 0;
}

#pragma mark - battle : buff
int UserInfoManager::getBuffTime(E_BUFF_TYPE eType)
{
    int nResult = _uTimeBuff[eType];
    if ( eType == E_BUFF_TYPE::BUFF_HERO_BONUS && _uTimeBuff[eType] > C_MAX_HERO_COUNT )
    {
        nResult = C_MAX_HERO_COUNT;
    }
    
    if ( nResult < 0 )
    {
        nResult = 0;
    }
    return nResult;
}

void UserInfoManager::setBuffTime(E_BUFF_TYPE eType, int nTime, bool bSave)
{
    if ( _uTimeBuff[eType] < 0 )
    {
        _uTimeBuff[eType] = 0;
    }
        
    _uTimeBuff[eType] += nTime;
    if ( eType == E_BUFF_TYPE::BUFF_HERO_BONUS && _uTimeBuff[eType] > C_MAX_HERO_COUNT )
    {
        _uTimeBuff[eType] = C_MAX_HERO_COUNT;
    }
        
    if ( bSave )
        SaveManager::saveBuffTime();
}

bool UserInfoManager::getBuffVisible(E_BUFF_TYPE eType)
{
    bool bResult = true;
    if ( eType == E_BUFF_TYPE::BUFF_HERO_BONUS )
    {
        bResult = UserDefault::getInstance()->getBoolForKey(KEY_VISIBLE_HEOR_BONUS, true);
    }
    
    return bResult;
}

void UserInfoManager::setBuffVisible(E_BUFF_TYPE eType, bool bVisible)
{
    if ( eType == E_BUFF_TYPE::BUFF_HERO_BONUS )
    {
        UserDefault::getInstance()->setBoolForKey(KEY_VISIBLE_HEOR_BONUS, bVisible);
    }
}

time_t UserInfoManager::getBuffRevival()
{
    time_t nTime = 0;
    
    std::string strEncrypt = UserDefault::getInstance()->getStringForKey(KEY_REVIVE_BUFF);
    if ( !strEncrypt.empty() )
    {
        std::string strDecrypt = MafAes256::Decrypt(strEncrypt);
        nTime = std::atoi(strDecrypt.c_str());
    }
    
    return nTime;
}

void UserInfoManager::setBuffRevival(time_t nTime)
{
    UserDefault::getInstance()->setStringForKey(KEY_REVIVE_BUFF, MafAes256::Encrypt(MafUtils::toString(nTime)));
}

#pragma mark - battle : revival
int UserInfoManager::getAdsTime(E_SHOP_ADS eType)
{
    return _uTimeAds[eType];
}

void UserInfoManager::setAdsTime(E_SHOP_ADS eType, int nTime, bool bSave)
{
    _uTimeAds[eType] += nTime;
    if ( _uTimeAds[eType] < 0 )
    {
        _uTimeAds[eType] = 0;
    }
    
    if ( bSave )
        SaveManager::saveAdsTime();
}

void UserInfoManager::setTimeAdsAllMinus()
{
    bool bChange = false;
    for ( int i = E_SHOP_ADS::SHOP_GEM_ADS; i <= E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL; i++ )
    {
        E_SHOP_ADS eType = (E_SHOP_ADS)i;
        
        if ( getAdsTime(eType) > 0 )
        {
            setAdsTime(eType, -1, false);
            bChange = true;
        }
    }
    
    if ( bChange )
        SaveManager::saveAdsTime();
}



#pragma mark - battle : hit or kill
bool UserInfoManager::getOneKill(E_PLACE ePlace)
{
    bool bResult = false;
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        bResult = _bOneKillPrison;
    }
    else
    {
        bResult = _bOneKill;
    }
    
    return  bResult;
}

void UserInfoManager::setOneKill(E_PLACE ePlace, bool bKill)
{
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        _bOneKillPrison = bKill;
    }
    else
    {
        _bOneKill = bKill;
    }
}

int UserInfoManager::getHitCount(E_PLACE ePlace)
{
    int hitCount = 0;
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        hitCount = _hitCountDungeon;
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        hitCount = _hitCountDevil;
    }
    else
    {
        hitCount = _hitCount;
    }
        
    if ( hitCount < 0 )
        hitCount = 0;
    
    return hitCount;
}

void UserInfoManager::setHitCount(E_PLACE ePlace, int hitCount)
{
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        _hitCountDungeon = hitCount;
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _hitCountDevil = hitCount;
    }
    else
    {
        _hitCount = hitCount;
    }
}

int UserInfoManager::getHongHitCount(E_PLACE ePlace)
{
    int hitCount = 0;
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        hitCount = _honghitCountDungeon;
    }
    else
    {
        hitCount = _honghitCount;
    }
        
    if ( hitCount < 0 )
        hitCount = 0;
    
    return hitCount;
}

void UserInfoManager::setHongHitCount(E_PLACE ePlace, int hitCount)
{
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        _honghitCountDungeon = hitCount;
    }
    else
    {
        _honghitCount = hitCount;
    }
}

int UserInfoManager::getHitCountDevilHedgehog()
{
    int nResult = _hitCountDevilHedgehog;
    if ( nResult < 0 )
        nResult = 0;
    
    return nResult;
}
void UserInfoManager::setHitCountDevilHedgehog(int nVal)
{
    _hitCountDevilHedgehog = nVal;
}
int UserInfoManager::getHitCountDevilFlash()
{
    int nResult = _hitCountDevilFlash;
    if ( nResult < 0 )
        nResult = 0;
    
    return nResult;
}
void UserInfoManager::setHitCountDevilFlash(int nVal)
{
    _hitCountDevilFlash = nVal;
}

int UserInfoManager::getPrisonEnterCount()
{
    return _prisonEnterCount;
}
void UserInfoManager::setPrisonEnterCount(int nVal)
{
    _prisonEnterCount = nVal;
}

int UserInfoManager::getRaidAttackCount()
{
    return _raidAtkCount;
}
void UserInfoManager::setRaidAttackCount(int nVal)
{
    _raidAtkCount = nVal;
}






#pragma mark - battle : 획득 골드
std::string UserInfoManager::getSecondEarnGoldNormal(bool bChange)
{
    //
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    
    //
    std::string total = "10";
    std::string earnGold[C_COUNT_QUEST+1] = {"0",};
    int perTime = getQuestTimeRate(ePlace);
    bool bChangeCheck = bChange;
    
    for ( int i = 1; i <= C_COUNT_QUEST; i++ )
    {
        int level = getQuestLevel(ePlace, (E_QUEST)i);
        earnGold[i] = getQuestEarnGold(ePlace, (E_QUEST)i, level);
    }
    
    if(bChangeCheck == false)
    {
        if ( _secondEarnCheck1 != perTime )
        {
            _secondEarnCheck1 = perTime;
            bChangeCheck = true;
        }
        
        for ( int i = 1; i <= C_COUNT_QUEST; i++ )
        {
            if (_secondEarnCheck3[i].compare(earnGold[i]) != 0) {
                _secondEarnCheck3[i] = earnGold[i];
                bChangeCheck = true;
            }
        }
        
        if ( bChangeCheck )
        {
            _saveSecondEarnGold = "";
        }
        else
        {
            if ( _saveSecondEarnGold.length() > 0 )
            {
                return _saveSecondEarnGold;
            }
        }
    }
    
    for(int i=1; i<=C_COUNT_QUEST;i++)
    {
        
        int level = getQuestLevel(ePlace, (E_QUEST)i);
        if(level==0)
            continue;
        
        std::string gold = getQuestEarnGold(ePlace, (E_QUEST)i, level);
        double timer = DataManager::GetQuestSpendTime(ePlace, (E_QUEST)i);
        if(perTime<100){
            timer = timer*perTime*0.01;
        }
        
        //나눗셈 역으로계산.
        if(timer>1){
            std::string temp = MafUtils::toString(timer);
            size_t len = temp.length()+1;
            temp.clear();
            temp.insert(0, "1");
            for(int i=0; i<len-1; i++){
                temp.append("0");
            }
            double val = utils::atof(temp.c_str());
            val /= timer;
            temp = MafUtils::toString(val);
            //std::string a = gold/(int)timer;
            val*=10; //소숫점 계산 정확성위해..
            std::string a =  MafUtils::bigMulNum(gold, MafUtils::doubleToString(val));
            a.pop_back();
            
            size_t size = MafUtils::toString(timer).length();
            for(int kk=0; kk<size; kk++)
                a.pop_back();
            
            total = MafUtils::bigAddNum(total, a);
        }
        else{
            total = MafUtils::bigAddNum(total,gold);
        }
        
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_3))
    {
        total = MafUtils::bigMulNum(total, MafUtils::doubleToString(125.f));
        total.pop_back();
        total.pop_back();
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_31))
    {
        double per = 7 + WeaponSpiritManager::getInstance()->getCurrentMileageLv() * .5f;
        per *= 100;
        
        total = MafUtils::bigMulNum(total, MafUtils::doubleToString(per));
        total.pop_back();
        total.pop_back();
    }
    
    float per = 1.0f;
    per += DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_5, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_5), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_5));
    per *= 100;
    
    total = MafUtils::bigMulNum(total, MafUtils::doubleToString(per));
    total.pop_back();
    total.pop_back();
    
    //모험 유물 적용
    double advtRelicBuff = 100.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicEffectBuffCalculator(E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_GOLD);
    
    total = MafUtils::bigMulNum(total, MafUtils::doubleToString(advtRelicBuff));
    total.pop_back();
    total.pop_back();
    
    _saveSecondEarnGold = total;
    
    return total;
}

std::string UserInfoManager::getSecondEarnGoldDevil()
{
    //
    E_PLACE ePlace = E_PLACE::DG_DEVIL_DOM;
    
    //
    std::string total = "10";
    std::string earnGold[C_COUNT_QUEST_DEVIL+1] = {"0",};
    
    int perTime = getQuestTimeRate(E_PLACE::DG_DEVIL_DOM);
    bool bChangeCheck = false;
    
    for ( int i = 1; i <= C_COUNT_QUEST_DEVIL; i++ )
    {
        int level = getQuestLevel(ePlace, (E_QUEST)i);
        earnGold[i] = getQuestEarnGoldDevil(ePlace, (E_QUEST)i, level);
    }

    if ( _secondEarnCheckDevil1 != perTime )
    {
        _secondEarnCheckDevil1 = perTime;
        bChangeCheck = true;
    }
  
    
    for ( int i = 1; i <= C_COUNT_QUEST_DEVIL; i++ )
    {
        if (_secondEarnCheckDevil3[i].compare(earnGold[i]) != 0)
        {
            _secondEarnCheckDevil3[i] = earnGold[i];
            bChangeCheck = true;
        }
    }
    
    if ( bChangeCheck )
    {
        _saveSecondEarnGoldDevil = "";
    }
    else
    {
        if ( _saveSecondEarnGoldDevil.length() > 0 )
        {
            return _saveSecondEarnGoldDevil;
        }
    }
    
    for(int i=1; i<=C_COUNT_QUEST_DEVIL;i++)
    {
        
        int level = getQuestLevel(ePlace, (E_QUEST)i);
        if(level==0)
            continue;
        
        std::string gold = getQuestEarnGoldDevil(ePlace, (E_QUEST)i, level);
        double timer = DataManager::GetQuestSpendTime(ePlace, (E_QUEST)i);
        if(perTime<100){
            timer = timer*perTime*0.01;
        }
        
        //나눗셈 역으로계산.
        if(timer>1){
            std::string temp = MafUtils::toString(timer);
            size_t len = temp.length()+1;
            temp.clear();
            temp.insert(0, "1");
            for(int i=0; i<len-1; i++){
                temp.append("0");
            }
            double val = utils::atof(temp.c_str());
            val /= timer;
            temp = MafUtils::toString(val);
            //std::string a = gold/(int)timer;
            val*=10; //소숫점 계산 정확성위해..
            std::string a =  MafUtils::bigMulNum(gold, MafUtils::doubleToString(val));
            a.pop_back();
            
            size_t size = MafUtils::toString(timer).length();
            for(int kk=0; kk<size; kk++)
                a.pop_back();
            
            total = MafUtils::bigAddNum(total, a);
        }
        else{
            total = MafUtils::bigAddNum(total, gold);
        }
        
    }
    
    _saveSecondEarnGoldDevil = total;
    
    return total;
}

std::string UserInfoManager::getEarnGoldFinal(E_PLACE ePlace, E_EARNGOLD eEARN, bool bShopBuy)
{
    std::string strEarn = "0";
    if ( eEARN == E_EARNGOLD::SECOND_DEFAULT )
    {
        //
        std::lock_guard<std::mutex> lock(_mutexGoldSecond);
        
        switch (ePlace) {
            case E_PLACE::DG_NORMAL:
                strEarn = getSecondEarnGoldNormal();
                break;
            case E_PLACE::DG_DEVIL_DOM:
                strEarn = getSecondEarnGoldDevil();
                break;
                
            default:
                break;
        }
    }
    else
    {
        if ( ePlace == E_PLACE::DG_NORMAL )
        {
            /*2016 12/15 공식 수정.
             현행방식
             골드충전 = 초당획득골드 * 4000 *4
             대량충전 = 초당획득골드 * 4000 *40
             동영상  = 초당획득골드 * 1/20 * 5
             
             업데이트
             골드충전 = 초당획득골드 * 4000 *4 +   적처리 * 100
             대량충전 = 초당획득골드 * 4000 *40 +  적처치 * 1000
             동영상  = 초당획득골드 * 4000 +      적처치 * 50
             몬스터처치 = 초당획득골드
             
             #적처리 = 초당획득골드*황금수정
             */
            strEarn = getSecondEarnGoldNormal();
            if( eEARN == E_EARNGOLD::MON_KILL )
            {
                //
                std::lock_guard<std::mutex> lock(_mutexGoldMonsterKill);
                
                if ( !MafUtils::compareStr(strEarn, "10") )
                {
                    strEarn = "10";
                }
                
                std::string bonus = getMonsterBonusRate(ePlace);
                strEarn = MafUtils::bigMulNum(strEarn, bonus);
                strEarn.pop_back();
                strEarn.pop_back();
                
                //공식의 10분의1
                if ( strEarn.size() >= 2 )
                    strEarn.pop_back();
                
                //최저는 30
                if ( !MafUtils::compareStr(strEarn, "30") )
                {
                    strEarn = "30";
                }
                
                if ( bShopBuy == false )
                {
                    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_3) )
                    {
                        int ran = random(1, 5);//20% 확률로 3배 골드 획득
                        if ( ran == 1 )
                            strEarn = MafUtils::bigMulNum(strEarn, MafUtils::toString(3));
                    }
                }
            }
            else
            {
                //
                std::lock_guard<std::mutex> lock(_mutexGold);
                
                std::string strEarnKill = getEarnGoldFinal(ePlace, E_EARNGOLD::MON_KILL, true);
                if( eEARN == E_EARNGOLD::ADS_SHOP )
                {
                    strEarn = MafUtils::bigMulNum(strEarn, "4000");
                    strEarnKill = MafUtils::bigMulNum(strEarnKill, "50");
                    strEarn = MafUtils::bigAddNum(strEarn, strEarnKill);
                }
                else if( eEARN == E_EARNGOLD::GEM_SHOP )
                {
                    strEarn = MafUtils::bigMulNum(strEarn, "16000");
                    strEarnKill.append("00");
                    strEarn = MafUtils::bigAddNum(strEarn, strEarnKill);
                    
                    int floorEffect = std::pow(10, (int)(getHighFloor(E_PLACE::DG_NORMAL) / 3000));
                    if ( floorEffect > 1 )
                    {
                        if ( floorEffect > 8 )
                            floorEffect = 8;
                        
                        strEarn = MafUtils::bigMulNum(strEarn, MafUtils::toString(floorEffect));
                    }
                }
                else if( eEARN == E_EARNGOLD::GEM_BIG_SHOP )
                {
                    strEarn = MafUtils::bigMulNum(strEarn, "160000");
                    strEarnKill.append("000");
                    strEarn = MafUtils::bigAddNum(strEarn, strEarnKill);
                    
                    double floorEffect = std::pow(10, (int)(getHighFloor(E_PLACE::DG_NORMAL) / 3000));
                    if ( floorEffect > 1 )
                    {
                        if ( floorEffect > 8 )
                            floorEffect = 8;
                        
                        strEarn = MafUtils::bigMulNum(strEarn, MafUtils::toString(floorEffect));
                    }
                }
            }
        }
        else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            
        }
    }
    
    return strEarn;
}

#pragma mark - battle : 적처치 골드 증가
std::string UserInfoManager::getMonsterBonusRate(E_PLACE ePlace, bool bArtifactPercent)
{
    std::string strEarn = getMonsterBonusRateNormal(ePlace, bArtifactPercent);
    return strEarn;
}

std::string UserInfoManager::getMonsterBonusRateNormal(E_PLACE ePlace, bool bArtifactPercent)
{
    std::string value = "100";
    double per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_8_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_8_EVOL1));
    value = MafUtils::bigAddNum(value, MafUtils::doubleToString(per));
    
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_16_EVOL2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_16_EVOL2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_16_EVOL2));
    if(per > 0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_20)){
            per += per*1.0;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_21_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_21_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_21_EVOL3));
    if(per > 0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_24)){
            per += per*0.7;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_26_EVOL4, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_26_EVOL4), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_26_EVOL4));
    if(per > 0){
        double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_8, getRelicLevel(ePlace, E_RELIC::RELIC_8)) / 100.0f;
        if (relicPer > 0) {
            per += per * relicPer;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_31_EVOL5, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_31_EVOL5), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_31_EVOL5));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //6차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_38_EVOL6, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_38_EVOL6), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_38_EVOL6));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //7차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_43_EVOL7, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_43_EVOL7), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_43_EVOL7));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //8차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_49_EVOL8, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_49_EVOL8), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_49_EVOL8));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //9차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_57_EVOL9, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_57_EVOL9), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_57_EVOL9));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //10차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_62_EVOL10, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_62_EVOL10), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_62_EVOL10));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //11차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_67_EVOL11, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_67_EVOL11), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_67_EVOL11));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    //12차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_72_EVOL12, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_72_EVOL12), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_72_EVOL12));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    //13차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_77_EVOL13, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_77_EVOL13), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_77_EVOL13));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    //14차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_82_EVOL14, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_82_EVOL14), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_82_EVOL14));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //15차 수정
    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_87_EVOL15, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_87_EVOL15), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_87_EVOL15));
    if(per > 0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
        value = MafUtils::bigAddNum(value, tt);
    }
    
    //무한(초월) 수정
    value = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(value, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
    
    //정령 효과
    value = WeaponSpiritManager::getInstance()->getSpiritCalculation(value, WeaponSpiritManager::E_EFFECT_TYPE::GOLD);
    
    //황금수정 효율 코스튬
    double costumeBuff = 1.0;
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_9)){
        double buff = PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2)*0.1f;
        if (buff > 0) {
            costumeBuff += costumeBuff * buff;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_14)){
        costumeBuff += costumeBuff*1.5f;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_17)){
        double keyBuff = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_10_KEY) * 0.06f;
        if (keyBuff > 0) {
            costumeBuff += costumeBuff * keyBuff;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_18)){
        double per = (CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HAND) + CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::FOOT)) * 0.07;
        if (per > 0) {
            costumeBuff += costumeBuff * per;       //7%씩 증가
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_3)){
        if(getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS)>0){
            costumeBuff += costumeBuff * 1.0f;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_30)){
        double per = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_5) * 0.02f;
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_3)){
        costumeBuff += costumeBuff * 1.0f;
    }
    
    //몬스터 처치 보상 증가 코스튬
    if ( bArtifactPercent == true )
    {
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_37)) {
            costumeBuff += costumeBuff * 3.0f;//300% 증가
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_7)){
            if ( WeaponManager::getInstance()->getEquip() == WeaponManager::getInstance()->getEquipReborn() )
            {
                costumeBuff += costumeBuff * 3.0f;
            }
            else{
                costumeBuff += costumeBuff * 2.0f;
            }
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_8)){
            if (isFloorCheck(815, true)) {
                costumeBuff += costumeBuff * 1.5f;
            }
        }
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_3)){
            double per = getAttendVip() * 0.56;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
    }
    
    costumeBuff *= 100;
    value = MafUtils::bigMulNum(value, MafUtils::doubleToString(costumeBuff));
    value.pop_back();
    value.pop_back();
    
    //봉식타워5c
    if ( bArtifactPercent == true )
    {
        int buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_5);
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
            buildinglevel += 7;
        }
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
            buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
        }
        double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, getRelicLevel(ePlace, E_RELIC::RELIC_9));
        if (relicPer > 0) {
            buildinglevel += relicPer;
        }
        
        double buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_5, buildinglevel);
        if(buildingPer>0){
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(buildingPer));
            tt.pop_back();
            tt.pop_back();
            value =MafUtils::bigAddNum(value, tt);
        }
    }
    
    //raid dmg
    float raidAccrueBuff = 100.f;
    
    raidAccrueBuff += NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::GOLD);
    value = MafUtils::bigMulNum(value, MafUtils::doubleToString(raidAccrueBuff));
    value.pop_back();
    value.pop_back();
    
    //모험 유물 적용
    double advtRelicBuff = 100.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_KILL_GOLD);
    
    value = MafUtils::bigMulNum(value, MafUtils::doubleToString(advtRelicBuff));
    value.pop_back();
    value.pop_back();
    
    return value;
}

#pragma mark - battle : 퀘스트 무기 골드 감소
int UserInfoManager::getBuySaleRate(E_PLACE ePlace)
{
    //2017/11/1 원래는 퍼센트를 구한후에 계산을 다시 진행했지만 한번더 계산할 필요없도록 변경함.
    //기존 if(per > 0)   ->  if(per<100)
    
    int nSale = 100;
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        nSale -= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_6, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_6), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_6));
    }
    else if( ePlace == E_PLACE::DG_DEVIL_DOM)
    {
        
    }
    
    return nSale;
}

#pragma mark - battle : 공격력
std::string UserInfoManager::getAttackDamage(E_PLACE ePlace, E_CHARACTER eCharac, bool bCri, bool bOffline)
{
    //
    std::lock_guard<std::mutex> lock(_mutexDamage);
    
    //위치별 공격력 분리 , 펫은 많지 않기 때문에 안에 넣었다.
    std::string dmg = "0";
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        dmg = getAttackDamageNormal(eCharac, bOffline);
    }
    else if ( ePlace == E_PLACE::DG_PRISON )
    {
        dmg = getAttackDamagePrison(eCharac);
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        dmg = getAttackDamageDevil();
        if ( eCharac == E_CHARACTER::CHA_D_PET_PENGUIN )
        {
            auto per = PetNewDevilManager::getInstance()->getPetEffect(E_PET::D_1_PENGUIN, PetNewDevilManager::getInstance()->getPetLevel(E_PET::D_1_PENGUIN));
            dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(per));
            dmg.pop_back();
            dmg.pop_back();
        }
    }
    
    if ( bCri )
    {
        dmg = getAttackCriDamage(ePlace, dmg);
    }
    
    return dmg;
}

std::string UserInfoManager::getAttackDamageNormal(E_CHARACTER eCharac, bool bOffline)
{
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    
    int weaponEquip = WeaponManager::getInstance()->getEquip();
    int weaponLevel = WeaponManager::getInstance()->getLevel(weaponEquip);
    
    int countHero = getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
    if ( countHero > 0 )
    {
        weaponLevel += countHero;
        if ( weaponLevel > C_MAX_EQUIP )
        {
            weaponLevel -= C_MAX_EQUIP;
            weaponEquip = weaponEquip + 1;
        }
    }
    
    std::string dmg = WeaponManager::getInstance()->getAttack(weaponEquip, weaponLevel, getHonorLevel(E_HONOR::HONOR_1));
    
    //힘반
    auto addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_1_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_1_EVOL1));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_2)){
            addPer += addPer*0.5;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_12)){
            addPer += addPer*1.5;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_24)){
            if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1) >= DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_12_CROSS_EVOL1))
                addPer += addPer*1.2;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //진화힘반
    addPer = DataManager::GetArtifactEffect(ARTI_13_EVOL2, getArtifactLevel(ePlace, ARTI_13_EVOL2), getArtifactReinforce(ePlace, ARTI_13_EVOL2));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_20)){
            addPer += addPer*1.0;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_24)){
            if (isFloorCheck(3001, true))
                addPer += addPer*2.5;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //벼려진힘반
    addPer = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_18_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_18_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_18_EVOL3));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_24)){
            addPer += addPer*0.7;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //초월의힘반
    addPer = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_23_EVOL4, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_23_EVOL4), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_23_EVOL4));
    if(addPer>0){
        //초월의 힘반 증가
        double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_8, getRelicLevel(ePlace, E_RELIC::RELIC_8)) / 100.0f;
        if (relicPer > 0) {
            addPer += addPer * relicPer;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //5차부터 공식같고, 예외없음.
    for(int i=E_ARTIFACT::ARTI_28_EVOL5; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++){
        E_ARTIFACT arti =(E_ARTIFACT)i;
        if ( DataManager::GetArtifactType(arti)==E_ARTIFACT_TYPE::ARTI_TYPE_ATK && DataManager::GetArtifactGeneration(arti) >=5 )
        {
            addPer = DataManager::GetArtifactEffect(arti, getArtifactLevel(ePlace, arti), getArtifactReinforce(ePlace, arti));
            if(addPer>0){
                auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
                dmg =MafUtils::bigAddNum(dmg, tt);
            }
        }
    }
    
    //무한(초월) 보물 반지
    dmg = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(dmg, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
    
    //십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_12_CROSS_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_21)){
            addPer += addPer*2.0;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //진화 십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_17_CROSS_EVOL2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_17_CROSS_EVOL2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_17_CROSS_EVOL2));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_24)){
            addPer += addPer*2.5;
        }
        
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //벼려진 십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_22_CROSS_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_22_CROSS_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_22_CROSS_EVOL3));
    if(addPer>0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    
    //4차부터 공식같고, 예외없음.
    for(int i=E_ARTIFACT::ARTI_27_CROSS_EVOL4; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++){
        E_ARTIFACT arti =(E_ARTIFACT)i;
        if(DataManager::GetArtifactGeneration(arti) >=4 && DataManager::GetArtifactType(arti)==E_ARTIFACT_TYPE::ARTI_TYPE_CROSS)
        {
            addPer = DataManager::GetArtifactEffect(arti, getArtifactLevel(ePlace, arti), getArtifactReinforce(ePlace, arti));
            if(addPer>0){
                auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
                dmg =MafUtils::bigAddNum(dmg, tt);
            }
        }
    }
    
    //무한(초월) 보물 십자가
    dmg = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(dmg, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
    
    // 차원 아뮬렛
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_45, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_45), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_45));
    if(addPer>0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    
    //강화석(공격력)
    double stoneBuff = 1.0f;
    stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::ATTACK);
    stoneBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //새로운 강화석(공격력)
    double stoneForceBuff = 1.0f;
    stoneForceBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_ATTACK);
    stoneForceBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneForceBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    double stoneForce2Buff = 1.0f;
    stoneForce2Buff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_ATTACK);
    stoneForce2Buff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneForce2Buff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(무기)
    double weaponDexBuff = 1.0f;
    weaponDexBuff += WeaponManager::getInstance()->getDexEffect();
    weaponDexBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(weaponDexBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(무기)
    double weaponReinforceBuff = 1.0f;
    weaponReinforceBuff += WeaponManager::getInstance()->getReinforceEffect();
    weaponReinforceBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(weaponReinforceBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(몬스터)
    double dexMonsterBuff = 1.0f;
    dexMonsterBuff += MonsterManager::getInstance()->getDexEffect();
    dexMonsterBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(dexMonsterBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    
    
    //펫 장비 수집 효과
    double petEquipper = PetNewManager::getInstance()->getPetEquipCollectEffect();
    if ( petEquipper > 0 )
    {
        std::string petequipper = MafUtils::doubleToString(petEquipper);
        dmg = MafUtils::bigMulNum(dmg, petequipper);
    }
    
    
    //봉식타워(봉식타워:일반던전, 지하감옥 공격력)
    int buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
        buildinglevel += 7;
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, getRelicLevel(ePlace, E_RELIC::RELIC_9));
    if (relicPer > 0) {
        buildinglevel += relicPer;
    }
    
    //봉식타워(누리봉 위성)
    buildinglevel += TownManager::getInstance()->getTownLevel(E_TOWN::SATELLITE);
    
    double buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_1, buildinglevel);
    if(buildingPer>0){
        auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(buildingPer));
        tt.pop_back();
        tt.pop_back();
        dmg =MafUtils::bigAddNum(dmg, tt);
        
        double costumeBuff = 1;
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_28)){
            double per = PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2) * 0.15f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        costumeBuff *= 100;
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
        dmg.pop_back();
        dmg.pop_back();
    }
    
    //봉식타워(봉파트:일반던전 공격력)
    buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_2);
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
        buildinglevel += 7;
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, getRelicLevel(ePlace, E_RELIC::RELIC_9));
    if (relicPer > 0) {
        buildinglevel += relicPer;
    }
    buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_2, buildinglevel);
    if(buildingPer>0){
        auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(buildingPer));
        tt.pop_back();
        tt.pop_back();
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    
    int floorTemp = getFloor(ePlace);
    double effect = PetNewManager::getInstance()->getPetEquipEffect(301, 1);
    if(effect > 1)
    {
        int oddcount = 0;//기본 0배
        while(true)
        {
            if(floorTemp <= 0)
            {
                break;
            }
            if((floorTemp%10)%2 == 1)
            {
                oddcount++;
            }
            floorTemp = floorTemp/10;
        }
        effect = effect * oddcount;
        if ( effect > 0 )
        {
            dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(effect * 100));
            dmg.pop_back();
            dmg.pop_back();
        }
    }
    effect = PetNewManager::getInstance()->getPetEquipEffect(801, 1);
    if ( effect > 1 )
    {
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(effect * 100));
        dmg.pop_back();
        dmg.pop_back();
    }
    
    if ( getHongHitCount(ePlace) >= 1 )
    {
        auto HongBuff = PetNewManager::getInstance()->getPetEquipEffect(602, 1);
        dmg = MafUtils::bigMulNum(dmg,MafUtils::doubleToString(HongBuff*100));
        dmg.pop_back();
        dmg.pop_back();
    }

    //코스튬 버프
    double costumeBuff = 1;
    
    //
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    
    //
    if ( !bOffline && getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS) > 0 )
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_2) )
            costumeBuff *= 10;
        else
            costumeBuff *= C_ATTACK;
        
        if ( objPass != nullptr && objPass->isBuyPass() == true )
        {
            time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( nDiffTime > 0 )
            {
                costumeBuff *= 2;
            }
        }
    }
    
    if ( !bOffline && getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS) > 0 )
    {
        costumeBuff *= 9;
    }
    
    if ( !bOffline && getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_10_BONUS) > 0 )
    {
        costumeBuff *= 10;
    }
    
    if ( objPass != nullptr && objPass->isBuyPass() == true )
    {
        time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
        if ( nDiffTime > 0 )
        {
            costumeBuff += costumeBuff * 1.5;   //150%증가
        }
    }
    
    // 뉴비 지원 버프
    if ( bOffline == false )
    {
        if ( UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5 )
        {
            E_NEWBIE_STEP eNewbie = UserInfoConfigManager::getInstance()->getNewbieStep();
            
            double per = 0.0;
            if ( eNewbie == E_NEWBIE_STEP::STEP_1 )
            {
                per = 5.0;
                per *= std::pow(2.0, MIN(20, getHitCount(ePlace) / 100));
            }
            else if ( eNewbie == E_NEWBIE_STEP::STEP_2 )
            {
                per = 4.0;
                per *= std::pow(2.0, MIN(19, getHitCount(ePlace) / 100));
            }
            else if ( eNewbie == E_NEWBIE_STEP::STEP_3 )
            {
                per = 3.0;
                per *= std::pow(2.0, MIN(18, getHitCount(ePlace) / 100));
            }
            else if ( eNewbie == E_NEWBIE_STEP::STEP_4 )
            {
                per = 2.0;
                per *= std::pow(2.0, MIN(17, getHitCount(ePlace) / 100));
            }
            else if ( eNewbie == E_NEWBIE_STEP::STEP_5 )
            {
                per = 1.0;
                per *= std::pow(2.0, MIN(16, getHitCount(ePlace) / 100));
            }
            
            if ( per > 0.0 )
            {
                costumeBuff += costumeBuff * per;
            }
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_4)){
        costumeBuff += costumeBuff * 1.0;   //100%증가
    }
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_6)) {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY) > 0) {
            double per = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY) * 0.05;
            costumeBuff += costumeBuff * per;       //3%씩 증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_13)) {
        double petBuff = 0;
        if(PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1) >= 20)
            petBuff += 0.3f;
        for (int i=E_PET::HAMSTER_2; i<=C_COUNT_PET; i++) {
            if(PetNewManager::getInstance()->getPetLevel((E_PET)i) >= PetNewManager::getInstance()->getPetLevelMax((E_PET)i))
                petBuff += 0.3f;
        }
        costumeBuff += costumeBuff * petBuff;
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_16) )
    {
        if (getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS) > 0 && getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS) > 0 && getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS) > 0)
            costumeBuff += costumeBuff * 3;   //300%증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = ItemsMoneyManager::getInstance()->getKey().length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getKeyLength();
        }
        
        double keyBuff = nLength * 0.2f;
        if (keyBuff > 0)
            costumeBuff += costumeBuff * keyBuff;   //20%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem()).length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getGemLength();
        }
        
        double gemBuff = nLength * 0.5f;
        if (gemBuff > 0)
            costumeBuff += costumeBuff * gemBuff;   //50%씩 증가
    }

    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_18)) {
        double per = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD) * 0.13;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //13%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_19)) {
        costumeBuff += costumeBuff;       //100% 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_20)) {
        double per = StoneManager::getInstance()->getCountAll() * 0.01f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //1%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_20)) {
        double per = getArtifactExistCount(ePlace) * 0.15f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_23)) {
        if ( bOffline == false )
        {
            if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_CRITICAL || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
                costumeBuff *= 8;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_24)) {
        double per = getRelicLevel(ePlace, E_RELIC::RELIC_1) * 0.15f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }

    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_25)) {
        double per = SpecialMissionManager::getInstance()->getSpecialMissionClear() * 0.04f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //4%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_33)){
        if(getHitCount(ePlace) > 1)
            costumeBuff += costumeBuff * 3.0f;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_37)){
        costumeBuff += costumeBuff * 2.5;   //250%증가
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_39)){
        costumeBuff += costumeBuff * 4.86;   //486%증가
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_4)){
        double per = CostumeManager::getInstance()->getExistAuraCount() * 0.4;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_12)){
        double per = getHighFloor(ePlace) / 1000 * 1.0f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_17)) {
        int petReinforceCount = 0;
        for (int i=1; i<=C_COUNT_PET; i++) {
            petReinforceCount += PetNewManager::getInstance()->getPetReinforce((E_PET)i);
        }
        double per = petReinforceCount * 0.3;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_18)) {
        double per = CostumeManager::getInstance()->getExistCount() * 0.03;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_35)) {
        int count = getArtifactMaxLevelCount(ePlace);
        double per = count * 0.25f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }

    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = ItemsMoneyManager::getInstance()->getGold().length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getGoldLength();
        }
        
        double goldBuff = nLength * 0.05f;
        if (goldBuff > 0)
            costumeBuff += costumeBuff * goldBuff;   //5%씩 증가
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_4)){
        if (isFloorCheck(1001, true))
            costumeBuff += costumeBuff * 2.0;   //마계에서 봉식이와 크르르 데미지200%
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_5)){
        if (isFloorCheck(1000, false))
            costumeBuff += costumeBuff * 0.75;     //1000층이하에서 봉식이,크르르 데미지75%
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_7)) {
        if (getQuestLevel(ePlace, E_QUEST::QUEST_24) > 0) {
            double per = getQuestLevel(ePlace, E_QUEST::QUEST_24) * 0.01;
            costumeBuff += costumeBuff * per;   //1%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_8)) {
        if (getQuest(ePlace, E_QUEST::QUEST_25)) {
            if (UserInfoStackManager::getInstance()->getQuest25Buff() > 0) {
                double per = UserInfoStackManager::getInstance()->getQuest25Buff() * 0.01;
                costumeBuff += costumeBuff * per;   //1%씩증가
            }
        }
    }

    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_11)) {
        if (UserInfoStackManager::getInstance()->getRevivalAttackBuff() > 0) {
            double per = UserInfoStackManager::getInstance()->getRevivalAttackBuff() * 0.2;
            costumeBuff += costumeBuff * per;   //20%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_15)) {
        if (UserInfoStackManager::getInstance()->getEnchantRevivalAttackBuff() > 0) {
            double per = UserInfoStackManager::getInstance()->getEnchantRevivalAttackBuff() * 0.5;
            costumeBuff += costumeBuff * per;   //50%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_12)) {
        if (UserInfoStackManager::getInstance()->getDragonBuff() > 0) {
            double per = UserInfoStackManager::getInstance()->getDragonBuff() * 0.1;
            costumeBuff += costumeBuff * per;   //10%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_14)) {
        if (UserInfoStackManager::getInstance()->getHellBuff() > 0) {
            double per = UserInfoStackManager::getInstance()->getHellBuff() * 0.1;
            costumeBuff += costumeBuff * per;   //10%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_14)) {
        if (isFloorCheck(1500, true))
            costumeBuff += costumeBuff * 2.0;   //1501층 이상에서 데미지200%
    }

    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_21)) {
        if(1001 <= getFloor(ePlace) && getFloor(ePlace) <= 2000){
            double per = getQuestLevel(ePlace, E_QUEST::QUEST_22) * 0.03f;
            if (per > 0)
                costumeBuff += costumeBuff * per;       //마계에서 마계정벌 레벨 당 데미지
        }else if (isFloorCheck(6001, true)){//else if (UserInfoManager::getInstance()->getFloor() > 6000){
            double per = getQuestLevel(ePlace, E_QUEST::QUEST_22) * 0.01f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_21)) {
        double per = (WeaponManager::getInstance()->getEquipPermanent() - 1) * 0.075;
        if ( per < 0 )
            per = 0;
        costumeBuff += costumeBuff * per;   //3000층 이하에서 데미지250%
    }
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_23)) {
        if(isFloorCheck(3000, false))
            costumeBuff += costumeBuff * 2.5;   //3000층 이하에서 데미지250%
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_23)) {
        if(isFloorCheck(3001, true))
            costumeBuff += costumeBuff * 2.5;   //3001층 이상에서 데미지250%
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_22)) {
        double per = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_15_EVOL2) * 0.00005f;//0.05
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_25)){
        if (UserInfoStackManager::getInstance()->getReviveSpecial() > 0)
            costumeBuff += costumeBuff*3.0;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_27)) {
        int count = StoneManager::getInstance()->getCountAll();
        if (count > 0)
            count /= 3;
        double per = count * 0.04f;
        if (per > 0) {
            if (per > 3.2f)
                per = 3.2f;
            costumeBuff += costumeBuff * per;
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_27)) {
        int count = 0;
        for(int arti = (int)E_ARTIFACT::ARTI_13_EVOL2; arti <= (int)E_ARTIFACT::ARTI_17_CROSS_EVOL2; arti++){
            E_ARTIFACT eArti = (E_ARTIFACT)arti;
            if (getArtifactLevel(ePlace, eArti) >= DataManager::GetArtifactMaxLevel(eArti))
                count+=1;
        }
        double per = count * 0.50f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_27)) {
        double per = getRelicLevel(ePlace, E_RELIC::RELIC_2) * 0.12f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_28)){
        double per = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1) * 0.015f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_28)){
        if (getQuestLevel(ePlace,E_QUEST::QUEST_25) >= C_MAX_QUEST)
            costumeBuff += costumeBuff * 2.5f;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_29)){
        if(isFloorCheck(2000, false))
            costumeBuff += costumeBuff * 3.0;   //2000층 이하에서 데미지300%
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_29)){
        if(isFloorCheck(3000, false))
            costumeBuff += costumeBuff * 2.5;   //3000층 이하에서 데미지300%
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_29)){
        if(CostumeManager::getInstance()->isExistSet(E_COSTUME_IDX::IDX_29))
            costumeBuff += costumeBuff * 3.0;
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_30)){
        int floor = 0;
        for (int i=1; i<=C_COUNT_BUILDING; i++) {
            floor += TownManager::getInstance()->getTownLevel((E_TOWN)i);
        }
        if (floor >= 2) {
            double per = (floor/2) * 0.01f;
            if (per > 4.0f)
                per = 4.0f;
            costumeBuff += costumeBuff * per;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_31))
    {
        double per = 4 + WeaponSpiritManager::getInstance()->getCurrentMileageLv() * .5f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_31))
    {
        int64_t nLength = MafUtils::toString(NewRaidManager::getInstance()->getTotalAbilityPoint()).length();
        
        double per = nLength;
        if (per > 0)
            costumeBuff += costumeBuff * per;   //100%씩 증가
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_32)){
        double per = getAttendDay() * 0.1f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_34)){
        if(isFloorCheck(5000, false))
            costumeBuff += costumeBuff * 2.5;   //5000층 이하에서 데미지250%
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_35)) {
        int level = 0;
        for (int i=1; i<=C_COUNT_RELIC; i++) {
            level += getRelicLevel(ePlace, (E_RELIC)i);
        }
        double per = level * 0.04f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }

    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_35)) {
        int level = getRelicLevel(ePlace, E_RELIC::RELIC_3);
        double per = level * 0.15f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_35)) {
        int level = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY);
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_6))
            level+=10;
        if (level > 50) {
            level-=50;
            double per = level * 0.35f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_35)) {
        int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::FOOT);
        double per = count * 0.10f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_36) )
    {
        if (getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS) > 0)
            costumeBuff += costumeBuff * 2.0f;       //200% 증가
    }

    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_37)) {
        if (getFloorCount(ePlace)==C_MAX_FLOOR_COUNT)
            costumeBuff += costumeBuff * 5.0f;//500% 증가
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_5)){
        int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD);
        double per = count * 0.10f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_10)){
        if (UserInfoStackManager::getInstance()->getReviveKeyBooster()) {
            costumeBuff += costumeBuff * 10.0f;
        }
    }
    
    if (getFloor(ePlace) <= 3000) {
        double per = DataManager::GetRelicEffect(E_RELIC::RELIC_5, getRelicLevel(ePlace, E_RELIC::RELIC_5)) / 100.0f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }else{
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_39)) {
            double per = DataManager::GetRelicEffect(E_RELIC::RELIC_5, getRelicLevel(ePlace, E_RELIC::RELIC_5)) / 1000.0f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
    }

    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    if (eCharac == E_CHARACTER::CHA_HERO) {

        if ( bOffline == false )
        {
            if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ATTACK || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
            {
                if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_9)) {
                    costumeBuff *= 20;  //20배 증가
                }else{
                    costumeBuff *= C_BUFF_ATTACK; //공격력8배
                }
                
                double per = DataManager::GetRelicEffect(E_RELIC::RELIC_3, getRelicLevel(ePlace, E_RELIC::RELIC_3)) / 100.0f;
                if (per > 0) {
                    costumeBuff += costumeBuff * per;
                }
                
                int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::PRINCESS_4);
                double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::PRINCESS_4, reinforceLevel);
                if (reinforcePer > 0) {
                    costumeBuff *= reinforcePer;
                }
            }
        }
        
        
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_4)){
            double per = (getFloor(ePlace)/5) * 0.01;
            if(per > 0){
                costumeBuff += costumeBuff * per;   //1%씩증가
            }
        }
        
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_29)){
            if(getHitCount(ePlace) == 1)
                costumeBuff += costumeBuff * 10.0;   //첫 공격 1000%
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_14)) {
            if (UserInfoStackManager::getInstance()->getHitBuff() > 0) {
                double per = UserInfoStackManager::getInstance()->getHitBuff() * 0.2;
                costumeBuff += costumeBuff * per;   //20%씩증가
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_20)) {
            double per = getArtifactExistCount(ePlace) * 0.40f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;       //40%씩 증가
            }
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_37)) {
            costumeBuff += costumeBuff * 3.0;   //300%
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = 0.0;
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HEAD);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::BODY);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::CAPE);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HAND);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::FOOT);
            if (per > 0) {
                per = std::pow(per, 4.5);
                per /= 100;
                costumeBuff += costumeBuff * per;
            }
        }
        
        //펫 장비 수집 효과(봉식)
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = PetNewManager::getInstance()->getPetEquipCollectEffectBongsik();
            if(per > 0){
                costumeBuff *= per;
            }
        }
    }
    else if(eCharac == CHA_PET_DRAGON)
    {
        int level = PetNewManager::getInstance()->getPetLevel(E_PET::DRAGON_3);
        double per = PetNewManager::getInstance()->getPetEffect(E_PET::DRAGON_3, level);
        double minPer = (level-1)*4;
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_38)) {
            minPer += 10;
        }
        
        int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::DRAGON_3);
        double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::DRAGON_3, evolutionLevel);
        if (evolutionPer > 0) {
            minPer += evolutionPer;
        }
        
        int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::DRAGON_3);
        double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::DRAGON_3, reinforceLevel);
        if (reinforcePer > 0) {
            per += reinforcePer;
        }
        
        minPer *= 10;
        per *= 10;
        
        int attackPer = random(minPer, per);
        
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_9)) {
            if (UserInfoStackManager::getInstance()->getDragonSkillCheck() == 0) {
                if (random(1, 100) <= 20) {
                    attackPer = 1000;
                    UserInfoStackManager::getInstance()->setDragonSkillCheck(1);
                }
            }else{
                UserInfoStackManager::getInstance()->setDragonSkillCheck(0);
            }
        }
        
        int nType = 1;
        if(attackPer <=(minPer+100))
            nType = 1;
        else if(attackPer >= (per-100))
            nType = 3;
        else
            nType = 2;
        
        std::string myAtk = "0";
        auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
        if ( uiBattle != nullptr )
        {
            auto unitMonster = uiBattle->getUnitMonster();
            if ( unitMonster != nullptr )
            {
                myAtk = unitMonster->getMonsterHp();
            }
        }
        
        if ( attackPer != 1000 )
        {
            myAtk = MafUtils::bigMulNum(myAtk, MafUtils::toString(attackPer));
            if (myAtk.length() > 3) {
                myAtk.pop_back();
                myAtk.pop_back();
                myAtk.pop_back();
            }
            return myAtk;
        }
    }
    else
    {
        int level = PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
        double per = PetNewManager::getInstance()->getPetEffect(E_PET::CHICK_1, level);
        if(per!=1)
            costumeBuff *= per;
        
        int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::CHICK_1);
        double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::CHICK_1, reinforceLevel);
        if (reinforcePer > 0) {
            costumeBuff *= reinforcePer;
        }
     
        double petequipbonus = PetNewManager::getInstance()->getPetEquipEffect(100, 1);
        costumeBuff = costumeBuff * petequipbonus;
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_7))
        {
            if ( bOffline == false )
            {
                if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ATTACK || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
                {
                    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_9)) {
                        costumeBuff *= 20;
                    }else{
                        costumeBuff *= C_BUFF_ATTACK;
                    }
                    
                    double per = DataManager::GetRelicEffect(E_RELIC::RELIC_3, getRelicLevel(ePlace, E_RELIC::RELIC_3)) / 100.0f;
                    if (per > 0) {
                        costumeBuff += costumeBuff * per;
                    }
                    
                    int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::PRINCESS_4);
                    double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::PRINCESS_4, reinforceLevel);
                    if (reinforcePer > 0) {
                        costumeBuff *= reinforcePer;
                    }
                }
            }
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_18)) {
            double per = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::CAPE) * 0.20;
            if (per > 0) {
                costumeBuff += costumeBuff * per;       //20%씩 증가
            }
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_26)) {
            if (PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1) >= 21) {
                double per = (PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1)-20) * 0.30;
                if (per > 0) {
                    costumeBuff += costumeBuff * per;       //30%씩 증가
                }
            }
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_32)){
            double per = getAttendVip() * 0.50f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_9)) {
            double per = DataManager::GetRelicEffect(E_RELIC::RELIC_2, getRelicLevel(ePlace, E_RELIC::RELIC_2)) / 100.0f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_9)){
            if (isFloorCheck(1200, true)) {
                costumeBuff += costumeBuff * 3.5;//1200층 이상에서 크르르 데미지350%
            }
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_26)){
            if (isFloorCheck(3001, true)) {
                costumeBuff += costumeBuff * 4.0;//3001층 이상에서 크르르 데미지400%
            }
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_26)){
            if (isFloorCheck(3000, false)) {
                costumeBuff += costumeBuff * 5.0;//3000층 이하에서 크르르 데미지500%
            }
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_38)) {
            if (UserInfoStackManager::getInstance()->getTotalAttackChickBuffTime() > 0) {
                costumeBuff += costumeBuff * 8.0f;//800% 증가
            }
        }
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_26)) {
            double per = 0.0;
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS);
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS);
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS);
            per /= 275;  //200초당 1%
            if ( per > 1500 )
                per = 1500;
            else if ( per < 0 )
                per = 0;
            
            per /= 100;
            costumeBuff += costumeBuff * per;
        }
        
        //모험 유물 적용
        double advtRelicBuff = 0.f;
        advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CHICK_ATK);
        costumeBuff += costumeBuff * (advtRelicBuff * 0.01);
        
    }
    
    //전설
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::HEAD, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HEAD));
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::FOOT, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::FOOT));
        if (per > 0) {
            int legendEquipCount = WeaponManager::getInstance()->getLegendExistCount();
            if (legendEquipCount < 0) {
                legendEquipCount = 0;
            }
            per = legendEquipCount * per;
            costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::JEWEL, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::JEWEL));
        if (per > 0) {
            int stoneCount = StoneManager::getInstance()->getCountAll();
            if (stoneCount < 0) {
                stoneCount = 0;
            }
            per = stoneCount * per;
            costumeBuff += costumeBuff * per;
        }
    }
    
    if (CostumeManager::getInstance()->getLegendCount() >= 3) {
        costumeBuff += costumeBuff * DataManager::GetCostumeLegendSetEffect(3);
    }
    
    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_16)){
        double per = getHighFloor(ePlace) * 0.01;
        if(per > 0)
            costumeBuff += costumeBuff * per;   //1%씩증가
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_25)){
        double per = UserInfoStackManager::getInstance()->getSetItemEffectIdx25Count() * 0.1;
        if(per > 0)
            costumeBuff += costumeBuff * per;   //10%씩증가
    }
    
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_29) )
    {
        double per = getRevival(ePlace) * 0.01;
        if ( per > 10 )
            per = 10;
        
        if ( per > 0 )
            costumeBuff += costumeBuff * per;   //1%씩증가
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_24)){
        double per = WeaponManager::getInstance()->getEquipHigh()* 0.1;
        if(per > 0)
            costumeBuff += costumeBuff * per;   //10%씩증가
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_36)){
        int count = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_VIP_DAY);
        if(count>0){
            if(count>100)
                count = 100;
            double per = count *0.1;
            costumeBuff += costumeBuff * per;   //10%씩증가
        }
    }
    
    int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::HANDSOME_8);
    double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HANDSOME_8, evolutionLevel);
    if (evolutionPer > 0) {
        costumeBuff *= evolutionPer;
    }
    
    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    // skin
    double skinBuff = 1.0f;
    skinBuff += CostumeSkinManager::getInstance()->getEffectBuff();
    skinBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skinBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    double skinOwned = 1.0f;
    skinOwned += CostumeSkinManager::getInstance()->getEffectOwned();
    skinOwned *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skinOwned));
    dmg.pop_back();
    dmg.pop_back();
    
    double skillFrostCri = 100.f;
    skillFrostCri += PrisonFrostManager::getInstance()->getSkillEffect(50001, PrisonFrostManager::getInstance()->getSkillLevel(50001));
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skillFrostCri));
    dmg.pop_back();
    dmg.pop_back();
    
    if ( bOffline == false )
    {
        double skillFrostBuff = 100.f;
        skillFrostBuff += PrisonFrostManager::getInstance()->getSkillBuffEffect(50004, PrisonFrostManager::getInstance()->getSkillLevel(50004));
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skillFrostBuff));
        dmg.pop_back();
        dmg.pop_back();
    }
    
    //badge dmg
    double badgeBuff = 1.0f;
    badgeBuff += BadgeManager::getInstance()->getBadgeBuffCount(E_BADGE_BUFF::BADGE_ATK);
    badgeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(badgeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //Loots dmg
    double LootsBuff = 1.0f;
    LootsBuff += LootsManager::getInstance()->getEffect();
    LootsBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(LootsBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //raid dmg
    double raidAccrueBuff = 100.f;
    raidAccrueBuff += NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE);
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(raidAccrueBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //모험 유물 적용
    double advtRelicBuff = 100.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicEffectBuffCalculator(E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_ATK);
    
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(advtRelicBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //
    double advtRelicCollectBuff = 100.f;
    advtRelicCollectBuff += AdventureRelicManager::getInstance()->getRelicCollectEffect(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK);
    
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(advtRelicCollectBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //모험단 파티 어빌리티 공격력 증가 적용
    double advtPartyAbilityBuff = 100.f;
    advtPartyAbilityBuff += AdventureManager::getInstance()->getPartyAbilityDamage();
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(advtPartyAbilityBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //raid dmg
    double flashmanBuff = 100.f;
    flashmanBuff += FlashmanManager::getInstance()->getScoreEffect();
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(flashmanBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //정령 적용
    dmg = WeaponSpiritManager::getInstance()->getSpiritCalculation(dmg, WeaponSpiritManager::E_EFFECT_TYPE::DMG);
    
    //정령 버프
    if(WeaponSpiritManager::getInstance()->getBuffState() == WeaponSpiritManager::E_SPIRIT_BUFF_STATE::DURATION_TIME)
    {
        auto objBuff = WeaponSpiritManager::getInstance()->getInfoWeaponSpiritBuff();
        
        if(objBuff != nullptr)
        {
            if(objBuff->getType() == E_SPIRIT_BUFF_TYPE::DAMAGE)
            {
                double spiritBuff = 1.0f;
                
                spiritBuff *= objBuff->getIncreases();
                spiritBuff *= 100;
                
                dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(spiritBuff));
                dmg.pop_back();
                dmg.pop_back();
                
            }
        }
        
    }
    
    //디펜스 공격력 적용
    dmg = DefenseManager::getInstance()->getAttackCalculation(dmg);
    //마나 증폭기 적용
    dmg = DefenseManaAMPManager::getInstance()->calculatorAMPPower(dmg);
    
    
    return dmg;
}

std::string UserInfoManager::getAttackDamagePrison(E_CHARACTER eCharac)
{
    E_PLACE ePlace = E_PLACE::DG_PRISON;
    
    int weaponEquip = E_WEAPON::WEAPON_1;
    int weaponLevel = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_32_PRISON_WEAPON);
    
    //기본 데미지
    std::string dmg = WeaponManager::getInstance()->getAttack(weaponEquip, weaponLevel, getHonorLevel(E_HONOR::HONOR_1));
    
    //힘반
    auto addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_1_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_1_EVOL1));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_2)){
            addPer += addPer*0.5;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_12)){
            addPer += addPer*1.5;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_24)){
            if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1) >= DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_12_CROSS_EVOL1))
                addPer += addPer*1.2;
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //진화힘반
    addPer = DataManager::GetArtifactEffect(ARTI_13_EVOL2, getArtifactLevel(ePlace, ARTI_13_EVOL2), getArtifactReinforce(ePlace, ARTI_13_EVOL2));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_20)){
            addPer += addPer*1.0;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_24)){
            if (isFloorCheck(3001, true)) {
                addPer += addPer*2.5;
            }
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //벼려진힘반
    addPer = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_18_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_18_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_18_EVOL3));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_24)){
            addPer += addPer*0.7;
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //초월의힘반
    addPer = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_23_EVOL4, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_23_EVOL4), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_23_EVOL4));
    if(addPer>0){
        //초월의 힘반 증가
        double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_8, getRelicLevel(ePlace, E_RELIC::RELIC_8)) / 100.0f;
        if (relicPer > 0) {
            addPer += addPer * relicPer;
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    
    //5차부터 공식같고, 예외없음.
    for(int i=E_ARTIFACT::ARTI_28_EVOL5; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++){
        E_ARTIFACT arti =(E_ARTIFACT)i;
        if(DataManager::GetArtifactGeneration(arti) >=5 && DataManager::GetArtifactType(arti)==E_ARTIFACT_TYPE::ARTI_TYPE_ATK)
        {
            addPer = DataManager::GetArtifactEffect(arti, getArtifactLevel(ePlace, arti), getArtifactReinforce(ePlace, arti));
            if(addPer>0){
                auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
                dmg =MafUtils::bigAddNum(dmg, tt);
            }
        }
    }
    
    //무한(초월) 보물 반지
    dmg = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(dmg, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
    
    //십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_12_CROSS_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_21)){
            addPer += addPer*2.0;
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //진화 십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_17_CROSS_EVOL2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_17_CROSS_EVOL2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_17_CROSS_EVOL2));
    if(addPer>0){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_24)){
            addPer += addPer*2.5;
        }
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    
    //벼려진 십자가
    addPer= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_22_CROSS_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_22_CROSS_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_22_CROSS_EVOL3));
    if(addPer>0){
        auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
        dmg =MafUtils::bigAddNum(dmg, tt);
    }
    //4차부터 공식같고, 예외없음.
    for(int i=E_ARTIFACT::ARTI_27_CROSS_EVOL4; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++){
        E_ARTIFACT arti =(E_ARTIFACT)i;
        if(DataManager::GetArtifactGeneration(arti) >=4 && DataManager::GetArtifactType(arti)==E_ARTIFACT_TYPE::ARTI_TYPE_CROSS)
        {
            addPer = DataManager::GetArtifactEffect(arti, getArtifactLevel(ePlace, arti), getArtifactReinforce(ePlace, arti));
            if(addPer>0){
                auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, addPer);
                dmg =MafUtils::bigAddNum(dmg, tt);
            }
        }
    }
    
    //무한(초월) 보물 십자가
    dmg = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(dmg, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
    
    //강화석(공격력)
    double stoneBuff = 1.0f;
    stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::ATTACK);
    stoneBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    double stoneForceBuff = 1.0f;
    stoneForceBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_ATTACK);
    stoneForceBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneForceBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    double stoneForce2Buff = 1.0f;
    stoneForce2Buff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_ATTACK);
    stoneForce2Buff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(stoneForce2Buff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(무기)
    double dexWeaponBuff = 1.0f;
    dexWeaponBuff += WeaponManager::getInstance()->getDexEffect();
    dexWeaponBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(dexWeaponBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(무기)
    double weaponReinforceBuff = 1.0f;
    weaponReinforceBuff += WeaponManager::getInstance()->getReinforceEffect();
    weaponReinforceBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(weaponReinforceBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //도감 추가 공격력(몬스터)
    double dexMonsterBuff = 1.0f;
    dexMonsterBuff += MonsterManager::getInstance()->getDexEffect();
    dexMonsterBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(dexMonsterBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //펫 장비 수집 효과
    double petEquipper = PetNewManager::getInstance()->getPetEquipCollectEffect();
    if ( petEquipper > 0 )
    {
        std::string petequipper = MafUtils::doubleToString(petEquipper);
        dmg = MafUtils::bigMulNum(dmg, petequipper);
    }
    
    //봉식타워(봉식타워:일반던전, 지하감옥 공격력)
    int buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
        buildinglevel += 7;
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, getRelicLevel(ePlace, E_RELIC::RELIC_9));
    if (relicPer > 0) {
        buildinglevel += relicPer;
    }
    
    //봉식타워(누리봉 위성)
    buildinglevel += TownManager::getInstance()->getTownLevel(E_TOWN::SATELLITE);
    
    double buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_1, buildinglevel);
    if(buildingPer>0){
        auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(buildingPer));
        tt.pop_back();
        tt.pop_back();
        dmg =MafUtils::bigAddNum(dmg, tt);
        
        double costumeBuff = 1;
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_28)){
            double per = PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2) * 0.15f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        costumeBuff *= 100;
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
        dmg.pop_back();
        dmg.pop_back();
    }
    
    //코스튬 버프
    double costumeBuff = 1;
    
    // pass
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    
    //
    if ( getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS) > 0 )
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_2) )
            costumeBuff *= 10;
        else
            costumeBuff *= C_ATTACK;
        
        if ( objPass != nullptr && objPass->isBuyPass() == true )
        {
            time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( nDiffTime > 0 )
            {
                costumeBuff *= 2;
            }
        }
    }
    
    if ( objPass != nullptr && objPass->isBuyPass() == true )
    {
        time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
        if ( nDiffTime > 0 )
        {
            costumeBuff += costumeBuff * 1.5;   //150%증가
        }
    }
    
    
    // 뉴비 지원 버프
    if ( UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5 )
    {
        E_NEWBIE_STEP eNewbie = UserInfoConfigManager::getInstance()->getNewbieStep();
        
        double per = 0.0;
        if ( eNewbie == E_NEWBIE_STEP::STEP_1 )
        {
            per = 5.0;
            per *= std::pow(2.0, MIN(20, getHitCount(ePlace) / 100));
        }
        else if ( eNewbie == E_NEWBIE_STEP::STEP_2 )
        {
            per = 4.0;
            per *= std::pow(2.0, MIN(19, getHitCount(ePlace) / 100));
        }
        else if ( eNewbie == E_NEWBIE_STEP::STEP_3 )
        {
            per = 3.0;
            per *= std::pow(2.0, MIN(18, getHitCount(ePlace) / 100));
        }
        else if ( eNewbie == E_NEWBIE_STEP::STEP_4 )
        {
            per = 2.0;
            per *= std::pow(2.0, MIN(17, getHitCount(ePlace) / 100));
        }
        else if ( eNewbie == E_NEWBIE_STEP::STEP_5 )
        {
            per = 1.0;
            per *= std::pow(2.0, MIN(16, getHitCount(ePlace) / 100));
        }
        
        if ( per > 0.0 )
        {
            costumeBuff += costumeBuff * per;
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_4)){
        costumeBuff += costumeBuff * 1.0;   //100%증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_6)) {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY) > 0) {
            double per = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY) * 0.05;
            costumeBuff += costumeBuff * per;       //3%씩 증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_13)) {
        double petBuff = 0;
        if(PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1) >= 20)
            petBuff += 0.3f;
        for (int i=E_PET::HAMSTER_2; i<=C_COUNT_PET; i++) {
            if(PetNewManager::getInstance()->getPetLevel((E_PET)i) >= PetNewManager::getInstance()->getPetLevelMax((E_PET)i))
                petBuff += 0.3f;
        }
        costumeBuff += costumeBuff * petBuff;
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_16) )
    {
        if (getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS) > 0 && getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS) > 0 && getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS) > 0)
            costumeBuff += costumeBuff * 3;   //300%증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = ItemsMoneyManager::getInstance()->getKey().length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getKeyLength();
        }
        
        double keyBuff = nLength * 0.2f;
        if (keyBuff > 0)
            costumeBuff += costumeBuff * keyBuff;   //20%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem()).length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getGemLength();
        }
        
        double gemBuff = nLength * 0.5f;
        if (gemBuff > 0)
            costumeBuff += costumeBuff * gemBuff;   //50%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_18)) {
        double per = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD) * 0.13;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //13%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_19)) {
        costumeBuff += costumeBuff;       //100% 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_20)) {
        double per = StoneManager::getInstance()->getCountAll() * 0.01f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //1%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_20)) {
        double per = getArtifactExistCount(ePlace) * 0.15f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_23)) {
        if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_CRITICAL || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
            costumeBuff *= 8;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_24)) {
        double per = getRelicLevel(ePlace, E_RELIC::RELIC_1) * 0.15f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_25)) {
        double per = SpecialMissionManager::getInstance()->getSpecialMissionClear() * 0.04f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //4%씩 증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_33)){
        if (getHitCount(DG_PRISON) > 1)
            costumeBuff += costumeBuff * 3.0f;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_37)){
        costumeBuff += costumeBuff * 2.5;   //250%증가
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_39)){
        costumeBuff += costumeBuff * 4.86;   //486%증가
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_4)){
        double per = CostumeManager::getInstance()->getExistAuraCount() * 0.4;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_12)){
        double per = getHighFloor(ePlace) / 1000 * 1.0f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_17)) {
        int petReinforceCount = 0;
        for (int i=1; i<=C_COUNT_PET; i++) {
            petReinforceCount += PetNewManager::getInstance()->getPetReinforce((E_PET)i);
        }
        double per = petReinforceCount * 0.3;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_18)) {
        double per = CostumeManager::getInstance()->getExistCount() * 0.03;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_35)) {
        int count = getArtifactMaxLevelCount(ePlace);
        double per = count * 0.25f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_31))
    {
        int64_t nLength = MafUtils::toString(NewRaidManager::getInstance()->getTotalAbilityPoint()).length();
        
        double per = nLength;
        if (per > 0)
            costumeBuff += costumeBuff * per;   //100%씩 증가
    }
    
    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    
    int killcount = PrisonManager::getInstance()->getDungeonKill();
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_7))
    {
        if (getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS) > 0) {
            costumeBuff += costumeBuff * 1.5;   //150%증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_8)) {
        costumeBuff += costumeBuff * 3.0;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_8)) {
        if (PrisonManager::getInstance()->getDungeonCount() > 0) {
            double per = PrisonManager::getInstance()->getDungeonCount() * 0.03;
            costumeBuff += costumeBuff * per;   //2%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_8)) {
        if (killcount >= 100) {
            double per = (killcount/100) * 0.12;
            costumeBuff += costumeBuff * per;   //12%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_11)) {
        if (killcount >= 100) {
            double per = (killcount/100) * 0.15;
            costumeBuff += costumeBuff * per;   //15%씩증가
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_13)) {
        if (isMeterCheck(1000))
            costumeBuff += costumeBuff * 1.5;   //100%씩증가
    }
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_13)) {
        if (isMeterCheck(700))
            costumeBuff += costumeBuff * 2.5;   //250%씩증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_14)) {
            costumeBuff += costumeBuff * 2.0;   //200%씩증가
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_19)) {
        int timeNow = PrisonManager::getInstance()->getDungeonTimeDefault() - PrisonManager::getInstance()->getDungeonTimeNow();
        double per = timeNow * 0.20f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_30)){
        double per = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_2) * 0.01f;
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_33)){
        double per = getRelicLevel(ePlace, E_RELIC::RELIC_2) * 0.20f; //20%씩 증가
        if (per > 0)
            costumeBuff += costumeBuff * per;
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_34)){
        if (isMeterCheck(5000))
            costumeBuff += costumeBuff * 3.0;   //5000m 이하에서 300%
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_9)){
        int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::FOOT);
        double per = count * 0.15f;
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }

    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    if (eCharac == E_CHARACTER::CHA_HERO) {
        if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ATTACK || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
        {
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_9)) {
                costumeBuff *= 20;  //20배 증가
            }else{
                costumeBuff *= C_BUFF_ATTACK; //공격력8배
            }
            double per = DataManager::GetRelicEffect(E_RELIC::RELIC_3, getRelicLevel(ePlace, E_RELIC::RELIC_3)) / 100.0f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
            int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::PRINCESS_4);
            double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::PRINCESS_4, reinforceLevel);
            if (reinforcePer > 0) {
                costumeBuff *= reinforcePer;
            }
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_14)) {
            if (UserInfoStackManager::getInstance()->getHitBuff() > 0) {
                double per = UserInfoStackManager::getInstance()->getHitBuff() * 0.2;
                costumeBuff += costumeBuff * per;   //20%씩증가
            }
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_20)) {
            double per = getArtifactExistCount(ePlace) * 0.40f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;       //40%씩 증가
            }
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_37)) {
            costumeBuff += costumeBuff * 3.0;   //300%
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = 0.0;
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HEAD);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::BODY);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::CAPE);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HAND);
            per += CostumeManager::getInstance()->getLegendLevel(E_COSTUME::FOOT);
            if (per > 0) {
                per = std::pow(per, 4.5);
                per /= 100;
                costumeBuff += costumeBuff * per;
            }
        }
        
        //펫 장비 수집 효과(봉식)
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = PetNewManager::getInstance()->getPetEquipCollectEffectBongsik();
            if(per > 0){
                costumeBuff *= per;
            }
        }
    }
    else{
        int level = PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
        double per = PetNewManager::getInstance()->getPetEffect(E_PET::CHICK_1, level);
        if(per!=1)
            costumeBuff *= per;
        
        int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::CHICK_1);
        double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::CHICK_1, reinforceLevel);
        if (reinforcePer > 0) {
            costumeBuff *= reinforcePer;
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_7)) {
            if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ATTACK || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
            {
                if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_9)) {
                    costumeBuff *= 20;
                }else{
                    costumeBuff *= C_BUFF_ATTACK;
                }
                double per = DataManager::GetRelicEffect(E_RELIC::RELIC_3, getRelicLevel(ePlace, E_RELIC::RELIC_3)) / 100.0f;
                if (per > 0) {
                    costumeBuff += costumeBuff * per;
                }
                
                int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::PRINCESS_4);
                double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::PRINCESS_4, reinforceLevel);
                if (reinforcePer > 0) {
                    costumeBuff *= reinforcePer;
                }
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_18)) {
            double per = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::CAPE) * 0.20;
            if (per > 0) {
                costumeBuff += costumeBuff * per;       //20%씩 증가
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_26)) {
            if (PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1) >= 21) {
                double per = (PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1)-20) * 0.30;
                if (per > 0) {
                    costumeBuff += costumeBuff * per;       //30%씩 증가
                }
            }
        }
        
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_32)){
            double per = getAttendVip() * 0.50f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_9)) {
            double per = DataManager::GetRelicEffect(E_RELIC::RELIC_2, getRelicLevel(ePlace, E_RELIC::RELIC_2)) / 100.0f;
            if (per > 0) {
                costumeBuff += costumeBuff * per;
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_9)) {
            costumeBuff += costumeBuff * 2.5;   //250% 증가
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_26)) {
            int killcount = PrisonManager::getInstance()->getDungeonKill();
            if (killcount >= 2000)
                costumeBuff += costumeBuff * 4.0;   //400%씩증가
        }
        if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_26)) {
            double per = 0.0;
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS);
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS);
            per += UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS);
            per /= 275;  //200초당 1%
            if ( per > 1500 )
                per = 1500;
            else if ( per < 0 )
                per = 0;
            
            per /= 100;
            costumeBuff += costumeBuff * per;
        }
    }
    
    //전설
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::HEAD, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HEAD));
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::FOOT, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::FOOT));
        if (per > 0) {
            int legendEquipCount = WeaponManager::getInstance()->getLegendExistCount();
            if (legendEquipCount < 0) {
                legendEquipCount = 0;
            }
            per = legendEquipCount * per;
            costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::JEWEL, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::JEWEL));
        if (per > 0) {
            int stoneCount = StoneManager::getInstance()->getCountAll();
            if (stoneCount < 0) {
                stoneCount = 0;
            }
            per = stoneCount * per;
            costumeBuff += costumeBuff * per;
        }
    }
    
    if (CostumeManager::getInstance()->getLegendCount() >= 3) {
        costumeBuff += costumeBuff * DataManager::GetCostumeLegendSetEffect(3);
    }
    
    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    costumeBuff = 1.0f;
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_25)){
        double per = UserInfoStackManager::getInstance()->getSetItemEffectIdx25Count() * 0.1;
        if(per > 0)
            costumeBuff += costumeBuff * per;   //10%씩증가
    }
    
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_29) )
    {
        double per = getRevival(ePlace) * 0.01;
        if ( per > 10 )
            per = 10;
        
        if ( per > 0 )
            costumeBuff += costumeBuff * per;   //1%씩증가
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_24)){
        double per = WeaponManager::getInstance()->getEquipHigh()* 0.1;
        if(per > 0)
            costumeBuff += costumeBuff * per;   //10%씩증가
    }
    
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_36)){
        int count = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_VIP_DAY);
        if(count>0){
            if(count>100)
                count = 100;
            double per = count *0.1;
            costumeBuff += costumeBuff * per;   //10%씩증가
        }
    }
    
    int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::HANDSOME_8);
    double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HANDSOME_8, evolutionLevel);
    if (evolutionPer > 0) {
        costumeBuff *= evolutionPer;
    }
    
    costumeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(costumeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    // skin
    float skinBuff = 1.0f;
    skinBuff += CostumeSkinManager::getInstance()->getEffectBuff();
    skinBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skinBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    float skinOwned = 1.0f;
    skinOwned += CostumeSkinManager::getInstance()->getEffectOwned();
    skinOwned *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(skinOwned));
    dmg.pop_back();
    dmg.pop_back();
    
    //badge dmg
    float badgeBuff = 1.0f;
    badgeBuff += BadgeManager::getInstance()->getBadgeBuffCount(E_BADGE_BUFF::BADGE_ATK);
    badgeBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(badgeBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //Loots dmg
    float LootsBuff = 1.0f;
    LootsBuff += LootsManager::getInstance()->getEffect();
    LootsBuff *= 100;
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(LootsBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    //raid dmg
    float raidAccrueBuff = 100.f;
    raidAccrueBuff += NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE);
    dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(raidAccrueBuff));
    dmg.pop_back();
    dmg.pop_back();
    
    return dmg;

}
std::string UserInfoManager::getAttackDamageDevil()
{
    E_PLACE ePlace = E_PLACE::DG_DEVIL_DOM;
    
    auto type = E_EQUIP_TYPE::E_D_WEAPON;
    auto idx = getEquipDevil(type);
    auto level = getEquipLevelDevil(type);
    auto sum = DataManager::GetEquipEffectRateDevil(type, idx, level);
    auto reinforce = getEquipReinforceLevelDevil(type);
    auto reinPer = DataManager::GetEquipReinforceRateDevil(reinforce);
    auto evol = DataManager::GetEquipEvolutionRateDevil(idx);
    int optionPer = DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_WEAPON, idx, level, E_EQUIP_OPTION::E_D_DMG);
    
    auto vec = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK);
    double total = 1.0;
    for(auto info : vec){
        if(isArtifactHaveDevil(info->_idx)){
            if(info->_idx==0)
                continue;
            total += total*info->_effect*0.01;
        }
    }
    
    double bonus = 0;
    auto high = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL); // 일반 던전 최대층 보상
    high -= 10000;
    if(high>0){
        bonus = high/100*10;
    }
    
    double dmg = 10 + sum;
    if(bonus>0){
        dmg += dmg*bonus*0.01;
    }
    if(getBuffTime(E_BUFF_TYPE::BUFF_D_ATK_DMG)>0){
        dmg *= 3;
    }
    
    if(evol>1)
        dmg *= evol;
    if(reinPer>0)
        dmg *= reinPer;
//    if(per>0)
//        dmg += dmg*per*0.01;
    if(optionPer>0)
        dmg += dmg*optionPer*0.01;
    
    if(total>1.0){
        dmg *= total;
//        total*=100;
//        auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(total));
//        tt.pop_back();
//        tt.pop_back();
//        dmg = MafUtils::bigAddNum(dmg, tt);
    }
    
    // 문지개 공격 버프(200%증가)
    if ( getPetBuffGatekeeper(ePlace) == E_GATEKEEPER_BUFF::BUFF_ATTACK_GATEKEEPER )
    {
        dmg += dmg * 2;
    }
    
    
    std::string strDmg = MafUtils::doubleToString(dmg);
    return strDmg;
}

std::string UserInfoManager::getAttackCriDamage(E_PLACE ePlace, std::string &tmpAtk)
{//cri 분리
    auto dmg = tmpAtk;
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        auto per = getEquipTotalEffectPercentDevil(E_EQUIP_TYPE::E_D_CAPE);
        if(per>0){
            auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(per));
            tt.pop_back();
            tt.pop_back();
            dmg = MafUtils::bigAddNum(dmg, tt);
        }
        
        auto idx = getEquipDevil(E_EQUIP_TYPE::E_D_CAPE);
        auto level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_CAPE);
        auto per2 = (double)DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_CAPE, idx, level, E_EQUIP_OPTION::E_D_CRI_DMG);
        if(per2>0){
            auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(per2));
            tt.pop_back();
            tt.pop_back();
            dmg = MafUtils::bigAddNum(dmg, tt);
        }
        
        auto vec = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_2_CRI);
        double total = 1.0;
        for(auto info : vec){
            if(isArtifactHaveDevil(info->_idx)){
                total += total*info->_effect*0.01;
            }
        }
        total-=1;
        if(total>1.0){
            total*=100;
            auto tt = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(total));
            tt.pop_back();
            tt.pop_back();
            dmg = MafUtils::bigAddNum(dmg, tt);
        }
    }
    else
    {
        //칼날
        auto per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_3_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_3_EVOL1));
        if (per > 0) {
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_2)){
                per *= 2;   //100% 상승
            }
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_12)) {
                double keyBuff = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY) * 0.05f;
                if (keyBuff > 0)
                    per += per * keyBuff;
            }
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_14)){
                per += per * 1.5;   //150% 상승
            }
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_17)){
                double keyBuff = getArtifactLevel(ePlace, E_ARTIFACT::ARTI_11_KEY) * 0.06f;
                if (keyBuff > 0)
                    per += per * keyBuff;
            }
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_22)){
                if ( ePlace == E_PLACE::DG_NORMAL )
                {
                    int64_t nLength = ItemsMoneyManager::getInstance()->getGold().length();
                    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23) )
                    {
                        nLength = ItemsMoneyManager::getInstance()->getGoldLength();
                    }
                    
                    double goldBuff = nLength * 0.06f;
                    if (goldBuff > 0)
                        per += per * goldBuff;   //6%씩 증가
                }
            }
            
            if ( ePlace == E_PLACE::DG_NORMAL )
            {
                per = per * PetNewManager::getInstance()->getPetEquipEffect(800, 1);
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, per);
            dmg = MafUtils::bigAddNum(dmg,tt);
        }
        //진화된 칼날
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_14_EVOL2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_14_EVOL2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_14_EVOL2));
        if (per > 0) {
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_20)){
                per += per*1.0;
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, per);
            dmg = MafUtils::bigAddNum(dmg,tt);
        }
        //벼려진 칼날
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_19_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_19_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_19_EVOL3));
        if (per > 0) {
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_24)){
                per += per*0.7;
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, per);
            dmg = MafUtils::bigAddNum(dmg,tt);
        }
        //초월의 칼날
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_24_EVOL4, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_24_EVOL4), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_24_EVOL4));
        if (per > 0) {
            double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_8, getRelicLevel(ePlace, E_RELIC::RELIC_8)) / 100.0f;
            if (relicPer > 0) {
                per += per * relicPer;
            }
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, per);
            dmg = MafUtils::bigAddNum(dmg,tt);
        }
        
        //5차부터 공식같고, 예외없음.
        for(int i=E_ARTIFACT::ARTI_29_EVOL5; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++){
            E_ARTIFACT arti =(E_ARTIFACT)i;
            if(DataManager::GetArtifactGeneration(arti) >=5 && DataManager::GetArtifactType(arti)==E_ARTIFACT_TYPE::ARTI_TYPE_CRI)
            {
                per = DataManager::GetArtifactEffect(arti, getArtifactLevel(ePlace, arti), getArtifactReinforce(ePlace, arti));
                if (per > 0) {
                    auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(dmg, per);
                    dmg = MafUtils::bigAddNum(dmg,tt);
                }
            }
        }
        
        //무한(초월) 칼날
        dmg = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(dmg, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
        
        double criCostumeBuff = 1;
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_36)){
            if (getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS)>0)
                criCostumeBuff += criCostumeBuff * 2.0;   //200%증가
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_36)){
            if ( ePlace == E_PLACE::DG_PRISON )
                criCostumeBuff += criCostumeBuff * 3.0;   //300%증가
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_37)){
            criCostumeBuff += criCostumeBuff * 2.5;   //250%증가
        }
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_20)){
            criCostumeBuff += criCostumeBuff * 3.0;   //300%증가
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_8)){
            int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HAND);
            double per = count * 0.07f;
            if (per > 0)
                criCostumeBuff += criCostumeBuff * per;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = DataManager::GetCostumeLegendEffect(E_COSTUME::CAPE, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::CAPE));
            if (per > 0)
                criCostumeBuff += criCostumeBuff * per;
        }
        if (CostumeManager::getInstance()->getLegendCount() >= 2) {
            criCostumeBuff += criCostumeBuff * DataManager::GetCostumeLegendSetEffect(2);
        }
        
        criCostumeBuff *= 100;
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(criCostumeBuff));
        dmg.pop_back();
        dmg.pop_back();
        
        //모험 유물 적용
        double advtRelicBuff = 100.f;
        advtRelicBuff += AdventureRelicManager::getInstance()->getRelicEffectBuffCalculator(E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_CRI);
        
        dmg = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(advtRelicBuff));
        dmg.pop_back();
        dmg.pop_back();
    }
    
    return dmg;
}

#pragma mark - battle : 크리티컬, 공격속도, 이동속도
bool UserInfoManager::getCriticalRate(E_PLACE ePlace, bool bBongsik, bool bOffline)
{
    int nRate = getCriticalRatePercent(ePlace, bBongsik, bOffline);
    if ( nRate >= 100 )
        return true;
    
    if( nRate >= random(1, 100) )
        return true;
    else
        return false;
}

int UserInfoManager::getCriticalRatePercent(E_PLACE ePlace, bool bBongsik, bool bOffline)
{
    //
    std::lock_guard<std::mutex> lock(_mutexCritical);
    
    //
    int cri = 5;
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        //옵션중 무기와 망토에 크리 확률이 두군데 붙어있다.
        auto idx = getEquipDevil(E_EQUIP_TYPE::E_D_WEAPON);
        auto level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_WEAPON);
        cri += DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_WEAPON, idx, level, E_EQUIP_OPTION::E_D_CRITICAL);
        
        idx = getEquipDevil(E_EQUIP_TYPE::E_D_CAPE);
        level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_CAPE);
        cri += DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_CAPE, idx, level, E_EQUIP_OPTION::E_D_CRITICAL);
        
        // 문지개 크리확률 버프
        if ( getPetBuffGatekeeper(ePlace) == E_GATEKEEPER_BUFF::BUFF_CRITICAL_GATEKEEPER )
        {
            cri += 30;
        }
    }
    else
    {
        cri= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_2));
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_2) )            cri+=25;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_22) )           cri+=25;
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_37) )        cri-=20;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_37) )           cri-=15;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_37) )           cri-=15;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_37) )           cri+=10;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_37) )           cri+=10;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_37) )           cri+=10;
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_20) )        cri+=20;
        
        if ( bOffline == false )
        {
            if ( bBongsik )
            {
                if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_CRITICAL || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
                    cri += C_BUFF_CRI;
            }
            else
            {
                if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_7) )
                {
                    if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_CRITICAL || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
                        cri += C_BUFF_CRI;
                }
            }
        }
        
        
        if (cri > 100)      cri = 100;
        else if(cri < 0)    cri = 0;
    }
    
    return cri;
}

/**
 기존 이동 속도 공격속도 계산 방법이 잘못되어 있음 2022년 3월 7일 이후 신규 기능들은 정상적으로 개발이 되야 됨.
 15% 증가 : speed *= 1 / (15 / 100 + 1.0f)
 20% 증가 : speed *= 1 / (20 / 100 + 1.0f)
 */
double UserInfoManager::getAttackSpeed(E_PLACE ePlace)
{
    //
    std::lock_guard<std::mutex> lock(_mutexAttackSpeed);
    
    double nSpeed = 0.5;
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nSpeed = 0.5;
        
        auto vec = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD);
        for(auto info : vec)
        {
            if ( isArtifactHaveDevil(info->_idx) )
            {
                nSpeed -= nSpeed*info->_effect*0.01;
            }
        }
        
        if ( getBuffTime(E_BUFF_TYPE::BUFF_D_ATK_SPD) > 0 )
        {
            nSpeed *= 0.5;
        }
        
        // 문지개 공속 버프(20%증가)
        if ( getPetBuffGatekeeper(E_PLACE::DG_DEVIL_DOM) == E_GATEKEEPER_BUFF::BUFF_ATTACKSPPED_GATEKEEPER )
        {
            nSpeed *= 0.8;
        }
    }
    else
    {
        nSpeed = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_7, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_7), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_7));
        nSpeed -= DataManager::GetRelicEffect(E_RELIC::RELIC_1, getRelicLevel(ePlace, E_RELIC::RELIC_1));
        
        if ( ePlace == E_PLACE::DG_NORMAL )
        {
            
        }
        else if ( ePlace == E_PLACE::DG_PRISON )
        {
            // 이용시간 30초 > 15초로 변경에 따른 이동속도 증가
            nSpeed *= 0.5f;
        }
    }
    
    return nSpeed;
}

/**
 기존 이동 속도 공격속도 계산 방법이 잘못되어 있음 2022년 3월 7일 이후 신규 기능들은 정상적으로 개발이 되야 됨.
 15% 증가 : speed *= 1 / (15 / 100 + 1.0f)
 20% 증가 : speed *= 1 / (20 / 100 + 1.0f)
 */
double UserInfoManager::getUserSpeed(E_PLACE ePlace, bool bOffline)
{
    //
    std::lock_guard<std::mutex> lock(_mutexSpeed);
    
    //
    double nSpeed = 0.125f;
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if ( getBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD) > 0 )
        {
            nSpeed = 0.0625f;
        }
        
        auto idx = getEquipDevil(E_EQUIP_TYPE::E_D_BODY);
        auto level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_BODY);
        double per = (double)DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_BODY, idx, level, E_EQUIP_OPTION::E_D_SPEED);
        nSpeed*= (1-per*0.01);
        
        auto vec = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_5_MOVE_SPD);
        for(auto info : vec)
        {
            if ( isArtifactHaveDevil(info->_idx) )
            {
                nSpeed *= 1-info->_effect*0.01;
            }
        }
        
        // 문지개 이속 버프(30%증가)
        if ( getPetBuffGatekeeper(ePlace) == E_GATEKEEPER_BUFF::BUFF_MOVESPPED_GATEKEEPER )
        {
            nSpeed *= 0.7;
        }
    }
    else if ( ePlace == E_PLACE::DG_NORMAL || ePlace == E_PLACE::DG_PRISON)
    {
        if ( bOffline == false && getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS) > 0 )
        {
            nSpeed = 0.0625f;
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_10))
                nSpeed *= 0.85f;
        }
        
        // 뉴비 지원 버프
        if ( bOffline == false )
        {
            if ( UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5 )
            {
                E_NEWBIE_STEP eNewbie = UserInfoConfigManager::getInstance()->getNewbieStep();
                
                double per = 0.0;
                if ( eNewbie == E_NEWBIE_STEP::STEP_1 )
                    per = 0.5;
                else if ( eNewbie == E_NEWBIE_STEP::STEP_2 )
                    per = 0.6;
                else if ( eNewbie == E_NEWBIE_STEP::STEP_3 )
                    per = 0.7;
                else if ( eNewbie == E_NEWBIE_STEP::STEP_4 )
                    per = 0.8;
                else if ( eNewbie == E_NEWBIE_STEP::STEP_5 )
                    per = 0.9;
                
                if ( per > 0.0 )
                {
                    nSpeed *= per;
                }
            }
            
            if ( getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_MOVE || getPetBuffPriencess(ePlace) == E_PRINCESS_BUFF::BUFF_ALL )
                nSpeed *= C_BUFF_MOVE;
        }
        
        if ( FriendManager::getInstance()->getFriendListAmount() > 0 )
        {
            float per = 1;
            float discount = FriendManager::getInstance()->getFriendBonusSpeed() / 100;
            per = per - discount;
            
            nSpeed *= per;
        }
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_2) )
            nSpeed *= C_TIME_UP_MOVE_10;
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_15) )
            nSpeed *= C_TIME_UP_MOVE_10;
        
        if ( CostumeManager::getInstance()->getLegendCount() >= 5 )
            nSpeed *= DataManager::GetCostumeLegendSetEffect(5);
        
        if ( ePlace == E_PLACE::DG_NORMAL )
        {
            if (!bOffline && getFeverTime() > 0 )
                nSpeed *= 0.25;
            else
            {
                if(!bOffline && getItemFeverTime() > 0)
                {
                    double per = ParameterManager::getInstance()->getParameterDouble("item_fever_effect");
                    if(per != 0)
                        nSpeed *= per;
                }
            }
        }
        else if ( ePlace == E_PLACE::DG_PRISON )
        {
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_2) )
                nSpeed *= C_TIME_UP_MOVE_15; //신발 지하감옥에서 15%
            
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_20) )
                nSpeed *= C_TIME_UP_MOVE_15; //신발 지하감옥에서 10%
            
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_32) )
            {
                double per = getAttendVip() * 0.01;
                if ( 0 < per && per <= 0.1f )
                {
                    nSpeed *= (1.0f - per);   //신발 지하감옥에서 1%씩
                }
            }
            
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_23) )
            {
                if ( getOneKill(ePlace) == true )
                {
                    nSpeed *= 0.7f;   //신발 지하감옥에서 30%
                }
            }
            
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_14) )
            {
                int killcount = PrisonManager::getInstance()->getDungeonKill();
                if ( killcount > 20 && ((killcount-1) % 20 < 10) )
                {
                    nSpeed *= 0.5; //신발 지하감옥에서 2배
                }
            }
            
            // 이용시간 30초 > 15초로 변경에 따른 이동속도 증가
            nSpeed *= 0.5f;
        }
    }
    
    return nSpeed;
}

#pragma mark - quest
bool UserInfoManager::getQuest(E_PLACE ePlace, E_QUEST eQuest)
{
    bool bQuest = false;
    
    int nLevel = getQuestLevel(ePlace, eQuest);
    if ( nLevel > 0 )
    {
        bQuest = true;
    }
     
    return bQuest;
}

int UserInfoManager::getQuestLevel(E_PLACE ePlace, E_QUEST eQuest)
{
    int nLevel = 0;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        nLevel = _uQuestLevel[eQuest].valueInt();
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        nLevel = _uQuestLevelDevil[eQuest].valueInt();
    }
    
    return nLevel;
}

void UserInfoManager::setQuestLevel(E_PLACE ePlace, E_QUEST eQuest, int nLevel, bool bSave)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        _uQuestLevel[eQuest] = nLevel;
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uQuestLevelDevil[eQuest] = nLevel;
    }
    
    if ( bSave )
        SaveManager::saveQuest(ePlace);
}

double UserInfoManager::getQuestTime(E_PLACE ePlace, E_QUEST eQuest)
{
    double nTime = 0;
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM)
    {
        nTime = _uQuestTimeDevil[eQuest];
    }
    
    return nTime;
}

//퀘스트 골드 주는 곳 : 퀘스트 시간 저장
void UserInfoManager::setQuestTime(E_PLACE ePlace, E_QUEST eQuest, double nTime, bool bSave)
{
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uQuestTimeDevil[eQuest] = nTime;
    }
    
    if ( bSave )
        SaveManager::saveQuestTime(ePlace);
}

//퀘스트 골드 주는 곳 : 골드 가져오는 곳
std::string UserInfoManager::getQuestEarnGold(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    std::string strEarn = "0";
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        strEarn = getQuestEarnGoldNomal(ePlace, eQuest, nLevel);
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        strEarn = getQuestEarnGoldDevil(ePlace, eQuest, nLevel);
    }
    
    return strEarn;
}

std::string UserInfoManager::getQuestEarnGoldNomal(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    std::string strResult = "0";
    std::string perGold = getQuestGoldRate(ePlace);
    
    double costumeBuff = 100;
    if( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_3))
    {
        costumeBuff += costumeBuff * 0.5f;//50% 증가
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_13))
    {
        if( eQuest != E_QUEST::QUEST_25 )
            costumeBuff += costumeBuff * 2.0f;//200% 증가
    }
    if( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_15) )
    {
        double per = UserInfoStackManager::getInstance()->getRevivalQuestBuff() * 0.1f;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //10%씩 증가
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_13) )
    {
        if (isFloorCheck(1000, false))
            costumeBuff += costumeBuff * 0.5f;//50% 증가
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_18) )
    {
        double per = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::BODY) * 0.15;
        if (per > 0)
            costumeBuff += costumeBuff * per;       //15%씩 증가
    }
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_37) )
    {
        costumeBuff += costumeBuff * 2.0f;//200% 증가
    }
    
    // 획득 골드
    if ( nLevel > 0 )
    {
        std::string strQuestEarnMoney = DataManager::GetQuestEarnMoney(ePlace, eQuest, nLevel);
        strQuestEarnMoney = MafUtils::bigMulNum(strQuestEarnMoney, MafUtils::doubleToString(costumeBuff));
        strQuestEarnMoney.pop_back();
        strQuestEarnMoney.pop_back();
        
        strQuestEarnMoney = MafUtils::bigMulNum(strQuestEarnMoney, perGold);
        if ( strQuestEarnMoney.length() > 2 )
        {
            strQuestEarnMoney.pop_back();
            strQuestEarnMoney.pop_back();
        }
        
        strResult = strQuestEarnMoney;
    }
    
    return strResult;;
}

std::string UserInfoManager::getQuestEarnGoldDevil(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    std::string strResult = "0";
    std::string perGold = getQuestGoldRate(ePlace);
    
    if ( nLevel > 0 )
    {
        std::string strQuestEarnMoney = DataManager::GetQuestEarnMoney(ePlace, eQuest, nLevel);
        strQuestEarnMoney = MafUtils::bigMulNum(strQuestEarnMoney, perGold);
        strQuestEarnMoney.pop_back();
        strQuestEarnMoney.pop_back();
        
        strResult = strQuestEarnMoney;
    }
    
    return strResult;;
}

std::string UserInfoManager::getQuestSecondEarnGoldNomal(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    std::string strResult = "0";
    if(nLevel == 0)
        return strResult;
    
    std::string gold = getQuestEarnGold(ePlace, eQuest, nLevel);
    double timer = DataManager::GetQuestSpendTime(ePlace, (E_QUEST)eQuest);
    int perTime = getQuestTimeRate(ePlace);
    
    if(perTime<100)
    {
        timer = timer*perTime*0.01;
    }
    
    //나눗셈 역으로계산.
    if(timer>1)
    {
        std::string temp = MafUtils::toString(timer);
        size_t len = temp.length()+1;
        temp.clear();
        temp.insert(0, "1");
        for(int i=0; i<len-1; i++)
        {
            temp.append("0");
        }
        double val = utils::atof(temp.c_str());
        val /= timer;
        temp = MafUtils::toString(val);
        //std::string a = gold/(int)timer;
        val*=10; //소숫점 계산 정확성위해..
        std::string a =  MafUtils::bigMulNum(gold, MafUtils::doubleToString(val));
        a.pop_back();
        
        size_t size = MafUtils::toString(timer).length();
        for(int kk=0; kk<size; kk++)
            a.pop_back();
        
        strResult = a;
    }
    else
    {
        strResult = gold;
    }
    
    return strResult;;
}

std::string UserInfoManager::getQuestGoldRate(E_PLACE ePlace)
{
    std::string value = "100";
    
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if ( getBuffTime(E_BUFF_TYPE::BUFF_D_QUEST_GOLD ) > 0 )
        {
            value = "300"; //3qㅐ
        }
        
        auto per = getEquipTotalEffectPercentDevil(E_EQUIP_TYPE::E_D_HEAD);
        if(per>0){
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(per));
            tt.pop_back();
            tt.pop_back();
            value = MafUtils::bigAddNum(value, tt);
        }
        
        auto idx = getEquipDevil(E_EQUIP_TYPE::E_D_HEAD);
        auto level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_HEAD);
        auto per2 = DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_HEAD, idx, level, E_EQUIP_OPTION::E_D_QUEST_REWARD);
        if(per2>0){
            auto tt = MafUtils::bigMulNum(value, MafUtils::toString(per2));
            tt.pop_back();
            tt.pop_back();
            value = MafUtils::bigAddNum(value, tt);
        }
        
        auto vec = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_3_GOLD);
        double total = 1.0;
        for(auto info : vec){
            if(isArtifactHaveDevil(info->_idx)){
                total += total*info->_effect*0.01;
            }
        }
        total-=1;
        if(total>0){
            total*=100;
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(total));
            tt.pop_back();
            tt.pop_back();
            value = MafUtils::bigAddNum(value, tt);
        }
    }
    else if( ePlace == E_PLACE::DG_NORMAL )
    {
        double per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_4_EVOL1, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_4_EVOL1));
        if(per>0){
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_24)){
                if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1) >= DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_12_CROSS_EVOL1))
                {
                    per += per*1.2;
                }
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_15_EVOL2, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_15_EVOL2), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_15_EVOL2));
        if(per > 0){
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_20)){
                per += per*1.0;
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_20_EVOL3, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_20_EVOL3), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_20_EVOL3));
        if(per > 0){
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_24)){
                per += per*0.7;
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_25_EVOL4, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_25_EVOL4), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_25_EVOL4));
        if(per > 0){
            double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_8, getRelicLevel(ePlace, E_RELIC::RELIC_8)) / 100.0f;
            if (relicPer > 0) {
                per += per * relicPer;
            }
            
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_30_EVOL5, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_30_EVOL5), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_30_EVOL5));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //6차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_37_EVOL6, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_37_EVOL6), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_37_EVOL6));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //7차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_42_EVOL7, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_42_EVOL7), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_42_EVOL7));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //8차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_48_EVOL8, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_48_EVOL8), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_48_EVOL8));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //9차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_56_EVOL9, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_56_EVOL9), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_56_EVOL9));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //10차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_61_EVOL10, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_61_EVOL10), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_61_EVOL10));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //11차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_66_EVOL11, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_66_EVOL11), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_66_EVOL11));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //12차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_71_EVOL12, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_71_EVOL12), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_71_EVOL12));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //13차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_76_EVOL13, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_76_EVOL13), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_76_EVOL13));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //14차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_81_EVOL14, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_81_EVOL14), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_81_EVOL14));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        //15차 조각상
        per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_86_EVOL15, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_86_EVOL15), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_86_EVOL15));
        if(per > 0){
            auto tt = DataManager::getInstance()->GetArtifacEffecttCalculation(value, per);
            value = MafUtils::bigAddNum(value, tt);
        }
        
        //무한(초월) 조각상
        value = InfiniteArtifactManager::getInstance()->getInfiniteArtiCalculation(value, INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
        
        //모험 유물 적용
        double advtRelicBuff = 100.f;
        advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_QUEST_GOLD);
        
        value = MafUtils::bigMulNum(value, MafUtils::doubleToString(advtRelicBuff));
        value.pop_back();
        value.pop_back();

        if(getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS)>0){
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_4))
                value.append("0");
            else
                value = MafUtils::bigMulNum(value, MafUtils::toString(C_DOUBLE_COIN));
        }
        
        //Costume
        double costumeBuff = 1.0f;
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_7)){
            costumeBuff += costumeBuff * 2.0f;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_12)){
            costumeBuff += costumeBuff * 1.5f;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_16)){
            
            int64_t nLength = ItemsMoneyManager::getInstance()->getKey().length();
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
            {
                nLength = ItemsMoneyManager::getInstance()->getKeyLength();
            }
            
            double keyBuff = nLength * 0.2f;
            if (keyBuff > 0)
                costumeBuff += costumeBuff * keyBuff;   //20%씩 증가
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_17)) {
            
            int64_t nLength = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem()).length();
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
            {
                nLength = ItemsMoneyManager::getInstance()->getGemLength();
            }
            
            double gemBuff = nLength * 0.5f;
            if (gemBuff > 0)
                costumeBuff += costumeBuff * gemBuff;   //50%씩 증가
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_25)){
            if (UserInfoStackManager::getInstance()->getReviveSpecial() > 0)
                costumeBuff += costumeBuff*3.0;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_30)){
            double per = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_4) * 0.02f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_32)){
            double per = getAttendVip() * 0.30f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_34)){
            if(isFloorCheck(5000, false))
                costumeBuff += costumeBuff * 3.0;   //5000층 이하에서 획득량 300%
        }
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_6)){
            int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::BODY);
            double per = count * 0.10f;
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_LEGEND)) {
            double per = DataManager::GetCostumeLegendEffect(E_COSTUME::HAND, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::HAND));
            if (per > 0)
                costumeBuff += costumeBuff * per;
        }
        if (CostumeManager::getInstance()->getLegendCount() >= 1) {
            costumeBuff += costumeBuff * DataManager::GetCostumeLegendSetEffect(1);
        }
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_22)){
            costumeBuff += costumeBuff * 5.0;
        }

        
        double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_6, getRelicLevel(ePlace, E_RELIC::RELIC_6)) / 100.0f;
        if (relicPer > 0) {
            costumeBuff += costumeBuff * relicPer;
        }
        
        costumeBuff *= 100;
        value = MafUtils::bigMulNum(value, MafUtils::doubleToString(costumeBuff));
        value.pop_back();
        value.pop_back();
        
        //EnchantStone
        double stoneBuff = 1.0f;
        stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::GOLD);
        stoneBuff *= 100;
        value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
        value.pop_back();
        value.pop_back();
        
        double stoneForceBuff = 1.0f;
        stoneForceBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_GOLD);
        stoneForceBuff *= 100;
        value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneForceBuff));
        value.pop_back();
        value.pop_back();
        
        double stoneForce2Buff = 1.0f;
        stoneForce2Buff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_GOLD);
        stoneForce2Buff *= 100;
        value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneForce2Buff));
        value.pop_back();
        value.pop_back();
        
        //봉식타워4
        int buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_4);
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
            buildinglevel += 7;
        }
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
            buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
        }
        relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, getRelicLevel(ePlace, E_RELIC::RELIC_9));
        if (relicPer > 0) {
            buildinglevel += relicPer;
        }
        double buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_4, buildinglevel);
        if(buildingPer>0){
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(buildingPer));
            tt.pop_back();
            tt.pop_back();
            value =MafUtils::bigAddNum(value, tt);
        }
    }
    
    return value;
}

double UserInfoManager::getQuestTimeRate(E_PLACE ePlace)
{
    //2017/11/1 원래는 퍼센트를 구한후에 계산을 다시 진행했지만 한번더 계산할 필요없도록 변경함.
    //기존 if(per > 0)   ->  if(per<100)
    double nTime = 100;
    if( ePlace == E_PLACE::DG_NORMAL )
    {
//        nTime -= DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_5, getArtifactLevel(ePlace, E_ARTIFACT::ARTI_5), getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_5));
//        if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_3)){
//            nTime*=0.8;
//        }
    }
    else if( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        auto idx = getEquipDevil(E_EQUIP_TYPE::E_D_HEAD);
        auto level = getEquipLevelDevil(E_EQUIP_TYPE::E_D_HEAD);
        auto per = DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_HEAD, idx, level, E_EQUIP_OPTION::E_D_QUEST_TIME);
        if(per>0)
            nTime -= per;
    }
    
    return nTime;
}

#pragma mark - equip
int UserInfoManager::getEquipLevelDevil(E_EQUIP_TYPE type)
{
    return _uDevilEquipLv[type].valueInt();
}
void UserInfoManager::setEquipLevelDevil(E_EQUIP_TYPE type,int level,bool bSave)
{
    _uDevilEquipLv[type] = level;
    
    if ( bSave )
        SaveManager::saveEquip(E_PLACE::DG_DEVIL_DOM);
}

int UserInfoManager::getEquipReinforceLevelDevil(E_EQUIP_TYPE type)
{
    return _uDevilEquipReinforce[type].valueInt();
}
void UserInfoManager::setEquipReinforceLevelDevil(E_EQUIP_TYPE type,int level,bool bSave)
{
    _uDevilEquipReinforce[type] = level;
    
    if(_onModifyEquip!=nullptr)
        _onModifyEquip();
    
    if ( bSave )
        SaveManager::saveEquip(E_PLACE::DG_DEVIL_DOM);
}

E_EQUIP_IDX UserInfoManager::getEquipDevil(E_EQUIP_TYPE type)
{
    return _uDevilEquip[type];
}
void UserInfoManager::setEquipDevil(E_EQUIP_TYPE type,E_EQUIP_IDX equip,bool bSave)
{
    _uDevilEquip[type] = equip;
    
    if ( _onModifyEquip != nullptr )
        _onModifyEquip();
    
    if ( bSave )
        SaveManager::saveEquip(E_PLACE::DG_DEVIL_DOM);
}

#pragma mark - artifact
int UserInfoManager::getArtifactLevel(E_PLACE ePlace, E_ARTIFACT eArti)
{
    int nLevel = 0;
    if ( eArti == E_ARTIFACT::ARTI_45 )
    {
        int nFloorHigh = getHighFloor(E_PLACE::DG_NORMAL);
        if ( nFloorHigh >= DEVIL_JOIN_FLOOR )
        {
            nLevel = getHighFloor(E_PLACE::DG_DEVIL_DOM);
        }
    }
    else
    {
        nLevel = _uArtifactLevel[eArti].valueInt();
    }
    
    return nLevel;
}

void UserInfoManager::setArtifactLevel(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel, bool bSave)
{
    if ( eArti == E_ARTIFACT::ARTI_32_PRISON_WEAPON || eArti == E_ARTIFACT::ARTI_34_PICKAX)
    {
        if ( nLevel == 0 )
            _uArtifactLevel[eArti] = 1;
        else
            _uArtifactLevel[eArti] = nLevel;
    }
    else
    {
        _uArtifactLevel[eArti] = nLevel;
    }
    
    if ( bSave )
    {
        SaveManager::saveAllData();
    
        TimingPackageManager::getInstance()->invokeEventTimingPackage();
    }
}

void UserInfoManager::addHighArtifact(E_PLACE ePlace, E_ARTIFACT_TYPE eArtiType, int nLevel)
{
    E_ARTIFACT eArtifact;
    
    
    int c_gen = getArtifactCurrentGeneration(ePlace, eArtiType);
    int high_gen = DataManager::GetArtifactHighGeneration(eArtiType);
    
    
    for ( int i = 1; i < E_ARTIFACT::E_ARTIFACT_999_NONE; i++ )
    {
        E_ARTIFACT eArti = (E_ARTIFACT)i;
        auto artiInfo = DataManager::GetArtifactInfo(eArti);
        
        if ( artiInfo == nullptr )
            continue;
            
        if ( artiInfo->getType() != eArtiType )
            continue;
        
        if ( artiInfo->getGeneration() == c_gen )
        {
            eArtifact = eArti;
            if ( artiInfo->getGeneration() == high_gen && getArtifactLevel(ePlace, eArti) >= artiInfo->getLevelMax() )
            {
                return;
            }
        }
    }
    
    int nLevelNow = getArtifactLevel(ePlace, eArtifact) + nLevel;
    int nLevelMax = DataManager::GetArtifactMaxLevel(eArtifact);
    if ( nLevelNow > nLevelMax )
    {
        nLevelNow = nLevelMax;
    }

    //
    setArtifactLevel(ePlace, eArtifact, nLevelNow, true);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)eArtifact);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ARTIFACT, listData);
}
const int UserInfoManager::getArtifactCurrentGrade(const E_ARTIFACT_TYPE eArtiType)
{
    int result = 0;
    
    switch (eArtiType) {
        case E_ARTIFACT_TYPE::ARTI_TYPE_ATK:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_13_EVOL2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_18_EVOL3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_23_EVOL4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_28_EVOL5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_35_EVOL6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_40_EVOL7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_46_EVOL8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_54_EVOL9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_59_EVOL10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_64_EVOL11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_69_EVOL12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_74_EVOL13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_79_EVOL14) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_84_EVOL15) > 0)
               result++;
        }
            break;
        case E_ARTIFACT_TYPE::ARTI_TYPE_CRI:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_14_EVOL2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_19_EVOL3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_24_EVOL4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_29_EVOL5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_36_EVOL6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_41_EVOL7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_47_EVOL8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_55_EVOL9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_60_EVOL10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_65_EVOL11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_70_EVOL12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_75_EVOL13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_80_EVOL14) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_85_EVOL15) > 0)
               result++;
        }
            break;
        case E_ARTIFACT_TYPE::ARTI_TYPE_QUEST:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_4_EVOL1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_15_EVOL2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_20_EVOL3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_25_EVOL4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_30_EVOL5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_37_EVOL6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_42_EVOL7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_48_EVOL8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_56_EVOL9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_61_EVOL10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_66_EVOL11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_71_EVOL12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_76_EVOL13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_81_EVOL14) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_86_EVOL15) > 0)
               result++;
        }
            break;
        case E_ARTIFACT_TYPE::ARTI_TYPE_KILL:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_8_EVOL1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_16_EVOL2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_21_EVOL3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_26_EVOL4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_31_EVOL5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_38_EVOL6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_43_EVOL7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_49_EVOL8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_57_EVOL9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_62_EVOL10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_67_EVOL11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_72_EVOL12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_77_EVOL13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_82_EVOL14) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_87_EVOL15) > 0)
               result++;
        }
            break;
        case E_ARTIFACT_TYPE::ARTI_TYPE_CROSS:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_12_CROSS_EVOL1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_17_CROSS_EVOL2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_22_CROSS_EVOL3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_27_CROSS_EVOL4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_33_CROSS_EVOL5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_39_CROSS_EVOL6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_44_CROSS_EVOL7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_53_CROSS_EVOL8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_58_CROSS_EVOL9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_63_CROSS_EVOL10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_68_CROSS_EVOL11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_73_CROSS_EVOL12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_78_CROSS_EVOL13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_83_CROSS_EVOL14) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_88_CROSS_EVOL15) > 0)
               result++;
        }
            break;
        case E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT:
        {
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_89_DISCOUNT_1) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_90_DISCOUNT_2) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_91_DISCOUNT_3) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_92_DISCOUNT_4) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_93_DISCOUNT_5) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_94_DISCOUNT_6) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_95_DISCOUNT_7) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_96_DISCOUNT_8) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_97_DISCOUNT_9) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_98_DISCOUNT_10) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_99_DISCOUNT_11) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_100_DISCOUNT_12) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_101_DISCOUNT_13) > 0)
               result++;
            if(getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_102_DISCOUNT_14) > 0)
               result++;
        }
            break;
        default:
            break;
    }
    
    return result;
}
void UserInfoManager::setArtifactDevil(int idx,bool val,bool bSave)
{
    _bArtifactDevil[idx] = val;
    
    if ( bSave )
        SaveManager::saveAllData();
}

bool UserInfoManager::isArtifactHaveDevil(int idx)
{
    return _bArtifactDevil[idx];
}

int UserInfoManager::getArtifactCurrentGeneration(E_PLACE ePlace, E_ARTIFACT_TYPE eArtiType)
{
    int generation = 0;
    int highGeneration = DataManager::GetArtifactHighGeneration(eArtiType);
    
    //내 현재 차수 보물은?
    for ( int i = 1; i < E_ARTIFACT::E_ARTIFACT_999_NONE; i++ )
    {
        E_ARTIFACT eArti = (E_ARTIFACT)i;
        auto artiInfo = DataManager::GetArtifactInfo(eArti);
        
        if ( artiInfo == nullptr )
            continue;
        
        if ( artiInfo->getType() != eArtiType )
            continue;
        
        if ( getArtifactLevel(ePlace, eArti) < artiInfo->getLevelMax() )
        {
            generation = artiInfo->getGeneration();
            break;
        }
    }
    
    if ( generation == 0 )
    {
        generation = highGeneration;
    }

    return generation;
}

int UserInfoManager::getArtifactMaxLevelCount(E_PLACE ePlace)
{
    int nTotal = 0;
    for ( int i = 1; i < E_ARTIFACT::E_ARTIFACT_999_NONE; i++ )
    {
        E_ARTIFACT eType = (E_ARTIFACT)i;
        auto obj = DataManager::GetArtifactInfo(eType);
        if(obj == nullptr)
            continue;
        
        if ( DataManager::GetArtifactType(eType) != E_ARTIFACT_TYPE::ARTI_TYPE_NONE && getArtifactLevel(ePlace, eType) >= obj->getLevelMax() )
        {
            nTotal++;
        }
    }
    
    return nTotal;
}

InfoArtifact* UserInfoManager::getHighArtifactInfo(E_PLACE ePlace, E_ARTIFACT eArti)
{
    if ( eArti == E_ARTIFACT::ARTI_1_EVOL1 )
    {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_84_EVOL15) > 0)
        {
            eArti = E_ARTIFACT::ARTI_84_EVOL15;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_79_EVOL14) > 0)
        {
            eArti = E_ARTIFACT::ARTI_79_EVOL14;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_74_EVOL13) > 0)
        {
            eArti = E_ARTIFACT::ARTI_74_EVOL13;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_69_EVOL12) > 0)
        {
            eArti = E_ARTIFACT::ARTI_69_EVOL12;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_64_EVOL11) > 0)
        {
            eArti = E_ARTIFACT::ARTI_64_EVOL11;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_59_EVOL10) > 0)
        {
            eArti = E_ARTIFACT::ARTI_59_EVOL10;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_54_EVOL9) > 0)
        {
            eArti = E_ARTIFACT::ARTI_54_EVOL9;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_46_EVOL8) > 0)
        {
            eArti = E_ARTIFACT::ARTI_46_EVOL8;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_40_EVOL7) > 0)
        {
            eArti = E_ARTIFACT::ARTI_40_EVOL7;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_35_EVOL6) > 0)
        {
            eArti = E_ARTIFACT::ARTI_35_EVOL6;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_28_EVOL5) > 0)
        {
            eArti = E_ARTIFACT::ARTI_28_EVOL5;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_23_EVOL4) > 0)
        {
            eArti = E_ARTIFACT::ARTI_23_EVOL4;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_18_EVOL3) > 0)
        {
            eArti = E_ARTIFACT::ARTI_18_EVOL3;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_13_EVOL2) > 0)
        {
            eArti = E_ARTIFACT::ARTI_13_EVOL2;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) > 0)
        {
            eArti = E_ARTIFACT::ARTI_1_EVOL1;
        }
    }
    else if ( eArti == E_ARTIFACT::ARTI_3_EVOL1 )
    {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_85_EVOL15) > 0)
        {
            eArti = E_ARTIFACT::ARTI_85_EVOL15;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_80_EVOL14) > 0)
        {
            eArti = E_ARTIFACT::ARTI_80_EVOL14;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_75_EVOL13) > 0)
        {
            eArti = E_ARTIFACT::ARTI_75_EVOL13;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_70_EVOL12) > 0)
        {
            eArti = E_ARTIFACT::ARTI_70_EVOL12;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_65_EVOL11) > 0)
        {
            eArti = E_ARTIFACT::ARTI_65_EVOL11;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_60_EVOL10) > 0)
        {
            eArti = E_ARTIFACT::ARTI_60_EVOL10;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_55_EVOL9) > 0)
        {
            eArti = E_ARTIFACT::ARTI_55_EVOL9;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_47_EVOL8) > 0)
        {
            eArti = E_ARTIFACT::ARTI_47_EVOL8;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_41_EVOL7) > 0)
        {
            eArti = E_ARTIFACT::ARTI_41_EVOL7;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_36_EVOL6) > 0)
        {
            eArti = E_ARTIFACT::ARTI_36_EVOL6;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_29_EVOL5) > 0)
        {
            eArti = E_ARTIFACT::ARTI_29_EVOL5;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_24_EVOL4) > 0)
        {
            eArti = E_ARTIFACT::ARTI_24_EVOL4;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_19_EVOL3) > 0)
        {
            eArti = E_ARTIFACT::ARTI_19_EVOL3;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_14_EVOL2) > 0)
        {
            eArti = E_ARTIFACT::ARTI_14_EVOL2;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) > 0)
        {
            eArti = E_ARTIFACT::ARTI_3_EVOL1;
        }
    }
    else if ( eArti == E_ARTIFACT::ARTI_4_EVOL1 )
    {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_86_EVOL15) > 0)
        {
            eArti = E_ARTIFACT::ARTI_86_EVOL15;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_81_EVOL14) > 0)
        {
            eArti = E_ARTIFACT::ARTI_81_EVOL14;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_76_EVOL13) > 0)
        {
            eArti = E_ARTIFACT::ARTI_76_EVOL13;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_71_EVOL12) > 0)
        {
            eArti = E_ARTIFACT::ARTI_71_EVOL12;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_66_EVOL11) > 0)
        {
            eArti = E_ARTIFACT::ARTI_66_EVOL11;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_61_EVOL10) > 0)
        {
            eArti = E_ARTIFACT::ARTI_61_EVOL10;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_56_EVOL9) > 0)
        {
            eArti = E_ARTIFACT::ARTI_56_EVOL9;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_48_EVOL8) > 0)
        {
            eArti = E_ARTIFACT::ARTI_48_EVOL8;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_42_EVOL7) > 0)
        {
            eArti = E_ARTIFACT::ARTI_42_EVOL7;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_37_EVOL6) > 0)
        {
            eArti = E_ARTIFACT::ARTI_37_EVOL6;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_30_EVOL5) > 0)
        {
            eArti = E_ARTIFACT::ARTI_30_EVOL5;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_25_EVOL4) > 0)
        {
            eArti = E_ARTIFACT::ARTI_25_EVOL4;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_20_EVOL3) > 0)
        {
            eArti = E_ARTIFACT::ARTI_20_EVOL3;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_15_EVOL2) > 0)
        {
            eArti = E_ARTIFACT::ARTI_15_EVOL2;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) > 0)
        {
            eArti = E_ARTIFACT::ARTI_4_EVOL1;
        }
    }
    else if ( eArti == E_ARTIFACT::ARTI_8_EVOL1 )
    {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_87_EVOL15) > 0)
        {
            eArti = E_ARTIFACT::ARTI_87_EVOL15;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_82_EVOL14) > 0)
        {
            eArti = E_ARTIFACT::ARTI_82_EVOL14;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_77_EVOL13) > 0)
        {
            eArti = E_ARTIFACT::ARTI_77_EVOL13;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_72_EVOL12) > 0)
        {
            eArti = E_ARTIFACT::ARTI_72_EVOL12;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_67_EVOL11) > 0)
        {
            eArti = E_ARTIFACT::ARTI_67_EVOL11;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_62_EVOL10) > 0)
        {
            eArti = E_ARTIFACT::ARTI_62_EVOL10;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_57_EVOL9) > 0)
        {
            eArti = E_ARTIFACT::ARTI_57_EVOL9;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_49_EVOL8) > 0)
        {
            eArti = E_ARTIFACT::ARTI_49_EVOL8;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_43_EVOL7) > 0)
        {
            eArti = E_ARTIFACT::ARTI_43_EVOL7;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_38_EVOL6) > 0)
        {
            eArti = E_ARTIFACT::ARTI_38_EVOL6;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_31_EVOL5) > 0)
        {
            eArti = E_ARTIFACT::ARTI_31_EVOL5;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_26_EVOL4) > 0)
        {
            eArti = E_ARTIFACT::ARTI_26_EVOL4;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_21_EVOL3) > 0)
        {
            eArti = E_ARTIFACT::ARTI_21_EVOL3;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_16_EVOL2) > 0)
        {
            eArti = E_ARTIFACT::ARTI_16_EVOL2;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) > 0)
        {
            eArti = E_ARTIFACT::ARTI_8_EVOL1;
        }
    }
    else if ( eArti == E_ARTIFACT::ARTI_12_CROSS_EVOL1 )
    {
        if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_88_CROSS_EVOL15) > 0)
        {
            eArti = E_ARTIFACT::ARTI_88_CROSS_EVOL15;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_83_CROSS_EVOL14) > 0)
        {
            eArti = E_ARTIFACT::ARTI_83_CROSS_EVOL14;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_78_CROSS_EVOL13) > 0)
        {
            eArti = E_ARTIFACT::ARTI_78_CROSS_EVOL13;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_73_CROSS_EVOL12) > 0)
        {
            eArti = E_ARTIFACT::ARTI_73_CROSS_EVOL12;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_68_CROSS_EVOL11) > 0)
        {
            eArti = E_ARTIFACT::ARTI_68_CROSS_EVOL11;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_63_CROSS_EVOL10) > 0)
        {
            eArti = E_ARTIFACT::ARTI_63_CROSS_EVOL10;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_58_CROSS_EVOL9) > 0)
        {
            eArti = E_ARTIFACT::ARTI_58_CROSS_EVOL9;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_53_CROSS_EVOL8) > 0)
        {
            eArti = E_ARTIFACT::ARTI_53_CROSS_EVOL8;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_44_CROSS_EVOL7) > 0)
        {
            eArti = E_ARTIFACT::ARTI_44_CROSS_EVOL7;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_39_CROSS_EVOL6) > 0)
        {
            eArti = E_ARTIFACT::ARTI_39_CROSS_EVOL6;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_33_CROSS_EVOL5) > 0)
        {
            eArti = E_ARTIFACT::ARTI_33_CROSS_EVOL5;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_27_CROSS_EVOL4) > 0)
        {
            eArti = E_ARTIFACT::ARTI_27_CROSS_EVOL4;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_22_CROSS_EVOL3) > 0)
        {
            eArti = E_ARTIFACT::ARTI_22_CROSS_EVOL3;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_17_CROSS_EVOL2) > 0)
        {
            eArti = E_ARTIFACT::ARTI_17_CROSS_EVOL2;
        }
        else if (getArtifactLevel(ePlace, E_ARTIFACT::ARTI_12_CROSS_EVOL1) > 0)
        {
            eArti = E_ARTIFACT::ARTI_12_CROSS_EVOL1;
        }
    }
    
    auto objArtifact = DataManager::GetArtifactInfo(eArti);
    return objArtifact;
}

bool UserInfoManager::isArtifactReinforce(E_PLACE ePlace, int idx)
{
    bool isResult = false;
    
    auto iter = _listArtifactReinforce.find(idx);
    if ( iter != _listArtifactReinforce.end() )
    {
        isResult = true;
    }
    
    return isResult;
}

int UserInfoManager::getArtifactReinforce(E_PLACE ePlace, int idx)
{
    int nResult = 0;
    
    auto iter = _listArtifactReinforce.find(idx);
    if ( iter != _listArtifactReinforce.end() )
    {
        nResult = iter->second;
    }
    
    return nResult;
}

void UserInfoManager::setArtifactReinforce(E_PLACE ePlace, int idx, int count, bool bSave)
{
    _listArtifactReinforce[idx] = count;
    
    if ( bSave == true )
    {
        std::string str = "";
        for( const auto& obj : _listArtifactReinforce )
        {
            if ( str.length() != 0 )
            {
                str.append(",");
            }
            str.append(MafUtils::format("%d_%d", obj.first, obj.second));
        }
        UserDefault::getInstance()->setStringForKey(KEY_ARTIFACT_REINFORCE, MafAes256::Encrypt(str));
        
        SaveManager::saveAllData();
    }
}

int UserInfoManager::getArtifactExistCount(E_PLACE ePlace)
{
    int nCount = 0; //20200421 , 기존에는 0레벨 보물이 포함되었으나, 1레벨부터로 수정하였음. 게산 및 적용 설명에 용이함
    for ( int i = ARTI_1_EVOL1; i < E_ARTIFACT_999_NONE; i++ )
    {
        E_ARTIFACT eArti = (E_ARTIFACT)i;
        if ( getArtifactLevel(ePlace, eArti) > 0 )
            nCount++;
    }
    
    return nCount;
}

int UserInfoManager::getArtifactPointGain(E_PLACE ePlace)
{
    int nPoint = 0;
    
    std::string strEncrypt = UserDefault::getInstance()->getStringForKey(KEY_REVIVE_POINT);
    if ( !strEncrypt.empty() )
    {
        std::string strDecrypt = MafAes256::Decrypt(strEncrypt);
        nPoint = std::atoi(strDecrypt.c_str());
    }
    
    return nPoint;
}

void UserInfoManager::setArtifactPointGain(E_PLACE ePlace, int nPoint)
{
    UserDefault::getInstance()->setStringForKey(KEY_REVIVE_POINT, MafAes256::Encrypt(MafUtils::toString(nPoint)));
}

#pragma mark - relic
bool UserInfoManager::getRelic(E_PLACE ePlace, E_RELIC eRelic)
{
    bool bResult = false;
    if ( getRelicLevel(ePlace, eRelic) > 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

int UserInfoManager::getRelicLevel(E_PLACE ePlace, E_RELIC eRelic)
{
    int nLevel = _uRelicLevel[eRelic].valueInt();
    return nLevel;
}

void UserInfoManager::setRelicLevel(E_PLACE ePlace, E_RELIC eRelic, int nLevel, bool bSave)
{
    _uRelicLevel[eRelic] = nLevel;
    
    if ( bSave )
        SaveManager::saveAllData();
}


#pragma mark-
void UserInfoManager::setCubeDay(int day)
{
    if ( C_MAX_CUBE_DAY < day )
    {
        day = C_MAX_CUBE_DAY;
    }
    
    std::string str = MafUtils::toString(day);
    
    std::string enData = MafAes256::Encrypt(str);
    UserDefault::getInstance()->setStringForKey(KEY_REVIVE_CUBE_DAY, enData.c_str());
}

int UserInfoManager::getCubeDay()
{
    int cube = C_MAX_CUBE_DAY;
    std::string data = UserDefault::getInstance()->getStringForKey(KEY_REVIVE_CUBE_DAY);
    if ( !data.empty() )
    {
        data = MafAes256::Decrypt(data);
        cube = atoi(data.c_str());
    }
    
    return cube;
}

int UserInfoManager::getCubePlusDay()
{
    int nResult = 0;
    if ( getCubeDay() == C_MAX_CUBE_DAY )
    {
        nResult = 1;
    }
    
    return nResult;
}

int UserInfoManager::getCubeGain()
{
    int nResult = 0;
    nResult = getFloor(E_PLACE::DG_NORMAL) / 500 + 1;
    
    return nResult;
}

int UserInfoManager::getCubePlusGain()
{
    int nResult = 0;
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
 
    for ( int i = 1; i <= C_COUNT_ARTIFACT; i++ )
    {
        if ( isArtifactReinforce(ePlace, (E_ARTIFACT)i) == true )
        {
            nResult += 1;
        }
    }
    
    return nResult;
}

int UserInfoManager::getCubeTotalGain()
{
    int nResult = 0;
    E_PLACE ePlace = E_PLACE::DG_NORMAL;

    if ( getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 99999 ||
         getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 99999 ||
         getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 99999 ||
         getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 99999 )
    {
        float range = 0.7f;
        
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM,E_COSTUME_IDX::IDX_8))
        {
            range = 0.65f;
        }
        
        if ( getFloor(ePlace) >= getHighFloor(ePlace) * range )
        {
            nResult = getCubeGain();
            nResult += getCubePlusGain();
        }
    }
    
    return nResult;
}

#pragma mark-
void UserInfoManager::setFeverTime(float time)
{
    _uFeverTiem = time;
}

float UserInfoManager::getFeverTime()
{
    return _uFeverTiem;
}

void UserInfoManager::setFeverDay(int day)
{
    std::string str = MafUtils::toString(day);
    
    std::string enData = MafAes256::Encrypt(str);
    UserDefault::getInstance()->setStringForKey(KEY_REVIVE_FEVER_DAY, enData.c_str());
}

int UserInfoManager::getFeverDay()
{
    int point = C_MAX_FEVER_DAY;
    std::string data = UserDefault::getInstance()->getStringForKey(KEY_REVIVE_FEVER_DAY);
    if ( !data.empty() )
    {
        data = MafAes256::Decrypt(data);
        point = atoi(data.c_str());
    }
    
    return point;
}

void UserInfoManager::setItemFeverTime(float time, bool bSave)
{
    _uItemFeverTiem = time;
    
    if ( bSave )
        SaveManager::saveBuffTime();
}

float UserInfoManager::getItemFeverTime()
{
    return _uItemFeverTiem;
}

int UserInfoManager::getItemFeverCount()
{
    return _uItemFeverCount;
}

void UserInfoManager::setItemFeverCount(int value, bool bSave)
{
    _uItemFeverCount = value;
    
    if(bSave)
        SaveManager::saveBuffTime();
}

#pragma mark-
void UserInfoManager::setPetBafometzHave(bool data)
{
    std::string test = MafUtils::toString(data);
    UserDefault::getInstance()->setStringForKey(KEY_PET_BAFOMETZ, MafAes256::Encrypt(MafUtils::toString(data)));
}

bool UserInfoManager::getPetBafometzHave()
{
    bool isResult = false;
    std::string strValue = UserDefault::getInstance()->getStringForKey(KEY_PET_BAFOMETZ);
    if ( !strValue.empty() )
    {
        strValue = MafAes256::Decrypt(strValue);
        isResult = atoi(strValue.c_str());
    }
    
    return isResult;
}

E_PRINCESS_BUFF UserInfoManager::getPetBuffPriencess(E_PLACE ePlace)
{
    E_PRINCESS_BUFF eBuff = E_PRINCESS_BUFF::BUFF_NONE;
    
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        eBuff = _ePrincessBuffPrison;
    }
    else
    {
        eBuff = _ePrincessBuff;
    }

    return eBuff;
}

void UserInfoManager::setPetBuffPriencess(E_PLACE ePlace, E_PRINCESS_BUFF eBuff)
{
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        _ePrincessBuffPrison = eBuff;
    }
    else
    {
        _ePrincessBuff = eBuff;
    }
}

E_GATEKEEPER_BUFF UserInfoManager::getPetBuffGatekeeper(E_PLACE ePlace)
{
    E_GATEKEEPER_BUFF eBuff = E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER;
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        eBuff = _eGatekeeperBuffPrison;
    }
    else
    {
        eBuff = _eGatekeeperBuff;
    }

    return eBuff;
}
void UserInfoManager::setPetBuffGatekeeper(E_PLACE ePlace, E_GATEKEEPER_BUFF eBuff)
{
    if ( ePlace == E_PLACE::DG_PRISON )
    {
        _eGatekeeperBuffPrison = eBuff;
    }
    else
    {
        _eGatekeeperBuff = eBuff;
    }
}

int UserInfoManager::getTicketMax()
{
    int max = 0;
    
    if ( getPetBafometzHave() == false )
    {
        max = 1500;
    }
    else
    {
        max = 2500;
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_33) )
        {
            max -= 500;
        }
    }
    
    return max;
    
}


#pragma mark-



void UserInfoManager::setHonorLevel(E_HONOR honor, int level, bool bSave)
{
    int idx = (int)honor;
    idx -= 1000;
    
    _uHonorLevel[idx] = level;
    
    if(bSave){
        std::vector<std::string> vec;
        for (int i=E_HONOR::HONOR_1; i<=E_HONOR::HONOR_3; i++){
            vec.push_back(MafUtils::toString(getHonorLevel((E_HONOR)i)));
        }
        std::string str = MafUtils::splitCreate(vec, ',');
        UserDefault::getInstance()->setStringForKey(KEY_HONOR, MafAes256::Encrypt(str));
    }
}
int UserInfoManager::getHonorLevel(E_HONOR honor)
{
    int idx = (int)honor;
    idx -= 1000;
    
    int nLevel = _uHonorLevel[idx].valueInt();
    int nLevelMax = DataManager::GetHonorMaxLevel(honor);
    if ( nLevel > nLevelMax )
    {
        nLevel = nLevelMax;
    }
    
    return nLevel;
}
int UserInfoManager::getHonorHaveCount()
{
    int count = 0;
    for (int i=E_HONOR::HONOR_1; i<=E_HONOR::HONOR_3; i++) {
        if (getHonorLevel((E_HONOR)i) > 0) {
            count++;
        }
    }
    return count;
}


#pragma mark - SpecialMission
void UserInfoManager::setAttendVip(int value,bool bSave){
    _uAttendVip = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_ATTEND_VIP, MafAes256::Encrypt(MafUtils::toString(_uAttendVip)));
    }
}
int UserInfoManager::getAttendVip(){
    return _uAttendVip;
}
void UserInfoManager::setAttendDay(int value,bool bSave){
    _uAttendDay = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_ATTEND_DAY, MafAes256::Encrypt(MafUtils::toString(_uAttendDay)));
    }
}
int UserInfoManager::getAttendDay(){
    if (_uAttendDay > 20) {
        _uAttendDay = 20;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_11)) {
        _uAttendDay = 20;
    }
    return _uAttendDay;
}

#pragma mark-
void UserInfoManager::loadLocalData(){
    
    UserInfoStackManager::getInstance()->loadLocalData();
}

#pragma mark-
void UserInfoManager::setMyhomeLevel(int floor, int value,bool save)
{
    _uMyhomeLevelList[floor-1] = value;
    if(save == true)
    {
        SaveManager::getInstance()->saveMyhomeLevel();
    }
}
void UserInfoManager::setMyhomeWatchcount(int floor, int value,bool save)
{
    _uMyhomeWatchcountList[floor-1] = value;
    if(save == true)
    {
        SaveManager::getInstance()->saveMyhomeWatchcount();
    }
}
int UserInfoManager::getMyhomeLevel(int floor)
{
    return _uMyhomeLevelList[floor-1];
}
int UserInfoManager::getMyhomeLevelMax()
{
    return 10;
}
int UserInfoManager::getMyhomeWatchcount(int floor)
{
    return _uMyhomeWatchcountList[floor-1];
}
void UserInfoManager::setMyhomeAdsTime(int floor,int plus,bool bSave)
{
    _uTimeMyhomeAds[floor-1] += plus;
    
    if ( _uTimeMyhomeAds[floor-1] < 0 )
    {
        _uTimeMyhomeAds[floor-1] = 0;
    }
    
    if(bSave)
        SaveManager::saveMyhomeAdsTime();
}

int UserInfoManager::getMyhomeAdsTime(int floor){
    return _uTimeMyhomeAds[floor-1];
}

void UserInfoManager::setTimeMyhomeAdsAllMinus()
{
    
    bool bChange = false;
    for(int i=1;i<=C_MAX_MYHOME;i++){
        if(getMyhomeAdsTime(i) >0){
            setMyhomeAdsTime(i,-1,false);
            bChange = true;
        }
    }
    if(bChange)
        SaveManager::saveMyhomeAdsTime();
}
#pragma mark-

double UserInfoManager::getEquipTotalEffectPercentDevil(E_EQUIP_TYPE type){
    
    auto idx = getEquipDevil(type);
    auto level = getEquipLevelDevil(type);
    auto per = DataManager::GetEquipEffectRateDevil(type, idx, level);
    auto reinforce = getEquipReinforceLevelDevil(type);
    auto reinPer = DataManager::GetEquipReinforceRateDevil(reinforce);
    auto evol = DataManager::GetEquipEvolutionRateDevil(idx);
    
    /*석판 추가필요*/
     /*아이템샵 추가는?*/
//    if(per>0){
//        per *= 100;
//    }
//    }
    if(evol>1)
        per *= evol;
    if(reinPer>0){
//        reinPer -=1;
        per *= reinPer;
    }
//    std::string strPer = MafUtils::doubleToString(per);
//    return strPer;
    return per;
}
#pragma mark-

bool UserInfoManager::getGiftHave()
{
    return _uGiftHave;
}
void UserInfoManager::setGiftHave(bool bGift)
{
    _uGiftHave = bGift;
}

bool UserInfoManager::getAttendHave()
{
    return _uAttendHave;
}
void UserInfoManager::setAttendHave(bool bAttend)
{
    _uAttendHave = bAttend;
}

#pragma mark-
