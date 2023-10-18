//
//  CostumeManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "CostumeManager.h"

#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

CostumeManager::CostumeManager(void) :
_nAdsCount(0),
_infoExist{0,},
_infoExistAura{0,},
_infoEquip{0,},
_infoEquipAura(0),
_infoRent{0,},
_infoRentTime{0,},
_infoRentCount{0,},
_infoHonor{0,},

_callbackInfo(nullptr),
_callbackAds(nullptr),
_callbackRent(nullptr),
_callbackBuy(nullptr),
_callbackAuraBuy(nullptr),
_callbackHonorBuy(nullptr),
_callbackLegendBuy(nullptr)
{
    
}

CostumeManager::~CostumeManager(void)
{
    
}

bool CostumeManager::init()
{
    // load
    setLoad();
    
    // 기본 코스튬은 적용
    for( int i = 1; i <= C_COUNT_COSTUME_TYPE; i++)
    {
        _infoEquip[(E_COSTUME)i] = 1;
    }
    _infoEquipAura = 1;
    
    for( int i = 1; i <= C_COUNT_COSTUME_TYPE; i++)
    {
        _infoExist[E_COSTUME_IDX::IDX_1_DEFAULT][(E_COSTUME)i] = 1;
    }
    _infoExistAura[E_COSTUME_IDX::IDX_1_DEFAULT] = 1;
    
    return true;
}

#pragma mark - load
void CostumeManager::setLoad()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::COSTUME);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
     
    _listCostume.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        std::string strType = jsonValue["type"].GetString();
        int nIdx = jsonValue["idx"].GetInt();
        int nSort = jsonValue["sort"].GetInt();
        std::string strTitleKey = jsonValue["name_key"].GetString();
        std::string strContentsKey = jsonValue["comment_key"].GetString();
        
        
        int nTypeNormal = jsonValue["is_dungeon"].GetInt();
        int nTypePrison = jsonValue["is_prison"].GetInt();
        
        int nPrice = jsonValue["price"].GetInt();
        
        // obj
        auto obj = InfoCostume::create();
        obj->setType(strType);
        obj->setIdx(nIdx);
        obj->setSort(nSort);
        
        obj->setTitle(strTitleKey);
        obj->setDesc(strContentsKey);
        
        obj->setTypeNormal(nTypeNormal);
        obj->setTypePrison(nTypePrison);
        
        obj->setPrice(nPrice);
        
        _listCostume.pushBack(obj);
    }
    
    std::sort(_listCostume.begin(), _listCostume.end(), [](InfoCostume *a, InfoCostume *b) -> bool{
        if (a->getType() == b->getType())
        {
            return a->getSort() < b->getSort();
        }
        
        return a->getType() < b->getType();
    });
}

