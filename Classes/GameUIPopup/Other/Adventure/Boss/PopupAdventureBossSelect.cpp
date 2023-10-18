//
//  PopupAdventureBossSelect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupAdventureBossSelect.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Adventure/InfoAdventureBoss.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Other/Adventure/Boss/PopupAdventureBossBattle.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureBossSelect::PopupAdventureBossSelect(void) :
_bInitUI(false),

//
_tab(0),

// widget
_uiContentsList(nullptr),

_uiList(nullptr)
{
    
}

PopupAdventureBossSelect::~PopupAdventureBossSelect(void)
{
    
}

bool PopupAdventureBossSelect::init()
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

void PopupAdventureBossSelect::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiTab();
        uiList();
        
        // network

        _bInitUI = true;
    }
    
    //
    drawTab();
    drawList();
}

#pragma mark - override DelegateListView
void PopupAdventureBossSelect::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupAdventureBossSelect::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width ;
    double height = 159 + 20 ;

    return Size(width, height);
}

void PopupAdventureBossSelect::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(696, 159);

    // obj
    auto obj = _listInfoBoss.at(idx);
    
    auto name = obj->getName();
    auto hp = obj->getHpMax();
    auto bgPath = obj->getIconBannerPath();
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    uiLayout->setTouchEnabled(true);
    uiLayout->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossSelect::onClickSelect, this));
    uiLayout->setTag((int)idx);
    childview->addChild(uiLayout);
   
    
    auto uiBG = ui::ImageView::create(bgPath);
    uiBG->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
    uiLayout->addChild(uiBG);
    
    auto nameBGPath = "Assets/ui_common/base_text_2_7.png";
    if ( obj->isHard() == true )
        nameBGPath = "Assets/ui_common/base_text_2_8.png";
        
    auto uiNameBG = ui::ImageView::create(nameBGPath);
    uiNameBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiNameBG->setPosition(Vec2(0, sizeLayout.height));
    uiLayout->addChild(uiNameBG);
    {
        auto uiName = ui::CText::create(name, GAME_FONT, 28);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiName->setPosition(Vec2(10, uiNameBG->getContentSize().height/2));
        uiName->setTextAreaSize(Size(175, uiName->getContentSize().height));
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        uiNameBG->addChild(uiName);
    }
    
    auto uiHPBG = ui::ImageView::create("Assets/ui/adventure/boss/adventure_boss_hp_01_1.png");
    uiHPBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiHPBG->setPosition(Vec2(10, sizeLayout.height - 70));
    uiLayout->addChild(uiHPBG);
    {
        auto uiHP = ui::CText::create(MafUtils::format("%d/%d", hp, hp), GAME_FONT, 16);
        uiHP->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiHP->setPosition(Vec2(35, uiHPBG->getContentSize().height / 2 + 1));
        uiHPBG->addChild(uiHP);
    }
    
    // rewad
    auto uiReward = ui::Layout::create();
    uiReward->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiReward->setPosition(Vec2(10, 10));
    uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayout->addChild(uiReward);
    {
        auto listReward = obj->getListReward();
        
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(58, 58));
            uiReward->addChild(uiItemBG);
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(5, 0, 0, 0));
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
    
    
    
    // reward bonus
    auto uiRewardBonus = ui::Layout::create();
    uiRewardBonus->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiRewardBonus->setPosition(Vec2(sizeLayout.width - 10, 10));
    uiRewardBonus->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayout->addChild(uiRewardBonus);
    {
        auto listRewardBonus = obj->getListRewardBonus();
        
        for ( int i = 0; i < listRewardBonus.size(); i++ )
        {
            auto objItems = listRewardBonus.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item2_1.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(58, 58));
            uiRewardBonus->addChild(uiItemBG);
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(5, 0, 0, 0));
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
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiRewardBonus);
        uiRewardBonus->setContentSize(sizeReward);
        uiRewardBonus->requestDoLayout();
    }
    
    {
        auto uiText = ui::ImageView::create("Assets/ui_common/bonus_reward_01.png");
        uiText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiText->setPosition(Vec2(uiRewardBonus->getPositionX(), uiRewardBonus->getPositionY() + uiRewardBonus->getContentSize().height));
        uiLayout->addChild(uiText);
    }
    
    if ( obj->isHard() == true )
    {
        auto uiLineEffect = ui::ImageView::create("Assets/ui/adventure/boss/adventure_bossenter_2_fx.png");
        uiLineEffect->setPositionType(ui::Widget::PositionType::PERCENT);
        uiLineEffect->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiLineEffect->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0), FadeIn::create(1.0), NULL)));
        uiLayout->addChild(uiLineEffect);
    }
}

int64_t PopupAdventureBossSelect::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoBoss.size();
    
    return count;
}

#pragma mark - init
void PopupAdventureBossSelect::initVar()
{
    _listInfoBoss = AdventureManager::getInstance()->getListInfoBoss(_tab);
}

