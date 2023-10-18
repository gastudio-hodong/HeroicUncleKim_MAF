//
//  PopupRaidDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/28.
//

#include "PopupRaidDefault.h"

#include "GameObject/InfoQuest.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidReward.h"
#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidStatus.h"
#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidMain.h"
#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidParent.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

PopupRaidDefault* PopupRaidDefault::create()
{
    PopupRaidDefault* pRet = new(std::nothrow) PopupRaidDefault();
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

PopupRaidDefault::PopupRaidDefault(void) :
_lyContainerParent(nullptr),
_lyContainerTop(nullptr),
_lyContainerCurrency(nullptr),
_lyContainerContent(nullptr),
_lyContainerBottom(nullptr)
{
    
}

PopupRaidDefault::~PopupRaidDefault(void)
{
    
}

bool PopupRaidDefault::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::RAID_STAT, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::RAID_ALL, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::RAID_ATTACK, this);
    
    return true;
}

void PopupRaidDefault::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    NewRaidManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupRaidDefault::callbackInfo, this));
}

#pragma mark - override
void PopupRaidDefault::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    switch(eType)
    {
        case E_REFRESH::RAID_STAT :
        {
            uiStatus();
        }break;
        case E_REFRESH::RAID_ATTACK :
        {
            uiMain();
        }break;
        case E_REFRESH::RAID_ALL :
        {
            uiMain();
            uiStatus();
        }
    }
}
void PopupRaidDefault::hide()
{
    PopupBase::hide();
}

#pragma mark - init
void PopupRaidDefault::initVar()
{
    
}

void PopupRaidDefault::initUi()
{
    _lyContainerParent = Layer::create();
    _lyContainerParent->setContentSize( getContentSize() );
    _lyContainerParent->setIgnoreAnchorPointForPosition(false);
    _lyContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _lyContainerParent->setPosition( getContentSize().width / 2, getContentSize().height / 2 );
    addChild(_lyContainerParent);
    
    // top layer
    _lyContainerTop = LayerColor::create(Color4B::RED);
    _lyContainerTop->setContentSize(Size(_lyContainerParent->getContentSize().width, _lyContainerParent->getContentSize().height/2 - UserInfoManager::getInstance()->_fHeightIphoneX));
    _lyContainerTop->setIgnoreAnchorPointForPosition(false);
    _lyContainerTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyContainerTop->setPosition(_lyContainerParent->getContentSize().width / 2, _lyContainerParent->getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    _lyContainerParent->addChild(_lyContainerTop);
    
    // currency layer
    _lyContainerCurrency = LayerColor::create(Color4B::BLACK);
    _lyContainerCurrency->setContentSize( Size(_lyContainerTop->getContentSize().width, 60) );
    _lyContainerCurrency->setIgnoreAnchorPointForPosition(false);
    _lyContainerCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyContainerCurrency->setPosition( _lyContainerTop->getPosition() + Vec2::DOWN * _lyContainerTop->getContentSize().height);
    _lyContainerParent->addChild(_lyContainerCurrency);
    {
        //
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::COSTUME_COIN);
        list.push_back(E_ITEMS::MATERIAL);
        list.push_back(E_ITEMS::HONOR);
        list.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);

        auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
        uiCurrency->setPosition(Vec2(_lyContainerCurrency->getContentSize().width / 2, _lyContainerCurrency->getContentSize().height / 2));
        _lyContainerCurrency->addChild(uiCurrency, E_ZORER::UI+1);
    }
    
    // content layer
    _lyContainerContent = Layer::create();
    _lyContainerContent->setContentSize( Size(_lyContainerTop->getContentSize().width,
                                            _lyContainerParent->getContentSize().height - _lyContainerTop->getContentSize().height - 165) );
    _lyContainerContent->setIgnoreAnchorPointForPosition(false);
    _lyContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyContainerContent->setPosition( _lyContainerCurrency->getPosition() + Vec2::DOWN * _lyContainerCurrency->getContentSize().height);
    _lyContainerParent->addChild(_lyContainerContent);
    
    // button layer
    _lyContainerBottom = LayerColor::create(Color4B(19, 26, 33, 255));
    _lyContainerBottom->setContentSize(Size(_lyContainerParent->getContentSize().width, 105));
    _lyContainerBottom->setIgnoreAnchorPointForPosition(false);
    _lyContainerBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyContainerBottom->setPosition(_lyContainerParent->getContentSize().width / 2, 0);
    _lyContainerParent->addChild(_lyContainerBottom);
    
    NewRaidManager::getInstance()->getQuestList();
    
    //
    uiMain();
    uiButton();
    uiStatus();
}

#pragma mark - ui
void PopupRaidDefault::uiMain()
{
    auto lyTemp = _lyContainerTop->getChildByName("MAIN");
    
    if(lyTemp == nullptr)
    {
        lyTemp = UILayerRaidMain::create(_lyContainerTop->getContentSize());
        lyTemp->setName("MAIN");
        ((UILayerRaidMain*)lyTemp)->setCallbackHide(CC_CALLBACK_0(PopupRaidDefault::callbackHide, this));
        _lyContainerTop->addChild(lyTemp);
    }
    ((UILayerRaidParent*)lyTemp)->refreshUI();
}

