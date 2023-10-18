//
//  BadgeManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "BadgeManager.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

BadgeManager::BadgeManager(void) :

_callbackInfo(nullptr),
_callbackUpdate(nullptr)
{
    
}

BadgeManager::~BadgeManager(void)
{
    
}

bool BadgeManager::init()
{
    // load
    setLoad();
    
    return true;
}

void BadgeManager::setLoad()
{
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::BADGE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listBadgeAll.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["idx"].GetInt();
        int nIdxSub = jsonValue.HasMember("idx_sub") == true ? jsonValue["idx_sub"].GetInt() : 0;
        
        std::string strNameKey = jsonValue["name_key"].GetString();
        std::string strDescKey = jsonValue["comment_key"].GetString();
        std::string strPath = jsonValue["path"].GetString();
        std::string strGroup = jsonValue.HasMember("group") == true ? jsonValue["group"].GetString() : "";
        std::string strVersion = jsonValue["version"].GetString();
        E_BADGE_BUFF nBuffType = (E_BADGE_BUFF)jsonValue["buff_type"].GetInt();
        float nBuffCount = jsonValue["buff_count"].GetFloat();
        
        auto obj = InfoBadge::create();
        obj->setIdx(nIdx);
        obj->setIdxSub(nIdxSub);
        obj->setNameKey(strNameKey);
        obj->setDescKey(strDescKey);
        obj->setPath(strPath);
        obj->setGroup(strGroup);
        obj->setBuffType(nBuffType);
        obj->setBuffCount(nBuffCount);
        
        _listBadgeAll.pushBack(obj);
    }
}

#pragma mark - set, get : list
cocos2d::Vector<InfoBadge*> BadgeManager::getListAll()
{
    return _listBadgeAll;
}

cocos2d::Vector<InfoBadgeHave*> BadgeManager::getListHave()
{
    return _listBadgeHave;
}

cocos2d::Vector<InfoBadgeHave*> BadgeManager::getListSelect()
{
    cocos2d::Vector<InfoBadgeHave*> listSelect;
    for ( auto iter = _listBadgeSelect.begin(); iter != _listBadgeSelect.end(); ++iter )
    {
        auto objBadgeHave = getBadgeHave(iter->second);
        if ( objBadgeHave != nullptr )
        {
            listSelect.pushBack(objBadgeHave);
        }
    }
    
    return listSelect;
}

std::vector<std::string> BadgeManager::getListGroup()
{
    std::vector<std::string> listGroup;
    
    std::string strGroupList = ParameterManager::getInstance()->getParameterString("badge_group_list");
    listGroup = MafUtils::split(strGroupList, ',');
    
    return listGroup;
}

cocos2d::Vector<InfoBadge*> BadgeManager::getListFromGroup(std::string strGroup)
{
    cocos2d::Vector<InfoBadge*> listSelect;
    listSelect.clear();
    
    for(auto info : _listBadgeAll)
    {
        if (info->getGroup().compare(strGroup) != 0)
        {
            continue;
        }
        
        auto listHaveBadge = getListHaveFromBadgeIdx(info->getIdx());
        if (listHaveBadge.size() <=0)
            continue;

        listSelect.pushBack(info);
    }
    
    // sort
    std::sort(listSelect.begin(), listSelect.end(), [](InfoBadge *a, InfoBadge *b) -> bool{
        if (a->getIdxSub() == b->getIdxSub())
        {
            return a->getIdx() < b->getIdx();
        }
        
        return a->getIdxSub() < b->getIdxSub();
    });
    
    return listSelect;
}

cocos2d::Vector<InfoBadgeHave*> BadgeManager::getListHaveFromBadgeIdx(int nBadgeIdx)
{
    cocos2d::Vector<InfoBadgeHave*> listSelect;
    listSelect.clear();
    
    for(auto info : _listBadgeHave)
    {
        if(info->getIdxBadge() == nBadgeIdx)
            listSelect.pushBack(info);
    }
    
    return listSelect;
}


