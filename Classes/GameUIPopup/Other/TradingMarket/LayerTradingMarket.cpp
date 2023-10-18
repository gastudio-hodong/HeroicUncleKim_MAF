//
//  LayerTradingMarket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/11.
//

#include "LayerTradingMarket.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Prison/InfoTradingMarket.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIComboBox.h"

#include "ManagerGame/TradingMarketManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

LayerTradingMarket* LayerTradingMarket::create(int type)
{
    LayerTradingMarket* pRet = new(std::nothrow) LayerTradingMarket();
    if ( pRet && pRet->init(type) )
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

LayerTradingMarket::LayerTradingMarket(void) :
_bInitUI(false),

//
_type(0),

// widget
_layerContainerList(nullptr),

_uiList(nullptr)
{
    
}

LayerTradingMarket::~LayerTradingMarket(void)
{
    
}

bool LayerTradingMarket::init(int type)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //
    _type = type;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerTradingMarket::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
        
        //
        TradingMarketManager::getInstance()->requestInfo(true, _type, CC_CALLBACK_1(LayerTradingMarket::callbackInfo, this));
    }
    
    //
    drawList();
}

#pragma mark - override DelegateListView
void LayerTradingMarket::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size LayerTradingMarket::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 118 + 10;

    return Size(width, height);
}

