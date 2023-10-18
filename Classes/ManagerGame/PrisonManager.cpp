//
//  PrisonManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "PrisonManager.h"

#include "GameObject/InfoQuest.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

PrisonManager::PrisonManager(void) :
_ePrisonType(PrisonType::DEFAULT),

_nResetTime(0),

_nHeart(0),
_nHeartMax(0),
_nHeartTime(0),
_nEnterAuto(0),
_nEnterAutoMax(0),
_bEnterAuto(false),

_dungeonPlay(false),
_dungeonKill(0),
_dungeonKillAdd(0),
_dungeonCount(0),
_dungeonTime(0),
_dungeonHp("0"),
_dungeonEarnKey("0"),

_nTicketCount(0),
_nTicketUseRatio(0),

_nBafometzCount(0),

_nAdsCount(0),
_nAdsMax(0),

_nJelly(0),
_nGummy(0),
_nJellyPrice(0),
_nJellyLvTime(0),
_nJellyLvDouble(0),
_nJellyLvExpand(0),
_nJellyReward(0),
_nJellyRewardTime(0),

_callbackDungeonInfo(nullptr),
_callbackDungeonResult(nullptr),
_callbackAds(nullptr),
_callbackAdsInterstitial(nullptr),
_callbackTickeetUse(nullptr),
_callbackJellyInfo(nullptr)
{
    
}

PrisonManager::~PrisonManager(void)
{
    _eventInfo.Clear();
    _eventInfoTime.Clear();
}

bool PrisonManager::init()
{
    setLoadQuestDefault();
    setLoadQuestHell();
    
    return true;
}

#pragma mark - load
void PrisonManager::setLoadQuestDefault()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PRISON_QUEST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listQuestDefault.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["type"].GetInt();
        auto countMax = jsonValue["type_count"].GetInt();
        auto titleKey = jsonValue["name_key"].GetString();
        auto reward = jsonValue["reward"].GetString();;
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setType(type);
        objQuest->setCountMax(countMax);
        objQuest->setTitleKey(titleKey);
        objQuest->setReward(reward);
        
        //
        _listQuestDefault.pushBack(objQuest);
    }
}

void PrisonManager::setLoadQuestHell()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PRISON_HELL_QUEST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listQuestHell.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["type"].GetInt();
        auto countMax = jsonValue["type_count"].GetInt();
        auto title = GAME_TEXTFORMAT(jsonValue["name_key"].GetString(), idx);
        auto reward = jsonValue["reward"].GetString();
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setType(type);
        objQuest->setCountMax(countMax);
        objQuest->setTitle(title);
        objQuest->setReward(reward);
         
        //
        _listQuestHell.pushBack(objQuest);
    }
}

#pragma mark - set, get : prison
cocos2d::Vector<InfoQuest*> PrisonManager::getListQuestDefault()
{
    return _listQuestDefault;
}

cocos2d::Vector<InfoQuest*> PrisonManager::getListQuestHell()
{
    return _listQuestHell;
}

PrisonManager::PrisonType PrisonManager::getPrisonType()
{
    return _ePrisonType;
}

void PrisonManager::setPrisonType(PrisonType type)
{
    _ePrisonType = type;
}

int PrisonManager::getResetTime()
{
    int nResult = 0;
    if ( _nResetTime != 0 )
    {
        nResult = (int)(_nResetTime - UtilsDate::getInstance()->getTime());
    }
    
    return nResult;
}

int PrisonManager::getHeart()
{
    return _nHeart;
}

void PrisonManager::setHeart(int nCount)
{
    _nHeart = nCount;
}

int PrisonManager::getHeartMax()
{
    return _nHeartMax;
}

int64_t PrisonManager::getHeartTime()
{
    return _nHeartTime;
}

int PrisonManager::getEnterAutoCount()
{
    if ( _nEnterAuto < 0 )
    {
        _nEnterAuto = 0;
    }
    
    return _nEnterAuto;
}

void PrisonManager::setEnterAutoCount(int nAuto)
{
    _nEnterAuto = nAuto;
}

int PrisonManager::getEnterAutoCountMax()
{
    return _nEnterAutoMax;
}

