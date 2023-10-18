//
//  PopupAdventureBossBattle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupAdventureBossBattle.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Adventure/InfoAdventureBoss.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Other/Adventure/Boss/Layer/LayerAdventureBossBattle.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupAdventureBossBattle::PopupAdventureBossBattle(void) :
_bInitUI(false),

//
_objInfoBoss(nullptr),
_bulletIdx(0),
_bulletItemCount(0),

// widget
_uiContentsBattle(nullptr),
_uiContentsDefault(nullptr),
_uiContentsReward(nullptr),
_uiContentsButtons(nullptr)
{
    
}

PopupAdventureBossBattle::~PopupAdventureBossBattle(void)
{
    CC_SAFE_RELEASE_NULL(_objInfoBoss);
}

bool PopupAdventureBossBattle::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupAdventureBossBattle::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiDefault();
        uiReward();
        uiButtons();
        
        // network

        _bInitUI = true;
    }
    
    //
    drawDefault();
    drawReward();
    drawButtons();
}

#pragma mark - init
void PopupAdventureBossBattle::initVar()
{
    auto bossIdx = AdventureManager::getInstance()->getBossIdx();
    
    _objInfoBoss = AdventureManager::getInstance()->getInfoBoss(bossIdx);
    _objInfoBoss->retain();
    
    _listBullet.push_back(117);
    _listBullet.push_back(118);
    _listBullet.push_back(119);
    _listBullet.push_back(120);
    _listBullet.push_back(121);
    
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountMax = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());
    if ( itemCountMax > 0 )
    {
        _bulletItemCount = itemCountMax;
    }
}

void PopupAdventureBossBattle::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(750, 1030) );
    
    // label title
    auto uiTitle = ui::CText::create(_objInfoBoss->getName(), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossBattle::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::ADVENTURE_BOSS_BULLET_FOREST);
    list.push_back(E_ITEMS::ADVENTURE_BOSS_BULLET_EARTH);
    list.push_back(E_ITEMS::ADVENTURE_BOSS_BULLET_GLACIER);
    list.push_back(E_ITEMS::ADVENTURE_BOSS_BULLET_VOLCANO);
    list.push_back(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui :
void PopupAdventureBossBattle::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 18;
    size.height = size.height - 75 - 25 - 50;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    // 183 * 138(732 * 552)
    auto uiContentsBattle = LayerAdventureBossBattle::create(Size(size.width, 552));
    uiContentsBattle->setContentSize( Size(size.width, 552) );
    uiContentsBattle->setIgnoreAnchorPointForPosition(false);
    uiContentsBattle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    uiContentsBattle->setPosition( Vec2(size.width / 2, size.height) );
    uiContentsBattle->setCallbackAttackFinish(CC_CALLBACK_0(PopupAdventureBossBattle::callbackAttackFinish, this));
    uiContentsBattle->setBulletItemIdx(117);
    uiContentsBattle->setBulletItemCount(_bulletItemCount);
    layout->addChild(uiContentsBattle);
    
    _uiContentsBattle = uiContentsBattle;
    
    _uiContentsDefault = ui::Layout::create();
    _uiContentsDefault->setContentSize( Size(size.width, 552) );
    _uiContentsDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsDefault->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsDefault);
    
    _uiContentsReward = ui::Layout::create();
    _uiContentsReward->setContentSize( Size(size.width, 135) );
    _uiContentsReward->setIgnoreAnchorPointForPosition(false);
    _uiContentsReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsReward->setPosition( Vec2(size.width / 2, size.height - _uiContentsDefault->getContentSize().height) );
    layout->addChild(_uiContentsReward);
    
    _uiContentsButtons = ui::Layout::create();
    _uiContentsButtons->setContentSize( Size(size.width, size.height - _uiContentsDefault->getContentSize().height - _uiContentsReward->getContentSize().height) );
    _uiContentsButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsButtons);
}

