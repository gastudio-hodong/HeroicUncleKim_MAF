//
//  UILayoutQuestCell.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutQuestCell.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/Equip/PopupWeaponCraft.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/QuestManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

UILayoutQuestCell* UILayoutQuestCell::create(E_QUEST eQuest /*= E_QUEST::QUEST_NONE*/)
{
    UILayoutQuestCell* pRet = new(std::nothrow) UILayoutQuestCell();
    if ( pRet && pRet->init(eQuest) )
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

UILayoutQuestCell::UILayoutQuestCell(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),

_eQuest(E_QUEST::QUEST_NONE),

//
_levelAdd(1),
_tutorialIdx(0),

//
_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    
}

UILayoutQuestCell::~UILayoutQuestCell(void)
{
    
}

bool UILayoutQuestCell::init(E_QUEST eQuest)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    _eQuest = eQuest;
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::TUTORIAL, this);
    
    RefreshManager::getInstance()->addUI(E_REFRESH::QUEST_INFO, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::QUEST_BUY, this);
    
    return true;
}

void UILayoutQuestCell::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
    }
    
    // draw
    drawList();
    
    //
    onListMove();
}

#pragma mark - refresh
void UILayoutQuestCell::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::TUTORIAL )
    {
        _tutorialIdx = 0;
        
        auto progress = TutorialManager::getInstance()->getTutorialProgress();
        if ( progress.first != PROGRESS_QUEST || progress.second == SUB_SHORTCUT_BUTTON || progress.second == SUB_REWARD_BUTTON )
        {
            return;
        }

        if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 0 )
        {
            _tutorialIdx = 1;
        }
        else if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 1 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 10 )
        {
            _tutorialIdx = 2;
        }
        else if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 2 )
        {
            _tutorialIdx = 3;
        }
        else if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 4 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 11 )
        {
            _tutorialIdx = 4;
        }
        else if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 5 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 8 )
        {
            _tutorialIdx = 5;
        }
        else if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 7 )
        {
            _tutorialIdx = 6;
        }
        
        //
        drawList();
        
        //
        if ( _tutorialIdx != 0 )
        {
            _uiList->jumpToItem(_tutorialIdx - 1, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        }
    }
    else if ( eType == E_REFRESH::WEAPON_BUY )
    {
        drawList();
    }
    else if ( eType == E_REFRESH::WEAPON_INFO )
    {
        drawList();
    }
}

