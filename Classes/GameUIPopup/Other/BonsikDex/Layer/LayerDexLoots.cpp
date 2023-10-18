//
//  LayerDexLoots.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/11.
//

#include "LayerDexLoots.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIComboBox.h"

#include "GameUIPopup/Other/BonsikDex/PopupBonsikSoulChange.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexLootTradingMarket.h"

#include "ManagerGame/LootsManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

LayerDexLoots* LayerDexLoots::create(cocos2d::Size size)
{
    LayerDexLoots* pRet = new(std::nothrow) LayerDexLoots();
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

LayerDexLoots::LayerDexLoots(void) :
_bInitUI(false),

//
_selectLootIdx(0),
_selectLootPieceGrade(0),
_selectLootPieceCount(0),

_callbackInfo(nullptr),

//
_layerContainerInfo(nullptr),
_layerContainerList(nullptr),
_layerContainerSynthetic(nullptr),
_layerContainerEffect(nullptr),

_uiList(nullptr)
{
    
}

LayerDexLoots::~LayerDexLoots(void)
{
    
}

bool LayerDexLoots::init(cocos2d::Size size)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    size.width = 728;
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::LOOT_TRADE, this);
    
    return true;
}

void LayerDexLoots::onEnter()
{
    LayerColor::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiSynthetic();
        uiList();
        uiEffect();
        
        //
        LootsManager::getInstance()->requestLootsInfo(true, CC_CALLBACK_1(LayerDexLoots::callbackInfo, this));
        
        //
        _bInitUI = true;
    }
    
    drawInfo();
    drawSynthetic();
    drawList();
    drawEffect();
}

#pragma mark - override : refresh
void LayerDexLoots::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::LOOT_TRADE )
    {
        _selectLootPieceGrade = 0;
        _selectLootPieceCount = 0;
        
        //
        drawSynthetic();
    }
}

#pragma mark - override : DelegateListView
void LayerDexLoots::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerDexLoots::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 170 + 10;

    return Size(width, height);
}

void LayerDexLoots::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, size.height - 10);
    
    // data
    
    // ui
    childview->removeAllChildren();
    
    auto uiCellGroup = ui::Layout::create();
    uiCellGroup->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiCellGroup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCellGroup->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiCellGroup);
    
    // obj
    ssize_t idxStart = idx * 5;
    ssize_t idxEnd = idxStart + 5;
    if ( idxEnd > _listLoots.size() )
    {
        idxEnd = _listLoots.size();
    }
    
    for ( int i = 0; idxStart + i < idxEnd; i++ )
    {
        ssize_t idxCell = idxStart + i;
        
        auto objLoot = _listLoots.at(idxCell);
        
        int idxLoot = objLoot->getIdx();
        
        int levelMax = objLoot->getLevelMax();
        int levelNow = LootsManager::getInstance()->getLootLevelNow(idxLoot);
        
        int pieceMax = LootsManager::getInstance()->getLootCountMax(objLoot->getGrade(), levelNow + 1);
        int pieceNow = LootsManager::getInstance()->getLootCountNow(idxLoot);
        int grade = objLoot->getGrade();
        
        int percent = (double)pieceNow / pieceMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        if ( levelNow >= levelMax )
            percent = 100;
        
        //
        auto sizeCell = Size(137, 170);
        
        auto uiCell = ui::Layout::create();
        uiCell->setContentSize(sizeCell);
        uiCell->setTouchEnabled(true);
        uiCell->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickLoot, this));
        uiCell->setTag((int)idxCell);
        uiCellGroup->addChild(uiCell);
        
        std::string pathGradeBG = "Assets/ui/bonsikdex/box_book_1_1.png";
        switch (grade)
        {
            case 2: pathGradeBG = "Assets/ui/bonsikdex/box_book_1_2.png"; break;
            case 3: pathGradeBG = "Assets/ui/bonsikdex/box_book_1_3.png"; break;
            case 4: pathGradeBG = "Assets/ui/bonsikdex/box_book_1_4.png"; break;
            case 5: pathGradeBG = "Assets/ui/bonsikdex/box_book_1_5.png"; break;
        }
        
        auto uiGradeBG = ui::ImageView::create(pathGradeBG);
        uiGradeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiGradeBG->setPosition(Vec2(sizeCell.width / 2, sizeCell.height));
        uiCell->addChild(uiGradeBG);
      
        auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/ui/bonsikdex/booty_%d.png", idxLoot));
        uiIcon->setPosition(Vec2(uiGradeBG->getContentSize().width / 2, uiGradeBG->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale(1.5);
        uiGradeBG->addChild(uiIcon);
        if ( levelNow == 0 )
        {
            uiIcon->setColor(Color3B(80, 80, 80));
        }
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/bonsikdex/book_collect_progress_bar1_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBarBG->setPosition(Vec2(sizeCell.width / 2, 0));
        uiCell->addChild(uiBarBG);
        
        std::string pathBar = "Assets/ui/bonsikdex/book_collect_progress_bar1_2.png";
        if ( percent == 100 )
        {
            pathBar = "Assets/ui/bonsikdex/book_collect_progress_bar1_3.png";
        }
        auto uiBar = ui::LoadingBar::create(pathBar);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBar->setPercent(percent);
        uiBarBG->addChild(uiBar);
        
        auto uiBarText = ui::CText::create(MafUtils::format("%d/%d", pieceNow, pieceMax), GAME_FONT, 16);
        uiBarText->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarBG->addChild(uiBarText);
        
        if ( idxCell == _selectLootIdx )
        {
            auto uiSelect = ui::ImageView::create("Assets/ui/bonsikdex/book_box_select.png");
            uiSelect->setPosition(Vec2(sizeCell.width / 2, sizeCell.height / 2));
            uiCell->addChild(uiSelect);
        }
    }
    
    //
    UtilsWidget::getInstance()->sortWidgetGrid(uiCellGroup, 5, sizeLayout.width, sizeLayout.height, 0, false, false);
}