void LayerTradingMarket::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, size.height - 10);
    
    // data
    auto obj = _listTradingMarket.at(idx);
    
    
    int countMax = obj->getCountMax();
    int countNow = TradingMarketManager::getInstance()->getCount(_type, obj->getIdx());
    int countAdd = 1;
    auto iter = _listCountAdd.find(obj->getIdx());
    if ( iter != _listCountAdd.end() )
    {
        countAdd = iter->second;
    }
    
    InfoItems* itemFrom = obj->getItemFrom();
    InfoItems* itemTo = obj->getItemTo();
    InfoItems* openCost = obj->getOpenCost();
    
    int itemFromCount = std::atoi(itemFrom->getCount().c_str()) * countAdd;
    int itemToCount = std::atoi(itemTo->getCount().c_str()) * countAdd;
    
    bool bOpen = TradingMarketManager::getInstance()->isOpen(_type, obj->getIdx());
    bool bOpenPrev = TradingMarketManager::getInstance()->isOpen(_type, obj->getOpenStep());
    
    std::string pathBG = "Assets/ui/prison/beelzebub/box_beelzebub_list2.png";
    if ( bOpen == true )
        pathBG = "Assets/ui/prison/beelzebub/box_beelzebub_list1.png";
    
    // ui
    childview->removeAllChildren();
    
    auto uiCell = ui::Layout::create();
    uiCell->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiCell->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCell->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiCell);
    
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setPosition(Vec2(uiCell->getContentSize().width / 2, uiCell->getContentSize().height / 2));
    uiCell->addChild(uiBG);
    
    if ( itemFrom == nullptr || itemTo == nullptr || openCost == nullptr )
    {
        return;
    }
    
    if ( bOpen == true )
    {
        auto uiDirectionBG = ui::ImageView::create("Assets/ui/prison/beelzebub/box_beelzebub_direction.png");
        uiDirectionBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiDirectionBG->setPosition(Vec2(0, sizeLayout.height / 2));
        uiCell->addChild(uiDirectionBG);
        
        // item
        auto uiItemFromBG = ui::Button::create("Assets/ui_common/box_item.png");
        uiItemFromBG->setScale9Enabled(true);
        uiItemFromBG->setContentSize(Size(82, 82));
        uiItemFromBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        uiItemFromBG->setPosition(Vec2(10, sizeLayout.height / 2));
        uiCell->addChild(uiItemFromBG);
        {
            //
            auto uiIcon = ui::ImageView::create(itemFrom->getPath());
            uiIcon->setPosition(Vec2(uiItemFromBG->getContentSize().width / 2, uiItemFromBG->getContentSize().height / 2));
            ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiItemFromBG->addChild(uiIcon);
        }
        
        // item count
        auto uiText = ui::CText::create(MafUtils::format("x %d", itemFromCount), GAME_FONT, 24);
        uiText->setPosition(Vec2(200, sizeLayout.height / 2));
        uiCell->addChild(uiText);
        
        auto uiBtnArrowL = ui::Button::create("Assets/ui_common/btn_quantity_down.png");
        uiBtnArrowL->setPosition(Vec2(200 - 60, sizeLayout.height / 2));
        uiBtnArrowL->addClickEventListener(CC_CALLBACK_1(LayerTradingMarket::onClickTradeArrowL, this));
        uiBtnArrowL->setTag((int)idx);
        uiCell->addChild(uiBtnArrowL);

        auto uiBtnArrowR = ui::Button::create("Assets/ui_common/btn_quantity_up.png");
        uiBtnArrowR->setPosition(Vec2(200 + 60, sizeLayout.height / 2));
        uiBtnArrowR->addClickEventListener(CC_CALLBACK_1(LayerTradingMarket::onClickTradeArrowR, this));
        uiBtnArrowR->setTag((int)idx);
        uiCell->addChild(uiBtnArrowR);
    }
    else
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_msg_34"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(10, sizeLayout.height / 2));
        uiText->setTextAreaSize(Size(350, uiText->getContentSize().height * 0.9));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiCell->addChild(uiText);
    }
    
    // item
    auto uiItemToBG = ui::Button::create("Assets/ui_common/box_item.png");
    uiItemToBG->setScale9Enabled(true);
    uiItemToBG->setContentSize(Size(82, 82));
    uiItemToBG->setPosition(Vec2(sizeLayout.width - 250, sizeLayout.height / 2 + 10));
    uiCell->addChild(uiItemToBG);
    {
        //
        auto uiIcon = ui::ImageView::create(itemTo->getPath());
        uiIcon->setPosition(Vec2(uiItemToBG->getContentSize().width / 2, uiItemToBG->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiItemToBG->addChild(uiIcon);

        //
        std::string itemToCountText = MafUtils::toString(itemToCount);
        if ( itemTo->getIdx() == (int)E_ITEMS::BUFF_ALL )
        {
            itemToCountText = GAME_TEXTFORMAT("t_ui_minute", itemToCount / 60);
        }
        auto uiText = ui::CText::create(itemToCountText, GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiText->setPosition(Vec2(uiItemToBG->getContentSize().width / 2, 5));
        uiItemToBG->addChild(uiText);
    }
    
    // button
    if ( bOpen == true )
    {
        std::string pathTrade = "Assets/ui_common/btn_1_1_on.png";
        
        auto itemNeed = std::to_string(itemFromCount);
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)itemFrom->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            pathTrade = "Assets/ui_common/btn_1_off.png";
        }
        
        if ( countMax != 0 )
        {
            if ( countNow >= countMax )
            {
                pathTrade = "Assets/ui_common/btn_1_off.png";
            }
        }
        
        auto uiBtnTrade = ui::Button::create(pathTrade);
        uiBtnTrade->setScale9Enabled(true);
        uiBtnTrade->setContentSize(Size(142, 86));
        uiBtnTrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBtnTrade->setPosition(Vec2(sizeLayout.width - 10, sizeLayout.height / 2));
        uiBtnTrade->addClickEventListener(CC_CALLBACK_1(LayerTradingMarket::onClickTrade, this));
        uiBtnTrade->setTag((int)idx);
        uiCell->addChild(uiBtnTrade);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_msg_35"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtnTrade->getContentSize().width / 2, uiBtnTrade->getContentSize().height / 2));
            uiBtnTrade->addChild(uiText);

            if ( countMax != 0 )
            {
                auto uiTextLimit = ui::CText::create(MafUtils::format("(%d/%d)", countMax - countNow, countMax), GAME_FONT, 16);
                uiTextLimit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiTextLimit->setPosition(Vec2(uiBtnTrade->getContentSize().width / 2, 5));
                uiBtnTrade->addChild(uiTextLimit);
            }
        }
    }
    else
    {
        std::string pathOpen = "Assets/ui_common/btn_1_1_on.png";
        
        auto itemNeed = openCost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)openCost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            pathOpen = "Assets/ui_common/btn_1_off.png";
        }
        
        if ( bOpenPrev == false )
        {
            pathOpen = "Assets/ui_common/btn_1_off.png";
        }
        
        auto uiBtnOpen = ui::Button::create(pathOpen);
        uiBtnOpen->setScale9Enabled(true);
        uiBtnOpen->setContentSize(Size(142, 86));
        uiBtnOpen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBtnOpen->setPosition(Vec2(uiBG->getContentSize().width - 10, uiBG->getContentSize().height / 2));
        uiBtnOpen->addClickEventListener(CC_CALLBACK_1(LayerTradingMarket::onClickTradeOpen, this));
        uiBtnOpen->setTag((int)idx);
        uiBG->addChild(uiBtnOpen);
        {
            auto uiIcon = ui::ImageView::create("Assets/icon/evilcrystal_2.png");
            uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiIcon->setPosition(Vec2(60, uiBtnOpen->getContentSize().height / 2));
            uiBtnOpen->addChild(uiIcon);

            auto uiText = ui::CText::create(MafUtils::format("x%s", openCost->getCount().c_str()), GAME_FONT, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiText->setPosition(Vec2(60, uiBtnOpen->getContentSize().height / 2));
            uiBtnOpen->addChild(uiText);
        }
    }
}

