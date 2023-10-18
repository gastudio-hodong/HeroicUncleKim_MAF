//
//  ShopItemManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "ShopItemManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

ShopItemManager::ShopItemManager(void):
_timeUpdate(0)


{
    
}

ShopItemManager::~ShopItemManager(void)
{
    
}

bool ShopItemManager::init()
{
    
    return true;
}

#pragma mark - load

#pragma mark - update
void ShopItemManager::update(float dt)
{
    _timeUpdate += dt;
    if ( _timeUpdate >= 1)
    {
        _timeUpdate -= 1;

    }
}

#pragma mark - get, set
std::string ShopItemManager::getPathIcon(E_SHOP eType)
{
    std::string path = "";
    switch(eType)
    {
        case E_SHOP::SHOP_HERO_GEM :        path = "Assets/ui/shop/shop_new_1.png";     break;
        case E_SHOP::SHOP_FEVER :           path = "Assets/ui/shop/shop_fever_1.png";   break;
        case E_SHOP::SHOP_ATTACK_BONUS :    path = "Assets/ui/shop/shop_new_2.png";     break;
        case E_SHOP::SHOP_MOVE_BONUS :      path = "Assets/ui/shop/shop_new_3.png";     break;
        case E_SHOP::SHOP_COIN_BONUS :      path = "Assets/ui/shop/shop_new_4.png";     break;
        case E_SHOP::SHOP_GOLD_GEM :        path = "Assets/ui/shop/shop_new_5.png"; 	break;
        case E_SHOP::SHOP_GOLD_GEM_BIG :    path = "Assets/ui/shop/shop_new_6.png";     break;
            
        case E_SHOP::SHOP_D_MOVE_SPD :      path = "Assets/ui/shop/shop_new_7.png";     break;
        case E_SHOP::SHOP_D_ATK_SPD :       path = "Assets/ui/shop/shop_new_8.png";     break;
        case E_SHOP::SHOP_D_ATK_DMG :       path = "Assets/ui/shop/shop_new_9.png";     break;
        case E_SHOP::SHOP_D_QUEST_GOLD :    path = "Assets/ui/shop/shop_new_10.png";    break;
        case E_SHOP::SHOP_D_GOLD_GEM :      path = "Assets/ui/shop/shop_new_11.png";    break;
            
        default:
            break;
    }
    
    if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
        if ( count >= C_MAX_HERO_COUNT - 1 )
        {
            path = "Assets/ui/shop/shop_new_1_max.png";
        }
    }
    
    return path;
}

std::string ShopItemManager::getName(E_SHOP eType)
{
    std::string name = DataManager::GetShopName(eType);
    
    if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
        if ( count >= C_MAX_HERO_COUNT )
        {
            name += " MAX!!";
        }
        else if ( count == C_MAX_HERO_COUNT - 1 )
        {
            name += " MAX!!";
        }
        else
        {
            name += MafUtils::format(" LV.%d", count + 1);
        }
    }
    else if ( eType == E_SHOP::SHOP_ATTACK_BONUS )
    {
        if ( PassManager::getInstance()->isBuyPass(E_PASS_TYPE::PASS_DUNGEON) == true )
        {
            name = GAME_TEXT("t_item_d1_title_pass_1");
        }
    }
    
    return name;
}

std::string ShopItemManager::getDesc(E_SHOP eType)
{
    std::string desc = DataManager::GetShopExpain(eType);
    
    if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
        if ( count >= C_MAX_HERO_COUNT )
        {
            desc = MafUtils::format(desc.c_str(), count);
        }
        else if ( count == C_MAX_HERO_COUNT - 1 )
        {
            desc = MafUtils::format(desc.c_str(), count + 1);
        }
        else
        {
            desc = MafUtils::format(desc.c_str(), count + 1);
        }
    }
    else if ( eType == E_SHOP::SHOP_ATTACK_BONUS )
    {
        if ( PassManager::getInstance()->isBuyPass(E_PASS_TYPE::PASS_DUNGEON) == true )
        {
            desc = GAME_TEXT("t_item_d1_contents_pass_1");
        }
    }
    else if ( eType == E_SHOP::SHOP_FEVER )
    {
        int time = ParameterManager::getInstance()->getParameterInteger("item_fever_time");
        int count = UserInfoManager::getInstance()->getItemFeverCount();
        int countMax = ParameterManager::getInstance()->getParameterInteger("item_fever_max_count");
        desc = GAME_TEXTFORMAT("t_item_d1_contents_7", time, count, countMax);
    }
    else if ( eType == E_SHOP::SHOP_GOLD_GEM || eType == E_SHOP::SHOP_GOLD_GEM_BIG )
    {
        std::string earn = "0";
        
        if( eType == E_SHOP::SHOP_GOLD_GEM)
            earn = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::GEM_SHOP);
        else if( eType == E_SHOP::SHOP_GOLD_GEM_BIG)
            earn = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::GEM_BIG_SHOP);
        
        MafUtils::convertNumberToShort(earn);
        
        desc = StringUtils::format(desc.c_str(), earn.c_str());
    }
    
    return desc;
}