int64_t LayerDexLoots::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listLoots.size() / 5;
    if ( _listLoots.size() % 5 != 0 )
        count++;
   
    return count;
}
 
#pragma mark - init
void LayerDexLoots::initVar()
{
    _listLoots = LootsManager::getInstance()->getListInfoLoots();
    std::sort(std::begin(_listLoots), std::end(_listLoots), [=](InfoLoot* a, InfoLoot* b) {
         
        bool result = a->getGrade() < b->getGrade();
        if ( a->getGrade() == b->getGrade() )
            result = a->getIdx() < b->getIdx();
        
        return result;
    });
}

void LayerDexLoots::initUi()
{
    
    
 
}

#pragma mark - ui
void LayerDexLoots::uiContainer()
{
    Size size = getContentSize();
    if ( size.width >= 750 )
        size.width = 750;
    
    //
    _layerContainerInfo = cocos2d::ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, 400) );
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_layerContainerInfo, 1);

    //
    _layerContainerSynthetic = cocos2d::ui::Layout::create();
    _layerContainerSynthetic->setContentSize( Size(size.width, 388) );
    _layerContainerSynthetic->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerSynthetic->setPosition(Vec2(size.width / 2, size.height - _layerContainerInfo->getContentSize().height - 5));
    addChild(_layerContainerSynthetic);

    //
    _layerContainerEffect = cocos2d::ui::Layout::create();
    _layerContainerEffect->setContentSize( Size(size.width, 50) );
    _layerContainerEffect->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerEffect->setPosition(Vec2(size.width / 2, 0));
    addChild(_layerContainerEffect);
    
    //
    _layerContainerList = cocos2d::ui::Layout::create();
    _layerContainerList->setContentSize( Size(size.width, size.height - _layerContainerInfo->getContentSize().height - 5 - _layerContainerSynthetic->getContentSize().height - _layerContainerEffect->getContentSize().height) );
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition(Vec2(size.width / 2, _layerContainerEffect->getPositionY() + _layerContainerEffect->getContentSize().height));
    addChild(_layerContainerList);
}