void PrisonManager::setEnterAutoCountMax(int nAuto)
{
    _nEnterAutoMax = nAuto;
}

bool PrisonManager::isEnterAuto()
{
    return _bEnterAuto;
}

void PrisonManager::setEnterAuto(bool bEnable)
{
    _bEnterAuto = bEnable;
}

bool PrisonManager::isDungeonPlay()
{
    return _dungeonPlay;
}

void PrisonManager::setDungeonPlay(bool bPlay)
{
    _dungeonPlay = bPlay;
}


int PrisonManager::getBafometzCount()
{
    return _nBafometzCount;
}

int PrisonManager::getDungeonCount()
{
    return _dungeonCount;
}

int PrisonManager::getDungeonKill()
{
    return _dungeonKill + _dungeonKillAdd;
}

int PrisonManager::getDungeonKillAdd()
{
    return _dungeonKillAdd;
}

void PrisonManager::setDungeonKillAdd(int dungeonKill)
{
    _dungeonKillAdd = dungeonKill;
}

std::string PrisonManager::getDungeonHp()
{
    if ( _dungeonHp.length() == 0 )
    {
        _dungeonHp = "0";
    }
    
    return _dungeonHp;
}

void PrisonManager::setDungeonHp(std::string dungeonHp)
{
    _dungeonHp = dungeonHp;
}

std::string PrisonManager::getDungeonEarnKey()
{
    return _dungeonEarnKey;
}

void PrisonManager::setDungeonEarnKey(std::string dungeonEarnKey)
{
    _dungeonEarnKey = dungeonEarnKey;
}

double PrisonManager::getDungeonTimeDefault()
{
    return 15;
}

double PrisonManager::getDungeonTimeNow()
{
    return _dungeonTime;
}

void PrisonManager::setDungeonTimeNow(double nTime)
{
    double nTimeDefault = getDungeonTimeDefault();
    if ( nTimeDefault < nTime )
    {
        nTime = nTimeDefault;
    }
    else if ( 0 > nTime )
    {
        nTime = 0 ;
    }
    
    _dungeonTime = nTime;
}

int PrisonManager::getAuthorityCount()
{
    int count = 0;
    
    if ( _ePrisonType == PrisonManager::PrisonType::DEFAULT )
    {
        std::string strDamage = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_PRISON, E_CHARACTER::CHA_HERO, false, false);
        int maxCount = 7000;

        // 저승사자의 불꽃
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_33) )
        {
            maxCount = 8000;
        }
        
        for ( int i = 0; i <= maxCount; ++i )
        {
            std::string hpMax = DataManager::GetMonsterHpPrison(i);
            if ( MafUtils::compareStr(strDamage, hpMax) == false )
            {
                break;
            }
            count++;
        }
    }
    else
    {
        std::string strDamage = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_PRISON, E_CHARACTER::CHA_HERO, false, false);
        int maxcount = 2000;
        
        if(CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_19))
        {
            maxcount += 1500;
        }
        if(CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_20))
        {
            maxcount += 1500;
        }
        if(CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_21))
        {
            maxcount += 1000;
        }
        if(CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_22))
        {
            maxcount += 1000;
        }
        
        for ( int i = 0; i <= maxcount; ++i )
        {
            std::string hpMax = DataManager::GetMonsterHpPrison(i + 15000);
            if ( MafUtils::compareStr(strDamage, hpMax) == false )
            {
                break;
            }
            count++;
        }
    }
    
    return count;
}



#pragma mark - set, get : ticket
int PrisonManager::getTicketCount()
{
    return _nTicketCount;
}

void PrisonManager::setTicketCount(int ticket)
{
    _nTicketCount = ticket;
}

double PrisonManager::getTicketUseMax()
{
    double result = 0.0;
    
    std::string strText = ParameterManager::getInstance()->getParameterString("p_ticket_value");
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if ( strText.length() != 0 && jsonParser.HasParseError() == false )
    {
        int use_max = jsonParser["use_max"].GetInt();
        result = jsonParser["use_ratio"][use_max-1].GetDouble();
    }
    
    return result;
}

double PrisonManager::getTicketUseRatio()
{
    return _nTicketUseRatio;
}

