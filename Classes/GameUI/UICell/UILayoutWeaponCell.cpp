//
//  UILayoutWeaponCell.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutWeaponCell.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/Equip/PopupWeaponCraft.h"
#include "GameUIPopup/Other/Equip/PopupWeaponReinforce.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

UILayoutWeaponCell* UILayoutWeaponCell::create(int weaponEquip /*= E_WEAPON::WEAPON_NONE*/)
{
    UILayoutWeaponCell* pRet = new(std::nothrow) UILayoutWeaponCell();
    if ( pRet && pRet->init(weaponEquip) )
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

UILayoutWeaponCell::UILayoutWeaponCell(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),

_weaponEquip(0),

//
_tutorialIdx(0),

//
_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    
}

UILayoutWeaponCell::~UILayoutWeaponCell(void)
{
    
}

bool UILayoutWeaponCell::init(int weaponEquip)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    _weaponEquip = weaponEquip;
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::TUTORIAL, this);
    
    RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_INFO, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_LEGEND, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_PERMANENT, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_REINFORCE, this);
    
    return true;
}

void UILayoutWeaponCell::onEnter()
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
void UILayoutWeaponCell::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::TUTORIAL )
    {
        _tutorialIdx = 0;
        
        auto progress = TutorialManager::getInstance()->getTutorialProgress();
        if ( progress.first != PROGRESS_WEAPON || progress.second == SUB_SHORTCUT_BUTTON || progress.second == SUB_REWARD_BUTTON )
        {
            return;
        }
        
        if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 3)
        {
            if ( progress.second == PROGRESS_SUB::SUB_OBJECT_1 )        _tutorialIdx = 1;
            else if( progress.second == PROGRESS_SUB::SUB_OBJECT_2 )    _tutorialIdx = 2;
            else if( progress.second == PROGRESS_SUB::SUB_OBJECT_3)     _tutorialIdx = 3;
        }
        else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 6)
        {
            if ( progress.second == PROGRESS_SUB::SUB_OBJECT_1 )        _tutorialIdx = 3;
            else if( progress.second == PROGRESS_SUB::SUB_OBJECT_2 )    _tutorialIdx = 4;
            else if( progress.second == PROGRESS_SUB::SUB_OBJECT_3)     _tutorialIdx = 5;
        }
        else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 82)
        {
            _tutorialIdx = 2;
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
        if ( listData.find("idx") == listData.end() )
        {
            int weaponEquip = WeaponManager::getInstance()->getEquip();
            _uiList->jumpToItem(weaponEquip - 1, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        }
    }
    else if ( eType == E_REFRESH::WEAPON_INFO || eType == E_REFRESH::WEAPON_BUY_LEGEND || eType == E_REFRESH::WEAPON_BUY_PERMANENT || eType == E_REFRESH::WEAPON_BUY_REINFORCE )
    {
        drawList();
    }
}