#pragma mark - set, get, add
cocos2d::Vector<InfoCostume*> CostumeManager::getCostumeAll()
{
    return _listCostume;
}
cocos2d::Vector<InfoCostume*> CostumeManager::getCostumeType(std::string strType)
{
    E_COSTUME eType = E_COSTUME::HEAD;
    
    if ( strType.compare("BODY") == 0 )             eType = E_COSTUME::BODY;
    else if ( strType.compare("HEAD") == 0 )        eType = E_COSTUME::HEAD;
    else if ( strType.compare("HAND") == 0 )        eType = E_COSTUME::HAND;
    else if ( strType.compare("FOOT") == 0 )        eType = E_COSTUME::FOOT;
    else if ( strType.compare("CAPE") == 0 )        eType = E_COSTUME::CAPE;
    else if ( strType.compare("JEWEL") == 0 )       eType = E_COSTUME::JEWEL;
    else if ( strType.compare("SETITEM") == 0 )     eType = E_COSTUME::SETITEM;
    
    return getCostumeType(eType);
}
cocos2d::Vector<InfoCostume*> CostumeManager::getCostumeType(E_COSTUME eType)
{
    Vector<InfoCostume*> listResult;
    for ( auto &obj : _listCostume )
    {
        if ( obj->getType() == eType )
        {
            if(obj->getType() == E_COSTUME::HEAD && obj->getIdx() == 10)
            {
                listResult.insert(2, obj);
                continue;
            }
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}
InfoCostume* CostumeManager::getCostume(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    InfoCostume *pResult = nullptr;
    
    for ( auto &obj : _listCostume )
    {
        if ( obj->getType() == eType && obj->getIdx() == eIdx )
        {
            pResult = obj;
            break;
        }
    }
    
    return pResult;
}

#pragma mark - set, get : info
bool CostumeManager::isExist(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    bool bResult = false;
    
    if ( eType == E_COSTUME::SETITEM )
    {
        if ( isExistSet(eIdx) == true )
        {
            bResult = _infoExistAura[eIdx];
        }
    }
    else
    {
        if(eIdx <= E_COSTUME_IDX::IDX_LEGEND && eType <= E_COSTUME::SETITEM )
        {
            if ( _infoExist[eIdx][eType] > 0 )
                bResult = true;
        }
    }

    return bResult;
}

int CostumeManager::getExist(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    int nResult = 0;
    if ( eType == E_COSTUME::SETITEM )
    {
        nResult = _infoExistAura[eIdx];
    }
    else
    {
        nResult = _infoExist[eIdx][eType];
    }
    
    return nResult;
}

void CostumeManager::setExist(E_COSTUME eType, E_COSTUME_IDX eIdx, int nValue)
{
    if ( eType == E_COSTUME::SETITEM )
    {
        _infoExistAura[eIdx] = nValue;
    }
    else
    {
        _infoExist[eIdx][eType] = nValue;
    }
}

bool CostumeManager::isEquip(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    bool bResult = false;
    if ( eType == E_COSTUME::SETITEM )
    {
        if ( _infoEquipAura == eIdx )
        {
            bResult = true;
        }
    }
    else
    {
        if ( _infoEquip[eType] == eIdx )
        {
            bResult = true;
        }
        else
        {   //jewel 5투구,6갑옷,7망토,8장갑,9신발
            if ( isExist(eType, eIdx) == true ) //장착은 하지 않아도 가지고는 있어야 한다.
                bResult = true;
            
            if ( isRent(eType, eIdx) == true ) //대여로 보유
                bResult = true;
        }
    }
    
    return bResult;
}

int CostumeManager::getEquip(E_COSTUME eType)
{
    int nResult = 0;
    if ( eType == E_COSTUME::SETITEM )
    {
        nResult = _infoEquipAura;
    }
    else
    {
        nResult = _infoEquip[eType];
    }
    
    return nResult;
}

void CostumeManager::setEquip(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if ( eIdx == 0 )
    {
        return;
    }
    
    if ( eType == E_COSTUME::SETITEM )
    {
        _infoEquipAura = eIdx;
    }
    else
    {
        _infoEquip[eType] = eIdx;
    }
}

bool CostumeManager::isRent(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    bool bResult = false;
    if ( eType == E_COSTUME::SETITEM )
    {
        bResult = false;
    }
    else
    {
        if ( getRent(eType) == eIdx && isExist(eType, eIdx) == false )
        {// 소유하고 있으면 랜트가 아님.
            bResult = true;
        }
    }
    
    return bResult;
}
int CostumeManager::getRent(E_COSTUME eType)
{
    int nResult = 0;
    if ( eType == E_COSTUME::SETITEM )
    {
        nResult = 0;
    }
    else
    {
        nResult = _infoRent[eType];
    }
    return nResult;
}
void CostumeManager::setRent(E_COSTUME eType, E_COSTUME_IDX eIdx, bool bSave)
{
    _infoRent[eType] = eIdx;
    if ( bSave == true )
    {
        SaveManager::saveCostumeRentList();
    }
}
int CostumeManager::getRentTime(E_COSTUME eType)
{
    int nResult = 0;
    if ( eType == E_COSTUME::SETITEM )
    {
        nResult = 0;
    }
    else
    {
        nResult = _infoRentTime[eType];
    }
    return nResult;
}
void CostumeManager::setRentTime(E_COSTUME eType, int64_t nTime, bool bSave)
{
    _infoRentTime[eType] = (int)nTime;
    if ( bSave == true )
    {
        SaveManager::saveCostumeRentTime();
    }
}

int CostumeManager::getHonorLevel(E_HONOR eHonor)
{
    int nIdx = (int)eHonor;
    nIdx -= 1000;
    
    int nResult = _infoHonor[nIdx];
    return nResult;
}

void CostumeManager::setHonorLevel(E_HONOR eHonor, int nLevel, bool bSave)
{
    int nIdx = (int)eHonor;
    nIdx -= 1000;
    
    _infoHonor[nIdx] = nLevel;
    
    if(bSave)
    {
        std::vector<std::string> vec;
        for (int i=E_HONOR::HONOR_1; i<=E_HONOR::HONOR_3; i++)
        {
            vec.push_back(MafUtils::toString(getHonorLevel((E_HONOR)i)));
        }
        std::string str = MafUtils::splitCreate(vec, ',');
        UserDefault::getInstance()->setStringForKey(KEY_HONOR, MafAes256::Encrypt(str));
    }
}

#pragma mark - set, get : other
int CostumeManager::getAdsCount()
{
    return _nAdsCount;
}

std::string CostumeManager::getIconPath(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    std::string strResult = "";
    switch (eType) {
        case E_COSTUME::BODY:       strResult = "Assets/icon_costume/body_%02d.png";   break;
        case E_COSTUME::HEAD:       strResult = "Assets/icon_costume/head_%02d.png";   break;
        case E_COSTUME::HAND:       strResult = "Assets/icon_costume/hand_%02d.png";   break;
        case E_COSTUME::FOOT:       strResult = "Assets/icon_costume/foot_%02d.png";   break;
        case E_COSTUME::CAPE:       strResult = "Assets/icon_costume/cape_%02d.png";   break;
        case E_COSTUME::JEWEL:      strResult = "Assets/icon_costume/jewel_%02d.png";   break;
        default:
            break;
    }
    
    strResult = MafUtils::format(strResult.c_str(), eIdx);
    return strResult;
}

int CostumeManager::getExistTypeCount(E_COSTUME eType)
{
    int nResult = 0;
    for ( int i = 1; i <= C_COUNT_COSTUME; i++ )
    {
        if ( isExist(eType, (E_COSTUME_IDX)i) == true )
        {
            nResult++;
        }
    }
    return nResult;
}

int CostumeManager::getExistCount()
{
    int nResult = 0;
    nResult += getExistTypeCount(E_COSTUME::HEAD);
    nResult += getExistTypeCount(E_COSTUME::BODY);
    nResult += getExistTypeCount(E_COSTUME::CAPE);
    nResult += getExistTypeCount(E_COSTUME::HAND);
    nResult += getExistTypeCount(E_COSTUME::FOOT);
    nResult += getExistTypeCount(E_COSTUME::JEWEL);
    if (getHonorLevel(E_HONOR::HONOR_1) > 0) {
        nResult++;
    }
    if (getHonorLevel(E_HONOR::HONOR_2) > 0) {
        nResult++;
    }
    if (getHonorLevel(E_HONOR::HONOR_3) > 0) {
        nResult++;
    }
    
    return nResult;
}

int CostumeManager::getExistAuraCount()
{
    int nResult = 0;
    for ( int i = 1; i <= C_COUNT_COSTUME; i++ )
    {
        if ( isExist(E_COSTUME::SETITEM, (E_COSTUME_IDX)i) == true )
        {
            nResult++;
        }
    }
    return nResult;
}

int CostumeManager::getExistHonorCount()
{
    int nResult = 0;
    for (int i=E_HONOR::HONOR_1; i<=E_HONOR::HONOR_3; i++)
    {
        if (getHonorLevel((E_HONOR)i) > 0)
        {
            nResult++;
        }
    }
    
    return nResult;
}

int CostumeManager::getRentCount(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    int nResult = 0;
    if ( eType == E_COSTUME::SETITEM )
    {
        nResult = 0;
    }
    else
    {
        nResult = _infoRentCount[eType][eIdx];
    }
    
    return nResult;
}

int CostumeManager::getLegendLevel(E_COSTUME eType)
{
    int nLevel = getExist(eType, E_COSTUME_IDX::IDX_LEGEND);
    int nLevelMax = DataManager::GetCostumeLegendMaxLevel();
    
    if (nLevel > nLevelMax)
    {
        nLevel = nLevelMax;
    }
    
    return nLevel;
}
int CostumeManager::getLegendEnableLevel(E_COSTUME eType)
{
    int nResult = 1;
    
    if ( eType == E_COSTUME::JEWEL )
    {
        nResult = 5;
    }
    else
    {
        for (int i = 1; i < E_COSTUME_IDX::IDX_LEGEND; i++ )
        {
            if (isExist(eType, (E_COSTUME_IDX)i) == true )
            {
                nResult++;
            }
        }
        
        nResult -= DataManager::GetCostumeLegendTermsCount(eType);
        if ( nResult <= 0 )
        {
            nResult = 0;
        }
    }
    
    return nResult;
}
int CostumeManager::getLegendCount()
{
    int nResult = 0;
    if ( isExist(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_LEGEND) == true )
        nResult++;
    if ( isExist(E_COSTUME::BODY, E_COSTUME_IDX::IDX_LEGEND) == true )
        nResult++;
    if ( isExist(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_LEGEND) == true )
        nResult++;
    if ( isExist(E_COSTUME::HAND, E_COSTUME_IDX::IDX_LEGEND) == true )
        nResult++;
    if ( isExist(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_LEGEND) == true )
        nResult++;
    
    return nResult;
}

bool CostumeManager::isExistSet(E_COSTUME_IDX eIdx)
{
    bool bResult = false;
    if ( isExist(E_COSTUME::HEAD, eIdx) &&
         isExist(E_COSTUME::BODY, eIdx) &&
         isExist(E_COSTUME::CAPE, eIdx) &&
         isExist(E_COSTUME::HAND, eIdx) &&
         isExist(E_COSTUME::FOOT, eIdx) )
    {
        bResult = true;
    }
    
    return bResult;
}

bool CostumeManager::isExistAll()
{
    bool bResult = true;
    
    for ( int j = 1; j <= IDX_LEGEND; j++ )
    {
        if ( isExistSet((E_COSTUME_IDX)j) == false )
        {
            bResult = false;
            break;
        }
    }
    
    return bResult;
}

#pragma mark - callback


#pragma mark - network
void CostumeManager::requestInfo(const std::function<void(bool)>& callback)
{
    _callbackInfo = callback;
    
    std::string url = "/costume/v6/usercoin/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void CostumeManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
            _callbackInfo = nullptr;
        }
        return;
    }

    int nCurrencyCoin = jsonParser["_coin"].GetInt();
    int nCurrencyBong = jsonParser["_material"].GetInt();
    int nCurrencyHonorPoint = jsonParser["_honor"].GetInt();
    int nCurrentEventRaceSkinTicket = jsonParser["_race_skin_ticket"].GetInt();
    int nCurrentEventHalloweenCandy = jsonParser.HasMember("_halloween_candy") ? jsonParser["_halloween_candy"].GetInt() : 0;
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyBong);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, nCurrencyHonorPoint);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_RACE_SKIN, 0, nCurrentEventRaceSkinTicket);
    EventBoardGameManager::getInstance()->setTotalCandy(nCurrentEventHalloweenCandy);

    _nAdsCount = jsonParser["_freecount"].GetInt();

    //
    for (int i=0; i<=C_COUNT_COSTUME_TYPE; i++)
    {
        for (int j=0; j<=C_COUNT_COSTUME; j++)
        {
            _infoRentCount[i][j] = 0;
        }
    }
    for ( int i = 0; i < jsonParser["_rentlist"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_rentlist"][i];
        int nType = jsonValue["_ct"].GetInt();
        int nNum = jsonValue["_cn"].GetInt();
        
        _infoRentCount[nType][nNum]++;
    }
    
    
    //
    for ( int i = 0; i < jsonParser["_honorlist"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_honorlist"][i];
        
        int nNum = jsonValue["_hn"].GetInt() + 1000;
        int nLevel = jsonValue["_hl"].GetInt();
        
        UserInfoManager::getInstance()->setHonorLevel((E_HONOR)nNum, nLevel);
    }
    
    //
    SaveManager::saveCostume();
    
    // callback
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
        _callbackInfo = nullptr;
    }
}

