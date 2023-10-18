//
//  ItemsManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "ItemsManager.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownload.h"

#include "ManagerEvent/EventBingoManager.h"
#include "ManagerEvent/EventTradingMarketManager.h"
#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerEvent/EventVoteManager.h"
#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/MonsterManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

ItemsManager::ItemsManager(void) :
_nParameterArtifact(0)

{
    
}

ItemsManager::~ItemsManager(void)
{
    
}

bool ItemsManager::init()
{
    
    // load
    setLoad();
    
    return true;
}

#pragma mark - load
void ItemsManager::setLoad()
{
    std::string strData = TableManager::getInstance()->getTableData(E_TABLE::ITEMS);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoItems.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["idx"].GetInt();
        std::string strTextKey = jsonValue["text_key"].GetString();
        std::string strCommentKey = jsonValue["comment_key"].GetString();
        std::string strPath = jsonValue["path"].GetString();
        
        std::string strType = jsonValue["type"].GetString();
        std::string strTypeDetail = jsonValue["type_detail"].GetString();
        std::string strTypeName = jsonValue["type_server"].GetString();
        int nTypeParams = jsonValue["type_params"].GetInt();
        
        std::string strReward = jsonValue["reward"].GetString();
        
        bool bString = jsonValue["is_string"].GetInt();
        bool bServer = jsonValue["is_server"].GetInt();
        
        std::string strVersion = jsonValue["version"].GetString();
        
        // obj
        auto obj = InfoItems::create(nIdx);
        obj->setTextKey(strTextKey);
        obj->setCommentKey(strCommentKey);
        obj->setPath(strPath);
        
        obj->setType(strType);
        obj->setTypeDetail(strTypeDetail);
        obj->setTypeName(strTypeName);
        obj->setTypeParams(nTypeParams);
        
        obj->setString(bString);
        obj->setServer(bServer);
        
        obj->setReward(strReward);
        
        obj->setVersion(strVersion);
        
        
        //
        _listInfoItems.pushBack(obj);
    }
}

#pragma mark - set, get : Parameter
int ItemsManager::getParameterArtifact()
{
    return _nParameterArtifact;
}

void ItemsManager::setParameterArtifact(int nData)
{
    _nParameterArtifact = nData;
}

#pragma mark - convert
InfoItems* ItemsManager::getConvert(std::string strItem)
{
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strItem.c_str());
    if( jsonParser.HasParseError() )
    {
        return nullptr;
    }
    
    int idx = 0;
    
    if(jsonParser.HasMember("idx"))
        idx = jsonParser["idx"].GetInt();
    else if(jsonParser.HasMember("item_idx"))
        idx = jsonParser["item_idx"].GetInt();
    
    auto objReward = getInfoItems(idx);
    if ( objReward == nullptr )
        return nullptr;
    
    int typeParams = objReward->getTypeParams();
    
    int tmpParams = 0;
    if(jsonParser.HasMember("type_params"))
        tmpParams = jsonParser["type_params"].GetInt();
    else if(jsonParser.HasMember("item_type_params"))
        tmpParams = jsonParser["item_type_params"].GetInt();
    
    if(tmpParams != 0)
        typeParams = tmpParams;
    
    if(jsonParser.HasMember("count"))
    {
        if ( jsonParser["count"].IsString() == true )
            objReward->setCount(jsonParser["count"].GetString());
        else
            objReward->setCount(jsonParser["count"].GetInt());
    }
    
    if(jsonParser.HasMember("item_count"))
    {
        if ( jsonParser["item_count"].IsString() == true )
            objReward->setCount(jsonParser["item_count"].GetString());
        else
            objReward->setCount(jsonParser["item_count"].GetInt());
    }
    
    objReward->setTypeParams(typeParams);
    
    return objReward;
}

Vector<InfoItems*> ItemsManager::getConvertArray(std::string strItems)
{
    Vector<InfoItems*> listItems;
    
    if ( strItems.length() == 0 )
    {
        return listItems;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strItems.c_str());
    if( jsonParser.HasParseError() )
    {
        return listItems;
    }
    
    if ( jsonParser.Size() == 0 )
    {
        return listItems;
    }
    
    //
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int idx = 0;
        
        if(jsonValue.HasMember("idx"))
            idx = jsonValue["idx"].GetInt();
        else if(jsonValue.HasMember("item_idx"))
            idx = jsonValue["item_idx"].GetInt();
        
        auto objReward = getInfoItems(idx);
        if ( objReward == nullptr )
            continue;
        
        int typeParams = objReward->getTypeParams();
        
        int tmpParams = 0;
        if(jsonValue.HasMember("type_params"))
            tmpParams = jsonValue["type_params"].GetInt();
        else if(jsonValue.HasMember("item_type_params"))
            tmpParams = jsonValue["item_type_params"].GetInt();
        
        if(tmpParams != 0)
            typeParams = tmpParams;

        if(jsonValue.HasMember("count"))
        {
            if ( jsonValue["count"].IsString() == true )
                objReward->setCount(jsonValue["count"].GetString());
            else
                objReward->setCount(jsonValue["count"].GetInt());
        }
        
        if(jsonValue.HasMember("item_count"))
        {
            if ( jsonValue["item_count"].IsString() == true )
                objReward->setCount(jsonValue["item_count"].GetString());
            else
                objReward->setCount(jsonValue["item_count"].GetInt());
        }
        
        objReward->setTypeParams(typeParams);
        
        //
        listItems.pushBack(objReward);
    }
    
    return listItems;
}

