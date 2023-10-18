//
//  PopupFlashman.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFlashman.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Ranking/PopupPageRanking.h"
#include "GameUIPopup/Other/Flashman/EnumFlashman.h"
#include "GameUIPopup/Other/Flashman/PopupFlashmanEnterBuy.h"
#include "GameUIPopup/Other/Flashman/PopupFlashmanMission.h"
#include "GameUIPopup/Other/Flashman/PopupFlashmanRankReward.h"
#include "GameUIPopup/Other/Flashman/Layer/LayerFlashmanBattle.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/FlashmanManager.h"

USING_NS_CC;

PopupFlashman* PopupFlashman::create()
{
    PopupFlashman* pRet = new(std::nothrow) PopupFlashman();
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

PopupFlashman::PopupFlashman() :
_bInitUI(false),

//
_bAttackBtnPressed(false),
_attackBtnAnimDelay(0),

//
_resultScoreBest(0),
_resultScoreNow(0),
_resultScoreNew(false),
_resultReward(""),

// widget
_uiContentsBattle(nullptr),
_uiContentsUIDefault(nullptr),
_uiContentsUIButtons(nullptr),

_uiContentsUIResult(nullptr),
_uiContentsUIScoreAccReward(nullptr),

_uiFeverBG(nullptr)
{
    
}

PopupFlashman::~PopupFlashman(void)
{
    FlashmanManager::getInstance()->undelegateResult(this);
    
    SoundManager::ChangeSoundState(E_SOUND_TYPE::SCENE);
}

bool PopupFlashman::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    setName("PopupFlashman");
    
    //
    initVar();
    initUI();
    
    //
    FlashmanManager::getInstance()->delegateResult(new MafFunction<void(bool, int, int, bool, std::string)>(CC_CALLBACK_5(PopupFlashman::callbackResult, this), this));
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::FLASHMAN_INFO, this);
    
    //
    scheduleUpdate();
    
    return true;
}

void PopupFlashman::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiDefault();
        uiResult();
        uiScoreAccReward();
        uiButtons();
        
        // 시나리오 연출
        if(DialogManager::getInstance()->isViewDialog("MINIGAME_FLASHMAN_EP") == 0)
        {
            DialogManager::getInstance()->showDialog("MINIGAME_FLASHMAN_EP");
        }
        
        //
        FlashmanManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFlashman::callbackInfo, this));
        FlashmanManager::getInstance()->requestRankReward(true, CC_CALLBACK_2(PopupFlashman::callbackRankReward, this));
        
        //
        SoundManager::ChangeSoundState(E_SOUND_TYPE::CONTENTS);
        SoundManager::SoundBgmStart(SOUND_FLASH_BGM_1, E_SOUND_TYPE::CONTENTS);
    }
    
    //
    drawDefault();
    drawButtons();
}

void PopupFlashman::update(float dt)
{
    if ( _attackBtnAnimDelay > 0 )
    {
        _attackBtnAnimDelay -= dt;
        _attackBtnAnimDelay = MAX(_attackBtnAnimDelay, 0);
    }
}

#pragma mark - override
void PopupFlashman::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::FLASHMAN_INFO )
    {
        drawDefault();
        drawButtons();
    }
}

#pragma mark - init
void PopupFlashman::initVar()
{
    
}

void PopupFlashman::initUI()
{
    // size
    Size size = Size(185 * 4, 307 * 4);

    _spriteContainer = ui::Scale9Sprite::create(PIXEL, Rect::ZERO);
    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(Vec2(getContentSize().width / 2, 25));
    _spriteContainer->setOpacity(0);
    addChild(_spriteContainer);
    {
        auto spriteBg = Sprite::create("Assets/ui/flashman/bsbs_popup_1_1.png");
        spriteBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBg->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
        spriteBg->getTexture()->setTexParameters(texParams);
        spriteBg->setScale(4);
        _spriteContainer->addChild(spriteBg);
    }
    
    // label title
    auto uiTitle = ui::ImageView::create(TextManager::getInstance()->getLangFileName("Assets/ui/flashman/flashman_logo_%s.png"));
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 52));
    _spriteContainer->addChild(uiTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFlashman::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 52);
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupFlashman::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsBattle = LayerFlashmanBattle::create(Size(684, 532));
    _uiContentsBattle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsBattle->setPosition( Vec2(size.width / 2, size.height - 140) );
    layout->addChild(_uiContentsBattle);
    
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(684, 532) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height - 140) );
    layout->addChild(_uiContentsUIDefault);
    
    _uiContentsUIResult = ui::Layout::create();
    _uiContentsUIResult->setContentSize( Size(684, 532) );
    _uiContentsUIResult->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIResult->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIResult->setPosition( Vec2(size.width / 2, size.height - 140) );
    _uiContentsUIResult->setTouchEnabled(true);
    _uiContentsUIResult->setVisible(false);
    layout->addChild(_uiContentsUIResult);
    
    _uiContentsUIScoreAccReward = ui::Layout::create();
    _uiContentsUIScoreAccReward->setContentSize( Size(684, 532) );
    _uiContentsUIScoreAccReward->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIScoreAccReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIScoreAccReward->setPosition( Vec2(size.width / 2, size.height - 140) );
    _uiContentsUIScoreAccReward->setTouchEnabled(true);
    _uiContentsUIScoreAccReward->setVisible(false);
    layout->addChild(_uiContentsUIScoreAccReward);
    
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(183 * 4, 104 * 4) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 320) );
    layout->addChild(_uiContentsUIButtons);
}