void LayerDexLoots::uiInfo()
{
    auto layout = _layerContainerInfo;
    auto size = layout->getContentSize();
    
    // 182 * 100
    auto uiBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg4_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    //
    auto uiDetailBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg4_2.png");
    uiDetailBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiDetailBG->setPosition(Vec2(layout->getContentSize().width - 13, layout->getContentSize().height / 2));
    layout->addChild(uiDetailBG);
    
    // 200 30
    auto uiEffect = ui::CText::create(" ", GAME_FONT, 24);
    uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiEffect->setPosition(Vec2(10, 19));
    uiEffect->setTextAreaSize(Size(200, 30));
    uiEffect->setTextOverflow(Label::Overflow::SHRINK);
    uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiEffect->setTextColor(Color4B(COLOR_TITLE));
    uiEffect->setName("UI_EFFECT");
    layout->addChild(uiEffect);
    
    auto uiIcon = ui::ImageView::create("Assets/ui/bonsikdex/booty_1.png");
    uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiIcon->setPosition(Vec2(169, 139));
    ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiIcon->setScale(2);
    uiIcon->setColor(Color3B(80, 80, 80));
    uiIcon->setName("UI_ICON");
    layout->addChild(uiIcon);
    
    auto uiName = ui::CText::create("LV0.", GAME_FONT, 30);
    uiName->setPosition(Vec2(size.width - 193, size.height - 78));
    uiName->setTextAreaSize(Size(260, uiName->getContentSize().height));
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiName->setTextColor(Color4B(102, 51, 0, 255));
    uiName->setName("UI_NAME");
    layout->addChild(uiName);

    auto uiDesc = ui::CText::create(" ", GAME_FONT, 20);
    uiDesc->setPosition(Vec2(size.width - 193, 146));
    uiDesc->setTextAreaSize(Size(246, 134));
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiDesc->setTextColor(Color4B(102, 51, 0, 255));
    uiDesc->setName("UI_DESC");
    layout->addChild(uiDesc);
        
    auto uiLevelUP = ui::Button::create("Assets/ui/bonsikdex/book_btn_level.png");
    uiLevelUP->setPosition(Vec2(size.width - 193, size.height - 140));
    uiLevelUP->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickLootLevelUp, this));
    uiLevelUP->setName("UI_LEVELUP");
    layout->addChild(uiLevelUP);
}

