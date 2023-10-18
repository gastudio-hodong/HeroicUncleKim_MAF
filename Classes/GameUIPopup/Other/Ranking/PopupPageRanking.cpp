//
//  PopupPageRanking.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/08.
//

#include "PopupPageRanking.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameScene/Base/BaseGameScene.h"

#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceRankRewardInfo.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceReset.h"
#include "GameUIPopup/Other/Flashman/PopupFlashmanRankRewardInfo.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/RankingManager.h"

PopupPageRanking* PopupPageRanking::create(E_RANK_LOCAL_TYPE type)
{
    PopupPageRanking* pRet = new(std::nothrow) PopupPageRanking();
    if ( pRet && pRet->init(type) )
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

PopupPageRanking::PopupPageRanking(void):
_menu(nullptr),
_sprBG(nullptr),
_sprTopCell(nullptr),
_lyRankUser(nullptr),
_toggleRankAutoRefresh(nullptr),
_lbNick(nullptr),
_callbackHide(nullptr),
_type(E_RANK_LOCAL_TYPE::NONE)
{
    _vecUserInfo.clear();
    _vecTopRankerInfo.clear();
}

PopupPageRanking::~PopupPageRanking(void)
{
    for(int i = 0; i<_vecUserInfo.size(); ++i)
    {
        delete _vecUserInfo[i];
        
    }
    
    _vecUserInfo.clear();
    
    for(int i = 0; i<_vecTopRankerInfo.size(); ++i)
    {
        delete _vecTopRankerInfo[i];
        
    }
    
    _vecTopRankerInfo.clear();
}

#pragma mark - init
bool PopupPageRanking::init(E_RANK_LOCAL_TYPE type)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupPageRanking");
    
    //
    _bHideBackKey = false;
    _type = type;
    
    //
    initVar();
    initUI();

    return true;
}

void PopupPageRanking::initVar()
{
    double nWidth = 734;
    double nHeight = getContentSize().height;
    if ( nHeight > 1210 )
        nHeight = 1210;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(110);
}