#pragma mark - override DelegateListView
void UILayoutWeaponCell::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutWeaponCell::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutWeaponCell::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 130);

    // data
    auto objWeapon = _listEquip.at(idx);
    
    int weaponIdx = objWeapon->getIdx();
    
    int weaponEquipIdx = WeaponManager::getInstance()->getEquip();
    int weaponEquipHighIdx = WeaponManager::getInstance()->getEquipHigh();
    int weaponEquipPermanentIdx = WeaponManager::getInstance()->getEquipPermanent() + 1;
    
    int levelNow = WeaponManager::getInstance()->getLevel(weaponIdx);
    int levelMax = WeaponManager::getInstance()->getLevelMax();
    int levelEffect = DataManager::GetHonorEffect(E_HONOR::HONOR_1, UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_1));
    bool bLevelMax = levelNow >= levelMax ? true : false;
    
    bool bDisable = false;
    if ( weaponIdx > weaponEquipIdx )
    {
        int levelPrev = WeaponManager::getInstance()->getLevel(weaponIdx-1);
        if ( levelPrev < levelMax )
        {
            bDisable = true;
        }
    }
    
    std::string pathIconBG = "Assets/ui_common/box_quest.png";
    if ( objWeapon->getLegend() > 1 )
    {
        pathIconBG = "Assets/ui/mine/week_weaponbox_2_1.png";
    }
    else if ( objWeapon->getLegend() > 0 )
    {
        pathIconBG = "Assets/ui/mine/week_weaponbox.png";
    }
    
    std::string pathIcon = WeaponManager::getInstance()->getImagePath(weaponIdx);
    
    std::string nameText = MafUtils::format("%d. %s", weaponIdx, objWeapon->getName().c_str());
    std::string levelText = MafUtils::format("LV.%d/%d", levelNow, levelMax);
    if ( levelNow > 0 && levelEffect > 0)
    {
        levelText = MafUtils::format("LV.%d/%d [+%dLV]", levelNow, levelMax, levelEffect);
    }
    
    std::string infoText = "ATK";
     
    
    std::string atk = WeaponManager::getInstance()->getAttack(weaponIdx, levelNow, UserInfoManager::getInstance()->getHonorLevel(HONOR_1));
    std::string atkNext = WeaponManager::getInstance()->getAttack(weaponIdx, levelNow+1, UserInfoManager::getInstance()->getHonorLevel(HONOR_1));
    atkNext = MafUtils::bigSubNum(atkNext, atk);
    
    atk = MafUtils::convertNumberToShort(atk);
    atkNext = MafUtils::convertNumberToShort(atkNext);
    if ( levelNow == 0 )
    {
        atkNext = "0";
    }
    
    int reinforceNow = WeaponManager::getInstance()->getReinforce(weaponIdx);
    int reinforceMax = objWeapon->getEnhanceMax();
    int reinforceEffectNow = WeaponManager::getInstance()->getReinforceEffect(weaponIdx, reinforceNow) * 100;
    int reinforceEffectDiff = WeaponManager::getInstance()->getReinforceEffect(weaponIdx, reinforceNow + 1) * 100;
    reinforceEffectDiff -= reinforceEffectNow;
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    std::string pathBG = "Assets/ui_common/list_bg_1_s.png";
    if ( weaponIdx == weaponEquipIdx )
    {
        pathBG = "Assets/ui_common/list_bg_2.png";
    }
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(10,60,10,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
     
    auto uiIconBG = ui::Button::create(pathIconBG);
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, sizeLayout.height / 2));
    uiIconBG->addClickEventListener(CC_CALLBACK_1(UILayoutWeaponCell::onClickWeapon, this));
    uiIconBG->setTag((int)idx);
    uiLayout->addChild(uiIconBG);
    {
        if ( objWeapon->getLegend() > 1 )
        {
            auto uiEffect = ui::ImageView::create("Assets/ui/mine/week_weaponbox_2_2.png");
            uiEffect->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
            uiEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
            uiIconBG->addChild(uiEffect);
        }
        
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale((uiIconBG->getContentSize().width - 20) / uiIcon->getContentSize().width);
        uiIconBG->addChild(uiIcon);
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiName->setPosition(Vec2(posTextX, sizeLayout.height - 35));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    if ( objWeapon->getLegend() > 1 )
    {
        uiName->setTextColor(Color4B(0, 245, 225, 255));
    }
    else if ( objWeapon->getLegend() > 0 )
    {
        uiName->setTextColor(Color4B(212, 34, 204, 255));
    }
    
    auto uiLevel = ui::CText::create(levelText, GAME_FONT_AL, 22);
    uiLevel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiLevel->setPosition(Vec2(posTextX + 3, sizeLayout.height - 30));
    uiLayout->addChild(uiLevel);
    
    auto uiInfo = ui::CText::create(infoText, GAME_FONT_AL, 38);
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiInfo->setPosition(Vec2(posTextX, 40));
    uiInfo->setTextAreaSize(Size(sizeLayout.width - 200 - posTextX, uiInfo->getContentSize().height));
    uiInfo->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setLineSpacing(-10);
    uiLayout->addChild(uiInfo);
    
    auto uiAtk = ui::CText::create(atk, GAME_FONT_AL, 38);
    uiAtk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiAtk->setPosition(Vec2(sizeLayout.width - 170, 40));
    uiAtk->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiAtk);
    
    auto uiReinforceBG = ui::ImageView::create("Assets/ui/bonsikdex/book_rating_icon_1_1.png");
    uiReinforceBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiReinforceBG->setPosition(Vec2(sizeLayout.width - 170, sizeLayout.height - 4));
    uiLayout->addChild(uiReinforceBG);
    {
        if ( reinforceNow >= reinforceMax )
        {
            uiReinforceBG->loadTexture("Assets/ui/bonsikdex/book_rating_icon_1_2.png");
        }
        
        auto uiReinforce = ui::CText::create(MafUtils::toString(reinforceNow), GAME_FONT, 20);
        uiReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiReinforce->setPosition(Vec2(uiReinforceBG->getContentSize().width - 10, uiReinforceBG->getContentSize().height / 2));
        uiReinforce->setTextColor(Color4B::YELLOW);
        uiReinforceBG->addChild(uiReinforce);
        
        auto uiEffect = ui::CText::create(MafUtils::format("%d%%", reinforceEffectNow), GAME_FONT_AL, 24);
        uiEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiEffect->setPosition(Vec2(uiReinforceBG->getContentSize().width, 0));
        uiEffect->setTextColor(Color4B::YELLOW);
        uiReinforceBG->addChild(uiEffect);
    }
    
    if ( bLevelMax == true )
    {
        if ( weaponIdx == weaponEquipPermanentIdx )
        {
            auto uiBuyPermanent = ui::Button::create("Assets/ui_common/btn_weaponkeep.png");
            uiBuyPermanent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiBuyPermanent->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
            uiBuyPermanent->addClickEventListener(CC_CALLBACK_1(UILayoutWeaponCell::onClickBuyPermanent, this));
            uiBuyPermanent->setTag((int)idx);
            uiLayout->addChild(uiBuyPermanent);
            
            auto uiText  = ui::CText::create(GAME_TEXT("t_ui_weapon_permanent_1").c_str(), GAME_FONT, 25);
            uiText->setAnchorPoint(Vec2(0.5,0.5));
            uiText->setPosition(Vec2(uiBuyPermanent->getContentSize().width / 2, uiBuyPermanent->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiBuyPermanent->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiBuyPermanent->addChild(uiText);
        }
        else if ( weaponIdx < weaponEquipPermanentIdx && reinforceNow < reinforceMax )
        {
            auto uiBuyReinforce = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
            uiBuyReinforce->setScale9Enabled(true);
            uiBuyReinforce->setCapInsets(Rect(33,33,2,2));
            uiBuyReinforce->setContentSize(Size(150, 116));
            uiBuyReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiBuyReinforce->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
            uiBuyReinforce->addClickEventListener(CC_CALLBACK_1(UILayoutWeaponCell::onClickBuyReinforce, this));
            uiBuyReinforce->setTag((int)idx);
            uiLayout->addChild(uiBuyReinforce);
            
            auto uiDiff = ui::CText::create(MafUtils::format("+%d%%", reinforceEffectDiff), GAME_FONT_AL, 28);
            uiDiff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiDiff->setPosition(Vec2(uiBuyReinforce->getContentSize().width * 0.5, uiBuyReinforce->getContentSize().height * 0.60));
            uiBuyReinforce->addChild(uiDiff);
            
            auto uiText  = ui::CText::create(GAME_TEXT("t_ui_bonsikdex_weapon_enhance_popup_1").c_str(), GAME_FONT, 25);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiText->setPosition(Vec2(uiBuyReinforce->getContentSize().width / 2, uiBuyReinforce->getContentSize().height * 0.55));
            uiText->setTextAreaSize(Size(uiBuyReinforce->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiBuyReinforce->addChild(uiText);
        }
    }
    else if ( objWeapon->getLegend() > 0 && weaponIdx > weaponEquipHighIdx && levelNow == 0 )
    {
        std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
        if ( bDisable == true )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
        
        auto uiBuyMine = ui::Button::create(pathBtn);
        uiBuyMine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBuyMine->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiBuyMine->addClickEventListener(CC_CALLBACK_1(UILayoutWeaponCell::onClickBuyLegend, this));
        uiBuyMine->setTag((int)idx);
        uiLayout->addChild(uiBuyMine);
        
        auto uiIcon = ui::ImageView::create("Assets/ui/mine/lock.png");
        uiIcon->setPosition(Vec2(uiBuyMine->getContentSize().width / 2, uiBuyMine->getContentSize().height / 2));
        uiBuyMine->addChild(uiIcon);
    }
    else
    {
        auto itemNeed = WeaponManager::getInstance()->getBuyPrice(weaponIdx, levelNow);
        auto itemNow= ItemsMoneyManager::getInstance()->getGold();

        std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
        
        if ( bDisable == true )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            
            auto itemNeed = WeaponManager::getInstance()->getBuyPrice(weaponIdx, levelNow);
            auto itemNow= ItemsMoneyManager::getInstance()->getGold();
            
            auto uiBuy = (ui::Button*)sender;
            std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
            if ( MafUtils::compareStr(itemNow, itemNeed) == false )
            {
                pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
            }
            
            uiBuy->loadTextureNormal(pathBtn);
        });
        auto uiBuy = ui::Button::create(pathBtn);
        uiBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBuy->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiBuy->addClickEventListener(CC_CALLBACK_1(UILayoutWeaponCell::onClickBuy, this));
        uiBuy->setTag((int)idx);
        uiLayout->addChild(uiBuy);
        if ( bDisable == false )
        {
            uiBuy->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        }
        
        
        //
        itemNeed = MafUtils::convertNumberToShort(itemNeed);
        
        auto uiItemIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
        uiItemIcon->setPosition(Vec2(uiBuy->getContentSize().width * 0.175, uiBuy->getContentSize().height * 0.3));
        uiBuy->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(itemNeed, GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiBuy->getContentSize().width * 0.35, uiBuy->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(95, uiItemCount->getContentSize().height));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiItemCount);
        
        //
        atkNext.insert(0, "+");
        
        auto uiAtk = ui::CText::create(atkNext, GAME_FONT_AL, 28);
        uiAtk->setPosition(Vec2(uiBuy->getContentSize().width * 0.5, uiBuy->getContentSize().height * 0.75));
        uiBuy->addChild(uiAtk);
        
        if ( _tutorialIdx != 0 && _tutorialIdx == weaponIdx )
        {
            auto progress = TutorialManager::getInstance()->getTutorialProgress();
            TutorialManager::getInstance()->attachEffect(EFFECT::EFFECT_FINGER, progress, uiBuy);
        }
    }
    
    if ( bDisable == true )
    {
        auto uiDisable = ui::Layout::create();
        uiDisable->setContentSize(sizeLayout);
        uiDisable->setTouchEnabled(true);
        uiDisable->setBackGroundColor(Color3B::BLACK);
        uiDisable->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiDisable->setOpacity(150);
        uiLayout->addChild(uiDisable);
    }
}

