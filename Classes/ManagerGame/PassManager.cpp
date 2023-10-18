//
//  PassManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/16.
//

#include "PassManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

#pragma mark - PassInfo
PassStep* PassStep::create()
{
    PassStep *pRet = new(std::nothrow) PassStep();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

PassStep::PassStep() :
_nIdx(0),
_nPoint(0),
_nStepUpCost(0),

_nRewardFreeIdx(0),
_nRewardFreeTypeParams(0),
_nRewardFreeCnt(0),
_bRewardFree(0),

_nRewardPurIdx(0),
_nRewardPurTypeParams(0),
_nRewardPurCnt(0),
_bRewardPur(0)
{
    
}

PassStep::~PassStep()
{
    
}

bool PassStep::init()
{
    return true;
}

int PassStep::getIdx()
{
    return _nIdx;
}
void PassStep::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int PassStep::getPoint()
{
    return _nPoint;
}
void PassStep::setPoint(int nPoint)
{
    _nPoint = nPoint;
}

int PassStep::getStepUpCost()
{
    return _nStepUpCost;
}

void PassStep::setStepUpCost(int nCost)
{
    _nStepUpCost = nCost;
}

int PassStep::getRewardFreeIdx()
{
    return _nRewardFreeIdx;
}
void PassStep::setRewardFreeIdx(int nIdx)
{
    _nRewardFreeIdx = nIdx;
}

int PassStep::getRewardFreeTypeParams()
{
    return _nRewardFreeTypeParams;
}
void PassStep::setRewardFreeTypeParams(int nTypeParams)
{
    _nRewardFreeTypeParams = nTypeParams;
}

int PassStep::getRewardFreeCount()
{
    return _nRewardFreeCnt;
}
void PassStep::setRewardFreeCount(int nCount)
{
    _nRewardFreeCnt = nCount;
}

bool PassStep::isRewardFree()
{
    return _bRewardFree;
}
void PassStep::setRewardFree(bool bReward)
{
    _bRewardFree = bReward;
}

int PassStep::getRewardPurIdx()
{
    return _nRewardPurIdx;
}
void PassStep::setRewardPurIdx(int nIdx)
{
    _nRewardPurIdx = nIdx;
}

int PassStep::getRewardPurTypeParams()
{
    return _nRewardPurTypeParams;
}
void PassStep::setRewardPurTypeParams(int nTypeParams)
{
    _nRewardPurTypeParams = nTypeParams;
}

int PassStep::getRewardPurCount()
{
    return _nRewardPurCnt;
}
void PassStep::setRewardPurCount(int nCount)
{
    _nRewardPurCnt = nCount;
}

bool PassStep::isRewardPur()
{
    return _bRewardPur;
}
void PassStep::setRewardPur(bool bReward)
{
    _bRewardPur = bReward;
}

#pragma mark - PassInfo
PassInfo* PassInfo::create()
{
    PassInfo *pRet = new(std::nothrow) PassInfo();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

PassInfo::PassInfo() :
_eType(E_PASS_TYPE::PASS_NONE),

_nSeason(0),
_nPurchaseTime(0),
_nResetTime(0),

_nPoint(0),
_bBuyPass(false),

_nBadgeFree(0),
_nBadgePur(0)
{
    _listStep.clear();
}

PassInfo::~PassInfo()
{
    _listStep.clear();
}

bool PassInfo::init()
{
    return true;
}

E_PASS_TYPE PassInfo::getType()
{
    return _eType;
}

void PassInfo::setType(E_PASS_TYPE eType)
{
    _eType = eType;
}

int PassInfo::getSeason()
{
    return _nSeason;
}

void PassInfo::setSeason(int nSeason)
{
    _nSeason = nSeason;
}

int64_t PassInfo::getPurchaseTime()
{
    return _nPurchaseTime;
}

void PassInfo::setPurchaseTime(int64_t nTime)
{
    _nPurchaseTime = nTime;
}

int64_t PassInfo::getResetTime()
{
    return _nResetTime;
}

void PassInfo::setResetTime(int64_t nTime)
{
    _nResetTime = nTime;
}

int PassInfo::getPoint()
{
    return _nPoint;
}

void PassInfo::setPoint(int nPoint)
{
    _nPoint = nPoint;
}

bool PassInfo::isBuyPass()
{
    return _bBuyPass;
}

void PassInfo::setBuyPass(bool bBuyPass)
{
    _bBuyPass = bBuyPass;
}

int PassInfo::getBadgeFree()
{
    return _nBadgeFree;
}
void PassInfo::setBadgeFree(int nBadge)
{
    _nBadgeFree = nBadge;
}

int PassInfo::getBadgePur()
{
    return _nBadgePur;
}
void PassInfo::setBadgePur(int nBadge)
{
    _nBadgePur = nBadge;
}

cocos2d::Vector<PassStep*> PassInfo::getListStep()
{
    return _listStep;
}

void PassInfo::setListStep(cocos2d::Vector<PassStep*> list)
{
    _listStep.clear();
    _listStep.pushBack(list);
}

PassStep* PassInfo::getStep(int nIdx)
{
    PassStep* objResult = nullptr;
    
    for ( auto info : _listStep )
    {
        if ( info->getIdx() == nIdx )
        {
            objResult = info;
            break;
        }
    }
    return objResult;
}

int PassInfo::getLevel()
{
    int nResult = 0;
    
    for ( auto info : _listStep )
    {
        if ( getPoint() >= info->getPoint() )
        {
            nResult = info->getIdx();
        }
        else
        {
            break;
        }
    }
    
    return nResult;
}

bool PassInfo::isLevelFinish()
{
    bool bResult = false;
    if ( _listStep.empty() == true )
    {
        return bResult;
    }
    
    auto objPassLast = _listStep.back();
    if ( objPassLast->getIdx() == getLevel() )
    {
        bResult = true;
    }
    
    return bResult;
}

int PassInfo::getExpNext()
{
    int nResult = 0;
    int nLevel = getLevel();

    auto objStep = getStep(nLevel);
    auto objStepNext = getStep(nLevel+1);
    if ( objStep == nullptr || objStepNext == nullptr )
    {
        return 999;
    }
    
    nResult = objStepNext->getPoint() - objStep->getPoint();
    return nResult;
}

int PassInfo::getExp()
{
    int nResult = 0;
    int nLevel = getLevel();

    auto objStep = getStep(nLevel);
    if ( objStep == nullptr )
    {
        return 0;
    }
    
    nResult = getPoint() - objStep->getPoint();
    return nResult;
}

bool PassInfo::isReward()
{
    bool result = false;
    
    for ( auto info : _listStep )
    {
        if ( _bBuyPass == true )
        {
            if( getPoint() >= info->getPoint() && info->isRewardPur() == false )
            {
                result = true;
                break;
            }
        }
        
        if ( getPoint() >= info->getPoint() && info->isRewardFree() == false )
        {
            result = true;
            break;
        }
    }
    
    return result;
}



#pragma mark - PassManager
PassManager::PassManager():

_callbackInfo(nullptr),
_callbacReward(nullptr),
_callbackStepUp(nullptr)
{
    
}
PassManager::~PassManager(void)
{
    
}

bool PassManager::init()
{
    return true;
}

#pragma mark - PassManager : set, get, other
PassInfo* PassManager::getPassInfo(E_PASS_TYPE type)
{
    PassInfo* resultInfo = nullptr;
    
    for(auto info : _listPass)
    {
        if(info->getType() == type)
        {
            resultInfo = info;
            break;
        }
    }
    
    return resultInfo;
}

bool PassManager::isBuyPass(E_PASS_TYPE type)
{
    bool result = false;
    
    auto objPass = PassManager::getInstance()->getPassInfo(type);
    if ( objPass != nullptr && objPass->isBuyPass() == true )
    {
        time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
        if ( nDiffTime > 0 )
        {
            result = true;
        }
    }
    
    return result;
}


#pragma mark - PassManager : network
void PassManager::requestInfo(const std::function<void(bool)>& pCallback)
{
    _callbackInfo = pCallback;
    
    //
    std::string url = "/seasonpass/info/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void PassManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    int nSeason = jsonParser["_season"].GetInt();
    int64_t nPurchaseTime = jsonParser["_reset_time"].GetInt64();
    int64_t nResetTime = jsonParser["_init_reset_time"].GetInt64();
    int nPoint = jsonParser["_point"].GetInt();
    bool bBuyPass = jsonParser["_is_buy_pass"].GetBool();

    //
    auto objPassPrev = getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( objPassPrev != nullptr )
    {
        _listPass.eraseObject(objPassPrev);
        objPassPrev = nullptr;
    }
    
    //
    auto objPass = PassInfo::create();
    objPass->setType(E_PASS_TYPE::PASS_DUNGEON);
    objPass->setSeason(nSeason);
    objPass->setPurchaseTime(nPurchaseTime);
    objPass->setResetTime(nResetTime);
    objPass->setPoint(nPoint);
    objPass->setBuyPass(bBuyPass);
    
    int nFinishBadgeFree = 0;
    int nFinishBadgePur = 0;
    for ( int i = 0; i < jsonParser["_finish_badge_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_finish_badge_list"][i];
        
        int nType = jsonValue["type"].GetInt();
        if ( nType == 0 )
        {// 무료
            nFinishBadgeFree = jsonValue["type_params"].GetInt();
        }
        else if ( nType == 1 )
        {// 유료
            nFinishBadgePur = jsonValue["type_params"].GetInt();
        }
    }
    objPass->setBadgeFree(nFinishBadgeFree);
    objPass->setBadgePur(nFinishBadgePur);

    cocos2d::Vector<PassStep*> listStep;
    for ( int i = 0; i < jsonParser["_pass_info_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_pass_info_list"][i];
        
        int nIdx = jsonValue["idx"].GetInt();
        int nPoint= jsonValue["point"].GetInt();
        
        int nStepUpCost = 50;
        if ( jsonValue.HasMember("step_up_cost") )
            nStepUpCost= jsonValue["step_up_cost"].GetInt();
        
        int nRewardFreeIdx = jsonValue["reward_free_idx"].GetInt();
        int nRewardFreeTypeParams = 0;
        if ( jsonValue.HasMember("reward_free_type_params") )
        {
            nRewardFreeTypeParams = jsonValue["reward_free_type_params"].GetInt();
        }
        int nRewardFreeCnt = jsonValue["reward_free_count"].GetInt();
        bool bRewardFree = jsonValue["is_free_reward"].GetBool();
        
        int nRewardPurIdx = jsonValue["reward_pur_idx"].GetInt();
        int nRewardPurTypeParams = 0;
        if ( jsonValue.HasMember("reward_pur_type_params") )
        {
            nRewardPurTypeParams = jsonValue["reward_pur_type_params"].GetInt();
        }
        
        int nRewardPurCnt = jsonValue["reward_pur_count"].GetInt();
        bool bRewardPur = jsonValue["is_pur_reward"].GetBool();
        
        auto objStep = PassStep::create();
        objStep->setIdx(nIdx);
        objStep->setPoint(nPoint);
        objStep->setStepUpCost(nStepUpCost);
        objStep->setRewardFreeIdx(nRewardFreeIdx);
        objStep->setRewardFreeTypeParams(nRewardFreeTypeParams);
        objStep->setRewardFreeCount(nRewardFreeCnt);
        objStep->setRewardFree(bRewardFree);
        objStep->setRewardPurIdx(nRewardPurIdx);
        objStep->setRewardPurTypeParams(nRewardPurTypeParams);
        objStep->setRewardPurCount(nRewardPurCnt);
        objStep->setRewardPur(bRewardPur);
        listStep.pushBack(objStep);
    }
    objPass->setListStep(listStep);
    _listPass.pushBack(objPass);
     
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
        _callbackInfo = nullptr;
    }
    
    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DUNGEONPASS_REWARD);
}