void PopupPageRanking::initUI()
{
    _menu = MafNode::MafMenu::create();
    _menu->setAnchorPoint(Vec2::ZERO);
    _menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(_menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    _menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_rank_title"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    initTopUI();
    initBottomUI();
}

void PopupPageRanking::initTopUI()
{
    std::string strBgPath = "";
    
    switch (_type) {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
            strBgPath = "Assets/ui/ranking/ranking_user_bg1.png";
            break;
        
        default:
            strBgPath = "Assets/ui/ranking/ranking_user_bg2.png";
            break;
    }
    
    _sprBG = Sprite::create(strBgPath);
    _sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sprBG->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_sprBG);
    
    //랭커 단상 UI
    auto itemTopRank1st = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_1.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemTopRank1st->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank1st->setPosition(_sprBG->getPosition() + Vec2::DOWN * (_sprBG->getContentSize().height/2 - 50));
    itemTopRank1st->setName("TOP_USER");
    itemTopRank1st->setTag(0);
    _menu->addChild(itemTopRank1st);
    _vecItemTopRank.push_back(itemTopRank1st);
    
    auto itemTopRank2nd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_2.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemTopRank2nd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank2nd->setPosition(itemTopRank1st->getPosition() + Vec2::LEFT * itemTopRank1st->getContentSize().width + Vec2::DOWN * 15);
    itemTopRank2nd->setName("TOP_USER");
    itemTopRank2nd->setTag(1);
    _menu->addChild(itemTopRank2nd);
    _vecItemTopRank.push_back(itemTopRank2nd);
    
    auto itemTopRank3rd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_3.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemTopRank3rd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank3rd->setPosition(itemTopRank1st->getPosition() + Vec2::RIGHT * itemTopRank1st->getContentSize().width + Vec2::DOWN * 15);
    itemTopRank3rd->setName("TOP_USER");
    itemTopRank3rd->setTag(2);
    _menu->addChild(itemTopRank3rd);
    _vecItemTopRank.push_back(itemTopRank3rd);
    
    _toggleRankAutoRefresh = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/mission/mission_on.png"), nullptr, [&](Ref* sender){
        bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
        isRefreshRank = !isRefreshRank;
        
        if(isRefreshRank) _toggleRankAutoRefresh->setNormalImage(Sprite::create("Assets/ui/mission/mission_on.png"));
        else                   _toggleRankAutoRefresh->setNormalImage(Sprite::create("Assets/ui/mission/mission_off.png"));
        
        UserDefault::getInstance()->setBoolForKey(KEY_REFRESH_RANK, isRefreshRank);
    });
    
    _toggleRankAutoRefresh->setPosition(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height + 5);
    _toggleRankAutoRefresh->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _menu->addChild(_toggleRankAutoRefresh);
    bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
    if(isRefreshRank) _toggleRankAutoRefresh->setNormalImage(Sprite::create("Assets/ui/mission/mission_on.png"));
    else                   _toggleRankAutoRefresh->setNormalImage(Sprite::create("Assets/ui/mission/mission_off.png"));
    
    auto toggle_desc = Label::createWithTTF(GAME_TEXT("t_ui_rank_toggle_desc"), GAME_FONT, 24);
    toggle_desc->setColor(Color3B::WHITE);
    toggle_desc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    toggle_desc->setPosition(_toggleRankAutoRefresh->getContentSize() + Size(0,10));
    _toggleRankAutoRefresh->addChild(toggle_desc);
    
    switch (_type) {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            _toggleRankAutoRefresh->setVisible(true);
        }
            break;
        default:
        {
            _toggleRankAutoRefresh->setVisible(false);
        }
            break;
    }
    
    if(_type == E_RANK_LOCAL_TYPE::EVENT_RACE)
    {
        auto btnRankInfo = ui::Button::create("Assets/ui/ranking/icon_ranking_2.png");
        btnRankInfo->addClickEventListener([=](Ref*){
            auto popup = PopupEventRaceRankRewardInfo::create();
            popup->show();
        });
        btnRankInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btnRankInfo->setZoomScale(0.05f);
        btnRankInfo->setPosition(Vec2(_sprBG->getContentSize().width - 15, 115));
        _sprBG->addChild(btnRankInfo);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 18);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbText->setPosition(btnRankInfo->getContentSize().width / 2, 20);
            lbText->enableOutline(Color4B::BLACK, 1);
            btnRankInfo->addChild(lbText);
        }
    }
    else if(_type == E_RANK_LOCAL_TYPE::FLASHMAN)
    {
        auto btnRankInfo = ui::Button::create("Assets/ui/ranking/icon_ranking_2.png");
        btnRankInfo->addClickEventListener([=](Ref*){
            auto popup = PopupFlashmanRankRewardInfo::create();
            popup->show();
        });
        btnRankInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btnRankInfo->setZoomScale(0.05f);
        btnRankInfo->setPosition(Vec2(_sprBG->getContentSize().width - 15, 115));
        _sprBG->addChild(btnRankInfo);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 18);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbText->setPosition(btnRankInfo->getContentSize().width / 2, 20);
            lbText->enableOutline(Color4B::BLACK, 1);
            btnRankInfo->addChild(lbText);
        }
    }
}