int64_t UILayoutWeaponCell::listviewNumberOfCells(ui::CListView *listview)
{
    return _listEquip.size();
}

#pragma mark - init
void UILayoutWeaponCell::initVar()
{
    if ( _weaponEquip != E_WEAPON::WEAPON_NONE )
    {
        auto obj = WeaponManager::getInstance()->getInfoWeapon(_weaponEquip);
        _listEquip.pushBack(obj);
    }
    else
    {
        _listEquip = WeaponManager::getInstance()->getListWeaponAll();
    }
}

void UILayoutWeaponCell::initUi()
{
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

#pragma mark - ui
void UILayoutWeaponCell::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutWeaponCell::uiList()
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
void UILayoutWeaponCell::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
    
}
 
#pragma mark - set, get, add

#pragma mark - callback
void UILayoutWeaponCell::callbackBuyLegend(int idx)
{
    int result = WeaponManager::getInstance()->onWeaponBuyLegend(idx);
    if ( result != 0 )
    {
        return;
    }
}

void UILayoutWeaponCell::callbackBuyPermanent(int idx)
{
    int result = WeaponManager::getInstance()->onWeaponBuyPermanent(idx);
    if ( result != 0 )
    {
        return;
    }
}

#pragma mark - click
void UILayoutWeaponCell::onClickWeapon(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = _listEquip.at(uiBtn->getTag());
    
    int result = WeaponManager::getInstance()->onWeaponChange(objWeapon->getIdx());
    if ( result != 0 )
    {
        return;
    }
}

