//
//  PopupOtherUserInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/18.
//

#include "PopupOtherUserInfo.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoArtifact.h"
#include "GameObject/Defense/InfoDefenseManaAMP.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUIPopup/Other/User/PopupUserReport.h"
#include "GameUIPopup/Other/Ranking/PopupOtherUserManaAMPList.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/FriendManager.h"
#include "ManagerGame/StoneManager.h"


USING_NS_CC;

PopupOtherUserInfo* PopupOtherUserInfo::create(int userIdx, std::string platform, std::string nick)
{
    PopupOtherUserInfo* pRet = new(std::nothrow) PopupOtherUserInfo();
    if ( pRet && pRet->init(userIdx, platform, nick) )
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

PopupOtherUserInfo::PopupOtherUserInfo():
_userIdx(0),
_highFloor(0),
_totalFloor(0),
_skinIdx(0),
_weaponIdx(0),
_totalCostumeNum(0),
_defenseHighFloor(0),

_platform(""),
_nick(""),
_startDay(""),

_infoSpirit(nullptr),

_lbHighFloor(nullptr),
_lbTotalFloor(nullptr),
_lbStarDay(nullptr),
_lbTotalCostume(nullptr),

_lyUserInfoArea(nullptr),
_lyArtifactArea(nullptr),
_lyBadgeArea(nullptr),
_lyButtonArea(nullptr),

_isInfinite(false),
_nInfiniteGrade(0),
_nBongTowerFloor(0)
{
    _mapCostume.clear();
    _vecArtifact.clear();
    _vecSprArtiBox.clear();
    _vecInfiniteArtiLv.clear();
    _vecEnchantStoneLv.clear();
    _vecBadge.clear();
    _listManaAMP.clear();
    for(int i = E_ARTIFACT_TYPE::ARTI_TYPE_ATK; i < E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT; ++i)
    {
        auto info = new tagArtiInfo();
        info->_type = (E_ARTIFACT_TYPE)i;
        _vecArtifact.push_back(info);
    }
    
    for(int i = 1; i <= C_COUNT_ENCHANT_STONE; ++i)
    {
        _vecEnchantStoneLv.push_back(std::make_pair((E_STONE) i, 0));
    }
    
    for(int i = 1; i <= 6; ++i)
    {
        _vecBuildingLv.push_back(std::make_pair((E_TOWN) i, 0));
    }
    
}

PopupOtherUserInfo::~PopupOtherUserInfo()
{
    for(int i = 0 ; i < _vecArtifact.size(); ++i)
        delete _vecArtifact[i];
    
    delete _infoSpirit;
    _vecArtifact.clear();
    _vecInfiniteArtiLv.clear();
    _vecEnchantStoneLv.clear();
    _vecBadge.clear();
    _listManaAMP.clear();
}

#pragma mark -init
bool PopupOtherUserInfo::init(int userIdx, std::string platform, std::string nick)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _userIdx = userIdx;
    _platform = platform;
    _nick = nick;
    
    initVar();
    initUI();
    
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        
        cocos2d::Rect rect = _spriteContainer->getBoundingBox();
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
        {
            hide();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    requestUserInfo();
    
    return true;
}
void PopupOtherUserInfo::initVar()
{
    double nWidth = 734;
    double nHeight = 925;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    
    _lyUserInfoArea = LayerColor::create();
    _lyUserInfoArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyUserInfoArea->setContentSize(Size(_spriteContainer->getContentSize().width, 380));
    _lyUserInfoArea->setIgnoreAnchorPointForPosition(false);
    _lyUserInfoArea->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyUserInfoArea);
    
    _lyArtifactArea = LayerColor::create();
    _lyArtifactArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyArtifactArea->setContentSize(Size(_spriteContainer->getContentSize().width, 210));
    _lyArtifactArea->setIgnoreAnchorPointForPosition(false);
    _lyArtifactArea->setPosition(_spriteContainer->getContentSize().width/2, _lyUserInfoArea->getPositionY() - _lyUserInfoArea->getContentSize().height);
    _spriteContainer->addChild(_lyArtifactArea);
    
    _lyBadgeArea = LayerColor::create();
    _lyBadgeArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBadgeArea->setContentSize(Size(_spriteContainer->getContentSize().width, 170));
    _lyBadgeArea->setIgnoreAnchorPointForPosition(false);
    _lyBadgeArea->setPosition(_spriteContainer->getContentSize().width/2, _lyArtifactArea->getPositionY() - _lyArtifactArea->getContentSize().height);
    _spriteContainer->addChild(_lyBadgeArea);
    
    _lyButtonArea = LayerColor::create();
    _lyButtonArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyButtonArea->setContentSize(Size(_spriteContainer->getContentSize().width, 120));
    _lyButtonArea->setIgnoreAnchorPointForPosition(false);
    _lyButtonArea->setPosition(_spriteContainer->getContentSize().width/2, 0);
    _spriteContainer->addChild(_lyButtonArea);
}

void PopupOtherUserInfo::initUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupOtherUserInfo::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithSystemFont(_nick, SYSTEM_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    initTopUI();
    initArtifactUI();
    initButtonUI();
}