void PopupFlashman::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    //
    auto uiBtnRank = ui::Button::create("Assets/ui/ranking/icon_ranking_2.png");
    uiBtnRank->setPosition(Vec2(size.width - 60, size.height - 50));
    uiBtnRank->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickRank, this));
    layout->addChild(uiBtnRank);
    {
        auto uiText = Label::createWithTTF(GAME_TEXT("t_ui_rank_icon"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiText->setPosition(uiBtnRank->getContentSize().width / 2, 20);
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnRank->addChild(uiText);
    }
    
    //
    auto uiBtnScoreBonus = ui::Button::create("Assets/ui/raid/icon_cumulative_lv.png");
    uiBtnScoreBonus->setPosition(Vec2(size.width - 60, size.height - 150));
    uiBtnScoreBonus->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickScoreAcc, this));
    layout->addChild(uiBtnScoreBonus);
    {
        auto uiText = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_icon"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiText->setPosition(uiBtnScoreBonus->getContentSize().width / 2, 10);
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnScoreBonus->addChild(uiText);
    }
    
    auto uiBtnDexLoots = ui::Button::create("Assets/ui/flashman/icon_booty.png");
    uiBtnDexLoots->setPosition(Vec2(size.width - 60, size.height - 250));
    uiBtnDexLoots->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickDexLoots, this));
    layout->addChild(uiBtnDexLoots);
    {
        auto uiText = Label::createWithTTF(GAME_TEXT("t_ui_loots_title_1"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiText->setPosition(uiBtnDexLoots->getContentSize().width / 2, 10);
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnDexLoots->addChild(uiText);
    }
    
    //
    auto uiTimer = ui::CText::create(" ", GAME_FONT, 24);
    uiTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiTimer->setPosition(Vec2(25, size.height - 25));
    uiTimer->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiTimer);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            long time = FlashmanManager::getInstance()->getTimeReset() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            time = MAX(time, 0);
            
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            day = MAX(day, 0);
            hour = MAX(hour, 0);
            min = MAX(min, 0);
            
            ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_minigame_end_time", day, hour, min));
        });
        auto action2 = DelayTime::create(60.f);
        uiTimer->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
    }
    
    auto uiScoreIcon = ui::ImageView::create("Assets/ui/flashman/total_score_01.png");
    uiScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiScoreIcon->setPosition(Vec2(25, size.height - 75));
    layout->addChild(uiScoreIcon);
    
    auto uiScore = ui::CText::create("98562378", GAME_FONT, 28);
    uiScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiScore->setPosition(Vec2(uiScoreIcon->getPositionX() + uiScoreIcon->getContentSize().width + 10, uiScoreIcon->getPositionY()));
    uiScore->setTextColor(Color4B::YELLOW);
    uiScore->enableOutline(Color4B::BLACK, 2);
    uiScore->setName("UI_SCORE");
    layout->addChild(uiScore);
    
    //
    auto uiMission = ui::Button::create("Assets/ui/bg_icon/icon_mission.png");
    uiMission->setPosition(Vec2(50, size.height - 140));
    uiMission->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickMission, this));
    uiMission->setName("UI_MISSION");
    layout->addChild(uiMission);
    {
        auto uiTextBox = ui::Button::create("Assets/ui_common/base_text_2_4.png");
        uiTextBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextBox->setPosition(Vec2(uiMission->getContentSize().width, uiMission->getContentSize().height / 2));
        uiTextBox->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickMission, this));
        uiTextBox->setName("UI_MISSION_BG");
        uiMission->addChild(uiTextBox);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_raid_tickt_noti_off"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTextBox->getContentSize().width / 2, uiTextBox->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiTextBox->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextColor(Color4B(204, 204, 204, 255));
        uiText->setName("UI_MISSION_TEXT");
        uiTextBox->addChild(uiText);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(0, uiMission->getContentSize().height - 15);
        uiReddot->setVisible(false);
        uiMission->addChild(uiReddot);
        
        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::FLASHMAN_MISSION_COMPLETE, uiReddot);
    }
    
    // 설명 버튼
    auto uiBtnHelp = ui::Button::create("Assets/ui_common/icon_help.png");
    uiBtnHelp->setPosition(Vec2(50, size.height - 220));
    uiBtnHelp->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickHelp, this));
    layout->addChild(uiBtnHelp);
    
    // 입장 버튼
    auto uiBtnEnter = ui::Button::create("Assets/ui/flashman/bsbs_start_01.png");
    uiBtnEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnEnter->setPosition(Vec2(size.width / 2, 25));
    uiBtnEnter->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickEnter, this));
    uiBtnEnter->setName("UI_ENTER");
    layout->addChild(uiBtnEnter);
    {
        auto uiText = ui::CText::create("(0/0)", GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtnEnter->getContentSize().width / 2, 26));
        uiText->setName("UI_TEXT");
        uiBtnEnter->addChild(uiText);
    }
    
    // 스킵 버튼
    auto uiBtnEnterSkip = ui::Button::create("Assets/ui/flashman/bsbs_start_02.png");
    uiBtnEnterSkip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnEnterSkip->setPosition(Vec2(size.width - 25, 25));
    uiBtnEnterSkip->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickEnterSkip, this));
    uiBtnEnterSkip->setName("UI_ENTER_SKIP");
    layout->addChild(uiBtnEnterSkip);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_clear_bt"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtnEnterSkip->getContentSize().width / 2, uiBtnEnterSkip->getContentSize().height - 3 - 19.5));
        uiText->enableShadow(Color4B(71, 0, 131, 255));
        uiBtnEnterSkip->addChild(uiText);
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_coin_2.png", "0", uiBtnEnterSkip->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiBtnEnterSkip->getContentSize().width / 2, 3 + 22.5));
        uiCost->setName("UI_COST");
        uiBtnEnterSkip->addChild(uiCost);
        
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_grade_c.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIcon->setPosition(Vec2(5, uiBtnEnterSkip->getContentSize().height));
        uiIcon->setName("UI_ICON");
        uiBtnEnterSkip->addChild(uiIcon);
    }
}

