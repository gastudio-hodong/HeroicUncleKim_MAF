//
//  PopupRanking.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/16.
//

#include "PopupRanking.h"

#include "Common/ConfigKey.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/RankingManager.h"

USING_NS_CC;

PopupRanking* PopupRanking::create(E_RANK_LOCAL_TYPE type)
{
    PopupRanking* pRet = new(std::nothrow) PopupRanking();
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

PopupRanking::PopupRanking(void):
_menu(nullptr),
_sprBG(nullptr),
_sprTopCell(nullptr),
_lyRankUser(nullptr),
_toggleViewSpQuest(nullptr),
_type(E_RANK_LOCAL_TYPE::NONE)
{
    _vecUserInfo.clear();
    _vecTopRankerInfo.clear();
}

PopupRanking::~PopupRanking(void)
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
    _vecItemTopRank.clear();
    _vecCellRank.clear();
}

#pragma mark - init
bool PopupRanking::init(E_RANK_LOCAL_TYPE type)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();
    _type = type;

    return true;
}

void PopupRanking::initVar()
{
    double nWidth = 734;
    double nHeight = getContentSize().height;
    if ( nHeight > 1210 )
        nHeight = 1210;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
}

void PopupRanking::initUI()
{
    _menu = MafNode::MafMenu::create();
    _menu->setAnchorPoint(Vec2::ZERO);
    _menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(_menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
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

void PopupRanking::initTopUI()
{
    _sprBG = Sprite::create("Assets/ui/ranking/ranking_user_bg1.png");
    _sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sprBG->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_sprBG);
    
    //랭커 단상 UI
    auto itemTopRank1st = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_1.png"), nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
    itemTopRank1st->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank1st->setPosition(_sprBG->getPosition() + Vec2::DOWN * (_sprBG->getContentSize().height/2 - 50));
    itemTopRank1st->setName("TOP_USER");
    itemTopRank1st->setTag(0);
    _menu->addChild(itemTopRank1st);
    _vecItemTopRank.push_back(itemTopRank1st);
    
    auto itemTopRank2nd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_2.png"), nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
    itemTopRank2nd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank2nd->setPosition(itemTopRank1st->getPosition() + Vec2::LEFT * itemTopRank1st->getContentSize().width + Vec2::DOWN * 15);
    itemTopRank2nd->setName("TOP_USER");
    itemTopRank2nd->setTag(1);
    _menu->addChild(itemTopRank2nd);
    _vecItemTopRank.push_back(itemTopRank2nd);
    
    auto itemTopRank3rd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_3.png"), nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
    itemTopRank3rd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank3rd->setPosition(itemTopRank1st->getPosition() + Vec2::RIGHT * itemTopRank1st->getContentSize().width + Vec2::DOWN * 15);
    itemTopRank3rd->setName("TOP_USER");
    itemTopRank3rd->setTag(2);
    _menu->addChild(itemTopRank3rd);
    _vecItemTopRank.push_back(itemTopRank3rd);
    
    _toggleViewSpQuest = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/mission/mission_on.png"), nullptr, [&](Ref* sender){
        bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
        isRefreshRank = !isRefreshRank;
        
        if(isRefreshRank) _toggleViewSpQuest->setNormalImage(Sprite::create("Assets/ui/mission/mission_on.png"));
        else                   _toggleViewSpQuest->setNormalImage(Sprite::create("Assets/ui/mission/mission_off.png"));
        
        UserDefault::getInstance()->setBoolForKey(KEY_REFRESH_RANK, isRefreshRank);
    });
    
    _toggleViewSpQuest->setPosition(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height + 5);
    _toggleViewSpQuest->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _menu->addChild(_toggleViewSpQuest);
    bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
    if(isRefreshRank) _toggleViewSpQuest->setNormalImage(Sprite::create("Assets/ui/mission/mission_on.png"));
    else                   _toggleViewSpQuest->setNormalImage(Sprite::create("Assets/ui/mission/mission_off.png"));
    
    auto toggle_desc = Label::createWithTTF(GAME_TEXT("t_ui_rank_toggle_desc"), GAME_FONT, 24);
    toggle_desc->setColor(Color3B::WHITE);
    toggle_desc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    toggle_desc->setPosition(_toggleViewSpQuest->getContentSize() + Size(0,10));
    _toggleViewSpQuest->addChild(toggle_desc);

    
}