#pragma mark - set, get
InfoItems* ItemsManager::getInfoItems(int nIdx)
{
    InfoItems* objResult = nullptr;
    if ( nIdx == 0 )
    {
        return objResult;
    }
    
    for ( auto& obj : _listInfoItems )
    {
        if ( obj->getIdx() == nIdx && obj->isVersion() == true )
        {
            objResult = obj->clone();
            break;
        }
    }
    
    return objResult;
}

InfoItems* ItemsManager::getInfoItems(E_ITEMS eType)
{
    return getInfoItems((int)eType);
}

#pragma mark - item add
void ItemsManager::addItems(std::string strReward, bool bToast)
{
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strReward.c_str());
    if( strReward.length() == 0 || jsonParser.HasParseError() || jsonParser.Size() == 0 )
    {
        return;
    }
    
    cocos2d::Vector<InfoItems*> listReward;
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        int idx = 0;
        
        if(jsonValue.HasMember("idx"))
            idx = jsonValue["idx"].GetInt();
        else if(jsonValue.HasMember("item_idx"))
            idx = jsonValue["item_idx"].GetInt();
        
        auto objReward = getInfoItems(idx);
        if ( objReward == nullptr )
            continue;
        
        int typeParams = objReward->getTypeParams();
        
        int tmpParams = 0;
        if(jsonValue.HasMember("type_params"))
            tmpParams = jsonValue["type_params"].GetInt();
        else if(jsonValue.HasMember("item_type_params"))
            tmpParams = jsonValue["item_type_params"].GetInt();
        
        if(tmpParams != 0)
            typeParams = tmpParams;

        if(jsonValue.HasMember("count"))
        {
            if ( jsonValue["count"].IsString() == true )
                objReward->setCount(jsonValue["count"].GetString());
            else
                objReward->setCount(jsonValue["count"].GetInt());
        }
        
        if(jsonValue.HasMember("item_count"))
        {
            if ( jsonValue["item_count"].IsString() == true )
                objReward->setCount(jsonValue["item_count"].GetString());
            else
                objReward->setCount(jsonValue["item_count"].GetInt());
        }
        
        objReward->setTypeParams(typeParams);
        
        //
        listReward.pushBack(objReward);
    }
    
    //
    addItems(listReward, bToast);
}
void ItemsManager::addItems(cocos2d::Vector<InfoItems*> listReward, bool bToast)
{
    //
    if ( listReward.size() == 0 )
    {
        return;
    }
    
    cocos2d::Vector<InfoItems*> listRewardResult;
    for ( auto& objReward : listReward )
    {
        if ( objReward->getIdx() == 51 && objReward->getTypeParams() == 0 )
        {
            int nCount = std::atoi(objReward->getCount().c_str());
            for ( int i = 0; i < nCount; i++ )
            {
                int nTypeParams = random(1, C_COUNT_MONSTER);
                
                InfoItems* objTemp = nullptr;
                for ( auto& obj : listReward )
                {
                    if ( obj->getTypeParams() == nTypeParams )
                    {
                        objTemp = obj;
                        break;
                    }
                }
                
                if ( objTemp != nullptr )
                {
                    objTemp->setCount(std::atoi(objTemp->getCount().c_str()) + 1);
                }
                else
                {
                    auto objTemp = getInfoItems(E_ITEMS::MONSTER_SOUL);
                    objTemp->setCount(1);
                    objTemp->setTypeParams(nTypeParams);
                    listRewardResult.pushBack(objTemp);
                }
            }
        }
        else if ( objReward->getIdx() == 12000 && objReward->getTypeParams() == 0 )
        {
            objReward->setTypeParams(_nParameterArtifact);
            listRewardResult.pushBack(objReward);
        }
        else
        {
            listRewardResult.pushBack(objReward);
        }
    }
    
    //
    if ( listRewardResult.size() == 0 )
    {
        return;
    }
    
    //
    for ( auto& objReward : listRewardResult )
    {
        addItemsApply(objReward);
    }
    
    //
    if ( bToast == true )
    {
        PopupToastItems::show(listRewardResult);
    }
}

