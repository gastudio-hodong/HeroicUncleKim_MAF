//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupFriend.h"

#include "GameUIPopup/Other/Friend/LayerFriendList.h"
#include "GameUIPopup/Other/Friend/LayerFriendInvite.h"

USING_NS_CC;

PopupFriend* PopupFriend::create()
{
    PopupFriend* pRet = new(std::nothrow) PopupFriend();
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

PopupFriend::PopupFriend(void) :
_eType(PopupFriend::type_list),

_layerContainerMenu(nullptr),
_layerContainerNow(nullptr),
_layerContainerFriendList(nullptr),
_layerContainerFriendInvite(nullptr)
{
    
}

PopupFriend::~PopupFriend(void)
{
    
}

bool PopupFriend::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    return true;
}
#pragma mark - init
void PopupFriend::initVar()
{
    
}

void PopupFriend::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(658, getContentSize().height * 0.90) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(20);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_friend"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFriend::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerMenu = LayerColor::create();
    _layerContainerMenu->setContentSize( Size(_spriteContainer->getContentSize().width - 12, 80) );
    _layerContainerMenu->setIgnoreAnchorPointForPosition(false);
    _layerContainerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMenu->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerMenu);
    
    
    //
    uiMenu();
}
#pragma mark - ui
void PopupFriend::uiMenu()
{
    //
    _layerContainerMenu->removeAllChildren();
    
    //
    int listMenu[] = {PopupFriend::type_list,PopupFriend::type_invite};
    std::string listMenuTextKey[] = {"t_ui_friend_list", "t_ui_friend_invite"};
        
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setName("MENU");
    _layerContainerMenu->addChild(menu);
        
    float nMenuWidth = _layerContainerMenu->getContentSize().width / (sizeof(listMenu) / sizeof(int));
    for ( int i = 0; i < sizeof(listMenu) / sizeof(int); i++ )
    {
        std::string strPathButton = "Assets/ui_common/btn_tap_off.png";
        if ( _eType == listMenu[i] )
        {
            strPathButton = "Assets/ui_common/btn_tap_on.png";
        }
            
        auto spriteButtonOn = ui::Scale9Sprite::create(strPathButton, Rect::ZERO, Rect(35,25,2,10));
        spriteButtonOn->setContentSize(Size(nMenuWidth, spriteButtonOn->getContentSize().height));
            
        auto itemButton = MafNode::MafMenuItemSprite::create(spriteButtonOn, nullptr, nullptr, CC_CALLBACK_1(PopupFriend::onClickMenu, this));
        itemButton->setPosition(nMenuWidth * i + itemButton->getContentSize().width /2, itemButton->getContentSize().height / 2 );
        itemButton->setTag(listMenu[i]);
        menu->addChild(itemButton);
            
        auto labelButton = Label::createWithTTF(GAME_TEXT(listMenuTextKey[i]), GAME_FONT, 24);
        labelButton->setPosition(itemButton->getContentSize().width / 2, itemButton->getContentSize().height / 2);
        labelButton->setTextColor(Color4B(86, 86, 86, 255));
        labelButton->setDimensions(itemButton->getContentSize().width * 0.9, labelButton->getContentSize().height);
        labelButton->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelButton->setOverflow(Label::Overflow::SHRINK);
        labelButton->setName("LABEL");
        itemButton->addChild(labelButton);
        if ( _eType == listMenu[i] )
        {
            labelButton->setTextColor(Color4B(255, 255, 255, 255));
        }
    }
    
    //
    drawMenu();
}

#pragma mark - set, get, other
void PopupFriend::drawMenu()
{
    if ( _layerContainerNow != nullptr )
    {
        _layerContainerNow->setVisible(false);
    }
    
    if ( _eType == PopupFriend::type_list )
    {
        if ( _layerContainerFriendList == nullptr )
        {
            auto layer = LayerFriendList::create(Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - _layerContainerMenu->getContentSize().height - 25), _ePlace);
            layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            layer->setPosition(_spriteContainer->getContentSize().width / 2, 25);
            layer->setCallbackHide(CC_CALLBACK_0(PopupFriend::callbackHide, this));
            _spriteContainer->addChild(layer);
            
            _layerContainerFriendList = layer;
        }
        _layerContainerNow = _layerContainerFriendList;
        _layerContainerNow->setVisible(true);
        
        // 친구요청 새로고침
        ((LayerFriendList*)_layerContainerNow)->requestInfo();
    }
    else if ( _eType == PopupFriend::type_invite )
    {
        if ( _layerContainerFriendInvite == nullptr )
        {
            auto layer = LayerFriendInvite::create(Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - _layerContainerMenu->getContentSize().height - 25));
            layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            layer->setPosition(_spriteContainer->getContentSize().width / 2, 25);
            layer->setCallbackHide(CC_CALLBACK_0(PopupFriend::callbackHide, this));
            _spriteContainer->addChild(layer);
            
            _layerContainerFriendInvite = layer;
        }
        _layerContainerNow = _layerContainerFriendInvite;
        _layerContainerNow->setVisible(true);
    }
}

#pragma mark - click
void PopupFriend::onClickClose(Ref* sender)
{
    hide();
}

void PopupFriend::onClickMenu(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    PopupFriend::eType nType = (eType)item->getTag();
    if ( _eType == nType )
    {
        return;
    }
    
    //
    _eType = nType;
    
    //
    uiMenu();
}

#pragma mark - callback
void PopupFriend::callbackHide()
{
    hide();
}
