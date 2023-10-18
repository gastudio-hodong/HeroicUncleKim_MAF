//
//  PopupChatInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupChatInfo.h"

#include "GameObject/InfoChat.h"

#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"
#include "GameUIPopup/Other/Chat/PopupChatBlock.h"
#include "GameUIPopup/Other/User/PopupUserReport.h"

USING_NS_CC;

PopupChatInfo* PopupChatInfo::create(InfoChat* infoChat)
{
    PopupChatInfo* pRet = new(std::nothrow) PopupChatInfo();
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

PopupChatInfo::PopupChatInfo():
_infoChat(nullptr),

_layerContainerButton(nullptr)
{
    
}

PopupChatInfo::~PopupChatInfo()
{
    if ( _infoChat != nullptr )
    {
        _infoChat->release();
        _infoChat = nullptr;
    }
}

bool PopupChatInfo::init(InfoChat* infoChat)
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
void PopupChatInfo::initVar()
{
    
}

void PopupChatInfo::initUI()
{
    // size
    Size size = Size(500, 444);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(_infoChat->getUserNick(), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupChatInfo::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, size.height - 75 - 50) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerButton->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerButton);
    
    //
    uiButton();
}
 
#pragma mark - ui
void PopupChatInfo::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    //
    auto spriteDetail = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteDetail->setContentSize(Size(380, 70));
    
    auto spriteDetailDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(32,35,10,2));
    spriteDetailDisable->setContentSize(Size(380, 70));
    
    auto itemDetail = MafNode::MafMenuItemSprite::create(spriteDetail, nullptr, spriteDetailDisable, CC_CALLBACK_1(PopupChatInfo::onClickDetail, this));
    menu->addChild(itemDetail);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_menu_stat_button_1"), GAME_FONT, 26);
        labelText->setDimensions(itemDetail->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setPosition(itemDetail->getContentSize().width / 2, itemDetail->getContentSize().height / 2);
        itemDetail->addChild(labelText);
    }
    
    //
    auto spriteReport = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteReport->setContentSize(Size(380, 70));
    
    auto spriteReportDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(32,35,10,2));
    spriteReportDisable->setContentSize(Size(380, 70));
    
    auto itemReport = MafNode::MafMenuItemSprite::create(spriteReport, nullptr, spriteReportDisable, CC_CALLBACK_1(PopupChatInfo::onClickReport, this));
    menu->addChild(itemReport);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_report"), GAME_FONT, 26);
        labelText->setDimensions(itemReport->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setPosition(itemReport->getContentSize().width / 2, itemReport->getContentSize().height / 2);
        itemReport->addChild(labelText);
    }
    
    //
    auto spriteBlock = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteBlock->setContentSize(Size(380, 70));
    
    auto spriteBlockDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(32,35,10,2));
    spriteBlockDisable->setContentSize(Size(380, 70));
    
    auto itemBlock = MafNode::MafMenuItemSprite::create(spriteBlock, nullptr, spriteBlockDisable, CC_CALLBACK_1(PopupChatInfo::onClickBlock, this));
    menu->addChild(itemBlock);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_userblock"), GAME_FONT, 26);
        labelText->setDimensions(itemBlock->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setPosition(itemBlock->getContentSize().width / 2, itemBlock->getContentSize().height / 2);
        itemBlock->addChild(labelText);
    }
    
    menu->alignItemsVerticallyWithPadding(20);
    
    //
    drawButton();
}

void PopupChatInfo::drawButton()
{
    
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void PopupChatInfo::onClickClose(Ref* sender)
{
    hide();
}

void PopupChatInfo::onClickDetail(Ref* sender)
{
    auto popup = PopupOtherUserInfo::create(_infoChat->getUserIdx(), _infoChat->getUserPlatform(), _infoChat->getUserNick());
    popup->show();
}

void PopupChatInfo::onClickReport(Ref* sender)
{
    auto popup = PopupUserReport::create(E_REPORT_TYPE::CHAT, _infoChat->getUserIdx(), _infoChat->getUserNick(), _infoChat->getUserPlatform(), _infoChat->getMessage());
    popup->show();
}

void PopupChatInfo::onClickBlock(cocos2d::Ref* sender)
{
    auto popup = PopupChatBlock::create(_infoChat);
    popup->show();
    
    hide();
}
