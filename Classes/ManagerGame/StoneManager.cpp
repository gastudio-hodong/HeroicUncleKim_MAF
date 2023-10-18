//
//  StoneManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "StoneManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ServerMissionManager.h"

USING_NS_CC;

StoneManager::StoneManager(void) :
_ePlace(E_PLACE::DG_NORMAL)

{
    
}

StoneManager::~StoneManager(void)
{
    
}

bool StoneManager::init()
{
    for ( int i = 0; i < sizeof(_uStone) / sizeof(TInteger); i++ )
    {
        _uStone[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uStoneLocal) / sizeof(TInteger); i++ )
    {
        _uStoneLocal[i] = 0;
    }
    
    _uStoneStorage = 0;
    
    return true;
}

#pragma mark - update
void StoneManager::update(float dt)
{
     
}

#pragma mark - data
void StoneManager::saveData()
{
    std::stringstream str;
    
    //
    str.str("");
    str << getStorageCountNow();
    UserDefault::getInstance()->setStringForKey(KEY_ENCHANT_STONE_STORAGE, MafAes256::Encrypt(str.str()));
    
    // stone server
    str.str("");
    for ( int i = 1; i <= C_COUNT_ENCHANT_STONE; i++ )
    {
        str << getCountNow(E_STONE_TYPE::SERVER, (E_STONE)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_ENCHANT_STONE, MafAes256::Encrypt(str.str()));
    
    // stone local
    str.str("");
    for ( int i = 1; i <= C_COUNT_ENCHANT_STONE; i++ )
    {
        str << getCountNow(E_STONE_TYPE::LOCAL, (E_STONE)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_ENCHANT_STONE_LOCAL, MafAes256::Encrypt(str.str()));
}

void StoneManager::loadData()
{
    std::string str = "";
    
    // storage
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ENCHANT_STONE_STORAGE);
    if ( !str.empty() )
    {
        setStorageCount(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    // stone server
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ENCHANT_STONE);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for (int i = 1; i <= C_COUNT_ENCHANT_STONE; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setCount(E_STONE_TYPE::SERVER, (E_STONE)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setCount(E_STONE_TYPE::SERVER, (E_STONE)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // stone local
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ENCHANT_STONE_LOCAL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i = 1; i <= C_COUNT_ENCHANT_STONE; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setCount(E_STONE_TYPE::LOCAL, (E_STONE)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setCount(E_STONE_TYPE::LOCAL, (E_STONE)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
}

#pragma mark - load
void StoneManager::setLoad()
{
    
}

#pragma mark - info

#pragma mark - get, set : stone
std::string StoneManager::getPath(E_STONE stone)
{
    auto path = std::string("");
    
    switch (stone) {
        case E_STONE::ATTACK:           path = "Assets/icon/socket_jewelry_1.png"; break;
        case E_STONE::GOLD:             path = "Assets/icon/socket_jewelry_2.png"; break;
        case E_STONE::KEY:              path = "Assets/icon/socket_jewelry_3.png"; break;
        case E_STONE::FORCE_ATTACK:     path = "Assets/icon/socket_jewelry_5.png"; break;
        case E_STONE::FORCE_GOLD:       path = "Assets/icon/socket_jewelry_6.png"; break;
        case E_STONE::FORCE_KEY:        path = "Assets/icon/socket_jewelry_7.png"; break;
        case E_STONE::FORCE_2_ATTACK:   path = "Assets/icon/socket_jewelry_8.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string StoneManager::getName(E_STONE stone)
{
    auto name = std::string("");
    
    switch (stone) {
        case E_STONE::ATTACK:           name = GAME_TEXT("t_ui_stone_special_1"); break;
        case E_STONE::GOLD:             name = GAME_TEXT("t_ui_stone_special_2"); break;
        case E_STONE::KEY:              name = GAME_TEXT("t_ui_stone_special_3"); break;
        case E_STONE::FORCE_ATTACK:     name = GAME_TEXT("t_ui_stone_special_4"); break;
        case E_STONE::FORCE_GOLD:       name = GAME_TEXT("t_ui_stone_special_5"); break;
        case E_STONE::FORCE_KEY:        name = GAME_TEXT("t_ui_stone_special_6"); break;
        case E_STONE::FORCE_2_ATTACK:   name = GAME_TEXT("t_ui_stone_special_7"); break;
            
        default:
            break;
    }
    
    return name;
}

std::string StoneManager::getEffectName(E_STONE stone)
{
    auto name = std::string("");
    
    switch (stone) {
        case E_STONE::ATTACK:           name = GAME_TEXT("t_ui_costume_stone_option_1"); break;
        case E_STONE::GOLD:             name = GAME_TEXT("t_ui_costume_stone_option_2"); break;
        case E_STONE::KEY:              name = GAME_TEXT("t_ui_costume_stone_option_3"); break;
        case E_STONE::FORCE_ATTACK:     name = GAME_TEXT("t_ui_costume_stone_option_4"); break;
        case E_STONE::FORCE_GOLD:       name = GAME_TEXT("t_ui_costume_stone_option_5"); break;
        case E_STONE::FORCE_KEY:        name = GAME_TEXT("t_ui_costume_stone_option_6"); break;
        case E_STONE::FORCE_2_ATTACK:   name = GAME_TEXT("t_ui_costume_stone_option_7"); break;
            
        default:
            break;
    }
    
    return name;
}


int StoneManager::getPrice(E_STONE stone, int count)
{
    int price = 0;
    
    switch (stone) {
        case E_STONE::ATTACK:
        case E_STONE::GOLD:
        case E_STONE::KEY:
        {
            price = 70;
            break;
        }
            
        case E_STONE::FORCE_ATTACK:
        case E_STONE::FORCE_GOLD:
        case E_STONE::FORCE_KEY:
        {
            if ( count % 6 == 0 )
            {// 봉트리움
                price = 5;
            }
            else
            {// 마왕의 코인
                price = count / 6 + 10;
                price = MIN(price, 200);
            }
            break;
        }
            
        case E_STONE::FORCE_2_ATTACK:
        {
            if ( count % 6 == 0 )
            {// 봉트리움
                price = count / 6 + 4;
                price = MIN(price, 50);
            }
            else
            {// 마왕의 코인
                price = count / 6 + 50;
                price = MIN(price, 300);
            }
            break;
        }
            
        default:
            break;
    }
    
    return price;
}

cocos2d::Vector<InfoItems*> StoneManager::getCost(E_STONE stone, int count)
{
    Vector<InfoItems*> listResult;
    Vector<InfoItems*> listCost;
    
    int countNow = getCountNow(E_STONE_TYPE::ALL, stone);
    int countMax = getCountMax(stone);
    int countBuy = countNow + count;
    countBuy = MIN(countBuy, countMax);
    
    for ( int i = countNow + 1; i <= countBuy; i++ )
    {
        InfoItems *objItem = nullptr;
        
        int price = getPrice(stone, i);
        
        switch (stone) {
            case E_STONE::ATTACK:
            case E_STONE::GOLD:
            case E_STONE::KEY:
            {
                objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
                break;
            }
                
            case E_STONE::FORCE_ATTACK:
            case E_STONE::FORCE_GOLD:
            case E_STONE::FORCE_KEY:
            {
                if ( i % 6 == 0 )
                {
                    objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::MATERIAL);
                }
                else
                {
                    objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
                }
                break;
            }
                
            case E_STONE::FORCE_2_ATTACK:
            {
                if ( i % 6 == 0 )
                {
                    objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::MATERIAL);
                }
                else
                {
                    objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
                }
                break;
            }
                
            default:
                break;
        }
        
        if ( objItem != nullptr )
        {
            objItem->setCount(price);
            
            listCost.pushBack(objItem);
        }
    }
    
    for ( int j = 0; j < listCost.size(); j++ )
    {
        auto objItem = listCost.at(j);

        bool bHave = false;
        for ( auto& obj : listResult )
        {
            if ( obj->getIdx() == objItem->getIdx() )
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
            listResult.pushBack(objItemClone);
        }
    }
    
    return listResult;
}

double StoneManager::getEffect(E_STONE stone, int count/* = -1*/)
{
    if (count == -1 )
    {
        count = getCountNow(E_STONE_TYPE::ALL, stone);
    }
    
    int countMax = getCountMax(stone);
    count = MIN(count, countMax); //4300개 이상 보유 시 계산 오류 추후 수정 필요, 기존 강화석 3종에만 사용할 것!
    
    double value = 1.0;
    if ( stone == E_STONE::ATTACK && count > 0 )
    {
        value *= std::powl(1.15, MIN(count, 200));
        if (  count > 200 )
            value *= std::powl(1.13, MIN(count, 400) - 200);
        if ( count > 400 )
            value *= std::powl(1.12, count - 400);
    }
    else if ( stone == E_STONE::GOLD && count > 0 )
    {
        value *= std::powl(1.25, MIN(count, 200));
        if (  count > 200 )
            value *= std::powl(1.20, MIN(count, 400) - 200);
        if ( count > 400 )
            value *= std::powl(1.17, count - 400);
    }
    else if ( stone == E_STONE::KEY && count > 0 )
    {
        value *= std::powl(1.01, count);
    }
    else if ( stone == E_STONE::FORCE_ATTACK && count > 0 )
    {
        int progress = count % 6;
        int step = count / 6;
        
        value = std::pow(1.6, step * 1.06 + (progress / 10.0) + 1);
    }
    else if ( stone == E_STONE::FORCE_GOLD && count > 0 )
    {
        int progress = count % 6;
        int step = count / 6;
        
        value = std::pow(1.6, step * 1.4 + (progress / 10.0) + 1);
    }
    else if ( stone == E_STONE::FORCE_KEY && count > 0 )
    {
        int progress = count % 6;
        int step = count / 6;
        
        value = std::pow(1.1, step * 1.2 + (progress / 10.0) + 1);
    }
    else if ( stone == E_STONE::FORCE_2_ATTACK && count > 0 )
    {
        int progress = count % 6;
        int step = count / 6;
        
        value = std::pow(1.5, step * 1.15 + (progress / 10.0) + 1);
    }
    
    value -= 1;
    return value;
}

int StoneManager::getCountAll()
{
    int count = 0;
    
    for ( int i = 1; i <= C_COUNT_ENCHANT_STONE; i++ )
    {
        if ( i >= (int)E_STONE::FORCE_ATTACK )
        {
            count = count + getCountNow(E_STONE_TYPE::ALL, (E_STONE)i) / 6;
        }
        else
        {
            count += getCountNow(E_STONE_TYPE::ALL, (E_STONE)i);
        }
    }
    return count;
}

int StoneManager::getCountMax(E_STONE stone)
{
    int count = 0;
    if ( stone == E_STONE::ATTACK || stone == E_STONE::GOLD || stone == E_STONE::KEY )
    {
        count = 4300;
    }
    else if ( stone == E_STONE::FORCE_ATTACK || stone == E_STONE::FORCE_GOLD || stone == E_STONE::FORCE_KEY )
    {
        count = 4001;
    }
    else if ( stone == E_STONE::FORCE_2_ATTACK || stone == E_STONE::FORCE_2_GOLD || stone == E_STONE::FORCE_2_KEY )
    {
        count = 4205;
    }
    
    return count;
}

int StoneManager::getCountNow(E_STONE_TYPE type, E_STONE stone)
{
    int count = 0;
    if ( type == E_STONE_TYPE::SERVER )
    {
        count = _uStone[(int)stone].valueInt();
    }
    else if ( type == E_STONE_TYPE::LOCAL )
    {
        if ( stone > E_STONE::KEY )
        {
            count = 0;
        }
        else
        {
            count = _uStoneLocal[(int)stone].valueInt();
        }
    }
    else if ( type == E_STONE_TYPE::ALL )
    {
        if ( stone > E_STONE::KEY )
        {
            count = _uStone[(int)stone].valueInt();
        }
        else
        {
            count = _uStone[(int)stone].valueInt() + _uStoneLocal[(int)stone].valueInt();
        }
    }
    
    return count;
}
void StoneManager::setCount(E_STONE_TYPE type, E_STONE stone, int count)
{
    if(type == E_STONE_TYPE::SERVER)
        _uStone[(int)stone] = count;
    else if(type == E_STONE_TYPE::LOCAL)
        _uStoneLocal[(int)stone] = count;
}

#pragma mark - get, set : storage
std::string StoneManager::getStoragePath()
{
    return "Assets/icon/icon_stone.png";
}

std::string StoneManager::getStorageName()
{
    return GAME_TEXT("t_ui_costume_msg_39");
}

int StoneManager::getStoragePrice()
{
    return C_ENCHANT_STONE_STORAGE_PRICE;
}

cocos2d::Vector<InfoItems*> StoneManager::getStorageCost(int count)
{
    Vector<InfoItems*> listResult;
    Vector<InfoItems*> listCost;
    
    int countNow = getStorageCountNow();
    int countBuy = countNow + count;
    
    int price = getStoragePrice();
    
    for ( int i = countNow + 1; i <= countBuy; i++ )
    {
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        objItem->setCount(price);
        
        listCost.pushBack(objItem);
    }
    
    for ( int j = 0; j < listCost.size(); j++ )
    {
        auto objItem = listCost.at(j);

        bool bHave = false;
        for ( auto& obj : listResult )
        {
            if ( obj->getIdx() == objItem->getIdx() )
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
            listResult.pushBack(objItemClone);
        }
    }
    
    return listResult;
}

int StoneManager::getStorageCountNow()
{
    return _uStoneStorage.valueInt();
}

void StoneManager::setStorageCount(int count)
{
    _uStoneStorage = count;
}

#pragma mark - event
int StoneManager::onBuy(E_STONE stone, int count, const std::function<void(bool, int)>& callback)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     3 : 가방 부족
     */
    int countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, stone);
    int countMax = StoneManager::getInstance()->getCountMax(stone);
    int countAll = StoneManager::getInstance()->getCountAll();
    int storageCount = StoneManager::getInstance()->getStorageCountNow();
    
    if ( countNow + count > countMax )
    {
        return 1;
    }
    
    count = MIN(count, countMax - countNow);
    
    if ( count <= 0 )
    {
        return 1;
    }
    
    if ( countAll + count > storageCount )
    {
        return 3;
    }
    
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        return 1;
    }
    
    auto listCost = getCost(stone, count);
    
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto cost = listCost.at(i);
        
        auto itemNeed = cost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 2;
        }
    }
    
    //
    requestBuy(true, stone, count, callback);
    return 0;
}

int StoneManager::onStorageBuy(int count)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    
    int countNow = getStorageCountNow();
    int countBuy = countNow + count;
    
    if ( count <= 0 )
    {
        return 1;
    }
    
    auto listCost = getStorageCost(count);
    
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto cost = listCost.at(i);
        
        auto itemNeed = cost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 2;
        }
    }
    
    //
    setStorageCount(countBuy);
    
    //
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto item = listCost.at(i);
        ItemsManager::getInstance()->subItem(item);
    }
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::STONE_STORAGE_BUY);
    
    return 0;
}

