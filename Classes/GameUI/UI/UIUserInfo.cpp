//
//  UIUserInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UIUserInfo.h"

#include "GameUIPopup/Other/Stat/PopupStat.h"
#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/Ranking/PopupRanking.h"

#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/RankingManager.h"

USING_NS_CC;

UIUserInfo* UIUserInfo::create(E_PLACE eType)
{
    UIUserInfo* pRet = new(std::nothrow) UIUserInfo();
    if ( pRet && pRet->init(eType) )
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

UIUserInfo::UIUserInfo(void) :
_eType(E_PLACE::EMPTY)
{
    
}

UIUserInfo::~UIUserInfo(void)
{
    
}

bool UIUserInfo::init(E_PLACE eType)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    setContentSize(Size(240, 62));
    
    //
    _eType = eType;
    
    // init
    initVar();
    initUi();
     
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME_BUY, this);
    
    return true;
}

#pragma mark - override : refresh
void UIUserInfo::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME || eType == E_REFRESH::COSTUME_BUY )
    {
        drawContainer();
    }
}
 

#pragma mark - init
void UIUserInfo::initVar()
{
    RankingManager::getInstance()->setCallbackReDraw(CC_CALLBACK_1(UIUserInfo::callbackRank, this));
}

void UIUserInfo::initUi()
{
    Size size = getContentSize();
    
    _uiContainer = ui::Layout::create();
    _uiContainer->setContentSize(size);
    _uiContainer->setAnchorPoint(Vec2::ZERO);
    _uiContainer->setPosition(Vec2::ZERO);
    addChild(_uiContainer);
   
    //
    uiContainer();
}

// ui
void UIUserInfo::uiContainer()
{
    _uiContainer->removeAllChildren();
    
    auto layout = _uiContainer;
    
    auto uiBtnBG = ui::Button::create("Assets/ui/main/main_hero_bg.png");
    uiBtnBG->setScale9Enabled(true);
    uiBtnBG->setContentSize(layout->getContentSize());
    uiBtnBG->setAnchorPoint(Vec2::ZERO);
    uiBtnBG->setPosition(Vec2::ZERO);
    uiBtnBG->addClickEventListener(CC_CALLBACK_1(UIUserInfo::onClickStat, this));
    layout->addChild(uiBtnBG);
    
    _uiImgSkinBG = ui::ImageView::create("Assets/ui/main/main_hero_edge.png");
    _uiImgSkinBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _uiImgSkinBG->setPosition(Vec2(5, layout->getContentSize().height * 0.5));
    layout->addChild(_uiImgSkinBG);

    auto uiTextNick = ui::CText::create(AccountManager::getInstance()->getUserNick(), GAME_FONT, 20);
    uiTextNick->setAnchorPoint(Vec2::ZERO);
    uiTextNick->setPosition(Vec2(66, layout->getContentSize().height * 0.5));
    uiTextNick->setColor(Color3B(213, 192, 125));
    uiTextNick->setTextAreaSize(Size(layout->getContentSize().width, uiTextNick->getContentSize().height));
    uiTextNick->setTextOverflow(Label::Overflow::SHRINK);
    uiTextNick->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextNick->setTextVerticalAlignment(TextVAlignment::CENTER);
    layout->addChild(uiTextNick);
    {
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            // nick
            auto uiTextNick = (ui::CText*)sender;
            uiTextNick->setString(AccountManager::getInstance()->getUserNick());
        });
        auto seq = Sequence::create(action1, action2, NULL);
        uiTextNick->runAction(RepeatForever::create(seq));
    }

    auto spriteTrophy = Sprite::create("Assets/ui/ranking/icon_ranking.png");
    spriteTrophy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spriteTrophy->setPosition(66, layout->getContentSize().height * 0.3);
    spriteTrophy->setScale(0.6);
    layout->addChild(spriteTrophy);

    _uiTextRank = ui::CText::create(GAME_TEXT("t_ui_rank_title"), GAME_FONT, 24);
    _uiTextRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _uiTextRank->setPosition(Vec2(90, layout->getContentSize().height * 0.3));
    _uiTextRank->setColor(Color3B(92, 230, 255));
    _uiTextRank->enableOutline(Color4B::BLACK, 1);
    layout->addChild(_uiTextRank);
    
    auto uiBtnRank = ui::Layout::create();
    uiBtnRank->setContentSize(Size(170, 35));
    uiBtnRank->setAnchorPoint(Vec2::ZERO);
    uiBtnRank->setPosition(Vec2(66, 0));
    uiBtnRank->setTouchEnabled(true);
    uiBtnRank->addClickEventListener(CC_CALLBACK_1(UIUserInfo::onClickRank, this));
    layout->addChild(uiBtnRank);
    
    // draw
    drawContainer();
}

#pragma mark - ui
void UIUserInfo::drawContainer()
{
    // skin
    _uiImgSkinBG->removeAllChildren();
    
    int nHead = CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD);
    int nSkin = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    if ( nSkin != 0 )
    {
        auto spriteSkin = Sprite::create(MafUtils::format("Assets/icon_hero/skin_%d.png", nSkin));
        if ( spriteSkin != nullptr )
        {
            spriteSkin->setPosition(_uiImgSkinBG->getContentSize().width / 2, _uiImgSkinBG->getContentSize().height / 2);
            spriteSkin->setScale(_uiImgSkinBG->getContentSize().width / spriteSkin->getContentSize().width);
            _uiImgSkinBG->addChild(spriteSkin);
        }
    }
    
    if ( _uiImgSkinBG->getChildrenCount() == 0 )
    {
        auto spriteSkin = Sprite::create(MafUtils::format("Assets/icon_costume/head_%02d.png", nHead));
        spriteSkin->setPosition(_uiImgSkinBG->getContentSize().width / 2, _uiImgSkinBG->getContentSize().height / 2);
        spriteSkin->setScale(1.2);
        _uiImgSkinBG->addChild(spriteSkin);
    }
    
    // rank
    auto rankInfo = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
    if ( rankInfo != nullptr )
    {
        _uiTextRank->setString(GAME_TEXTFORMAT("t_ui_rank_now", rankInfo->getRank()));
    }
}
 
#pragma mark - set, get

#pragma mark - schedule

#pragma mark - callback
void UIUserInfo::callbackRank(bool Result)
{
    drawContainer();
}

#pragma mark - click
void UIUserInfo::onClickStat(cocos2d::Ref* sender)
{
    auto popup = PopupStat::create(E_PLACE::DG_NORMAL);
    popup->show();
}

void UIUserInfo::onClickRank(cocos2d::Ref* sender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( AccountManager::getInstance()->getUserNick().length() == 0 )
    {
        auto popup = PopupUserNickname::create();
        popup->show();
        return;
    }
    
    int diff = UserInfoConfigManager::getInstance()->getSaveTimeDiff();
    if ( diff < 30 )
    {
        auto popup = PopupRanking::create(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
        popup->show();
    }
    else
    {
        //30일 이상 팝업
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_51"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
     
}

#pragma mark - event
