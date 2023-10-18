//
//  PopupChatBlock.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupChatBlock.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoChat.h"

#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"
#include "GameUIPopup/Other/Chat/PopupChatBlockList.h"

#include "ManagerGame/ChatManager.h"

USING_NS_CC;

PopupChatBlock* PopupChatBlock::create(InfoChat* infoChat)
{
    PopupChatBlock* pRet = new(std::nothrow) PopupChatBlock();
    if ( pRet && pRet->init(infoChat) )
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

PopupChatBlock::PopupChatBlock():
_infoChat(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),

_labelBlockCount(nullptr)
{
    
}

PopupChatBlock::~PopupChatBlock()
{
    if ( _infoChat != nullptr )
    {
        _infoChat->release();
        _infoChat = nullptr;
    }
}

bool PopupChatBlock::init(InfoChat* infoChat)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _infoChat = infoChat;
    _infoChat->retain();
    
    //
    initVar();
    initUI();
    
    return true;
}

#pragma mark - init
void PopupChatBlock::initVar()
{
    
}

void PopupChatBlock::initUI()
{
    // size
    Size size = Size(500, 444);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_userblock"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupChatBlock::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 100) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    //
    uiContents();
    uiButton();
}
 
#pragma mark - ui
void PopupChatBlock::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_userblock_chat"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    labelText->setDimensions(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height - 50);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
    
    auto labelNick = Label::createWithTTF(_infoChat->getUserNick(), GAME_FONT, 24);
    labelNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelNick->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    labelNick->setColor(Color3B(255, 255, 0));
    _layerContainerContents->addChild(labelNick);
}

void PopupChatBlock::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    //
    auto spriteList = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteList->setContentSize(Size(212, 100));

    auto itemList = MafNode::MafMenuItemSprite::create(spriteList, nullptr, CC_CALLBACK_1(PopupChatBlock::onClickList, this));
    menu->addChild(itemList);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_userblock_blocklist_btn"), GAME_FONT, 26);
        labelText->setDimensions(itemList->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setPosition(itemList->getContentSize().width / 2, itemList->getContentSize().height / 2);
        itemList->addChild(labelText);
    }
    
    //
    auto spriteReport = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteReport->setContentSize(Size(212, 100));
    
    auto itemReport = MafNode::MafMenuItemSprite::create(spriteReport, nullptr, CC_CALLBACK_1(PopupChatBlock::onClickReport, this));
    menu->addChild(itemReport);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_userblock"), GAME_FONT, 26);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelText->setDimensions(itemReport->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setPosition(itemReport->getContentSize().width / 2, itemReport->getContentSize().height / 2);
        itemReport->addChild(labelText);
        
        _labelBlockCount = Label::createWithTTF(" ", GAME_FONT, 20);
        _labelBlockCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _labelBlockCount->setPosition(itemReport->getContentSize().width / 2, itemReport->getContentSize().height / 2);
        itemReport->addChild(_labelBlockCount);
    }
    
    menu->alignItemsHorizontallyWithPadding(10);
    
    //
    drawButton();
}

void PopupChatBlock::drawButton()
{
    int nBlockCount = ChatManager::getInstance()->getBlockListCount();
    int nBlockMax = ChatManager::getInstance()->getBlockListMax();
    
    std::string strBlockCount = UtilsString::getInstance()->format("(%d/%d)", nBlockCount, nBlockMax);
    _labelBlockCount->setString(strBlockCount);
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void PopupChatBlock::onClickClose(Ref* sender)
{
    hide();
}

void PopupChatBlock::onClickList(Ref* sender)
{
    auto popup = PopupChatBlockList::create();
    popup->show();
}

void PopupChatBlock::onClickReport(Ref* sender)
{
    int nBlockCount = ChatManager::getInstance()->getBlockListCount();
    int nBlockMax = ChatManager::getInstance()->getBlockListMax();
    
    if ( nBlockCount >= nBlockMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_time_jump_mag_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    // add block
    ChatManager::getInstance()->addBlockList(_infoChat->getUserId(), _infoChat->getUserNick());
    
    // refresh
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CHATTING);
    
    //
    hide();
}