void PopupOtherUserInfo::initTopUI()
{
    Size lySize = _lyUserInfoArea->getContentSize();
    
    auto sprHighFloor = Sprite::create("Assets/icon/icon_tower.png");
    sprHighFloor->setContentSize(Size(58, 58));
    sprHighFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprHighFloor->setPosition(Vec2(lySize.width - lySize.width/3, lySize.height) + Vec2::DOWN * 30);
    _lyUserInfoArea->addChild(sprHighFloor);
    
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_high_floor"), GAME_FONT, 20);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(Vec2(sprHighFloor->getContentSize()/2) + Vec2::RIGHT * sprHighFloor->getContentSize().width/2);
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setDimensions(100, 25);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        sprHighFloor->addChild(lbTitle);
        
        _lbHighFloor = Label::createWithTTF("0", GAME_FONT, 20);
        _lbHighFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbHighFloor->setPosition(_lyUserInfoArea->getContentSize().width - 15, sprHighFloor->getPosition().y);
        _lbHighFloor->setColor(Color3B(228, 162, 48));
        _lbHighFloor->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        _lbHighFloor->setDimensions(100, 25);
        _lbHighFloor->setOverflow(Label::Overflow::SHRINK);
        _lyUserInfoArea->addChild(_lbHighFloor);
    
    }
    
    auto sprTotalFloor = Sprite::create("Assets/icon/icon_power.png");
    sprTotalFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprTotalFloor->setPosition(sprHighFloor->getPosition() + Vec2::DOWN * (sprHighFloor->getContentSize().height/2 + 5));
    _lyUserInfoArea->addChild(sprTotalFloor);
    
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_total_floor"), GAME_FONT, 20);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(Vec2(sprTotalFloor->getContentSize()/2) + Vec2::RIGHT * sprTotalFloor->getContentSize().width/2);
        
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setDimensions(100, 25);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        sprTotalFloor->addChild(lbTitle);
        
        _lbTotalFloor = Label::createWithTTF("0", GAME_FONT, 20);
        _lbTotalFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbTotalFloor->setPosition(_lyUserInfoArea->getContentSize().width - 15, sprTotalFloor->getPosition().y);
        _lbTotalFloor->setColor(Color3B(228, 162, 48));
        _lbTotalFloor->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        _lbTotalFloor->setDimensions(100, 25);
        _lbTotalFloor->setOverflow(Label::Overflow::SHRINK);
        _lyUserInfoArea->addChild(_lbTotalFloor);
    
    }
    
    auto sprStartDay = Sprite::create("Assets/icon/icon_start1_1.png");
    sprStartDay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprStartDay->setPosition(sprTotalFloor->getPosition() + Vec2::DOWN * (sprTotalFloor->getContentSize().height/2 + 5));
    _lyUserInfoArea->addChild(sprStartDay);
    
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_vip_count"), GAME_FONT, 20);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(Vec2(sprStartDay->getContentSize()/2) + Vec2::RIGHT * sprStartDay->getContentSize().width/2);
        
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setDimensions(100, 25);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        sprStartDay->addChild(lbTitle);
        
        _lbStarDay = Label::createWithTTF("0", GAME_FONT, 20);
        _lbStarDay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbStarDay->setPosition(_lyUserInfoArea->getContentSize().width - 15, sprStartDay->getPosition().y);
        _lbStarDay->setColor(Color3B(228, 162, 48));
        _lbStarDay->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        _lbStarDay->setDimensions(100, 25);
        _lbStarDay->setOverflow(Label::Overflow::SHRINK);
        _lyUserInfoArea->addChild(_lbStarDay);
    
    }
    
    auto sprTotalCostume = Sprite::create("Assets/icon/icon_costume.png");
    sprTotalCostume->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprTotalCostume->setPosition(sprStartDay->getPosition() + Vec2::DOWN * (sprStartDay->getContentSize().height/2 +5));
    _lyUserInfoArea->addChild(sprTotalCostume);
    
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_stat_button_3"), GAME_FONT, 20);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(Vec2(sprTotalCostume->getContentSize()/2) + Vec2::RIGHT * sprTotalCostume->getContentSize().width/2);
        
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setDimensions(100, 25);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        sprTotalCostume->addChild(lbTitle);
        
        _lbTotalCostume = Label::createWithTTF("0", GAME_FONT, 20);
        _lbTotalCostume->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbTotalCostume->setPosition(_lyUserInfoArea->getContentSize().width - 15, sprTotalCostume->getPosition().y);
        _lbTotalCostume->setColor(Color3B(228, 162, 48));
        _lbTotalCostume->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        _lbTotalCostume->setDimensions(100, 25);
        _lbTotalCostume->setOverflow(Label::Overflow::SHRINK);
        _lyUserInfoArea->addChild(_lbTotalCostume);
    
    }
    
}

void PopupOtherUserInfo::initArtifactUI()
{
    auto sprArtiTitle = Sprite::create("Assets/ui_common/base_text_1_3.png");
    sprArtiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprArtiTitle->setPosition(_lyArtifactArea->getContentSize().width/2, _lyArtifactArea->getContentSize().height);
    _lyArtifactArea->addChild(sprArtiTitle);
    
    auto lbArtiTile = Label::createWithTTF(GAME_TEXT("t_ui_menu_stat_treasure"), GAME_FONT, 24);
    lbArtiTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbArtiTile->setPosition(sprArtiTitle->getContentSize()/2);
    lbArtiTile->setColor(COLOR_COIN);
    sprArtiTitle->addChild(lbArtiTile);
    
    for ( int i = 0; i < 5; ++i )
    {
        
        Sprite* sprStatbox = Sprite::create(MafUtils::format("Assets/ui/stat/stat_box_%d.png",i+1));
        sprStatbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprStatbox->setPosition(sprArtiTitle->getContentSize().width * 0.04 + (sprStatbox->getContentSize().width + 35) * i, sprArtiTitle->getPosition().y - 50);
        sprStatbox->setTag(E_ARTIFACT_TYPE::ARTI_TYPE_ATK + i);
        
        _lyArtifactArea->addChild(sprStatbox);
        _vecSprArtiBox.push_back(sprStatbox);
    }
}

void PopupOtherUserInfo::initBadgeUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _lyBadgeArea->addChild(menu);
    
    auto sprArtiTitle = Sprite::create("Assets/ui_common/base_text_1_3.png");
    sprArtiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprArtiTitle->setPosition(_lyBadgeArea->getContentSize().width/2, _lyBadgeArea->getContentSize().height);
    _lyBadgeArea->addChild(sprArtiTitle);
    
    auto lbArtiTile = Label::createWithTTF(GAME_TEXT("t_ui_badge_title_select"), GAME_FONT, 24);
    lbArtiTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbArtiTile->setPosition(sprArtiTitle->getContentSize()/2);
    lbArtiTile->setColor(COLOR_COIN);
    sprArtiTitle->addChild(lbArtiTile);
    
    Vec2 startPos = Vec2(_lyBadgeArea->getContentSize().width/5, sprArtiTitle->getPosition().y - 50);
    for ( int i = 0; i < _vecBadge.size(); ++i )
    {
        auto objBadgeHave = _vecBadge.at(i);
        if(objBadgeHave == nullptr)
            continue;
        
        auto objBadge = BadgeManager::getInstance()->getBadge(objBadgeHave->getIdxBadge());
        
        if(objBadge == nullptr)
            continue;
        
        Sprite* sprBadge = Sprite::create(objBadge->getPath());
        if(sprBadge == nullptr)
        {
            sprBadge = Sprite::create("Assets/ui/stat/badge_000.png");
        }
        sprBadge->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprBadge->setPosition(sprArtiTitle->getContentSize().width * 0.04 + (sprBadge->getContentSize().width + 45) * i, sprArtiTitle->getPosition().y - 50);
        
        _lyBadgeArea->addChild(sprBadge);
    }
    
    
}