void CostumeManager::requestAds(const std::function<void(bool, int)>& callback)
{
    //
    _callbackAds = callback;
    
    //
    std::string url = "/costume/v1/videocoin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAds(response,dd);
    };
    req->send(cb);
    
    //
    MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotCostumeShop, CashManager::getInstance()->isAdsfreeTime());
}

void CostumeManager::responseAds(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, -1);
            _callbackAds = nullptr;
        }
        return;
    }
    
    _nAdsCount--;
    if ( _nAdsCount < 0 )
    {
        _nAdsCount = 0;
    }
    

    int nCurrencyCoin = jsonParser["_coin"].GetInt();
    
    std::string strCurrencyCoin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    strCurrencyCoin =  MafUtils::bigAddNum(strCurrencyCoin, MafUtils::toString(nCurrencyCoin));
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, strCurrencyCoin);
    
    RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::COSTUME_COIN, MafUtils::toString(nCurrencyCoin), false);
    
    // callback
    if ( _callbackAds != nullptr )
    {
        _callbackAds(true, nCurrencyCoin);
        _callbackAds = nullptr;
    }
}

void CostumeManager::requestRent(int nType, int nIdx, int nPrice, const std::function<void(bool)>& callback)
{
    //
    _callbackRent = callback;
    
    //
    std::string url = "/costume/v1/rentcostume";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_price", nPrice);
    req->setData("_costumetype", nType);
    req->setData("_costumenum", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRent(response,dd);
    };
    req->send(cb);
}
void CostumeManager::responseRent(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackRent != nullptr )
        {
            _callbackRent(false);
            _callbackRent = nullptr;
        }
        return;
    }
    
    //{"_price": 45, "_costumetype": 1, "_costumenum": 14}
    int nPrice = jsonParser["_price"].GetInt();
    E_COSTUME eType = (E_COSTUME)jsonParser["_costumetype"].GetInt();
    E_COSTUME_IDX eIdx = (E_COSTUME_IDX)jsonParser["_costumenum"].GetInt();
    
    //
    CostumeManager::getInstance()->setEquip(eType, eIdx);
    CostumeManager::getInstance()->setRent(eType, eIdx);
    CostumeManager::getInstance()->setRentTime(eType, TIME_COSTUME_RENT);
    _infoRentCount[eType][eIdx]++;
    
    //
    auto nCurrencyCoin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    nCurrencyCoin = MafUtils::bigSubNum(nCurrencyCoin, MafUtils::toString(nPrice));
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
    
    //
    SaveManager::saveCostume();
    SaveManager::saveCostumeRentList();
    SaveManager::saveCostumeRentTime();
    
    UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
    //
    if (isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_15))
    {
        DataManager::getInstance()->initArtifactSaveData(DG_NORMAL);
    }
    
    //
    if ( _callbackRent != nullptr )
    {
        _callbackRent(true);
        _callbackRent = nullptr;
    }
}