int PrisonManager::getTicketUseIdx()
{
    int idx = -1;
    
    //
    std::string strText = ParameterManager::getInstance()->getParameterString("p_ticket_value");
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if ( strText.length() != 0 && jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser["use_ratio"].Size(); i++ )
        {
            double ratio = jsonParser["use_ratio"][i].GetDouble();
            if ( ratio == _nTicketUseRatio )
            {
                idx = i;
                break;
            }
        }
    }
    
    return idx;
}

#pragma mark - set, get : ads
bool PrisonManager::isAds()
{
    bool bResult = false;
    
    if ( _nAdsCount > 0 )
        bResult = true;
    
    return bResult;
}

#pragma mark - set, get : jelly
bool PrisonManager::isJelly()
{
    bool bResult = false;
    if ( _nJelly == 1 )
    {
        bResult = true;
    }
    
    return bResult;
}

bool PrisonManager::isGummy()
{
    bool bResult = false;
    if ( _nGummy == 1 )
    {
        bResult = true;
    }
    
    return bResult;
}

void PrisonManager::setGummy(int nGummy)
{
    _nGummy = nGummy;
}

int PrisonManager::getJellyPrice()
{
    return _nJellyPrice;
}

int PrisonManager::getJellyLvTime()
{
    return _nJellyLvTime;
}

int PrisonManager::getJellyLvDouble()
{
    return _nJellyLvDouble;
}

int PrisonManager::getJellyLvExpand()
{
    return _nJellyLvExpand;
}

bool PrisonManager::isJellyRewardMax()
{
    bool bResult = false;
    if ( _nJellyReward >= getJellyRewardMax() )
    {
        bResult = true;
    }
    
    return bResult;
}

int PrisonManager::getJellyReward()
{
    return _nJellyReward;
}

int PrisonManager::getJellyRewardMax()
{
    return DataManager::GetJellyEffect(E_JELLY::JELLY_EXPAND, _nJellyLvExpand);
}

int64_t PrisonManager::getJellyRewardTime()
{
    return _nJellyRewardTime;
}

#pragma mark - game

#pragma mark - network
void PrisonManager::subEventInfo(MafFunction<void(bool, bool, int)>* func)
{
    _eventInfo += func;
}

void PrisonManager::invokeEventInfo(bool bResult, bool bAvailable, int nType)
{
    _eventInfo(bResult, bAvailable, nType);
}

void PrisonManager::unSubEventInfo(void* obj)
{
    _eventInfo.Clear(obj);
}

void PrisonManager::subEventInfoTime(MafFunction<void()>* func)
{
    _eventInfoTime += func;
}
void PrisonManager::invokeEventInfoTime()
{
    _eventInfoTime();
}
void PrisonManager::unSubEventInfoTime(void* obj)
{
    _eventInfoTime.Clear(obj);
}

#pragma mark - network : info
void PrisonManager::requestInfo(bool bPopupLoading, int nType, const std::function<void(bool, bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    _callbackDungeonInfo = callback;
    
    //
    int nCostume10 = 0;
    int nCostume12 = 0;
    int nCostume15 = 0;
    int nCostume17 = 0;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10) ) nCostume10 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12) ) nCostume12 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15) ) nCostume15 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17) ) nCostume17 = 1;
    
    std::string url = "/game/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_prison_type", nType);
    
    //key,gem, 보물,펫 등
    req->setData("_gem", ItemsMoneyManager::getInstance()->getGem());
    req->setData("_revival", UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL));
    req->setData("_floor", UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL));
    
    req->setData("_art1", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1));
    req->setData("_art3", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1));
    req->setData("_art4", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_4_EVOL1));
    req->setData("_art8", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_8_EVOL1));
    req->setData("_art9", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_9_KEY));
    req->setData("_art10", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_10_KEY));
    req->setData("_art11", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_11_KEY));
    req->setData("_art12", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_12_CROSS_EVOL1));
    req->setData("_art13", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_17_CROSS_EVOL2));
    
    req->setData("_costume1", nCostume10);
    req->setData("_costume2", nCostume12);
    req->setData("_costume3", nCostume15);
    req->setData("_costume4", nCostume17);
    
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        
        if ( bPopupLoading == true )
        {
            PopupLoading::hide();
        }
        
        if ( nType == 2 )
        {
            responseInfoFire(response,dd);
        }
        else
        {
            responseInfoDefault(response,dd);
        }
        
    };
    req->send(cb);
}

