//
//  PopupBadgeDetail.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupBadgeDetail.h"

#include "GameObject/InfoBadge.h"

#include "ManagerGame/BadgeManager.h"

USING_NS_CC;

PopupBadgeDetail* PopupBadgeDetail::create(int nBadgeIdx)
{
    PopupBadgeDetail* pRet = new(std::nothrow) PopupBadgeDetail();
    if ( pRet && pRet->init(nBadgeIdx) )
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

PopupBadgeDetail::PopupBadgeDetail(void) :
_nBadgeIdx(0),

_layerContainerBadgeInfo(nullptr),
_layerContainerBadgeList(nullptr),

_tableList(nullptr)
{
}

PopupBadgeDetail::~PopupBadgeDetail(void)
{
}

bool PopupBadgeDetail::init(int nBadgeIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nBadgeIdx = nBadgeIdx;
    
    //
    initVar();
    initUi();

    return true;
}

void PopupBadgeDetail::onEnter()
{
    PopupBase::onEnter();
    
    
}

#pragma mark - table
void PopupBadgeDetail::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size PopupBadgeDetail::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 110);
}

extension::TableViewCell* PopupBadgeDetail::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    auto obj = _listBadgeHave.at(idx);
 
    int nBonusValue = obj->getBuffCount() * 100;
    
    std::string strTime = UtilsDate::getInstance()->getTimeString(obj->getCreated(), UtilsDate::eType::date_real_Ymd);
    std::string strText = GAME_TEXTFORMAT("t_ui_badge_created", strTime.c_str());
    strText.append("\n");
    strText.append(GAME_TEXTFORMAT("t_ui_badge_spec", nBonusValue));
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_7.png", Rect::ZERO, Rect(10,10,20,20));
    spriteBG->setContentSize(Size(size.width, size.height - 5));
    spriteBG->setPosition(size.width / 2, size.height / 2);
    cell->addChild(spriteBG);
    
    auto labelInfo = Label::createWithTTF(strText, GAME_FONT, 20);
    labelInfo->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
    labelInfo->setDimensions(spriteBG->getContentSize().width * 0.95, spriteBG->getContentSize().height * 0.95);
    labelInfo->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    spriteBG->addChild(labelInfo);
    
    return cell;
}

ssize_t PopupBadgeDetail::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    size_t size = _listBadgeHave.size();

    return size;
}



#pragma mark - init
void PopupBadgeDetail::initVar()
{
    _objBadge = BadgeManager::getInstance()->getBadge(_nBadgeIdx);
    _listBadgeHave = BadgeManager::getInstance()->getListHaveFromBadgeIdx(_nBadgeIdx);
    
    _nBadgeBuffValue = BadgeManager::getInstance()->getBadgeBuffCount(_objBadge->getBuffType(), _objBadge->getIdx());
}

void PopupBadgeDetail::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(528, 688) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_shop_bonus_msg_5"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBadgeDetail::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerBadgeInfo = Layer::create();
    _layerContainerBadgeInfo->setContentSize( Size(496, 150) );
    _layerContainerBadgeInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadgeInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadgeInfo->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerBadgeInfo);
 
    _layerContainerBadgeList = Layer::create();
    _layerContainerBadgeList->setContentSize( Size(496, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeInfo->getContentSize().height - 25) );
    _layerContainerBadgeList->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadgeList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadgeList->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeInfo->getContentSize().height);
    _spriteContainer->addChild(_layerContainerBadgeList);
    
    //
    uiBadgeInfo();
    uiBadgeList();
}

#pragma mark - ui
void PopupBadgeDetail::uiBadgeInfo()
{
    _layerContainerBadgeInfo->removeAllChildren();
 
    //
    auto spriteIcon = Sprite::create(_objBadge->getPath());
    if(spriteIcon == nullptr)
    {
        spriteIcon = Sprite::create("Assets/ui/stat/badge_000.png");
    }
    spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteIcon->setPosition(130, _layerContainerBadgeInfo->getContentSize().height / 2);
    _layerContainerBadgeInfo->addChild(spriteIcon);
    
    //
    auto labelName = Label::createWithTTF(_objBadge->getDesc(), GAME_FONT, 24);
    labelName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    labelName->setPosition(140, _layerContainerBadgeInfo->getContentSize().height / 2);
    labelName->setTextColor(Color4B(255, 255, 0, 255));
    _layerContainerBadgeInfo->addChild(labelName);
    
    auto labelBonus = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_badge_total", (int)(_nBadgeBuffValue * 100)), GAME_FONT, 20);
    labelBonus->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelBonus->setPosition(140, _layerContainerBadgeInfo->getContentSize().height / 2);
    labelBonus->setTextColor(Color4B(255, 140, 0, 255));
    labelBonus->setDimensions(350, labelBonus->getContentSize().height);
    labelBonus->setOverflow(Label::Overflow::SHRINK);
    labelBonus->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _layerContainerBadgeInfo->addChild(labelBonus);
}

void PopupBadgeDetail::uiBadgeList()
{
    _layerContainerBadgeList->removeAllChildren();
    
 
    // table
    _tableList = extension::TableView::create(this, Size(_layerContainerBadgeList->getContentSize().width, _layerContainerBadgeList->getContentSize().height) );
    _tableList->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableList->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableList->setIgnoreAnchorPointForPosition(false);
    _tableList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tableList->setPosition(_layerContainerBadgeList->getContentSize().width / 2, 0);
    _tableList->setDelegate(this);
    _tableList->setBounceable(false);
    _layerContainerBadgeList->addChild(_tableList);

}
 
#pragma mark - draw
 
#pragma mark - set, get

#pragma mark - click
void PopupBadgeDetail::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