int ShopItemManager::getBuyPrice(E_SHOP eType)
{
    int price = 0;
    
    switch(eType)
    {
        case E_SHOP::SHOP_HERO_GEM :        price = DataManager::GetShopPrice(E_SHOP::SHOP_HERO_GEM);          break;
        case E_SHOP::SHOP_FEVER :           price = DataManager::GetShopPrice(E_SHOP::SHOP_FEVER);             break;
        case E_SHOP::SHOP_ATTACK_BONUS :    price = MafRemoteConfig::GetRemoteValueInt("BUY_ATTACK_BONUS"); break;
        case E_SHOP::SHOP_MOVE_BONUS :      price = MafRemoteConfig::GetRemoteValueInt("BUY_MOVE_BONUS");   break;
        case E_SHOP::SHOP_COIN_BONUS :      price = MafRemoteConfig::GetRemoteValueInt("BUY_COIN_BONUS");   break;
        case E_SHOP::SHOP_GOLD_GEM :        price = MafRemoteConfig::GetRemoteValueInt("BUY_GOLD_GEM");     break;
        case E_SHOP::SHOP_GOLD_GEM_BIG :    price = MafRemoteConfig::GetRemoteValueInt("BUY_GOLD_GEM_BIG"); break;
            
        case E_SHOP::SHOP_D_MOVE_SPD :      price = BUY_D_MOVE_SPD;         break;
        case E_SHOP::SHOP_D_ATK_SPD :       price = BUY_D_ATK_SPD;          break;
        case E_SHOP::SHOP_D_ATK_DMG :       price = BUY_D_ATK_DMG;          break;
        case E_SHOP::SHOP_D_QUEST_GOLD :    price = BUY_D_QUEST_GOLD;       break;
        case E_SHOP::SHOP_D_GOLD_GEM :      price = BUY_D_GOLD;             break;
            
        default:
            break;
    }
     
    return price;
}

#pragma mark - event
int ShopItemManager::onBuy(E_SHOP eType)
{
    int result = 0;
    /*
     0 : 성공
     1 : 가격 부족
     2 : 최대 구입
     */
    
    int currency = ItemsMoneyManager::getInstance()->getGem();
    int price = getBuyPrice(eType);
    
    if ( currency < price )
    {
        result = 1;
        return result;
    }
    
    E_BUFF_TYPE buffType = E_BUFF_TYPE::BUFF_ATTACK_BONUS;
    if(eType == E_SHOP::SHOP_ATTACK_BONUS)     buffType = E_BUFF_TYPE::BUFF_ATTACK_BONUS;
    else if(eType == E_SHOP::SHOP_COIN_BONUS)  buffType = E_BUFF_TYPE::BUFF_COIN_BONUS;
    else if(eType == E_SHOP::SHOP_MOVE_BONUS)  buffType = E_BUFF_TYPE::BUFF_MOVE_BONUS;
    else if(eType == E_SHOP::SHOP_HERO_GEM)    buffType = E_BUFF_TYPE::BUFF_HERO_BONUS;
    
    else if(eType == E_SHOP::SHOP_D_MOVE_SPD)  buffType = E_BUFF_TYPE::BUFF_D_MOVE_SPD;
    else if(eType == E_SHOP::SHOP_D_ATK_SPD)   buffType = E_BUFF_TYPE::BUFF_D_ATK_SPD;
    else if(eType == E_SHOP::SHOP_D_ATK_DMG)   buffType = E_BUFF_TYPE::BUFF_D_ATK_DMG;
    else if(eType == E_SHOP::SHOP_D_QUEST_GOLD) buffType = E_BUFF_TYPE::BUFF_D_QUEST_GOLD;
   
    int time = TIME_BUFF_BONUS;
    if ( eType == E_SHOP::SHOP_ATTACK_BONUS || eType == E_SHOP::SHOP_COIN_BONUS || eType == E_SHOP::SHOP_MOVE_BONUS )
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_21) )
            time +=600;
    }
    else if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        time = 1;
        
        int count = UserInfoManager::getInstance()->getBuffTime(buffType);
        if ( count >= C_MAX_HERO_COUNT )
        {
            result = 2;
            return result;
        }
    }
    else if ( eType == E_SHOP::SHOP_FEVER)
    {
        time = ParameterManager::getInstance()->getParameterInteger("item_fever_time");
        
        int count = UserInfoManager::getInstance()->getItemFeverCount();
        if ( count >= ParameterManager::getInstance()->getParameterInteger("item_fever_max_count") )
        {
            result = 2;
            return result;
        }
        else if ( UserInfoManager::getInstance()->getItemFeverTime() > 0 || UserInfoManager::getInstance()->getFeverTime() > 0 )
        {
            result = 2;
            return result;
        }
    }

    //
    if ( eType == E_SHOP::SHOP_FEVER )
    {
        UserInfoManager::getInstance()->setItemFeverTime(time);
        UserInfoManager::getInstance()->setItemFeverCount(UserInfoManager::getInstance()->getItemFeverCount() + 1);
    }
    else
    {
        UserInfoManager::getInstance()->setBuffTime(buffType, time);
    }
    ItemsMoneyManager::getInstance()->setGem(-price);
    
    //
    if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
    }
    else if ( eType == E_SHOP::SHOP_FEVER )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ITEM_FEVER);
    }
    else if ( eType == E_SHOP::SHOP_D_MOVE_SPD || eType == E_SHOP::SHOP_D_QUEST_GOLD )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::BUFF_BUY_DEVIL);
    }
    
    
    // history
    SaveManager::setHistoryData(E_HISTORY_DATA::K_BUFF_COUNT, SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_BUFF_COUNT) + 1);
    
    // toast
    if ( eType == E_SHOP::SHOP_ATTACK_BONUS )
    {
        PopupToastBuff::show(E_TOAST_BUFF::BUFF_ATK);
    }
    else if ( eType ==E_SHOP::SHOP_COIN_BONUS )
    {
        PopupToastBuff::show(E_TOAST_BUFF::BUFF_COIN);
    }
    else if ( eType ==E_SHOP::SHOP_MOVE_BONUS )
    {
        PopupToastBuff::show(E_TOAST_BUFF::BUFF_SPD);
    }
    
    // mission
    if ( eType == E_SHOP::SHOP_MOVE_BONUS)
    {
        SpecialMissionManager::getInstance()->clearSpecialMission(93);
    }
    
    // analytics
    if ( eType == E_SHOP::SHOP_HERO_GEM || eType == E_SHOP::SHOP_ATTACK_BONUS || eType == E_SHOP::SHOP_MOVE_BONUS || eType == E_SHOP::SHOP_COIN_BONUS )
    {
        int nItem = 0;
        if ( eType == E_SHOP::SHOP_HERO_GEM )              nItem = 0;
        else if ( eType == E_SHOP::SHOP_ATTACK_BONUS )     nItem = 1;
        else if ( eType == E_SHOP::SHOP_MOVE_BONUS )       nItem = 2;
        else if ( eType == E_SHOP::SHOP_COIN_BONUS )       nItem = 3;
        
        std::string strEventName = MafUtils::format(kAnalEventItemBuy, nItem);
        MafAnalyticsManager::LogEvent(strEventName.c_str(),kRepeatTrue);
    }
    
    return result;
}