void LayerDexLoots::uiList()
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
    _uiList->setScrollBarAutoHideEnabled(false);
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void LayerDexLoots::uiSynthetic()
{
    auto layout = _layerContainerSynthetic;
    auto size = layout->getContentSize();
    
    // 182 * 97
    auto uiBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg4_3.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_loots_content_1"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, size.height - 5 - 22));
    uiDesc->setTextAreaSize(Size(size.width - 10, uiDesc->getContentSize().height));
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiDesc);
    
    auto uiGrade = ui::Layout::create();
    uiGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiGrade->setPosition(Vec2(size.width / 2, size.height - 170));
    uiGrade->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(uiGrade);
   
    std::vector<int> listGrade = {1, 2, 3, 4, 5};
    for ( int i = 0; i < listGrade.size(); i++ )
    {
        int grade = listGrade.at(i);
        
        auto sizeCell = Size(138, 170);
        
        auto uiCell = ui::Layout::create();
        uiCell->setContentSize(sizeCell);
        uiCell->setName(MafUtils::format("UI_GRADE_%d", grade));
        uiGrade->addChild(uiCell);
        if ( i != 0 )
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(5, 0, 0, 0));
            uiCell->setLayoutParameter(params);
        }
        
        auto uiGradeBG = ui::ImageView::create(MafUtils::format("Assets/ui/bonsikdex/box_book_1_%d.png", grade));
        uiGradeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiGradeBG->setPosition(Vec2(sizeCell.width / 2, sizeCell.height));
        uiCell->addChild(uiGradeBG);
      
        auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/ui/bonsikdex/booty0_%d.png", grade));
        uiIcon->setPosition(Vec2(uiGradeBG->getContentSize().width / 2, uiGradeBG->getContentSize().height / 2 + 10));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale(1.5);
        uiGradeBG->addChild(uiIcon);
        
        auto uiCount = ui::CText::create("0", GAME_FONT, 20);
        uiCount->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiCount->setPosition(Vec2(uiGradeBG->getContentSize().width - 5, uiGradeBG->getContentSize().height - 2));
        uiCount->setTextColor(Color4B(COLOR_TITLE));
        uiCount->setName("UI_COUNT");
        uiGradeBG->addChild(uiCount);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/bonsikdex/book_collect_progress_bar1_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBarBG->setPosition(Vec2(sizeCell.width / 2, 0));
        uiCell->addChild(uiBarBG);
        
        auto uiSyntheticCount = ui::CText::create("0", GAME_FONT, 20);
        uiSyntheticCount->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiSyntheticCount->setName("UI_SYNTHETIC_COUNT");
        uiBarBG->addChild(uiSyntheticCount);
        
        auto uiArrowUp = ui::Button::create("Assets/ui_common/btn_up_s.png");
        uiArrowUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiArrowUp->setPosition(Vec2(sizeCell.width / 2, sizeCell.height + 5));
        uiCell->addChild(uiArrowUp);
        {
            auto uiArrowUpClick = ui::Layout::create();
            uiArrowUpClick->setContentSize(Size(138, 25));
            uiArrowUpClick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiArrowUpClick->setPosition(Vec2(uiArrowUp->getContentSize().width / 2, uiArrowUp->getContentSize().height / 2));
            uiArrowUpClick->setTouchEnabled(true);
            uiArrowUpClick->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickLootPieceUp, this));
            uiArrowUpClick->setTag(grade);
            uiArrowUp->addChild(uiArrowUpClick);
        }
        
        auto uiArrowDown = ui::Button::create("Assets/ui_common/btn_down_s.png");
        uiArrowDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiArrowDown->setPosition(Vec2(sizeCell.width / 2, - 5));
        uiCell->addChild(uiArrowDown);
        {
            auto uiArrowUpClick = ui::Layout::create();
            uiArrowUpClick->setContentSize(Size(138, 25));
            uiArrowUpClick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiArrowUpClick->setPosition(Vec2(uiArrowDown->getContentSize().width / 2, uiArrowDown->getContentSize().height / 2));
            uiArrowUpClick->setTouchEnabled(true);
            uiArrowUpClick->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickLootPieceDown, this));
            uiArrowUpClick->setTag(grade);
            uiArrowDown->addChild(uiArrowUpClick);
        }
        
        auto uiBlack = ui::Layout::create();
        uiBlack->setContentSize(sizeCell);
        uiBlack->setBackGroundColor(Color3B::BLACK);
        uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiBlack->setOpacity(100);
        uiBlack->setName("UI_BLACK");
        uiCell->addChild(uiBlack);
    }
    
    auto sizeGrade = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiGrade);
    uiGrade->setContentSize(sizeGrade);
    uiGrade->requestDoLayout();
     
    //
    auto uiButtons = ui::Layout::create();
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPosition(Vec2(size.width / 2, 50));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(uiButtons);
    {
        auto uiBtnTrade = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiBtnTrade->setScale9Enabled(true);
        uiBtnTrade->setContentSize(Size(217, 76));
        uiBtnTrade->setCapInsets(Rect(33,33,2,2));
        uiBtnTrade->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickTrade, this));
        uiButtons->addChild(uiBtnTrade);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_msg_35"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtnTrade->getContentSize().width / 2, uiBtnTrade->getContentSize().height / 2));
            uiBtnTrade->addChild(uiText);
        }
        
        auto uiBtnSynthetic = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiBtnSynthetic->setScale9Enabled(true);
        uiBtnSynthetic->setContentSize(Size(217, 76));
        uiBtnSynthetic->setCapInsets(Rect(33,33,2,2));
        uiBtnSynthetic->addClickEventListener(CC_CALLBACK_1(LayerDexLoots::onClickSynthetic, this));
        uiButtons->addChild(uiBtnSynthetic);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiBtnSynthetic->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_loots_synthetic_1"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtnSynthetic->getContentSize().width / 2, uiBtnSynthetic->getContentSize().height / 2));
            uiBtnSynthetic->addChild(uiText);
        }
        
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
        uiButtons->setContentSize(size);
        uiButtons->requestDoLayout();
    }
}

void LayerDexLoots::uiEffect()
{
    auto layout = _layerContainerEffect;
    auto size = layout->getContentSize();
    
    auto uiEffect = ui::CText::create(GAME_TEXTFORMAT("t_ui_bonsikdex_total", "0"), GAME_FONT, 20);
    uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiEffect->setPosition(Vec2(24, size.height/2));
    uiEffect->setColor(Color3B(204, 204, 0));
    uiEffect->setName("UI_EFFECT");
    layout->addChild(uiEffect);
}

