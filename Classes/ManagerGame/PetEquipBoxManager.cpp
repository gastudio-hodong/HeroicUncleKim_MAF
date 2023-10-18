//
//  PetEquipBoxManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 28/06/2019.
//

#include "PetEquipBoxManager.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

PetEquipBoxManager::PetEquipBoxManager() :
_nEquipBoxCount(0),
_nEquipBoxEventCount(0),
_nEquipBoxSpecialCount(0),
_nEquipBoxOpenHonor(0),
_nEquipBoxOpenBong(0),
_nEquipBoxOpenDevil(0),
_nEquipBoxOpenFree(0),
_nEquipBoxOpenCash(0),

_nCostDevil(0),
_nCostHonor(0),
_nCostBong(0),
_nCostEventDevil(0),
_nCostEventHonor(0),
_nCostEventBong(0),
_nEquipBoxMileage(0),

_callbackEquipBoxInfo(nullptr),
_callbackEquipBoxBuy(nullptr),
_callbackEquipBoxUse(nullptr)
{
    
}

PetEquipBoxManager::~PetEquipBoxManager(void)
{
    
}

bool PetEquipBoxManager::init()
{
    return true;
}

#pragma mark - load
void PetEquipBoxManager::setLoad()
{
    setLoadPetBoxShop();
}

void PetEquipBoxManager::setLoadPetBoxShop()
{
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::PET_EQUIP_BOX_SHOP);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        E_PET_EQUIP_BUY nIdx = (E_PET_EQUIP_BUY)jsonValue["idx"].GetInt();
        int nCostCount = jsonValue["buy_cost_count"].GetInt();
        int nEventCostCount = jsonValue["event_cost"].GetInt();
        
        switch (nIdx) {
            case E_PET_EQUIP_BUY::BONG:
            {
                _nCostBong = nCostCount;
                _nCostEventBong = nEventCostCount;
                break;
            }
                
            case E_PET_EQUIP_BUY::COIN:
            {
                _nCostDevil = nCostCount;
                _nCostEventDevil = nEventCostCount;
                break;
            }
                
            case E_PET_EQUIP_BUY::HONOR:
            {
                _nCostHonor = nCostCount;
                _nCostEventHonor = nEventCostCount;
                break;
            }
        }
    }
}

#pragma mark - pet equip : box
int PetEquipBoxManager::getEquipBoxCount()
{
    return _nEquipBoxCount;
}
void PetEquipBoxManager::setEquipBoxCount(int nCount)
{
    _nEquipBoxCount = nCount;
}

int PetEquipBoxManager::getEquipBoxEventCount()
{
    return _nEquipBoxEventCount;
}

int PetEquipBoxManager::getEquipBoxSpecialCount()
{
    return _nEquipBoxSpecialCount;
}
void PetEquipBoxManager::setEquipBoxSpecialCount(int nCount)
{
    _nEquipBoxSpecialCount = nCount;
}

int PetEquipBoxManager::getEquipBoxMileage()
{
    return _nEquipBoxMileage;
}

int PetEquipBoxManager::getEquipBoxOpenHonor()
{
    return _nEquipBoxOpenHonor;
}
int PetEquipBoxManager::getEquipBoxOpenBong()
{
    return _nEquipBoxOpenBong;
}
int PetEquipBoxManager::getEquipBoxOpenDevil()
{
    return _nEquipBoxOpenDevil;
}
int PetEquipBoxManager::getEquipBoxOpenFree()
{
    return _nEquipBoxOpenFree;
}
int PetEquipBoxManager::getEquipBoxOpenCash()
{
    return _nEquipBoxOpenCash;
}

int PetEquipBoxManager::getPetEquipFreeBoxCount()
{
    return ParameterManager::getInstance()->getParameterInteger("pet_equip_free_box_count");
}

int PetEquipBoxManager::getEquipBoxOpenDevilMax()
{
    int result = 5;
    
    if(EventManager::getInstance()->isActivatedEvent(16) == true)
        result = 20;
    
    return result;
}

int PetEquipBoxManager::getCost(E_PET_EQUIP_BUY eType, bool isEvent)
{
    unsigned long long result = 0;
    
    switch (eType)
    {
        case E_PET_EQUIP_BUY::BONG:
        {
            result = getCostData(eType, isEvent);
            break;
        }
        case E_PET_EQUIP_BUY::COIN:
        {
            result = getCostData(eType, isEvent);
            break;
        }
        case E_PET_EQUIP_BUY::HONOR:
        {
            result = getCostData(eType, isEvent) * std::pow(2, getEquipBoxOpenHonor());
            if ( result > 2560 )
            {
                result = 2560;
            }
        }
            break;
    }
    
    
    return (int)result;
}

