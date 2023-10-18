//
//  LayerStoryList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/09.
//

#include "LayerStoryList.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownloadList.h"

#include "ManagerGame/StoryManager.h"

USING_NS_CC;

LayerStoryList* LayerStoryList::create(const Size size)
{
    LayerStoryList* pRet = new(std::nothrow) LayerStoryList();
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

LayerStoryList::LayerStoryList(void) :
_nIdxMenu(0),

_layerContainerMenu(nullptr),
_layerContainerContents(nullptr),
_table(nullptr)
{
    
}

LayerStoryList::~LayerStoryList(void)
{
    
}

bool LayerStoryList::init(const Size size)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    setIgnoreAnchorPointForPosition(false);
    setContentSize(size);
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - table
void LayerStoryList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size LayerStoryList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 200);
}

extension::TableViewCell* LayerStoryList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    if ( nIdxEnd > listStoryTable.size() )
    {
        nIdxEnd = listStoryTable.size();
    }
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdxStory = listStoryTable.at(nIdxStart + i);
        
        auto obj = StoryManager::getInstance()->getStory(nIdxStory);
        if ( obj == nullptr )
        {
            continue;
        }
        
        double nPosX = 87.5 + 175 * i + 25 * i;
        double nPosY = size.height - 90;
        
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create(obj->getFileNameIcon()), nullptr, CC_CALLBACK_1(LayerStoryList::onClickStory, this));
        item->setPosition(nPosX, nPosY);
        item->setTag(nIdxStory);
        menu->addChild(item);
        
        auto labelName = Label::createWithTTF(obj->getTextName(), GAME_FONT, 20);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(item->getContentSize().width / 2, 0));
        labelName->setDimensions(item->getContentSize().width, 40);
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        item->addChild(labelName);
        
        
        bool bOpen = StoryManager::getInstance()->isStoryOpenException(nIdxStory);
        bool bFile = StoryManager::getInstance()->isStoryFile(nIdxStory);
        
        if ( _nIdxMenu == 2 )
        {// event 탭일때
            bOpen = true;
        }
         
        if ( bOpen == false || bFile == false )
        {
            auto layeyBlack = LayerColor::create(Color4B(0, 0, 0, 200), item->getContentSize().width, item->getContentSize().height);
            layeyBlack->setPosition( Vec2::ZERO );
            item->addChild(layeyBlack);
        }
        
        if ( StoryManager::getInstance()->isStoryFile(nIdxStory) == false )
        {// file download icon
            std::string strPath = "Assets/ui/story/comic_down_on.png";;
            if ( bOpen == false )
            {
                strPath = "Assets/ui/story/comic_down_off.png";
            }
            
            auto spriteDownload = Sprite::create(strPath);
            spriteDownload->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            spriteDownload->setPosition(item->getContentSize().width - 5, 40 + 5);
            item->addChild(spriteDownload);
        }
        
        if ( bOpen == false )
        {
            auto labelCondition = Label::createWithTTF(obj->getTextCondition(), GAME_FONT, 20);
            labelCondition->setPosition(item->getContentSize().width / 2, 40 + (item->getContentSize().height - 40) / 2);
            labelCondition->setDimensions(item->getContentSize().width * 0.98, labelCondition->getContentSize().height);
            labelCondition->setOverflow(Label::Overflow::SHRINK);
            labelCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            item->addChild(labelCondition);
        }
    }
    
    return cell;
}

ssize_t LayerStoryList::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = listStoryTable.size() / 3;
    if ( listStoryTable.size() % 3 != 0 )
        size++;
    
    return size;
}