void PopupPageRanking::initBottomUI()
{
    _sprTopCell = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    _sprTopCell->setContentSize(Size(_spriteContainer->getContentSize().width - 12, 34));
    _sprTopCell->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sprTopCell->setPosition(_sprBG->getPosition() + Vec2::DOWN * (_sprBG->getContentSize().height - _sprBG->getContentSize().height/5));
    _spriteContainer->addChild(_sprTopCell, E_ZORER::UI);
    {
        auto labelRank = Label::createWithTTF(GAME_TEXT("t_ui_rank_title"), GAME_FONT, 24);
        labelRank->setPosition( Vec2(105, _sprTopCell->getContentSize().height / 2) );
        labelRank->setColor(Color3B(81, 189, 140));
        _sprTopCell->addChild(labelRank);
        
        auto labelNick = Label::createWithTTF(GAME_TEXT("t_ui_rank_nick"), GAME_FONT, 24);
        labelNick->setPosition( Vec2(250, _sprTopCell->getContentSize().height / 2) );
        labelNick->setColor(Color3B(81, 189, 140));
        _sprTopCell->addChild(labelNick);
        
        auto labelScore = Label::createWithTTF(GAME_TEXT("t_ui_rank_score_1"), GAME_FONT, 24);
        labelScore->setPosition( Vec2(610, _sprTopCell->getContentSize().height / 2) );
        labelScore->setColor(Color3B(81, 189, 140));
        _sprTopCell->addChild(labelScore);
    }
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setContentSize(Size(658, 108));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_spriteContainer->getContentSize().width/2, 30);
    _spriteContainer->addChild(sprBox, E_ZORER::UI+1);
    
    std::string strDesc = "";
    
    switch (_type) {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
            strDesc = GAME_TEXT("t_ui_rank_desc_4");
            break;
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
            strDesc = GAME_TEXT("t_ui_rank_desc_5");
            break;
        default:
            break;
    }
    
    auto lbDesc = Label::createWithTTF(strDesc, GAME_FONT, 17);
    lbDesc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbDesc->setPosition(20, 0);
    lbDesc->setDimensions(sprBox->getContentSize().width - 150, sprBox->getContentSize().height - 10);
    lbDesc->setOverflow(Label::Overflow::SHRINK);
    lbDesc->setColor(Color3B(173, 151, 125));
    sprBox->addChild(lbDesc);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    sprBox->addChild(menu, E_ZORER::UI);
    
    auto itemNick = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/raid/icon_name.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemNick->setName("NICK");
    itemNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    Size boxSize = sprBox->getContentSize();
    itemNick->setPosition(boxSize.width - itemNick->getContentSize().width/2 - 15, boxSize.height/2);
    menu->addChild(itemNick);
    
    auto itemResetSprite = ui::Scale9Sprite::create("Assets/ui_common/btn_2_4_on.png", Rect::ZERO, Rect(15,25,2,2));
    itemResetSprite->setContentSize(Size(106, 46));
    
    if ( _type == E_RANK_LOCAL_TYPE::EVENT_RACE )
    {
        auto itemReset = MafNode::MafMenuItemSprite::create(itemResetSprite, nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
        itemReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemReset->setPosition(Vec2(20, 33));
        itemReset->setTouchRect(itemReset->getContentSize() + Size(15, 40));
        itemReset->setName("RESET");
        itemReset->setVisible(false);
        menu->addChild(itemReset);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_reset_bt"), GAME_FONT, 28);
            lbText->setPosition(itemReset->getContentSize().width / 2, itemReset->getContentSize().height / 2 + 4);
            lbText->setColor(Color3B(160, 148, 109));
            itemReset->addChild(lbText);
        }
        if ( EventManager::getInstance()->isActivatedEvent(33) )
        {
            itemReset->setVisible(true);
        }
    }
    
    _lyRankUser = LayerColor::create();
    _lyRankUser->setPosition(Vec2(5,0) + Vec2::UP * (sprBox->getContentSize().height + 20));
    _lyRankUser->setContentSize(Size(_spriteContainer->getContentSize().width - 10, _sprTopCell->getPosition().y - _sprTopCell->getContentSize().height - sprBox->getContentSize().height - 30));
    _spriteContainer->addChild(_lyRankUser);
    
    auto itemPre = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_L02.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemPre->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    itemPre->setPosition(Vec2(-itemPre->getContentSize().width/2, _sprTopCell->getPosition().y/2));
    itemPre->setTouchRect(itemPre->getContentSize() + Size(15, 40));
    itemPre->setName("PREV");
    menu->addChild(itemPre);
    
    auto itemNext = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_R02.png"), nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
    itemNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemNext->setPosition(Vec2(_spriteContainer->getContentSize().width - itemNext->getContentSize().width - 30, _sprTopCell->getPosition().y/2));
    itemNext->setTouchRect(itemNext->getContentSize() + Size(15, 40));
    itemNext->setName("NEXT");
    menu->addChild(itemNext);
    
    auto sprMyrankBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_4_on.png");
    sprMyrankBox->setContentSize(Size(658, 48));
    sprMyrankBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprMyrankBox->setPosition(_spriteContainer->getContentSize().width/2, sprBox->getPosition().y + sprBox->getContentSize().height + 5);
    _spriteContainer->addChild(sprMyrankBox);
    
    auto myRankMenu = MafNode::MafMenu::create();
    myRankMenu->setAnchorPoint(Vec2::ZERO);
    myRankMenu->setPosition(Vec2::ZERO);
    sprMyrankBox->addChild(myRankMenu, E_ZORER::UI);
    
    _lbMyRank = Label::createWithTTF("", GAME_FONT, 23);
    _lbMyRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbMyRank->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbMyRank->setPosition(20, sprMyrankBox->getContentSize().height/2);
    _lbMyRank->setColor(Color3B(255, 199, 0));
    sprMyrankBox->addChild(_lbMyRank);
    
    _lbNick = Label::createWithTTF(AccountManager::getInstance()->getUserNick(), GAME_FONT, 23);
    _lbNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbNick->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _lbNick->setPosition(Vec2(sprMyrankBox->getContentSize()/2) + Vec2::LEFT * 80);
    _lbNick->setColor(Color3B(255, 199, 0));
    sprMyrankBox->addChild(_lbNick);
    
    auto sprMyRank = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprMyRank->setContentSize(Size(212, 60));
    
    auto itemMyRank = MafNode::MafMenuItemSprite::create(sprMyRank, nullptr, CC_CALLBACK_1( PopupPageRanking::onClick, this));
    itemMyRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemMyRank->setPosition(sprMyrankBox->getContentSize().width, sprMyrankBox->getContentSize().height/2);
    itemMyRank->setName("VIEW_MY_RANK");
    myRankMenu->addChild(itemMyRank);
    
    auto lbViewMyRank = Label::createWithTTF(GAME_TEXT("t_ui_rank_view_my_rank"), GAME_FONT, 30);
    lbViewMyRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbViewMyRank->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbViewMyRank->setDimensions(itemMyRank->getContentSize().width, itemMyRank->getContentSize().height);
    lbViewMyRank->setOverflow(Label::Overflow::SHRINK);
    lbViewMyRank->setPosition(itemMyRank->getContentSize()/2);
    itemMyRank->addChild(lbViewMyRank);
}

