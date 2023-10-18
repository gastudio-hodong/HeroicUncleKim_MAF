//
//  PopupEventDrawLimitInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupEventDrawLimitInfo.h"

#include "GameObject/EventDraw/InfoEventDraw.h"

#include "ManagerEvent/EventDrawLimitManager.h"

USING_NS_CC;

PopupEventDrawLimitInfo* PopupEventDrawLimitInfo::create()
{
    PopupEventDrawLimitInfo* pRet = new(std::nothrow) PopupEventDrawLimitInfo();
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

PopupEventDrawLimitInfo::PopupEventDrawLimitInfo(void) :

_layerContainerTop(nullptr),
_layerContainerReward(nullptr),

_tableList(nullptr)
{
    
}

PopupEventDrawLimitInfo::~PopupEventDrawLimitInfo(void)
{
}

bool PopupEventDrawLimitInfo::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    

    return true;
}

#pragma mark - table
void PopupEventDrawLimitInfo::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size PopupEventDrawLimitInfo::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 30);
}

extension::TableViewCell* PopupEventDrawLimitInfo::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    auto objInfo = _listInfoDraw.at(idx);
    auto objItem = objInfo->getReward();
    if ( objItem != nullptr )
    {// width 600
        auto labelItemName = Label::createWithTTF(objItem->getText(), GAME_FONT, 20);
        labelItemName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelItemName->setPosition(0, size.height / 2);
        labelItemName->setDimensions(200, labelItemName->getContentSize().height);
        labelItemName->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelItemName->setOverflow(Label::Overflow::SHRINK);
        cell->addChild(labelItemName);
        
        auto labelItemCount = Label::createWithTTF(objItem->getCount(), GAME_FONT, 20);
        labelItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelItemCount->setPosition(200, size.height / 2);
        labelItemCount->setDimensions(100, labelItemCount->getContentSize().height);
        labelItemCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelItemCount->setOverflow(Label::Overflow::SHRINK);
        cell->addChild(labelItemCount);
        
        auto labelItemRate = Label::createWithTTF(MafUtils::format("%.2f%%", objInfo->getRate() / 100.0), GAME_FONT, 20);
        labelItemRate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelItemRate->setPosition(300, size.height / 2);
        labelItemRate->setDimensions(100, labelItemRate->getContentSize().height);
        labelItemRate->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelItemRate->setOverflow(Label::Overflow::SHRINK);
        cell->addChild(labelItemRate);
    }
    
    return cell;
}

ssize_t PopupEventDrawLimitInfo::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _listInfoDraw.size();
}

#pragma mark - init
void PopupEventDrawLimitInfo::initVar()
{
    _listInfoDraw = EventDrawLimitManager::getInstance()->getListInfoItemsAll();
}

void PopupEventDrawLimitInfo::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(732, 1284) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_token_title_info"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventDrawLimitInfo::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerTop = Layer::create();
    _layerContainerTop->setContentSize( Size(712, 200) );
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerTop);
    
    _layerContainerReward = Layer::create();
    _layerContainerReward->setContentSize( Size(712, _spriteContainer->getContentSize().height - 75 - _layerContainerTop->getContentSize().height - 25) );
    _layerContainerReward->setIgnoreAnchorPointForPosition(false);
    _layerContainerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerReward->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerReward);
    
    
    //
    uiTop();
    uiReward();
    
}

#pragma mark - ui
void PopupEventDrawLimitInfo::uiTop()
{
    _layerContainerTop->removeAllChildren();
    
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_token_text_infocontents"), GAME_FONT, 24);
    labelInfo->setPosition(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height / 2);
    labelInfo->setDimensions(_layerContainerTop->getContentSize().width, _layerContainerTop->getContentSize().height);
    labelInfo->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    _layerContainerTop->addChild(labelInfo);
}

void PopupEventDrawLimitInfo::uiReward()
{
    _layerContainerReward->removeAllChildren();
    
    // table
    _tableList = extension::TableView::create(this, Size(600, _layerContainerReward->getContentSize().height) );
    _tableList->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableList->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableList->setIgnoreAnchorPointForPosition(false);
    _tableList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tableList->setPosition(_layerContainerReward->getContentSize().width / 2, 0);
    _tableList->setDelegate(this);
    _tableList->setBounceable(false);
    _layerContainerReward->addChild(_tableList);
}

#pragma mark - draw
void PopupEventDrawLimitInfo::drawReward()
{
     
}

#pragma mark - set, get

#pragma mark - click
void PopupEventDrawLimitInfo::onClickClose(Ref* sender)
{
    hide();
}
 