void PopupRanking::initBottomUI()
{
    _sprTopCell = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    _sprTopCell->setContentSize(Size(_spriteContainer->getContentSize().width - 12, 34));
    _sprTopCell->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sprTopCell->setPosition(_sprBG->getPosition() + Vec2::DOWN * (_sprBG->getContentSize().height - _sprBG->getContentSize().height/5));
    _spriteContainer->addChild(_sprTopCell);
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
    _spriteContainer->addChild(sprBox);
    
    auto lbDesc = Label::createWithTTF(GAME_TEXT("t_ui_rank_desc_1"), GAME_FONT, 17);
    lbDesc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbDesc->setPosition(20, 0);
    lbDesc->setDimensions(sprBox->getContentSize().width - 100, sprBox->getContentSize().height - 10);
    lbDesc->setOverflow(Label::Overflow::SHRINK);
    lbDesc->setColor(Color3B(173, 151, 125));
    sprBox->addChild(lbDesc);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    sprBox->addChild(menu);
    
    auto itemNick = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/raid/icon_name.png"), nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
    itemNick->setName("NICK");
    itemNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    Size boxSize = sprBox->getContentSize();
    itemNick->setPosition(boxSize.width - itemNick->getContentSize().width/2 - 15, boxSize.height/2);
    menu->addChild(itemNick);
    
    _lyRankUser = LayerColor::create();
    _lyRankUser->setPosition(Vec2(5,0) + Vec2::UP * (sprBox->getContentSize().height + 20));
    _lyRankUser->setContentSize(Size(_spriteContainer->getContentSize().width - 10, _sprTopCell->getPosition().y - _sprTopCell->getContentSize().height - sprBox->getContentSize().height - 30));
    _spriteContainer->addChild(_lyRankUser);
}

void PopupRanking::show()
{
    PopupBase::show();
    
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
            if(info == nullptr)
            {
                RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupRanking::callbackUpdateAfterInit, this), true);
            }
            else
            {
                if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) != info->getScore())
                    RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupRanking::callbackUpdateAfterInit, this), true);
                else
                    RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupRanking::callbackUserRankInfoInit, this));
            }
        }
        break;
            
        default:
        break;
    }
}

#pragma mark - afterInit
void PopupRanking::afterInit()
{
    afterInitTopUI();
    afterInitBottomUI();
}

void PopupRanking::afterInitTopUI()
{
    
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
            MafUtils::addCommaToString(strScore, 3, ',');
            
            auto lbScore = Label::createWithTTF(GAME_TEXTFORMAT("t_relic_option_9", strScore.c_str()), GAME_FONT, 20);
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

void PopupRanking::afterInitBottomUI()
{
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyRankUser->addChild(menu, E_ZORER::UI+1);
    
    
    for(int i = 0; i < _vecUserInfo.size(); ++i)
    {
        auto cell =  CellRank::create(_type, _vecUserInfo[i], Size(_lyRankUser->getContentSize().width, 48));
        _vecCellRank.push_back(cell);
        auto itemIcon = MafNode::MafMenuItemSprite::create(cell, nullptr, CC_CALLBACK_1(PopupRanking::onClick, this));
        itemIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemIcon->setPosition(_lyRankUser->getContentSize().width/2, (_lyRankUser->getContentSize().height - 15) - i * cell->getContentSize().height);
        itemIcon->setName("USER");
        itemIcon->setTag(i);
        
        menu->addChild(itemIcon);
    }
    
}

#pragma mark - callback Init
void PopupRanking::callbackUserRankInfoInit(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
{
    if(bResult == false)
    {
        hide();
        return;
    }
    
    _vecUserInfo = vecUserInfo;
    _vecTopRankerInfo = vecTopRank;
    afterInit();
}

void PopupRanking::callbackUpdateAfterInit(bool bResult)
{
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        hide();
        return;
    }
    RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupRanking::callbackUserRankInfoInit, this));
}