int PetEquipBoxManager::getCostData(E_PET_EQUIP_BUY eType, bool isEvent)
{
    unsigned long long result = 0;
    
    switch (eType)
    {
        case E_PET_EQUIP_BUY::BONG:
        {
            if(isEvent)
                result = _nCostEventBong;
            else
                result = _nCostBong;
            break;
        }
        case E_PET_EQUIP_BUY::COIN:
        {
            if(isEvent)
                result = _nCostEventDevil;
            else
                result = _nCostDevil;
            break;
        }
        case E_PET_EQUIP_BUY::HONOR:
        {
            if(isEvent)
                result = _nCostEventHonor;
            else
                result = _nCostHonor;
            break;
        }
    }
    
    return (int)result;
}

#pragma mark - network
void PetEquipBoxManager::requestEquipBoxInfo(const std::function<void(bool)>& callback)
{
    _callbackEquipBoxInfo = callback;
    
    //
    std::string url = "/pet/equip/v1/boxinfo/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseEquipBoxInfo(response,dd);
    };
    req->send(cb);
}

void PetEquipBoxManager::responseEquipBoxInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackEquipBoxInfo != nullptr )
        {
            _callbackEquipBoxInfo(false);
            _callbackEquipBoxInfo = nullptr;
        }
        return;
    }

    auto nCurrencyCoin = jsonParser["_total_devil"].GetInt();
    auto nCurrencyBong = jsonParser["_total_bong"].GetInt();
    auto nCurrencyHonorPoint = jsonParser["_total_honor"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyBong);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, nCurrencyHonorPoint);
    
    _nEquipBoxCount = jsonParser["_count"].GetInt();
    _nEquipBoxEventCount = jsonParser["_event_count"].GetInt();
    _nEquipBoxSpecialCount = jsonParser["_cash_count"].GetInt();
    _nEquipBoxMileage = jsonParser[("_mileage")].GetInt();
    
    _nEquipBoxOpenHonor = jsonParser["_buy_honor"].GetInt();
    _nEquipBoxOpenBong = jsonParser["_buy_bong"].GetInt();
    _nEquipBoxOpenDevil = jsonParser["_buy_devil"].GetInt();
    _nEquipBoxOpenFree = jsonParser["_free_count"].GetInt();
    _nEquipBoxOpenCash = jsonParser[("_buy_cash")].GetInt();

    // callback
    if ( _callbackEquipBoxInfo != nullptr )
    {
        _callbackEquipBoxInfo(true);
        _callbackEquipBoxInfo = nullptr;
    }
    
    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_BOX);
}

void PetEquipBoxManager::requestEquipBoxBuy(int nType, const std::function<void(bool, int, int)>& callback)
{
    _callbackEquipBoxBuy = callback;
    
    //
    std::string url = "/pet/equip/v2/buybox";

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nType);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseEquipBoxBuy(response, dd, nType);
    };
    req->send(cb);
}

void PetEquipBoxManager::responseEquipBoxBuy(cocos2d::network::HttpResponse* response,std::string &data, int nType)
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
        if ( _callbackEquipBoxBuy != nullptr )
        {
            _callbackEquipBoxBuy(false, -1, -1);
            _callbackEquipBoxBuy = nullptr;
        }
        return;
    }
    
    /*
    _RESULT_FAILURE = 0 # 코인 없음
    _RESULT_SUCCESS = 1 # 구매성공
    _RESULT_MAX_BOX = 2 # 상자 소유 개수 MAX
    _RESULT_MAX_BUY = 3 # 구매 max 초과
    _RESULT_NOT_CONDITION = 4 # 구매가능조건이 아님 ( free )
    _RESULT_NOT_TYPE = 5 # 구입가능한 타입이 없음
    */
    // {"_buy_honor": 1, "_buy_bong": 0, "_buy_devil": 0, "_free_count": 0, "_total_box": 7, "_total_event_box": 0, "_total_count": 9920, "_result": 1}
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strAmount = MafUtils::toString(jsonParser[("_total_count")].GetInt());
        
        _nEquipBoxCount = jsonParser[("_total_box")].GetInt();
        _nEquipBoxEventCount = jsonParser[("_total_event_box")].GetInt();
        _nEquipBoxSpecialCount = jsonParser["_cash_count"].GetInt();
        
        _nEquipBoxOpenHonor = jsonParser[("_buy_honor")].GetInt();
        _nEquipBoxOpenBong = jsonParser[("_buy_bong")].GetInt();
        _nEquipBoxOpenDevil = jsonParser[("_buy_devil")].GetInt();
        _nEquipBoxOpenFree = jsonParser[("_free_count")].GetInt();
        _nEquipBoxOpenCash = jsonParser[("_buy_cash")].GetInt();
        
        if( (E_PET_EQUIP_BUY)nType == E_PET_EQUIP_BUY::HONOR)
        {
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, strAmount);
        }
        else if( (E_PET_EQUIP_BUY)nType == E_PET_EQUIP_BUY::BONG)
        {
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, strAmount);
        }
        else if( (E_PET_EQUIP_BUY)nType == E_PET_EQUIP_BUY::COIN)
        {
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, strAmount);
        }
    
        //
        auto callback = _callbackEquipBoxBuy;
        _callbackEquipBoxBuy = nullptr;
        
        if ( callback != nullptr )
        {
            callback(true, nResult, nType);
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_EQUIP);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_BOX);
    }
    else
    {
        if ( _callbackEquipBoxBuy != nullptr )
        {
            _callbackEquipBoxBuy(false, nResult, -1);
            _callbackEquipBoxBuy = nullptr;
        }
    }
}