void PopupPageRanking::show()
{
    PopupBase::show();
    
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            PopupLoading::show();
            RankingManager::getInstance()->requestEventRaceRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            PopupLoading::show();
            RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;;
        }
            
        default:
            break;
    }
}

#pragma mark - afterInit
void PopupPageRanking::afterInit()
{
    afterInitTopUI();
    afterInitBottomUI();
}

void PopupPageRanking::afterInitTopUI()
{
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    
    for(int i = 0; i < _vecTopRankerInfo.size(); ++i)
    {
        auto item = _vecItemTopRank[i];
        auto info = _vecTopRankerInfo[i];
        
        {//hero
            int skinIdx = info->getSkinIdx();
            auto mapCostume = info->getCostume();
            
            auto hero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
            hero->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            hero->setPosition(Vec2(item->getContentSize()/2) + Vec2::UP * 80 + Vec2::RIGHT * 60);
            if(skinIdx == 0 || CostumeSkinManager::getInstance()->getInfoSkin(skinIdx) == nullptr)
            {
                hero->setImage(info->getWeaponIdx(), mapCostume[E_COSTUME::HEAD], mapCostume[E_COSTUME::BODY], mapCostume[E_COSTUME::CAPE], mapCostume[E_COSTUME::HAND], mapCostume[E_COSTUME::FOOT]);
                hero->setMotion(info->getWeaponIdx(), mapCostume[E_COSTUME::HEAD], mapCostume[E_COSTUME::BODY], mapCostume[E_COSTUME::CAPE], mapCostume[E_COSTUME::HAND], mapCostume[E_COSTUME::FOOT]);
            }
            else
            {
                hero->setSkin(skinIdx);
            }
            hero->actionMove();
            hero->cocos2d::Node::setName("hero");
            item->addChild(hero);
        }
        
        {//info
            auto sprScoreBG = Sprite::create("Assets/ui_common/base_text_1_5.png");
            sprScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprScoreBG->setPosition(Vec2(item->getContentSize()/2) + Vec2::DOWN * 55);
            sprScoreBG->setName("bg");
            item->addChild(sprScoreBG);
            
            std::string strScore = MafUtils::doubleToString(info->getScore());
            if(_type == E_RANK_LOCAL_TYPE::EVENT_RACE)
                MafUtils::convertNumberToShort(strScore);
            else
                MafUtils::addCommaToString(strScore, 3, ',');
            
            auto lbScore = Label::createWithTTF(strScore, GAME_FONT, 20);
            lbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbScore->setPosition(sprScoreBG->getContentSize()/2);
            lbScore->setColor(Color3B(255, 199, 0));
            lbScore->setName("score");
            sprScoreBG->addChild(lbScore);
            
            auto lbNick = Label::createWithSystemFont(info->getNick(), SYSTEM_FONT, 21);
            lbNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbNick->setPosition(sprScoreBG->getPosition() + Vec2::DOWN * 40);
            lbNick->setName("nick");
            item->addChild(lbNick);
        }
        
    }

}

