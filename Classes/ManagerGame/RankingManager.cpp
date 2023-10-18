//
//  RankingManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/13.
//

#include "RankingManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"

#pragma mark -sort

static bool sortUserRank(RankInfo *a, RankInfo *b)
{
    return a->getRank() < b->getRank();
}

#pragma mark -Info
RankInfo::RankInfo():
_eLocalType(E_RANK_LOCAL_TYPE::NONE)
,_eType(E_RANK_TYPE::NONE)
,_rank(0)
,_score(0)
,_nick("")
,_platform("")
,_userIdx(0)
,_weaponIdx(0)
,_skinIdx(0)
,_tier(0)
,_percentage(0)
,_eFluctuation(E_RANK_FLUCTUATION::NONE)
{
    _mapCostume.clear();
}

RankInfo::RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, std::string nick, std::string platform, std::string userIdx,int skin, std::string costume, std::string weapon, int tier):
_eLocalType(localType)
,_eType(type)
,_rank(rank)
,_score(score)
,_nick(nick)
,_platform(platform)
,_userIdx(userIdx)
,_skinIdx(skin)
,_tier(tier)
,_eFluctuation(E_RANK_FLUCTUATION::NONE)
,_percentage(0)
{
    if(!weapon.empty())
    {
        _weaponIdx = std::atoi(weapon.c_str());
        if ( _weaponIdx <= 0 )
        {
            _weaponIdx = 1;
        }
    }
    
    if(!costume.empty())
    {
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++){
            auto offset = costume.find(",");
            if(offset != std::string::npos){
                auto result = costume.substr(0,offset);
                int ret = std::atoi(result.c_str());
                if((E_COSTUME)i != E_COSTUME::JEWEL)
                    _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                
                costume = costume.substr(offset+1);
            }
            else{
                int ret = std::atoi(costume.c_str());
                _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                break;
            }
        }
        
    }
}

RankInfo::RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, std::string nick, std::string platform, std::string userIdx, int tier):
_eLocalType(localType)
,_eType(type)
,_rank(rank)
,_score(score)
,_nick(nick)
,_platform(platform)
,_userIdx(userIdx)
,_tier(tier)
,_skinIdx(0)
,_weaponIdx(0)
,_eFluctuation(E_RANK_FLUCTUATION::NONE)
,_percentage(0)
{
    _mapCostume.clear();
}

RankInfo::RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, int tier):
_eLocalType(localType)
,_eType(type)
,_rank(rank)
,_score(score)
,_nick("")
,_platform("")
,_userIdx("")
,_tier(tier)
,_skinIdx(0)
,_weaponIdx(0)
,_eFluctuation(E_RANK_FLUCTUATION::NONE)
,_percentage(0)
{
    _mapCostume.clear();
}

RankInfo::~RankInfo()
{
}

#pragma mark -Info get & set
int RankInfo::getRank()
{
    return _rank;
}

int RankInfo::getPercentage()
{
    return _percentage;
}

double RankInfo::getScore()
{
    return _score;
}

int RankInfo::getTier()
{
    return _tier;
}

std::string RankInfo::getNick()
{
    return _nick;
}

std::string RankInfo::getPlatform()
{
    return _platform;
}

std::string RankInfo::getUserIdx()
{
    return _userIdx;
}

E_RANK_TYPE RankInfo::getRankType()
{
    return _eType;
}

E_RANK_LOCAL_TYPE RankInfo::getLocalRankType()
{
    return _eLocalType;
}

E_RANK_FLUCTUATION RankInfo::getFluctuation()
{
    return _eFluctuation;
}

std::map<E_COSTUME, E_COSTUME_IDX> RankInfo::getCostume()
{
    return _mapCostume;
}

int RankInfo::getWeaponIdx()
{
    return _weaponIdx;
}

int RankInfo::getSkinIdx()
{
    return _skinIdx;
}

void RankInfo::setRank(int rank)
{
    
    if(_rank == rank) _eFluctuation = E_RANK_FLUCTUATION::NONE;
    if(_rank < rank) _eFluctuation = E_RANK_FLUCTUATION::UP;
    if(_rank > rank) _eFluctuation = E_RANK_FLUCTUATION::DOWN;
    
    _rank = rank;
}

void RankInfo::setPercentage(int value)
{
    _percentage = value;
}

void RankInfo::setScore(double score)
{
    _score = score;
}

void RankInfo::setTier(int tier)
{
    _tier = tier;
}

void RankInfo::setName(std::string value)
{
    _nick = value;
}

