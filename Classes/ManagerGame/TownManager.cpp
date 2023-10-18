//
//  TownManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "TownManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AchievementManager.h"

USING_NS_CC;

TownManager::TownManager(void):
_ePlace(E_PLACE::DG_NORMAL),

//

//
_nSatelliteStep(0),
_nSatelliteTutorial(0)
{
    
}

TownManager::~TownManager(void)
{
    
}

bool TownManager::init()
{
    for ( int i = 0; i < sizeof(_listTownLevel) / sizeof(TInteger); i++ )
    {
        _listTownLevel[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_listTownProgress) / sizeof(std::string); i++ )
    {
        _listTownProgress[i] = "0";
    }
    
    setLoad();
    
    return true;
}

#pragma mark - update
void TownManager::update(float dt)
{
     
}

#pragma mark - data
void TownManager::saveData()
{
    std::stringstream str;
    
    //
    str.str("");
    for ( int i = 1; i <= C_COUNT_BUILDING; i++ )
    {
        str << getTownLevel((E_TOWN)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_TOWN_LEVEL, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    for ( int i = 1; i <= C_COUNT_BUILDING; i++ )
    {
        str << getBuildingProgress((E_TOWN)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_TOWN_BUILDING_PROGRESS, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    str << MafUtils::toString(_nSatelliteStep.value());
    UserDefault::getInstance()->setStringForKey(KEY_TOWN_SATELLITE_STEP, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    str << MafUtils::toString(_nSatelliteTutorial.value());
    UserDefault::getInstance()->setStringForKey(KEY_TOWN_SATELLITE_TUTORIAL, MafAes256::Encrypt(str.str()));
}

void TownManager::loadData()
{
    std::string str = "";
    
    str = UserDefault::getInstance()->getStringForKey(KEY_TOWN_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_BUILDING; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setTownLevel((E_TOWN)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setTownLevel((E_TOWN)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TOWN_BUILDING_PROGRESS);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_BUILDING; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setBuildingProgress((E_TOWN)i, result);
                
                str = str.substr(offset+1);
            }
            else
            {
                setBuildingProgress((E_TOWN)i, str);
                break;
            }
        }
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TOWN_SATELLITE_STEP);
    if ( !str.empty() )
    {
        setSatelliteStep(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    else
    {
        setSatelliteStep(0);
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TOWN_SATELLITE_TUTORIAL);
    if ( !str.empty() )
    {
        setSatelliteTutorial(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    else
    {
        setSatelliteTutorial(0);
    }
}

#pragma mark - load
void TownManager::setLoad()
{
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::TOWN_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listTownReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nType = jsonValue["type"].GetInt();
        int nCondition = jsonValue["condition"].GetInt();
        
        std::string strItems = jsonValue["reward"].GetString();
        
        
        auto obj = InfoTownReward::create();
        obj->setType(nType);
        obj->setCondition(nCondition);
        obj->setItems(strItems);
        
        _listTownReward.pushBack(obj);
    }
}

#pragma mark - get, set : town
cocos2d::Vector<InfoTownReward*> TownManager::getListTownReward()
{
    return _listTownReward;
}

InfoTownReward* TownManager::getObjectTownReward(int type, int condition)
{
    InfoTownReward* objResult = nullptr;
    
    for ( auto& obj : _listTownReward )
    {
        if ( obj->getType() == type && obj->getCondition() == condition )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

InfoTownReward* TownManager::getObjectTownRewardLast(int type)
{
    InfoTownReward* objResult = nullptr;
    
    for ( auto& obj : _listTownReward )
    {
        if ( obj->getType() == type && obj->isReward() == false )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

std::string TownManager::getTownPathIcon(E_TOWN type)
{
    std::string path = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    path = "Assets/ui/tower/icon_tower.png"; break;
        case E_TOWN::BUILDING_2:    path = "Assets/ui/tower/icon_apt.png"; break;
        case E_TOWN::BUILDING_3:    path = "Assets/ui/tower/icon_villa.png"; break;
        case E_TOWN::BUILDING_4:    path = "Assets/ui/tower/icon_mansion.png"; break;
        case E_TOWN::BUILDING_5:    path = "Assets/ui/tower/icon_building.png"; break;
        case E_TOWN::SATELLITE:     path = "Assets/ui/tower/satellite_icon.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string TownManager::getTownName(E_TOWN type)
{
    std::string text = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    text = GAME_TEXT("t_ui_town_tower"); break; //봉식타워
        case E_TOWN::BUILDING_2:    text = GAME_TEXT("t_ui_town_apart"); break; //봉파트
        case E_TOWN::BUILDING_3:    text = GAME_TEXT("t_ui_town_villa"); break; //봉빌라
        case E_TOWN::BUILDING_4:    text = GAME_TEXT("t_ui_town_mansion"); break; //봉멘션
        case E_TOWN::BUILDING_5:    text = GAME_TEXT("t_ui_town_building"); break; //봉빌딩
        case E_TOWN::SATELLITE:     text = GAME_TEXT("t_ui_bong_town_rokcet_title"); break; //인공위성
            
        default:
            break;
    }
    
    return text;
}

std::string TownManager::getTownExplain(E_TOWN type)
{
    std::string text = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    text = GAME_TEXT("t_ui_town_msg_1"); break;
        case E_TOWN::BUILDING_2:    text = GAME_TEXT("t_ui_town_msg_2"); break;
        case E_TOWN::BUILDING_3:    text = GAME_TEXT("t_ui_town_msg_3"); break;
        case E_TOWN::BUILDING_4:    text = GAME_TEXT("t_ui_town_msg_4"); break;
        case E_TOWN::BUILDING_5:    text = GAME_TEXT("t_ui_town_msg_5"); break;
        case E_TOWN::SATELLITE:     text = GAME_TEXT("t_ui_town_msg_1"); break;
            
        default:
            break;
    }
    
    return text;
}


double TownManager::getTownEffect(E_TOWN type, int level)
{
    double effect = 0;
    
    if ( level > 2000 )
    {
        level = 2000;
    }
    
    if ( type == E_TOWN::BUILDING_1 || type == E_TOWN::BUILDING_2 )
    {
        double value = 1.0f;
        int evol = 10;
        
        for ( int i=1; i<=level; i++ )
        {
            if ( i > 1 )
            {
                if ( (i-1) % evol == 0 )
                    value *= 2;
            }
            effect += value;
        }
    }
    else if ( type == E_TOWN::BUILDING_3 )
    {
        effect = level * 1.0f;
    }
    else if ( type == E_TOWN::BUILDING_4 || type == E_TOWN::BUILDING_5 )
    {
        double value = 1.0f;
        int evol = 20;
        
        for ( int i=1; i<=level; i++ )
        {
            if ( i > 1 )
            {
                if ( (i-1) % evol == 0 )
                    value *= 2;
            }
            effect += value;
        }
    }
    else if ( type == E_TOWN::SATELLITE )
    {
        double value = 1.0f;
        int evol = 10;
        if ( level > 0 )
        {
            level += getTownLevel(E_TOWN::BUILDING_1);
            
            double minus = 0.0;
            for ( int i=1; i<=level; i++ )
            {
                if ( i > 1 )
                {
                    if ( (i-1) % evol == 0 )
                        value *= 2;
                }
                effect += value;
                
                if ( i == 700 )
                {
                    minus = effect;
                }
            }
            
            effect -= minus;
        }
    }
    
    return effect;
}

int TownManager::getTownLevelMax(E_TOWN type)
{
    int level = 0;
    
    if ( type == E_TOWN::BUILDING_1)        level = 700; //1000레벨이상올리면안된다.
    else if ( type == E_TOWN::BUILDING_2)   level = 700;
    else if ( type == E_TOWN::BUILDING_3)   level = 700;
    else if ( type == E_TOWN::BUILDING_4)   level = 700;
    else if ( type == E_TOWN::BUILDING_5)   level = 700;
    else if ( type == E_TOWN::SATELLITE)    level = 1000;
    
    return level;
}

int TownManager::getTownLevel(E_TOWN type)
{
    int level = _listTownLevel[type].valueInt();
    int levelMax = getTownLevelMax(type);
    
    if ( level > levelMax )
    {
        _listTownLevel[type] = levelMax;
        level = levelMax;
    }
    
    return level;
}
void TownManager::setTownLevel(E_TOWN type, int level)
{
    _listTownLevel[type] = level;
}

std::string TownManager::getTownPriceCoin(E_TOWN type, int level)
{
    std::string price = "100";
    
    if ( type == E_TOWN::SATELLITE )
    {
        price = getTownPriceCoin(E_TOWN::BUILDING_1, getTownLevelMax(E_TOWN::BUILDING_1));
        std::string priceAppend(level, '0');
        price.append(priceAppend);
    }
    else
    {
        if      ( type == E_TOWN::BUILDING_1 )     price = "1000000000000000000000000000000";
        else if ( type == E_TOWN::BUILDING_2 )     price = "1000000000000000000000000000000000";
        else if ( type == E_TOWN::BUILDING_3 )     price = "1000000000000000000000000000000000";
        else if ( type == E_TOWN::BUILDING_4 )     price = "1000000000000000000000000000000000";
        else if ( type == E_TOWN::BUILDING_5 )     price = "1000000000000000000000000000000000";
        
        double value = 0;
        int remainLevel = level%1023;
        if ( remainLevel == 0 )
        {
            value = 1;
        }
        else
        {
            value = std::pow(2, (remainLevel));
        }
        price = MafUtils::bigMulNum(price, MafUtils::doubleToString(value));
        
        
        //
        if ( type == E_TOWN::BUILDING_1 )
        {
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_28) )
            {// 파라오의 갑주
                price.pop_back(); //90%감소합니다. 즉10%만 남기면된다.
            }
        }
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_32) )
        {// VIP 망토
            int level = UserInfoManager::getInstance()->getAttendVip();
            
            int per = 100 - (level * 9);
            if (per > 0) {
                price = MafUtils::bigMulNum(price, MafUtils::toString(per));
                price.pop_back();
                price.pop_back();
            }
        }
    }
    
    
    
    return price;
}

int TownManager::getTownPriceGem(E_TOWN type, int level)
{
    int price = 0;
    
    if ( type == E_TOWN::BUILDING_1 )
    {
        price = 100;
        price += (level/10) * 100;
    }
    else if ( type == E_TOWN::SATELLITE )
    {// 1만+int(위성 레벨*0.1)*100
        price = 10000 + (int)(level * 0.1) * 100;
    }
    
    return price;
}

#pragma mark - get, set : building
std::string TownManager::getBuildingPathFloorTop(E_TOWN type)
{
    std::string path = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    path = "Assets/ui/tower/tower5_top.png"; break;
        case E_TOWN::BUILDING_2:    path = "Assets/ui/tower/tower1_top.png"; break;
        case E_TOWN::BUILDING_3:    path = "Assets/ui/tower/tower2_top.png"; break;
        case E_TOWN::BUILDING_4:    path = "Assets/ui/tower/tower3_top.png"; break;
        case E_TOWN::BUILDING_5:    path = "Assets/ui/tower/tower4_top.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string TownManager::getBuildingPathFloorBottom(E_TOWN type)
{
    std::string path = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    path = "Assets/ui/tower/tower5_bottom.png"; break;
        case E_TOWN::BUILDING_2:    path = "Assets/ui/tower/tower1_bottom.png"; break;
        case E_TOWN::BUILDING_3:    path = "Assets/ui/tower/tower2_bottom.png"; break;
        case E_TOWN::BUILDING_4:    path = "Assets/ui/tower/tower3_bottom.png"; break;
        case E_TOWN::BUILDING_5:    path = "Assets/ui/tower/tower4_bottom.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string TownManager::getBuildingPathFloor(E_TOWN type)
{
    std::string path = "";
    switch (type) {
        case E_TOWN::BUILDING_1:    path = "Assets/ui/tower/tower5_middle.png"; break;
        case E_TOWN::BUILDING_2:    path = "Assets/ui/tower/tower1_middle.png"; break;
        case E_TOWN::BUILDING_3:    path = "Assets/ui/tower/tower2_middle.png"; break;
        case E_TOWN::BUILDING_4:    path = "Assets/ui/tower/tower3_middle.png"; break;
        case E_TOWN::BUILDING_5:    path = "Assets/ui/tower/tower4_middle.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string TownManager::getBuildingProgress(E_TOWN type)
{
    return _listTownProgress[type];
}

void TownManager::setBuildingProgress(E_TOWN type, std::string progress)
{
    _listTownProgress[type] = progress;
}

int TownManager::getBuildingUpgradeExpectLevel(E_TOWN type, std::string gold)
{
    int levelMax = getTownLevelMax(type);
    int levelExpect = getTownLevel(type);
    
    std::string currentProgress = getBuildingProgress(type);
    
    while(true)
    {
        std::string needCoin = getTownPriceCoin(type, levelExpect);
        needCoin = MafUtils::bigSubNum(needCoin, currentProgress);
        
        if ( levelExpect >= levelMax )
        {
            break;
        }
        
        if (!MafUtils::compareStr(gold, needCoin))
        {
            break;
        }
        
        gold = MafUtils::bigSubNum(gold, needCoin);
        
        levelExpect++;
        currentProgress = "0";
    }
    
    return levelExpect;
}

int TownManager::getBuildingUpgradeExpectPercent(E_TOWN type, std::string gold)
{
    int levelMax = getTownLevelMax(type);
    int levelExpect = getTownLevel(type);
    
    std::string currentProgress = getBuildingProgress(type);
    
    while(true)
    {
        std::string needCoin = getTownPriceCoin(type, levelExpect);
        needCoin = MafUtils::bigSubNum(needCoin, currentProgress);
        
        if ( levelExpect >= levelMax )
        {
            break;
        }
        
        if (!MafUtils::compareStr(gold, needCoin))
        {
            break;
        }
        
        gold = MafUtils::bigSubNum(gold, needCoin);
        
        levelExpect++;
        currentProgress = "0";
    }
    currentProgress = MafUtils::bigAddNum(gold, currentProgress);
    
    //
    int percent = 0;
    {
        std::string tmpCoin = getTownPriceCoin(type, levelExpect);
        tmpCoin.pop_back();
        tmpCoin.pop_back();
        std::string addCoin = tmpCoin;
        
        for(int i=0; i <100; i++)
        {
            if(!MafUtils::compareStr(currentProgress, addCoin))
            {
                break;
            }
            addCoin = MafUtils::bigAddNum(addCoin, tmpCoin);
            percent++;
        }
    }
    
    return percent;
}

#pragma mark - get, set : satellite
int TownManager::getSatelliteStep()
{
    return _nSatelliteStep.valueInt();
}

void TownManager::setSatelliteStep(int step)
{
    _nSatelliteStep = step;
}

int TownManager::getSatelliteTutorial()
{
    return _nSatelliteTutorial.valueInt();
}

void TownManager::setSatelliteTutorial(int tutorial)
{
    _nSatelliteTutorial = tutorial;
}

#pragma mark - event
int TownManager::onTownUpgrade(E_TOWN type, std::string gold)
{
    /*
     0 : 성공
     1 : 가격 부족
     2 : 최대 레벨
     */
    E_TOWN eType = type;
    
    int level = getTownLevel(eType);
    int levelMax = getTownLevelMax(eType);
    if ( level >= levelMax )
    {
        return 2;
    }
    
    while(true)
    {
        level = getTownLevel(eType);
        if ( level >= levelMax )
        {
            break;
        }
        
        std::string needCoin = getTownPriceCoin(eType, level);
        needCoin = MafUtils::bigSubNum(needCoin, getBuildingProgress(eType));
        if (!MafUtils::compareStr(gold, needCoin))
        {
            break;
        }
        
        gold = MafUtils::bigSubNum(gold, needCoin);
        
        setTownLevel(eType, level+1);
        setBuildingProgress(eType, "0");
    }
    level = getTownLevel(eType);
    
    //
    std::string progressCoin;
    if ( level >= levelMax )
    {
        progressCoin = "0";
    }
    else
    {
        progressCoin = getBuildingProgress(eType);
        progressCoin = MafUtils::bigAddNum(progressCoin, gold);
    }
    setBuildingProgress(eType, progressCoin);
    
    //
    ItemsMoneyManager::getInstance()->setGoldZero();
    
    //업적
    switch (eType) {
        case E_TOWN::BUILDING_1: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_1, level); break;
        case E_TOWN::BUILDING_2: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_2, level); break;
        case E_TOWN::BUILDING_3: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_3, level); break;
        case E_TOWN::BUILDING_4: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_4, level); break;
        case E_TOWN::BUILDING_5: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_5, level); break;
            
        default:
            break;
    }
    
    //
    saveData();
    
    return 0;
}

int TownManager::onTownUpgradeTower()
{
    /*
     0 : 성공
     1 : 가격 부족
     2 : 최대 레벨
     */
    
    E_TOWN eType = E_TOWN::BUILDING_1;
    
    int level = getTownLevel(eType);
    int levelMax = getTownLevelMax(eType);
    if ( level >= levelMax )
    {
        return 2;
    }
    
    std::string price = getTownPriceCoin(eType, level);
    std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
    
    int priceGem = getTownPriceGem(eType, level);
    int currencyGem = ItemsMoneyManager::getInstance()->getGem();
    
    
    if ( MafUtils::compareStr(currencyGold, price) == false )
    {
        return 1;
    }
    
    if ( currencyGem < priceGem)
    {
        return 1;
    }
    
    price.insert(0, "-");
    ItemsMoneyManager::getInstance()->setGold(price);
    ItemsMoneyManager::getInstance()->setGem(-priceGem);
    
    level = level + 1;
    setTownLevel(eType, level);
    
    //업적
    switch (eType) {
        case E_TOWN::BUILDING_1: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_1, level); break;
        case E_TOWN::BUILDING_2: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_2, level); break;
        case E_TOWN::BUILDING_3: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_3, level); break;
        case E_TOWN::BUILDING_4: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_4, level); break;
        case E_TOWN::BUILDING_5: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_5, level); break;
            
        default:
            break;
    }
    
    //
    saveData();
    
    return 0;
}

