//
//  PopupTierRanking.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/04.
//

#include "PopupTierRanking.h"

#include "Common/ConfigKey.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidRankRewardInfo.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/RankingManager.h"

PopupTierRanking* PopupTierRanking::create(E_RANK_LOCAL_TYPE type)
{
    PopupTierRanking* pRet = new(std::nothrow) PopupTierRanking();
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

PopupTierRanking::PopupTierRanking(void):
_menu(nullptr),
_sprBG(nullptr),
_sprTopCell(nullptr),
_lyRankUser(nullptr),
_toggleRankAutoRefresh(nullptr),
_itemTierDropDown(nullptr),
_lbNick(nullptr),
_sprDropDownBg(nullptr),
_type(E_RANK_LOCAL_TYPE::NONE)
{
    _vecUserInfo.clear();
    _vecTopRankerInfo.clear();
}

PopupTierRanking::~PopupTierRanking(void)
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
bool PopupTierRanking::init(E_RANK_LOCAL_TYPE type)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _type = type;
    initVar();
    initUI();

    return true;
}

void PopupTierRanking::initVar()
{
    double nWidth = 734;
    double nHeight = getContentSize().height;
    if ( nHeight > 1210 )
        nHeight = 1210;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
}

void PopupTierRanking::initUI()
{
    _menu = MafNode::MafMenu::create();
    _menu->setAnchorPoint(Vec2::ZERO);
    _menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(_menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
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

void PopupTierRanking::initTopUI()
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
    auto itemTopRank1st = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_1.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
    itemTopRank1st->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank1st->setPosition(_sprBG->getPosition() + Vec2::DOWN * (_sprBG->getContentSize().height/2 - 50));
    itemTopRank1st->setName("TOP_USER");
    itemTopRank1st->setTag(0);
    _menu->addChild(itemTopRank1st);
    _vecItemTopRank.push_back(itemTopRank1st);
    
    auto itemTopRank2nd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_2.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
    itemTopRank2nd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemTopRank2nd->setPosition(itemTopRank1st->getPosition() + Vec2::LEFT * itemTopRank1st->getContentSize().width + Vec2::DOWN * 15);
    itemTopRank2nd->setName("TOP_USER");
    itemTopRank2nd->setTag(1);
    _menu->addChild(itemTopRank2nd);
    _vecItemTopRank.push_back(itemTopRank2nd);
    
    auto itemTopRank3rd = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/ranking/ranking_user1_3.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
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
    
    _itemTierDropDown = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_drop_down1_1.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick,this));
    _itemTierDropDown->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _itemTierDropDown->setPosition(5, _spriteContainer->getContentSize().height - 80);
    _itemTierDropDown->setName("TIER_DROP_DOWN");
    _menu->addChild(_itemTierDropDown);
    
    auto sprDropDownIcon = Sprite::create("Assets/icon/icon_tier_01.png");
    sprDropDownIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprDropDownIcon->setPosition(_itemTierDropDown->getContentSize()/2);
    sprDropDownIcon->setName("ICON");
    _itemTierDropDown->addChild(sprDropDownIcon);
    
    _sprDropDownBg = Sprite::create("Assets/ui_common/btn_drop_down1_2.png");
    _sprDropDownBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _sprDropDownBg->setPosition(_itemTierDropDown->getPosition() + Vec2::DOWN * _itemTierDropDown->getContentSize().height);
    _spriteContainer->addChild(_sprDropDownBg, E_ZORER::UI+2);
    _sprDropDownBg->setVisible(false);

    Vec2 startPos = Vec2(_sprDropDownBg->getContentSize().width/2, _sprDropDownBg->getContentSize().height);
    
    Vec2 lastPos = Vec2::ZERO;
    Size iconSize = Size::ZERO;
    
    auto dropBoxMenu = MafNode::MafMenu::create();
    dropBoxMenu->setAnchorPoint(Vec2::ZERO);
    dropBoxMenu->setPosition(Vec2::ZERO);
    _sprDropDownBg->addChild(dropBoxMenu, E_ZORER::UI);
    
    for(int i = 0; i < 6; ++i)
    {
        std::string path = "Assets/icon/icon_tier_%02d.png";
        std::string name = "TIER";
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create(MafUtils::format(path.c_str(), i+1)), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick,this));
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        float height = item->getContentSize().height;
        item->setPosition(startPos + Vec2::DOWN * (height + i * (height + height/2)));
        item->setName(name);
        item->setTag(i+1);
        dropBoxMenu->addChild(item);
        
        lastPos = item->getPosition() + Vec2::DOWN * height;
        iconSize = item->getContentSize();
    }
    
    
    auto sprAllView = Sprite::create("Assets/icon/icon_tier_00.png");
    
    auto itemIcon = MafNode::MafMenuItemSprite::create(sprAllView, nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
    itemIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemIcon->setPosition(lastPos + Vec2::DOWN * iconSize.height / 4);
    itemIcon->setName("VIEW_ALL");
    dropBoxMenu->addChild(itemIcon);
    
    if(_type == E_RANK_LOCAL_TYPE::NEW_RAID)
    {
        auto btnRankInfo = ui::Button::create("Assets/ui/ranking/icon_ranking_2.png");
        btnRankInfo->addClickEventListener([=](Ref*){
            auto popup = PopupRaidRankRewardInfo::create();
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

void PopupTierRanking::initBottomUI()
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
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
            strDesc = GAME_TEXT("t_ui_rank_desc_1");
            break;

        case E_RANK_LOCAL_TYPE::NEW_RAID:
            strDesc = GAME_TEXT("t_ui_rank_desc_3");
            break;
        default:
            break;
    }
    
    auto lbDesc = Label::createWithTTF(strDesc, GAME_FONT, 17);
    lbDesc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbDesc->setPosition(20, 0);
    lbDesc->setDimensions(sprBox->getContentSize().width - 100, sprBox->getContentSize().height - 10);
    lbDesc->setOverflow(Label::Overflow::SHRINK);
    lbDesc->setColor(Color3B(173, 151, 125));
    sprBox->addChild(lbDesc);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    sprBox->addChild(menu, E_ZORER::UI);
    
    auto itemNick = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/raid/icon_name.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
    itemNick->setName("NICK");
    itemNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    Size boxSize = sprBox->getContentSize();
    itemNick->setPosition(boxSize.width - itemNick->getContentSize().width/2 - 15, boxSize.height/2);
    menu->addChild(itemNick);
    
    _lyRankUser = LayerColor::create();
    _lyRankUser->setPosition(Vec2(5,0) + Vec2::UP * (sprBox->getContentSize().height + 20));
    _lyRankUser->setContentSize(Size(_spriteContainer->getContentSize().width - 10, _sprTopCell->getPosition().y - _sprTopCell->getContentSize().height - sprBox->getContentSize().height - 30));
    _spriteContainer->addChild(_lyRankUser);
    
    auto itemPre = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_L02.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
    itemPre->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    itemPre->setPosition(Vec2(-itemPre->getContentSize().width/2, _sprTopCell->getPosition().y/2));
    itemPre->setTouchRect(itemPre->getContentSize() + Size(15, 40));
    itemPre->setName("PREV");
    menu->addChild(itemPre);
    
    auto itemNext = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_R02.png"), nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
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
    
    _sprMyTier = Sprite::create("Assets/icon/icon_tier_06.png");
    _sprMyTier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprMyTier->setPosition(_sprMyTier->getContentSize().width/2, sprMyrankBox->getContentSize().height/2);
    sprMyrankBox->addChild(_sprMyTier);
    
    _lbMyRank = Label::createWithTTF("", GAME_FONT, 23);
    _lbMyRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbMyRank->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbMyRank->setPosition(_sprMyTier->getPosition() + Vec2::RIGHT * _sprMyTier->getContentSize().width/2);
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
    
    auto itemMyRank = MafNode::MafMenuItemSprite::create(sprMyRank, nullptr, CC_CALLBACK_1( PopupTierRanking::onClick, this));
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

void PopupTierRanking::show()
{
    PopupBase::show();
    
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
            if(info == nullptr)
            {
                RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupTierRanking::callbackUpdateAfterInit, this), true);
            }
            else
            {
                if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) != info->getScore())
                    RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupTierRanking::callbackUpdateAfterInit, this), true);
                else
                    RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoInit, this));
            }
        }
        break;
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            PopupLoading::show();
            auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
            int nTire = 0;
            if(infoRank != nullptr)
            {
                nTire = infoRank->getTier();
            }
            RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoInit, this));
        }
            
        default:
        break;
    }
}