void RankInfo::setPlatform(std::string value)
{
    _nick = value;
}

void RankInfo::setUserIdx(std::string value)
{
    _nick = value;
}

void RankInfo::setRankType(E_RANK_TYPE value)
{
    _eType = value;
}

void RankInfo::setLocalRankType(E_RANK_LOCAL_TYPE value)
{
    _eLocalType = value;
}

void RankInfo::setRankType(E_RANK_FLUCTUATION value)
{
    _eFluctuation = value;
}

void RankInfo::setCostume(std::string value)
{
    _mapCostume.clear();
    if(!value.empty())
    {
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++){
            auto offset = value.find(",");
            if(offset != std::string::npos){
                auto result = value.substr(0,offset);
                int ret = std::atoi(result.c_str());
                if((E_COSTUME)i != E_COSTUME::JEWEL)
                    _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                
                value = value.substr(offset+1);
            }
            else{
                int ret = std::atoi(value.c_str());
                _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                break;
            }
        }
        
    }
}

void RankInfo::setWeaponIdx(std::string value)
{
    _weaponIdx = std::atoi(value.c_str());
}

void RankInfo::setSkinIdx(int value)
{
    _skinIdx = value;
}

#pragma mark - manager
RankingManager::RankingManager():
_rankUpdateTimerMax(3600),
_rankUpdateTimer(0),
_nNowViewTier(0),
_nCurrentPage(0),
_nMaxPage(0),
_callbackHighFloorRank(nullptr),
_callbackUserRankInfo(nullptr),
_callbackReDraw(nullptr),
_bPrev(false),
_bNext(false)
{
    _vecMyRankInfo.clear();
}

RankingManager::~RankingManager(void)
{
    for(int i = 0; i < _vecMyRankInfo.size(); ++i)
        delete _vecMyRankInfo[i];
    
    _vecMyRankInfo.clear();
}

bool RankingManager::init()
{
    return true;
}

void RankingManager::update(float dt, E_RANK_LOCAL_TYPE type)
{
    _rankUpdateTimer += dt;
    
    if(_rankUpdateTimer >= _rankUpdateTimerMax)
    {
        _rankUpdateTimer = 0;
        switch (type) {
            case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
                requestHighFloorRankUpdate(nullptr);
                break;
            default:
                break;
        }
        
    }
}

#pragma mark - Manager Utils
RankInfo* RankingManager::getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE type)
{
    RankInfo* result = nullptr;
    
    for(auto info : _vecMyRankInfo)
    {
        if(info->getLocalRankType() == type)
        {
            result = info;
            break;
        }
    }
    
    return result;
}

#pragma mark - Manager Network

void RankingManager::requestHighFloorRankUpdate(const std::function<void(bool)>& pCallback, bool bShowLoading)
{
    if(bShowLoading == true)
        PopupLoading::show();
    
    _callbackHighFloorRank = pCallback;
    std::string url = "/dungeonRanking/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_ranktype",(int) E_RANK_TYPE::RANK_HIGH_FLOOR);
    req->setData("_score", UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL));
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responsHighFloorRankUpdate(response,dd);
    };
    req->send(cb);
}

void RankingManager::responsHighFloorRankUpdate(cocos2d::network::HttpResponse *response, std::string &data)
{
    if(PopupLoading::isShow())
        PopupLoading::hide();
    
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
        if(_callbackHighFloorRank != nullptr)
        {
            _callbackHighFloorRank(false);
        }
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        _callbackHighFloorRank = nullptr;
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
       
    if(result == 0)
    {
        if(_callbackHighFloorRank != nullptr)
        {
            _callbackHighFloorRank(false);
        }
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        _callbackHighFloorRank = nullptr;
        return;
    }
    else if(result == 4)
    {
        if(_callbackHighFloorRank != nullptr)
        {
            _callbackHighFloorRank(true);
        }
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(true);
        }
        
        _callbackHighFloorRank = nullptr;
        return;
    }
    
    if(result == 1)
    {
        auto objMyRank = jsonParser["_my_rank"].GetObject();
        
        int rank = objMyRank["rank"].GetInt();
        int score = objMyRank["score"].GetInt();
        
        auto info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
        
        if(info == nullptr)
        {
            _vecMyRankInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::HIGH_FLOOR, E_RANK_TYPE::RANK_HIGH_FLOOR, rank, score));
        }
        else
        {
            info->setRank(rank);
            info->setScore(score);
        }
    }
    
    if(_callbackHighFloorRank != nullptr)
    {
        _callbackHighFloorRank(true);
    }
    
    if(_callbackReDraw != nullptr)
    {
        _callbackReDraw(true);
    }
    
    //랭크 정보 갱신 완료 했으니 타이머 초기화
    _rankUpdateTimer = 0;
    
    _callbackHighFloorRank = nullptr;
}