void PopupPageRanking::afterInitBottomUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyRankUser->addChild(menu, E_ZORER::UI);
    
    for(int i = 0; i < 11; ++i)
    {
        auto cell =  CellPageRank::create(_type, nullptr, Size(_lyRankUser->getContentSize().width, 48));
        _vecCellRank.push_back(cell);
        auto itemCell = MafNode::MafMenuItemSprite::create(cell, nullptr, CC_CALLBACK_1(PopupPageRanking::onClick, this));
        itemCell->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemCell->setPosition(_lyRankUser->getContentSize().width/2, (_lyRankUser->getContentSize().height - 15) - i * cell->getContentSize().height);
        itemCell->setTouchRect(itemCell->getContentSize() - Size(150, 0));
        itemCell->setName("USER");
        itemCell->setTag(i);
        cell->setItem(itemCell);
        menu->addChild(itemCell);
    }
    
    for(int i = 0; i < _vecCellRank.size(); ++i)
    {
        if(i < _vecUserInfo.size())
        {
            _vecCellRank[i]->setRankInfo(_vecUserInfo[i]);
            _vecCellRank[i]->reDraw();
        }
        else
        {
            _vecCellRank[i]->hide();
        }
    }
    
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    
    if(info != nullptr)
    {
        _lbMyRank->setString(MafUtils::toString(info->getRank()));
    }
}
void PopupPageRanking::setCallbackHide(const std::function<void()>& pCallback)
{
    _callbackHide = pCallback;
}
#pragma mark - callback Init
void PopupPageRanking::callbackUserRankInfoInit(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
{
    if(PopupLoading::isShow())
        PopupLoading::hide();
    if(bResult == false)
        return;
    
    _vecUserInfo = vecUserInfo;
    _vecTopRankerInfo = vecTopRank;
    afterInit();
    
}

void PopupPageRanking::callbackUpdateAfterInit(bool bResult)
{
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    PopupLoading::show();
    
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            RankingManager::getInstance()->requestEventRaceRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;
        }
    }
}