#pragma mark - afterInit
void PopupTierRanking::afterInit()
{
    afterInitTopUI();
    afterInitBottomUI();
}

void PopupTierRanking::afterInitTopUI()
{
    auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(_type);
    std::string path = "Assets/icon/icon_tier_%02d.png";
    
    auto sprTierIcon = (Sprite*)_itemTierDropDown->getChildByName("ICON");
    if(sprTierIcon != nullptr)
        sprTierIcon->setTexture(MafUtils::format(path.c_str(), info->getTier()));
    
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
            if(_type != E_RANK_LOCAL_TYPE::NEW_RAID)
                MafUtils::addCommaToString(strScore, 3, ',');
            std::string strScoreFinal = "";
            
            if(_type == E_RANK_LOCAL_TYPE::HIGH_FLOOR)
                strScoreFinal = GAME_TEXTFORMAT("t_relic_option_9", strScore.c_str());
            else
                strScoreFinal = strScore;
            
            if(_type == E_RANK_LOCAL_TYPE::NEW_RAID)
                MafUtils::convertNumberToShort(strScoreFinal);
            
            auto lbScore = Label::createWithTTF(strScoreFinal, GAME_FONT, 20);
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

void PopupTierRanking::afterInitBottomUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyRankUser->addChild(menu, E_ZORER::UI);
    
    
    for(int i = 0; i < 11; ++i)
    {
        auto cell =  CellTierRank::create(_type, nullptr, Size(_lyRankUser->getContentSize().width, 48));
        _vecCellRank.push_back(cell);
        auto itemCell = MafNode::MafMenuItemSprite::create(cell, nullptr, CC_CALLBACK_1(PopupTierRanking::onClick, this));
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
        
        if(info->getTier() > 0)
        {
            _sprMyTier->setTexture(MafUtils::format("Assets/icon/icon_tier_%02d.png",info->getTier()));
        }
    }
}

