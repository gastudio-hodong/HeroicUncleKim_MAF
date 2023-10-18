//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupCash.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/PurchaseCash/Layer/UILayoutCashGroup.h"
#include "GameUIPopup/Other/PurchaseCash/Layer/UILayoutCashHotDeal.h"
#include "GameUIPopup/Other/PurchaseCash/Layer/UILayoutCashNews.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBox.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"
#include "GameUIPopup/Other/CostumeSkin/PopupCostumeSkin.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"
#include "GameUIPopup/Other/PurchaseCash/PopupSummonShopSelect.h"

#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ServerMissionManager.h"

USING_NS_CC;

PopupCash* PopupCash::create()
{
    PopupCash* pRet = new(std::nothrow) PopupCash();
    if ( pRet && pRet->init() )
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

PopupCash::PopupCash(void) :
_bInitUI(false),

//
_eTab(E_TAB::NEWS),

_nTableProgress(0),
_nTableDownloadTotal(0),
_nTableDownloadSuccess(0),

//
_uiContainerContents(nullptr),
_uiContainerButtons(nullptr),

_uiContainerProgress(nullptr),

// widget content
_uiContentNew(nullptr),
_uiContentBonus(nullptr),
_uiContentSpecial(nullptr),
_uiContentMonthly(nullptr),
_uiContentNormal(nullptr),
_uiContentHotDeal(nullptr)
{
    
}

PopupCash::~PopupCash(void)
{
    CashManager::getInstance()->unSubDelegateInfo(this);
    CashManager::getInstance()->unSubDelegateDailyInfo(this);
    CashManager::getInstance()->unSubDelegatePurchaseRemainingAll(this);
    
    CC_SAFE_RELEASE_NULL(_uiContentNew);
    CC_SAFE_RELEASE_NULL(_uiContentBonus);
    CC_SAFE_RELEASE_NULL(_uiContentSpecial);
    CC_SAFE_RELEASE_NULL(_uiContentMonthly);
    CC_SAFE_RELEASE_NULL(_uiContentNormal);
    CC_SAFE_RELEASE_NULL(_uiContentHotDeal);
    
}

bool PopupCash::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupCash");
    
    //
    initVar();
    initUi();
    
    //
    CashManager::getInstance()->subDelegateInfo(new MafFunction<void(bool)>([=](bool bResult){
        if ( bResult == false )
        {
            hide();
            return;
        }
    }, this));
    
    CashManager::getInstance()->subDelegateDailyInfo(new MafFunction<void(bool)>([=](bool bResult){
        if ( bResult == false )
        {
            hide();
            return;
        }
    }, this));
    
    CashManager::getInstance()->subDelegatePurchaseRemainingAll(new MafFunction<void(bool)>([=](bool bResult){
        if ( bResult == false )
        {
            hide();
            return;
        }
    }, this));
    
    return true;
}

void PopupCash::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiContents();
        uiButtons();
        uiProgress();
        
        //
        PopupLoading::show();
        TableManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupCash::callbackTable, this), CC_CALLBACK_3(PopupCash::callbackTableProgress, this));
        
        //
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::VISIT_SHOP, 1);
        
        //
        _bInitUI = true;
    }
    
    //
    drawContents();
    drawButtons();
}

#pragma mark - init
void PopupCash::initVar()
{
    
}