void RankingManager::requestHighFloorUserRankInfo(const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback)
{
    PopupLoading::show();
    _callbackUserRankInfo = pCallback;
    std::string url = "/dungeonRanking/info/%d/%d";
    //데일리 리셋 타입 : 0
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), (int)E_RANK_TYPE::RANK_HIGH_FLOOR);
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responstHighFloorUserRankInfo(response,dd);
    };
    req->send(cb);
}

void RankingManager::responstHighFloorUserRankInfo(cocos2d::network::HttpResponse *response, std::string &data)
{
    PopupLoading::hide();
    bool bError = false;
    std::vector<RankInfo*> vecRankUserInfo;
    std::vector<RankInfo*> vecTopRankUserInfo;
    vecRankUserInfo.clear();
    vecTopRankUserInfo.clear();
    
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
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
       
    if(result != 1)
    {
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        std::string strText = GAME_TEXT("t_ui_error_2");
        if(result == 4)
        {
            strText = GAME_TEXT("t_ui_error_52");
        }
        else if(result == 5)
        {
            strText = GAME_TEXT("t_ui_error_30");
        }
        else if(result == 6)
        {
            strText = GAME_TEXT("t_ui_error_51");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto arrayUserRank = jsonParser["_ranklist"].GetArray();
    
    for(int i = 0; i < arrayUserRank.Size(); ++i)
    {
        auto objUserRank = arrayUserRank[i].GetObject();
        int score = objUserRank["score"].GetInt();
        int rank = objUserRank["rank"].GetInt();
        std::string nick = objUserRank["nick"].GetString();
        std::string userIdx = objUserRank["useridx"].GetString();
        std::string platform = objUserRank["platform"].GetString();
        
        vecRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::HIGH_FLOOR, E_RANK_TYPE::RANK_HIGH_FLOOR, rank, score, nick, platform, userIdx));
    }
    
    std::sort(vecRankUserInfo.begin(), vecRankUserInfo.end(), sortUserRank);
    
    auto arrayTopRank = jsonParser["_top_rank"].GetArray();
    
    for(int i = 0; i < arrayTopRank.Size(); ++i)
    {
        auto objTopRank = arrayTopRank[i].GetObject();
        int score = objTopRank["score"].GetInt();
        int rank = objTopRank["rank"].GetInt();
        std::string nick = objTopRank["nick"].GetString();
        std::string userIdx = objTopRank["useridx"].GetString();
        std::string platform = objTopRank["platform"].GetString();
        int skinIdx = objTopRank["skin"].GetInt();
        std::string costume = objTopRank["costume"].GetString();
        std::string weapon = objTopRank["weapons"].GetString();
        
        vecTopRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::HIGH_FLOOR, E_RANK_TYPE::RANK_HIGH_FLOOR, rank, score, nick, platform, userIdx, skinIdx, costume, weapon));
    }
    
    std::sort(vecTopRankUserInfo.begin(), vecTopRankUserInfo.end(), sortUserRank);
    
    if(result == 1)
    {
        auto objMyRank = jsonParser["_my_rank"].GetObject();
        
        int rank = objMyRank["rank"].GetInt();
        int score = objMyRank["score"].GetInt();
        
        auto info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
        
        if(info == nullptr)
        {
            _vecMyRankInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::HIGH_FLOOR, E_RANK_TYPE::RANK_HIGH_FLOOR, rank, score));
        }
        else
        {
            info->setRank(rank);
            info->setScore(score);
        }
    }
    
    if(_callbackUserRankInfo != nullptr)
    {
        _callbackUserRankInfo(true, vecRankUserInfo, vecTopRankUserInfo);
    }
    
    if(_callbackReDraw != nullptr)
    {
        _callbackReDraw(true);
    }
    
    //랭크 정보 갱신 완료 했으니 타이머 초기화
    _rankUpdateTimer = 0;
    
    _callbackUserRankInfo = nullptr;
}

void RankingManager::requestRaidUserRankInfo(int nTier, int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback)
{
    _callbackUserRankInfo = pCallback;
    std::string url = "/new/raid/ranking/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_tier", nTier);
    req->setData("_page", nPage);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responstRaidUserRankInfo(response,dd);
    };
    req->send(cb);
}

