//
//  PopupFrost.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFrost.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/BattleFrost/PopupFrostEnter.h"
#include "GameUIPopup/Other/BattleFrost/PopupFrostInfo.h"
#include "GameUIPopup/Other/BattleFrost/PopupFrostResult.h"
#include "GameUIPopup/Other/BattleFrost/Layer/LayerFrostBattle.h"
#include "GameUIPopup/Other/BattleFrost/Layer/LayerFrostList.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupFrost* PopupFrost::create()
{
    PopupFrost* pRet = new(std::nothrow) PopupFrost();
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

PopupFrost::PopupFrost() :
_bInitUI(false),

// widget
_uiContentsBattle(nullptr),
_uiContentsUIDefault(nullptr),
_uiContentsUIFloorInfo(nullptr),
_uiContentsUIList(nullptr)
{
    
}

PopupFrost::~PopupFrost(void)
{
    PrisonFrostManager::getInstance()->unSubDelegateInfo(this);
    PrisonFrostManager::getInstance()->unSubDelegateSkillLevelUp(this);
    PrisonFrostManager::getInstance()->unSubDelegateSkillReset(this);
    PrisonFrostManager::getInstance()->unSubDelegateResult(this);
}

bool PopupFrost::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFrost");
    
    //
    initVar();
    initUI();
    
    //
    PrisonFrostManager::getInstance()->subDelegateInfo(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupFrost::callbackInfo, this), this));
    PrisonFrostManager::getInstance()->subDelegateSkillLevelUp(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupFrost::callbackSkillLevelUp, this), this));
    PrisonFrostManager::getInstance()->subDelegateSkillReset(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupFrost::callbackSkillReset, this), this));
    PrisonFrostManager::getInstance()->subDelegateResult(new MafFunction<void(bool, int, std::string)>(CC_CALLBACK_3(PopupFrost::callbackBattleResult, this), this));
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PRISON_FROST_INFO, this);
    
    return true;
}

void PopupFrost::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiDefault();
        uiFloorInfo();
    }
    
    //
    drawDefault();
    drawFloorInfo();
    drawBattle();
    drawList();
    
    //
    PrisonFrostManager::getInstance()->requestInfo(true);
    
    //
    if ( DialogManager::getInstance()->isViewDialog("STORY_FROST_PRSION_EP") == false )
    {
        DialogManager::getInstance()->showDialog("STORY_FROST_PRSION_EP");
    }
}

#pragma mark - override
void PopupFrost::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PRISON_FROST_INFO )
    {
        drawDefault();
        drawFloorInfo();
        drawBattle();
        drawList();
    }
}

#pragma mark - init
void PopupFrost::initVar()
{
    
}

void PopupFrost::initUI()
{
    // size
    Size size = Size(750, 1300);

    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_frost_prison_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFrost::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::MATERIAL);
    list.push_back(E_ITEMS::FROST_PRISON_POINT);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui
void PopupFrost::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 14;
    size.height = size.height - 75 - 50;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsBattle = LayerFrostBattle::create(Size(size.width, 600));
    _uiContentsBattle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsBattle->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsBattle);
    
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, 600) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsUIDefault);
    
    _uiContentsUIFloorInfo = ui::Layout::create();
    _uiContentsUIFloorInfo->setContentSize( Size(size.width, 110) );
    _uiContentsUIFloorInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIFloorInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIFloorInfo->setPosition( Vec2(size.width / 2, size.height - _uiContentsUIDefault->getContentSize().height) );
    layout->addChild(_uiContentsUIFloorInfo);
    
    _uiContentsUIList = LayerFrostList::create(Size(size.width - 5, size.height - _uiContentsUIDefault->getContentSize().height - _uiContentsUIFloorInfo->getContentSize().height));
    _uiContentsUIList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsUIList->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsUIList);
}