cocos2d::Vector<InfoBadgeHave*> BadgeManager::getListHaveFromBuffType(E_BADGE_BUFF type)
{
    cocos2d::Vector<InfoBadgeHave*> listSelect;
    listSelect.clear();
    
    for(auto info : _listBadgeHave)
    {
        if(info->getBuffType() == type)
            listSelect.pushBack(info);
    }
    
    return listSelect;
}

void BadgeManager::setRewardBadge(int nIdx)
{
    auto obj = InfoBadgeHave::create();
    
    obj->setIdx(_listBadgeHave.size() + 1);
    
    obj->setIdxBadge(nIdx);
    
    _listBadgeHave.pushBack(obj);
}

#pragma mark - set, get : other
InfoBadge* BadgeManager::getBadge(int nIdx)
{
    InfoBadge* objBadge = nullptr;
    
    for ( auto obj : _listBadgeAll )
    {
        if ( obj->getIdx() == nIdx )
        {
            objBadge = obj;
            break;
        }
    }
    
    return objBadge;
}

InfoBadgeHave* BadgeManager::getBadgeHave(int nIdx)
{
    InfoBadgeHave* objBadgeHave = nullptr;
    
    for ( auto obj : _listBadgeHave )
    {
        if ( obj->getIdx() == nIdx )
        {
            objBadgeHave = obj;
            break;
        }
    }
    
    return objBadgeHave;
}

float BadgeManager::getBadgeBuffCount(E_BADGE_BUFF type, int nBadgeIdx/* = 0*/)
{
    float result = 0;
    
    auto list = getListHaveFromBuffType(type);
    
    for(auto info : list)
    {
        if (nBadgeIdx == 0 || nBadgeIdx == info->getIdxBadge() )
        {
            result += info->getBuffCount();
        }
    }
    
    return result;
}

#pragma mark - network : info
void BadgeManager::requestInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/badge/read/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void BadgeManager::responseInfo(network::HttpResponse* response,std::string &data)
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
        }
        return;
    }
    
    _listBadgeSelect[1] = jsonParser["_badge_slot"]["slot1"].GetInt();
    _listBadgeSelect[2] = jsonParser["_badge_slot"]["slot2"].GetInt();
    _listBadgeSelect[3] = jsonParser["_badge_slot"]["slot3"].GetInt();
    _listBadgeSelect[4] = jsonParser["_badge_slot"]["slot4"].GetInt();
    _listBadgeSelect[5] = jsonParser["_badge_slot"]["slot5"].GetInt();
    
    _listBadgeHave.clear();
    for ( int i = 0; i < jsonParser["_badge_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_badge_list"][i];
        
        int nIdx = jsonValue["idx"].GetInt();
        
        int nIdxBadge = jsonValue["badgeidx"].GetInt();
        int nIdxSeason = jsonValue["seasonidx"].GetInt();
        int64_t nCreated = jsonValue["created"].GetInt64();
    
        //
        auto obj = InfoBadgeHave::create();
        obj->setIdx(nIdx);
        obj->setIdxBadge(nIdxBadge);
        obj->setIdxSeason(nIdxSeason);
        obj->setCreated(nCreated);
        
        _listBadgeHave.pushBack(obj);
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::BADGE);
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
    }
}

void BadgeManager::requestUpdate(std::vector<int> listBadge, const std::function<void(bool)>& pCallback)
{
    if ( listBadge.size() != 5 )
    {
        if ( pCallback != nullptr )
        {
            pCallback(false);
        }
        return;
    }
    
    //
    _callbackUpdate = pCallback;
    
    //
    std::string url = "/badge/slot/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_slot_list", listBadge);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUpdate(response,dd);
    };
    req->send(cb);
}

void BadgeManager::responseUpdate(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(false);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _listBadgeSelect[1] = jsonParser["_badge_slot"]["slot1"].GetInt();
        _listBadgeSelect[2] = jsonParser["_badge_slot"]["slot2"].GetInt();
        _listBadgeSelect[3] = jsonParser["_badge_slot"]["slot3"].GetInt();
        _listBadgeSelect[4] = jsonParser["_badge_slot"]["slot4"].GetInt();
        _listBadgeSelect[5] = jsonParser["_badge_slot"]["slot5"].GetInt();
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::BADGE);
        
        // callback
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(true);
        }
    }
    else
    {
        // callback
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(false);
        }
    }
}