void RankingManager::responstRaidUserRankInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    std::vector<RankInfo*> vecRankUserInfo;
    std::vector<RankInfo*> vecTopRankUserInfo;
    vecRankUserInfo.clear();
    vecTopRankUserInfo.clear();
    
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
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int result = 1;
    
    if(jsonParser.HasMember("_result"))
        result = jsonParser["_result"].GetInt();
    
    if(result == 0 || result == 2)
    {
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        std::string strError = "";
        if(result == 0)
            strError = GAME_TEXT("t_ui_error_2");
        else if(result == 2)
            strError = GAME_TEXT("t_ui_error_6");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto arrayUserRank = jsonParser["_ranklist"].GetArray();
    
    for(int i = 0; i < arrayUserRank.Size(); ++i)
    {
        auto objUserRank = arrayUserRank[i].GetObject();
        int score = objUserRank["score"].GetInt();
        int rank = objUserRank["rank"].GetInt();
        std::string nick = objUserRank["nick"].GetString();
        std::string userIdx = objUserRank["useridx"].GetString();
        std::string platform = objUserRank["platform"].GetString();
        
        vecRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::NEW_RAID, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx));
    }
    
    std::sort(vecRankUserInfo.begin(), vecRankUserInfo.end(), sortUserRank);
    
    auto arrayTopRank = jsonParser["_top_rank"].GetArray();
    
    for(int i = 0; i < arrayTopRank.Size(); ++i)
    {
        auto objTopRank = arrayTopRank[i].GetObject();
        int score = objTopRank["score"].GetInt();
        int rank = objTopRank["rank"].GetInt();
        std::string nick = objTopRank["nick"].GetString();
        std::string userIdx = objTopRank["useridx"].GetString();
        std::string platform = objTopRank["platform"].GetString();
        int skinIdx = objTopRank["skin"].GetInt();
        std::string costume = objTopRank["costume"].GetString();
        std::string weapon = objTopRank["weapons"].GetString();
        
        vecTopRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::NEW_RAID, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx, skinIdx, costume, weapon));
    }
    
    std::sort(vecTopRankUserInfo.begin(), vecTopRankUserInfo.end(), sortUserRank);
    
    _nNowViewTier = jsonParser["_tier"].GetInt();
    
    _nCurrentPage = jsonParser["_page"].GetInt();
    _nMaxPage = jsonParser["_max_page"].GetInt();
    
    auto objMyRank = jsonParser["_my_rank"].GetObject();
    
    int rank = 0;
    int score = 0;
    int tier = 0;
    int percentage = 0;
    
    if(objMyRank.HasMember("rank"))
        rank = objMyRank["rank"].GetInt();
    
    if(objMyRank.HasMember("score"))
        score = objMyRank["score"].GetInt();
    
    if(objMyRank.HasMember("tier"))
        tier = objMyRank["tier"].GetInt();
    
    if(objMyRank.HasMember("percentage"))
        percentage = objMyRank["percentage"].GetInt();
    
    auto info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
    
    if(info == nullptr)
    {
        _vecMyRankInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::NEW_RAID, E_RANK_TYPE::NONE, rank, score, tier));
        info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
        info->setPercentage(percentage);
        
    }
    else
    {
        info->setRank(rank);
        info->setScore(score);
        info->setTier(tier);
        info->setPercentage(percentage);
    }
    
    if(_callbackUserRankInfo != nullptr)
    {
        _callbackUserRankInfo(true, vecRankUserInfo, vecTopRankUserInfo);
    }
    
    if(_callbackReDraw != nullptr)
    {
        _callbackReDraw(true);
    }
    
    
    _callbackUserRankInfo = nullptr;
}

void RankingManager::requestEventRaceRankInfo(int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback)
{
    _callbackUserRankInfo = pCallback;
    
    std::string url = "/event/race/rank/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_page", nPage);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responstEventRaceRankInfo(response,dd);
    };
    req->send(cb);
}