void ItemsManager::addItem(int nIdx, int nTypeParams, int nCount, bool bToast)
{
    addItem((E_ITEMS)nIdx, nTypeParams, nCount, bToast);
}
void ItemsManager::addItem(int nIdx, int nTypeParams, std::string strCount, bool bToast)
{
    addItem((E_ITEMS)nIdx, nTypeParams, strCount, bToast);
}
void ItemsManager::addItem(E_ITEMS eType, int nTypeParams, int nCount, bool bToast)
{
    addItem(eType, nTypeParams, MafUtils::toString(nCount), bToast);
}
void ItemsManager::addItem(E_ITEMS eType, int nTypeParams, std::string strCount, bool bToast)
{
    auto objItems = getInfoItems(eType);
    if ( objItems == nullptr || objItems->isVersion() == false )
    {
        return;
    }
    
    objItems->setTypeParams(nTypeParams);
    objItems->setCount(strCount);
    
    addItem(objItems, bToast);
}
void ItemsManager::addItem(InfoItems* objItem, bool bToast)
{
    if ( objItem->getIdx() == 51 && objItem->getTypeParams() == 0 )
    {// 몬스터 정수 예외처리
        objItem->setTypeParams(random(1, C_COUNT_MONSTER));
    }
    else if ( objItem->getIdx() == 12000 && objItem->getTypeParams() == 0 )
    {
        objItem->setTypeParams(_nParameterArtifact);
    }
    addItemsApply(objItem);
    
    //
    cocos2d::Vector<InfoItems*> listItem;
    listItem.pushBack(objItem);
    
    //
    if ( bToast == true )
    {
        PopupToastItems::show(listItem);
    }
}