void PopupPageRanking::callbackNicknameInit()
{
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            RankingManager::getInstance()->requestEventRaceRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoInit, this));
            break;
        }
    }
}

#pragma mark - callback Update

void PopupPageRanking::callbackUserRankInfoUpdate(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
{
    if(PopupLoading::isShow())
        PopupLoading::hide();
    if(bResult == false)
        return;
    
    for(int i = 0; i < _vecUserInfo.size(); ++i)
    {
        delete _vecUserInfo[i];
        
    }
    
    _vecUserInfo.clear();
    
    for(int i = 0; i < _vecTopRankerInfo.size(); ++i)
    {
        delete _vecTopRankerInfo[i];
        
    }
    
    _vecTopRankerInfo.clear();
    
    _vecUserInfo = vecUserInfo;
    _vecTopRankerInfo = vecTopRank;
    
    for(int i = 0; i < _vecCellRank.size(); ++i)
    {
        if(i < _vecUserInfo.size())
        {
            _vecCellRank[i]->setRankInfo(_vecUserInfo[i]);
            _vecCellRank[i]->reDraw();
        }
        else
        {
            _vecCellRank[i]->hide();
        }
    }
    
    for(int i = 0; i < _vecTopRankerInfo.size(); ++i)
    {
        auto item = _vecItemTopRank[i];
        auto info = _vecTopRankerInfo[i];
        auto sprBg = (Node*) item->getChildByName("bg");
        auto lbScore = (Label*)sprBg->getChildByName("score");
        auto lbnick = (Label*)item->getChildByName("nick");
        
        std::string strScore = MafUtils::doubleToString(info->getScore());
        if(_type == E_RANK_LOCAL_TYPE::EVENT_RACE)
            MafUtils::convertNumberToShort(strScore);
        else
            MafUtils::addCommaToString(strScore, 3, ',');
        
        lbScore->setString(strScore);
        lbnick->setString(info->getNick());
        
        {//hero
            int skinIdx = info->getSkinIdx();
            auto mapCostume = info->getCostume();
            
            auto hero = (UnitBehaviorHeroAnother*) item->getChildByName("hero");
            
            if(skinIdx == 0 || CostumeSkinManager::getInstance()->getInfoSkin(skinIdx) == nullptr)
            {
                hero->setImage(info->getWeaponIdx(), mapCostume[E_COSTUME::HEAD], mapCostume[E_COSTUME::BODY], mapCostume[E_COSTUME::CAPE], mapCostume[E_COSTUME::HAND], mapCostume[E_COSTUME::FOOT]);
                hero->setMotion(info->getWeaponIdx(), mapCostume[E_COSTUME::HEAD], mapCostume[E_COSTUME::BODY], mapCostume[E_COSTUME::CAPE], mapCostume[E_COSTUME::HAND], mapCostume[E_COSTUME::FOOT]);
            }
            else
            {
                hero->setSkin(skinIdx);
            }
            hero->actionMove();
        }
    }
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    
    if(info != nullptr)
    {
        _lbMyRank->setString(MafUtils::toString(info->getRank()));
    }
    if(_lbNick != nullptr)
    {
        _lbNick->setString(AccountManager::getInstance()->getUserNick());
    }
}

void PopupPageRanking::callbackUpdateAfterUpdate(bool bResult)
{
    if(bResult == false)
        return;
    
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            RankingManager::getInstance()->requestEventRaceRankInfo( 0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
    }
}

void PopupPageRanking::callbackNicnameUpdate()
{
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            RankingManager::getInstance()->requestEventRaceRankInfo( 0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
    }
}

#pragma mark - utils

void PopupPageRanking::onClick(Ref *sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        if(_callbackHide != nullptr)
            _callbackHide();
        hide();
        return;
    }
    else if(item->getName().compare("NICK") == 0)
    {
        auto popup = PopupUserNickname::create();
        popup->setCallbackResult(CC_CALLBACK_0(PopupPageRanking::callbackNicnameUpdate, this));
        popup->setHideBackKey(false);
        popup->setHideAll(false);
        popup->show();
    }
    else if(item->getName().compare("USER") == 0)
    {
        if(_vecUserInfo.size() <= 0)
            return;
        
        int idx = item->getTag();
        
        if(idx > _vecUserInfo.size())
            return;
        
        auto info = _vecUserInfo[item->getTag()];
        
        auto popup = PopupOtherUserInfo::create(std::atoi(info->getUserIdx().c_str()), info->getPlatform(), info->getNick());
        popup->show();
        
    }
    else if(item->getName().compare("TOP_USER") == 0)
    {
        if(_vecTopRankerInfo.size() < item->getTag()+1)
        {
            return;
        }
        
        auto info = _vecTopRankerInfo[item->getTag()];
        
        auto popup = PopupOtherUserInfo::create(std::atoi(info->getUserIdx().c_str()), info->getPlatform(), info->getNick());
        popup->show();
        
    }
    else if(item->getName().compare("VIEW_MY_RANK") == 0)
    {
        PopupLoading::show();
        switch(_type)
        {
            case E_RANK_LOCAL_TYPE::EVENT_RACE:
            {
                RankingManager::getInstance()->requestEventRaceRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
                break;
            }
                
            case E_RANK_LOCAL_TYPE::FLASHMAN:
            {
                RankingManager::getInstance()->requestFlashmanRankInfo(0, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
                break;
            }
                
        }
    }
    else if(item->getName().compare("NEXT") == 0)
    {
        onClickPage(E_RANK_PAGE::PAGE_NEXT);
        
    }
    else if(item->getName().compare("PREV") == 0)
    {
        onClickPage(E_RANK_PAGE::PAGE_PREV);
    }
    else if(item->getName().compare("RESET") == 0)
    {
        if ( EventManager::getInstance()->isActivatedEvent(33) )
        {
            auto popup = PopupEventRaceReset::create();
            popup->show();
        }
        else
        {
            item->setVisible(false);
        }
    }
    
}

void PopupPageRanking::onClickPage(E_RANK_PAGE page)
{
    int nPage = RankingManager::getInstance()->getNowPage();
    int nMaxPage = RankingManager::getInstance()->getMaxPage();
    
    switch(page)
    {
        case E_RANK_PAGE::NONE:  break;
        case E_RANK_PAGE::PAGE_NEXT:
        {
            if(nPage == nMaxPage)
                return;
            
            nPage += 1;
            if(nPage >= nMaxPage)
                nPage = nMaxPage;
        }break;
        case E_RANK_PAGE::PAGE_PREV:
        {
            if(nPage == 1)
                return;
            
            nPage -= 1;
            if(nPage <= 1)
                nPage = 1;
        } break;
    }
    
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::EVENT_RACE:
        {
            PopupLoading::show();
            RankingManager::getInstance()->requestEventRaceRankInfo(nPage, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
            
        case E_RANK_LOCAL_TYPE::FLASHMAN:
        {
            PopupLoading::show();
            RankingManager::getInstance()->requestFlashmanRankInfo(nPage, CC_CALLBACK_3(PopupPageRanking::callbackUserRankInfoUpdate, this));
            break;
        }
    }
    
}

RankInfo* PopupPageRanking::getUserRankInfoFromIdx(int idx)
{
    RankInfo* result = nullptr;
    
    for(int i = 0; i < _vecUserInfo.size(); ++i)
    {
        if(i == idx)
        {
            result = _vecUserInfo[i];
            break;
        }
    }
    
    return result;
}
#pragma mark - cell
CellPageRank::CellPageRank():
_isMyRank(false)
,_info(nullptr)
,_sprBg(nullptr)
,_sprLuctuation(nullptr)
,_lbRank(nullptr)
,_lbNick(nullptr)
,_lbScore(nullptr)
,_item(nullptr)
,_type(E_RANK_LOCAL_TYPE::NONE)
{
}

CellPageRank::~CellPageRank()
{
}

CellPageRank* CellPageRank::create(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
{
    CellPageRank* pRet = new(std::nothrow) CellPageRank();
    if ( pRet && pRet->init(type, info, size) )
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

bool CellPageRank::init(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
{
    _type = type;
    setRankInfo(info);
    setContentSize(size);
    
    _sprBg = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    _sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprBg->setPosition(size/2);
    _sprBg->setContentSize(size);
    addChild(_sprBg);
    _sprBg->setVisible(false);
    
    _lbRank = Label::createWithTTF("", GAME_FONT, 22);
    _lbRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbRank->setPosition(105, size.height/2);
    addChild(_lbRank);
    
    _lbNick = Label::createWithSystemFont("", SYSTEM_FONT, 22);
    _lbNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbNick->setPosition(220, size.height/2);
    addChild(_lbNick);
    
    _lbScore = Label::createWithTTF("", GAME_FONT, 22);
    _lbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbScore->setPosition(610, size.height/2);
    addChild(_lbScore);
    
    
    _sprLuctuation = Sprite::create("Assets/ui_common/rank_rise.png");
    _sprLuctuation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprLuctuation->setPosition(size.width - _sprLuctuation->getContentSize().width, size.height/2);
    addChild(_sprLuctuation);
    _sprLuctuation->setVisible(false);
    
    reDraw();
    
    return true;
}

void CellPageRank::reDraw()
{
    if(_info == nullptr)
        return;
    
    if(!_item->isVisible())
        _item->setVisible(true);
    
    _lbRank->setString(MafUtils::toString(_info->getRank()));
    _lbNick->setString(_info->getNick());
    
    std::string strScore = MafUtils::doubleToString(_info->getScore());
    
    MafUtils::convertNumberToShort(strScore);
    _lbScore->setString(strScore);
    
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    if ( info != nullptr && _isMyRank == true )
    {
        _sprBg->setVisible(true);
        _lbRank->setColor(Color3B(255, 199, 0));
        _lbNick->setColor(Color3B(255, 199, 0));
        _lbScore->setColor(Color3B(255, 199, 0));
        
        switch (info->getFluctuation()) {
            case E_RANK_FLUCTUATION::NONE:
            {
                _sprLuctuation->setVisible(false);
            }
                break;
            case E_RANK_FLUCTUATION::UP:
            {
                _sprLuctuation->setVisible(true);
                _sprLuctuation->setTexture("Assets/ui_common/rank_rise.png");
            }
            break;
                
            case E_RANK_FLUCTUATION::DOWN:
            {
                _sprLuctuation->setVisible(true);
                _sprLuctuation->setTexture("Assets/ui_common/decline.png");
            }
            break;
                
            default:
                break;
        }
    }
    else
    {
        _sprBg->setVisible(false);
        _sprLuctuation->setVisible(false);
        _lbRank->setColor(Color3B(255, 255, 255));
        _lbNick->setColor(Color3B(255, 255, 255));
        _lbScore->setColor(Color3B(255, 255, 255));
    }
}

void CellPageRank::hide()
{
    _info = nullptr;
    _item->setVisible(false);
}

void CellPageRank::setRankInfo(RankInfo *info)
{
    _info = info;
    
    if(_info == nullptr)
        return;
    
    if(_info->getUserIdx() == MafUtils::toString(AccountManager::getInstance()->getUserIdx()))
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if(_info->getPlatform() == "ios")
                _isMyRank = true;
            else
                _isMyRank = false;
        
        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(_info->getPlatform() == "aos")
                _isMyRank = true;
            else
                _isMyRank = false;
        #endif
    }
    else
    {
        _isMyRank = false;
    }
}

void CellPageRank::setItem(MafNode::MafMenuItemSprite* item)
{
    _item = item;
}

MafNode::MafMenuItemSprite* CellPageRank::getItem()
{
    return _item;
}