void RankingManager::responstEventRaceRankInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    std::vector<RankInfo*> vecRankUserInfo;
    std::vector<RankInfo*> vecTopRankUserInfo;
    vecRankUserInfo.clear();
    vecTopRankUserInfo.clear();
    
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
        if ( _callbackUserRankInfo != nullptr )
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
            _callbackUserRankInfo = nullptr;
        }
        
        if ( _callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int result = 1;
    if(jsonParser.HasMember("_result"))
        result = jsonParser["_result"].GetInt();
    
    if(result == 0 || result == 2)
    {
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
            _callbackUserRankInfo = nullptr;
        }
        
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        std::string strError = "";
        if(result == 0)
            strError = GAME_TEXT("t_ui_error_2");
        else if(result == 2)
            strError = GAME_TEXT("t_ui_error_6");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto arrayUserRank = jsonParser["_ranklist"].GetArray();
    
    for(int i = 0; i < arrayUserRank.Size(); ++i)
    {
        auto objUserRank = arrayUserRank[i].GetObject();
        double score = objUserRank["score"].GetDouble();
        int rank = objUserRank["rank"].GetInt();
        std::string nick = objUserRank["nick"].GetString();
        std::string userIdx = objUserRank["useridx"].GetString();
        std::string platform = objUserRank["platform"].GetString();
        
        vecRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::EVENT_RACE, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx));
    }
    
    std::sort(vecRankUserInfo.begin(), vecRankUserInfo.end(), sortUserRank);
    
    auto arrayTopRank = jsonParser["_top_rank"].GetArray();
    
    for(int i = 0; i < arrayTopRank.Size(); ++i)
    {
        auto objTopRank = arrayTopRank[i].GetObject();
        double score = objTopRank["score"].GetDouble();
        int rank = objTopRank["rank"].GetInt();
        std::string nick = objTopRank["nick"].GetString();
        std::string userIdx = objTopRank["useridx"].GetString();
        std::string platform = objTopRank["platform"].GetString();
        int skinIdx = objTopRank["skin"].GetInt();
        std::string costume = objTopRank["costume"].GetString();
        std::string weapon = objTopRank["weapons"].GetString();
        
        vecTopRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::EVENT_RACE, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx, skinIdx, costume, weapon));
    }
    
    std::sort(vecTopRankUserInfo.begin(), vecTopRankUserInfo.end(), sortUserRank);
    
    _nCurrentPage = jsonParser["_page"].GetInt();
    _nMaxPage = jsonParser["_max_page"].GetInt();
    
    auto objMyRank = jsonParser["_my_rank"].GetObject();
    
    int rank = 0;
    double score = 0;
    int tier = 0;
    int percentage = 0;
    
    if(objMyRank.HasMember("rank"))
        rank = objMyRank["rank"].GetInt();
    
    if(objMyRank.HasMember("score"))
        score = objMyRank["score"].GetDouble();
    
    if(objMyRank.HasMember("tier"))
        tier = objMyRank["tier"].GetInt();
    
    if(objMyRank.HasMember("percentage"))
        percentage = objMyRank["percentage"].GetInt();
    
    auto info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::EVENT_RACE);
    if(info == nullptr)
    {
        _vecMyRankInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::EVENT_RACE, E_RANK_TYPE::NONE, rank, score, tier));
        info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::EVENT_RACE);
        info->setPercentage(percentage);
    }
    else
    {
        info->setRank(rank);
        info->setScore(score);
        info->setTier(tier);
        info->setPercentage(percentage);
    }
    
    if(_callbackUserRankInfo != nullptr)
    {
        _callbackUserRankInfo(true, vecRankUserInfo, vecTopRankUserInfo);
        _callbackUserRankInfo = nullptr;
    }
    
    if(_callbackReDraw != nullptr)
    {
        _callbackReDraw(true);
    }
}