void PopupAdventureBossBattle::uiDefault()
{
    auto layout = _uiContentsDefault;
    auto size = layout->getContentSize();
    
    auto uiClear = ui::Layout::create();
    uiClear->setContentSize(size);
    uiClear->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiClear->setPosition(Vec2(size.width / 2, size.height / 2));
    uiClear->setBackGroundColor(Color3B::BLACK);
    uiClear->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiClear->setOpacity(200);
    uiClear->setName("UI_CLEAR");
    uiClear->setVisible(false);
    layout->addChild(uiClear);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/purchase/icon_complete_1.png");
        uiIcon->setPosition(Vec2(uiClear->getContentSize().width / 2, uiClear->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale(3);
        uiClear->addChild(uiIcon);
    }
    
    auto uiHPBarBG = ui::ImageView::create("Assets/ui/adventure/boss/adventure_boss_hp_02_1.png");
    uiHPBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiHPBarBG->setPosition(Vec2(size.width / 2, size.height - 20));
    layout->addChild(uiHPBarBG);
    {
        auto uiHPBar = ui::LoadingBar::create("Assets/ui/adventure/boss/adventure_boss_hp_02_2.png");
        uiHPBar->setPosition(Vec2(uiHPBarBG->getContentSize().width / 2, uiHPBarBG->getContentSize().height / 2));
        uiHPBar->setPercent(50);
        uiHPBar->setName("UI_HPBAR");
        uiHPBarBG->addChild(uiHPBar);
        
        auto uiHP = ui::CText::create("HP 0 / 0", GAME_FONT, 20);
        uiHP->setPosition(Vec2(uiHPBarBG->getContentSize().width / 2, uiHPBarBG->getContentSize().height / 2));
        uiHP->setName("UI_HP");
        uiHPBarBG->addChild(uiHP);
    }
    
    auto uiTimeBG = ui::ImageView::create("Assets/ui_common/base_text_1_2.png");
    uiTimeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTimeBG->setPosition(Vec2(size.width / 2, size.height - 50));
    layout->addChild(uiTimeBG);
    {
        auto uiTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_advt_boss_msg1", 0, 0, 0), GAME_FONT, 24);
        uiTime->setPosition(Vec2(uiTimeBG->getContentSize().width / 2, uiTimeBG->getContentSize().height / 2));
        uiTime->setTextColor(Color4B(COLOR_TITLE));
        uiTime->setName("UI_TIME");
        uiTimeBG->addChild(uiTime);
    }
    
    auto uiMissionBG = ui::ImageView::create("Assets/ui_common/base_box_1_5.png");
    uiMissionBG->setScale9Enabled(true);
    uiMissionBG->setCapInsets(Rect(20, 50, 10, 10));
    uiMissionBG->setContentSize(Size(160, 130));
    uiMissionBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiMissionBG->setPosition(Vec2(10, size.height - 100));
    layout->addChild(uiMissionBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_boss_msg4"), GAME_FONT, 22);
        uiText->setPosition(Vec2(uiMissionBG->getContentSize().width / 2, uiMissionBG->getContentSize().height - 16.5));
        uiText->setTextAreaSize(Size(uiMissionBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->enableOutline(Color4B::BLACK, 1);
        uiMissionBG->addChild(uiText);
        
        auto uiMission = ui::Layout::create();
        uiMission->setContentSize(Size(uiMissionBG->getContentSize().width, uiMissionBG->getContentSize().height - 33));
        uiMission->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiMission->setPosition(Vec2(uiMissionBG->getContentSize().width / 2, 0));
        uiMission->setName("UI_MISSION");
        uiMission->setLayoutType(ui::Layout::Type::VERTICAL);
        uiMissionBG->addChild(uiMission);
    }
    
    auto uiCountBG = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiCountBG->setScale9Enabled(true);
    uiCountBG->setContentSize(Size(110, 50));
    uiCountBG->setPosition(Vec2(size.width / 2, 40));
    layout->addChild(uiCountBG);
    {
        auto uiCount = ui::CText::create("0", GAME_FONT, 24);
        uiCount->setPositionType(ui::Widget::PositionType::PERCENT);
        uiCount->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiCount->setName("UI_COUNT");
        uiCountBG->addChild(uiCount);
        
        auto uiArrowL = ui::Button::create("Assets/ui_common/btn_quantity_2_down.png");
        uiArrowL->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiArrowL->setPosition(Vec2(-10, uiCountBG->getContentSize().height / 2));
        uiArrowL->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossBattle::onClickArrow, this));
        uiArrowL->setTag(-1);
        uiCountBG->addChild(uiArrowL);
        
        auto uiArrowR = ui::Button::create("Assets/ui_common/btn_quantity_2_up.png");
        uiArrowR->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiArrowR->setPosition(Vec2(uiCountBG->getContentSize().width + 10, uiCountBG->getContentSize().height / 2));
        uiArrowR->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossBattle::onClickArrow, this));
        uiArrowR->setTag(+1);
        uiCountBG->addChild(uiArrowR);
    }
}