void PopupCash::initUi()
{
    Size size = Size(750, 1330);
    if ( size.height > getContentSize().height )
    {
        size.height = getContentSize().height;
    }
    
    // size
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_shop_gem"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCash::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::KEY);
    list.push_back(E_ITEMS::POINT);
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::MATERIAL);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    uiCurrency->setName("UI_CURRENCY");
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui
void PopupCash::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    
    _uiContainerButtons = ui::Layout::create();
    _uiContainerButtons->setContentSize(Size(size.width, 212));
    _uiContainerButtons->setIgnoreAnchorPointForPosition(false);
    _uiContainerButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerButtons->setPosition(Vec2(size.width / 2, 0));
    _spriteContainer->addChild(_uiContainerButtons);
    
    _uiContainerContents = ui::Layout::create();
    _uiContainerContents->setContentSize(Size(size.width - 7 - 7, size.height - 75 - 50 - _uiContainerButtons->getContentSize().height));
    _uiContainerContents->setIgnoreAnchorPointForPosition(false);
    _uiContainerContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerContents->setPosition(Vec2(size.width / 2, _uiContainerButtons->getContentSize().height));
    _spriteContainer->addChild(_uiContainerContents);
    
    _uiContainerProgress = ui::Layout::create();
    _uiContainerProgress->setContentSize(Size(size.width, size.height));
    _uiContainerProgress->setIgnoreAnchorPointForPosition(false);
    _uiContainerProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerProgress->setPosition(Vec2(size.width / 2, 0));
    _uiContainerProgress->setTouchEnabled(true);
    _uiContainerProgress->setVisible(false);
    _spriteContainer->addChild(_uiContainerProgress, 100);
}

void PopupCash::uiContents()
{
    auto layout = _uiContainerContents;
    Size size = layout->getContentSize();
    
    // content
    auto uiContent = ui::Layout::create();
    uiContent->setContentSize(Size(size.width, size.height));
    uiContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiContent->setPosition(Vec2(size.width / 2, 0));
    uiContent->setName("UI_CONTENT");
    layout->addChild(uiContent);
}

void PopupCash::uiButtons()
{
    auto layout = _uiContainerButtons;
    Size size = layout->getContentSize();
    
    auto uiTab = ui::Layout::create();
    uiTab->setContentSize(Size(size.width, size.height));
    uiTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTab->setPosition(Vec2(size.width / 2, size.height));
    uiTab->setName("UI_TAB");
    layout->addChild(uiTab);
}

void PopupCash::uiProgress()
{
    auto layout = _uiContainerProgress;
    Size size = layout->getContentSize();
    
    auto uiText = ui::CText::create("Download(0/0)", GAME_FONT, 24);
    uiText->setPosition(Vec2(size.width / 2, size.height / 2));
    uiText->setName("UI_TEXT");
    layout->addChild(uiText);
}

#pragma mark - ui
void PopupCash::drawContents()
{
    auto layout = _uiContainerContents;
    
    auto uiContent = layout->getChildByName<ui::Layout*>("UI_CONTENT");
    if ( uiContent != nullptr )
    {
        ui::Layout* uiContentChild = nullptr;
        
        if ( _eTab == E_TAB::NEWS )
        {
            if ( _uiContentNew == nullptr )
            {
                _uiContentNew = UILayoutCashNews::create();
                _uiContentNew->retain();
            }
            uiContentChild = _uiContentNew;
        }
        else if ( _eTab == E_TAB::BONUS )
        {
            if ( _uiContentBonus == nullptr )
            {
                _uiContentBonus = UILayoutCashGroup::create({1, 100});
                _uiContentBonus->retain();
            }
            uiContentChild = _uiContentBonus;
        }
        else if ( _eTab == E_TAB::SPECIAL )
        {
            if ( _uiContentSpecial == nullptr )
            {
                _uiContentSpecial = UILayoutCashGroup::create({4});
                _uiContentSpecial->retain();
            }
            uiContentChild = _uiContentSpecial;
        }
        else if ( _eTab == E_TAB::MONTHLY )
        {
            if ( _uiContentMonthly == nullptr )
            {
                _uiContentMonthly = UILayoutCashGroup::create({5});
                _uiContentMonthly->retain();
            }
            uiContentChild = _uiContentMonthly;
        }
        else if ( _eTab == E_TAB::NORMAL )
        {
            if ( _uiContentNormal == nullptr )
            {
                _uiContentNormal = UILayoutCashGroup::create({6, 7});
                _uiContentNormal->retain();
            }
            uiContentChild = _uiContentNormal;
        }
        else if ( _eTab == E_TAB::HOTDEAL )
        {
            if ( _uiContentHotDeal == nullptr )
            {
                _uiContentHotDeal = UILayoutCashHotDeal::create();
                _uiContentHotDeal->retain();
            }
            uiContentChild = _uiContentHotDeal;
        }
        
        if ( uiContentChild != nullptr )
        {
            uiContentChild->setContentSize(Size(uiContent->getContentSize()));
            uiContentChild->setName("UI_GROUP");
            if ( auto uiCashBase = dynamic_cast<UILayoutCashBase*>(uiContentChild); uiCashBase != nullptr )
            {
                uiCashBase->onDraw();
            }
            
            uiContent->removeAllChildrenWithCleanup(false);
            uiContent->addChild(uiContentChild);
        }
    }
}