void PrisonManager::responseInfoDefault(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackDungeonInfo != nullptr )
        {
            _callbackDungeonInfo(false, false, 1);
            _callbackDungeonInfo = nullptr;
        }
        invokeEventInfo(false, false, 1);
        return;
    }
    
    bool bAvailable = jsonParser["_available"].GetBool();
    if ( bAvailable == false )
    {
        if ( _callbackDungeonInfo != nullptr )
        {
            _callbackDungeonInfo(true, bAvailable, 1);
            _callbackDungeonInfo = nullptr;
        }
        invokeEventInfo(true, bAvailable, 1);
        return;
    }
    
    //
    setPrisonType(PrisonManager::PrisonType::DEFAULT);
    
    //
    _nHeart = jsonParser["_heart"].GetInt();
    _nHeartMax = jsonParser["_heart_max"].GetInt();
    _nHeartTime = UtilsDate::getInstance()->getTime() + jsonParser["_heart_time"].GetInt();
    
    _nResetTime = jsonParser["_reset_time"].GetInt();
    _nTicketCount = jsonParser["_ticket"].GetInt();
    _nTicketUseRatio = jsonParser["_event"].GetDouble();
    _nAdsCount = jsonParser["_ads_count"].GetInt();
    _nAdsMax = jsonParser["_ads_max"].GetInt();
    _nBafometzCount = jsonParser["_returnCount"].GetInt();
    
    _dungeonKill  = jsonParser["_kill"].GetInt();
    _dungeonCount = jsonParser["_count"].GetInt();
    _dungeonHp  = jsonParser["_hp"].GetString();
    _dungeonTime = 0;
    if ( _dungeonCount >= 3 )
    {
        SpecialMissionManager::getInstance()->clearSpecialMission(50);
    }
    if ( _dungeonCount >= 7 )
    {
        SpecialMissionManager::getInstance()->clearSpecialMission(80);
    }
    if ( _dungeonCount >= 10 )
    {
        SpecialMissionManager::getInstance()->clearSpecialMission(96);
    }
    
    if ( _dungeonKill > SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER) )
    {
        SaveManager::setHistoryData(E_HISTORY_DATA::K_PRISON_METER, _dungeonKill);
        TimingPackageManager::getInstance()->invokeEventTimingPackage();
    }
    
    if ( _dungeonCount > SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_COUNT) )
    {
        SaveManager::setHistoryData(E_HISTORY_DATA::K_PRISON_COUNT, _dungeonCount);
    }
    
    //
    SaveManager::setHistoryData(E_HISTORY_DATA::K_START_DAY, jsonParser["_startday"].GetString());
    
    //
    int nCoin = jsonParser["_coin"].GetInt();
    int nBong = jsonParser["_material"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nBong);
    
    //
    for( int i = 0; i < _listQuestDefault.size(); i++)
    {
        std::string strKey = MafUtils::format("_q%d", i+1);
        if ( jsonParser.HasMember(strKey.c_str()) == false )
        {
            continue;
        }
        
        //
        auto obj = (InfoQuest*)_listQuestDefault.at(i);
        
        //
        int recv = jsonParser[strKey.c_str()].GetInt();
        if ( recv != 0 )    obj->setReceive(true);
        else                obj->setReceive(false);
        
        if ( obj->getType() == 1 )
            obj->setCountNow((int)_dungeonCount);
        else
            obj->setCountNow((int)_dungeonKill) ;
    }
    
    //타이밍 패키지
    TimingPackageManager::getInstance()->invokeEventTimingPackage();
    
    //
    if ( _callbackDungeonInfo != nullptr )
    {
        _callbackDungeonInfo(true, bAvailable, 1);
        _callbackDungeonInfo = nullptr;
    }
    invokeEventInfo(true, bAvailable, 1);
}