int ShopItemManager::onBuyGold(E_SHOP eType)
{
    int result = 0;
    /*
     0 : 성공
     1 : 가격 부족
     */
    
    int currency = ItemsMoneyManager::getInstance()->getGem();
    int price = getBuyPrice(eType);
    
    std::string earnGold = "0";
    if ( eType == E_SHOP::SHOP_GOLD_GEM )
    {
        earnGold = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::GEM_SHOP);
    }
    else if ( eType == E_SHOP::SHOP_GOLD_GEM_BIG)
    {
        earnGold = UserInfoManager::getInstance()->getEarnGoldFinal(E_PLACE::DG_NORMAL, E_EARNGOLD::GEM_BIG_SHOP);
    }
    else if ( eType == E_SHOP::SHOP_D_GOLD_GEM )
    {
        for ( int i = 1; i <= C_COUNT_QUEST_DEVIL; i++ )
        {
            int level = UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_DEVIL_DOM, (E_QUEST)i);
            std::string earnTemp = UserInfoManager::getInstance()->getQuestEarnGold(E_PLACE::DG_DEVIL_DOM, (E_QUEST)i, level);
            
            earnGold = MafUtils::bigAddNum(earnGold, earnTemp);
        }
        earnGold.append("00");//퀘스트완료 x100회 보상을 준다.
    }
    
    if ( currency < price )
    {
        result = 1;
        return result;
    }
    
    //
    if ( eType == E_SHOP::SHOP_D_GOLD_GEM )
    {
        ItemsMoneyManager::getInstance()->setDevilGold(earnGold);
    }
    else
    {
        ItemsMoneyManager::getInstance()->setGold(earnGold);
    }
    ItemsMoneyManager::getInstance()->setGem(-price);
    
    // analytics
    if ( eType == E_SHOP::SHOP_GOLD_GEM || eType == E_SHOP::SHOP_GOLD_GEM_BIG )
    {
        int nItem = 0;
        
        if ( eType == E_SHOP::SHOP_GOLD_GEM )              nItem = 4;
        else if ( eType == E_SHOP::SHOP_GOLD_GEM_BIG )     nItem = 5;
        
        std::string strEventName = MafUtils::format(kAnalEventItemBuy, nItem);
        MafAnalyticsManager::LogEvent(strEventName.c_str(),kRepeatTrue);
    }
    
    return result;
}

#pragma mark - network