void CostumeManager::requestBuy(int nType, int nIdx, const std::function<void(bool, int)>& callback)
{
    //
    _callbackBuy = callback;
    
    //
    std::string url = "/costume/v5/buycostume";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_costumetype", nType);
    req->setData("_costumenum", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuy(response,dd);
    };
    req->send(cb);
}

void CostumeManager::responseBuy(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackBuy != nullptr )
        {
            _callbackBuy(false, -1);
            _callbackBuy = nullptr;
        }
        return;
    }
    
    //{"_coin_total": 77915, "_material_total": 0, "_costumetype": 1, "_costumenum": 7, "_result": 1}
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 || nResult == 4 )
    {
        int nCurrencyCoin = jsonParser["_coin_total"].GetInt();
        int nCurrencyMaterial = jsonParser["_material_total"].GetInt();
        
        E_COSTUME eType = (E_COSTUME)jsonParser["_costumetype"].GetInt();
        E_COSTUME_IDX eIdx = (E_COSTUME_IDX)jsonParser["_costumenum"].GetInt();
        
        
        //
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyMaterial);

        //
        setEquip(eType, eIdx);
        setExist(eType, eIdx, 1);
        
        //
        if ( getRent(eType) == eIdx )
        {
            setRent(eType, (E_COSTUME_IDX)0);
            setRentTime(eType, 0);
            SaveManager::saveCostumeRentList();
            SaveManager::saveCostumeRentTime();
        }
        SaveManager::saveCostume();

        //
        int nCount = 0;
        nCount += getExistTypeCount(E_COSTUME::HEAD);
        nCount += getExistTypeCount(E_COSTUME::BODY);
        nCount += getExistTypeCount(E_COSTUME::CAPE);
        nCount += getExistTypeCount(E_COSTUME::HAND);
        nCount += getExistTypeCount(E_COSTUME::FOOT);
        nCount += getExistTypeCount(E_COSTUME::JEWEL);
        MafGooglePlay::IncrementImmediateAchievement(ACHIEVE_COSTUME_90, nCount, 90);
        
        //
        if ( isExistAll() )
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_REALENDING);
        }

        //
        if (isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_15))
        {
            DataManager::getInstance()->initArtifactSaveData(DG_NORMAL);
        }
        
        //
        if ( eIdx == E_COSTUME_IDX::IDX_10 && eType == E_COSTUME::HEAD )
        {
            auto tutorial = TutorialManager::getInstance();
            tutorial->drawTutorial();
        }
        
        //
        if ( _callbackBuy != nullptr )
        {
            _callbackBuy(true, nResult);
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_BUY);
        RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
        
        //
        switch (eType)
        {
            case E_COSTUME::HEAD: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HEAD, 1, true); break;
            case E_COSTUME::BODY: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_BODY, 1, true); break;
            case E_COSTUME::CAPE: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_CAPE, 1, true); break;
            case E_COSTUME::HAND: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HAND, 1, true); break;
            case E_COSTUME::FOOT: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_FOOT, 1, true); break;
            case E_COSTUME::SETITEM: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_SETITEM, 1, true); break;
            default:
                break;
        }
        
        
        UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_44"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
        
    }
    else
    {
        if ( _callbackBuy != nullptr )
        {
            _callbackBuy(false, nResult);
        }
    }
}