void PopupFrost::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    // 설명 버튼
    auto uiBtnHelp = ui::Button::create("Assets/ui/race_event/race_btn_gameinfo.png");
    uiBtnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiBtnHelp->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnHelp->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, 10, size.height - 5));
    uiBtnHelp->addClickEventListener(CC_CALLBACK_1(PopupFrost::onClickHelp, this));
    layout->addChild(uiBtnHelp);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_info"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiText->setPosition(Vec2(uiBtnHelp->getContentSize().width - 45, uiBtnHelp->getContentSize().height / 2 + 2));
        uiText->setTextAreaSize(Size(90, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setColor(Color3B(150,120,55));
        uiBtnHelp->addChild(uiText);
    }
 
    // 탑 버튼
    auto uiBtnTowerInfo = ui::Button::create("Assets/ui/frost_prison/frost_tower_icon.png");
    uiBtnTowerInfo->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnTowerInfo->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width - 45, size.height - 50));
    uiBtnTowerInfo->addClickEventListener(CC_CALLBACK_1(PopupFrost::onClickTowerInfo, this));
    layout->addChild(uiBtnTowerInfo);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_reward_info"), GAME_FONT, 18);
        uiText->setPosition(Vec2(uiBtnTowerInfo->getContentSize().width / 2, 0));
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnTowerInfo->addChild(uiText);
    }
    
    // 입장하기
    auto uiBtnEnter = ui::Button::create("Assets/ui/frost_prison/btn_frost_tower_start.png");
    uiBtnEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnEnter->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnEnter->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width / 2, 35));
    uiBtnEnter->addClickEventListener(CC_CALLBACK_1(PopupFrost::onClickBattle, this));
    uiBtnEnter->setName("UI_ENTER");
    layout->addChild(uiBtnEnter);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_enter"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(105, uiBtnEnter->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(120, 60));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextColor(Color4B(15, 50, 55, 255));
        uiText->enableBold();
        uiBtnEnter->addChild(uiText);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::MATERIAL);
        if ( objItem != nullptr )
        {
            objItem->setCount("11");

            auto uiCost = UtilsWidget::getInstance()->createIconText(objItem->getPath(), objItem->getCount(), 75, 24, 5);
            uiCost->setPosition(Vec2(50, 60));
            uiCost->setName("UI_COST");
            uiBtnEnter->addChild(uiCost);
        }
    }
    
    // 광고 버튼
    auto uiBtnAds = ui::Button::create("Assets/ui_common/btn_ad_1.png");
    uiBtnAds->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnAds->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnAds->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width - 10, 35));
    uiBtnAds->addClickEventListener(CC_CALLBACK_1(PopupFrost::onClickAds, this));
    uiBtnAds->setName("UI_ADS");
    layout->addChild(uiBtnAds);
    {
        int bonus = PrisonFrostManager::getInstance()->getInfoSetting("ad_att_bonus")->getValueToInt();
        std::string bonusString = MafUtils::toString(bonus);
        bonusString.insert(0, "x");
        
        auto uiBonus = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_power.png", bonusString, 130, 24, 0);
        uiBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBonus->setPosition(Vec2(uiBtnAds->getContentSize().width / 2, uiBtnAds->getContentSize().height / 2 - 10));
        uiBtnAds->addChild(uiBonus);
    }
    
    auto uiAdsInfo = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
    uiAdsInfo->setScale9Enabled(true);
    uiAdsInfo->setContentSize(Size(168, 36));
    uiAdsInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiAdsInfo->setPosition(Vec2(uiBtnAds->getPositionX() - uiBtnAds->getContentSize().width / 2, uiBtnAds->getPositionY() + uiBtnAds->getContentSize().height + 5));
    uiAdsInfo->setName("UI_ADS_INFO");
    layout->addChild(uiAdsInfo);
    {
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_power.png");
        uiIcon->setPosition(Vec2(30, uiAdsInfo->getContentSize().height / 2));
        uiAdsInfo->addChild(uiIcon);
        
        auto uiEffect = ui::CText::create("x1.0A", GAME_FONT, 24);
        uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiEffect->setPosition(Vec2(uiAdsInfo->getContentSize().width - 10, uiAdsInfo->getContentSize().height / 2));
        uiEffect->setName("UI_EFFECT");
        uiAdsInfo->addChild(uiEffect);
    }
}

void PopupFrost::uiFloorInfo()
{
    auto layout = _uiContentsUIFloorInfo;
    Size size = layout->getContentSize();
    
 
    auto uiBG = ui::ImageView::create("Assets/ui/prison/btn_prison_2_off.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiFloorBG = ui::ImageView::create("Assets/ui_common/base_box_1_15.png");
    uiFloorBG->setScale9Enabled(true);
    uiFloorBG->setContentSize(Size(100, 80));
    uiFloorBG->setPosition(Vec2(70, size.height / 2));
    layout->addChild(uiFloorBG);
    {
        auto uiFloor = ui::CText::create("99F", GAME_FONT_AL, 36);
        uiFloor->setTextColor(Color4B(45, 215, 255, 255));
        uiFloor->setPosition(Vec2(uiFloorBG->getContentSize().width / 2, uiFloorBG->getContentSize().height / 2));
        uiFloor->setName("UI_FLOOR");
        uiFloorBG->addChild(uiFloor);
    }
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_clear"), GAME_FONT, 28);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiText->setPosition(Vec2(145, size.height / 2));
    uiText->setName("UI_CLEAR");
    layout->addChild(uiText);
    
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(240, 70));
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiLayoutReward->setPosition(Vec2(size.width - 20, size.height / 2));
    uiLayoutReward->setName("UI_REWARD");
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(uiLayoutReward);
    
}
          