void ItemsManager::addItemsApply(InfoItems* objReward)
{
    if ( objReward == nullptr || objReward->isVersion() == false )
    {
        return;
    }
        
    int nIdx = objReward->getIdx();
    std::string strType = objReward->getType();
    std::string strCount = objReward->getCount();
    std::string strTypeDetail = objReward->getTypeDetail();
    std::string strReward = objReward->getReward();
    if ( strReward.length() != 0 )
    {
        addItems(strReward);
        if (strType.compare("BADGE") == 0)
        {
            BadgeManager::getInstance()->setRewardBadge(objReward->getTypeParams());
        }
        return;
    }
    
    if ( strType.compare("ITEM") == 0 )
    {
        if ( nIdx == 1 )
        {// gold
            ItemsMoneyManager::getInstance()->setGold(strCount);
        }
        else if ( nIdx == 2 )
        {// key
            ItemsMoneyManager::getInstance()->setKey(strCount);
        }
        else if ( nIdx == 3 )
        {// gem
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setGem(nCount);
        }
        else if ( nIdx == 7 )
        {// point
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setPoint(nCount);
        }
        else if ( nIdx == 8 )
        {// cube
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setCube(nCount);
        }
        else if ( nIdx == 9 )
        {// key(devil)
            ItemsMoneyManager::getInstance()->setDevilKey(strCount);
        }
        else if ( nIdx == 10 )
        {// gold(devil)
            ItemsMoneyManager::getInstance()->setDevilGold(strCount);
        }
        else if ( nIdx == 13 )
        {// costume coupon
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setCostumeCoupon(ItemsMoneyManager::getInstance()->getCostumeCoupon() + nCount);
        }
        else if ( nIdx == 14 )
        {// stone bag
            int nCount = std::atoi(strCount.c_str());
            StoneManager::getInstance()->setStorageCount(StoneManager::getInstance()->getStorageCountNow() + nCount);
        }
        else if ( nIdx == 15 )
        {// key booster
            int nCount = std::atoi(strCount.c_str());
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS, nCount);
        }
        else if ( nIdx == 51 )
        {//몬스터 정수
            int nCount = std::atoi(strCount.c_str());
            int nTypeParams = objReward->getTypeParams();
            if ( nTypeParams == 0 )
            {
                nTypeParams = random(1, C_COUNT_MONSTER);
            }
            
            MonsterManager::getInstance()->addSoulCount(nTypeParams, nCount);
            MonsterManager::getInstance()->saveData();
            
            //
            RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, MonsterManager::getInstance()->isSoulLevelComplete());
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP);
        }
        else if ( nIdx == 52 )
        {//몬스터 정수 조각
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setMonsterSoulPiece(nCount);
        }
        else if ( nIdx == 81 )
        {//빙고 티켓
            int nCount = std::atoi(strCount.c_str());
            EventBingoManager::getInstance()->setBingoTicketCount(nCount);
        }
        else if ( nIdx == 86 )
        {//용옥
            int nCount = std::atoi(strCount.c_str());
            NewRaidManager::getInstance()->setDragonBall(NewRaidManager::getInstance()->getDragonBall() + nCount);
        }
        else if ( nIdx == (int)E_ITEMS::SPIRIT_DRAW_TICKET_1 )
        {//1뽑
            int nCount = std::atoi(strCount.c_str());
            WeaponSpiritManager::getInstance()->setSummonTicket_1(WeaponSpiritManager::getInstance()->getSummonTicket_1() + nCount);
        }
        else if ( nIdx == (int)E_ITEMS::SPIRIT_DRAW_TICKET_11 )
        {//11연뽑
            int nCount = std::atoi(strCount.c_str());
            WeaponSpiritManager::getInstance()->setSummonTicket_11(WeaponSpiritManager::getInstance()->getSummonTicket_11() + nCount);
        }
        else if ( nIdx == (int)E_ITEMS::SPIRIT_DRAW_TICKET_33 )
        {//33연뽑
            int nCount = std::atoi(strCount.c_str());
            WeaponSpiritManager::getInstance()->setSummonTicket_33(WeaponSpiritManager::getInstance()->getSummonTicket_33() + nCount);
        }
        else if ( nIdx == (int)E_ITEMS::EVENT_TRADING_MARKET_COIN )
        {
            if(objReward->getTypeParams() == 1)
            {
                int nCount = std::atoi(strCount.c_str());
                
                EventTradingMarketManager::getInstance()->setTodayCoin(EventTradingMarketManager::getInstance()->getTodayCoin() + nCount);
                
                EventTradingMarketManager::getInstance()->setTotalCoin(EventTradingMarketManager::getInstance()->getTotalCoin() + nCount);
            }
            
        }
        else if ( nIdx == (int)E_ITEMS::ADVENTURE_CORE )
        {
            auto objCore = AdventureRelicManager::getInstance()->getInfoCore(objReward->getTypeParams());
            if(objCore != nullptr)
            {
                objCore->setIsActice(true);
                objCore->setCount(std::atoi(strCount.c_str()));
            }
        }
        else if ( nIdx == (int)E_ITEMS::ADVENTURE_TICKET )
        {
            AdventureManager::getInstance()->setTicket(AdventureManager::getInstance()->getTicket() + std::atoi(strCount.c_str()));
        }
        else if ( nIdx == 112 )
        {//미스릴
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setMythril(nCount, true);
        }
        else if (nIdx == (int)E_ITEMS::DEFENSE_COIN)
        {
            int64_t nCount = std::atoll(strCount.c_str());
            DefenseManager::getInstance()->setDefenseCoin(nCount);
        }
    }
    else if ( strType.compare("BUFF") == 0 )
    {
        int nCount = std::atoi(strCount.c_str());
        
        if ( strTypeDetail.compare("ATK") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("GOLD") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("MOVE") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("ALL") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, nCount);
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, nCount);
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("ATK_9") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("ATK_10") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_10_BONUS, nCount);
        }
        else if ( strTypeDetail.compare("REVIVAL") == 0 )
        {
            time_t nTime = UserInfoManager::getInstance()->getBuffRevival();
            time_t nTimeNow = UtilsDate::getInstance()->getTime();
            if ( nTime > nTimeNow )
            {
                nTime = nTime + (nCount * 60 * 60);
            }
            else
            {
                nTime = nTimeNow + (nCount * 60 * 60);
            }
            
            UserInfoManager::getInstance()->setBuffRevival(nTime);
        }
        else if ( strTypeDetail.compare("DEVIL_ATK") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_DMG, nCount);
        }
        else if ( strTypeDetail.compare("DEVIL_ASPD") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_SPD, nCount);
        }
        else if ( strTypeDetail.compare("DEVIL_GOLD") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_QUEST_GOLD, nCount);
        }
        else if ( strTypeDetail.compare("DEVIL_MOVE") == 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD, nCount);
        }
        else if ( strTypeDetail.compare("FEVER")==0 )
        {
            UserInfoManager::getInstance()->setItemFeverTime(nCount);
        }
    }
    else if ( strType.compare("MINE") == 0 )
    {
        int nCount = std::atoi(strCount.c_str());
        int nTypeParams = objReward->getTypeParams();
        
        if((E_MINERAL)nTypeParams != E_MINERAL::MINERAL_NONE)
        {
            nCount = MineralManager::getInstance()->getCount((E_MINERAL)nTypeParams) + nCount;
            MineralManager::getInstance()->setCount((E_MINERAL)nTypeParams, nCount);
            MineralManager::getInstance()->saveData();
        }
        else
        {
            /*
             enum E_MINERAL
                 MINERAL_NONE = 0,      item_info idx
                 MINERAL_1 = 1,                 32
                 MINERAL_2,                     33
                 MINERAL_3,                     34
                 MINERAL_4,                     35
                 MINERAL_5,                     36
                 MINERAL_6,                     37
                 MINERAL_7_SAPPHIRE,            38
                 MINERAL_8_OPAL,                39
                 MINERAL_9_AQUA,                40
                 MINERAL_10_EMERALD,            41
                 MINERAL_11_AMETHYST            42
             */
            
            E_MINERAL nType = (E_MINERAL)(objReward->getIdx() - 31);
            nCount = MineralManager::getInstance()->getCount(nType) + nCount;
            MineralManager::getInstance()->setCount(nType, nCount);
            MineralManager::getInstance()->saveData();
        }
        
        RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE);
    }
    else if ( strType.compare("STONE") == 0 )
    {
        int nCount = std::atoi(strCount.c_str());
        
        if ( strTypeDetail.compare("ATK") == 0 )
        {
            nCount = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::ATTACK) + nCount;
            StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::ATTACK, nCount);
        }
        else if ( strTypeDetail.compare("GOLD") == 0 )
        {
            nCount = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::GOLD) + nCount;
            StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::GOLD, nCount);
        }
        else if ( strTypeDetail.compare("KEY") == 0 )
        {
            nCount = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::KEY) + nCount;
            StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::KEY, nCount);
        }
        else if ( strTypeDetail.compare("FORCE") == 0 )
        {
            nCount = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::SERVER, E_STONE::FORCE_ATTACK) + nCount;
            StoneManager::getInstance()->setCount(E_STONE_TYPE::SERVER, E_STONE::FORCE_ATTACK, nCount);
        }
    }
    else if ( strType.compare("COSTUME") == 0 )
    {
        if ( strTypeDetail.compare("SKIN") == 0 )
        {
            int nTypeParams = objReward->getTypeParams();
            CostumeSkinManager::getInstance()->addCostumeSkinBuy(nTypeParams);
        }
        else if ( strTypeDetail.compare("JEWEL") == 0 && objReward->getTypeParams() >= E_COSTUME_IDX::IDX_HONOR_1001 )
        {//훈장
            int nCount = std::atoi(strCount.c_str());
            
            E_HONOR eType = (E_HONOR)objReward->getTypeParams();
            int nLevel = UserInfoManager::getInstance()->getHonorLevel(eType);
            int nLevelMax = DataManager::GetHonorMaxLevel(eType);
            if ( nLevel < nLevelMax )
            {
                nLevel = nLevel + nCount;
                if ( nLevel > nLevelMax )
                {
                    nLevel = nLevelMax;
                }
                
                UserInfoManager::getInstance()->setHonorLevel(eType, nLevel);
            }
        }
        else
        {
            E_COSTUME_IDX eIdx = (E_COSTUME_IDX)objReward->getTypeParams();
            E_COSTUME eType = E_COSTUME::HEAD;
            if ( strTypeDetail.compare("HEAD") == 0 )           eType = E_COSTUME::HEAD;
            else if ( strTypeDetail.compare("BODY") == 0 )      eType = E_COSTUME::BODY;
            else if ( strTypeDetail.compare("CAPE") == 0 )      eType = E_COSTUME::CAPE;
            else if ( strTypeDetail.compare("HAND") == 0 )      eType = E_COSTUME::HAND;
            else if ( strTypeDetail.compare("FOOT") == 0 )      eType = E_COSTUME::FOOT;
            else if ( strTypeDetail.compare("JEWEL") == 0 )     eType = E_COSTUME::JEWEL;
            else if ( strTypeDetail.compare("SETITEM") == 0 )   eType = E_COSTUME::SETITEM;
            
            //
            if ( eType != E_COSTUME::SETITEM && eIdx == E_COSTUME_IDX::IDX_LEGEND )
            {// 투구, 갑옷 망토, 장갑, 신발, 장신구 레전드
                int nLevel = CostumeManager::getInstance()->getLegendLevel(eType);
                int nLevelMax = DataManager::GetCostumeLegendMaxLevel();
                
                if ( nLevel < nLevelMax )
                {
                    CostumeManager::getInstance()->setExist(eType, eIdx, nLevel + 1);
                }
            }
            else
            {
                CostumeManager::getInstance()->setExist(eType, eIdx, 1);
                if ( CostumeManager::getInstance()->getRent(eType) == eIdx )
                {
                    CostumeManager::getInstance()->setRent(eType, (E_COSTUME_IDX)0);
                    CostumeManager::getInstance()->setRentTime(eType, 0);
                    SaveManager::saveCostumeRentList();
                    SaveManager::saveCostumeRentTime();
                }
            }
            SaveManager::saveCostume();
            
            //
            int nCount = 0;
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD);
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::BODY);
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::CAPE);
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HAND);
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::FOOT);
            nCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::JEWEL);
            MafGooglePlay::IncrementImmediateAchievement(ACHIEVE_COSTUME_90, nCount, 90);
            
            //
            if ( CostumeManager::getInstance()->isExistAll() )
            {
                StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_REALENDING);
            }

            //
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_15))
            {
                DataManager::getInstance()->initArtifactSaveData(DG_NORMAL);
            }
            
            //
            if ( eIdx == E_COSTUME_IDX::IDX_10 && eType == E_COSTUME::HEAD )
            {
                auto tutorial = TutorialManager::getInstance();
                tutorial->drawTutorial();
            }
        }
    }
    else if ( strType.compare("ARTIFACT") == 0 )
    {
        int nCount = std::atoi(strCount.c_str());
        
        E_ARTIFACT eType = (E_ARTIFACT)objReward->getTypeParams();
        auto objArtifact = DataManager::GetArtifactInfo(eType);
        if ( objArtifact == nullptr )
        {
            return;
        }
        
        //
        if ( objArtifact->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_NONE )
        {
            int nLevel = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, eType);
            int nLevelMax = objArtifact->getLevelMax();
            
            if ( nLevel < nLevelMax )
            {
                nLevel = nLevel + nCount;
                if ( nLevel >= nLevelMax )
                {
                    nLevel = nLevelMax;
                }
                
                UserInfoManager::getInstance()->setArtifactLevel(E_PLACE::DG_NORMAL, eType, nLevel);
            }
        }
        else if ( objArtifact->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT )
        {
            UserInfoManager::getInstance()->setArtifactLevel(E_PLACE::DG_NORMAL, eType, 1);
        }
        else
        {
            E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
            
            nCount = UserInfoManager::getInstance()->getArtifactLevel(ePlace, eType) + nCount;
            UserInfoManager::getInstance()->setArtifactLevel(ePlace, eType, nCount);
        }
        
        auto layer = UtilsScene::getInstance()->getRunningLayer();
        
        if(layer != nullptr)
        {
            auto scene = dynamic_cast<HelloWorldScene *>(layer);
            
            if(scene != nullptr)
            {
                scene->reDrawArtifact(eType);
            }
        }
    }
    else if ( strType.compare("ARTIFACT_INFINITE") == 0)
    {
        int nCount = std::atoi(strCount.c_str());
        
        INFINITE_ARTIFACT_TYPE eType = (INFINITE_ARTIFACT_TYPE)objReward->getTypeParams();
        
        auto info = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(eType);
        
        if(info == nullptr)
            return;
        
        InfiniteArtifactManager::getInstance()->setInfiniteArtifactLevel(info, info->getCurLv() + nCount);
    }
    else if ( strType.compare("PET_SKIN") == 0 )
    {
        E_PET ePet = E_PET::PET_NONE;
        if ( nIdx == 14000 )            ePet = E_PET::CHICK_1;
        else if ( nIdx == 14001 )       ePet = E_PET::HAMSTER_2;
        else if ( nIdx == 14002 )       ePet = E_PET::DRAGON_3;
        else if ( nIdx == 14003 )       ePet = E_PET::PRINCESS_4;
        else if ( nIdx == 14004 )       ePet = E_PET::PURIRING_5;
        else if ( nIdx == 14005 )       ePet = E_PET::HONG_6;
        else if ( nIdx == 14006 )       ePet = E_PET::MOLE_7;
        else if ( nIdx == 14007 )       ePet = E_PET::HANDSOME_8;
        
        if ( ePet != E_PET::PET_NONE )
        {
            int eTypeParams = objReward->getTypeParams();
            PetNewManager::getInstance()->setPetSkinBuy(ePet, eTypeParams);
        }
    }
    else if ( strType.compare("PET_EQUIP") == 0 )
    {
        int nIdx = objReward->getTypeParams();
        int nCount = std::atoi(strCount.c_str());
        
        int nEquipAmount = PetNewManager::getInstance()->getPetEquipAmount(nIdx);
        PetNewManager::getInstance()->setPetEquipAmount(nIdx, nEquipAmount + nCount);
        
        if ( PetNewManager::getInstance()->getPetEquipLevel(nIdx) == 0 )
        {
            PetNewManager::getInstance()->setPetEquipLevel(nIdx, 1);
        }
    }
}