#pragma mark - draw
void LayerDexLoots::drawInfo()
{
    auto layout = _layerContainerInfo;
    
    auto objLoot = _listLoots.at(_selectLootIdx);
    
    int idxLoot = objLoot->getIdx();
    
    int levelMax = objLoot->getLevelMax();
    int levelNow = LootsManager::getInstance()->getLootLevelNow(idxLoot);
    
    int pieceMax = LootsManager::getInstance()->getLootCountMax(objLoot->getGrade(), levelNow + 1);
    int pieceNow = LootsManager::getInstance()->getLootCountNow(idxLoot);
    int grade = objLoot->getGrade();
    
    int percent = (double)pieceNow / pieceMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    if ( levelNow >= levelMax )
        percent = 100;
    
    double effect = 0;
    if ( levelNow > 0 )
    {
        effect = (objLoot->getEffectDefault() + objLoot->getEffectIncreasing() * (levelNow - 1)) * 100;
    }
    
    if ( auto uiEffect = layout->getChildByName<ui::CText*>("UI_EFFECT"); uiEffect != nullptr )
    {
        uiEffect->setString(GAME_TEXTFORMAT("t_costume_contents_head_40", effect));
    }
    
    if ( auto uiIcon = layout->getChildByName<ui::ImageView*>("UI_ICON"); uiIcon != nullptr )
    {
        uiIcon->loadTexture(MafUtils::format("Assets/ui/bonsikdex/booty_%d.png", idxLoot));
        uiIcon->setColor(Color3B::WHITE);
        if ( levelNow == 0 )
        {
            uiIcon->setColor(Color3B(80, 80, 80));
        }
    }
    
    if ( auto uiName = layout->getChildByName<ui::CText*>("UI_NAME"); uiName != nullptr )
    {
        uiName->setString(MafUtils::format("LV%d. %s", levelNow, objLoot->getName().c_str()));
    }
    
    if ( auto uiDesc = layout->getChildByName<ui::CText*>("UI_DESC"); uiDesc != nullptr )
    {
        uiDesc->setString(objLoot->getDesc());
    }
    
    if ( auto uiLevelUP = layout->getChildByName<ui::Button*>("UI_LEVELUP"); uiLevelUP != nullptr )
    {
        uiLevelUP->setColor(Color3B::WHITE);
        if ( levelNow <= 0 || pieceNow < pieceMax )
        {
            uiLevelUP->setColor(Color3B(80,80,80));
        }
        
        if ( levelNow >= levelMax )
        {
            uiLevelUP->setVisible(false);
        }
    }
}

void LayerDexLoots::drawSynthetic()
{
    auto layout = _layerContainerSynthetic;
    
    
    std::vector<int> listGrade = {1, 2, 3, 4, 5};
    for ( int i = 0; i < listGrade.size(); i++ )
    {
        int grade = listGrade.at(i);
        
        E_ITEMS eItemType = LootsManager::getInstance()->getGradeToItemsType(grade);
        
        auto count = ItemsManager::getInstance()->getItems(eItemType);
        auto countSynthetic = 0;
        if ( grade == _selectLootPieceGrade )
        {
            countSynthetic = _selectLootPieceCount;
        }
        
        auto uiCell = utils::findChild<ui::Layout*>(layout, MafUtils::format("UI_GRADE_%d", grade));
        if ( uiCell == nullptr )
            continue;
        
        if ( auto uiCount = utils::findChild<ui::CText*>(uiCell, "UI_COUNT"); uiCount != nullptr )
        {
            uiCount->setString(count);
        }
        
        if ( auto uiSyntheticCount = utils::findChild<ui::CText*>(uiCell, "UI_SYNTHETIC_COUNT"); uiSyntheticCount != nullptr )
        {
            uiSyntheticCount->setString(std::to_string(countSynthetic));
        }
        
        if ( auto uiBlack = utils::findChild<ui::Layout*>(uiCell, "UI_BLACK"); uiBlack != nullptr )
        {
            uiBlack->setVisible(false);
            
            if ( _selectLootPieceCount > 0 && countSynthetic <= 0 )
            {
                uiBlack->setVisible(true);
            }
        }
    }
    
    
}