#pragma mark - draw
void PopupFrost::drawDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    // 출력 유무 
    auto layoutBattle = (LayerFrostBattle*)_uiContentsBattle;
    if ( layoutBattle != nullptr )
    {
        if ( layoutBattle->getState() == LayerFrostBattle::E_STATE::None )
        {
            layout->setVisible(true);
        }
        else
        {
            layout->setVisible(false);
        }
    }
    
    
    // 입장하기
    auto uiBtnEnter = layout->getChildByName<ui::Button*>("UI_ENTER");
    if ( uiBtnEnter != nullptr )
    {
        uiBtnEnter->removeChildByName("UI_COST");
        
        int clearFloor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
        auto objInfoFloor = PrisonFrostManager::getInstance()->getInfoFloor(clearFloor);
        if ( objInfoFloor != nullptr )
        {
            auto objItem = objInfoFloor->getCost();
            if ( objItem != nullptr )
            {
                auto uiCost = UtilsWidget::getInstance()->createIconText(objItem->getPath(), objItem->getCount(), 75, 24, 5);
                uiCost->setPosition(Vec2(50, 60));
                uiCost->setName("UI_COST");
                uiBtnEnter->addChild(uiCost);
            }
        }
        else
        {
            uiBtnEnter->setVisible(false);
        }
    }
    
    // 광고
    auto uiBtnAds = layout->getChildByName<ui::Button*>("UI_ADS");
    if ( uiBtnAds != nullptr )
    {
        uiBtnAds->removeChildByName("UI_COUNT");
        
        int count = PrisonFrostManager::getInstance()->getAdsCount();
        int countMax = PrisonFrostManager::getInstance()->getAdsCountMax();
        
        std::string countString = MafUtils::format("%d/%d", countMax - count, countMax);
        
        if ( count >= countMax )
        {
            uiBtnAds->loadTextureNormal("Assets/ui_common/btn_ad_gray.png");
        }
        else
        {
            uiBtnAds->loadTextureNormal("Assets/ui_common/btn_ad_1.png");
        }
        
        auto uiCount = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_ad_1_1.png", countString, 130, 24, 0);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiCount->setPosition(Vec2(uiBtnAds->getContentSize().width / 2, uiBtnAds->getContentSize().height / 2 + 10));
        uiCount->setName("UI_COUNT");
        uiBtnAds->addChild(uiCount);
    }
    
    
    // 광고 정보
    auto uiAdsInfo = layout->getChildByName<ui::ImageView*>("UI_ADS_INFO");
    if ( uiAdsInfo != nullptr )
    {
        int count = PrisonFrostManager::getInstance()->getAdsCount();
        if ( count <= 0 )
        {
            uiAdsInfo->setVisible(false);
        }
        else
        {
            uiAdsInfo->setVisible(true);
            
            double effect = PrisonFrostManager::getInstance()->getAdsEffect();
            std::string effectString = MafUtils::doubleToString(effect);
            effectString = MafUtils::convertNumberToShort(effectString);
            effectString.insert(0, "x");
            
            auto uiEffect = uiAdsInfo->getChildByName<ui::CText*>("UI_EFFECT");
            uiEffect->setString(effectString);
        }
    }
}