int64_t LayerTradingMarket::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listTradingMarket.size();
    
    return count;
}
 
#pragma mark - init
void LayerTradingMarket::initVar()
{
    _listTradingMarket = TradingMarketManager::getInstance()->getListInfoTradingMarketFromType(_type);
    
    for ( auto& obj : _listTradingMarket )
    {
        _listCountAdd[obj->getIdx()] = 1;
    }
}

void LayerTradingMarket::initUi()
{
    
    
 
}

#pragma mark - ui
void LayerTradingMarket::uiContainer()
{
    Size size = getContentSize();
    
    //
    _layerContainerList = ui::Layout::create();
    _layerContainerList->setContentSize( Size(size.width, size.height) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerList->setPosition( Vec2(size.width / 2, 0) );
    _layerContainerList->setLayoutType(ui::Layout::Type::HORIZONTAL);
    addChild(_layerContainerList);
}

void LayerTradingMarket::uiList()
{
    auto layout = _layerContainerList;
    auto size = layout->getContentSize();
    
    //580 + 40
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(layout->getContentSize().width / 2, 0));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->setScrollBarColor(Color3B::YELLOW);
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void LayerTradingMarket::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get
void LayerTradingMarket::setCallbackInfo(const std::function<void(bool)>& callback)
{
    _callbackInfo = callback;
}

#pragma mark - callback
void LayerTradingMarket::callbackInfo(bool bResult)
{
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(bResult);
    }
    
    if ( bResult == false )
    {
        return;
    }
    
    //
    for ( auto& obj : _listTradingMarket )
    {
        _listCountAdd[obj->getIdx()] = 1;
    }
    
    //
    drawList();
}

void LayerTradingMarket::callbackOpen(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    TradingMarketManager::getInstance()->requestInfo(true, _type, CC_CALLBACK_1(LayerTradingMarket::callbackInfo, this));
}

void LayerTradingMarket::callbackTrade(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    // 전리품 교환일때
    if ( _type == 1 )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::LOOT_TRADE);
    }
    
    //
    TradingMarketManager::getInstance()->requestInfo(true, _type, CC_CALLBACK_1(LayerTradingMarket::callbackInfo, this));
}

#pragma mark - schedule

#pragma mark - click
void LayerTradingMarket::onClickTradeOpen(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listTradingMarket.at(uiBtn->getTag());
    if ( obj == nullptr )
    {
        return;
    }
    
    int result = TradingMarketManager::getInstance()->onTradeOpen(_type, obj->getIdx(), CC_CALLBACK_1(LayerTradingMarket::callbackOpen, this));
    if ( result != 0 )
    {
        
    }
}

void LayerTradingMarket::onClickTrade(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listTradingMarket.at(uiBtn->getTag());
    if ( obj == nullptr )
    {
        return;
    }
    
    int idx = obj->getIdx();
    int count = _listCountAdd[idx];
    
    int result = TradingMarketManager::getInstance()->onTrade(_type, idx, count, CC_CALLBACK_1(LayerTradingMarket::callbackTrade, this));
    if ( result != 0 )
    {
        
    }
}

void LayerTradingMarket::onClickTradeArrowL(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listTradingMarket.at(uiBtn->getTag());
    
    int idx = obj->getIdx();
    
    int countNow = _listCountAdd[idx];
    int countMax = 0;
    if ( obj->getCountMax() == 0 )
    {
        int countItem = std::atoi(ItemsManager::getInstance()->getItems((E_ITEMS)obj->getItemFrom()->getIdx()).c_str());
        countMax = countItem / std::atoi(obj->getItemFrom()->getCount().c_str());
    }
    else
    {
        countMax = obj->getCountMax() - TradingMarketManager::getInstance()->getCount(_type, idx);
    }
    
    //
    countNow -= 1;
    if ( countNow <= 0 )
    {
        countNow = MAX(countMax, 1);
    }
    _listCountAdd[idx] = countNow;
    

    //
    drawList();
}

void LayerTradingMarket::onClickTradeArrowR(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listTradingMarket.at(uiBtn->getTag());
    
    int idx = obj->getIdx();
    
    
    int countNow = _listCountAdd[idx];
    int countMax = 0;
    if ( obj->getCountMax() == 0 )
    {
        int countItem = std::atoi(ItemsManager::getInstance()->getItems((E_ITEMS)obj->getItemFrom()->getIdx()).c_str());
        countMax = countItem / std::atoi(obj->getItemFrom()->getCount().c_str());
    }
    else
    {
        countMax = obj->getCountMax() - TradingMarketManager::getInstance()->getCount(_type, idx);
    }
    
    //
    countNow += 1;
    if ( countNow > countMax )
    {
        countNow = 1;
    }
    _listCountAdd[idx] = countNow;
    
    //
    drawList();
}