int TownManager::onTownUpgradeSatellite()
{
    /*
     0 : 성공
     1 : 가격 부족
     2 : 최대 레벨
     */
    
    E_TOWN eType = E_TOWN::SATELLITE;
    
    int level = getTownLevel(eType);
    int levelMax = getTownLevelMax(eType);
    if ( level >= levelMax )
    {
        return 2;
    }
    
    std::string price = getTownPriceCoin(eType, level);
    std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
    
    int priceGem = getTownPriceGem(eType, level);
    int currencyGem = ItemsMoneyManager::getInstance()->getGem();
    
    
    if ( MafUtils::compareStr(currencyGold, price) == false )
    {
        return 1;
    }
    
    if ( currencyGem < priceGem)
    {
        return 1;
    }
    
    price.insert(0, "-");
    ItemsMoneyManager::getInstance()->setGold(price);
    ItemsMoneyManager::getInstance()->setGem(-priceGem);
    
    level = level + 1;
    setTownLevel(eType, level);
    
    //
    saveData();
    
    return 0;
}

#pragma mark - network
void TownManager::requestInfo(const std::function<void(bool)>& callback)
{
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/user/town/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
    
}

void TownManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
    
    //
    for ( auto& obj : _listTownReward )
    {
        obj->setReward(false);
    }
    
    //
    const rapidjson::Value& jsonList = jsonParser["_town_info_list"];
    for ( int i = 0; i < jsonList.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonList[i];
    
        int nType = jsonValue["type"].GetInt();
        for ( int j = 0; j < jsonValue["rewards"].Size(); j++ )
        {
            
            int nCondition = jsonValue["rewards"][j].GetInt();
            auto objTownReward = getObjectTownReward(nType, nCondition);
            if ( objTownReward != nullptr )
            {
                objTownReward->setReward(true);
            }
        }
    }
    
    // callback
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
        _callbackInfo = nullptr;
    }
}

