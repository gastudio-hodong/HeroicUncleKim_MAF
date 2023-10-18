//
//  QuestManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "QuestManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/StoryManager.h"

USING_NS_CC;

QuestManager::QuestManager(void) :
_ePlace(E_PLACE::DG_NORMAL)

{
    
}

QuestManager::~QuestManager(void)
{
    
}

bool QuestManager::init()
{
    
    return true;
}

#pragma mark - update
void QuestManager::update(float dt)
{
     
}

#pragma mark - data
void QuestManager::saveData()
{

}

void QuestManager::loadData()
{
     
}

#pragma mark - load

#pragma mark - info

#pragma mark - get, set
std::string QuestManager::getImagePath(int idx)
{
    return MafUtils::format("Assets/icon_quest/normal/quest_%d.png", idx);
}

#pragma mark - get, set : level
int QuestManager::getLevel(int idx)
{
    int level = 0;
    
     
    return level;
}

int QuestManager::getLevelMax()
{
    return C_MAX_QUEST;
}

void QuestManager::setLevel(int level)
{
    
}
 

#pragma mark - event
int QuestManager::onReset(int floor)
{
    int result = 0;
    
    //
    if ( floor != -1 )
    {
        if ( UserInfoStackManager::getInstance()->getReviveSpecial() > 0 )
        {
            if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_30))
            {//음흑의 불꽃 : 강화, 초강화 환생시 퀘스트, 무기 초기화 되지 않음
                saveData();
                return result;
            }
        }
    }
    
    //
    for ( int i = 0; i <= C_COUNT_QUEST; i++ )
    {
        UserInfoManager::getInstance()->setQuestLevel(_ePlace, (E_QUEST)i, 0, false);
    }
    
    if ( floor >= 1001 )
    {
        for ( int i = 0; i <= E_QUEST::QUEST_25; i++ )
        {
            UserInfoManager::getInstance()->setQuestLevel(_ePlace, (E_QUEST)i, 1, false);
        }
    }
    
    if ( floor != -1 )
    {
        SaveManager::saveQuest(_ePlace);
    }
    
    return result;
}

int QuestManager::onQuestBuy(int idx, int levelPlus)
{
    int result = 0;
    
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    
    auto eQuest = (E_QUEST)idx;
    
    int levelNow = UserInfoManager::getInstance()->getQuestLevel(_ePlace, eQuest);
    int levelOld = levelNow;
    int levelMax = QuestManager::getInstance()->getLevelMax();
    bool bLevelMax = levelNow >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return 1;
    }
    
    
    if ( levelNow + levelPlus >= levelMax )
    {
        levelPlus = levelMax - levelNow;
    }
    
    double itemNeedTotal = 0;
    for ( int i = levelNow; i < levelNow + levelPlus; i++ )
    {
        itemNeedTotal = itemNeedTotal + DataManager::GetQuestUpgradeMoney(_ePlace, eQuest, i);
    }
    
    auto itemNow = ItemsMoneyManager::getInstance()->getGold();
    auto itemNeed = MafUtils::doubleToString(itemNeedTotal);
    auto itemNeedPer  = UserInfoManager::getInstance()->getBuySaleRate(_ePlace);
    if ( itemNeedPer < 100 )
    {
        itemNeed = MafUtils::bigMulNum(itemNeed, MafUtils::toString(itemNeedPer));
        if ( itemNeed.length() > 2 )
        {
            itemNeed.pop_back();
            itemNeed.pop_back();
        }
        else
        {
            itemNeed = "0";
        }
    }
    
    if ( MafUtils::compareStr(itemNow, itemNeed) == true )
    {
        levelNow = levelNow + levelPlus;
        
        std::string itemNeedTemp = std::string("-").append(itemNeed);
        ItemsMoneyManager::getInstance()->setGold(itemNeedTemp, false, false);
    }
    else
    {
        for ( int i = levelNow; i < levelNow + levelPlus; i++ )
        {
            double itemNeedTotal = DataManager::GetQuestUpgradeMoney(_ePlace, eQuest, i);
            
            auto itemNow = ItemsMoneyManager::getInstance()->getGold();
            auto itemNeed = MafUtils::doubleToString(itemNeedTotal);
            if ( itemNeedPer < 100 )
            {
                itemNeed = MafUtils::bigMulNum(itemNeed, MafUtils::toString(itemNeedPer));
                if ( itemNeed.length() > 2 )
                {
                    itemNeed.pop_back();
                    itemNeed.pop_back();
                }
                else
                {
                    itemNeed = "0";
                }
            }
            
            if ( MafUtils::compareStr(itemNow, itemNeed) == false )
            {
                break;
            }
            
            std::string itemNeedTemp = std::string("-").append(itemNeed);
            ItemsMoneyManager::getInstance()->setGold(itemNeedTemp, false, false);
            
            levelNow = i + 1;
        }
    }
    
    if ( levelOld == levelNow )
    {
        return 2;
    }
    
    //
    if ( levelOld > 1 && CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_10) )
    {
        if ( random(1, 100) <= 20 )
        {
            levelNow += 5;
        }
    }
    
    if ( levelNow >= levelMax )
    {
        levelNow = levelMax;
    }
    UserInfoManager::getInstance()->setQuestLevel(_ePlace, eQuest, levelNow);
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_QUEST, false);
    
    //
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_8) )
    {
        if ( eQuest == E_QUEST::QUEST_25)
        {
            UserInfoStackManager::getInstance()->setQuest25Buff(UserInfoStackManager::getInstance()->getQuest25Buff() + (levelOld - levelNow));
        }
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_BUY);
    
    //
    if ( idx == E_QUEST::QUEST_20 && levelOld == 0 && levelNow >= 1 )
    {
        bool bAllQuest = true;
        for ( int i = 1; i <= E_QUEST::QUEST_20; i++ )
        {
            if ( UserInfoManager::getInstance()->getQuestLevel(_ePlace,(E_QUEST)i) > 1 )
            {
                bAllQuest=false;
                break;
            }
        }
        
        if ( bAllQuest )
            MafGooglePlay::UnlockAchievement(ACHIEVE_1_ALLQUEST, 1, 1);
    }
    if ( idx == E_QUEST::QUEST_1 )
    {
        if ( levelNow >= 300 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_300_MOUSE, 1, 1);
        }
    }
    if ( idx == E_QUEST::QUEST_25 && levelNow >= 10 )
    {
        if ( UserInfoManager::getInstance()->getFloor(_ePlace) <= 100 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_10_QUEST25, 1, 1);
        }
    }
    if ( idx == E_QUEST::QUEST_25 && levelNow >= 500 )
    {
        MafGooglePlay::UnlockAchievement(ACHIEVE_500_QUEST25, 1, 1);
    }
    
    //
    if( idx == E_QUEST::QUEST_7 && levelNow > 0 )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_7);
    }
    else if( idx == E_QUEST::QUEST_11 && levelNow > 0 )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_11);
    }
    else if( idx == E_QUEST::QUEST_24 && levelNow > 0 )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_24);
    }
    
    //
    SaveManager::saveQuest(_ePlace);
    
    return result;
}