void ItemsManager::subItem(InfoItems* objItem)
{
    subItemsApply(objItem);
}

void ItemsManager::subItemsApply(InfoItems* objReward)
{
    if ( objReward == nullptr || objReward->isVersion() == false )
    {
        return;
    }
        
    int nIdx = objReward->getIdx();
    std::string strType = objReward->getType();
    std::string strCount = objReward->getCount();
    std::string strTypeDetail = objReward->getTypeDetail();
    
    if ( strType.compare("ITEM") == 0 )
    {
        if ( nIdx == 1 )
        {// gold
            strCount.insert(0, "-");
            ItemsMoneyManager::getInstance()->setGold(strCount);
        }
        else if ( nIdx == 2 )
        {// key
            strCount.insert(0, "-");
            ItemsMoneyManager::getInstance()->setKey(strCount);
        }
        else if ( nIdx == 3 )
        {// gem
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setGem(-nCount);
        }
        else if ( nIdx == 7 )
        {// point
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setPoint(-nCount);
        }
        else if ( nIdx == 8 )
        {// cube
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setCube(-nCount);
        }
        else if ( nIdx == 9 )
        {// key(devil)
            strCount.insert(0, "-");
            ItemsMoneyManager::getInstance()->setDevilKey(strCount);
        }
        else if ( nIdx == 10 )
        {// gold(devil)
            strCount.insert(0, "-");
            ItemsMoneyManager::getInstance()->setDevilGold(strCount);
        }
        else if ( nIdx == 13 )
        {// costume coupon
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setCostumeCoupon(ItemsMoneyManager::getInstance()->getCostumeCoupon() - nCount);
        }
        else if ( nIdx == 15 )
        {// key booster
            int nCount = std::atoi(strCount.c_str());
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS, -nCount);
        }
        else if ( nIdx == 112 )
        {// mythril
            int nCount = std::atoi(strCount.c_str());
            ItemsMoneyManager::getInstance()->setMythril(-nCount, true);
        }
        else if (nIdx == (int)E_ITEMS::DEFENSE_COIN)
        {
            int64_t nCount = std::atoll(strCount.c_str());
            DefenseManager::getInstance()->setDefenseCoin(nCount);
        }
    }
    else if ( strType.compare("MINE") == 0 )
    {
        int nCount = std::atoi(strCount.c_str());
        int nTypeParams = objReward->getTypeParams();
        
        if((E_MINERAL)nTypeParams != E_MINERAL::MINERAL_NONE)
        {
            nCount = MineralManager::getInstance()->getCount((E_MINERAL)nTypeParams) - nCount;
            MineralManager::getInstance()->setCount((E_MINERAL)nTypeParams, nCount);
            MineralManager::getInstance()->saveData();
        }
        else
        {
            /*
             enum E_MINERAL
                 MINERAL_NONE = 0,      item_info idx
                 MINERAL_1 = 1,                 32
                 MINERAL_2,                     33
                 MINERAL_3,                     34
                 MINERAL_4,                     35
                 MINERAL_5,                     36
                 MINERAL_6,                     37
                 MINERAL_7_SAPPHIRE,            38
                 MINERAL_8_OPAL,                39
                 MINERAL_9_AQUA,                40
                 MINERAL_10_EMERALD,            41
                 MINERAL_11_AMETHYST            42
             */
            
            nTypeParams = (E_MINERAL)(objReward->getIdx() - 31);
            
            nCount = MineralManager::getInstance()->getCount((E_MINERAL)nTypeParams) - nCount;
            MineralManager::getInstance()->setCount((E_MINERAL)nTypeParams, nCount);
            MineralManager::getInstance()->saveData();
        }
        
        RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE);
    }
}