void PopupFlashman::uiResult()
{
    auto layout = _uiContentsUIResult;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_bg_2_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x_1.png");
    uiClose->setPosition(Vec2(size.width - 50, size.height - 40));
    uiClose->addClickEventListener([=](Ref* sender){
        _uiContentsUIResult->setVisible(false);
    });
    layout->addChild(uiClose);
    
    auto uiGrade = ui::CText::create("C", GAME_FONT, 80);
    uiGrade->setPosition(Vec2(size.width / 2, size.height - 45));
    uiGrade->setTextColor(Color4B(255, 240, 0, 255));
    uiGrade->enableBold();
    uiGrade->setName("UI_GRADE");
    layout->addChild(uiGrade);
    
    auto uiPoint = ui::CText::create("(+0)", GAME_FONT, 25);
    uiPoint->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiPoint->setPosition(Vec2(size.width / 2, size.height - 45));
    uiPoint->setTextColor(Color4B(255, 139, 0, 255));
    uiPoint->setName("UI_POINT");
    uiPoint->setVisible(false);
    layout->addChild(uiPoint);
    
    auto uiScoreBest = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
    uiScoreBest->setScale9Enabled(true);
    uiScoreBest->setCapInsets(Rect(10,10,2,2));
    uiScoreBest->setContentSize(Size(642, 48));
    uiScoreBest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiScoreBest->setPosition(Vec2(size.width / 2, size.height - 160 + 5));
    layout->addChild(uiScoreBest);
    {
        auto uiIconNew = ui::ImageView::create("Assets/ui_common/icon_new.png");
        uiIconNew->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiIconNew->setPosition(Vec2(0, uiScoreBest->getContentSize().height));
        uiIconNew->setName("UI_ICON_NEW");
        uiIconNew->setVisible(false);
        uiScoreBest->addChild(uiIconNew);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_result_1"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiScoreBest->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(165, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiScoreBest->addChild(uiText);
        
        auto uiScore = ui::CText::create("0", GAME_FONT, 32);
        uiScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiScore->setPosition(Vec2(uiScoreBest->getContentSize().width - 20, uiScoreBest->getContentSize().height / 2));
        uiScore->setTextColor(Color4B(0, 255, 0, 255));
        uiScore->setName("UI_SCORE_BEST");
        uiScoreBest->addChild(uiScore);
    }
    
    auto uiScoreNow = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
    uiScoreNow->setScale9Enabled(true);
    uiScoreNow->setCapInsets(Rect(10,10,2,2));
    uiScoreNow->setContentSize(Size(642, 48));
    uiScoreNow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiScoreNow->setPosition(Vec2(size.width / 2, size.height - 160 - 5));
    layout->addChild(uiScoreNow);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_result_2"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiScoreNow->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(165, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiScoreNow->addChild(uiText);
        
        auto uiScore = ui::CText::create("0", GAME_FONT, 32);
        uiScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiScore->setPosition(Vec2(uiScoreNow->getContentSize().width - 20, uiScoreNow->getContentSize().height / 2));
        uiScore->setTextColor(Color4B(0, 255, 0, 255));
        uiScore->setName("UI_SCORE_NOW");
        uiScoreNow->addChild(uiScore);
    }
    
    auto uiRewardBG = ui::ImageView::create("Assets/ui/flashman/bsbs_box_01.png");
    uiRewardBG->setScale9Enabled(true);
    uiRewardBG->setCapInsets(Rect(16, 16, 2, 2));
    uiRewardBG->setContentSize(Size(642, 250));
    uiRewardBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiRewardBG->setPosition(Vec2(size.width / 2, size.height - 275));
    layout->addChild(uiRewardBG);
    {
        auto uiRewardTitleBG = ui::ImageView::create("Assets/ui/flashman/bsbs_box_text.png");
        uiRewardTitleBG->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height));
        uiRewardBG->addChild(uiRewardTitleBG);
        
        auto uiRewardTitle = ui::CText::create(GAME_TEXT("t_ui_minigame_info_title_2"), GAME_FONT, 28);
        uiRewardTitle->setPosition(Vec2(uiRewardTitleBG->getContentSize().width / 2, uiRewardTitleBG->getContentSize().height / 2));
        uiRewardTitle->setTextAreaSize(Size(264, uiRewardTitle->getContentSize().height));
        uiRewardTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiRewardTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRewardTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiRewardTitleBG->addChild(uiRewardTitle);
        
        auto uiReward = ui::Layout::create();
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 3 + 110));
        uiReward->setName("UI_REWARD");
        uiRewardBG->addChild(uiReward);
    }
}