void PopupAdventureBossSelect::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(750, 1030) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_advt_boss_title"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossSelect::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

#pragma mark - ui :
void PopupAdventureBossSelect::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height  - 75 - 25 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsTab = ui::Layout::create();
    _uiContentsTab->setContentSize( Size(size.width, 90) );
    _uiContentsTab->setIgnoreAnchorPointForPosition(false);
    _uiContentsTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsTab->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsTab);
    
    _uiContentsList = ui::Layout::create();
    _uiContentsList->setContentSize( Size(size.width, size.height - _uiContentsTab->getContentSize().height) );
    _uiContentsList->setIgnoreAnchorPointForPosition(false);
    _uiContentsList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsList->setPosition( Vec2(size.width / 2, size.height - _uiContentsTab->getContentSize().height) );
    layout->addChild(_uiContentsList);
}

void PopupAdventureBossSelect::uiTab()
{
    auto layout = _uiContentsTab;
    auto size = layout->getContentSize();
    
    auto uiButtons = ui::Layout::create();
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPositionType(ui::Layout::PositionType::PERCENT);
    uiButtons->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_TABS");
    layout->addChild(uiButtons);
    
    {
        auto uiTab = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiTab->setScale9Enabled(true);
        uiTab->setCapInsets(Rect(33,33,2,2));
        uiTab->setContentSize(Size(276, 78));
        uiTab->setTag(0);
        uiTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossSelect::onClickTab, this));
        uiButtons->addChild(uiTab);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_boss_normal_bt"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTab->getContentSize().width / 2, uiTab->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiTab->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiTab->addChild(uiText);
    }
    
    {
        auto uiTab = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
        uiTab->setScale9Enabled(true);
        uiTab->setCapInsets(Rect(33,33,2,2));
        uiTab->setContentSize(Size(276, 78));
        uiTab->setTag(1);
        uiTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureBossSelect::onClickTab, this));
        uiButtons->addChild(uiTab);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_boss_hard_bt"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTab->getContentSize().width / 2, uiTab->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiTab->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiTab->addChild(uiText);
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(15, 0, 0, 0));
        uiTab->setLayoutParameter(params);
    }
    
    auto sizeTab = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeTab);
    uiButtons->requestDoLayout();
}

void PopupAdventureBossSelect::uiList()
{
    auto layout = _uiContentsList;
    auto size = layout->getContentSize();
   
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}


#pragma mark - ui : draw
void PopupAdventureBossSelect::drawTab()
{
    auto layout = _uiContentsTab;
    
    auto uiButtons = utils::findChild<ui::Layout*>(layout, "UI_TABS");
    
    if ( auto uiTab = utils::findChild<ui::Button*>(uiButtons, 0); uiTab != nullptr )
    {
        auto path = "Assets/ui_common/btn_1_off.png";
        if ( _tab == 0 )
            path = "Assets/ui_common/btn_1_1_on.png";
        
        uiTab->loadTextureNormal(path);
    }
    
    if ( auto uiTab = utils::findChild<ui::Button*>(uiButtons, 1); uiTab != nullptr )
    {
        auto path = "Assets/ui_common/btn_1_off.png";
        if ( _tab == 1 )
            path = "Assets/ui_common/btn_1_9_on.png";
        
        uiTab->loadTextureNormal(path);
    }
}

void PopupAdventureBossSelect::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - click
void PopupAdventureBossSelect::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupAdventureBossSelect::onClickTab(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto tab = uiBtn->getTag();
    
    if ( _tab == tab )
    {
        return;
    }
    
    _tab = tab;
    _listInfoBoss = AdventureManager::getInstance()->getListInfoBoss(_tab);
    
    drawTab();
    drawList();
}

void PopupAdventureBossSelect::onClickSelect(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Layout*>(sender);
    
    auto idx = uiBtn->getTag();
    
    auto objInfoBoss = _listInfoBoss.at(idx);
    if ( objInfoBoss == nullptr )
    {
        return;
    }
    
    auto bossIdx = objInfoBoss->getIdx();
    auto bossName = objInfoBoss->getName();
    
    auto message = GAME_TEXTFORMAT("t_ui_advt_boss_msg6", bossName.c_str());
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), message);
    popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
    
        PopupLoading::show();
        AdventureManager::getInstance()->requestBossSelect(bossIdx, CC_CALLBACK_2(PopupAdventureBossSelect::callbackSelect, this));
    });
    popup->show();
}

#pragma mark - callback
void PopupAdventureBossSelect::callbackSelect(bool bResult, int nResult)
{
    /*
    10001: 이미 클리어한 보스
    10005: 공격볼?? 부족
    10007: 잘못된값 요청 (보스정보가없거나 DB에 저장된정보와 다른정보요청시)
     */
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    auto popup = PopupAdventureBossBattle::create();
    popup->show();
    
    hide();
}

#pragma mark - set, get