void PrisonManager::responseInfoFire(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackDungeonInfo != nullptr )
        {
            _callbackDungeonInfo(false, false, 2);
            _callbackDungeonInfo = nullptr;
        }
        invokeEventInfo(false, false, 2);
        return;
    }
    
    bool bAvailable = jsonParser["_available"].GetBool();
    if ( bAvailable == false )
    {
        if ( _callbackDungeonInfo != nullptr )
        {
            _callbackDungeonInfo(true, bAvailable, 2);
            _callbackDungeonInfo = nullptr;
        }
        invokeEventInfo(true, bAvailable, 2);
        return;
    }
    
    //
    setPrisonType(PrisonManager::PrisonType::FIRE);
    
    //
    _nHeart = jsonParser["_heart"].GetInt();
    _nHeartMax = jsonParser["_heart_max"].GetInt();
    _nHeartTime = UtilsDate::getInstance()->getTime() + jsonParser["_heart_time"].GetInt();
    
    _nResetTime = jsonParser["_reset_time"].GetInt();
    
    _dungeonKill  = jsonParser["_kill_h"].GetInt();
    _dungeonCount = jsonParser["_count_h"].GetInt();
    _dungeonHp  = jsonParser["_hp_h"].GetString();
    _dungeonTime = 0;
    if ( _dungeonKill > SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_HELL_METER) )
    {
        SaveManager::setHistoryData(E_HISTORY_DATA::K_PRISON_HELL_METER, (int)_dungeonKill);
    }
    if ( _dungeonCount > SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_COUNT) )
    {
        SaveManager::setHistoryData(E_HISTORY_DATA::K_PRISON_COUNT, (int)_dungeonCount);
    }
    
    //
    SaveManager::setHistoryData(E_HISTORY_DATA::K_START_DAY, jsonParser["_startday"].GetString());
    
    //
    int nHell = jsonParser["_helltrium"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::HELL, 0, nHell);
    
    //
    std::stringstream str;
    for( int i = 0; i < _listQuestHell.size(); i++)
    {
        std::string strKey = MafUtils::format("_qh%d", i+1);
        if ( jsonParser.HasMember(strKey.c_str()) == false )
        {
            continue;
        }
        
        //
        auto obj = _listQuestHell.at(i);
        
        //
        int recv = jsonParser[strKey.c_str()].GetInt();
        if ( recv != 0 )    obj->setReceive(true);
        else                obj->setReceive(false);
        
        if ( obj->getType() == 1 )
            obj->setCountNow((int)_dungeonCount);
        else
            obj->setCountNow((int)_dungeonKill);
    }
    
    //
    if ( _callbackDungeonInfo != nullptr )
    {
        _callbackDungeonInfo(true, bAvailable, 2);
        _callbackDungeonInfo = nullptr;
    }
    invokeEventInfo(true, bAvailable, 2);
}

void PrisonManager::requestInfoTime(int nType)
{
    int nCostume10 = 0;
    int nCostume12 = 0;
    int nCostume15 = 0;
    int nCostume17 = 0;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10) ) nCostume10 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12) ) nCostume12 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15) ) nCostume15 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17) ) nCostume17 = 1;
    
    std::string url = "/game/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_prison_type", nType);
    
    //key,gem, 보물,펫 등
    req->setData("_gem", ItemsMoneyManager::getInstance()->getGem());
    req->setData("_revival", UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL));
    req->setData("_floor", UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL));
    
    req->setData("_art1", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1));
    req->setData("_art3", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1));
    req->setData("_art4", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_4_EVOL1));
    req->setData("_art8", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_8_EVOL1));
    req->setData("_art9", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_9_KEY));
    req->setData("_art10", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_10_KEY));
    req->setData("_art11", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_11_KEY));
    req->setData("_art12", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_12_CROSS_EVOL1));
    req->setData("_art13", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_17_CROSS_EVOL2));
    
    req->setData("_costume1", nCostume10);
    req->setData("_costume2", nCostume12);
    req->setData("_costume3", nCostume15);
    req->setData("_costume4", nCostume17);
    
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfoTime(response,dd);
    };
    req->send(cb);
}

