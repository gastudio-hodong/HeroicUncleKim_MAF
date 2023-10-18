//
//  LayerDialogList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/09.
//

#include "LayerDialogList.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownloadList.h"
#include "GameUIPopup/Other/StoryDialog/PopupDialog.h"

#include "ManagerGame/DialogManager.h"

USING_NS_CC;

LayerDialogList* LayerDialogList::create(const Size size)
{
    LayerDialogList* pRet = new(std::nothrow) LayerDialogList();
    if ( pRet && pRet->init(size) )
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

LayerDialogList::LayerDialogList(void) :
_eType(LayerDialogList::E_TYPE::STORY_MAIN),

_layerContainerMenu(nullptr),
_layerContainerContents(nullptr),
_table(nullptr)
{
    _listDialog.clear();
}

LayerDialogList::~LayerDialogList(void)
{
    _listDialog.clear();
}

bool LayerDialogList::init(const Size size)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    setIgnoreAnchorPointForPosition(false);
    setContentSize(size);
    
    if ( _eType == LayerDialogList::E_TYPE::STORY_MAIN )
        _listDialog = DialogManager::getInstance()->getListDialogByType("STORY_MAIN");
    else if ( _eType == LayerDialogList::E_TYPE::STORY_SUB )
        _listDialog = DialogManager::getInstance()->getListDialogByType("STORY_SUB");
    else if ( _eType == LayerDialogList::E_TYPE::EVENT )
        _listDialog = DialogManager::getInstance()->getListDialogByType("EVENT");
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - table
void LayerDialogList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size LayerDialogList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 200);
}

extension::TableViewCell* LayerDialogList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    // obj
    ssize_t nIdxStart = idx * 3;
    ssize_t nIdxEnd = nIdxStart + 3;
    if ( nIdxEnd > _listDialog.size() )
    {
        nIdxEnd = _listDialog.size();
    }
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        auto objDialog = _listDialog.at(nIdxStart + i);
        
        if ( objDialog == nullptr )
        {
            continue;
        }
        
        double nPosX = 87.5 + 175 * i + 25 * i;
        double nPosY = size.height - 90;
        
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create(objDialog->getFileNameIcon()), nullptr, CC_CALLBACK_1(LayerDialogList::onClickDialog, this));
        item->setPosition(nPosX, nPosY);
        item->setTag(objDialog->getIdx());
        menu->addChild(item);
        
        auto labelName = Label::createWithTTF(objDialog->getTextName(), GAME_FONT, 20);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(item->getContentSize().width / 2, 0));
        labelName->setDimensions(item->getContentSize().width, 40);
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        item->addChild(labelName);
        
        
        bool bOpen = DialogManager::getInstance()->isDialogOpenException(objDialog->getIdx());
        
        if ( _eType == LayerDialogList::E_TYPE::EVENT )
        {// event 탭일때
            bOpen = true;
        }
         
        if ( bOpen == false)
        {
            auto layeyBlack = LayerColor::create(Color4B(0, 0, 0, 200), item->getContentSize().width, item->getContentSize().height);
            layeyBlack->setPosition( Vec2::ZERO );
            item->addChild(layeyBlack);
        }
        
        if ( bOpen == false )
        {
            auto labelCondition = Label::createWithTTF(objDialog->getTextLock(), GAME_FONT, 20);
            labelCondition->setPosition(item->getContentSize().width / 2, 40 + (item->getContentSize().height - 40) / 2);
            labelCondition->setDimensions(item->getContentSize().width * 0.98, labelCondition->getContentSize().height);
            labelCondition->setOverflow(Label::Overflow::SHRINK);
            labelCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            item->addChild(labelCondition);
        }
    }
    
    return cell;
}

ssize_t LayerDialogList::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listDialog.size() / 3;
    if ( _listDialog.size() % 3 != 0 )
        size++;
    
    return size;
}

#pragma mark - init
void LayerDialogList::initVar()
{
    
}