void PopupFlashman::uiScoreAccReward()
{
    auto layout = _uiContentsUIScoreAccReward;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_bg_2_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x_1.png");
    uiClose->setPosition(Vec2(size.width - 50, size.height - 40));
    uiClose->addClickEventListener([=](Ref* sender){
        _uiContentsUIScoreAccReward->setVisible(false);
    });
    layout->addChild(uiClose);
    
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_minigame_total_title"), GAME_FONT, 40);
    uiTitle->setPosition(Vec2(size.width / 2, size.height - 45));
    uiTitle->setTextAreaSize(Size(520, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(255, 240, 0, 255));
    uiTitle->enableBold();
    layout->addChild(uiTitle);
    
    auto uiScoreBG = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
    uiScoreBG->setScale9Enabled(true);
    uiScoreBG->setCapInsets(Rect(10,10,2,2));
    uiScoreBG->setContentSize(Size(642, 48));
    uiScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiScoreBG->setPosition(Vec2(size.width / 2, size.height - 160 + 5));
    layout->addChild(uiScoreBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_total_msg_2"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiScoreBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(200, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiScoreBG->addChild(uiText);
        
        auto uiScore = ui::CText::create("0", GAME_FONT, 32);
        uiScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiScore->setPosition(Vec2(uiScoreBG->getContentSize().width - 20, uiScoreBG->getContentSize().height / 2));
        uiScore->setTextAreaSize(Size(400, uiText->getContentSize().height));
        uiScore->setTextOverflow(Label::Overflow::SHRINK);
        uiScore->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiScore->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiScore->setTextColor(Color4B(0, 255, 0, 255));
        uiScore->setName("UI_SCORE_TOTAL");
        uiScoreBG->addChild(uiScore);
    }
    
    auto uiEffectBG = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
    uiEffectBG->setScale9Enabled(true);
    uiEffectBG->setCapInsets(Rect(10,10,2,2));
    uiEffectBG->setContentSize(Size(642, 48));
    uiEffectBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiEffectBG->setPosition(Vec2(size.width / 2, size.height - 160 - 5));
    layout->addChild(uiEffectBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_total_msg_3"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiEffectBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(200, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiEffectBG->addChild(uiText);
        
        auto uiEffect = ui::CText::create("0", GAME_FONT, 32);
        uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiEffect->setPosition(Vec2(uiEffectBG->getContentSize().width - 20, uiEffectBG->getContentSize().height / 2));
        uiEffect->setTextAreaSize(Size(400, uiText->getContentSize().height));
        uiEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffect->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiEffect->setTextColor(Color4B(0, 255, 0, 255));
        uiEffect->setName("UI_EFFECT");
        uiEffectBG->addChild(uiEffect);
    }
    
    auto uiRewardBG = ui::ImageView::create("Assets/ui/flashman/bsbs_box_01.png");
    uiRewardBG->setScale9Enabled(true);
    uiRewardBG->setCapInsets(Rect(16, 16, 2, 2));
    uiRewardBG->setContentSize(Size(642, 151));
    uiRewardBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiRewardBG->setPosition(Vec2(size.width / 2, size.height - 275));
    layout->addChild(uiRewardBG);
    {
        auto uiRewardTitleBG = ui::ImageView::create("Assets/ui/flashman/bsbs_box_text.png");
        uiRewardTitleBG->setScale9Enabled(true);
        uiRewardTitleBG->setContentSize(Size(508, uiRewardTitleBG->getContentSize().height));
        uiRewardTitleBG->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height));
        uiRewardBG->addChild(uiRewardTitleBG);
        
        auto uiRewardTitle = ui::CText::create(GAME_TEXT("t_ui_minigame_total_msg_5"), GAME_FONT, 28);
        uiRewardTitle->setPosition(Vec2(uiRewardTitleBG->getContentSize().width / 2, uiRewardTitleBG->getContentSize().height / 2));
        uiRewardTitle->setTextAreaSize(Size(445, uiRewardTitle->getContentSize().height));
        uiRewardTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiRewardTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRewardTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiRewardTitleBG->addChild(uiRewardTitle);
        
        auto uiReward = ui::Layout::create();
        uiReward->setContentSize(Size(610, 90));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 3 + 60.5));
        uiReward->setName("UI_REWARD");
        uiRewardBG->addChild(uiReward);
    }
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_minigame_total_msg_1"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, 57));
    uiDesc->setTextAreaSize(Size(580, uiTitle->getContentSize().height));
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiDesc->setTextColor(Color4B(128, 128, 128, 255));
    layout->addChild(uiDesc);
    
}

void PopupFlashman::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_btn_base_1_1.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    {
        auto action1 = DelayTime::create(0.35);
        auto action2 = CallFuncN::create([=](Node* sender){
            
            auto layoutBattle = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
            if ( layoutBattle != nullptr )
            {
                if ( layoutBattle->getState() != E_FLASHMAN_STATE::None )
                    return;
            }
                    
            if ( _bAttackBtnPressed == true )
                return;
            
            if ( _attackBtnAnimDelay > 0 )
                return;
            
            int listOrder[] = {1, 2, 3, 1, 2, 3, 1, 3, 2, 3, 2};
            int idx = sender->getTag() + 1;
            if ( sizeof(listOrder) / sizeof(int) <= idx )
                idx = 0;
            
            sender->setTag(idx);
            
            std::string name = "";
            std::string path = "";
            if ( listOrder[idx] == 1 )
            {
                path = "Assets/ui/flashman/bsbs_btn_red_1_%d.png";
                name = "UI_RED";
            }
            else if ( listOrder[idx] == 2 )
            {
                path = "Assets/ui/flashman/bsbs_btn_green_1_%d.png";
                name = "UI_GREEN";
            }
            else if ( listOrder[idx] == 3 )
            {
                path = "Assets/ui/flashman/bsbs_btn_blue_1_%d.png";
                name = "UI_BLUE";
            }
            
            auto uiIcon = (ui::ImageView*)utils::findChild(_uiContentsUIButtons, name);
            if ( uiIcon == nullptr )
            {
                return;
            }
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            animation->addSpriteFrameWithFile(MafUtils::format(path.c_str(), 2));
            animation->addSpriteFrameWithFile(MafUtils::format(path.c_str(), 1));
            
            uiIcon->getVirtualRenderer()->stopAllActions();
            uiIcon->getVirtualRenderer()->runAction(Animate::create(animation));
            ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        });
        uiBG->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
    }
    
    auto uiRed = ui::ImageView::create("Assets/ui/flashman/bsbs_btn_red_1_1.png");
    uiRed->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiRed->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiRed->setScale(4);
    uiRed->setName("UI_RED");
    layout->addChild(uiRed);
    
    auto uiGreen = ui::ImageView::create("Assets/ui/flashman/bsbs_btn_green_1_1.png");
    uiGreen->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiGreen->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiGreen->setScale(4);
    uiGreen->setName("UI_GREEN");
    layout->addChild(uiGreen);
    
    auto uiBlue = ui::ImageView::create("Assets/ui/flashman/bsbs_btn_blue_1_1.png");
    uiBlue->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBlue->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBlue->setScale(4);
    uiBlue->setName("UI_BLUE");
    layout->addChild(uiBlue);
    
    auto uiBtnRed = ui::Layout::create();
    uiBtnRed->setContentSize(Size(156, 156));
    uiBtnRed->setIgnoreAnchorPointForPosition(false);
    uiBtnRed->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBtnRed->setPosition(Vec2(76 + 78, 60 + 78));
    uiBtnRed->setTouchEnabled(true);
    uiBtnRed->addTouchEventListener(CC_CALLBACK_2(PopupFlashman::onTouchAttack, this));
    uiBtnRed->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickAttack, this));
    uiBtnRed->setName("UI_BTN_RED");
    layout->addChild(uiBtnRed);
    
    auto uiBtnGreen = ui::Layout::create();
    uiBtnGreen->setContentSize(Size(156, 156));
    uiBtnGreen->setIgnoreAnchorPointForPosition(false);
    uiBtnGreen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBtnGreen->setPosition(Vec2(288 + 78, 204 + 78));
    uiBtnGreen->setTouchEnabled(true);
    uiBtnGreen->addTouchEventListener(CC_CALLBACK_2(PopupFlashman::onTouchAttack, this));
    uiBtnGreen->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickAttack, this));
    uiBtnGreen->setName("UI_BTN_GREEN");
    layout->addChild(uiBtnGreen);
    
    auto uiBtnBlue = ui::Layout::create();
    uiBtnBlue->setContentSize(Size(156, 156));
    uiBtnBlue->setIgnoreAnchorPointForPosition(false);
    uiBtnBlue->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBtnBlue->setPosition(Vec2(500 + 78, 60 + 78));
    uiBtnBlue->setTouchEnabled(true);
    uiBtnBlue->addTouchEventListener(CC_CALLBACK_2(PopupFlashman::onTouchAttack, this));
    uiBtnBlue->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickAttack, this));
    uiBtnBlue->setName("UI_BTN_BLUE");
    layout->addChild(uiBtnBlue);
    
    _uiFeverBG = ui::ImageView::create("Assets/ui/flashman/bsbs_fever_gauge1_1.png");
    _uiFeverBG->setPosition(Vec2(size.width / 2, 145));
    layout->addChild(_uiFeverBG);
    {
        auto uiFeverBar = ui::LoadingBar::create("Assets/ui/flashman/bsbs_fever_gauge1_2.png");
        uiFeverBar->setPercent(0);
        uiFeverBar->setPosition(Vec2(_uiFeverBG->getContentSize().width / 2, _uiFeverBG->getContentSize().height / 2));
        uiFeverBar->setName("UI_BAR");
        _uiFeverBG->addChild(uiFeverBar);
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 8; i++ )
        {
            animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/flashman/bsbs_fever_gauge2_%d.png", i));
        }
        auto uiFeverEffect = ui::ImageView::create("Assets/ui/flashman/bsbs_fever_gauge2_1.png");
        uiFeverEffect->setPosition(Vec2(_uiFeverBG->getContentSize().width / 2, _uiFeverBG->getContentSize().height / 2));
        uiFeverEffect->getVirtualRenderer()->stopAllActions();
        uiFeverEffect->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiFeverEffect->setName("UI_EFFECT");
        uiFeverEffect->setVisible(false);
        _uiFeverBG->addChild(uiFeverEffect);
    }
}