void PopupFrost::drawFloorInfo()
{
    auto layout = _uiContentsUIFloorInfo;
    Size size = layout->getContentSize();
    
    int clearFloor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
    int clearFloorMax = PrisonFrostManager::getInstance()->getClearFloorMax();
    
    if ( clearFloor > clearFloorMax )
    {
        auto uiFloor = (ui::CText*)utils::findChild(layout, "UI_FLOOR");
        if ( uiFloor != nullptr )
        {
            uiFloor->setString(UtilsString::getInstance()->format("%dF", clearFloorMax));
        }
        
        auto uiText = (ui::CText*)utils::findChild(layout, "UI_CLEAR");
        if ( uiText != nullptr )
        {
            uiText->setString(GAME_TEXT("t_ui_frost_prison_max_floor"));
        }
        
        auto uiLayoutReward = (ui::Layout*)utils::findChild(layout, "UI_REWARD");
        if ( uiLayoutReward != nullptr )
        {
            uiLayoutReward->removeAllChildren();
        }
    }
    else
    {
        auto objInfoFloor = PrisonFrostManager::getInstance()->getInfoFloor(clearFloor);
        if ( objInfoFloor == nullptr )
        {
            return;
        }
        
        auto uiFloor = (ui::CText*)utils::findChild(layout, "UI_FLOOR");
        if ( uiFloor != nullptr )
        {
            uiFloor->setString(UtilsString::getInstance()->format("%dF", clearFloor));
        }
        
        auto uiText = (ui::CText*)utils::findChild(layout, "UI_CLEAR");
        if ( uiText != nullptr )
        {
            uiText->setString(GAME_TEXT("t_ui_frost_prison_clear"));
        }
        
        auto uiLayoutReward = (ui::Layout*)utils::findChild(layout, "UI_REWARD");
        if ( uiLayoutReward != nullptr )
        {
            uiLayoutReward->removeAllChildren();
            
            auto listReward = ItemsManager::getInstance()->getConvertArray(objInfoFloor->getReward());
            
            for ( int i = 0; i < listReward.size(); i++ )
            {
                auto objItems = listReward.at(i);
                
                auto uiRewardBG = ui::ImageView::create("Assets/ui_common/box_item.png");
                uiRewardBG->setScale9Enabled(true);
                uiRewardBG->setContentSize(Size(64, 64));
                uiLayoutReward->addChild(uiRewardBG);
                {
                    auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                    uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 5));
                    uiRewardBG->addChild(uiImgItemIcon);
                    
                    std::string rewardCount = objItems->getCount();
                    std::string rewardCountBonus = objItems->getCount();
                     
                    if ( objItems->isString() == true )
                    {
                        MafUtils::convertNumberToShort(rewardCount);
                        MafUtils::convertNumberToShort(rewardCountBonus);
                    }
                    
                    auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                    uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 10));
                    uiTextReward->enableOutline(Color4B::BLACK, 2);
                    uiRewardBG->addChild(uiTextReward);
                }
            }
            
            auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
            uiLayoutReward->setContentSize(sizeReward);
            uiLayoutReward->requestDoLayout();
        }
    }
    
    
}

void PopupFrost::drawBattle()
{
    auto layout = (LayerFrostBattle*)_uiContentsBattle;
    layout->onDrawInfo();
}

void PopupFrost::drawList()
{
    auto layout = (LayerFrostList*)_uiContentsUIList;
    layout->onDrawList();
}
 
#pragma mark - ui : set, get

#pragma mark - callback
void PopupFrost::callbackInfo(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 10003 )
        {
            strMessage = GAME_TEXT("t_ui_prison_error_2");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    drawDefault();
    drawFloorInfo();
    drawBattle();
    drawList();
}

void PopupFrost::callbackSkillLevelUp(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        else if ( nResult == 3 )
        {
            strMessage = GAME_TEXT("t_ui_costume_msg_33");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawList();
}

void PopupFrost::callbackSkillReset(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawList();
}

void PopupFrost::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotPrisonFrostAttBonus, CashManager::getInstance()->isAdsfreeTime());
        
        //
        PrisonFrostManager::getInstance()->requestAds(true, CC_CALLBACK_2(PopupFrost::callbackAdsResult, this));
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupFrost::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotPrisonFrostAttBonus);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

void PopupFrost::callbackAdsResult(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 10004 )
        {
            strMessage = GAME_TEXT("t_ui_error_15");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    //
    drawDefault();
}

void PopupFrost::callbackBattleEnter(int count, bool bAnimation)
{
    PrisonFrostManager::getInstance()->setBattleAuto(true);
    PrisonFrostManager::getInstance()->setBattleAutoNow(count);
    PrisonFrostManager::getInstance()->setBattleAutoMax(count);
    _battleAnimation = bAnimation;
    
    if ( count <= 0 )
    {
        return;
    }
    
    //
    onBattle();
}