#pragma mark - item set
void ItemsManager::setItemsServer(std::string strItemsJson)
{
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strItemsJson.c_str());
    if( strItemsJson.length() == 0 || jsonParser.HasParseError() || jsonParser.Size() == 0 )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int idx = 0;
        
        if(jsonValue.HasMember("idx"))
            idx = jsonValue["idx"].GetInt();
        else if(jsonValue.HasMember("item_idx"))
            idx = jsonValue["item_idx"].GetInt();
        
        auto objItems = getInfoItems(idx);
        if ( objItems == nullptr )
            continue;
        
        int typeParams = objItems->getTypeParams();
        
        int tmpParams = 0;
        if(jsonValue.HasMember("type_params"))
            tmpParams = jsonValue["type_params"].GetInt();
        else if(jsonValue.HasMember("item_type_params"))
            tmpParams = jsonValue["item_type_params"].GetInt();
        
        if(tmpParams != 0)
            typeParams = tmpParams;

        if(jsonValue.HasMember("count"))
        {
            if ( jsonValue["count"].IsString() == true )
                objItems->setCount(jsonValue["count"].GetString());
            else
                objItems->setCount(jsonValue["count"].GetInt());
        }
        
        if(jsonValue.HasMember("item_count"))
        {
            if ( jsonParser["item_count"].IsString() == true )
                objItems->setCount(jsonValue["item_count"].GetString());
            else
                objItems->setCount(jsonValue["item_count"].GetInt());
        }
        
        objItems->setTypeParams(typeParams);
        
        //
        setItemsServer(objItems);
    }
}