#pragma mark - network
void StoneManager::requestInfo(bool bPopupLoading, const std::function<void(bool)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    std::string url = "/costume/v6/usercoin/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void StoneManager::responseInfo(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response, std::string &data)
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
        if ( callback != nullptr )
        {
            callback(false);
        }
        return;
    }

    int nCurrencyCoin = jsonParser["_coin"].GetInt();
    int nCurrencyBong = jsonParser["_material"].GetInt();
    int nCurrencyHonorPoint = jsonParser["_honor"].GetInt();
    int nCurrentEventRaceSkinTicket = jsonParser["_race_skin_ticket"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCurrencyBong);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, nCurrencyHonorPoint);
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_RACE_SKIN, 0, nCurrentEventRaceSkinTicket);
     
    
    //
    std::map<E_STONE, int> serverCount;
    serverCount[E_STONE::ATTACK] = jsonParser[("_stone1")].GetInt();
    serverCount[E_STONE::GOLD] = jsonParser[("_stone2")].GetInt();
    serverCount[E_STONE::KEY] = jsonParser[("_stone3")].GetInt();
    serverCount[E_STONE::FORCE_ATTACK] = jsonParser[("_stone4")].GetInt();
    serverCount[E_STONE::FORCE_GOLD] = jsonParser[("_stone5")].GetInt();
    serverCount[E_STONE::FORCE_KEY] = jsonParser[("_stone6")].GetInt();
    serverCount[E_STONE::FORCE_2_ATTACK] = jsonParser[("_stone7")].GetInt();
    
    std::map<E_STONE, int> userCount;
    userCount[E_STONE::ATTACK] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::ATTACK);
    userCount[E_STONE::GOLD] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::GOLD);
    userCount[E_STONE::KEY] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::KEY);
    userCount[E_STONE::FORCE_ATTACK] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::FORCE_ATTACK);
    userCount[E_STONE::FORCE_GOLD] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::FORCE_GOLD);
    userCount[E_STONE::FORCE_KEY] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::FORCE_KEY);
    userCount[E_STONE::FORCE_2_ATTACK] = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::FORCE_2_ATTACK);

    if ( userCount[E_STONE::ATTACK] < serverCount[E_STONE::ATTACK] )
        StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::ATTACK, serverCount[E_STONE::ATTACK]);
    
    if ( userCount[E_STONE::GOLD] < serverCount[E_STONE::GOLD] )
        StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::GOLD, serverCount[E_STONE::GOLD]);
    
    if ( userCount[E_STONE::KEY] < serverCount[E_STONE::KEY] )
        StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::KEY, serverCount[E_STONE::KEY]);
    
    StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::FORCE_ATTACK, serverCount[E_STONE::FORCE_ATTACK]);
    StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::FORCE_GOLD, serverCount[E_STONE::FORCE_GOLD]);
    StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::FORCE_KEY, serverCount[E_STONE::FORCE_KEY]);
    StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::FORCE_2_ATTACK, serverCount[E_STONE::FORCE_2_ATTACK]);
    
    // callback
    if ( callback != nullptr )
    {
        callback(true);
    }
}

void StoneManager::requestBuy(bool bPopupLoading, E_STONE eType, int nCount, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/costume/v4/buystone";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_stonetype", (int)eType);
    req->setData("_stonecount", nCount);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuy(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void StoneManager::responseBuy(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( callback != nullptr )
        {
            callback(false, 0);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        E_STONE stone = (E_STONE)jsonParser["_stonetype"].GetInt();
        int countNowServer = jsonParser["_stoneNowCount"].GetInt();
        int countBuyServer = jsonParser["_stoneBuyCount"].GetInt();
        
        //
        if ( stone == E_STONE::ATTACK || stone == E_STONE::GOLD || stone == E_STONE::KEY )
        {
            int countNow = getCountNow(E_STONE_TYPE::SERVER, stone);
            setCount(E_STONE_TYPE::SERVER, stone, countNow + countBuyServer);
        }
        else
        {
            setCount(E_STONE_TYPE::SERVER, stone, countNowServer);
        }
        saveData();
        

        //
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::ESTONE_BUY, countBuyServer);
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::STONE_BUY);
        
        //
        if ( callback != nullptr )
        {
            callback(true, nResult);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, nResult);
        }
    }

    
}