void PopupRanking::callbackNicknameInit()
{
    RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupRanking::callbackUpdateAfterInit, this), true);
}

#pragma mark - callback Update

void PopupRanking::callbackUserRankInfoUpdate(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
{
    if(bResult == false)
    {
        hide();
        return;
    }
    
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
    
    _vecUserInfo = vecUserInfo;
    _vecTopRankerInfo = vecTopRank;
    
    for(int i = 0; i < _vecCellRank.size(); ++i)
    {
        _vecCellRank[i]->setRankInfo(_vecUserInfo[i]);
        _vecCellRank[i]->reDraw();
    }
    
    for(int i = 0; i < vecTopRank.size(); ++i)
    {
        auto item = _vecItemTopRank[i];
        auto info = vecTopRank[i];
        auto sprBg = (Node*) item->getChildByName("bg");
        auto lbScore = (Label*)sprBg->getChildByName("score");
        auto lbnick = (Label*)item->getChildByName("nick");
        std::string strScore = MafUtils::doubleToString(info->getScore());
        MafUtils::addCommaToString(strScore, 3, ',');
        
        lbScore->setString(GAME_TEXTFORMAT("t_relic_option_9", strScore.c_str()));
        lbnick->setString(info->getNick());
    }
}

void PopupRanking::callbackUpdateAfterUpdate(bool bResult)
{
    if(bResult == false)
        return;
    
    RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupRanking::callbackUserRankInfoUpdate, this));
}

void PopupRanking::callbackNicnameUpdate()
{
    RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupRanking::callbackUpdateAfterUpdate, this), true);
}

#pragma mark - utils

void PopupRanking::onClick(Ref *sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
        return;
    }
    else if(item->getName().compare("NICK") == 0)
    {
        auto popup = PopupUserNickname::create();
        popup->setCallbackResult(CC_CALLBACK_0(PopupRanking::callbackNicnameUpdate, this));
        popup->setHideBackKey(false);
        popup->setHideAll(false);
        popup->show();
    }
    else if(item->getName().compare("USER") == 0)
    {
        if(_vecUserInfo.size() <= 0)
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
}

CellRank::CellRank():
_isMyRank(false)
,_info(nullptr)
,_sprBg(nullptr)
,_sprLuctuation(nullptr)
,_lbRank(nullptr)
,_lbNick(nullptr)
,_lbScore(nullptr)
,_type(E_RANK_LOCAL_TYPE::NONE)
{
}

CellRank::~CellRank()
{
}

CellRank* CellRank::create(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
{
    CellRank* pRet = new(std::nothrow) CellRank();
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

bool CellRank::init(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
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
    
    _lbRank = Label::createWithTTF(MafUtils::toString(_info->getRank()), GAME_FONT, 22);
    _lbRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbRank->setPosition(105, size.height/2);
    addChild(_lbRank);
    
    _lbNick = Label::createWithSystemFont(_info->getNick(), SYSTEM_FONT, 22);
    _lbNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbNick->setPosition(220, size.height/2);
    addChild(_lbNick);
    
    _lbScore = Label::createWithTTF(MafUtils::doubleToString(_info->getScore()), GAME_FONT, 22);
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

void CellRank::reDraw()
{
    _lbRank->setString(MafUtils::toString(_info->getRank()));
    _lbNick->setString(_info->getNick());
    _lbScore->setString(MafUtils::doubleToString(_info->getScore()));
    
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    
    if(_isMyRank)
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
        if(info->getRank() < _info->getRank())
        {
            _lbRank->setColor(Color3B(128, 128, 128));
            _lbNick->setColor(Color3B(128, 128, 128));
            _lbScore->setColor(Color3B(128, 128, 128));
        }
        else
        {
            _lbRank->setColor(Color3B(255, 255, 255));
            _lbNick->setColor(Color3B(255, 255, 255));
            _lbScore->setColor(Color3B(255, 255, 255));
        }
    }
}

void CellRank::setRankInfo(RankInfo *info)
{
    _info = info;
    
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