void ItemsManager::setItemsServer(E_ITEMS eType, int nTypeParams, int nCount)
{
    setItemsServer(eType, nTypeParams, MafUtils::toString(nCount));
}
void ItemsManager::setItemsServer(E_ITEMS eType, int nTypeParams, std::string strCount)
{
    auto objItems = getInfoItems(eType);
    if ( objItems == nullptr || objItems->isVersion() == false )
    {
        return;
    }
    
    objItems->setTypeParams(nTypeParams);
    objItems->setCount(strCount);
    
    setItemsServer(objItems);
}

void ItemsManager::setItemsServer(InfoItems* objItems)
{
    if ( objItems == nullptr || objItems->isVersion() == false )
    {
        return;
    }
    
    int nIdx = objItems->getIdx();
    std::string strType = objItems->getType();
    std::string strTypeDetail = objItems->getTypeDetail();
    int nTypeParams = objItems->getTypeParams();
    
    std::string strCount = objItems->getCount();
    
    if ( strType.compare("ITEM") == 0 )
    {
        if ( nIdx == 22 )
        {// 지하감옥 입장권
            int nCount = std::atoi(strCount.c_str());
            PrisonManager::getInstance()->setHeart(nCount);
        }
        else if ( nIdx == 23 )
        {
            int nCount = std::atoi(strCount.c_str());
            PrisonManager::getInstance()->setTicketCount(nCount);
        }
        else if ( nIdx == 50 )
        {// 펫 장비 박스
            int nCount = std::atoi(strCount.c_str());
            PetEquipBoxManager::getInstance()->setEquipBoxCount(nCount);
        }
        else if ( nIdx == 67 )
        {// 펫 장비 박스
            int nCount = std::atoi(strCount.c_str());
            PetEquipBoxManager::getInstance()->setEquipBoxSpecialCount(nCount);
        }
        else if ( nIdx == 84 )
        {//2021 할로윈 캔디
            int nCount = std::atoi(strCount.c_str());
            EventBoardGameManager::getInstance()->setTotalCandy(nCount);
        }
        else if ( nIdx == 85 )
        {//보드게임 주사위 포인트
            int nCount = std::atoi(strCount.c_str());
            EventBoardGameManager::getInstance()->setTotalPoint(nCount);
        }
        else if ( nIdx == 109 )
        {//투표 이벤트 투표권
            int nCount = std::atoi(strCount.c_str());
            EventVoteManager::getInstance()->setVoteCoinCount(nCount);
        }
        else if ( nIdx == 110 )
        {//
            int nCount = std::atoi(strCount.c_str());
            EventLuckyBagManager::getInstance()->setLuckyBagCount(nCount);
        }
        else if (nIdx == (int)E_ITEMS::NEW_RAID_DRAGON_BALL)
        {
            int nCount = std::atoi(strCount.c_str());
            NewRaidManager::getInstance()->setDragonBall(nCount);
        }
        else if (nIdx == (int)E_ITEMS::DEFENSE_DRAW_TICKET_1)
        {
            int nCount = std::atoi(strCount.c_str());
            DefenseManager::getInstance()->setDrawTicket_1(nCount);
        }
        else if (nIdx == (int)E_ITEMS::DEFENSE_DRAW_TICKET_11)
        {
            int nCount = std::atoi(strCount.c_str());
            DefenseManager::getInstance()->setDrawTicket_11(nCount);
        }
        else if (nIdx == (int)E_ITEMS::DEFENSE_DRAW_TICKET_33)
        {
            int nCount = std::atoi(strCount.c_str());
            DefenseManager::getInstance()->setDrawTicket_33(nCount);
        }
        else
        {
            _listItems[nIdx] = strCount;
        }
    }
    else if ( strType.compare("ADS") == 0 )
    {
        if ( nIdx == 48 || nIdx == 53 )
        {// 광고스킵 정액제
            int nCount = std::atoi(strCount.c_str());
            CashManager::getInstance()->setAdsfreeTime(nCount);
        }
    }
    else if ( strType.compare("COSTUME") == 0 )
    {
        if ( strTypeDetail.compare("SKIN") == 0 )
        {
            CostumeSkinManager::getInstance()->setCostumeSkinBuy(nTypeParams);
        }
    }
}