void PopupOtherUserInfo::initButtonUI()
{
    _lyButtonArea->removeAllChildren();
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_lyButtonArea->getContentSize().width / 2, _lyButtonArea->getContentSize().height / 2);
    _lyButtonArea->addChild(menu);
    
    
    
    if ( _userIdx != AccountManager::getInstance()->getUserIdx() || _platform.compare(AccountManager::getInstance()->getUserPlatform()) != 0 )
    {
        // 친구 추가
        auto spriteFriend = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
        spriteFriend->setContentSize(Size(180, 78));
        
        auto spriteFriendDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(32,35,10,2));
        spriteFriendDisable->setContentSize(Size(180, 78));
        
        auto itemFriend = MafNode::MafMenuItemSprite::create(spriteFriend, nullptr, spriteFriendDisable, CC_CALLBACK_1(PopupOtherUserInfo::onClickFriendAdd, this));
        menu->addChild(itemFriend);
        {
            auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui_common/icon_friend.png", GAME_TEXT("t_ui_friend_invite"), itemFriend->getContentSize().width - 40);
            nodeContents->setPosition(Vec2(itemFriend->getContentSize().width / 2, itemFriend->getContentSize().height / 2));
            itemFriend->addChild(nodeContents);
            
            auto nodeText = (ui::CText*)nodeContents->getChildByName("WIDGET_TEXT");
            nodeText->setFontSize(24);
        }
        
        if ( FriendManager::getInstance()->IsFriend(_userIdx, _platform) == true )
        {
            itemFriend->setEnabled(false);
        }
        
        // 신고하기 버튼
        auto itemReport = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"), nullptr, CC_CALLBACK_1(PopupOtherUserInfo::onClickReport, this));
        menu->addChild(itemReport);
        {
            auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui_common/icon_report.png", GAME_TEXT("t_ui_report"), itemReport->getContentSize().width - 40);
            nodeContents->setPosition(Vec2(itemReport->getContentSize().width / 2, itemReport->getContentSize().height / 2));
            itemReport->addChild(nodeContents);
            
            auto nodeText = (ui::CText*)nodeContents->getChildByName("WIDGET_TEXT");
            nodeText->setFontSize(24);
        }
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

#pragma mark -after init
void PopupOtherUserInfo::afterInit()
{
    afterInitTopUI();
    afterInitBottomUI();
    initBadgeUI();
}

void PopupOtherUserInfo::afterInitTopUI()
{
    _lbHighFloor->setString(MafUtils::toString(_highFloor));
    _lbTotalFloor->setString(MafUtils::toString(_totalFloor));
    _lbStarDay->setString(_startDay);
    _lbTotalCostume->setString(MafUtils::toString(_totalCostumeNum));
    
}