#pragma mark - draw
void PopupFlashman::drawDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    // 출력 유무
    auto layoutBattle = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
    if ( layoutBattle != nullptr )
    {
        if ( layoutBattle->getState() == E_FLASHMAN_STATE::None )
        {
            layout->setVisible(true);
        }
        else
        {
            layout->setVisible(false);
        }
    }
    
    // 스코어
    auto uiScore = layout->getChildByName<ui::CText*>("UI_SCORE");
    uiScore->setString(std::to_string(FlashmanManager::getInstance()->getScoreTotal()));
    
    // 입장 버튼
    auto uiBtnEnter = layout->getChildByName<ui::Button*>("UI_ENTER");
    if ( uiBtnEnter != nullptr )
    {
        int enterNow = FlashmanManager::getInstance()->getEnterCount();
        int enterMax = FlashmanManager::getInstance()->getEnterCountMax();
        
        auto uiText = uiBtnEnter->getChildByName<ui::CText*>("UI_TEXT");
        uiText->setString(MafUtils::format("(%d/%d)", enterNow, enterMax));
    }
    
    // 입장 스킵 버튼
    auto uiBtnEnterSkip = layout->getChildByName<ui::Button*>("UI_ENTER_SKIP");
    if ( uiBtnEnterSkip != nullptr )
    {
        int cost = FlashmanManager::getInstance()->getInfoSetting("clear_cost")->getValueToInt();
        
        int scoreToday = FlashmanManager::getInstance()->getScoreToday();
        std::string gradeText = FlashmanManager::getInstance()->getScoreToGradeText(scoreToday);
        std::transform(gradeText.cbegin(), gradeText.cend(), gradeText.begin(), ::tolower);
        
        //
        uiBtnEnterSkip->removeChildByName("UI_COST");
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_coin_2.png", std::to_string(cost), uiBtnEnterSkip->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiBtnEnterSkip->getContentSize().width / 2, 3 + 22.5));
        uiCost->setName("UI_COST");
        uiBtnEnterSkip->addChild(uiCost);
        
        //
        std::string pathIcon = MafUtils::format("Assets/icon/icon_grade_%s.png", gradeText.c_str());
        auto uiIcon = uiBtnEnterSkip->getChildByName<ui::ImageView*>("UI_ICON");
        uiIcon->loadTexture(pathIcon);
        uiIcon->setVisible(false);
        if ( scoreToday > 0 )
        {
            uiIcon->setVisible(true);
        }
    }
    
    //
    auto uiMission = layout->getChildByName<ui::Button*>("UI_MISSION");
    if ( uiMission != nullptr )
    {
        auto uiTextBox = (ui::Button*)utils::findChild(uiMission, "UI_MISSION_BG");
        auto uiText = (ui::CText*)utils::findChild(uiMission, "UI_MISSION_TEXT");
        
        bool bTicket = FlashmanManager::getInstance()->isTicket();
        if ( bTicket == true )
        {
            uiMission->loadTextureNormal("Assets/ui/bg_icon/icon_mission2_1.png");
            uiTextBox->loadTextureNormal("Assets/ui_common/base_text_2_5.png");
            uiText->setString(GAME_TEXT("t_ui_minigame_tickt_noti_on"));
            uiText->setTextColor(Color4B(255, 228, 92, 255));
        }
        else
        {
            uiMission->loadTextureNormal("Assets/ui/bg_icon/icon_mission.png");
            uiTextBox->loadTextureNormal("Assets/ui_common/base_text_2_4.png");
            uiText->setString(GAME_TEXT("t_ui_minigame_tickt_noti_off"));
            uiText->setTextColor(Color4B(204, 204, 204, 255));
        }
    }
}