#pragma mark - callback Init
void PopupTierRanking::callbackUserRankInfoInit(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
{
    if(PopupLoading::isShow())
        PopupLoading::hide();
    if(bResult == false)
        return;
    
    _vecUserInfo = vecUserInfo;
    _vecTopRankerInfo = vecTopRank;
    afterInit();
    
}

void PopupTierRanking::callbackUpdateAfterInit(bool bResult)
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
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoInit, this));
        }
            break;
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
            int nTire = 0;
            if(infoRank != nullptr)
            {
                nTire = infoRank->getTier();
            }
            RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoInit, this));
        }
            break;
    }
}

void PopupTierRanking::callbackNicknameInit()
{
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupTierRanking::callbackUpdateAfterInit, this), true);
        }
            break;
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
            int nTire = 0;
            if(infoRank != nullptr)
            {
                nTire = infoRank->getTier();
            }
            RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoInit, this));
        }
            break;
    }
}

#pragma mark - callback Update

void PopupTierRanking::callbackUserRankInfoUpdate(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank)
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
        if(_type != E_RANK_LOCAL_TYPE::NEW_RAID)
            MafUtils::addCommaToString(strScore, 3, ',');
        std::string strScoreFinal = "";
        
        if(_type == E_RANK_LOCAL_TYPE::HIGH_FLOOR)
            strScoreFinal = GAME_TEXTFORMAT("t_relic_option_9", strScore.c_str());
        else
            strScoreFinal = strScore;
        
        if(_type == E_RANK_LOCAL_TYPE::NEW_RAID)
            MafUtils::convertNumberToShort(strScoreFinal);
        
        lbScore->setString(strScoreFinal);
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
        
        if(info->getTier() > 0)
        {
            _sprMyTier->setTexture(MafUtils::format("Assets/icon/icon_tier_%02d.png",info->getTier()));
        }
    }
    if(_sprDropDownBg != nullptr)
    {
        _sprDropDownBg->setVisible(false);
    }
    if(_lbNick != nullptr)
    {
        _lbNick->setString(AccountManager::getInstance()->getUserNick());
    }
    
    std::string path = "Assets/icon/icon_tier_%02d.png";
    if(RankingManager::getInstance()->getNowTier() == 7)
    {
        path = MafUtils::format(path.c_str(), 0);
    }
    else
    {
        path = MafUtils::format(path.c_str(), RankingManager::getInstance()->getNowTier());
        
    }
    auto sprTierIcon = (Sprite*)_itemTierDropDown->getChildByName("ICON");
    if(sprTierIcon != nullptr)
        sprTierIcon->setTexture(path);
}

