//
//  LootsManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#include "LootsManager.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

LootsManager::LootsManager(void) :

//
_callbackInfo(nullptr),
_callbackLevelUp(nullptr),
_callbackSynthesize(nullptr)
{
    
}

LootsManager::~LootsManager(void)
{
    
}

bool LootsManager::init()
{
    
    return true;
}

#pragma mark - load
void LootsManager::setLoad()
{
    setLoadLoots();
}

void LootsManager::setLoadLoots()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EXPEDITION_LOOT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoLoots.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int idx = jsonValue["idx"].GetInt();
        int grade = jsonValue["grade"].GetInt();
        
        std::string nameKey = jsonValue["name"].GetString();
        std::string descKey = jsonValue["desc"].GetString();
        
        double effectDefault = jsonValue["effect_default"].GetDouble();
        double effectIncreasing = jsonValue["effect_increasing"].GetDouble();
        
        
        int levelMax = 1;
        if ( jsonValue.HasMember("max_lv") == true )
        {
            levelMax = jsonValue["max_lv"].GetInt();
        }
        
        //
        auto obj = InfoLoot::create();
        obj->setIdx(idx);
        obj->setGrade(grade);
        
        obj->setNameKey(nameKey);
        obj->setDescKey(descKey);
        
        obj->setEffectDefault(effectDefault);
        obj->setEffectIncreasing(effectIncreasing);
        
        
        obj->setLevelMax(levelMax);
        
        _listInfoLoots.pushBack(obj);
    }
}

#pragma mark - info
Vector<InfoLoot*> LootsManager::getListInfoLoots()
{
    return _listInfoLoots;
}

InfoLoot* LootsManager::getInfoLoot(int nIdx)
{
    InfoLoot* result = nullptr;
    
    for(auto info : _listInfoLoots)
    {
        if(info->getIdx() == nIdx)
        {
            result = info;
            break;
        }
    }
    
    return result;
}

#pragma mark - set, get :
double LootsManager::getEffect()
{
    double result = 0;
    
    for ( auto& info : _listInfoLoots )
    {
        int level = getLootLevelNow(info->getIdx());
        if ( level <= 0 )
            continue;
        
        result += info->getEffectDefault() + (info->getEffectIncreasing() * (level - 1));
    }
    
    return result;
}

int LootsManager::getLootCountNow(int idx)
{
    int count = 0;
    
    auto iter = _listLootCount.find(idx);
    if ( iter != _listLootCount.end() )
    {
        count = _listLootCount[idx].valueInt();
    }
    
    return count;
}

int LootsManager::getLootCountMax(int grade, int level)
{
    int increasing = 0;
    switch (grade) {
        case 1: increasing = 2; break;
        case 2: increasing = 4; break;
        case 3: increasing = 6; break;
        case 4: increasing = 8; break;
        case 5: increasing = 10; break;
            
        default:
            break;
    }
    
    return (level - 1) * increasing;
}

int LootsManager::getLootLevelNow(int idx)
{
    int level = 0;
    
    auto iter = _listLootLevel.find(idx);
    if ( iter != _listLootLevel.end() )
    {
        level = _listLootLevel[idx].valueInt();
    }
    
    return level;
}

bool LootsManager::isLootLevelUPPossible()
{
    bool result = false;
    
    for ( auto& info : _listInfoLoots )
    {
        int levelNow = getLootLevelNow(info->getIdx());
        if ( levelNow <= 0 )
            continue;
        
        int countNow = getLootCountNow(info->getIdx());
        int countMax = getLootCountMax(info->getGrade(), levelNow + 1);
        if ( countNow >= countMax )
        {
            result = true;
            break;
        }
    }
    
    return result;
}

E_ITEMS LootsManager::getGradeToItemsType(int grade)
{
    E_ITEMS eType = E_ITEMS::NONE;
    switch (grade) {
        case 1: eType = E_ITEMS::LOOT_PIECE_1; break;
        case 2: eType = E_ITEMS::LOOT_PIECE_2; break;
        case 3: eType = E_ITEMS::LOOT_PIECE_3; break;
        case 4: eType = E_ITEMS::LOOT_PIECE_4; break;
        case 5: eType = E_ITEMS::LOOT_PIECE_5; break;
        default:
            break;
    }
    
    return eType;
}