void PopupFlashman::drawResult()
{
    auto layout = _uiContentsUIResult;
    layout->setVisible(true);
    
    auto uiGrade = (ui::CText*)utils::findChild(layout, "UI_GRADE");
    auto uiPoint = (ui::CText*)utils::findChild(layout, "UI_POINT");
    
    if (  uiGrade != nullptr && uiPoint != nullptr )
    {
        std::string gradeText = FlashmanManager::getInstance()->getScoreToGradeText(_resultScoreNow);
        uiGrade->setString(gradeText);
        
        if ( FlashmanManager::getInstance()->getInfoSetting("ranking_scroe_mark_switch")->getValueToBool() == true )
        {
            int point = FlashmanManager::getInstance()->getScoreToPoint(_resultScoreNow);
            uiPoint->setPosition(Vec2(uiGrade->getPositionX() + uiGrade->getContentSize().width / 2 + 20, uiGrade->getPositionY() - uiGrade->getContentSize().height / 2 + 20));
            uiPoint->setString(MafUtils::format("(+%d)", point));
            uiPoint->setVisible(true);
        }
        else
        {
            uiPoint->setVisible(false);
        }
    }
    
    if ( auto uiScore = (ui::CText*)utils::findChild(layout, "UI_SCORE_BEST"); uiScore != nullptr )
    {
        uiScore->setString(std::to_string(_resultScoreBest));
    }
    
    if ( auto uiScore = (ui::CText*)utils::findChild(layout, "UI_SCORE_NOW"); uiScore != nullptr )
    {
        uiScore->setString(std::to_string(_resultScoreNow));
    }
    
    if ( auto uiIconNew = (ui::ImageView*)utils::findChild(layout, "UI_ICON_NEW"); uiIconNew != nullptr )
    {
        uiIconNew->setVisible(_resultScoreNew);
    }
    
    
    if ( auto uiReward = (ui::Layout*)utils::findChild(layout, "UI_REWARD"); uiReward != nullptr )
    {
        //
        uiReward->removeAllChildren();
        
        //
        auto listReward = ItemsManager::getInstance()->getConvertArray(_resultReward);
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(72, 72));
            uiReward->addChild(uiItemBG);
            {
                auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                uiImgItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiImgItemIcon);
                
                std::string rewardCount = objItems->getCount();
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiTextReward);
            }
            
            int count = (int)uiReward->getChildrenCount();
            if ( count >= 6 )
            {
                count =  count / 2 + count % 2;
            }
            double width = (72 * count) + (10 * (count - 1));
            double height = 0;
            UtilsWidget::getInstance()->sortWidgetGrid(uiReward, 0, width, height, 10, false, false);
        }
    }
    
}

void PopupFlashman::drawScoreAccReward()
{
    auto layout = _uiContentsUIScoreAccReward;
    layout->setVisible(true);
    
    double effect = FlashmanManager::getInstance()->getScoreEffect();
    
    //
    int64_t scoreTotal = FlashmanManager::getInstance()->getScoreAccTotal();
    int64_t scoreTotalMax = scoreTotal;
    
    std::string scoreText = "";
    
    //
    int bonusIdx = FlashmanManager::getInstance()->getScoreAccIdx() + 1;
    InfoItems* objBonusItem = nullptr;
    
    auto objBonus = FlashmanManager::getInstance()->getScoreBonusReward(bonusIdx);
    if ( objBonus != nullptr )
    {
        scoreTotalMax = objBonus->getCountMax();
        scoreText = MafUtils::format("%lld / %lld", scoreTotal, scoreTotalMax);
        
        auto listReward = objBonus->getListReward();
        if ( listReward.size() > 0 )
        {
            objBonusItem = listReward.at(0);
        }
    }
    else
    {
        scoreText = "MAX";
    }
    
    double percent = (double)scoreTotal / scoreTotalMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    //
    if ( auto uiScore = (ui::CText*)utils::findChild(layout, "UI_SCORE_TOTAL"); uiScore != nullptr )
    {
        uiScore->setString(std::to_string(scoreTotal));
    }
    
    if ( auto uiEffect = (ui::CText*)utils::findChild(layout, "UI_EFFECT"); uiEffect != nullptr )
    {
        uiEffect->setString(GAME_TEXTFORMAT("t_ui_minigame_total_msg_4", effect));
    }
    
    if ( auto uiReward = (ui::Layout*)utils::findChild(layout, "UI_REWARD"); uiReward != nullptr )
    {
        uiReward->removeAllChildren();
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/flashman/bsbs_progressbar_1_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiBarBG->setPosition(Vec2(0, uiReward->getContentSize().height / 2));
        uiReward->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/flashman/bsbs_progressbar_1_2.png");
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBar->setPercent(percent);
        uiBarBG->addChild(uiBar);
        
        auto uiBarText = ui::CText::create(scoreText, GAME_FONT, 20);
        uiBarText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiBarText->setPosition(Vec2(20, uiBarBG->getContentSize().height / 2));
        uiBarBG->addChild(uiBarText);
        
        // btn btn_acheive_1.png, btn_acheive_2.png
        auto uiBtn = ui::Button::create("Assets/ui/mission/btn_acheive_2.png");
        uiBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBtn->setPosition(Vec2(uiReward->getContentSize().width, uiReward->getContentSize().height / 2));
        uiBtn->addClickEventListener(CC_CALLBACK_1(PopupFlashman::onClickScoreAccReward, this));
        uiReward->addChild(uiBtn);
        if ( percent >= 100 )
        {
            uiBtn->loadTextureNormal("Assets/ui/mission/btn_acheive_1.png");
        }
        
        // item
        auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiItemBG->setScale9Enabled(true);
        uiItemBG->setContentSize(Size(87, 87));
        uiItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiItemBG->setPosition(Vec2(uiBtn->getPositionX() - uiBtn->getContentSize().width - 5, uiReward->getContentSize().height / 2));
        uiReward->addChild(uiItemBG);
        if ( objBonusItem != nullptr )
        {
            auto uiImgItemIcon = ui::ImageView::create(objBonusItem->getPath());
            uiImgItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 10));
            uiItemBG->addChild(uiImgItemIcon);

            std::string rewardCount = objBonusItem->getCount();
            if ( objBonusItem->isString() == true )
            {
                MafUtils::convertNumberToShort(rewardCount);
            }

            auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
            uiTextReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
            uiTextReward->enableOutline(Color4B::BLACK, 2);
            uiItemBG->addChild(uiTextReward);
        }
    }
}

