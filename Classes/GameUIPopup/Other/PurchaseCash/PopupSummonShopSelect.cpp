//
//  PopupSummonShopSelect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/01.
//

#include "PopupSummonShopSelect.h"

#include "GameUI/UI/UIReddot.h"

#include "GameObject/InfoItems.h"
#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Other/Defense/Shop/PopupDefenseShopDefault.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/UserInfoManager.h"


USING_NS_CC;

PopupSummonShopSelect* PopupSummonShopSelect::create()
{
    PopupSummonShopSelect* pRet = new(std::nothrow) PopupSummonShopSelect();
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

PopupSummonShopSelect::PopupSummonShopSelect():
_lyTop(nullptr),
_lyMain(nullptr)
{
}

PopupSummonShopSelect::~PopupSummonShopSelect(void)
{
}

bool PopupSummonShopSelect::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    initVar();
    initUI();
    
    return true;
}

void PopupSummonShopSelect::initVar()
{
    Size size = Size(656, 522);
    
    _spriteContainer->setContentSize(size);
}

void PopupSummonShopSelect::initUI()
{
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raid_bt_shop"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSummonShopSelect::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 80));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTop);
    
    _lyMain = LayerColor::create();
    _lyMain->setContentSize(Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTop->getContentSize().height));
    _lyMain->setIgnoreAnchorPointForPosition(false);
    _lyMain->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyMain->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _spriteContainer->addChild(_lyMain);
    
    uiTop();
    uiMain();
}

#pragma mark - ui
void PopupSummonShopSelect::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto box = ui::Scale9Sprite::create("Assets/ui/race_event/race_info_hidden_title.png");
    box->setPosition(_lyTop->getContentSize()/2);
    box->getTexture()->setTexParameters(texParams);
    box->setScale(1.5f);
    _lyTop->addChild(box);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_shop_summon_select_1"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setPosition(_lyTop->getContentSize()/2);
    lbText->setDimensions(_lyTop->getContentSize().width - 300, lbText->getContentSize().height);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _lyTop->addChild(lbText);
}

void PopupSummonShopSelect::uiMain()
{
    _lyMain->removeAllChildren();
    
    auto btnSpirit = ui::Button::create("Assets/ui/shop/shop_spirit_recall.png");
    btnSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnSpirit->setPosition(Vec2(_lyMain->getContentSize()/2) + Vec2::LEFT * 20 + Vec2::UP * 10);
    btnSpirit->setZoomScale(0.05f);
    btnSpirit->addClickEventListener(CC_CALLBACK_1(PopupSummonShopSelect::onClickRaidShop, this));
    _lyMain->addChild(btnSpirit);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_bt"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(btnSpirit->getContentSize().width/2, 15);
        lbText->setDimensions(btnSpirit->getContentSize().width - 10, lbText->getContentSize().height);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnSpirit->addChild(lbText);
        
        bool bEnter = false;
        
        int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
        if ( nRevival > 1 )
        {
            bEnter = true;
        }
        
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto lock = Sprite::create("Assets/ui/raid/raid_shop/btn_spirit__box_icon_slot1_2.png");
            lock->setPosition(btnSpirit->getContentSize()/2);
            lock->getTexture()->setTexParameters(texParams);
            lock->setScale(1.65f);
            btnSpirit->addChild(lock);
            
            btnSpirit->setColor(Color3B(100,100,100));
            btnSpirit->setEnabled(false);
        }
    }
    
    auto btnDefense = ui::Button::create("Assets/ui/shop/shop_defense_recall.png");
    btnDefense->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnDefense->setPosition(Vec2(_lyMain->getContentSize()/2) + Vec2::RIGHT * 20 + Vec2::UP * 10);
    btnDefense->setZoomScale(0.05f);
    btnDefense->addClickEventListener(CC_CALLBACK_1(PopupSummonShopSelect::onClickDefenseShop, this));
    _lyMain->addChild(btnDefense);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_defense_shop_title_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(btnDefense->getContentSize().width/2, 15);
        lbText->setDimensions(btnDefense->getContentSize().width - 10, lbText->getContentSize().height);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnDefense->addChild(lbText);
        
        bool bEnter = false;
        
        if (DefenseManager::getInstance()->isFirstGift())
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto lock = Sprite::create("Assets/ui/raid/raid_shop/btn_spirit__box_icon_slot1_2.png");
            lock->setPosition(btnSpirit->getContentSize()/2);
            lock->getTexture()->setTexParameters(texParams);
            lock->setScale(1.65f);
            btnDefense->addChild(lock);
            
            btnDefense->setColor(Color3B(100,100,100));
            btnDefense->setEnabled(false);
        }
    }
}

#pragma mark - click
void PopupSummonShopSelect::onClickClose(Ref* sender)
{
    hide();
}
void PopupSummonShopSelect::onClickDefenseShop(Ref* sender)
{
    auto node = (Node*) sender;
    auto popup = PopupDefenseShopDefault::create();
    popup->show();
    hide();
}
void PopupSummonShopSelect::onClickRaidShop(Ref* sender)
{
    auto node = (Node*) sender;
    
    auto popup = PopupRaidShopDefault::create();
    popup->setCloseCondition(false);
    popup->show();
    
    hide();
}
#pragma mark - utils