#pragma mark - network
void LootsManager::requestLootsInfo(bool bPopupLoading, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/expedition/cat/loot/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLootsInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void LootsManager::responseLootsInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
       
    if ( bError == true )
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    _listLootCount.clear();
    _listLootLevel.clear();
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        for ( int i = 0; i < jsonParser["_catlootdata"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_catlootdata"][i];
            
            int idx = jsonValue["idx"].GetInt();
            _listLootCount[idx] = jsonValue["count"].GetInt();
            _listLootLevel[idx] = jsonValue["level"].GetInt();
        }
         
        if ( jsonParser.HasMember("_items") == true )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        //
        RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_LOOTS_LEVELUP, isLootLevelUPPossible());
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_LOOTS_LEVELUP);
        
        //
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
            _callbackInfo = nullptr;
        }
    }
}

void LootsManager::requestLootsLevelUp(bool bPopupLoading, int idx, int levelNow, const std::function<void(bool,int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackLevelUp = callback;
    
    //
    std::string url = "/v1/expedition/cat/loot/levelup";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", idx);
    req->setData("_level", levelNow);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLootsLevelUp(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void LootsManager::responseLootsLevelUp(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
       
    if ( bError == true )
    {
        if ( _callbackLevelUp != nullptr )
        {
            _callbackLevelUp(false, -1);
            _callbackLevelUp = nullptr;
        }
        return;
    }
    
    /*
     0 # 재화 부족
     1 # 성공
     3 # 레벨 정보 다름
     4 # max 레벨이거나 레벨정보가 없음
    */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackLevelUp != nullptr )
        {
            _callbackLevelUp(false, nResult);
            _callbackLevelUp = nullptr;
        }
        return;
    }
    
    //
    int idx = jsonParser["_idx"].GetInt();
    _listLootCount[idx] = jsonParser["_count"].GetInt();
    _listLootLevel[idx] = jsonParser["_level"].GetInt();
     
    if ( jsonParser.HasMember("_items") == true )
    {
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    //
    if ( _callbackLevelUp != nullptr )
    {
        _callbackLevelUp(true, nResult);
        _callbackLevelUp = nullptr;
    }
    
}

void LootsManager::requestLootPieceSynthesize(bool bPopupLoading, int grade, int count, const std::function<void(bool,int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackSynthesize = callback;
    
    int itemIdx = (int)getGradeToItemsType(grade);
    
    //
    std::string url = "/expedition/cat/loot/combine";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", itemIdx);
    req->setData("_grade", grade);
    req->setData("_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLootPieceSynthesize(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void LootsManager::responseLootPieceSynthesize(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
       
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackSynthesize != nullptr )
        {
            _callbackSynthesize(false, -1);
            _callbackSynthesize = nullptr;
        }
        return;
    }
    
    /*
     0 # 재화 부족
     1 # 성공
     4 : 필요 개수 다름
     5 : 데이터 확률 문제있음
    */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        
        Vector<InfoItems*> listItemsAll = ItemsManager::getInstance()->getConvertArray(strReward);
        Vector<InfoItems*> listItems;
        for ( int j = 0; j < listItemsAll.size(); j++ )
        {
            auto objItem = listItemsAll.at(j);

            bool bHave = false;
            for ( auto& obj : listItems )
            {
                if ( obj->getTypeParams() == objItem->getTypeParams() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                listItems.pushBack(objItemClone);
            }
        }
        ItemsManager::getInstance()->addItems(listItems);
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if ( _callbackSynthesize != nullptr )
        {
            _callbackSynthesize(true, nResult);
            _callbackSynthesize = nullptr;
        }
    }
    else
    {
        if ( _callbackSynthesize != nullptr )
        {
            _callbackSynthesize(false, nResult);
            _callbackSynthesize = nullptr;
        }
    }
}


#pragma mark - event
int LootsManager::onLootsLevelUp(int idx, const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 최대 레벨
     */
    
    auto objLoot = getInfoLoot(idx);
    
    int levelMax = objLoot->getLevelMax();
    int levelNow = getLootLevelNow(idx);
    if ( levelNow <= 0 || levelNow >= levelMax )
    {
        return 2;
    }
    
    int pieceMax = getLootCountMax(objLoot->getGrade(), levelNow + 1);
    int pieceNow = getLootCountNow(idx);
    if ( pieceMax <= 0 || pieceNow < pieceMax )
    {
        return 1;
    }
    
    // network
    requestLootsLevelUp(true, idx, levelNow, callback);
    
    return result;
}

int LootsManager::onLootPieceSynthesize(int grade, int count, const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     */
    
    int itemNow = count;
    auto itemMax = std::atoi(ItemsManager::getInstance()->getItems(getGradeToItemsType(grade)).c_str());
    itemMax = MIN(itemMax, 300);
    
    if ( itemNow <= 0 || itemNow > itemMax )
    {
        return 1;
    }
    
    // network
    requestLootPieceSynthesize(true, grade, count, callback);
    
    return result;
}

     