void LayerDialogList::initUi()
{
    Size size = getContentSize();

    
    //
    _layerContainerMenu = Layer::create();
    _layerContainerMenu->setContentSize( Size(size.width - 50, 60) );
    _layerContainerMenu->setIgnoreAnchorPointForPosition(false);
    _layerContainerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMenu->setPosition( Vec2(getContentSize().width / 2, getContentSize().height - 25) );
    addChild(_layerContainerMenu);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 25 - _layerContainerMenu->getContentSize().height - 25 - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( _layerContainerMenu->getPosition() + Vec2::DOWN * (_layerContainerMenu->getContentSize().height + 25) );
    addChild(_layerContainerContents);
    
    //
    uiMenu();
    uiContents();
}


#pragma mark - ui
void LayerDialogList::uiMenu()
{
    //
    _layerContainerMenu->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setName("MENU");
    _layerContainerMenu->addChild(menu);
    
    //
    std::vector<std::string> listText = {"t_ui_story_main", "t_ui_story_sub", "t_ui_story_event"};
    
    
    for ( int i = 0; i < listText.size(); i++ )
    {
        std::string strImage = "Assets/ui/story/story_btn_2_off.png";
        Color3B color = Color3B(167, 168, 169);
        if ( (int)_eType == i+1 )
        {
            strImage = "Assets/ui/story/story_btn_2_on.png";
            color = Color3B::WHITE;
        }
        
        double nPosX = 88 + i * 136 + i * 20;
        double nPosY = 24;
        
        auto btnMenu = ui::Button::create(strImage);
        btnMenu->setTag(i+1);
        btnMenu->setPosition(Vec2(nPosX, nPosY));
        btnMenu->addClickEventListener(CC_CALLBACK_1(LayerDialogList::onClickTab, this));
        btnMenu->setTitleText(GAME_TEXT(listText[i]));
        btnMenu->setTitleColor(color);
        btnMenu->setTitleFontSize(28);
        btnMenu->getTitleLabel()->setDimensions(btnMenu->getContentSize().width * 0.9, btnMenu->getContentSize().height);
        btnMenu->getTitleLabel()->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        btnMenu->getTitleLabel()->setOverflow(Label::Overflow::SHRINK);
        _layerContainerMenu->addChild(btnMenu);
    }
    
    // line
    auto spriteLine = Sprite::create("Assets/ui/story/story_line.png");
    spriteLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteLine->setPosition(0, 0);
    _layerContainerMenu->addChild(spriteLine);
    
}

void LayerDialogList::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
  
    //
    _table = extension::TableView::create(this, Size(575, _layerContainerContents->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerContents->addChild(_table);
    
    //
    _table->reloadData();
}

void LayerDialogList::drawContents(bool bRefresh)
{
    if ( bRefresh == false )
    {
        Vec2 pos = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(pos);
    }
    else
    {
        _table->reloadData();
    }
}

#pragma mark - set, get, other


void LayerDialogList::onClickTab(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    int nIdx = item->getTag();
    
    if ( _eType == (LayerDialogList::E_TYPE)nIdx )
    {
        return;
    }
    
    _eType = (LayerDialogList::E_TYPE)nIdx;
    
    if ( _eType == LayerDialogList::E_TYPE::STORY_MAIN )
        _listDialog = DialogManager::getInstance()->getListDialogByType("STORY_MAIN");
    else if ( _eType == LayerDialogList::E_TYPE::STORY_SUB )
        _listDialog = DialogManager::getInstance()->getListDialogByType("STORY_SUB");
    else if ( _eType == LayerDialogList::E_TYPE::EVENT )
        _listDialog = DialogManager::getInstance()->getListDialogByType("EVENT");
    
    //
    uiMenu();
    drawContents(true);
}

void LayerDialogList::onClickDialog(cocos2d::Ref* sender)
{
    auto node = (Node*)sender;
    int nIdx = node->getTag();
    
    bool bOpen = DialogManager::getInstance()->isDialogOpenException(nIdx);
    if ( _eType == LayerDialogList::E_TYPE::EVENT)
    {// event 탭일때
        bOpen = true;
    }
    
    if ( bOpen == false )
    {
        return;
    }
    
    auto objDialog = DialogManager::getInstance()->getInfoDialogByIdx(nIdx);
    if(objDialog == nullptr)
        return;
    // show
    auto popup = PopupDialog::create(objDialog->getSceneName());
    popup->show();
}