void PopupOtherUserInfo::afterInitBottomUI()
{
    Size lySize = _lyUserInfoArea->getContentSize();
    
    auto sprHeroBg = Sprite::create("Assets/character_hero/normal/shadow2.png");
    sprHeroBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprHeroBg->setPosition(lySize.width/2.6, lySize.height - lySize.height/3.2f);
    _lyUserInfoArea->addChild(sprHeroBg);
    
    auto hero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
    hero->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    hero->setPosition(sprHeroBg->getContentSize().width - sprHeroBg->getContentSize().width/3, sprHeroBg->getContentSize().height/1.7f - 20);
    if(_skinIdx == 0 || CostumeSkinManager::getInstance()->getInfoSkin(_skinIdx) == nullptr)
    {
        hero->setImage(_weaponIdx, _mapCostume[E_COSTUME::HEAD], _mapCostume[E_COSTUME::BODY], _mapCostume[E_COSTUME::CAPE], _mapCostume[E_COSTUME::HAND], _mapCostume[E_COSTUME::FOOT]);
        hero->setMotion(_weaponIdx, _mapCostume[E_COSTUME::HEAD], _mapCostume[E_COSTUME::BODY], _mapCostume[E_COSTUME::CAPE], _mapCostume[E_COSTUME::HAND], _mapCostume[E_COSTUME::FOOT]);
    }
    else
    {
        hero->setSkin(_skinIdx);
    }
    hero->actionMove();
    sprHeroBg->addChild(hero, 3);
    
    //defense AMP
    if(_listManaAMP.size() > 0)
    {
        auto btnDefenseManaAMP = ui::Button::create("Assets/ui_common/btn_5.png");
        btnDefenseManaAMP->setScale9Enabled(true);
        btnDefenseManaAMP->setContentSize(Size(142, 50));
        btnDefenseManaAMP->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        btnDefenseManaAMP->setPosition(Vec2(lySize.width - lySize.width/3, lySize.height) + Vec2::DOWN * 5 + Vec2::LEFT * 30);
        btnDefenseManaAMP->addClickEventListener(CC_CALLBACK_1(PopupOtherUserInfo::onClickManaAMP, this));
        btnDefenseManaAMP->setZoomScale(0.005f);
        btnDefenseManaAMP->setTitleFontSize(26);
        btnDefenseManaAMP->setTitleText(GAME_TEXT("t_ui_defense_manaamp"));
        btnDefenseManaAMP->setTitleAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
        btnDefenseManaAMP->getTitleLabel()->setDimensions(90, btnDefenseManaAMP->getTitleLabel()->getContentSize().height);
        btnDefenseManaAMP->getTitleLabel()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnDefenseManaAMP->getTitleLabel()->setPosition(Vec2(btnDefenseManaAMP->getContentSize().width - 10, btnDefenseManaAMP->getContentSize().height/2));
        btnDefenseManaAMP->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _lyUserInfoArea->addChild(btnDefenseManaAMP);
        
        auto icon = Sprite::create("Assets/icon/icon_defense_power_amplifier.png");
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        icon->setPosition(-5, btnDefenseManaAMP->getContentSize().height/2);
        icon->setScale(0.8f);
        btnDefenseManaAMP->addChild(icon);
    }

    
    
    //_listManaAMP
    
    if(_infoSpirit != nullptr)
    {
        hero->setPosition(hero->getPosition() + Vec2::RIGHT * 30);
        if(auto sprSpirit = Sprite::create(_infoSpirit->getResourcePath()); sprSpirit != nullptr)
        {
            sprSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprSpirit->setScale(0.7f);
            sprSpirit->setPosition(sprHeroBg->getContentSize().width/3, sprHeroBg->getContentSize().height/2);
            sprHeroBg->addChild(sprSpirit, 2);
            
            auto action1 = MoveBy::create(1.f,Vec2(0, 10));
            auto action2 = action1->reverse();
            auto seq = Sequence::create(action1,action2, NULL);
            sprSpirit->runAction(RepeatForever::create(seq));
            
            auto sprSmeltingBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
            sprSmeltingBox->setContentSize(Size(55, 35));
            sprSmeltingBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprSmeltingBox->setPosition(10, _lyUserInfoArea->getContentSize().height - 10);
            _lyUserInfoArea->addChild(sprSmeltingBox);
            {
                auto sprIcon = Sprite::create("Assets/ui_common/icon_star1_2.png");
                sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                sprIcon->setPosition(5, sprSmeltingBox->getContentSize().height/2);
                sprSmeltingBox->addChild(sprIcon);
                
                auto lbCount = Label::createWithTTF(MafUtils::format("%02d",_infoSpirit->getCurrentSmelting()), GAME_FONT, 25);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbCount->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 3));
                lbCount->setDimensions(sprSmeltingBox->getContentSize().width - 5 - sprIcon->getContentSize().width - 8, sprSmeltingBox->getContentSize().height);
                lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                lbCount->enableBold();
                lbCount->setColor(COLOR_COIN);
                sprSmeltingBox->addChild(lbCount);
            }
            
            auto sprLevelBox = Sprite::create("Assets/ui/spirit/spirit_lv_box1_1.png");
            sprLevelBox->setContentSize(Size(55, 35));
            sprLevelBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprLevelBox->setPosition(sprSmeltingBox->getPosition() + Vec2::RIGHT * (sprSmeltingBox->getContentSize().width + 8));
            _lyUserInfoArea->addChild(sprLevelBox);
            {
                auto lbCount = Label::createWithTTF(MafUtils::format("Lv.%02d",_infoSpirit->getCurrentLv()), GAME_FONT, 25);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbCount->setPosition(sprLevelBox->getContentSize()/2);
                lbCount->setDimensions(sprLevelBox->getContentSize().width - 10, sprLevelBox->getContentSize().height);
                lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                sprLevelBox->addChild(lbCount);
            }
        }
    }
    
    {//봉타워, 강화석
        auto lbBongTowerFloor = Label::createWithTTF(GAME_TEXT("t_ui_town_tower") + " : " + std::to_string(getBuildingLv(E_TOWN::BUILDING_1)) + "F", GAME_FONT, 20);
        lbBongTowerFloor->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbBongTowerFloor->setPosition(70, lySize.height - sprHeroBg->getContentSize().height - 10);
        _lyUserInfoArea->addChild(lbBongTowerFloor);
        
        auto lbBongTowerSatelliteLv = Label::createWithTTF(GAME_TEXT("t_ui_bong_town_rokcet_title") + " : Lv." + std::to_string(getBuildingLv(E_TOWN::SATELLITE)), GAME_FONT, 20);
        lbBongTowerSatelliteLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbBongTowerSatelliteLv->setPosition(lbBongTowerFloor->getPosition() + Vec2::RIGHT * 400);
        _lyUserInfoArea->addChild(lbBongTowerSatelliteLv);
        
        auto lbDefenseHighFloor = Label::createWithTTF(GAME_TEXT("t_ui_defense_title_2") + MafUtils::format(" : B%d", _defenseHighFloor), GAME_FONT, 20);
        lbDefenseHighFloor->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbDefenseHighFloor->setPosition(lbBongTowerFloor->getPosition() + Vec2::DOWN * lbBongTowerFloor->getContentSize().height);
        _lyUserInfoArea->addChild(lbDefenseHighFloor);
        
        auto uiStoneBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiStoneBG->setScale9Enabled(true);
        uiStoneBG->setContentSize(Size(692, 125));
        uiStoneBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiStoneBG->setPosition(Vec2(_lyUserInfoArea->getContentSize().width / 2, 10));
        _lyUserInfoArea->addChild(uiStoneBG);
        {
            auto uiGroup = ui::Layout::create();
            uiGroup->setContentSize(Size(uiStoneBG->getContentSize().width - 10, uiStoneBG->getContentSize().height - 10));
            uiGroup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiGroup->setPosition(Vec2(uiStoneBG->getContentSize().width / 2, uiStoneBG->getContentSize().height / 2));
            uiGroup->setLayoutType(ui::Layout::Type::HORIZONTAL);
            uiStoneBG->addChild(uiGroup);
            
            auto uiGroupAtk = ui::Layout::create();
            uiGroupAtk->setContentSize(Size(uiGroup->getContentSize().width / 3, uiGroup->getContentSize().height));
            uiGroupAtk->setLayoutType(ui::Layout::Type::VERTICAL);
            uiGroup->addChild(uiGroupAtk);
            {
                double width = uiGroupAtk->getContentSize().width;
                double height = uiGroupAtk->getContentSize().height / 3;
                
                std::vector<E_STONE> listStone = {E_STONE::ATTACK, E_STONE::FORCE_ATTACK, E_STONE::FORCE_2_ATTACK};
                for ( int i = 0; i < listStone.size(); i++ )
                {
                    auto eStone = listStone.at(i);
                    
                    auto count = getEnchantStoneLv(eStone);
                    auto text = std::to_string(count);
              
                    if ( eStone == E_STONE::FORCE_ATTACK || eStone == E_STONE::FORCE_2_ATTACK )
                    {
                        text = MafUtils::format("%d [%d/5]", count / 6, count % 6);
                    }
                    
                    auto uiStone = (ui::Layout*)UtilsWidget::getInstance()->createIconText(StoneManager::getInstance()->getPath(eStone), text, width, 24, 0);
                    uiStone->setContentSize(Size(width, height));
                    uiGroupAtk->addChild(uiStone);
                    
                    auto uiIcon = uiStone->getChildByName<ui::ImageView*>("WIDGET_ICON");
                    uiIcon->setPositionY(uiStone->getContentSize().height / 2);
                    uiIcon->setScale(0.9);
                    
                    auto uiText = uiStone->getChildByName<ui::CText*>("WIDGET_TEXT");
                    uiText->setPositionY(uiStone->getContentSize().height / 2);
                }
            }
            
            auto uiGroupKey = ui::Layout::create();
            uiGroupKey->setContentSize(Size(uiGroup->getContentSize().width / 3, uiGroup->getContentSize().height));
            uiGroupKey->setLayoutType(ui::Layout::Type::VERTICAL);
            uiGroup->addChild(uiGroupKey);
            {
                double width = uiGroupKey->getContentSize().width;
                double height = uiGroupKey->getContentSize().height / 3;
                
                std::vector<E_STONE> listStone = {E_STONE::KEY, E_STONE::FORCE_KEY};
                for ( int i = 0; i < listStone.size(); i++ )
                {
                    auto eStone = listStone.at(i);
                    
                    auto count = getEnchantStoneLv(eStone);
                    auto text = std::to_string(count);
              
                    if ( eStone == E_STONE::FORCE_KEY || eStone == E_STONE::FORCE_2_KEY )
                    {
                        text = MafUtils::format("%d [%d/5]", count / 6, count % 6);
                    }
                    
                    auto uiStone = (ui::Layout*)UtilsWidget::getInstance()->createIconText(StoneManager::getInstance()->getPath(eStone), text, width, 24, 0);
                    uiStone->setContentSize(Size(width, height));
                    uiGroupKey->addChild(uiStone);
                    
                    auto uiIcon = uiStone->getChildByName<ui::ImageView*>("WIDGET_ICON");
                    uiIcon->setPositionY(uiStone->getContentSize().height / 2);
                    uiIcon->setScale(0.9);
                    
                    auto uiText = uiStone->getChildByName<ui::CText*>("WIDGET_TEXT");
                    uiText->setPositionY(uiStone->getContentSize().height / 2);
                }
            }
            
            auto uiGroupGold = ui::Layout::create();
            uiGroupGold->setContentSize(Size(uiGroup->getContentSize().width / 3, uiGroup->getContentSize().height));
            uiGroupGold->setLayoutType(ui::Layout::Type::VERTICAL);
            uiGroup->addChild(uiGroupGold);
            {
                double width = uiGroupGold->getContentSize().width;
                double height = uiGroupGold->getContentSize().height / 3;
                
                std::vector<E_STONE> listStone = {E_STONE::GOLD, E_STONE::FORCE_GOLD};
                for ( int i = 0; i < listStone.size(); i++ )
                {
                    auto eStone = listStone.at(i);
                    
                    auto count = getEnchantStoneLv(eStone);
                    auto text = std::to_string(count);
              
                    if ( eStone == E_STONE::FORCE_GOLD || eStone == E_STONE::FORCE_2_GOLD )
                    {
                        text = MafUtils::format("%d [%d/5]", count / 6, count % 6);
                    }
                    
                    auto uiStone = (ui::Layout*)UtilsWidget::getInstance()->createIconText(StoneManager::getInstance()->getPath(eStone), text, width, 24, 0);
                    uiStone->setContentSize(Size(width, height));
                    uiGroupGold->addChild(uiStone);
                    
                    auto uiIcon = uiStone->getChildByName<ui::ImageView*>("WIDGET_ICON");
                    uiIcon->setPositionY(uiStone->getContentSize().height / 2);
                    uiIcon->setScale(0.9);
                    
                    auto uiText = uiStone->getChildByName<ui::CText*>("WIDGET_TEXT");
                    uiText->setPositionY(uiStone->getContentSize().height / 2);
                }
            }
        }
        
    }
    
    
    for(int i = 0; i < _vecSprArtiBox.size(); ++i)
    {
        auto sprBox = _vecSprArtiBox[i];
        
        if(_isInfinite)
        {
            std::string pathBG = "Assets/ui/stat/stat_box_6.png";
            if ( _nInfiniteGrade> 7 )
            {
                pathBG = "Assets/ui/stat/stat_box_7.png";
            }
            sprBox->setTexture(pathBG);
            
            std::string pathEffect = "Assets/ui_common/box_transcendence1_2.png";
            if ( _nInfiniteGrade > 7 )
            {
                pathEffect = "Assets/ui_common/box_transcendence2_2.png";
            }
            auto sprIconEffect = Sprite::create(pathEffect);
            sprIconEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIconEffect->setPosition(sprBox->getContentSize()/2);
            sprIconEffect->setScale(0.8f);
            sprBox->addChild(sprIconEffect);
            
            auto rotate = RotateBy::create(10, 360);
            sprIconEffect->runAction(RepeatForever::create(rotate));
            
            std::string pathIcon = MafUtils::format("Assets/icon_artifact/artifact_%d.png", 104 + i);
            if ( _nInfiniteGrade > 7 )
            {
                pathIcon = MafUtils::format("Assets/icon_artifact/artifact_%d.png", 113 + i);
            }
            Sprite* sprTresureIcon = Sprite::create(pathIcon);
            if(sprTresureIcon == nullptr)
                continue;
            
            sprTresureIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprTresureIcon->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height/2 + 5);
            sprTresureIcon->setScale(0.7f);
            sprBox->addChild(sprTresureIcon);
                
            auto labelGrade = MafNode::createBmFont(MafUtils::toString(_nInfiniteGrade), GAME_BM_FONT_AL, 20);
            labelGrade->setPosition(sprBox->getContentSize().width/2,sprBox->getContentSize().height-10);
            sprBox->addChild(labelGrade);
            
            if(_vecInfiniteArtiLv.size() <= i)
                continue;
            
            auto labelLevel = MafNode::createBmFont("Lv."+MafUtils::toString(_vecInfiniteArtiLv[i]), GAME_BM_FONT_AL, 20);
            labelLevel->setPosition(sprBox->getContentSize().width/2, 23.5f);
            sprBox->addChild(labelLevel);
        }
        else
        {
            auto info = getArtiInfoFromType((E_ARTIFACT_TYPE)sprBox->getTag());
            
            Sprite* sprTresureIcon = Sprite::create(info->_strImgPath);
            
            if(info == nullptr || sprTresureIcon == nullptr)
                continue;
                
            sprTresureIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprTresureIcon->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height*2/3);
            sprBox->addChild(sprTresureIcon);
                
            auto labelGrade = MafNode::createBmFont(MafUtils::toString(info->_generation), GAME_BM_FONT_AL, 26);
            labelGrade->setPosition(sprBox->getContentSize().width-15,sprBox->getContentSize().height-15);
            labelGrade->setColor(COLOR_COIN);
            sprBox->addChild(labelGrade);
                       
            auto labelLevel = MafNode::createBmFont("Lv."+MafUtils::toString(info->_level), GAME_BM_FONT_AL, 20);
            labelLevel->setPosition(sprBox->getContentSize().width/2,15);
            labelLevel->setColor(COLOR_COIN);
            sprBox->addChild(labelLevel);
        }
    }
}