void PopupRaidDefault::uiStatus()
{
    auto lyTemp = _lyContainerContent->getChildByName("STAT");
    
    if(lyTemp == nullptr)
    {
        lyTemp = UILayerRaidStatus::create(_lyContainerContent->getContentSize());
        lyTemp->setName("STAT");
        ((UILayerRaidStatus*)lyTemp)->setCallbackHide(CC_CALLBACK_0(PopupRaidDefault::callbackHide, this));
        _lyContainerContent->addChild(lyTemp);
        
        auto lyManin = _lyContainerTop->getChildByName("MAIN");
        
        if(lyManin != nullptr)
        {
            ((UILayerRaidStatus*)lyTemp)->setCallbackLevelup(CC_CALLBACK_0(UILayerRaidMain::callbackLevelup, ((UILayerRaidMain*)lyManin)));
        }
    }
    ((UILayerRaidParent*)lyTemp)->refreshUI();
}

void PopupRaidDefault::uiButton()
{
    _lyContainerBottom->removeAllChildren();
    
    auto btnShop = ui::Button::create("Assets/ui/raid/btn_on_raid_2_1.png");
    btnShop->setScale9Enabled(true);
    btnShop->setContentSize(Size(305, btnShop->getContentSize().height));
    btnShop->addClickEventListener(CC_CALLBACK_1(PopupRaidDefault::onClickShop, this));
    btnShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnShop->setZoomScale(0.05f);
    btnShop->setPosition(_lyContainerBottom->getContentSize()/2);
    _lyContainerBottom->addChild(btnShop);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raid_bt_shop"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnShop->getContentSize()/2);
        lbText->setDimensions(btnShop->getContentSize().width - 150, lbText->getContentSize().height);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnShop->addChild(lbText);
        
        auto sprIcon = Sprite::create("Assets/ui/raid/menu_btn_raid_shop.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprIcon->setPosition(lbText->getPosition() + Vec2::LEFT * (lbText->getContentSize().width/2 - 15));
        btnShop->addChild(sprIcon);
    }
    
    auto btnClose = ui::Button::create("Assets/ui/raid/btn_on_raid_1_1.png");
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupRaidDefault::onClickClose, this));
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnClose->setZoomScale(0.05f);
    btnClose->setPosition(btnShop->getPosition() + Vec2::LEFT * (btnShop->getContentSize().width/2));
    _lyContainerBottom->addChild(btnClose);
    {
        auto sprIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_normal.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(-10, btnClose->getContentSize().height/2 - 10);
        btnClose->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raid_bt_exit"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprIcon->getPosition().x + sprIcon->getContentSize().width - 10, btnClose->getContentSize().height/2);
        lbText->setDimensions(btnClose->getContentSize().width - sprIcon->getContentSize().width , lbText->getContentSize().height);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        btnClose->addChild(lbText);
    }
    
    
    
    auto btnSpirit = ui::Button::create("Assets/ui/raid/btn_on_raid_1_2.png");
    btnSpirit->addClickEventListener(CC_CALLBACK_1(PopupRaidDefault::onClickSpirit, this));
    btnSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnSpirit->setZoomScale(0.05f);
    btnSpirit->setPosition(btnShop->getPosition() + Vec2::RIGHT * (btnShop->getContentSize().width/2));
    _lyContainerBottom->addChild(btnSpirit);
    {
        auto sprIcon = Sprite::create("Assets/ui/raid/menu_btn_spirit.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(-10, btnSpirit->getContentSize().height/2);
        btnSpirit->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_weapon_spirit_1"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprIcon->getPosition().x + sprIcon->getContentSize().width - 10, btnSpirit->getContentSize().height/2);
        lbText->setDimensions(btnSpirit->getContentSize().width - sprIcon->getContentSize().width , lbText->getContentSize().height);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        btnSpirit->addChild(lbText);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(btnSpirit->getContentSize().width - 15, btnSpirit->getContentSize().height - 15);
        btnSpirit->addChild(uiReddot);
        uiReddot->setVisible(false);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            auto uiReddot = (UIReddot*)sender;
            
            if(WeaponSpiritManager::getInstance()->isReddotCondition())
            {
                uiReddot->setVisible(true);
            }
            else
            {
                uiReddot->setVisible(false);
            }
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
        
    }
}

#pragma mark - draw

#pragma mark - click
void PopupRaidDefault::onClickClose(Ref *sender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    auto item = (MafNode::MafMenuItemSprite*)sender;
    item->setEnabled(false);
    
    hide();
}

void PopupRaidDefault::onClickShop(Ref *sender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    
    auto popup = PopupRaidShopDefault::create();
    popup->show();
}

void PopupRaidDefault::onClickSpirit(Ref *sender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    auto popup = PopupListWeaponSpirit::create();
    popup->show();
}

#pragma mark - callback
void PopupRaidDefault::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 8 )
        {
            strMessageKey = "t_ui_new_raid_error_1";
        }
        else if( nResult == 9 )
        {
            strMessageKey = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    auto listReward = NewRaidManager::getInstance()->getListIntroReward();
    
    if(listReward.size() > 0)
    {
        auto popup = PopupRaidReward::create(listReward);
        popup->show();
        NewRaidManager::getInstance()->clearListIntroReward();
    }
    
    uiMain();
    uiStatus();
    uiButton();
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void PopupRaidDefault::callbackHide()
{
    hide();
}