void PetEquipBoxManager::requestEquipBoxUse(int nUse, int nType, const std::function<void(bool, int, cocos2d::Vector<InfoPetEquipDraw*>, cocos2d::Vector<InfoPetEquipDrawGrade*>)>& callback)
{
    _callbackEquipBoxUse = callback;
    
    //
    std::string url = "/pet/equip/v2/usebox";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_count", nUse);
    req->setData("_type", nType);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseEquipBoxUse(response, dd, nUse);
    };
    req->send(cb);
}
void PetEquipBoxManager::responseEquipBoxUse(cocos2d::network::HttpResponse* response,std::string &data, int nUse)
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
        if ( _callbackEquipBoxUse != nullptr )
        {
            cocos2d::Vector<InfoPetEquipDraw*> listDraw;
            cocos2d::Vector<InfoPetEquipDrawGrade*> lisDrawGrade;
            _callbackEquipBoxUse(false, -1, listDraw, lisDrawGrade);
            _callbackEquipBoxUse = nullptr;
        }
        return;
    }
    
    // {"_equipdata": [{"_equipidx": 100, "_count": 6}, {"_equipidx": 702, "_count": 6}], "_result_event_box_grade": [], "_use_box": 1, "_total_box": 14, "_total_event_box": 0, "_result": true}
    int bResult = jsonParser["_result"].GetInt();
    
    if(bResult != 1)
    {
        if ( _callbackEquipBoxUse != nullptr )
        {
            cocos2d::Vector<InfoPetEquipDraw*> listDraw;
            cocos2d::Vector<InfoPetEquipDrawGrade*> lisDrawGrade;
            _callbackEquipBoxUse(false, -1, listDraw, lisDrawGrade);
            _callbackEquipBoxUse = nullptr;
        }
    }
    else
    {
        _nEquipBoxCount = jsonParser[("_total_box")].GetInt();
        _nEquipBoxEventCount = jsonParser[("_total_event_box")].GetInt();
        _nEquipBoxSpecialCount = jsonParser[("_cash_count")].GetInt();
        _nEquipBoxMileage = jsonParser[("_mileage")].GetInt();
        

        cocos2d::Vector<InfoPetEquipDraw*> listDraw;
        for ( int i = 0; i < jsonParser["_equipdata"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_equipdata"][i];
            
            auto nRewardIdx = jsonValue["_equipidx"].GetInt();
            auto nRewardCount = jsonValue["_count"].GetInt();
            
            
            int nEquipAmount = PetNewManager::getInstance()->getPetEquipAmount(nRewardIdx);
            PetNewManager::getInstance()->setPetEquipAmount(nRewardIdx, nEquipAmount + nRewardCount);
            
            if ( PetNewManager::getInstance()->getPetEquipLevel(nRewardIdx) == 0 )
            {
                PetNewManager::getInstance()->setPetEquipLevel(nRewardIdx, 1);
            }
            

            auto obj = InfoPetEquipDraw::create();
            obj->setIdx(nRewardIdx);
            obj->setCount(nRewardCount);
            listDraw.pushBack(obj);
        }
        
        cocos2d::Vector<InfoPetEquipDrawGrade*> lisDrawGrade;
        for ( int i = 0; i < jsonParser["_result_box_grade"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_result_box_grade"][i];
            
            auto nDrawIdx = jsonValue["idx"].GetInt();
            auto nDrawCount = jsonValue["grade"].GetInt();
            
            auto obj = InfoPetEquipDrawGrade::create();
            obj->setIdx(nDrawIdx);
            obj->setGrade(nDrawCount);
            lisDrawGrade.pushBack(obj);
        }

        //
        if ( _callbackEquipBoxUse != nullptr )
        {
            _callbackEquipBoxUse(true, nUse, listDraw, lisDrawGrade);
            _callbackEquipBoxUse = nullptr;
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_EQUIP);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_BOX);
    }
}
 

 