#pragma mark - network

void PopupOtherUserInfo::requestUserInfo()
{
    PopupLoading::show();
    std::string url = "/user/save/info";
    //데일리 리셋 타입 : 0
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", _userIdx);
    req->setData("_platform",_platform);
     
     MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
         std::string dd = data;
         responsUserInfo(response,dd);
     };
     req->send(cb);
}

void PopupOtherUserInfo::responsUserInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    bool bError = false;
    
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    
    if(jsonParser.HasParseError())
    {
        bError = true;
    }

    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        hide();
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
       
    if(result != 1)
    {
        if(result == 0)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_11"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            hide();
            return;
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_7"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
                Director::getInstance()->end();
            });
            popup->addButton(GAME_TEXT("t_ui_menu_contactus"), false, [=](Ref* sender){

                std::string strGameId = "";
                std::string strVersion = "";
                std::string strAuth = "";
                
                if ( AccountManager::getInstance()->getUserID().length() != 0 )
                    strGameId = AccountManager::getInstance()->getUserID();
                else
                    strGameId = GAME_TEXT("t_ui_error_1");
                
                strVersion = MafNative::GetGameVersion();
                strAuth = MafAes256::Encrypt(strGameId);
                
                std::string strSubject = GAME_TEXT("t_ui_menu_msg_13");
                std::string strBody = GAME_TEXTFORMAT("t_ui_menu_msg_14", strGameId.c_str(), strVersion.c_str(), MafNative::getDeviceModel().c_str(), MafNative::getDeviceSystemVersion().c_str(), strAuth.c_str());
                
                MafNative::SendEmail(EMAIL_HELP, strSubject.c_str(), strBody.c_str());
                
            }, false);
            popup->setHideBackKey(false);
            popup->setHideAll(false);
            popup->show();
            return;
        }
    }
    /*
     his_data : 시작일
     d_high_stage : 최고 층
     d_total_stage : 누적층
     k_sk_a : 스킨
     k_gh : 무기
     
     k_opq : 보유중인 코스튬
     k_fg : 보물
     k_rst : 코스튬
     
     k_fg, k_sk_a, k_rst, k_gh 는 세이브 파일에 없는 경우 보내지 않음
     
     k_fe : 봉식타워 키 값
     k_wds : 서버 강화석
     k_wdss : 클라 강화석
     
     */
    if(jsonParser.HasMember("_weapon_spirit_info"))
    {
        //{"useridx": 1412212, "idx": 24, "level": 10, "smelting": 2, "count": 4, "wear": 1}
        auto objSpiritData = jsonParser["_weapon_spirit_info"].GetObject();
        int nSpiritIdx = objSpiritData["idx"].GetInt();
        int nSpiritLv = objSpiritData["level"].GetInt();
        int nSpiritSmelting = objSpiritData["smelting"].GetInt();
        
        _infoSpirit = new InfoWeaponSpirit();
        _infoSpirit->setIdx(nSpiritIdx);
        _infoSpirit->setCurrentLv(nSpiritLv);
        _infoSpirit->setCurrentSmelting(nSpiritSmelting);
    }
    auto objData = jsonParser["_data"].GetObject();
    
    if(jsonParser.HasMember("_totalday"))
    {
        _startDay = std::to_string(jsonParser["_totalday"].GetInt());
        auto offset = _startDay.find(" ");
        if(offset != std::string::npos)
        {
            _startDay = _startDay.substr(0, offset);
        }
    }
    
    _highFloor = std::atoi(objData["d_high_stage"].GetString());
    _totalFloor = std::atoi(objData["d_total_stage"].GetString());
    
    if(objData.HasMember("k_sk_a"))
       _skinIdx = std::atoi(objData["k_sk_a"].GetString());
    else
       _skinIdx = 0;
    
    if(objData.HasMember("k_gh"))
        _weaponIdx = std::atoi(objData["k_gh"].GetString());
    else
        _weaponIdx = 0;
    
    std::string strNowCostume = "";
    
    if(objData.HasMember("k_rst"))
        strNowCostume = objData["k_rst"].GetString();
        
    std::string strArtifact = "";
    if(objData.HasMember("k_fg"))
        strArtifact = objData["k_fg"].GetString();
    
    std::string strTotalCostume = "";
    if(objData.HasMember("k_opq"))
        strTotalCostume = objData["k_opq"].GetString();
    
    if(objData.HasMember("df_h_f"))
        _defenseHighFloor = std::atoi(objData["df_h_f"].GetString());
    
    if(jsonParser.HasMember("_mana_amp_list"))
    {
        for ( int i = 0; i < jsonParser["_mana_amp_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_mana_amp_list"][i];
            
            int idx = jsonValue["index"].GetInt();
            
            std::vector<int> stoneList;
            for(int j = 0; j < jsonValue["amp_list"].Size(); ++j)
            {
                stoneList.push_back(jsonValue["amp_list"][j].GetInt());
            }
            
            auto data = InfoDefenseManaAMP::create();
            data->setIndex(idx);
            data->setStoneList(stoneList);
            _listManaAMP.pushBack(data);
        }
    }
    
    if(strNowCostume == "")
    {
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++)
        {
            _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)1));
        }
    }
    else
    {
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++)
        {
            auto offset = strNowCostume.find(",");
            if(offset != std::string::npos){
                auto result = strNowCostume.substr(0,offset);
                int ret = std::atoi(result.c_str());
                if((E_COSTUME)i != E_COSTUME::JEWEL)
                {
                    if(ret == 0)
                        _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)1));
                    else
                        _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                }
                
                strNowCostume = strNowCostume.substr(offset+1);
            }
            else{
                int ret = std::atoi(strNowCostume.c_str());
                if((E_COSTUME)i != E_COSTUME::JEWEL)
                {
                    if(ret == 0)
                        _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)1));
                    else
                        _mapCostume.insert(std::make_pair((E_COSTUME)i, (E_COSTUME_IDX)ret));
                }
                break;
            }
        }
    }
    
    std::map<E_ARTIFACT, int> mapArtifact;
    if(!strArtifact.empty())
    {
        for(int i=1; i <= C_COUNT_ARTIFACT; i++)
        {
            auto offset = strArtifact.find(",");
            if(offset != std::string::npos)
            {
                auto result = strArtifact.substr(0,offset);
                int ret = std::atoi(result.c_str());
                mapArtifact.insert(std::make_pair((E_ARTIFACT)i, ret));
                strArtifact = strArtifact.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(strArtifact.c_str());
                mapArtifact.insert(std::make_pair((E_ARTIFACT)i, ret));
                break;
            }
        }
    }
    
    if(mapArtifact.size() > 0)
    {
        for(int i = 0; i < E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT; ++i)
        {
            for ( auto tempInfo : mapArtifact )
            {
                auto artiOriginInfo = DataManager::GetArtifactInfo(tempInfo.first);
                
                if(artiOriginInfo->getIdx() >= E_ARTIFACT::ARTI_103_BERSERKER_SPRIT)
                    continue;
                
                auto userArtiInfo = getArtiInfoFromType(artiOriginInfo->getType());
                
                if((artiOriginInfo->getType() != (E_ARTIFACT_TYPE)i) || userArtiInfo == nullptr)
                    continue;
                
                
                if(userArtiInfo == nullptr)
                    continue;
                
                if(userArtiInfo->_generation < artiOriginInfo->getGeneration() && tempInfo.second >= 1)
                {
                    userArtiInfo->_generation = artiOriginInfo->getGeneration();
                    userArtiInfo->_strImgPath = artiOriginInfo->getIconPath();
                    userArtiInfo->_level = 0;
                        
                    if(userArtiInfo->_level < tempInfo.second)
                    {
                        userArtiInfo->_level = tempInfo.second;
                    }
                }
                
                if(userArtiInfo->_generation == 0)
                {
                    userArtiInfo->_generation = artiOriginInfo->getGeneration();
                    userArtiInfo->_strImgPath = artiOriginInfo->getIconPath();
                    userArtiInfo->_level = 0;
                }
                
                
            }
            
        }
    }
    if(!strTotalCostume.empty())
    {
        for(int i=1; i<=C_COUNT_COSTUME;i++)
        {
            for(int j=1; j<=C_COUNT_COSTUME_TYPE; j++)
            {
                auto offset = strTotalCostume.find(",");
                if(offset != std::string::npos)
                {
                    auto result = strTotalCostume.substr(0,offset);
                    int ret = std::atoi(result.c_str());
                    
                    if(ret != 0)
                        _totalCostumeNum++;
                    
                    strTotalCostume = strTotalCostume.substr(offset+1);
                }
                else
                {
                    int ret = std::atoi(strTotalCostume.c_str());
                    strTotalCostume.clear();
                    
                    if(ret != 0)
                        _totalCostumeNum++;
                    break;
                }
            }
            if (strTotalCostume.length() == 0) {
                break;
            }
        }
    }
    
    auto badgeList = jsonParser["_badge_list"].GetArray();
    
    for(int i = 0; i< badgeList.Size(); ++i)
    {
        auto badgeInfo = badgeList[i].GetObject();
        
        auto idx = badgeInfo["idx"].GetInt();
        auto badgeIdx = badgeInfo["badgeidx"].GetInt();
        auto seasonIdx = badgeInfo["seasonidx"].GetInt();
        auto created = badgeInfo["created"].GetInt64();
        
        auto info = new InfoBadgeHave();
        info->setIdx(idx);
        info->setIdxBadge(badgeIdx);
        info->setIdxSeason(seasonIdx);
        info->setCreated(created);
        _vecBadge.pushBack(info);
    }
    
    if(objData.HasMember("k_ifat"))
    {
        std::string strInfinite = objData["k_ifat"].GetString();
        
        
        rapidjson::Document jsonInfiniteArtiParser;
        jsonInfiniteArtiParser.ParseInsitu<0>((char*) strInfinite.c_str());

        if(jsonInfiniteArtiParser.HasParseError() == false)
        {
            _nInfiniteGrade = jsonInfiniteArtiParser["cur_grade"].GetInt();

            auto nInfiniteAtkLv = jsonInfiniteArtiParser["atk_lv"].GetInt();
            auto nInfiniteCriLv = jsonInfiniteArtiParser["cri_lv"].GetInt();
            auto nInfiniteQuestLv = jsonInfiniteArtiParser["quest_lv"].GetInt();
            auto nInfiniteKillLv = jsonInfiniteArtiParser["kill_lv"].GetInt();
            auto nInfiniteCrossLv = jsonInfiniteArtiParser["cross_lv"].GetInt();
            _isInfinite = jsonInfiniteArtiParser["isActive"].GetBool();
            
            
            _vecInfiniteArtiLv.push_back(nInfiniteAtkLv);
            _vecInfiniteArtiLv.push_back(nInfiniteCriLv);
            _vecInfiniteArtiLv.push_back(nInfiniteQuestLv);
            _vecInfiniteArtiLv.push_back(nInfiniteKillLv);
            _vecInfiniteArtiLv.push_back(nInfiniteCrossLv);
        }
    }
    /*
     
     k_fe : 봉식타워 키 값
     k_wds : 서버 강화석
     k_wdss : 클라 강화석
     */
    
    
    if(objData.HasMember("k_fe"))
    {
        std::string strData = "";
        strData = objData["k_fe"].GetString();
        if(!strData.empty())
        {
            for(int i=1; i <= C_COUNT_BUILDING; i++)
            {
                auto offset = strData.find(",");
                if(offset != std::string::npos)
                {
                    auto result = strData.substr(0,offset);
                    int ret = std::atoi(result.c_str());
                    setBuildingLv((E_TOWN)i,ret);
                    strData = strData.substr(offset+1);
                }
                else
                {
                    int ret = std::atoi(strData.c_str());
                    setBuildingLv((E_TOWN)i,ret);
                    break;
                }
            }
        }
    }
    if(objData.HasMember("k_wds"))
    {
        std::string strData = "";
        strData = objData["k_wds"].GetString();
        
        if(!strData.empty())
        {
            for(int i=1; i <= C_COUNT_ENCHANT_STONE; i++)
            {
                auto offset = strData.find(",");
                if(offset != std::string::npos)
                {
                    auto result = strData.substr(0,offset);
                    int ret = std::atoi(result.c_str());
                    setEnchantStoneLv((E_STONE)i, ret);
                    strData = strData.substr(offset+1);
                }
                else
                {
                    int ret = std::atoi(strData.c_str());
                    setEnchantStoneLv((E_STONE)i, ret);
                    break;
                }
            }
        }
    }
    
    if(objData.HasMember("k_wdss"))
    {
        std::string strData = "";
        strData = objData["k_wdss"].GetString();
        
        if(!strData.empty())
        {
            for(int i=1; i <= C_COUNT_ENCHANT_STONE; i++)
            {
                auto offset = strData.find(",");
                if(offset != std::string::npos)
                {
                    auto result = strData.substr(0,offset);
                    int ret = std::atoi(result.c_str());
                    setEnchantStoneLv((E_STONE)i, ret);
                    strData = strData.substr(offset+1);
                }
                else
                {
                    int ret = std::atoi(strData.c_str());
                    setEnchantStoneLv((E_STONE)i, ret);
                    break;
                }
            }
        }
    }
    
    
    
    afterInit();
    
}