void UILayoutWeaponCell::onClickBuyLegend(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = _listEquip.at(uiBtn->getTag());
    
    auto popup = PopupWeaponCraft::create(objWeapon->getIdx());
    popup->show();
}

void UILayoutWeaponCell::onClickBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = _listEquip.at(uiBtn->getTag());
    
    int result = WeaponManager::getInstance()->onWeaponBuy(objWeapon->getIdx());
    if ( result != 0 )
    {
        if ( result == 2 )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GOLD);
            popup->show();
        }
        return;
    }
}

void UILayoutWeaponCell::onClickBuyPermanent(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = _listEquip.at(uiBtn->getTag());
    
    auto popup = PopupWeaponPermanent::create(objWeapon->getIdx());
    popup->setCallbackResult(CC_CALLBACK_1(UILayoutWeaponCell::callbackBuyPermanent, this));
    popup->show();
}

void UILayoutWeaponCell::onClickBuyReinforce(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = _listEquip.at(uiBtn->getTag());
    if ( objWeapon == nullptr )
    {
        return;
    }
    
    int reinforceNow = WeaponManager::getInstance()->getReinforce(objWeapon->getIdx());
    int reinforceMax = objWeapon->getEnhanceMax();
    if ( reinforceNow >= reinforceMax )
    {
        return;
    }
        
    auto weaponPermanent = WeaponManager::getInstance()->getEquipPermanent();
    if ( weaponPermanent < objWeapon->getIdx() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_bonsikdex_weapon_error_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_180_L )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_bonsikdex_weapon_error_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    auto popup = PopupWeaponReinforce::create();
    popup->setWeaponIdx(objWeapon->getIdx());
    popup->show();
}

void UILayoutWeaponCell::onListMove()
{
    if ( _uiList != nullptr )
    {
        int weaponEquip = WeaponManager::getInstance()->getEquip();
        _uiList->jumpToItem(weaponEquip - 1, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    }
}