void CostumeManager::requestAuraBuy(int nType, int nIdx, const std::function<void(bool, int)>& callback)
{
    //
    _callbackAuraBuy = callback;
    
    //
    std::string url = "/costume/v2/buycostumeset";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_costumetype", nType);
    req->setData("_costumenum", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAuraBuy(response,dd);
    };
    req->send(cb);
}
void CostumeManager::responseAuraBuy(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackAuraBuy != nullptr )
        {
            _callbackAuraBuy(false, -1);
            _callbackAuraBuy = nullptr;
        }
        return;
    }
    
    // {"_total_price": 9970, "_costumetype": 7, "_costumenum": 5, "_result": 1}
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 || nResult == 4 )
    {
        int nCurrencyMaterial = jsonParser["_total_price"].GetInt();
        
        E_COSTUME eType = (E_COSTUME)jsonParser["_costumetype"].GetInt();
        E_COSTUME_IDX eIdx = (E_COSTUME_IDX)jsonParser["_costumenum"].GetInt();
        
        //
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyMaterial);

        //
        setEquip(eType, eIdx);
        setExist(eType, eIdx, 1);
        SaveManager::saveCostume();
        
        //
        if ( _callbackAuraBuy != nullptr )
        {
            _callbackAuraBuy(true, nResult);
        }
        
        //
        switch (eType)
        {
            case E_COSTUME::HEAD: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HEAD, 1, true); break;
            case E_COSTUME::BODY: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_BODY, 1, true); break;
            case E_COSTUME::CAPE: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_CAPE, 1, true); break;
            case E_COSTUME::HAND: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HAND, 1, true); break;
            case E_COSTUME::FOOT: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_FOOT, 1, true); break;
            case E_COSTUME::SETITEM: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_SETITEM, 1, true); break;
            default:
                break;
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_BUY);
    }
    else
    {
        if ( _callbackAuraBuy != nullptr )
        {
            _callbackAuraBuy(false, nResult);
        }
    }
}

