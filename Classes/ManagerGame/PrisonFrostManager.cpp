//
//  PrisonFrostManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "PrisonFrostManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

PrisonFrostManager::PrisonFrostManager(void) :
_skillUse(0),

_clearFloor(0),
_timeReset(0),
_adsCount(0),

_bBattleAuto(false),
_battleAutoNow(0),
_battleAutoMax(0),

_callbackInfo(nullptr)
{
    
}

PrisonFrostManager::~PrisonFrostManager(void)
{
    _delegateInfo.Clear();
    _delegateSkillLevel.Clear();
    _delegateSkillReset.Clear();
    _delegateResult.Clear();
}

bool PrisonFrostManager::init()
{
    return true;
}

#pragma mark - update
void PrisonFrostManager::update(float dt)
{
    auto listSkill = getListSkill();
    for ( auto& objSkill : listSkill )
    {
        int idx = objSkill->getIdx();
        std::string type = objSkill->getType();
        std::transform(type.begin(), type.end(), type.begin(), tolower);
        
        int step = getSkillStep(idx);
        if ( step == -1 )
        {
            continue;
        }
        
        if ( type.compare("active") != 0 )
        {
            continue;
        }
        
        if ( idx == _skillUse )
        {
            continue;
        }
        
        double skillDuration = getSkillDuration(idx);
        double skillCollTime = getSkillCoolTime(idx);
        
        if ( skillDuration > 0 )
        {
            setSkillDuration(idx, skillDuration - dt);
        }
        else
        {
            setSkillCoolTime(idx, skillCollTime - dt);
        }
    }
}

#pragma mark - load
void PrisonFrostManager::setLoad()
{
    setLoadFloor();
    setLoadSkill();
    setLoadSetting();
}

void PrisonFrostManager::setLoadFloor()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FROST_PRISON_FLOOR);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listFloor.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        int idx = jsonValue["floor"].GetInt();
        int boss = jsonValue["boss"].GetInt();
        
        std::string cost = jsonValue["cost"].GetString();
        std::string reward = jsonValue["clear_reward"].GetString();
        
        int hpControl = jsonValue["hp_control"].GetInt();
 
        auto obj = InfoFrostFloor::create();
        obj->setIdx(idx);
        obj->setBoss(boss);
        obj->setCost(cost);
        obj->setReward(reward);
        obj->setHPControl(hpControl);
        
        //
        _listFloor.pushBack(obj);
    }
}

void PrisonFrostManager::setLoadSkill()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FROST_PRISON_SKILL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listSkillAll.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int idx = jsonValue["skill_idx"].GetInt();
        int step = jsonValue["skill_params"].GetInt();
        
        std::string type = jsonValue["skill_type"].GetString();
        std::string nameKey = jsonValue["skill_name"].GetString();
        std::string contentsKey = jsonValue["skill_contents"].GetString();
        std::string iconPath = jsonValue["skill_icon"].GetString();
        
        int levelMax = jsonValue["max_level"].GetInt();
        double valueBasic = jsonValue["basic_value"].GetDouble();
        double valueLevelPer = jsonValue["level_value"].GetDouble();
        
        int coolTime = jsonValue["cool_time"].GetInt();
        
        int duration = jsonValue["duration"].GetInt();
        
        auto obj = InfoFrostSkill::create();
        obj->setIdx(idx);
        obj->setStep(step);
        obj->setType(type);
        obj->setNameKey(nameKey);
        obj->setContentsKey(contentsKey);
        obj->setIconPath(iconPath);
        obj->setLevelMax(levelMax);
        obj->setValueBasic(valueBasic);
        obj->setValueLevelPer(valueLevelPer);
        obj->setCoolTime(coolTime);
        obj->setDuration(duration);
        
        //
        _listSkillAll.pushBack(obj);
    }
}

void PrisonFrostManager::setLoadSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FROST_PRISON_SETTING);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listSetting.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        
        auto obj = InfoSetting::create();
        obj->setKey(jsonValue["key"].GetString());
        obj->setType(jsonValue["type"].GetString());
        obj->setValue(jsonValue["value"].GetString());
        _listSetting.pushBack(obj);
    }
}