void PopupFrost::callbackBattleResult(bool bResult, int nResult, std::string reward)
{
    if ( bResult == false )
    {
        PrisonFrostManager::getInstance()->setBattleAuto(false);
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
        
        if ( nResult == 10000 )
        {
            auto popup = PopupFrostResult::create();
            popup->setResult(false, "");
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_30");
            if ( nResult == 2 )
            {
                strMessage = GAME_TEXT("t_ui_error_31");
            }
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    else
    {
        auto popup = PopupFrostResult::create();
        popup->setResult(true, reward);
        popup->show();
    }
    
    //
    drawDefault();
    drawFloorInfo();
    drawBattle();
    drawList();
    
    //
    PrisonFrostManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFrost::callbackBattleResultInfo, this));
}

void PopupFrost::callbackBattleResultInfo(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        //
        PrisonFrostManager::getInstance()->setBattleAuto(false);
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
        return;
    }
    
    if ( PrisonFrostManager::getInstance()->isBattleAuto() == false )
    {
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
    }
    
    if ( PrisonFrostManager::getInstance()->isBattleAuto() == true && PrisonFrostManager::getInstance()->getBattleAutoNow() > 0 )
    {
        PopupLoadingLock::show();
        auto seq = Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
            
            PopupLoadingLock::hide();
            onBattle();
        }), NULL);
        runAction(seq);
    }
}

#pragma mark - click
void PopupFrost::onClickClose(Ref* sender)
{
    hide();
}

void PopupFrost::onClickHelp(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int page = PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_info_last_number")->getValueToInt();

    auto popup = PopupHelp::create(page, "t_ui_frost_prison_info_title_%d", "t_ui_frost_prison_info_msg_%d");
    popup->show();
}

void PopupFrost::onClickTowerInfo(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupFrostInfo::create();
    popup->show();
}

void PopupFrost::onClickAds(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int count = PrisonFrostManager::getInstance()->getAdsCount();
    int countMax = PrisonFrostManager::getInstance()->getAdsCountMax();
    
    if ( count >= countMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_15"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( PrisonFrostManager::getInstance()->getTimeReset() < UtilsDate::getInstance()->getTime() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            PrisonFrostManager::getInstance()->requestInfo(true);
        });
        popup->show();
        return;
    }
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupFrost::callbackAds, this));
    }
}

void PopupFrost::onClickBattle(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);

    if ( PrisonFrostManager::getInstance()->getTimeReset() < UtilsDate::getInstance()->getTime() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            PrisonFrostManager::getInstance()->requestInfo(true);
        });
        popup->show();
        return;
    }
    
    auto popup = PopupFrostEnter::create();
    popup->setCallback(CC_CALLBACK_2(PopupFrost::callbackBattleEnter, this));
    popup->show();
    
    
}

#pragma mark - event
void PopupFrost::onBattle()
{
    std::string countNow = "0";
    std::string countNeed = "0";
    
    int battleAutoNow = PrisonFrostManager::getInstance()->getBattleAutoNow();
    if ( battleAutoNow <= 0 )
    {
        //
        PrisonFrostManager::getInstance()->setBattleAuto(false);
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
        return;
    }
    
    if ( PrisonFrostManager::getInstance()->getTimeReset() < UtilsDate::getInstance()->getTime() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            PrisonFrostManager::getInstance()->requestInfo(true);
        });
        popup->show();
        
        //
        PrisonFrostManager::getInstance()->setBattleAuto(false);
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
        return;
    }
    
    int clearFloor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
    auto objInfoFloor = PrisonFrostManager::getInstance()->getInfoFloor(clearFloor);
    if ( objInfoFloor != nullptr )
    {
        auto objItem = objInfoFloor->getCost();
        if ( objItem != nullptr )
        {
            countNeed = objItem->getCount();
            countNow = ItemsManager::getInstance()->getItems((E_ITEMS)objItem->getIdx());
        }
    }

    if ( countNow.compare("0") == 0 || countNeed.compare("0") == 0 || MafUtils::compareStr(countNow, countNeed) == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        //
        PrisonFrostManager::getInstance()->setBattleAuto(false);
        PrisonFrostManager::getInstance()->setBattleAutoNow(0);
        PrisonFrostManager::getInstance()->setBattleAutoMax(0);
        return;
    }
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();
    auto popupFrostResult =  scene->getChildByName<PopupFrostResult*>("PopupFrostResult");
    if ( popupFrostResult != nullptr )
    {
        popupFrostResult->hide();
    }
    
    //
    PrisonFrostManager::getInstance()->setBattleAutoNow(battleAutoNow - 1);
    
    //
    _uiContentsUIDefault->setVisible(false);

    //
    auto layout = (LayerFrostBattle*)_uiContentsBattle;
    layout->onBattleStart(_battleAnimation);
}