void CostumeManager::requestHonorBuy(int nType, int nPrice, const std::function<void(bool)>& callback)
{
    //
    _callbackHonorBuy = callback;
    
    //
    std::string url = "/costume/v1/buyhonor";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_honor", nType);
    req->setData("_price", nPrice);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseHonorBuy(response,dd);
    };
    req->send(cb);
}
void CostumeManager::responseHonorBuy(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackHonorBuy != nullptr )
        {
            _callbackHonorBuy(false);
            _callbackHonorBuy = nullptr;
        }
        return;
    }
    
    // {"_price": 4, "_honor": 1001}
    int nPrice = jsonParser[("_price")].GetInt();
    E_HONOR eHonor = (E_HONOR)jsonParser["_honor"].GetInt();
           
    //
    UserInfoManager::getInstance()->setHonorLevel(eHonor, UserInfoManager::getInstance()->getHonorLevel(eHonor) + 1);
    SaveManager::saveCostume();
    
    //
    std::string nCurrencyHonor = ItemsManager::getInstance()->getItems(E_ITEMS::HONOR);
    nCurrencyHonor = MafUtils::bigSubNum(nCurrencyHonor, MafUtils::toString(nPrice));
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, nCurrencyHonor);

    //
    if ( _callbackHonorBuy != nullptr )
    {
        _callbackHonorBuy(true);
        _callbackHonorBuy = nullptr;
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_BUY);
}