void PassManager::requestReward(int nSeason, int nIdx, E_PASS_REWARD_TYPE eRewardType, std::function<void(bool, bool, bool)> pCallback)
{
    _callbacReward = pCallback;
    
    //
    std::string url = "/v1/seasonpass/CompleteSeasonPass";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", nSeason);
    req->setData("_index", nIdx);
    if ( eRewardType == E_PASS_REWARD_TYPE::FREE )
    {
        req->setData("_type", 0);
    }
    else if ( eRewardType == E_PASS_REWARD_TYPE::PURCHASE )
    {
        req->setData("_type", 1);
    }
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response, dd);
    };
    req->send(cb);
}

void PassManager::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        std::vector<int> listRewardBadge;
        
        if ( _callbacReward != nullptr )
        {
            _callbacReward(false, false, false);
            _callbacReward = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
        bool bRewardBadgeFree = false;
        bool bRewardBadgePur = false;
        if ( objPass != nullptr )
        {
            auto listItems = ItemsManager::getInstance()->getConvertArray(strReward);
            for ( auto& objTemp : listItems )
            {
                if ( objTemp->getIdx() == 10000 )
                {
                    if ( objTemp->getTypeParams() == objPass->getBadgeFree() )
                    {
                        bRewardBadgeFree = true;
                    }
                    else if ( objTemp->getTypeParams() == objPass->getBadgePur() )
                    {
                        bRewardBadgePur = true;
                    }
                }
            }
        }
        
        //
        if ( _callbacReward != nullptr )
        {
            _callbacReward(true, bRewardBadgeFree, bRewardBadgePur);
            _callbacReward = nullptr;
        }
    }
    else
    {
        if ( _callbacReward != nullptr )
        {
            _callbacReward(false, false, false);
            _callbacReward = nullptr;
        }
    }
}

void PassManager::requestStepUp(int nSeason, int nIdx, const std::function<void(bool, int)>& pCallback)
{
    _callbackStepUp = pCallback;
    
    //
    std::string url = "/v0/seasonpass/GainPassPoint";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", nSeason);
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseStepUp(response,dd);
    };
    req->send(cb);
}

void PassManager::responseStepUp(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackStepUp != nullptr )
        {
            _callbackStepUp(false, -1);
            _callbackStepUp = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        if ( jsonParser.HasMember("_reward") )
        {
            std::string strReward = jsonParser["_reward"].GetString();
            ItemsManager::getInstance()->addItems(strReward);
        }
        
        if ( jsonParser.HasMember("_items") )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        if ( _callbackStepUp != nullptr )
        {
            _callbackStepUp(true, 0);
            _callbackStepUp = nullptr;
        }
    }
    else
    {
        if ( _callbackStepUp != nullptr )
        {
            _callbackStepUp(false, nResult);
            _callbackStepUp = nullptr;
        }
    }
       

    
}