void PrisonManager::responseInfoTime(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        return;
    }
    
    bool bAvailable = jsonParser["_available"].GetBool();
    if ( bAvailable == false )
    {
        return;
    }
    
    //
    _nHeart = jsonParser["_heart"].GetInt();
    _nHeartMax = jsonParser["_heart_max"].GetInt();
    _nHeartTime = UtilsDate::getInstance()->getTime() + jsonParser["_heart_time"].GetInt();
    
    _nResetTime = jsonParser["_reset_time"].GetInt();
    
    //
    invokeEventInfoTime();
}

#pragma mark - network : result
void PrisonManager::requestDefaultResult(bool bPopupLoading, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackDungeonResult = callback;
    
    //
    int nCostume10 = 0;
    int nCostume12 = 0;
    int nCostume15 = 0;
    int nCostume17 = 0;

    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10) ) nCostume10 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12) ) nCostume12 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15) ) nCostume15 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17) ) nCostume17 = 1;

    std::string url = "/v16/result";

    int nKill = _dungeonKill + _dungeonKillAdd;
    std::vector<int> logNums{50, 100, 250, 300, 600, 700, 1000, 1250, 1500, 1750, 2000, 2250};

    for(int i = 0; i < logNums.size(); ++i)
    {
        for(int j = (int)_dungeonKill; j <= nKill; ++j)
        {
            if(j == logNums[i])
            {
                std::string logContent = MafUtils::format(kAnalEventPrison, logNums[i]);
                MafAnalyticsManager::LogEvent(logContent.c_str(), kRepeatTrue);
                break;
            }
        }
    }

    _dungeonKill = nKill;
    _dungeonKillAdd = 0;

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_prison_type", 1);
    req->setData("_kill", nKill);
    req->setData("_count", _dungeonCount);
    req->setData("_hp", _dungeonHp.c_str());
    req->setData("_costume", MafUtils::toString(nCostume10).c_str());
    req->setData("_costume2", MafUtils::toString(nCostume12).c_str());
    req->setData("_costume3", MafUtils::toString(nCostume15).c_str());
    req->setData("_costume4", MafUtils::toString(nCostume17).c_str());
    req->setData("_heart", 0);
    
    int complete = 0;
    for ( auto& info : _listQuestDefault )
    {
        complete = 0;
        if ( info->isReceive() )
        {
            complete = 2;
        }
        else
        {
            if ( info->getType() == 1 )
            {
            if ( _dungeonCount + 1 >= info->getCountMax() )
                complete = 1;
            }
            else
            {
                if ( _dungeonKill >= info->getCountMax() )
                    complete = 1;
            }
        }
        std::stringstream str;
        str << "_q" << info->getIdx();
        req->setData(str.str().c_str(), complete);
    }
    
    MafResponseNew cb = [&, bPopupLoading](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, response,dd);
    };
    req->send(cb);
}
void PrisonManager::requestDefaultResultSkip(bool bPopupLoading, int nSkip, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackDungeonResult = callback;
    
    //
    int nCostume10 = 0;
    int nCostume12 = 0;
    int nCostume15 = 0;
    int nCostume17 = 0;
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10) ) nCostume10 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12) ) nCostume12 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15) ) nCostume15 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17) ) nCostume17 = 1;
    
    std::string url = "/v16/result";
    
    int nKill = _dungeonKill + _dungeonKillAdd;
    _dungeonKill = nKill;
    _dungeonKillAdd = 0;

    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_prison_type", 1);
    req->setData("_kill", nKill);
    req->setData("_count", _dungeonCount);
    req->setData("_hp", _dungeonHp.c_str());
    req->setData("_costume", MafUtils::toString(nCostume10).c_str());
    req->setData("_costume2", MafUtils::toString(nCostume12).c_str());
    req->setData("_costume3", MafUtils::toString(nCostume15).c_str());
    req->setData("_costume4", MafUtils::toString(nCostume17).c_str());
    req->setData("_heart", nSkip);
    
    int complete = 0;
    for ( auto& info : _listQuestDefault )
    {
        complete = 0;
        if ( info->isReceive() )
        {
            complete = 2;
        }
        else
        {
            if ( info->getType() == 1 )
            {
                if ( _dungeonCount + nSkip >= info->getCountMax() )
                    complete = 1;
            }
            else
            {
                if ( _dungeonKill >= info->getCountMax() )
                    complete = 1;
            }
        }
        std::stringstream str;
        str << "_q" << info->getIdx();
        req->setData(str.str().c_str(), complete);
    }
    
    MafResponseNew cb = [&, bPopupLoading](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, response,dd);
    };
    req->send(cb);
}
void PrisonManager::requestFireResult(bool bPopupLoading, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackDungeonResult = callback;
    
    //
    int nCostume10 = 0;
    int nCostume12 = 0;
    int nCostume15 = 0;
    int nCostume17 = 0;
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10) ) nCostume10 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12) ) nCostume12 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15) ) nCostume15 = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17) ) nCostume17 = 1;
    
    std::string url = "/v16/result";
    
    int nKill = _dungeonKill + _dungeonKillAdd;
    _dungeonKill = nKill;
    _dungeonKillAdd = 0;
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_prison_type", 2);
    req->setData("_kill", nKill);
    req->setData("_count", (int)_dungeonCount);
    req->setData("_hp", _dungeonHp.c_str());
    req->setData("_costume", MafUtils::toString(nCostume10).c_str());
    req->setData("_costume2", MafUtils::toString(nCostume12).c_str());
    req->setData("_costume3", MafUtils::toString(nCostume15).c_str());
    req->setData("_costume4", MafUtils::toString(nCostume17).c_str());
    req->setData("_heart", 0);
    
    int complete = 0;
    for ( auto& info : _listQuestHell )
    {
        complete = 0;
        if ( info->isReceive() )
        {
            complete = 2;
        }
        else
        {
            if ( info->getType() == 1 )
            {
                if ( _dungeonKill + 1 >= info->getCountMax() )
                    complete = 1;
            }
            else
            {
                if ( _dungeonKill >= info->getCountMax() )
                    complete = 1;
            }
        }
        std::stringstream str;
        str << "_q" << info->getIdx();
        req->setData(str.str().c_str(), complete);
    }
    
    MafResponseNew cb = [&, bPopupLoading](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonManager::responseResult(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false &&  (jsonParser.HasMember("_resultsuc") == false || jsonParser["_resultsuc"].GetBool() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( _callbackDungeonResult != nullptr )
        {
            _callbackDungeonResult(false);
            _callbackDungeonResult = nullptr;
        }
        return;
    }
    
    bool bKeyReward = false;
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    for ( auto& objTemp : listReward )
    {
        if ( objTemp->getIdx() == (int)E_ITEMS::HELL )
        {
            int nCount = std::atoi(objTemp->getCount().c_str());
            ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::HELLTRIUM_GET, nCount);
        }
        else if ( objTemp->getIdx() == (int)E_ITEMS::KEY )
        {
            if ( _dungeonEarnKey.length() > 1 )
            {
                bKeyReward = true;
                
                //
                std::string strCount = objTemp->getCount();
                strCount = MafUtils::bigAddNum(_dungeonEarnKey, strCount);
                
                objTemp->setCount(strCount);
            }
        }
    }
    
    if ( bKeyReward == false && _dungeonEarnKey.length() > 1 )
    {
        auto objTemp = ItemsManager::getInstance()->getInfoItems(E_ITEMS::KEY);
        objTemp->setCount(_dungeonEarnKey);
        listReward.pushBack(objTemp);
    }
    //
    ItemsManager::getInstance()->addItems(listReward);
    
    //
    ItemsManager::getInstance()->setItemsServer(strItems);
    _dungeonEarnKey = "0";
    
    //
    _nEnterAuto--;
    
    //
    if ( _callbackDungeonResult != nullptr )
    {
        _callbackDungeonResult(true);
        _callbackDungeonResult = nullptr;
    }
    
    //
    DataManager::getInstance()->requestPushInfo("prison");
}