void PopupFlashman::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
    auto uiFeverBar = _uiFeverBG->getChildByName<ui::LoadingBar*>("UI_BAR");
    uiFeverBar->setPercent(0);
    
    auto uiFeverEffect = _uiFeverBG->getChildByName<ui::ImageView*>("UI_EFFECT");
    uiFeverEffect->setVisible(false);
}

 
#pragma mark - set, get

#pragma mark - callback
void PopupFlashman::callbackInfo(bool bResult, int nResult)
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
    drawButtons();
}

void PopupFlashman::callbackResult(bool bResult, int nResult, int score, bool bBest, std::string reward)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        else if ( nResult == 10003 )
        {
            strMessage = GAME_TEXT("t_ui_prison_error_2");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    _resultScoreBest = FlashmanManager::getInstance()->getScoreBest();;
    _resultScoreNow = score;
    _resultScoreNew = bBest;
    _resultReward = reward;
    
    //
    drawDefault();
    drawResult();
    drawButtons();
    
    //
    auto layout = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
    if ( layout != nullptr )
    {
        layout->onBattleWait();
    }
    
    FlashmanManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFlashman::callbackInfo, this));
}

void PopupFlashman::callbackRankReward(int rank, std::string reward)
{
    if ( reward.length() != 0 )
    {
        auto popup = PopupFlashmanRankReward::create();
        popup->setRank(rank);
        popup->setRewrad(reward);
        popup->show();
    }
}

void PopupFlashman::callbackAccReward(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
       
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawScoreAccReward();
}

#pragma mark - schedule
void PopupFlashman::scheduleBattle(float dt)
{
    auto uiFeverBar = _uiFeverBG->getChildByName<ui::LoadingBar*>("UI_BAR");
    uiFeverBar->setPercent(0);
    
    auto uiFeverEffect = _uiFeverBG->getChildByName<ui::ImageView*>("UI_EFFECT");
    uiFeverEffect->setVisible(false);
    
    //
    auto layout = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
    if ( layout == nullptr )
    {
        return;
    }
    
    if ( (layout->getState() == E_FLASHMAN_STATE::Idle || layout->getState() == E_FLASHMAN_STATE::Battle) == false )
    {
        this->unschedule(schedule_selector(PopupFlashman::scheduleBattle));
        return;
    }
    
    int feverTimer = layout->getFeverTimer();
    
    int feverKill = layout->getFeverKill();
    int feverKillMax = FlashmanManager::getInstance()->getFeverCondition();
    
    double percent = (double)feverKill / feverKillMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    if ( feverTimer > 0 )
    {
        uiFeverEffect->setVisible(true);
    }
    else
    {
        uiFeverBar->setPercent(percent);
    }
}

#pragma mark - click
void PopupFlashman::onClickClose(Ref* sender)
{
    hide();
}

void PopupFlashman::onClickHelp(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    int page = FlashmanManager::getInstance()->getInfoSetting("minigame_info")->getValueToInt();

    auto popup = PopupHelp::create(page, "t_ui_minigame_info_title_%d", "t_ui_minigame_info_msg_%d");
    popup->show();
}

void PopupFlashman::onClickMission(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto popup = PopupFlashmanMission::create();
    popup->show();
}

void PopupFlashman::onClickRank(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    
    auto popup = PopupPageRanking::create(E_RANK_LOCAL_TYPE::FLASHMAN);
    popup->show();
}

void PopupFlashman::onClickScoreAcc(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    // 새로고침
    drawScoreAccReward();
    
    // 출력
    _uiContentsUIScoreAccReward->setVisible(true);
}

void PopupFlashman::onClickScoreAccReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    //
    int bonusIdx = FlashmanManager::getInstance()->getScoreAccIdx() + 1;
    
    auto objBonus = FlashmanManager::getInstance()->getScoreBonusReward(bonusIdx);
    if ( objBonus == nullptr )
    {
        return;
    }
    
    int64_t scoreTotal = FlashmanManager::getInstance()->getScoreAccTotal();
    int64_t scoreTotalMax = objBonus->getCountMax();
    if ( scoreTotal < scoreTotalMax )
    {
        return;
    }
     
    FlashmanManager::getInstance()->requestAccReward(true, CC_CALLBACK_2(PopupFlashman::callbackAccReward, this));
}

void PopupFlashman::onClickDexLoots(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto popup = PopupBonsikDexDefault::create(PopupBonsikDexDefault::E_TYPE::LOOTS);
    popup->show();
}

void PopupFlashman::onClickEnter(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    int enterNow = FlashmanManager::getInstance()->getEnterCount();
    if ( enterNow <= 0 )
    {
        auto popup = PopupFlashmanEnterBuy::create();
        popup->show();
        return;
    }
    
    //
    _listAttackBtnPressedTime.clear();
    
    //
    onBattle();
}

void PopupFlashman::onClickEnterSkip(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    int enterNow = FlashmanManager::getInstance()->getEnterCount();
    if ( enterNow <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_minigame_clear_msg2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int scoreToday = FlashmanManager::getInstance()->getScoreToday();
    auto time = FlashmanManager::getInstance()->getTodayScoreReset() - UtilsDate::getInstance()->getTime();
    
    if ( scoreToday <= 0 || time <= 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_minigame_clear_msg1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(time <= 0)
            FlashmanManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFlashman::callbackInfo, this));
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_minigame_clear_bt"), GAME_TEXT("t_ui_minigame_clear_msg"));
    auto uiPopupOk = popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender) {
        
        FlashmanManager::getInstance()->requestSkipResult(true, scoreToday);
    });
    popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
    popup->show();
    {
        uiPopupOk->removeAllChildren();
        
        int cost = FlashmanManager::getInstance()->getInfoSetting("clear_cost")->getValueToInt();
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_coin_2.png", std::to_string(cost), uiPopupOk->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiPopupOk->getContentSize().width / 2, uiPopupOk->getContentSize().height / 2));
        uiCost->setName("UI_COST");
        uiPopupOk->addChild(uiCost);
    }
}