void CostumeManager::requestLegendBuy(int nType, int nIdx, int nPrice, int nPriceBong, const std::function<void(bool)>& callback )
{
    //
    _callbackLegendBuy = callback;
    
    //
    std::string url = "/costume/v1/buycostumelegend";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_costumetype", nType);
    req->setData("_costumenum", nIdx);
    req->setData("_price", nPrice);
    req->setData("_material", nPriceBong);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLegendBuy(response,dd);
    };
    req->send(cb);
}
void CostumeManager::responseLegendBuy(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackLegendBuy != nullptr )
        {
            _callbackLegendBuy(false);
            _callbackLegendBuy = nullptr;
        }
        return;
    }
    
    // {"_price": 400, "_material": 20, "_costumetype": 1, "_costumenum": 40}
    int nPrice = jsonParser["_price"].GetInt();
    int nPriceBong = jsonParser["_material"].GetInt();
    
    E_COSTUME eType = (E_COSTUME)jsonParser["_costumetype"].GetInt();
    E_COSTUME_IDX eIdx = (E_COSTUME_IDX)jsonParser["_costumenum"].GetInt();
    
    std::string nCurrencyCoin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    nCurrencyCoin = MafUtils::bigSubNum(nCurrencyCoin, MafUtils::toString(nPrice));
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
    
    std::string nCurrencyBong = ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL);
    nCurrencyBong = MafUtils::bigSubNum(nCurrencyBong, MafUtils::toString(nPriceBong));
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyBong);
    
    //
    int nLevel = getExist(eType, eIdx) + 1;
    setExist(eType, eIdx, nLevel);
    setEquip(eType, eIdx);
    SaveManager::saveCostume();
    
    //
    if ( isExistAll() )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_REALENDING);
    }
    
    //
    if ( _callbackLegendBuy != nullptr )
    {
        _callbackLegendBuy(true);
        _callbackLegendBuy = nullptr;
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_BUY);
}