void RankingManager::requestFlashmanRankInfo(int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback)
{
    _callbackUserRankInfo = pCallback;
    
    std::string url = "/minigame/ranking/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_page", nPage);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responstFlashmanRankInfo(response,dd);
    };
    req->send(cb);
}
void RankingManager::responstFlashmanRankInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    std::vector<RankInfo*> vecRankUserInfo;
    std::vector<RankInfo*> vecTopRankUserInfo;
    vecRankUserInfo.clear();
    vecTopRankUserInfo.clear();
    
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
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int result = 1;
    
    if(jsonParser.HasMember("_result"))
        result = jsonParser["_result"].GetInt();
    
    if(result == 0 || result == 2)
    {
        if(_callbackUserRankInfo != nullptr)
        {
            _callbackUserRankInfo(false, vecRankUserInfo, vecTopRankUserInfo);
        }
        _callbackUserRankInfo = nullptr;
        
        if(_callbackReDraw != nullptr)
        {
            _callbackReDraw(false);
        }
        
        std::string strError = "";
        if(result == 0)
            strError = GAME_TEXT("t_ui_error_2");
        else if(result == 2)
            strError = GAME_TEXT("t_ui_error_6");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto arrayUserRank = jsonParser["_ranklist"].GetArray();
    
    for(int i = 0; i < arrayUserRank.Size(); ++i)
    {
        auto objUserRank = arrayUserRank[i].GetObject();
        double score = objUserRank["score"].GetDouble();
        int rank = objUserRank["rank"].GetInt();
        std::string nick = objUserRank["nick"].GetString();
        std::string userIdx = objUserRank["useridx"].GetString();
        std::string platform = objUserRank["platform"].GetString();
        
        vecRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::FLASHMAN, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx));
    }
    
    std::sort(vecRankUserInfo.begin(), vecRankUserInfo.end(), sortUserRank);
    
    auto arrayTopRank = jsonParser["_top_rank"].GetArray();
    
    for(int i = 0; i < arrayTopRank.Size(); ++i)
    {
        auto objTopRank = arrayTopRank[i].GetObject();
        double score = objTopRank["score"].GetDouble();
        int rank = objTopRank["rank"].GetInt();
        std::string nick = objTopRank["nick"].GetString();
        std::string userIdx = objTopRank["useridx"].GetString();
        std::string platform = objTopRank["platform"].GetString();
        int skinIdx = objTopRank["skin"].GetInt();
        std::string costume = objTopRank["costume"].GetString();
        std::string weapon = objTopRank["weapons"].GetString();
        
        vecTopRankUserInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::FLASHMAN, E_RANK_TYPE::NONE, rank, score, nick, platform, userIdx, skinIdx, costume, weapon));
    }
    
    std::sort(vecTopRankUserInfo.begin(), vecTopRankUserInfo.end(), sortUserRank);
    
    _nCurrentPage = jsonParser["_page"].GetInt();
    _nMaxPage = jsonParser["_max_page"].GetInt();
    
    auto objMyRank = jsonParser["_rank"].GetObject();
    
    int rank = 0;
    double score = 0;
    int tier = 0;
    int percentage = 0;
    
    if(objMyRank.HasMember("rank"))
        rank = objMyRank["rank"].GetInt();
    
    if(objMyRank.HasMember("score"))
        score = objMyRank["score"].GetDouble();
    
    if(objMyRank.HasMember("tier"))
        tier = objMyRank["tier"].GetInt();
    
    if(objMyRank.HasMember("percentage"))
        percentage = objMyRank["percentage"].GetInt();
    
    auto info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::FLASHMAN);
    
    if(info == nullptr)
    {
        _vecMyRankInfo.push_back(new RankInfo(E_RANK_LOCAL_TYPE::FLASHMAN, E_RANK_TYPE::NONE, rank, score, tier));
        info = getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::FLASHMAN);
        info->setPercentage(percentage);
        
    }
    else
    {
        info->setRank(rank);
        info->setScore(score);
        info->setTier(tier);
        info->setPercentage(percentage);
    }
    
    if(_callbackUserRankInfo != nullptr)
    {
        _callbackUserRankInfo(true, vecRankUserInfo, vecTopRankUserInfo);
    }
    
    if(_callbackReDraw != nullptr)
    {
        _callbackReDraw(true);
    }
    
    
    _callbackUserRankInfo = nullptr;
}

#pragma mark -utils

void RankingManager::renewalMyRankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, int tier, int percentage)
{
    auto info = getMyRankInfoFromLocalType(localType);
    
    if(info != nullptr)
    {
        info->setRank(rank);
        info->setScore(score);
        info->setTier(tier);
        info->setPercentage(percentage);
        return;
    }
    
    _vecMyRankInfo.push_back(new RankInfo(localType, type, rank, score, tier));
    info = getMyRankInfoFromLocalType(localType);
    info->setPercentage(percentage);
    
}

void RankingManager::setCallbackReDraw(const std::function<void (bool)> &pCallback)
{
    _callbackReDraw = pCallback;
}

void RankingManager::setRankUpdateTimer(float value)
{
    _rankUpdateTimer = value;
}

int RankingManager::getNowTier()
{
    return _nNowViewTier;
}

int RankingManager::getNowPage()
{
    return _nCurrentPage;
}

int RankingManager::getMaxPage()
{
    return _nMaxPage;
}

bool RankingManager::isNextPage()
{
    return _bNext;
}

bool RankingManager::isPrevPage()
{
    return _bPrev;
}

void RankingManager::sceneChange()
{
    _rankUpdateTimer = 0.f;
    
    _callbackReDraw = nullptr;
}