void PopupFlashman::onTouchAttack(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    auto uiBtn = dynamic_cast<ui::Layout*>(sender);
    
    std::string name = uiBtn->getName();
    std::string path = "";
    if ( name.compare("UI_BTN_RED") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_red_1_%d.png";
        name = "UI_RED";
    }
    else if ( name.compare("UI_BTN_GREEN") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_green_1_%d.png";
        name = "UI_GREEN";
    }
    else if ( name.compare("UI_BTN_BLUE") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_blue_1_%d.png";
        name = "UI_BLUE";
    }
    
    auto uiIcon = (ui::ImageView*)utils::findChild(_uiContentsUIButtons, name);
    if ( uiIcon == nullptr )
    {
        return;
    }
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            _bAttackBtnPressed = true;
            
            uiIcon->loadTexture((MafUtils::format(path.c_str(), 2)));
            break;
        }
        case ui::Widget::TouchEventType::MOVED:
        {
            
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        {
            _bAttackBtnPressed = false;
            _attackBtnAnimDelay = 1.0;
            break;
        }
        case ui::Widget::TouchEventType::CANCELED:
        {
            _bAttackBtnPressed = false;
            _attackBtnAnimDelay = 1.0;
            uiIcon->loadTexture((MafUtils::format(path.c_str(), 1)));
            break;
        }
        default:
            break;
    }
}

void PopupFlashman::onClickAttack(cocos2d::Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Layout*>(sender);
    
    E_FLASHMAN_TYPE eTypeColor = E_FLASHMAN_TYPE::Red;
    
    std::string name = uiBtn->getName();
    std::string path = "";
    if ( name.compare("UI_BTN_RED") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_red_1_%d.png";
        name = "UI_RED";
        
        eTypeColor = E_FLASHMAN_TYPE::Red;
    }
    else if ( name.compare("UI_BTN_GREEN") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_green_1_%d.png";
        name = "UI_GREEN";
        
        eTypeColor = E_FLASHMAN_TYPE::Green;
    }
    else if ( name.compare("UI_BTN_BLUE") == 0 )
    {
        path = "Assets/ui/flashman/bsbs_btn_blue_1_%d.png";
        name = "UI_BLUE";
        
        eTypeColor = E_FLASHMAN_TYPE::Blue;
    }
    
    auto uiIcon = (ui::ImageView*)utils::findChild(_uiContentsUIButtons, name);
    if ( uiIcon == nullptr )
    {
        return;
    }
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    animation->addSpriteFrameWithFile(MafUtils::format(path.c_str(), 3));
    animation->addSpriteFrameWithFile(MafUtils::format(path.c_str(), 1));
    
    uiIcon->getVirtualRenderer()->stopAllActions();
    uiIcon->getVirtualRenderer()->runAction(Animate::create(animation));
    ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    
    //
    auto layout = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
    if ( layout == nullptr )
    {
        return;
    }
    
    if ( layout->getState() != E_FLASHMAN_STATE::Battle )
    {
        return;
    }
    
    
    // check 1
    {
        
        int64_t timeNow = utils::getTimeInMilliseconds();;

        int clickNow = 0;
        int clickMax = FlashmanManager::getInstance()->getInfoSetting("click_limit_s")->getValueToInt();

        for (auto iter = _listAttackBtnPressedTime.rbegin(); iter != _listAttackBtnPressedTime.rend(); ++iter )
        {
            int64_t now = *iter;
            if ( now >= timeNow - 1000 )
            {
                clickNow++;
                if ( clickNow > clickMax )
                {
                    break;
                }
            }
            else
            {
                _listAttackBtnPressedTime.erase( std::next(iter).base() );
            }
        }
        
        if ( clickNow > clickMax )
        {
            return;
        }
    }
    
    // check 2
    {
        int clickNow = 0;
        int clickMax = FlashmanManager::getInstance()->getInfoSetting("click_point_limit")->getValueToInt();
        for ( Vec2& pos : _listAttackBtnPressedPos )
        {
            if ( pos.equals(uiBtn->getTouchEndPosition()) )
            {
                clickNow++;
            }
        }
        
        if ( clickNow > clickMax )
        {
            return;
        }
        
        if ( _listAttackBtnPressedPos.size() > 0 )
        {
            auto pos = _listAttackBtnPressedPos.at(0);
            if ( pos.equals(uiBtn->getTouchEndPosition()) == false )
            {
                _listAttackBtnPressedPos.clear();
            }
        }
    }
    
    
    _listAttackBtnPressedTime.push_back(utils::getTimeInMilliseconds());
    _listAttackBtnPressedPos.push_back(uiBtn->getTouchEndPosition());
    
    
    SoundManager::SoundEffectStart(SOUND_SELECT_3, E_SOUND_TYPE::CONTENTS);
    layout->onBattleAttack(eTypeColor);
}

#pragma mark - event
void PopupFlashman::onBattle()
{
    //
    if ( FlashmanManager::getInstance()->getTimeReset() < UtilsDate::getInstance()->getTime() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            FlashmanManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFlashman::callbackInfo, this));
        });
        popup->show();
        return;
    }
    
    //
    auto layout = dynamic_cast<LayerFlashmanBattle*>(_uiContentsBattle);
    if ( layout == nullptr )
    {
        return;
    }
    
    if ( layout->getState() != E_FLASHMAN_STATE::None )
    {
        return;
    }
    
    //
    layout->onBattleStart();
    
    //
    this->schedule(schedule_selector(PopupFlashman::scheduleBattle));
    
    //
    drawDefault();
    drawButtons();
}