void PopupAdventureBossBattle::uiReward()
{
    auto layout = _uiContentsReward;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/tab_bar_2_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiTitle = ui::ImageView::create("Assets/ui_common/bonus_2_1.png");
    uiTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiTitle->setPosition(Vec2(45, size.height - 55));
    layout->addChild(uiTitle);
    
    auto uiReward = ui::Layout::create();
    uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiReward->setPosition(Vec2(size.width / 2, 40));
    uiReward->setName("UI_REWARD");
    uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(uiReward);
}

void PopupAdventureBossBattle::uiButtons()
{
    auto layout = _uiContentsButtons;
    auto size = layout->getContentSize();
    
    //
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_boss_msg2"), GAME_FONT, 28);
    uiText->setPosition(Vec2(size.width / 2, size.height - 57.5));
    layout->addChild(uiText);
    
    //
    auto uiButtons = ui::Layout::create();
    uiButtons->setIgnoreAnchorPointForPosition(false);
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiButtons->setPosition(Vec2(size.width / 2, 0));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        auto uiBulletChange = ui::Button::create("Assets/ui_common/btn_1_2_on.png");
        uiBulletChange->setScale9Enabled(true);
        uiBulletChange->setContentSize(Size(180, 78));
        uiBulletChange->setCapInsets(Rect(30, 32, 2, 2));
        uiBulletChange->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossBattle::onClickBulletChange, this));
        uiButtons->addChild(uiBulletChange);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_boss_bt1"), GAME_FONT, 28);
            uiText->setPosition(Vec2(uiBulletChange->getContentSize().width / 2, uiBulletChange->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiBulletChange->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiBulletChange->addChild(uiText);
        }
        
        auto uiAttack = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiAttack->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossBattle::onClickAttack, this));
        uiButtons->addChild(uiAttack);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(20, 0, 0, 0));
            uiAttack->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_atk"), GAME_FONT, 28);
            uiText->setPosition(Vec2(uiAttack->getContentSize().width / 2, uiAttack->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiAttack->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiAttack->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void PopupAdventureBossBattle::drawDefault()
{
    auto layout = _uiContentsDefault;
    
    auto hpNow = _objInfoBoss->getHpNow();
    auto hpMax = _objInfoBoss->getHpMax();
    
    double percent = (double)hpNow / hpMax * 100;
    if ( percent > 100 )
        percent = 100;
   
    if ( auto uiClear = utils::findChild<ui::Layout*>(layout, "UI_CLEAR"); uiClear != nullptr )
    {
        if ( _objInfoBoss->getHpNow() <= 0 )
            uiClear->setVisible(true);
        else
            uiClear->setVisible(false);
    }
    
    if ( auto uiHPBar = utils::findChild<ui::LoadingBar*>(layout, "UI_HPBAR"); uiHPBar != nullptr )
    {
        uiHPBar->setPercent(percent);
    }
    
    if ( auto uiHP = utils::findChild<ui::CText*>(layout, "UI_HP"); uiHP != nullptr )
    {
        uiHP->setString(MafUtils::format("HP %d / %d", hpNow, hpMax));
    }
    
    if ( auto uiTime = utils::findChild<ui::CText*>(layout, "UI_TIME"); uiTime != nullptr )
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            long timeDiff = AdventureManager::getInstance()->getBossEndTime() - UtilsDate::getInstance()->getTime();
            
            int day = (int)(timeDiff / 86400);
            int hour = (int)(timeDiff % 86400 / 3600);
            int min = (int)(timeDiff % 3600 / 60);
            
            ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_advt_boss_msg1", day, hour, min));
        });
        auto action2 = DelayTime::create(60.0);
        
        uiTime->stopAllActions();
        uiTime->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
    }
    
    if ( auto uiCount = utils::findChild<ui::CText*>(layout, "UI_COUNT"); uiCount != nullptr )
    {
        uiCount->setString(std::to_string(_bulletItemCount));
    }
    
    if ( auto uiMission = utils::findChild<ui::Layout*>(layout, "UI_MISSION"); uiMission != nullptr )
    {
        uiMission->removeAllChildren();
        
        if ( auto weaknessIdx = _objInfoBoss->getWeakness1(); weaknessIdx != 0 )
        {
            auto uiMission1 = ui::Layout::create();
            uiMission1->setContentSize(Size(uiMission->getContentSize().width, 50));
            uiMission->addChild(uiMission1);
            
            auto weaknessCountMax = _objInfoBoss->getWeakness1CountMax();
            auto weaknessCountNow =  weaknessCountMax - _objInfoBoss->getWeakness1CountNow();
            weaknessCountNow = MAX(weaknessCountNow, 0);
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(weaknessIdx);
            if ( objItem != nullptr )
            {
                auto uiIcon = ui::ImageView::create(objItem->getPath());
                uiIcon->setPosition(Vec2(15, uiMission1->getContentSize().height / 2));
                uiMission1->addChild(uiIcon);
                
                auto uiCount = ui::CText::create(MafUtils::format("%d/%d", weaknessCountNow, weaknessCountMax), GAME_FONT, 20);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiCount->setPosition(Vec2(32, uiMission1->getContentSize().height / 2 + 2));
                uiMission1->addChild(uiCount);
                
                if ( weaknessCountNow >= weaknessCountMax )
                {
                    auto uiClear = ui::ImageView::create("Assets/ui_common/clear01.png");
                    uiClear->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    uiClear->setPosition(Vec2(uiMission1->getContentSize().width - 5, uiMission1->getContentSize().height / 2));
                    uiMission1->addChild(uiClear);
                }
            }
            
            if ( auto weaknessIdx = _objInfoBoss->getWeakness2(); weaknessIdx != 0 )
            {
                auto uiMission1 = ui::Layout::create();
                uiMission1->setContentSize(Size(uiMission->getContentSize().width, 50));
                uiMission->addChild(uiMission1);
                
                auto weaknessCountMax = _objInfoBoss->getWeakness2CountMax();
                auto weaknessCountNow =  weaknessCountMax - _objInfoBoss->getWeakness2CountNow();
                weaknessCountNow = MAX(weaknessCountNow, 0);
                
                auto objItem = ItemsManager::getInstance()->getInfoItems(weaknessIdx);
                if ( objItem != nullptr )
                {
                    auto uiIcon = ui::ImageView::create(objItem->getPath());
                    uiIcon->setPosition(Vec2(15, uiMission1->getContentSize().height / 2));
                    uiMission1->addChild(uiIcon);
                    
                    auto uiCount = ui::CText::create(MafUtils::format("%d/%d", weaknessCountNow, weaknessCountMax), GAME_FONT, 20);
                    uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    uiCount->setPosition(Vec2(32, uiMission1->getContentSize().height / 2 + 2));
                    uiMission1->addChild(uiCount);
                    
                    if ( weaknessCountNow >= weaknessCountMax )
                    {
                        auto uiClear = ui::ImageView::create("Assets/ui_common/clear01.png");
                        uiClear->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                        uiClear->setPosition(Vec2(uiMission1->getContentSize().width - 5, uiMission1->getContentSize().height / 2));
                        uiMission1->addChild(uiClear);
                    }
                }
            }
        }
    }
}