#pragma mark - set, get : prison
cocos2d::Vector<InfoFrostFloor*> PrisonFrostManager::getListFloor()
{
    return _listFloor;
}

cocos2d::Vector<InfoFrostSkill*> PrisonFrostManager::getListSkillAll()
{
    return _listSkillAll;
}

cocos2d::Vector<InfoFrostSkill*> PrisonFrostManager::getListSkill()
{
    Vector<InfoFrostSkill*> listResult;
    
    std::vector<int> listIdx;
    
    auto listSkill = getListSkillAll();
    for ( auto& objSkill : listSkill )
    {
        if ( std::find(listIdx.begin(), listIdx.end(), objSkill->getIdx()) == listIdx.end() )
        {
            listIdx.push_back(objSkill->getIdx());
        }
    }
    
    for ( int idx : listIdx )
    {
        int step = getSkillStep(idx);
        if ( step == -1 )
        {
            step = 0;
        }

        auto obj = getInfoSkill(idx, step);
        if ( obj != nullptr )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

InfoFrostFloor* PrisonFrostManager::getInfoFloor(int idx)
{
    InfoFrostFloor* objResult = nullptr;
    
    for ( auto& obj : _listFloor )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

InfoFrostSkill* PrisonFrostManager::getInfoSkill(int idx, int step)
{
    InfoFrostSkill* objResult = nullptr;
    
    for ( auto& obj : _listSkillAll )
    {
        if ( idx == obj->getIdx() && step == obj->getStep() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

InfoSetting* PrisonFrostManager::getInfoSetting(std::string key)
{
    InfoSetting* objResult = nullptr;
    
    for ( auto& obj : _listSetting )
    {
        if ( obj->getKey().compare(key) == 0 )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

#pragma mark - set, get :
int PrisonFrostManager::getClearFloor()
{
    return _clearFloor.valueInt();
}

int PrisonFrostManager::getClearFloorMax()
{
    auto objFloor = _listFloor.back();
    return objFloor->getIdx();
}

bool PrisonFrostManager::isBattleAuto()
{
    return _bBattleAuto;
}
void PrisonFrostManager::setBattleAuto(bool bEnable)
{
    _bBattleAuto = bEnable;
}

int PrisonFrostManager::getBattleAutoNow()
{
    return _battleAutoNow;
}
void PrisonFrostManager::setBattleAutoNow(int count)
{
    _battleAutoNow = count;
}

int PrisonFrostManager::getBattleAutoMax()
{
    return _battleAutoMax;
}
void PrisonFrostManager::setBattleAutoMax(int count)
{
    _battleAutoMax = count;
}


int64_t PrisonFrostManager::getTimeReset()
{
    return _timeReset;
}

#pragma mark - set, get : skill
std::map<int, int> PrisonFrostManager::getSkillStepAll()
{
    return _listSkillStep;
}
int PrisonFrostManager::getSkillStep(int idx)
{
    int step = -1;
    
    auto iter = _listSkillStep.find(idx);
    if ( iter != _listSkillStep.end() )
    {
        step = _listSkillStep[idx];
    }
    
    return step;
}
void PrisonFrostManager::setSkillStep(int idx, int step)
{
    _listSkillStep[idx] = step;
}

std::map<int, int> PrisonFrostManager::getSkillLevelAll()
{
    return _listSkillLevel;
}
int PrisonFrostManager::getSkillLevel(int idx)
{
    int level = 0;
    
    auto iter = _listSkillLevel.find(idx);
    if ( iter != _listSkillLevel.end() )
    {
        level = _listSkillLevel[idx];
    }
    
    return level;
}
void PrisonFrostManager::setSkillLevel(int idx, int level)
{
    _listSkillLevel[idx] = level;
}
int PrisonFrostManager::getSkillLevelCost(int idx, int levelFrom, int levelTo)
{
    int cost = 0;
    
    if ( idx == 50001 )
    {
        // 25+2*int(레벨/10)
        for ( int i = levelFrom; i <= levelTo; i++ )
        {
            cost += 25 + 2 * (int)(i/10);
        }
        
//        (INT(((100*10^-( 1/(레벨+3))+레벨*2)/10))*2
//        for ( int i = levelFrom; i <= levelTo; i++ )
//        {
//            cost += (int)((100 * std::pow(10, -(1.0/(i+3))) + i * 2) / 10) * 2;
//        }
    }
    else if ( idx == 50002 )
    {
        // 10+2*int(레벨/10)
        for ( int i = levelFrom; i <= levelTo; i++ )
        {
            cost +=  10 + 2 * (int)(i / 10);
        }
            
//        (INT(((100*10^-( 1/(레벨+3))+레벨*2)/10))*5
//        for ( int i = levelFrom; i <= levelTo; i++ )
//        {
//            cost += (int)((100 * std::pow(10, -(1.0/(i+3))) + i * 2) / 10) * 5;
//        }
    }
    else if ( idx == 50003 )
    {
        // 25+2*int(레벨/10)
        for ( int i = levelFrom; i <= levelTo; i++ )
        {
            cost += 25 + 2 * (int)(i / 10);
        }
        
//        (INT(((100*10^-( 1/(레벨+3))+레벨*2)/10))*2
//        for ( int i = levelFrom; i <= levelTo; i++ )
//        {
//            cost += (int)((100 * std::pow(10, -(1.0/(i+3))) + i * 2) / 10) * 2;
//        }
    }
    else
    {
        // 45+5*int(레벨/10)
        for ( int i = levelFrom; i <= levelTo; i++ )
        {
            cost += 45 + 5 * (int)(i / 10);
        }
        
//        (INT(((100*10^-( 1/(레벨+3))+레벨*2)/10))*4
//        for ( int i = levelFrom; i <= levelTo; i++ )
//        {
//            cost += (int)((100 * std::pow(10, -(1.0/(i+3))) + i * 2) / 10) * 4;
//        }
    }
    
    return cost;
}

std::map<int, double> PrisonFrostManager::getSkillCoolTimeAll()
{
    return _listSkillCoolTime;
}
double PrisonFrostManager::getSkillCoolTime(int idx)
{
    double coolTime = 9999;
    
    auto iter = _listSkillCoolTime.find(idx);
    if ( iter != _listSkillCoolTime.end() )
    {
        coolTime = _listSkillCoolTime[idx];
    }
    
    return coolTime;
}
void PrisonFrostManager::setSkillCoolTime(int idx, double coolTime)
{
    _listSkillCoolTime[idx] = coolTime;
}

std::map<int, double> PrisonFrostManager::getSkillDurationAll()
{
    return _listSkillDuration;
}
double PrisonFrostManager::getSkillDuration(int idx)
{
    double duration = 0;
    
    auto iter = _listSkillDuration.find(idx);
    if ( iter != _listSkillDuration.end() )
    {
        duration = _listSkillDuration[idx];
    }
    
    return duration;
}
void PrisonFrostManager::setSkillDuration(int idx, double duration)
{
    _listSkillDuration[idx] = duration;
}

double PrisonFrostManager::getSkillEffect(int idx, int level)
{
    double effect = 0;
    
    int step = getSkillStep(idx);
    if ( step == -1 )
    {
        return effect;
    }
    
    auto objSkill = getInfoSkill(idx, step);
    if ( objSkill == nullptr )
    {
        return effect;
    }
    
    if ( objSkill->getLevelMax() < level )
    {
        level = objSkill->getLevelMax();
    }
    
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( objPass != nullptr && objPass->isBuyPass() == true )
    {
        time_t timeDiff = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
        if ( timeDiff > 0 )
        {
            level += getInfoSetting("skill_pass_bonus")->getValueToInt();
        }
    }
    
    if ( idx == 50002 )
    {//basic_value + (level_value * 레벨)
        effect = objSkill->getValueBasic() + (objSkill->getValueLevelPer() * level);
    }
    else
    {//(기본 공격력)*(1+레벨당 증가율/100)^(레벨)
        effect = objSkill->getValueBasic() * std::pow(1 + objSkill->getValueLevelPer() / 100, level);
        effect = round(effect);
    }
    
    return effect;
}

double PrisonFrostManager::getSkillBuffEffect(int idx, int level)
{
    double effect = 0;
    
    double duration = getSkillDuration(idx);
    if ( duration > 0 )
    {
        effect = getSkillEffect(idx, level);
    }
    
    return effect;
}

void PrisonFrostManager::setSkillUse(int idx)
{
    _skillUse = idx;
}

#pragma mark - set, get : ads
int PrisonFrostManager::getAdsCount()
{
    return _adsCount.valueInt();
}

int PrisonFrostManager::getAdsCountMax()
{
    return getInfoSetting("ad_att_bonus_time")->getValueToInt();
}

double PrisonFrostManager::getAdsEffect()
{
    int count = getAdsCount();
    int countMax = getAdsCountMax();
    if ( count >= countMax )
    {
        count = countMax;
    }
    
    double bonus = getInfoSetting("ad_att_bonus")->getValueToFloat();
    return std::pow(bonus, count);
}


#pragma mark - game

#pragma mark - delegate
void PrisonFrostManager::subDelegateInfo(MafFunction<void(bool, int)>* func)
{
    _delegateInfo += func;
}
void PrisonFrostManager::invokeDelegateInfo(bool bResult, int nResult)
{
    _delegateInfo(bResult, nResult);
}
void PrisonFrostManager::unSubDelegateInfo(void* obj)
{
    _delegateInfo.Clear(obj);
}

void PrisonFrostManager::subDelegateSkillLevelUp(MafFunction<void(bool, int)>* func)
{
    _delegateSkillLevel += func;
}
void PrisonFrostManager::invokeDelegateSkillLevelUp(bool bResult, int nResult)
{
    _delegateSkillLevel(bResult, nResult);
}
void PrisonFrostManager::unSubDelegateSkillLevelUp(void* obj)
{
    _delegateSkillLevel.Clear(obj);
}

void PrisonFrostManager::subDelegateSkillReset(MafFunction<void(bool, int)>* func)
{
    _delegateSkillReset += func;
}
void PrisonFrostManager::invokeDelegateSkillReset(bool bResult, int nResult)
{
    _delegateSkillReset(bResult, nResult);
}
void PrisonFrostManager::unSubDelegateSkillReset(void* obj)
{
    _delegateSkillReset.Clear(obj);
}

void PrisonFrostManager::subDelegateResult(MafFunction<void(bool, int, std::string)>* func)
{
    _delegateResult += func;
}
void PrisonFrostManager::invokeDelegateResult(bool bResult, int nResult, std::string reward)
{
    _delegateResult(bResult, nResult, reward);
}
void PrisonFrostManager::unSubDelegateResult(void* obj)
{
    _delegateResult.Clear(obj);
}

#pragma mark - network : info
void PrisonFrostManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/frost/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonFrostManager::responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        invokeDelegateInfo(false, -1);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    //
    for (auto iter = _listSkillStep.begin() ; iter !=  _listSkillStep.end(); iter++)
    {
        iter->second = -1;
    }
    
    for (auto iter = _listSkillLevel.begin() ; iter !=  _listSkillLevel.end(); iter++)
    {
        iter->second = 0;
    }
    
    // {"_result": 0, "_kill_f": 0, "_count_f": 0, "_mamon": 0, "_skills": []}
    /*
     0 : 실패
     1 : 성공
     */
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        int clearFloor = jsonParser["_frost_floor"].GetInt();
        _clearFloor = clearFloor;
        
        _timeReset = jsonParser["_ad_cool_time"].GetInt64();
        _adsCount = jsonParser["_ad_count"].GetInt();
        
        //
        int frostPoint = jsonParser["_mamon"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::FROST_PRISON_POINT, 0, frostPoint);
        
        //
        auto listSkill = jsonParser["_skills"].GetArray();
        for ( int i = 0; i < listSkill.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = listSkill[i];
            
            int skillIdx = jsonValue["_skillidx"].GetInt();
            int skillStep = jsonValue["_grade"].GetInt();
            int skillLevel = jsonValue["_level"].GetInt();
            
            setSkillStep(skillIdx, skillStep);
            setSkillLevel(skillIdx, skillLevel);
        }
        
        //
        if ( _listSkillCoolTime.size() == 0 )
        {
            auto listSkill = getListSkill();
            for ( auto& objSkill : listSkill )
            {
                setSkillCoolTime(objSkill->getIdx(), objSkill->getCoolTime());
            }
        }
        
        //
        invokeDelegateInfo(true, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, result);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        //
        _clearFloor = 0;
        
        //
        invokeDelegateInfo(false, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, result);
            _callbackInfo = nullptr;
        }
    }
}

void PrisonFrostManager::requestSkillLevel(bool bPopupLoading, int skillIdx, int levelCount)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/frost/skill";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_skillidx", skillIdx);
    req->setData("_addlevel", levelCount);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSkillLevel(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonFrostManager::responseSkillLevel(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        invokeDelegateSkillLevelUp(false, -1);
        return;
    }
    
    // { '_result' : 0, '_point' : 0 }
    /*
     0 : 실패
     1 : 성공
     */
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        int skillIdx = jsonParser["_skillidx"].GetInt();
        int skillLevel = jsonParser["_level"].GetInt();
        setSkillLevel(skillIdx, skillLevel);
            
        //
        invokeDelegateSkillLevelUp(true, result);
    }
    else
    {
        //
        invokeDelegateSkillLevelUp(false, result);
    }
}

void PrisonFrostManager::requestSkillReset(bool bPopupLoading)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/frost/skillReset";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSkillReset(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonFrostManager::responseSkillReset(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        invokeDelegateSkillReset(false, -1);
        return;
    }
    
    // { '_result' : 0, '_point' : 0 }
    /*
     0 : 실패
     1 : 성공
     */
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        for (auto iter = _listSkillLevel.begin() ; iter !=  _listSkillLevel.end(); iter++)
        {
            iter->second = 0;
        }
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        invokeDelegateSkillReset(true, result);
    }
    else
    {
        //
        invokeDelegateSkillReset(false, result);
    }
}