#pragma mark - utils

void PopupOtherUserInfo::onClick(Ref* sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
}

void PopupOtherUserInfo::onClickReport(Ref* sender)
{
    auto popup = PopupUserReport::create(E_REPORT_TYPE::NONE, _userIdx, _nick, _platform, "");
    popup->show();
}

void PopupOtherUserInfo::onClickFriendAdd(Ref* sender)
{
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendAddRequest(CC_CALLBACK_2(PopupOtherUserInfo::callbackFriednAddRequest,this), _userIdx, _platform, 3);
}

void PopupOtherUserInfo::onClickManaAMP(Ref* sender)
{
    auto popup = PopupOtherUserManaAMPList::create(_listManaAMP);
    popup->show();
}

tagArtiInfo* PopupOtherUserInfo::getArtiInfoFromType(E_ARTIFACT_TYPE type)
{
    tagArtiInfo* resultInfo = nullptr;
    
    
    for(auto info : _vecArtifact)
    {
        if(info->_type == type)
        {
            resultInfo = info;
            break;
        }
    }
    
    return resultInfo;
}

#pragma mark - callback
void PopupOtherUserInfo::callbackFriednAddRequest(bool bResult, int nResult)
{
    //
    PopupLoading::hide();

    //
    if ( bResult == false )
    {

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    /*
     0 # 해당 친구가 친구 많음
     1 # 친구 추가 성공
     3 # 이미 친구요청을 보냄
     4 # 친구의 친구대기리스트 꽉참
     5 # 이미 친구 임
     6 # 자신은 친구가 될수없음
     */
    std::string strText = "";
    switch (nResult)
    {
        case 0: strText = GAME_TEXT("t_ui_friend_msg_14");      break;
        case 1: strText = GAME_TEXT("t_ui_friend_msg_29");       break;
        case 3: strText = GAME_TEXT("t_ui_friend_msg_30");      break;
        case 4: strText = GAME_TEXT("t_ui_friend_msg_3");       break;
        case 5: strText = GAME_TEXT("t_ui_friend_msg_5");       break;
        case 6: strText = GAME_TEXT("t_ui_friend_msg_6");       break;
        case 7: strText = GAME_TEXT("t_ui_friend_msg_31");       break;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}

void PopupOtherUserInfo::setEnchantStoneLv(E_STONE eType, int nLv)
{
    for(int i = 0; i < _vecEnchantStoneLv.size(); ++i)
    {
        if(_vecEnchantStoneLv[i].first == eType)
            _vecEnchantStoneLv[i].second += nLv;
    }
}

int PopupOtherUserInfo::getEnchantStoneLv(E_STONE eType)
{
    int result = 0;
    for(auto obj : _vecEnchantStoneLv)
    {
        if(obj.first == eType)
        {
            result = obj.second;
            break;
        }
    }
    return result;
}

void PopupOtherUserInfo::setBuildingLv(E_TOWN eType, int nLv)
{
    for(int i = 0; i < _vecBuildingLv.size(); ++i)
    {
        if(_vecBuildingLv[i].first == eType)
            _vecBuildingLv[i].second += nLv;
    }
}

int PopupOtherUserInfo::getBuildingLv(E_TOWN eType)
{
    int result = 0;
    for(auto obj : _vecBuildingLv)
    {
        if(obj.first == eType)
        {
            result = obj.second;
            break;
        }
    }
    return result;
}