void PopupAdventureBossBattle::drawReward()
{
    auto layout = _uiContentsReward;
    
    if ( auto uiReward = layout->getChildByName<ui::Layout*>("UI_REWARD"); uiReward != nullptr )
    {
        uiReward->removeAllChildren();
        
        auto weakness1CountMax = _objInfoBoss->getWeakness1CountMax();
        auto weakness1CountNow =  weakness1CountMax - _objInfoBoss->getWeakness1CountNow();
        weakness1CountNow = MAX(weakness1CountNow, 0);
        
        auto weakness2CountMax = _objInfoBoss->getWeakness2CountMax();
        auto weakness2CountNow =  weakness2CountMax - _objInfoBoss->getWeakness2CountNow();
        weakness2CountNow = MAX(weakness2CountNow, 0);
        
        auto listRewardBonus = _objInfoBoss->getListRewardBonus();
        for ( int i = 0; i < listRewardBonus.size(); i++ )
        {
            auto objItems = listRewardBonus.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item2_1.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(58, 58));
            uiReward->addChild(uiItemBG);
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiItemBG->setLayoutParameter(params);
            }
            
            {
                auto uiItemIcon = ui::ImageView::create(objItems->getPath());
                uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiItemIcon);
                
                std::string rewardCount = objItems->getCount();
                std::string rewardCountBonus = objItems->getCount();
                 
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                    MafUtils::convertNumberToShort(rewardCountBonus);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiTextReward);
            }
            
            if ( weakness1CountNow < weakness1CountMax || weakness2CountNow < weakness2CountMax )
            {
                auto uiDisable = ui::Layout::create();
                uiDisable->setContentSize(uiItemBG->getContentSize());
                uiDisable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                uiDisable->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiDisable->setBackGroundColor(Color3B::BLACK);
                uiDisable->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                uiDisable->setOpacity(200);
                uiItemBG->addChild(uiDisable);
            }
        }
        
        auto listReward = _objInfoBoss->getListReward();
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(58, 58));
            uiReward->addChild(uiItemBG);
            
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiItemBG->setLayoutParameter(params);
            }
            
            {
                auto uiItemIcon = ui::ImageView::create(objItems->getPath());
                uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiItemIcon);
                
                std::string rewardCount = objItems->getCount();
                std::string rewardCountBonus = objItems->getCount();
                 
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                    MafUtils::convertNumberToShort(rewardCountBonus);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiTextReward);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiReward);
        uiReward->setContentSize(sizeReward);
        uiReward->requestDoLayout();
    }
}