void PrisonFrostManager::requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackAds = callback;
    
    //
    std::string url = "/frost/adfinish";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAds(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonFrostManager::responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, -1);
            _callbackAds = nullptr;
        }
        return;
    }
    
    // {"_result": 0, "_kill_f": 0, "_count_f": 0, "_mamon": 0, "_skills": []}
    /*
     0 : 실패
     1 : 성공
     */
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _timeReset = jsonParser["_cool_time"].GetInt64();
        _adsCount = jsonParser["_count"].GetInt();
        
        //
        if ( _callbackAds != nullptr )
        {
            _callbackAds(true, result);
            _callbackAds = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, result);
            _callbackAds = nullptr;
        }
    }
}

void PrisonFrostManager::requestResult(bool bPopupLoading, int floor, bool bClear, std::string damage)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    damage = MafUtils::convertNumberToShort(damage);
    
    //
    std::string url = "/frost/complete";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_floor", floor);
    req->setData("_damage", damage);
    req->setData("_clear", bClear);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void PrisonFrostManager::responseResult(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        invokeDelegateResult(false, -1, "");
        return;
    }
    
    // { '_result' : 0, '_point' : 0 }
    /*
     0 : 실패
     1 : 성공
     */
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        invokeDelegateResult(true, result, strReward);
    }
    else if ( result == 10000 )
    {
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        invokeDelegateResult(false, result, "");
    }
    else
    {
        //
        invokeDelegateResult(false, result, "");
    }
}