void TownManager::requestReward(int type, int condition, const std::function<void(bool, int)>& callback)
{
    //
    _callbackReward = callback;
    
    //
    std::string url = "/user/town/reward";

    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_condition", condition);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void TownManager::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, -1);
            _callbackReward = nullptr;
        }
        return;
    }
    
    /*
    # 응답
    '_result' : 0 # 3 : 이미 획득
    '_town_info_list' : [],
    '_reward' : [],
    '_items' : [],
     */
    int nResult = jsonParser["_result"].GetInt();
    
    if ( nResult == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        for ( auto& obj : _listTownReward )
        {
            obj->setReward(false);
        }
        
        //
        const rapidjson::Value& jsonList = jsonParser["_town_info_list"];
        for ( int i = 0; i < jsonList.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonList[i];
        
            int nType = jsonValue["type"].GetInt();
            for ( int j = 0; j < jsonValue["rewards"].Size(); j++ )
            {
                
                int nCondition = jsonValue["rewards"][j].GetInt();
                auto objTownReward = getObjectTownReward(nType, nCondition);
                if ( objTownReward != nullptr )
                {
                    objTownReward->setReward(true);
                }
            }
        }
        
        if ( _callbackReward != nullptr )
        {
            _callbackReward(true, nResult);
            _callbackReward = nullptr;
        }
        
        //
        saveData();
    }
    else
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, nResult);
            _callbackReward = nullptr;
        }
    }
    
}