void PopupAdventureBossBattle::drawButtons()
{
    auto layout = _uiContentsButtons;
    
    if ( auto uiButtons = utils::findChild<ui::Layout*>(layout, "UI_BUTTONS"); uiButtons != nullptr )
    {
        auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
        uiButtons->setContentSize(sizeButtons);
        uiButtons->requestDoLayout();
    }
}

#pragma mark - click
void PopupAdventureBossBattle::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupAdventureBossBattle::onClickArrow(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountMax = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());
    
    //
    _bulletItemCount = _bulletItemCount + uiBtn->getTag();
    
    if ( _bulletItemCount <= 0 )
        _bulletItemCount = itemCountMax;
    else if ( _bulletItemCount > itemCountMax )
    {
        if ( itemCountMax > 0 )
            _bulletItemCount = 1;
        else
            _bulletItemCount = 0;
    }
    
    //
    drawDefault();
    
    //
    auto layout = (LayerAdventureBossBattle*)_uiContentsBattle;
    layout->setBulletItemCount(_bulletItemCount);
}

void PopupAdventureBossBattle::onClickBulletChange(cocos2d::Ref* sender)
{
    _bulletIdx++;
    if ( _bulletIdx >= _listBullet.size() )
    {
        _bulletIdx = 0;
    }
    
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountMax = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());

    _bulletItemCount = itemCountMax;
    
    //
    drawDefault();
    
    //
    auto layout = (LayerAdventureBossBattle*)_uiContentsBattle;
    layout->setBulletItemIdx(itemIdx);
    layout->setBulletItemCount(_bulletItemCount);
    layout->onBulletChange();
}

void PopupAdventureBossBattle::onClickAttack(cocos2d::Ref* sender)
{
    auto bossHp = _objInfoBoss->getHpNow();
    if ( bossHp <= 0 )
    {
        return;
    }
    
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountNow = _bulletItemCount;
    auto itemCountMax = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());
    if ( itemCountNow <= 0 || itemCountMax < itemCountNow )
    {
        return;
    }
    
    //
    PopupLoadingLock::show();
    
    //
    auto layout = (LayerAdventureBossBattle*)_uiContentsBattle;
    layout->onAttack();
}

#pragma mark - callback
void PopupAdventureBossBattle::callbackAttackFinish()
{
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountNow = _bulletItemCount;
    int bossIdx = AdventureManager::getInstance()->getBossIdx();
    
    //
    PopupLoadingLock::hide();
    
    //
    PopupLoading::show();
    AdventureManager::getInstance()->requestBossAttack(bossIdx, itemIdx, itemCountNow, CC_CALLBACK_2(PopupAdventureBossBattle::callbackNetworkAttack, this));
}

void PopupAdventureBossBattle::callbackNetworkAttack(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        if ( nResult == 10008 )
        {
            strMessageKey = "t_ui_advt_boss_error";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            PopupLoading::show();
            AdventureManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupAdventureBossBattle::callbackNetworkInfo, this));
        });
        popup->show();
        return;
    }
    
    auto itemIdx = _listBullet.at(_bulletIdx);
    auto itemCountMax = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());

    _bulletItemCount = itemCountMax;
    
    //
    auto layout = (LayerAdventureBossBattle*)_uiContentsBattle;
    layout->setBulletItemIdx(itemIdx);
    layout->setBulletItemCount(_bulletItemCount);
    
    //
    drawDefault();
    drawReward();
    drawButtons();
}

void PopupAdventureBossBattle::callbackNetworkInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    hide();
}


#pragma mark - set, get