std::string ItemsManager::getItems(E_ITEMS eType)
{
    std::string count = "0";
    
    if ( eType == (E_ITEMS)1 )
    {// gold
        count = ItemsMoneyManager::getInstance()->getGold();
    }
    else if ( eType == (E_ITEMS)2 )
    {// key
        count = ItemsMoneyManager::getInstance()->getKey();
    }
    else if ( eType == (E_ITEMS)3 )
    {// gem
        count = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
    }
    else if ( eType == (E_ITEMS)7 )
    {// point
        count = MafUtils::toString(ItemsMoneyManager::getInstance()->getPoint());
    }
    else if ( eType == (E_ITEMS)8 )
    {// cube
        count = MafUtils::toString(ItemsMoneyManager::getInstance()->getCube());
    }
    else if ( eType == (E_ITEMS)9 )
    {// key(devil)
        count = ItemsMoneyManager::getInstance()->getDevilKey();
    }
    else if ( eType == (E_ITEMS)10 )
    {// gold(devil)
        count = ItemsMoneyManager::getInstance()->getDevilGold();
    }
    else if ( eType == (E_ITEMS)13 )
    {// costume coupon
        count = MafUtils::toString(ItemsMoneyManager::getInstance()->getCostumeCoupon());
    }
    else if ( eType == (E_ITEMS)15 )
    {// key booster
        count = MafUtils::toString(UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS));
    }
    else if ( eType == (E_ITEMS)22 )
    {// 지하감옥 입장권
        count = MafUtils::toString(PrisonManager::getInstance()->getHeart());
    }
    else if ( eType == (E_ITEMS)23 )
    {
        count = MafUtils::toString(PrisonManager::getInstance()->getTicketCount());
    }
    else if ( eType == (E_ITEMS)32 || eType == (E_ITEMS)33 || eType == (E_ITEMS)34 || eType == (E_ITEMS)35 || eType == (E_ITEMS)36 || eType == (E_ITEMS)37 || eType == (E_ITEMS)38 || eType == (E_ITEMS)39 || eType == (E_ITEMS)40 || eType == (E_ITEMS)41 || eType == (E_ITEMS)42 )
    {//광물
        auto objItem = getInfoItems(eType);
        if ( objItem != nullptr )
        {
            E_MINERAL eType = (E_MINERAL)objItem->getTypeParams();
            count = MafUtils::toString(MineralManager::getInstance()->getCount(eType));
        }
    }
    else if ( eType == (E_ITEMS)50 )
    {// 펫 장비 박스
        count = MafUtils::toString(PetEquipBoxManager::getInstance()->getEquipBoxCount());
    }
    else if ( eType == (E_ITEMS)84 )
    {//2021 할로윈 캔디
        count = MafUtils::toString(EventBoardGameManager::getInstance()->getTotalCandy());
    }
    else if ( eType == (E_ITEMS)85 )
    {//보드게임 주사위 포인트
        count = MafUtils::toString(EventBoardGameManager::getInstance()->getTotalPoint());
    }
    else if ( eType == (E_ITEMS)103 )
    {//레이스 이벤트 코인
        count = MafUtils::doubleToString(EventRaceManager::getInstance()->getCoin());
    }
    else if ( eType == (E_ITEMS)104 )
    {//레이스 이벤트 게임 티켓
        count = MafUtils::toString(EventRaceManager::getInstance()->getTicket());
    }
    else if ( eType == (E_ITEMS)110 )
    {//럭키 백
        count = MafUtils::toString(EventLuckyBagManager::getInstance()->getLuckyBagCount());
    }
    else if ( eType == (E_ITEMS)112 )
    {//mythril
        count = MafUtils::toString(ItemsMoneyManager::getInstance()->getMythril());
    }
    
    else
    {
        auto iter = _listItems.find(enumToInt(eType));
        if ( iter != _listItems.end() )
        {
            count = _listItems[enumToInt(eType)];
        }
    }
    
    return count;
}
 