#pragma mark - network : other
void PrisonManager::requestAds(const std::function<void(bool)>& callback)
{
    //
    _callbackAds = callback;
    
    //
    std::string url = "/freeticket";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAds(response,dd);
    };
    req->send(cb);
}

void PrisonManager::responseAds(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_heart") == false || jsonParser.HasMember("_ads_count") == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false);
        }
        return;
    }
    
    _nHeart = jsonParser["_heart"].GetInt();
    _nAdsCount = jsonParser["_ads_count"].GetInt();
    
    //
    cocos2d::Vector<InfoItems *> listReward;
    
    auto objReward = ItemsManager::getInstance()->getInfoItems(E_ITEMS::TICKET);
    objReward->setCount(2);
    listReward.pushBack(objReward);
    
    if ( listReward.size() != 0 )
    {
        PopupToastItems::show(listReward);
    }
    
    //
    if ( _callbackAds != nullptr )
    {
        _callbackAds(true);
    }
}

void PrisonManager::requestAdsInterstitial(const std::function<void(bool, int)>& callback)
{
    //
    _callbackAdsInterstitial = callback;
    
    AdsUtilsManager::getInstance()->addAdsInterstitialErrorType(1, UtilsDate::getInstance()->getTime());
    
    //
    std::string url = "/v1/user/advertisement/reward/%d/1";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAdsInterstitial(response,dd);
    };
    req->send(cb);
}