#pragma mark - init
void LayerStoryList::initVar()
{
    //
    listStoryNormal.push_back((int)E_COMIC::BEGINNING);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_10);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_100);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_150);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_300);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_600);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_777);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_900);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_1000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_1100);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_1200);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_1400);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_1700);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_2000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_2200);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_2500);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_3000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_4000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_5000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_5600);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_6000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_6600);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_7000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_7600);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_8000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_4500);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_6500);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_9000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_10000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_12000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_14000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_16000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_18000);
    listStoryNormal.push_back((int)E_COMIC::CLEAR_REALENDING);
    listStoryNormal.push_back((int)E_COMIC::TUTORIAL);
    listStoryNormal.push_back((int)E_COMIC::REVIVE);
    listStoryNormal.push_back((int)E_COMIC::COSTUME_ROOM);
    listStoryNormal.push_back((int)E_COMIC::DUNGEON_JOIN);
    listStoryNormal.push_back((int)E_COMIC::PRISON_FIRE_JOIN);
    listStoryNormal.push_back((int)E_COMIC::BAFOMETZ);
    listStoryNormal.push_back((int)E_COMIC::BEELZEBUB);
    listStoryNormal.push_back((int)E_COMIC::JELLY);
    listStoryNormal.push_back((int)E_COMIC::RAID_JOIN);
    listStoryNormal.push_back((int)E_COMIC::MYHOME);
    listStoryNormal.push_back((int)E_COMIC::MINE_JOIN);
    listStoryNormal.push_back((int)E_COMIC::TOWN_JOIN);
    listStoryNormal.push_back((int)E_COMIC::PET_EQUIP_JOIN);
    listStoryNormal.push_back((int)E_COMIC::EXPEDITION_JOIN);
    listStoryNormal.push_back((int)E_COMIC::ARTIFACT_EAGLE);
    listStoryNormal.push_back((int)E_COMIC::ARTIFACT_RING);
    listStoryNormal.push_back((int)E_COMIC::ARTIFACT_STORMY);
    listStoryNormal.push_back((int)E_COMIC::MAX_ARTIFACT_1);
    listStoryNormal.push_back((int)E_COMIC::MAX_ARTIFACT_3);
    listStoryNormal.push_back((int)E_COMIC::MAX_ARTIFACT_4);
    listStoryNormal.push_back((int)E_COMIC::MAX_ARTIFACT_8);
    listStoryNormal.push_back((int)E_COMIC::MAX_ARTIFACT_12);
    listStoryNormal.push_back((int)E_COMIC::QUEST_OPEN_7);
    listStoryNormal.push_back((int)E_COMIC::QUEST_OPEN_11);
    listStoryNormal.push_back((int)E_COMIC::QUEST_OPEN_24);
    
    //
    listStoryDevil.push_back((int)E_COMIC::DEVIL_START);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_1000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_2000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_3000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_4000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_5000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_6000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_7000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_8000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_9000);
    listStoryDevil.push_back((int)E_COMIC::DEVIL_10000);
    
    //
    listStoryEvent.push_back((int)E_COMIC::EVENT_SUMMER);
    listStoryEvent.push_back((int)E_COMIC::EVENT_HALLOWEEN);
    listStoryEvent.push_back((int)E_COMIC::EVENT_CHRISTMAS);
    listStoryEvent.push_back((int)E_COMIC::EVENT_SUMMER_FISHING);
    listStoryEvent.push_back((int)E_COMIC::TEAM_RAID);
    listStoryEvent.push_back((int)E_COMIC::EXPEDITION_CAT_1);
    listStoryEvent.push_back((int)E_COMIC::EXPEDITION_CAT_2);
    listStoryEvent.push_back((int)E_COMIC::EXPEDITION_KRAKEN);
    listStoryEvent.push_back((int)E_COMIC::EVENT_RETURN_ATTEND);
        
    
    //
    for(const auto &str : listStoryNormal) {
        listStoryTable.push_back(str);
    }
}

void LayerStoryList::initUi()
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
void LayerStoryList::uiMenu()
{
    //
    _layerContainerMenu->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setName("MENU");
    _layerContainerMenu->addChild(menu);
    
    //
    std::string listText[] = {"t_ui_dungeon_normal", "t_ui_dungeon_devil", "t_ui_story_event"};
    
    
    for ( int i = 0; i < sizeof(listText)/sizeof(std::string); i++ )
    {
        std::string strImage = "Assets/ui/story/story_btn_2_off.png";
        Color3B color = Color3B(167, 168, 169);
        if ( _nIdxMenu == i )
        {
            strImage = "Assets/ui/story/story_btn_2_on.png";
            color = Color3B::WHITE;
        }
        
        double nPosX = 88 + i * 136 + i * 20;
        double nPosY = 24;
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create(strImage), nullptr, CC_CALLBACK_1(LayerStoryList::onClickTab, this));
        item->setPosition(nPosX, nPosY);
        item->setTag(i);
        menu->addChild(item);
        
        auto label = Label::createWithTTF(GAME_TEXT(listText[i]), GAME_FONT, 28);
        label->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        label->enableBold();
        label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setColor(color);
        label->setTag(100);
        item->addChild(label);
    }
    
    // line
    auto spriteLine = Sprite::create("Assets/ui/story/story_line.png");
    spriteLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteLine->setPosition(0, 0);
    _layerContainerMenu->addChild(spriteLine);
    
    
    // download all
    auto menuDownload = MafNode::MafMenu::create();
    menuDownload->setPosition(Vec2::ZERO);
    _layerContainerMenu->addChild(menuDownload);
    
    auto spriteDownload = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteDownload->setContentSize(Size(90, 48));
    
    auto itemDownload = MafNode::MafMenuItemSprite::create(spriteDownload, nullptr, CC_CALLBACK_1(LayerStoryList::onClickStoryDownload, this));
    itemDownload->setPosition(_layerContainerMenu->getContentSize().width - itemDownload->getContentSize().width / 2, 24);
    menuDownload->addChild(itemDownload);
    {
        auto spriteDownloadIcon = Sprite::create("Assets/ui/story/comic_down_on.png");
        spriteDownloadIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteDownloadIcon->setPosition(itemDownload->getContentSize().width - 10, itemDownload->getContentSize().height / 2);
        spriteDownloadIcon->setScale(0.8);
        itemDownload->addChild(spriteDownloadIcon);
        
        auto labelDownload = Label::createWithTTF("ALL", GAME_FONT, 20);
        labelDownload->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        labelDownload->setPosition(spriteDownloadIcon->getPositionX() - 10 - spriteDownloadIcon->getBoundingBox().size.width, itemDownload->getContentSize().height / 2);
        labelDownload->setColor(COLOR_COIN);
        itemDownload->addChild(labelDownload);
    }
    
}

