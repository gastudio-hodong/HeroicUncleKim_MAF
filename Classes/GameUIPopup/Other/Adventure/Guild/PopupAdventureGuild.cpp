//
//  PopupAdventureGuild.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#include "PopupAdventureGuild.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Adventure/Guild/Layer/UILayerAdventureGuildTop.h"
#include "GameUIPopup/Other/Adventure/Guild/Layer/UILayerAdventureGuildBottom.h"

#include "ManagerGame/AdventureManager.h"

PopupAdventureGuild* PopupAdventureGuild::create()
{
    PopupAdventureGuild* pRet = new(std::nothrow) PopupAdventureGuild();
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

PopupAdventureGuild::PopupAdventureGuild(void) :
_lyContainerParent(nullptr),
_lyContainerTop(nullptr),
_lyContainerBottom(nullptr)
{
    
}

PopupAdventureGuild::~PopupAdventureGuild(void)
{
    AdventureManager::getInstance()->setIsAdventurePopupShow(false);
}

bool PopupAdventureGuild::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    _bHideBackKey = false;
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}

void PopupAdventureGuild::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    AdventureManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupAdventureGuild::callbackInfo, this));
}

#pragma mark - override
void PopupAdventureGuild::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP-1);
}
void PopupAdventureGuild::hide()
{
    PopupBase::hide();
}
void PopupAdventureGuild::show()
{
    PopupBase::show();
    
    AdventureManager::getInstance()->setIsAdventurePopupShow(true);
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
#pragma mark - init
void PopupAdventureGuild::initVar()
{
    double nWidth = 750;
    double nHeight = getContentSize().height;
    if ( nHeight > 1332 )
        nHeight = 1332;
    
    // size
    _spriteContainer->setContentSize( Size(nWidth, nHeight) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
}

void PopupAdventureGuild::initUi()
{
    _lyContainerParent = Layer::create();
    _lyContainerParent->setContentSize(Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75));
    _lyContainerParent->setIgnoreAnchorPointForPosition(false);
    _lyContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyContainerParent->setPosition( _spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_lyContainerParent);
    
    // top layer
    _lyContainerTop = LayerColor::create(Color4B::BLACK);
    _lyContainerTop->setContentSize(Size(_lyContainerParent->getContentSize().width, 575));
    _lyContainerTop->setIgnoreAnchorPointForPosition(false);
    _lyContainerTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyContainerTop->setPosition(_lyContainerParent->getContentSize().width / 2, _lyContainerParent->getContentSize().height);
    _lyContainerParent->addChild(_lyContainerTop);
    
    // content layer
    _lyContainerBottom = LayerColor::create(Color4B::BLACK);
    _lyContainerBottom->setContentSize( Size(_lyContainerTop->getContentSize().width,
                                              _lyContainerParent->getContentSize().height - _lyContainerTop->getContentSize().height) );
    _lyContainerBottom->setIgnoreAnchorPointForPosition(false);
    _lyContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyContainerBottom->setPosition( _lyContainerTop->getPosition() + Vec2::DOWN * _lyContainerTop->getContentSize().height);
    _lyContainerParent->addChild(_lyContainerBottom);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_info_title_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureGuild::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiTop();
    uiBottom();
}

#pragma mark - ui
void PopupAdventureGuild::uiTop()
{
    auto lyTemp = _lyContainerTop->getChildByName("TOP");

    if(lyTemp == nullptr)
    {
        lyTemp = UILayerAdventureGuildTop::create(_lyContainerTop->getContentSize());
        lyTemp->setName("TOP");
        ((UILayerAdventureGuildTop*)lyTemp)->setCallbackHide(CC_CALLBACK_0(PopupAdventureGuild::callbackHide, this));
        _lyContainerTop->addChild(lyTemp);
        ((UILayerAdventureGuildTop*)lyTemp)->showButtonForceTutorial();
    }
    ((UILayerAdventureGuildTop*)lyTemp)->refreshUI();
}

void PopupAdventureGuild::uiBottom()
{
    auto lyTemp = _lyContainerBottom->getChildByName("BOTTOM");
    
    if(lyTemp == nullptr)
    {
        lyTemp = UILayerAdventureGuildBottom::create(_lyContainerBottom->getContentSize());
        lyTemp->setName("BOTTOM");
        ((UILayerAdventureGuildBottom*)lyTemp)->setCallbackHide(CC_CALLBACK_0(PopupAdventureGuild::callbackHide, this));
        _lyContainerBottom->addChild(lyTemp);
        ((UILayerAdventureGuildBottom*)lyTemp)->showButtonForceTutorial();
    }
    ((UILayerAdventureGuildBottom*)lyTemp)->refreshUI();
}

#pragma mark - draw

#pragma mark - click
void PopupAdventureGuild::onClickClose(const Ref* sender)
{
    hide();
}

#pragma mark - callback
void PopupAdventureGuild::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        hide();
        return;
    }
    
    
    uiTop();
    uiBottom();
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void PopupAdventureGuild::callbackHide()
{
    hide();
}