void PrisonManager::responseAdsInterstitial(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( _callbackAdsInterstitial != nullptr )
        {
            _callbackAdsInterstitial(false, -1);
        }
        return;
    }
    
    /*
     0 : adsType값이 없음
     1 : 성공
     3 : 이미 광고보상을 받음
     4 : 조건 불일치
     */
    int nResult         = jsonParser["_result"].GetInt();
    int nAdsType        = jsonParser["_ads_type"].GetInt();
    
    AdsUtilsManager::getInstance()->removeAdsInterstitialErrorType(nAdsType);
    
    if ( nResult == 1 )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if ( _callbackAdsInterstitial != nullptr )
        {
            _callbackAdsInterstitial(true, nResult);
        }
    }
    else
    {
        if ( _callbackAdsInterstitial != nullptr )
        {
            _callbackAdsInterstitial(false, nResult);
        }
    }
}

void PrisonManager::requestTicketUse(const std::function<void(bool, int)>& callback)
{
    //
    _callbackTickeetUse = callback;
    
    //
    std::string url = "/v1/useticket";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTicketUse(response,dd);
    };
    req->send(cb);
}

void PrisonManager::responseTicketUse(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }

    if ( bError == true )
    {
        if ( _callbackTickeetUse != nullptr )
        {
            _callbackTickeetUse(false, -1);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _nTicketCount = jsonParser["_ticket"].GetInt();
        _nTicketUseRatio = jsonParser["_event"].GetDouble();
        
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    //
    if ( _callbackTickeetUse != nullptr )
    {
        _callbackTickeetUse(true, nResult);
    }
}

void PrisonManager::requestJellyInfo(const std::function<void(bool)>& callback)
{
    //
    _callbackJellyInfo = callback;
    
    //
    std::string url = "/jelly/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseJellyInfo(response,dd);
    };
    req->send(cb);
}

void PrisonManager::responseJellyInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }

    if ( bError == true )
    {
        if ( _callbackJellyInfo != nullptr )
        {
            _callbackJellyInfo(false);
        }
        return;
    }
    
    _nJelly = jsonParser["_jelly"].GetInt();
    _nGummy = jsonParser["_gummie"].GetInt();
    
    if ( jsonParser.HasMember("_contract_cost") == true )
    {
        _nJellyPrice = jsonParser["_contract_cost"].GetInt();
    }
    else
    {
        _nJellyPrice = 1000;
    }
    
    if ( _nJelly == 1 )
    {
        _nJellyLvTime = jsonParser["_lv_time"].GetInt();
        _nJellyLvDouble = jsonParser["_lv_double"].GetInt();
        _nJellyLvExpand = jsonParser["_lv_expand"].GetInt();
        
        _nJellyReward = jsonParser["_new"].GetInt();
        _nJellyRewardTime = jsonParser["_time"].GetInt64();
    }
    else
    {
        _nJellyLvTime = 0;
        _nJellyLvDouble = 0;
        _nJellyLvExpand = 0;
        
        _nJellyReward = 0;
        _nJellyRewardTime = 0;
    }
    
    //
    if ( _callbackJellyInfo != nullptr )
    {
        _callbackJellyInfo(true);
    }
}