void PopupCash::drawButtons()
{
    auto layout = _uiContainerButtons;
    
    // button
    auto uiTab = layout->getChildByName<ui::Layout*>("UI_TAB");
    if ( uiTab != nullptr )
    {
        uiTab->removeAllChildren();
        
        //
        std::vector<E_TAB> listTab = {E_TAB::NEWS, E_TAB::BONUS, E_TAB::SPECIAL, E_TAB::MONTHLY, E_TAB::NORMAL, E_TAB::HOTDEAL, E_TAB::PET_EQUIP, E_TAB::SUMMON, E_TAB::COSTUME, E_TAB::RELIC};
        std::vector<std::string> listTabTextKey = {"t_ui_shop_news", "t_ui_shop_event", "t_ui_shop_special", "t_ui_shop_monthly", "t_ui_shop_normal", "t_ui_deals", "t_pet_equip", "t_ui_raid_bt_shop", "t_ui_costume_skin", "t_ui_advt_info_title_7"};
        std::vector<std::string> listTabIconPath = {
            "shop_category_0_2",
            "t_ui_shop_event",
            "t_ui_shop_special",
            "t_ui_shop_monthly",
            "t_ui_shop_normal",
            "t_ui_deals",
            "t_pet_equip", "t_ui_raid_bt_shop", "t_ui_costume_skin", "t_ui_advt_info_title_7"
            
        };
 
        size_t cellCount = listTab.size();
        double cellHeight = 106;
        double cellWidth = 150;
        
        for ( int i = 0; i < cellCount; i++ )
        {
            E_TAB eTab = listTab.at(i);
            
            std::string pathCell = "Assets/ui_common/btn_bottom_tap_off.png";
            std::string pathCellSelected = "Assets/ui_common/btn_bottom_tap_on.png";
            if ( eTab == _eTab )
            {
                pathCell = "Assets/ui_common/btn_bottom_tap_on.png";
            }
            
            std::string pathIcon = MafUtils::format("Assets/ui/purchase/shop_category_%d_2.png", i);
            if ( eTab == _eTab )
            {
                pathIcon = MafUtils::format("Assets/ui/purchase/shop_category_%d_1.png", i);
            }
            
            auto uiBtnCell = ui::Button::create(pathCell);
            uiBtnCell->setScale9Enabled(true);
            uiBtnCell->setContentSize(Size(cellWidth, cellHeight));
            uiBtnCell->setTag(enumToInt(eTab));
            uiBtnCell->addClickEventListener(CC_CALLBACK_1(PopupCash::onClickTab, this));
            uiTab->addChild(uiBtnCell);
            {
                auto uiIcon = ui::ImageView::create(pathIcon);
                uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiIcon->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, uiBtnCell->getContentSize().height - 5));
                uiBtnCell->addChild(uiIcon);
                
                auto uiText = ui::CText::create(GAME_TEXT(listTabTextKey.at(i)), GAME_FONT, 20);
                uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiText->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, 15));
                uiText->setTextColor(Color4B(100, 95, 95, 255));
                uiText->setTextAreaSize(Size(uiBtnCell->getContentSize().width - 10, uiText->getContentSize().height));
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiBtnCell->addChild(uiText);
                if ( eTab == _eTab )
                {
                    uiText->setTextColor(Color4B(255, 255, 255, 255));
                }
            }
        }
        
        //
        UtilsWidget::getInstance()->sortWidgetGrid(uiTab, 0);
    }
}