void LayerStoryList::uiContents()
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

void LayerStoryList::drawMenu()
{
    auto menu = (MafNode::MafMenu*)_layerContainerMenu->getChildByName("MENU");
    if ( menu == nullptr )
    {
        return;
    }
    
    for ( int i = 0; i < menu->getChildrenCount(); i++ )
    {
        std::string strImage = "Assets/ui/story/story_btn_2_off.png";
        Color3B color = Color3B(167, 168, 169);
        if ( _nIdxMenu == i )
        {
            strImage = "Assets/ui/story/story_btn_2_on.png";
            color = Color3B::WHITE;
        }
        
        auto item = (MafNode::MafMenuItemSprite*)menu->getChildByTag(i);
        ((Sprite*)item->getNormalImage())->setTexture(strImage);
        
        auto label = (Label*)item->getChildByTag(100);
        label->setColor(color);
    }
}

void LayerStoryList::drawContents(bool bRefresh)
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


void LayerStoryList::onClickTab(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    int nIdx = item->getTag();
    
    if ( _nIdxMenu == nIdx )
    {
        return;
    }
    
    _nIdxMenu = nIdx;
    
    listStoryTable.clear();
    if ( _nIdxMenu == 0 )
    {
        for(const auto &str : listStoryNormal) {
            listStoryTable.push_back(str);
        }
    }
    else if ( _nIdxMenu == 1 )
    {
        for(const auto &str : listStoryDevil) {
            listStoryTable.push_back(str);
        }
    }
    else if ( _nIdxMenu == 2 )
    {
        for(const auto &str : listStoryEvent) {
            listStoryTable.push_back(str);
        }
    }
    
    //
    drawMenu();
    drawContents(true);
}

void LayerStoryList::onClickStory(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    int nIdx = item->getTag();
    
    bool bFile = StoryManager::getInstance()->isStoryFile(nIdx);
    if ( bFile == false )
    {
        auto popup = PopupStoryDownloadList::create(nIdx);
        popup->setCallbackDownload(CC_CALLBACK_1(LayerStoryList::callbackDownload, this));
        popup->show();
        return;
    }
    
    bool bOpen = StoryManager::getInstance()->isStoryOpenException(nIdx);
    if ( _nIdxMenu == 2 )
    {// event 탭일때
        bOpen = true;
    }
    
    if ( bOpen == false )
    {
        return;
    }
    
    // show
    auto popup = PopupStory::create(nIdx);
    popup->show();
}

void LayerStoryList::onClickStoryDownload(cocos2d::Ref* sender)
{
    auto popup = PopupStoryDownloadList::create(0);
    popup->setCallbackDownload(CC_CALLBACK_1(LayerStoryList::callbackDownload, this));
    popup->show();
}

#pragma mark - callback
void LayerStoryList::callbackDownload(int nIdx)
{
    drawContents();
    
    // story open
    if ( nIdx != 0 )
    {
        bool bOpen = StoryManager::getInstance()->isStoryOpenException(nIdx);
        if ( _nIdxMenu == 2 )
        {// event 탭일때
            bOpen = true;
        }
        
        if ( bOpen == false )
        {
            return;
        }
        
        auto popup = PopupStory::create(nIdx);
        popup->show();
    }
}
