//
//  PopupChatBlockList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupChatBlockList.h"

#include "GameObject/InfoChat.h"

#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"

#include "ManagerGame/ChatManager.h"

USING_NS_CC;

PopupChatBlockList* PopupChatBlockList::create()
{
    PopupChatBlockList* pRet = new(std::nothrow) PopupChatBlockList();
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

PopupChatBlockList::PopupChatBlockList():

_layerContainerContents(nullptr),

_table(nullptr),
_labelListNothing(nullptr)
{
    
}

PopupChatBlockList::~PopupChatBlockList()
{
    
}

bool PopupChatBlockList::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}

#pragma mark - init
void PopupChatBlockList::initVar()
{
    _listBlock = ChatManager::getInstance()->getBlockList();
}

void PopupChatBlockList::initUI()
{
    // size
    Size size = Size(500, 444);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_userblock_blocklist"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupChatBlockList::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    //
    uiContents();
}
 
#pragma mark - table
void PopupChatBlockList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupChatBlockList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 75);
}

extension::TableViewCell* PopupChatBlockList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();

    // size
    Size size = tableCellSizeForIndex(table, idx);

    // obj
    std::string strBlock = _listBlock.at(idx);
    
    //
    std::vector<std::string> listBlockInfo = UtilsString::getInstance()->split(strBlock, '#');
    std::string userNick = listBlockInfo.at(1);
    
    
    auto uiBackground = Layer::create();
    uiBackground->setContentSize(Size(size.width, size.height));
    uiBackground->setIgnoreAnchorPointForPosition(false);
    uiBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBackground->setPosition(size.width / 2, size.height / 2);
    cell->addChild(uiBackground);
    
    auto uiTextNick = ui::Text::create(userNick, GAME_FONT, 24);
    uiTextNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiTextNick->setPosition(Vec2(20, uiBackground->getContentSize().height / 2));
    uiTextNick->setTextAreaSize(Size(270, uiTextNick->getContentSize().height));
    uiTextNick->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTextNick->setTextHorizontalAlignment(TextHAlignment::LEFT);
    ((Label*)uiTextNick->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
    uiBackground->addChild(uiTextNick);
    

    auto uiBtnUnlock = ui::Button::create("Assets/ui_common/btn_baphomet.png");
    uiBtnUnlock->addClickEventListener(CC_CALLBACK_1(PopupChatBlockList::onClickUnBlock, this));
    uiBtnUnlock->setZoomScale(0.05f);
    uiBtnUnlock->setPosition(Vec2(uiBackground->getContentSize().width - uiBtnUnlock->getContentSize().width / 2, uiBackground->getContentSize().height / 2));
    uiBtnUnlock->setTag((int)idx);
    uiBackground->addChild(uiBtnUnlock);
    {
        auto uiText = ui::Text::create(GAME_TEXT("t_ui_userblock_unblock"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtnUnlock->getContentSize().width / 2, uiBtnUnlock->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBtnUnlock->getContentSize().width * 0.9, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        ((Label*)uiText->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        uiBtnUnlock->addChild(uiText);
    }
    
    return cell;
}

ssize_t PopupChatBlockList::numberOfCellsInTableView(extension::TableView *table)
{
    return _listBlock.size();
}

#pragma mark - ui
void PopupChatBlockList::uiContents()
{
    _layerContainerContents->removeAllChildren();
  
    //
    _table = extension::TableView::create(this, _layerContainerContents->getContentSize());
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerContents->getContentSize().width / 2, 0) );
    _table->setDelegate(this);
    _table->setBounceable(true);
    _layerContainerContents->addChild(_table);
    
    // label nothing
    _labelListNothing = Label::createWithTTF(GAME_TEXT("t_ui_msg_nodata"), GAME_FONT, 24);
    _labelListNothing->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
    _labelListNothing->setVisible(false);
    _layerContainerContents->addChild(_labelListNothing);
    
    //
    drawTable();
}
 

void PopupChatBlockList::drawTable()
{
    _table->reloadData();
    
    // label empty
    if ( _listBlock.size() != 0 )
    {
        _labelListNothing->setVisible(false);
    }
    else
    {
        _labelListNothing->setVisible(true);
    }
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void PopupChatBlockList::onClickClose(Ref* sender)
{
    hide();
}

void PopupChatBlockList::onClickUnBlock(Ref* sender)
{
    auto item = (ui::Button*)sender;
    
    //
    std::string strBlock = _listBlock.at(item->getTag());
    std::vector<std::string> listBlockInfo = UtilsString::getInstance()->split(strBlock, '#');
    
    std::string userId = listBlockInfo.at(0);
    
    // del block
    ChatManager::getInstance()->delBlockList(userId);
    _listBlock.clear();
    _listBlock = ChatManager::getInstance()->getBlockList();
    
    // refresh
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CHATTING);
    
    //
    drawTable();
}
 
