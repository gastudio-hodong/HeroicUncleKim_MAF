//
//  PopupQuit.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupQuit.h"

#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupQuit* PopupQuit::create()
{
    PopupQuit* pRet = new(std::nothrow) PopupQuit();
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

PopupQuit::PopupQuit(void) :

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupQuit::~PopupQuit(void)
{
    
}

bool PopupQuit::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupQuit::initVar()
{

}

void PopupQuit::initUi()
{
    Size size = Size(650,650);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_exit_ui_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupQuit::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
  
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_exit_ui_msg_1"), GAME_FONT, 32);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    labelText->setColor(COLOR_TITLE);
    labelText->setDimensions(_layerContainerContents->getContentSize().width * 0.9, 140);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
    
    auto labelTalk = Label::createWithTTF(GAME_TEXT("t_ui_exit_ui_msg_2"), GAME_FONT, 28);
    labelTalk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelTalk->setPosition(_layerContainerContents->getContentSize().width / 2, 200);
    labelTalk->setDimensions(_layerContainerContents->getContentSize().width * 0.9, labelTalk->getContentSize().height);
    labelTalk->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTalk->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelTalk);
    
    auto labelQuit = Label::createWithTTF(GAME_TEXT("t_ui_exit_ui_msg_3"), GAME_FONT, 36);
    labelQuit->setPosition(_layerContainerContents->getContentSize().width / 2, 100);
    _layerContainerContents->addChild(labelQuit);
}

void PopupQuit::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
 
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupQuit::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 24);
        labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(labelText);
    }
    
    auto itemQuit = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupQuit::onClickQuit, this));
    menu->addChild(itemQuit);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
        labelText->setPosition(itemQuit->getContentSize().width / 2, itemQuit->getContentSize().height / 2);
        itemQuit->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);

}


#pragma mark - set, get, sort, add
 

#pragma mark - click
void PopupQuit::onClickClose(Ref* sender)
{
    hide();
}

void PopupQuit::onClickQuit(cocos2d::Ref* sender)
{
    SaveManager::saveAllData();
    
    // noti
    if ( UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::OFFLINE) == true  )
    {
        MafNative::SendLocalNotification(E_NOTI::NOTI_OFFLINE, 43200, GAME_TEXT("t_ui_push_offlinereward_title").c_str(), GAME_TEXT("t_ui_push_offlinereward_contents").c_str());
    }
    
    //
    Director::getInstance()->end();
}