#pragma mark - override DelegateListView
void UILayoutQuestCell::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size UILayoutQuestCell::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutQuestCell::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 130);

    // data
    E_QUEST eQuest = _listQuest.at(idx);
    
    int levelNow = UserInfoManager::getInstance()->getQuestLevel(_ePlace, eQuest);
    int levelMax = QuestManager::getInstance()->getLevelMax();
    int levelPlus = _levelAdd;
    if ( levelNow + levelPlus >= levelMax )
    {
        levelPlus = levelMax - levelNow;
    }
    bool bLevelMax = levelNow >= levelMax ? true : false;
    
    int levelEffect = DataManager::GetHonorEffect(E_HONOR::HONOR_2, UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_2));
    
    std::string pathIconBG = "Assets/ui_common/box_quest.png";
    if ( levelNow == 0 )
    {
        pathIconBG = "Assets/ui_common/box_quest_disable.png";
    }
    
    std::string pathIcon = QuestManager::getInstance()->getImagePath(eQuest);
    
    std::string nameText = DataManager::GetQuestName(_ePlace, eQuest);
    std::string levelText = MafUtils::format("LV.%d", levelNow);
    if ( levelNow > 0 && levelEffect > 0)
    {
        levelText = MafUtils::format("LV.%d [+%dLV]", levelNow, levelEffect);
    }
    
    std::string earnGold = UserInfoManager::getInstance()->getQuestSecondEarnGoldNomal(_ePlace, eQuest, levelNow);
    std::string earnGoldNext = UserInfoManager::getInstance()->getQuestSecondEarnGoldNomal(_ePlace, eQuest, levelNow + levelPlus);
    earnGoldNext = MafUtils::bigSubNum(earnGoldNext, earnGold);
    
    earnGold = MafUtils::convertNumberToShort(earnGold, true);
    earnGoldNext = MafUtils::convertNumberToShort(earnGoldNext);
    earnGoldNext.insert(0, "+");
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_1_s.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(10,60,10,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
     
    auto uiIconBG = ui::Button::create(pathIconBG);
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, sizeLayout.height / 2));
    uiIconBG->setTag((int)idx);
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIconBG->addChild(uiIcon);
        
        if ( levelNow == 0 )
        {
            uiIcon->setOpacity(200);
        }
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiName->setPosition(Vec2(posTextX, sizeLayout.height - 35));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiLevel = ui::CText::create(levelText, GAME_FONT_AL, 22);
    uiLevel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiLevel->setPosition(Vec2(posTextX + 3, sizeLayout.height - 30));
    uiLayout->addChild(uiLevel);
    
    auto uiBarBG = ui::ImageView::create("Assets/battle/gauge_bg.png");
    uiBarBG->setPosition(Vec2(posTextX, 40));
    uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiLayout->addChild(uiBarBG);
    {
        auto uiIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
        uiIcon->setPosition(Vec2(20, uiBarBG->getContentSize().height / 2));
        uiBarBG->addChild(uiIcon);
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            
            int levelNow = UserInfoManager::getInstance()->getQuestLevel(_ePlace, eQuest);
            
            std::string earn = UserInfoManager::getInstance()->getQuestSecondEarnGoldNomal(_ePlace, eQuest, levelNow);
            earn = MafUtils::convertNumberToShort(earn, true);
            
            ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_totalquest_gold", earn.c_str()));
        });
        
        auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_totalquest_gold", earnGold.c_str()), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiText->setPosition(Vec2(uiBarBG->getContentSize().width - 10, uiBarBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(280, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiText->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        uiBarBG->addChild(uiText);
    }
    
    
    //
    if ( bLevelMax == false )
    {
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
        auto itemNeedShort = itemNeed;
        itemNeedShort = MafUtils::convertNumberToShort(itemNeedShort);
        
        std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            
            auto uiLevelUp = (ui::Button*)sender;
            
            auto itemNow = ItemsMoneyManager::getInstance()->getGold();
            
            std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
            if ( MafUtils::compareStr(itemNow, itemNeed) == false )
            {
                pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
            }
            
            uiLevelUp->loadTextureNormal(pathBtn);
        });
        auto uiLevelUp = ui::Button::create(pathBtn);
        uiLevelUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiLevelUp->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiLevelUp->addClickEventListener(CC_CALLBACK_1(UILayoutQuestCell::onClickLevelUP, this));
        uiLevelUp->setTag((int)idx);
        uiLevelUp->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        uiLayout->addChild(uiLevelUp);
        
        auto uiItemIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
        uiItemIcon->setPosition(Vec2(uiLevelUp->getContentSize().width * 0.175, uiLevelUp->getContentSize().height * 0.3));
        uiLevelUp->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(itemNeedShort, GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiLevelUp->getContentSize().width * 0.35, uiLevelUp->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(95, uiItemCount->getContentSize().height));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiLevelUp->addChild(uiItemCount);
        
        auto uiEarnGoldNext = ui::CText::create(earnGoldNext, GAME_FONT_AL, 28);
        uiEarnGoldNext->setPosition(Vec2(uiLevelUp->getContentSize().width * 0.5, uiLevelUp->getContentSize().height * 0.75));
        uiLevelUp->addChild(uiEarnGoldNext);
        
        if ( _tutorialIdx != 0 && _tutorialIdx == eQuest )
        {
            auto progress = TutorialManager::getInstance()->getTutorialProgress();
            TutorialManager::getInstance()->attachEffect(EFFECT::EFFECT_FINGER, progress, uiLevelUp);
        }
    }
}

int64_t UILayoutQuestCell::listviewNumberOfCells(ui::CListView *listview)
{
    return _listQuest.size();
}

#pragma mark - init
void UILayoutQuestCell::initVar()
{
    if ( _eQuest != E_QUEST::QUEST_NONE )
    {
        _listQuest.push_back(_eQuest);
    }
    else
    {
        for ( int i = 1; i <= C_COUNT_QUEST; i++ )
        {
            _listQuest.push_back((E_QUEST)i);
        }
    }
}

void UILayoutQuestCell::initUi()
{
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

#pragma mark - ui
void UILayoutQuestCell::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutQuestCell::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void UILayoutQuestCell::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
    
}
 
#pragma mark - set, get, add
void UILayoutQuestCell::setLevelAdd(int levelAdd)
{
    _levelAdd = levelAdd;
    
    if ( _bInitUI == true )
    {
        drawList();
    }
}

#pragma mark - callback

#pragma mark - click
void UILayoutQuestCell::onClickLevelUP(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto eQuest = _listQuest.at(uiBtn->getTag());

    int result = QuestManager::getInstance()->onQuestBuy(eQuest, _levelAdd);
    if ( result != 0 )
    {
        if ( result == 2 )
        {
            int nTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL);
            if ( nTime <= 0 )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GOLD, PopupCurrencyDetail::E_TYPE::QUEST);
                popup->show();
            }
        }
        
        return;
    }
    
    //
    drawList();
}

#pragma mark - event
void UILayoutQuestCell::onListMove()
{
    if ( _uiList != nullptr )
    {
        int eQuest = E_QUEST::QUEST_1;
        for ( int i = 1; i <= C_COUNT_QUEST; i++ )
        {
            bool open = UserInfoManager::getInstance()->getQuest(E_PLACE::DG_NORMAL,(E_QUEST)i);
            if ( open == true )
            {
                eQuest = (E_QUEST)i;
            }
        }
            
        _uiList->jumpToItem(eQuest - 1, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    }
}