void LayerDexLoots::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void LayerDexLoots::drawEffect()
{
    auto layout = _layerContainerEffect;
    auto size = layout->getContentSize();
    
    if ( auto uiEffect = layout->getChildByName<ui::CText*>("UI_EFFECT"); uiEffect != nullptr )
    {
        double effect = LootsManager::getInstance()->getEffect() * 100;
        
        std::string effectText = MafUtils::doubleToString(effect);
        uiEffect->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_total", effectText.c_str()));
    }
}

#pragma mark - set, get, add
void LayerDexLoots::setCallbackInfo(const std::function<void(bool)>& callback)
{
    _callbackInfo = callback;
}

#pragma mark - click
void LayerDexLoots::onClickLoot(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Layout*>(sender);
    
    //
    _selectLootIdx = uiBtn->getTag();
    
    //
    drawInfo();
    drawList();
}

void LayerDexLoots::onClickLootLevelUp(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objLoot = _listLoots.at(_selectLootIdx);
    
    int result = LootsManager::getInstance()->onLootsLevelUp(objLoot->getIdx(), CC_CALLBACK_2(LayerDexLoots::callbackLevelUp, this));
    if ( result != 0 )
    {
        /*
         0 : 성공
         1 : 조건 안됨
         2 : 최대 레벨
         */
        
        
        return;
    }
}

void LayerDexLoots::onClickLootPieceUp(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Widget*>(sender);
        
    int grade = uiBtn->getTag();
    if ( grade != _selectLootPieceGrade )
    {
        _selectLootPieceGrade = grade;
        _selectLootPieceCount = 0;
    }
    
    _selectLootPieceCount += 10;
    
    auto itemNow = _selectLootPieceCount;
    auto itemMax = std::atoi(ItemsManager::getInstance()->getItems(LootsManager::getInstance()->getGradeToItemsType(grade)).c_str());
    itemMax = MIN(itemMax, 300);
    if ( itemNow > itemMax )
    {
        _selectLootPieceCount = 0;
    }
    
    //
    drawSynthetic();
}

void LayerDexLoots::onClickLootPieceDown(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Widget*>(sender);
        
    int grade = uiBtn->getTag();
    if ( grade != _selectLootPieceGrade )
    {
        _selectLootPieceGrade = grade;
        _selectLootPieceCount = 0;
    }
    
    _selectLootPieceCount -= 10;
    
    auto itemMax = std::atoi(ItemsManager::getInstance()->getItems(LootsManager::getInstance()->getGradeToItemsType(grade)).c_str());
    itemMax = MIN(itemMax, 300);
    if ( _selectLootPieceCount < 0 )
    {
        _selectLootPieceCount = itemMax;
        _selectLootPieceCount /= 10;
        _selectLootPieceCount *= 10;
    }
        
    //
    drawSynthetic();
}

void LayerDexLoots::onClickTrade(cocos2d::Ref* sender)
{
    auto popup = PopupBonsikDexLootTradingMarket::create();
    popup->show();
}

void LayerDexLoots::onClickSynthetic(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( _selectLootPieceGrade == 0 || _selectLootPieceCount == 0 )
    {
        return;
    }
    
    int result = LootsManager::getInstance()->onLootPieceSynthesize(_selectLootPieceGrade, _selectLootPieceCount, CC_CALLBACK_2(LayerDexLoots::callbackSynthetic, this));
    if ( result != 0 )
    {
        /*
         0 : 성공
         1 : 재화 부족
         */
        
        
        return;
    }
}

#pragma mark - callback
void LayerDexLoots::callbackInfo(bool bResult)
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
    _selectLootPieceGrade = 0;
    _selectLootPieceCount = 0;
    
    // ui
    drawInfo();
    drawSynthetic();
    drawList();
    drawEffect();
}

void LayerDexLoots::callbackLevelUp(bool bResult, int nResult)
{
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    LootsManager::getInstance()->requestLootsInfo(true, CC_CALLBACK_1(LayerDexLoots::callbackInfo, this));
}

void LayerDexLoots::callbackSynthetic(bool bResult, int nResult)
{
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    
    //
    LootsManager::getInstance()->requestLootsInfo(true, CC_CALLBACK_1(LayerDexLoots::callbackInfo, this));
}
 