#pragma mark - set, get, other
void PopupCash::setTab(E_TAB tab)
{
    _eTab = tab;
    
    if ( _bInitUI == true )
    {
        //
        drawContents();
        drawButtons();
    }
}

#pragma mark - callback
void PopupCash::callbackTable(bool bResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _nTableDownloadTotal = 0;
    _nTableDownloadSuccess = 0;
    _nTableProgress = 0;
    _uiContainerProgress->setVisible(false);
    
    //
    CashManager::getInstance()->setLoad();
    
    //
    PopupLoading::show();
    CashManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupCash::callbackInfo, this));
}

void PopupCash::callbackTableProgress(int nTotal, int nSuccess, int nProgress)
{
    _nTableDownloadTotal = nTotal;
    _nTableDownloadSuccess = nSuccess;
    _nTableProgress = nProgress;

    //
    _uiContainerProgress->setVisible(true);
    
    //
    if ( auto uiText = _uiContainerProgress->getChildByName<ui::CText*>("UI_TEXT"); uiText != nullptr )
    {
        uiText->setString(MafUtils::format("Download(%d/%d) - %d%%", _nTableDownloadSuccess, _nTableDownloadTotal, _nTableProgress));
    }
}

void PopupCash::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        std::string strMsg = GAME_TEXT("t_ui_error_2");
        if ( AccountManager::getInstance()->getUserID().length() == 0 )
        {
            strMsg = GAME_TEXT("t_ui_error_1");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    // info daily network
    PopupLoading::show();
    CashManager::getInstance()->requestDailyInfo(CC_CALLBACK_1(PopupCash::callbackDailyInfo, this));
}

void PopupCash::callbackDailyInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        hide();
        return;
    }
    
    // info Remaining
    PopupLoading::show();
    CashManager::getInstance()->requestPurchaseRemainingAll(CC_CALLBACK_1(PopupCash::callbackRemainingInfo, this));
}

void PopupCash::callbackRemainingInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        hide();
        return;
    }
    
    //
    drawContents();
    drawButtons();
}

#pragma mark - click
void PopupCash::onClickClose(Ref* sender)
{
    hide();
}

void PopupCash::onClickTab(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_TAB eTab = enumFromInt<E_TAB>(uiBtn->getTag());
    
    if ( eTab == E_TAB::PET_EQUIP )
    {
        bool bEnter = false;
        if ( TutorialManager::getInstance()->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35) )
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_locked_menu", 35));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
            
        auto popup = PopupPetEquipBox::create();
        popup->show();
    }
    else if ( eTab == E_TAB::SUMMON )
    {
        auto popup = PopupSummonShopSelect::create();
        popup->show();
        return;
    }
    else if ( eTab == E_TAB::COSTUME )
    {
        bool bEnter = false;
        if ( TutorialManager::getInstance()->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38) )
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_locked_menu", 38));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupCostumeSkin::create();
        popup->setTabType(PopupCostumeSkin::E_TYPE::SHOP);
        popup->show();
    }
    else if ( eTab == E_TAB::RELIC )
    {
        bool bEnter = true;
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE)
        {
            bEnter = false;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_info_title_1"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupAdventureRelicDefault::create(PopupAdventureRelicDefault::E_TYPE::CRAFTING);
        popup->showButtonForceTutorial();
        popup->show();
    }
    else if ( eTab == _eTab )
    {
        return;
    }
    else
    {
        _eTab = eTab;
        
        //
        drawContents();
        drawButtons();
    }
}

#pragma mark - game