void PopupTierRanking::callbackUpdateAfterUpdate(bool bResult)
{
    if(bResult == false)
        return;
    
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            RankingManager::getInstance()->requestHighFloorUserRankInfo(CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
        }
            break;
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
            int nTire = 0;
            if(infoRank != nullptr)
            {
                nTire = infoRank->getTier();
            }
            RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
        }
            break;
    }
}

void PopupTierRanking::callbackNicnameUpdate()
{
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::HIGH_FLOOR:
        {
            RankingManager::getInstance()->requestHighFloorRankUpdate(CC_CALLBACK_1(PopupTierRanking::callbackUpdateAfterUpdate, this), true);
        }
            break;
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
            int nTire = 0;
            if(infoRank != nullptr)
            {
                nTire = infoRank->getTier();
            }
            RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
        }
            break;
    }
}

#pragma mark - utils

void PopupTierRanking::onClick(Ref *sender)
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
        popup->setCallbackResult(CC_CALLBACK_0(PopupTierRanking::callbackNicnameUpdate, this));
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
    else if (item->getName().compare("TIER_DROP_DOWN") == 0)
    {
        _sprDropDownBg->setVisible(!_sprDropDownBg->isVisible());
    }
    else if(item->getName().compare("TIER") == 0)
    {
        onClickTier(item->getTag());
    }
    else if(item->getName().compare("VIEW_ALL") == 0)
    {
        onClickTier(0);
    }
    else if(item->getName().compare("VIEW_MY_RANK") == 0)
    {
        PopupLoading::show();
        switch(_type)
        {
            case E_RANK_LOCAL_TYPE::NEW_RAID:
            {
                auto infoRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
                int nTire = 0;
                if(infoRank != nullptr)
                {
                    nTire = infoRank->getTier();
                }
                RankingManager::getInstance()->requestRaidUserRankInfo(nTire, 0, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
            }
                break;
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
    
}

void PopupTierRanking::onClickTier(int idx)
{
    
    PopupLoading::show();
    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::NEW_RAID:
        {
            RankingManager::getInstance()->requestRaidUserRankInfo(idx, 1, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
        }
            break;
    }
}

void PopupTierRanking::onClickPage(E_RANK_PAGE page)
{

    switch(_type)
    {
        case E_RANK_LOCAL_TYPE::NEW_RAID :
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
            
            PopupLoading::show();
            RankingManager::getInstance()->requestRaidUserRankInfo(RankingManager::getInstance()->getNowTier(), nPage, CC_CALLBACK_3(PopupTierRanking::callbackUserRankInfoUpdate, this));
        }break;
    }
    
}

RankInfo* PopupTierRanking::getUserRankInfoFromIdx(int idx)
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
CellTierRank::CellTierRank():
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

CellTierRank::~CellTierRank()
{
}

CellTierRank* CellTierRank::create(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
{
    CellTierRank* pRet = new(std::nothrow) CellTierRank();
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

bool CellTierRank::init(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size)
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

void CellTierRank::reDraw()
{
    if(_info == nullptr)
        return;
    
    if(!_item->isVisible())
        _item->setVisible(true);
    
    _lbRank->setString(MafUtils::toString(_info->getRank()));
    _lbNick->setString(_info->getNick());
    
    std::string strScore = MafUtils::doubleToString(_info->getScore());
    
    strScore = MafUtils::addCommaToString(strScore, 3, ',');
    _lbScore->setString(strScore);
    
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
        _lbRank->setColor(Color3B(255, 255, 255));
        _lbNick->setColor(Color3B(255, 255, 255));
        _lbScore->setColor(Color3B(255, 255, 255));
    }
}

void CellTierRank::hide()
{
    _info = nullptr;
    _item->setVisible(false);
}

void CellTierRank::setRankInfo(RankInfo *info)
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

void CellTierRank::setItem(MafNode::MafMenuItemSprite* item)
{
    _item = item;
}

MafNode::MafMenuItemSprite* CellTierRank::getItem()
{
    return _item;
}
