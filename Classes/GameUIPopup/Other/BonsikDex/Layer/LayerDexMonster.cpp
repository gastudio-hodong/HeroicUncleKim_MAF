//
//  LayerDexMonster.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerDexMonster.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIComboBox.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikSoulChange.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/MonsterManager.h"

USING_NS_CC;

LayerDexMonster::LayerDexMonster(void) :
_bInitUI(false),

//
_nFloorSelectIdx(0),
_nMonsterSelectIdx(1),

//
_uiContentsInfo(nullptr),
_uiContentsList(nullptr),
_uiContentsEffect(nullptr),

_uiList(nullptr)
{
    
}

LayerDexMonster::~LayerDexMonster(void)
{
    
}

bool LayerDexMonster::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void LayerDexMonster::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        // init
        initVar();
        initUi();
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawList();
    drawEffect();
}

#pragma mark - override DelegateListView
void LayerDexMonster::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerDexMonster::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 180;

    return Size(width, height);
}

void LayerDexMonster::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 170);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);

    //
    ssize_t nIdxStart = idx * 5;
    ssize_t nIdxEnd = nIdxStart + 5;
    if ( nIdxEnd > _listMonster.size() )
    {
        nIdxEnd = _listMonster.size();
    }
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdxMonster = _listMonster.at(nIdxStart + i);
        
        int dexLevelMax = MonsterManager::getInstance()->getDexLevelMax();
        int dexLevelNow = MonsterManager::getInstance()->getDexLevelNow(nIdxMonster);
        
        int soulMax = MonsterManager::getInstance()->getDexInfoNeedCount(dexLevelNow + 1);
        int soulNow = MonsterManager::getInstance()->getSoulCountNow(nIdxMonster) - MonsterManager::getInstance()->getDexInfoNeedCountTotal(dexLevelNow);
            
        
        double posX = 138 * i + 10 * i;
        double posY = sizeLayout.height / 2;
        
        auto pathBG = "Assets/ui/bonsikdex/book_box_1_1.png";
        if ( dexLevelNow >= dexLevelMax )
        {
            pathBG = "Assets/ui/bonsikdex/book_box_2_1.png";
        }
        
        //
        auto uiItemBG = ui::Button::create(pathBG);
        uiItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemBG->setPosition(Vec2(posX, posY));
        uiItemBG->addClickEventListener(CC_CALLBACK_1(LayerDexMonster::onClickMonster, this));
        uiItemBG->setTag(nIdxMonster);
        uiLayout->addChild(uiItemBG);
        
        //
        auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/character_mon/normal/monster_%d.png", nIdxMonster));
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 33));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiItemBG->addChild(uiIcon);
        if ( dexLevelNow == 0 )
        {
            uiIcon->setColor(Color3B(80, 80, 80));
        }
        
        double scaleIcon = MIN((uiItemBG->getContentSize().width * 0.6) / uiIcon->getContentSize().width, (uiItemBG->getContentSize().height * 0.8) / uiIcon->getContentSize().height);
        if ( scaleIcon > 1 )
        {
            uiIcon->setScale(scaleIcon);
        }
        
        //
        if ( dexLevelNow < dexLevelMax )
        {
            int nPercent = (double)soulNow / soulMax * 100;
            if ( nPercent > 100 )
                nPercent = 100;

            std::string pathBar = "Assets/ui/bonsikdex/book_collect_progress_bar1_2.png";
            if ( nPercent == 100 )
            {
                pathBar = "Assets/ui/bonsikdex/book_collect_progress_bar1_3.png";
            }
            
            auto uiBar = ui::LoadingBar::create(pathBar);
            uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiBar->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 0));
            uiBar->setPercent(nPercent);
            uiItemBG->addChild(uiBar);
            
            auto uiBarText = ui::CText::create(MafUtils::format("%d/%d", soulNow, soulMax), GAME_FONT_AL, 20);
            uiBarText->setPosition(Vec2(uiBar->getContentSize().width / 2, uiBar->getContentSize().height / 2));
            uiBar->addChild(uiBarText);
        }
        
        //
        auto uiIdx = ui::CText::create(MafUtils::toString(nIdxMonster), GAME_FONT, 20);
        uiIdx->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiIdx->setPosition(Vec2(15, uiItemBG->getContentSize().height - 5));
        uiIdx->enableOutline(Color4B::BLACK, 2);
        uiItemBG->addChild(uiIdx);
        
        //
        if ( nIdxMonster == _nMonsterSelectIdx )
        {
            auto uiItemSelect = ui::ImageView::create("Assets/ui/bonsikdex/book_box_select.png");
            uiItemSelect->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
            uiItemBG->addChild(uiItemSelect, 10);
        }
        
        //
        auto uiLevel = ui::ImageView::create(MafUtils::format("Assets/ui/bonsikdex/box_lv_%d.png", dexLevelNow));
        uiLevel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiLevel->setPosition(Vec2(uiItemBG->getContentSize().width, uiItemBG->getContentSize().height));
        uiItemBG->addChild(uiLevel, 10);
    }
    
}

int64_t LayerDexMonster::listviewNumberOfCells(ui::CListView *listview)
{
    size_t size = _listMonster.size() / 5;
    if ( _listMonster.size() % 5 != 0 )
        size++;
    
    return size;
}
 

#pragma mark - init
void LayerDexMonster::initVar()
{
    setFloor();
    setMonster();
}

void LayerDexMonster::initUi()
{
    uiContainer();
    uiInfo();
    uiList();
    uiEffect();
}

#pragma mark - ui
void LayerDexMonster::uiContainer()
{
    Size size = getContentSize();
    
    // 182 * 100
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, 400 + 80));
    _uiContentsInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition( Vec2(size.width / 2, size.height) );
    addChild(_uiContentsInfo, 1);
    
    _uiContentsEffect = ui::Layout::create();
    _uiContentsEffect->setContentSize( Size(704, 185) );
    _uiContentsEffect->setIgnoreAnchorPointForPosition(false);
    _uiContentsEffect->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsEffect->setPosition( Vec2(size.width / 2, 0) );
    addChild(_uiContentsEffect, 1);
    
    _uiContentsList = ui::Layout::create();
    _uiContentsList->setContentSize( Size(size.width, size.height - _uiContentsInfo->getContentSize().height - _uiContentsEffect->getContentSize().height) );
    _uiContentsList->setIgnoreAnchorPointForPosition(false);
    _uiContentsList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsList->setPosition( Vec2(size.width / 2, _uiContentsEffect->getContentSize().height) );
    addChild(_uiContentsList);
}
void LayerDexMonster::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiInfoBG = ui::Layout::create();
    uiInfoBG->setContentSize(Size(728, 400));
    uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiInfoBG);
    {
        if ( auto uiBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg1_1.png"); uiBG != nullptr )
        {
            uiBG->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, uiInfoBG->getContentSize().height / 2));
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBG->setScale(4);
            uiInfoBG->addChild(uiBG);
        }
        
        if ( auto uiBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg2_1.png"); uiBG != nullptr )
        {
            uiBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiBG->setPosition(Vec2(uiInfoBG->getContentSize().width - 5, uiInfoBG->getContentSize().height - 5));
            uiInfoBG->addChild(uiBG);
        }
        
        auto uiLevelBG = ui::ImageView::create("Assets/ui_common/base_text_1_5.png");
        uiLevelBG->setScale9Enabled(true);
        uiLevelBG->setContentSize(Size(uiLevelBG->getContentSize().width, 44));
        uiLevelBG->setPosition(Vec2(170, uiInfoBG->getContentSize().height - 70));
        uiInfoBG->addChild(uiLevelBG);
        
        auto uiLevel = ui::CText::create("0", GAME_FONT, 28);
        uiLevel->setPosition(Vec2(uiLevelBG->getContentSize().width / 2, uiLevelBG->getContentSize().height / 2));
        uiLevel->enableOutline(Color4B::BLACK, 2);
        uiLevel->setName("UI_LEVEL");
        uiLevelBG->addChild(uiLevel);
        
        // monster icon
        auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/character_mon/normal/monster_%d.png", _nMonsterSelectIdx));
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiIcon->setPosition(Vec2(170, 40));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setName("UI_ICON");
        uiInfoBG->addChild(uiIcon);
        
        // monster bar
        auto uiBarBG = ui::ImageView::create("Assets/ui/bonsikdex/book_collect_progress_bar2_1.png");
        uiBarBG->setPosition(Vec2(170 , 40));
        uiInfoBG->addChild(uiBarBG);
        {
            auto uiBar = ui::LoadingBar::create("Assets/ui/bonsikdex/book_collect_progress_bar2_2.png");
            uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, 0));
            uiBar->setPercent(0);
            uiBar->setName("UI_MONSTER_BAR");
            uiBarBG->addChild(uiBar);
            
            auto uiBarText = ui::CText::create(MafUtils::format("%d/%d", 0, 0), GAME_FONT, 14);
            uiBarText->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiBarText->setName("UI_MONSTER_BAR_TEXT");
            uiBarBG->addChild(uiBarText);
            
            auto uiBarIcon = ui::ImageView::create("Assets/ui/bonsikdex/icon_essence_1.png");
            uiBarIcon->setPosition(Vec2(0, uiBarBG->getContentSize().height / 2));
            uiBarBG->addChild(uiBarIcon);
        }
        
        //
        
        auto uiDesc = ui::CText::create("", GAME_FONT, 20);
        uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiDesc->setPosition(Vec2(uiInfoBG->getContentSize().width - 175, uiInfoBG->getContentSize().height - 60));
        uiDesc->setTextAreaSize(Size(300, 136));
        uiDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDesc->setTextColor(Color4B(72, 46, 33, 255));
        uiDesc->setName("UI_DESC");
        uiInfoBG->addChild(uiDesc);
        {
            auto uiDescBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg2_4.png");
            uiDescBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiDesc->addChild(uiDescBG, -1);
        }
        
        auto uiDescFloor = ui::CText::create("", GAME_FONT, 20);
        uiDescFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiDescFloor->setPosition(Vec2(uiInfoBG->getContentSize().width - 175, uiInfoBG->getContentSize().height - 220));
        uiDescFloor->setTextColor(Color4B(133, 85, 69, 255));
        uiDescFloor->setName("UI_DESC_FLOOR");
        uiInfoBG->addChild(uiDescFloor);
        
        //
        auto uiEffectBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg3_1.png");
        uiEffectBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiEffectBG->setPosition(Vec2(uiInfoBG->getContentSize().width - 10, 10));
        uiInfoBG->addChild(uiEffectBG);
        {
            auto uiEffectLevel = ui::CText::create("", GAME_FONT, 20);
            uiEffectLevel->setPosition(Vec2(uiEffectBG->getContentSize().width / 2, uiEffectBG->getContentSize().height / 2 + 20));
            uiEffectLevel->setName("UI_EFFECT_LEVEL");
            uiEffectBG->addChild(uiEffectLevel);
            
            auto uiEffectValue = ui::CText::create("", GAME_FONT, 20);
            uiEffectValue->setPosition(Vec2(uiEffectBG->getContentSize().width / 2, uiEffectBG->getContentSize().height / 2 - 20));
            uiEffectValue->setTextColor(Color4B(160, 148, 109, 255));
            uiEffectValue->setName("UI_EFFECT_VALUE");
            uiEffectBG->addChild(uiEffectValue);
        }
        
        //
        auto uiLevelUp = ui::Button::create("Assets/ui/bonsikdex/book_bg3_2.png");
        uiLevelUp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiLevelUp->setPosition(Vec2(uiInfoBG->getContentSize().width - 10, 10));
        uiLevelUp->setVisible(false);
        uiLevelUp->addClickEventListener(CC_CALLBACK_1(LayerDexMonster::onClickLevelUp, this));
        uiLevelUp->setName("UI_LEVELUP");
        uiInfoBG->addChild(uiLevelUp);
        {
            auto uiText = ui::CText::create("LEVEL UP", GAME_FONT, 30);
            uiText->setPosition(Vec2(uiLevelUp->getContentSize().width / 2 - 30, uiLevelUp->getContentSize().height / 2));
            uiText->setTextColor(Color4B::YELLOW);
            uiLevelUp->addChild(uiText);
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
            objItem->setCount(100);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(62, 62));
            uiItemBG->setPosition(Vec2(uiLevelUp->getContentSize().width - 50, uiLevelUp->getContentSize().height / 2));
            uiLevelUp->addChild(uiItemBG);
            {
                auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiItemIcon, 10);
                
                std::string rewardCount = objItem->getCount();
                if ( objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                
                auto uiItemCount = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiItemCount->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiItemCount, 10);
                
                auto uiBonus = ui::ImageView::create("Assets/ui_common/bonus_2_1.png");
                uiBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBonus->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height));
                uiItemBG->addChild(uiBonus);
            }
        }
    }
    
    auto uiOtherBG = ui::Layout::create();
    uiOtherBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiOtherBG->setContentSize(Size(size.width, size.height - uiInfoBG->getContentSize().height));
    uiOtherBG->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(uiOtherBG);
    {
        ValueVector listRating;
        for ( int i = 0; i < _listFloor.size(); i++ )
        {
            listRating.push_back(Value(_listFloor.at(i)));
        }
        
        auto uiFloor = UIComboBox::create(listRating, UIComboBox::eType::type_down, "Assets/ui/bonsikdex/book_floors_1_1.png", "Assets/ui/bonsikdex/book_floors_1_2.png");
        uiFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiFloor->setIconArrow("Assets/ui/bonsikdex/book_floors_btn1_1.png", "Assets/ui/bonsikdex/book_floors_btn1_1.png");
        uiFloor->setDataSelect("ALL");
        uiFloor->setPosition(10, uiOtherBG->getContentSize().height / 2);
        uiFloor->setCallbackSelect(CC_CALLBACK_1(LayerDexMonster::callbackFloorSelect, this));
        uiOtherBG->addChild(uiFloor);
        
        //
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::MONSTER_SOUL_PIECE);
        
        auto uiCurrency = UICurrency::create(list, Size(170, 50));
        uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiCurrency->setPosition(Vec2(uiOtherBG->getContentSize().width - 145, uiOtherBG->getContentSize().height / 2));
        uiOtherBG->addChild(uiCurrency);
        
        //
        auto uiSoulChange = ui::Button::create("Assets/ui_common/btn_basic_box.png");
        uiSoulChange->setScale9Enabled(true);
        uiSoulChange->setCapInsets(Rect(11, 10, 20, 20));
        uiSoulChange->setContentSize(Size(138, 44));
        uiSoulChange->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiSoulChange->setPosition(Vec2(uiOtherBG->getContentSize().width - 5, uiOtherBG->getContentSize().height / 2));
        uiSoulChange->addClickEventListener(CC_CALLBACK_1(LayerDexMonster::onClickSoulChange, this));
        uiSoulChange->setTitleText(GAME_TEXT("t_ui_prison_msg_35"));
        uiSoulChange->setTitleFontSize(20);
        uiSoulChange->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        uiSoulChange->getTitleLabel()->setDimensions(uiSoulChange->getContentSize().width - 10, uiSoulChange->getTitleLabel()->getContentSize().height);
        uiSoulChange->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        uiOtherBG->addChild(uiSoulChange);
    }
}
void LayerDexMonster::uiList()
{
    auto layout = _uiContentsList;
    auto size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void LayerDexMonster::uiEffect()
{
    auto layout = _uiContentsEffect;
    auto size = layout->getContentSize();
    
    auto uiEffectBG = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiEffectBG->setScale9Enabled(true);
    uiEffectBG->setCapInsets(Rect(5,30,10,10));
    uiEffectBG->setContentSize(Size(704, 144));
    uiEffectBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiEffectBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiEffectBG);
    {
        auto uiBonusBG = ui::ImageView::create("Assets/ui_common/base_text_2_4.png");
        uiBonusBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBonusBG->setPosition(Vec2(13, uiEffectBG->getContentSize().height - 55));
        uiEffectBG->addChild(uiBonusBG);
        {
            auto uiBonusTotal = ui::CText::create("", GAME_FONT, 24);
            uiBonusTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBonusTotal->setPosition(Vec2(15, uiBonusBG->getContentSize().height / 2));
            uiBonusTotal->setName("UI_BONUS_TOTAL");
            uiBonusBG->addChild(uiBonusTotal);
        }
        
        auto uiHelp = ui::Button::create("Assets/ui_common/icon_info_1_2.png");
        uiHelp->setPosition(Vec2(uiEffectBG->getContentSize().width - 30, uiEffectBG->getContentSize().height - 27.5));
        uiHelp->addClickEventListener(CC_CALLBACK_1(LayerDexMonster::onClickHelp, this));
        uiEffectBG->addChild(uiHelp);
        
        auto uiEffectPrevBG = ui::ImageView::create("Assets/ui/bonsikdex/book_step2_1.png");
        uiEffectPrevBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiEffectPrevBG->setPosition(Vec2(13, uiEffectBG->getContentSize().height - 60));
        uiEffectPrevBG->setName("UI_BONUS_PREV");
        uiEffectBG->addChild(uiEffectPrevBG);
        {
            
        }
        
        auto uiEffectNextBG = ui::ImageView::create("Assets/ui/bonsikdex/book_step1_1.png");
        uiEffectNextBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiEffectNextBG->setPosition(Vec2(uiEffectPrevBG->getPositionX() + uiEffectPrevBG->getContentSize().width - 20, uiEffectPrevBG->getPositionY()));
        uiEffectNextBG->setName("UI_BONUS_NEXT");
        uiEffectBG->addChild(uiEffectNextBG);
        {
            
        }
    }
    
    auto uiEffectTotal = ui::CText::create("", GAME_FONT, 24);
    uiEffectTotal->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiEffectTotal->setPosition(Vec2(50, 0));
    uiEffectTotal->setTextColor(Color4B(204, 204, 0, 255));
    uiEffectTotal->setName("UI_EFFECT_TOTAL");
    uiEffectTotal->setTouchEnabled(true);
    uiEffectTotal->addClickEventListener([=](Ref* sender){
        
        int effect = MonsterManager::getInstance()->getDexEffect() * 100;
        std::string effectString = std::to_string(effect);
        effectString = MafUtils::addCommaToString(effectString, 3, ',');
        effectString.insert(0, "+");
        
        auto uiEffectTotal = (ui::CText*)sender;

        Vec2 posToast = uiEffectTotal->getParent()->convertToWorldSpace(uiEffectTotal->getPosition());
        posToast.x += uiEffectTotal->getContentSize().width / 2;
        posToast.y += uiEffectTotal->getContentSize().height / 2;
        posToast.y += 10;
        
        PopupToast::showImmediately(effectString, posToast.x, posToast.y);
        
    });
    layout->addChild(uiEffectTotal);
}
 
#pragma mark - draw
void LayerDexMonster::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    int dexLevelMax = MonsterManager::getInstance()->getDexLevelMax();
    int dexLevelNow = MonsterManager::getInstance()->getDexLevelNow(_nMonsterSelectIdx);
    
    int soulMax = MonsterManager::getInstance()->getDexInfoNeedCount(dexLevelNow + 1);
    int soulNow = MonsterManager::getInstance()->getSoulCountNow(_nMonsterSelectIdx) - MonsterManager::getInstance()->getDexInfoNeedCountTotal(dexLevelNow);
    
    //
    if ( auto uiIcon = utils::findChild<ui::ImageView*>(layout, "UI_ICON"); uiIcon != nullptr )
    {
        uiIcon->loadTexture(MafUtils::format("Assets/character_mon/normal/monster_%d.png", _nMonsterSelectIdx));
        uiIcon->setScale(MIN(150 / uiIcon->getContentSize().width, 150 / uiIcon->getContentSize().height));
        uiIcon->setColor(Color3B::WHITE);
        
        if ( dexLevelNow == 0 )
        {
            uiIcon->setColor(Color3B(80, 80, 80));
        }
    }
    
    //
    if ( auto uiBar = utils::findChild<ui::LoadingBar*>(layout, "UI_MONSTER_BAR"); uiBar != nullptr )
    {
        int percent = (double)soulNow / soulMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        uiBar->setPercent(percent);
    }
    
    if ( auto uiBarText = utils::findChild<ui::CText*>(layout, "UI_MONSTER_BAR_TEXT"); uiBarText != nullptr )
    {
        uiBarText->setString(MafUtils::format("%d/%d", soulNow, soulMax));
    }
    
    if ( auto uiLevel = utils::findChild<ui::CText*>(layout, "UI_LEVEL"); uiLevel != nullptr )
    {
        uiLevel->setString(MafUtils::format("Lv.%d", dexLevelNow));
    }
    
    if ( auto uiDesc = utils::findChild<ui::CText*>(layout, "UI_DESC"); uiDesc != nullptr )
    {
        std::string desc = MafUtils::format("%d. ???\n???", _nMonsterSelectIdx);
        desc.append("\n");
        desc.append(GAME_TEXT("t_ui_bonsikdex_msg_3"));
        if ( dexLevelNow > 1 )
        {
            desc = MafUtils::format("%d. %s", _nMonsterSelectIdx, GAME_TEXT(MafUtils::format("t_ui_bonsikdex_monster_title_%d", _nMonsterSelectIdx)).c_str());
            desc.append("\n");
            desc.append(GAME_TEXT(MafUtils::format("t_ui_bonsikdex_monster_contents_%d", _nMonsterSelectIdx)));
        }
        uiDesc->setString(desc);
    }
    
    if ( auto uiDescFloor = utils::findChild<ui::CText*>(layout, "UI_DESC_FLOOR"); uiDescFloor != nullptr )
    {
        std::string text = GAME_TEXT("t_ui_bonsikdex_monster_msg_3").append(" ").append(getMonsterToFloor(_nMonsterSelectIdx));
        uiDescFloor->setString(text);
    }
    
    if ( auto uiEffectLevel = utils::findChild<ui::CText*>(layout, "UI_EFFECT_LEVEL"); uiEffectLevel != nullptr )
    {
        uiEffectLevel->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_monster_lv_bonus_1", dexLevelNow));
    }
    
    if ( auto uiEffectValue = utils::findChild<ui::CText*>(layout, "UI_EFFECT_VALUE"); uiEffectValue != nullptr )
    {
        uiEffectValue->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_monster_lv_bonus_2", getEffectValue(dexLevelNow)));
        
    }
    
    if ( auto uiLevelUp = utils::findChild<ui::Button*>(layout, "UI_LEVELUP"); uiLevelUp != nullptr )
    {
        if ( soulNow >= soulMax && dexLevelNow < dexLevelMax )
        {// 레벨 조건이 되었을때 출력
            uiLevelUp->setVisible(true);
        }
        else
        {
            uiLevelUp->setVisible(false);
        }
    }
}

void LayerDexMonster::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void LayerDexMonster::drawEffect()
{
    auto layout = _uiContentsEffect;
    
    //
    if ( auto uiBonusTotal = utils::findChild<ui::CText*>(layout, "UI_BONUS_TOTAL"); uiBonusTotal != nullptr )
    {
        int dexBonusTotal = MonsterManager::getInstance()->getDexBonusEffect() * 100;
        uiBonusTotal->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_monster_msg_1", dexBonusTotal));
    }
    
    //
    if ( auto uiEffectPrevBG = utils::findChild<ui::ImageView*>(layout, "UI_BONUS_PREV"); uiEffectPrevBG != nullptr )
    {
        uiEffectPrevBG->removeAllChildren();
        
        int dexBonusLevelNow = MonsterManager::getInstance()->getDexBonusLevelNow();
        int dexBonusNeedCount = MonsterManager::getInstance()->getDexBonusInfoNeedCount(dexBonusLevelNow);
        int dexBonusNeedDexLevel = MonsterManager::getInstance()->getDexBonusInfoNeedDexLevel(dexBonusLevelNow);
        int dexBonusEffect = MonsterManager::getInstance()->getDexBonusInfoEffect(dexBonusLevelNow) * 100;
        
        std::string title = GAME_TEXT("t_ui_bonsikdex_monster_msg_2_non");
        if ( dexBonusLevelNow > 0 )
            title = GAME_TEXTFORMAT("t_ui_bonsikdex_monster_msg_2", dexBonusLevelNow, dexBonusNeedDexLevel, dexBonusNeedCount);
        
        auto uiEffectTitle = ui::CText::create(title, GAME_FONT, 20);
        uiEffectTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiEffectTitle->setPosition(Vec2(15, uiEffectPrevBG->getContentSize().height * 0.75));
        uiEffectTitle->setTextAreaSize(Size(290, uiEffectTitle->getContentSize().height));
        uiEffectTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiEffectTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffectTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiEffectPrevBG->addChild(uiEffectTitle);
        
        auto uiIcon = ui::ImageView::create("Assets/ui_common/icon_rage1_1.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIcon->setPosition(Vec2(15, uiEffectPrevBG->getContentSize().height * 0.25));
        uiEffectPrevBG->addChild(uiIcon);
        
        auto uiEffectValue = ui::CText::create(MafUtils::format("+%d%%", dexBonusEffect), GAME_FONT, 20);
        uiEffectValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiEffectValue->setPosition(Vec2(uiIcon->getPositionX() + 45, uiEffectPrevBG->getContentSize().height * 0.25 + 4));
        uiEffectValue->setTextColor(Color4B::YELLOW);
        uiEffectPrevBG->addChild(uiEffectValue);
    }
    
    //
    if ( auto uiEffectNextBG = utils::findChild<ui::ImageView*>(layout, "UI_BONUS_NEXT"); uiEffectNextBG != nullptr )
    {
        uiEffectNextBG->removeAllChildren();
        
        int dexBonusLevelMax = MonsterManager::getInstance()->getDexBonusLevelMax();
        int dexBonusLevelNext = MonsterManager::getInstance()->getDexBonusLevelNow() + 1;
        
        int dexBonusNeedCount = MonsterManager::getInstance()->getDexBonusInfoNeedCount(dexBonusLevelNext);
        int dexBonusNeedDexLevel = MonsterManager::getInstance()->getDexBonusInfoNeedDexLevel(dexBonusLevelNext);
        
        int dexBonusEffect = MonsterManager::getInstance()->getDexBonusInfoEffect(dexBonusLevelNext) * 100;
        
        if ( dexBonusLevelNext <= dexBonusLevelMax )
        {
            auto uiEffectTitle = ui::CText::create(GAME_TEXTFORMAT("t_ui_bonsikdex_monster_msg_2", dexBonusLevelNext, dexBonusNeedDexLevel, dexBonusNeedCount), GAME_FONT, 20);
            uiEffectTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiEffectTitle->setPosition(Vec2(50, uiEffectNextBG->getContentSize().height * 0.75));
            uiEffectTitle->setTextAreaSize(Size(260, uiEffectTitle->getContentSize().height));
            uiEffectTitle->setTextOverflow(Label::Overflow::SHRINK);
            uiEffectTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiEffectTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiEffectNextBG->addChild(uiEffectTitle);
            
            auto uiIcon = ui::ImageView::create("Assets/ui_common/icon_rage1_1.png");
            uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiIcon->setPosition(Vec2(50, uiEffectNextBG->getContentSize().height * 0.25));
            uiEffectNextBG->addChild(uiIcon);
            
            auto uiEffectValue = ui::CText::create(MafUtils::format("+%d%%", dexBonusEffect), GAME_FONT, 20);
            uiEffectValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiEffectValue->setPosition(Vec2(uiIcon->getPositionX() + 45, uiEffectNextBG->getContentSize().height * 0.25 + 4));
            uiEffectValue->setTextColor(Color4B::YELLOW);
            uiEffectNextBG->addChild(uiEffectValue);
        }
        else
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_bonsikdex_monster_msg_2_max"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiEffectNextBG->getContentSize().width / 2, uiEffectNextBG->getContentSize().height / 2));
            uiText->setTextColor(Color4B(128, 128, 128, 255));
            uiEffectNextBG->addChild(uiText);
        }
    }
    
    //
    if ( auto uiEffectTotal = utils::findChild<ui::CText*>(layout, "UI_EFFECT_TOTAL"); uiEffectTotal != nullptr )
    {
        int effect = MonsterManager::getInstance()->getDexEffect() * 100;
        std::string effectString = std::to_string(effect);
        effectString = MafUtils::convertNumberToShort(effectString);
        
        uiEffectTotal->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_total",  effectString.c_str()));
    }
    
}

#pragma mark - callback
void LayerDexMonster::callbackFloorSelect(std::string strFloor)
{
    _nFloorSelectIdx = getFloorIdx(strFloor);
    
    //
    setMonster();
 
    //
    _nMonsterSelectIdx = _listMonster.at(0);
    
    //
    drawInfo();
    
    //
    _uiList->reloadData();
}

void LayerDexMonster::callbackSoulChange()
{
    drawInfo();
    drawList();
}

#pragma mark - click
void LayerDexMonster::onClickHelp(cocos2d::Ref* sender)
{
    auto popup = PopupHelp::create(4, "t_ui_bonsikdex_info_title_%d", "t_ui_bonsikdex_info_msg_%d");
    popup->show();
}

void LayerDexMonster::onClickMonster(cocos2d::Ref *sender)
{
    auto item = (MenuItem*)sender;
    
    _nMonsterSelectIdx = item->getTag();
    
    //
    drawInfo();
    drawList();
}
   
void LayerDexMonster::onClickLevelUp(cocos2d::Ref *sender)
{
    int dexLevelMax = MonsterManager::getInstance()->getDexLevelMax();
    int dexLevelNow = MonsterManager::getInstance()->getDexLevelNow(_nMonsterSelectIdx);
    
    int soulMax = MonsterManager::getInstance()->getDexInfoNeedCount(dexLevelNow + 1);
    int soulNow = MonsterManager::getInstance()->getSoulCountNow(_nMonsterSelectIdx) - MonsterManager::getInstance()->getDexInfoNeedCountTotal(dexLevelNow);
    
    if ( dexLevelNow >= dexLevelMax )
    {
        return;
    }
    
    if ( soulNow < soulMax )
    {
        return;
    }
    
    //
    MonsterManager::getInstance()->addDexLevel(_nMonsterSelectIdx);
    MonsterManager::getInstance()->saveData();
    
    //
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, MonsterManager::getInstance()->isSoulLevelComplete());
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP);
    
    //
    dexLevelNow = MonsterManager::getInstance()->getDexLevelNow(_nMonsterSelectIdx);
    
    //
    drawInfo();
    drawList();
    drawEffect();
    
    //
    ItemsManager::getInstance()->addItem(E_ITEMS::GEM, 0, 100);
    
    // analytics
    if ( dexLevelNow == 1 )               MafAnalyticsManager::LogEvent(kAnalPediaLevel1, kRepeatTrue);
    else if ( dexLevelNow == 2 )          MafAnalyticsManager::LogEvent(kAnalPediaLevel2, kRepeatTrue);
    else if ( dexLevelNow == 3 )          MafAnalyticsManager::LogEvent(kAnalPediaLevel3, kRepeatTrue);
    else if ( dexLevelNow == 4 )          MafAnalyticsManager::LogEvent(kAnalPediaLevel4, kRepeatTrue);
    else if ( dexLevelNow == 5 )          MafAnalyticsManager::LogEvent(kAnalPediaLevel5, kRepeatTrue);
    
}

void LayerDexMonster::onClickSoulChange(cocos2d::Ref *sender)
{
    auto popup = PopupBonsikSoulChange::create();
    popup->setCallbackResult(CC_CALLBACK_0(LayerDexMonster::callbackSoulChange, this));
    popup->show();
}

#pragma mark - set, get, add
void LayerDexMonster::setFloor()
{
    _listFloor.clear();
    _listFloor.push_back("ALL");
    _listFloor.push_back("1~1000");
    _listFloor.push_back("1001~2000");
    _listFloor.push_back("2001~3000");
    _listFloor.push_back("3001~4000");
    _listFloor.push_back("4001~5000");
    _listFloor.push_back("5001~6000");
    _listFloor.push_back("6001~7000");
    _listFloor.push_back("7001~8000");
    _listFloor.push_back("8001~9000");
    _listFloor.push_back("9001~10000");
    _listFloor.push_back("10001~12000");
    _listFloor.push_back("12001~13000");
    _listFloor.push_back("14001~16000");
    _listFloor.push_back("16001~18000");
    _listFloor.push_back("18001~20000");
    _listFloor.push_back("20001~30000");
    _listFloor.push_back("30001~");
}

int LayerDexMonster::getFloorIdx(std::string strFloor)
{
    int nFloorIdx = 0;
    
    for ( int i = 0; i < _listFloor.size(); i++ )
    {
        if ( _listFloor.at(i).compare(strFloor) == 0 )
        {
            nFloorIdx = i;
            break;
        }
    }
    
    return nFloorIdx;
}

std::string LayerDexMonster::getMonsterToFloor(int idx)
{
    int listFloor1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int listFloor1001[] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    int listFloor2001[] = {31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};
    int listFloor3001[] = {61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor4001[] = {61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor5001[] = {46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor6001[] = {76,77,78,79,80,81,82,83,84,85,86,87,88,89,90};
    int listFloor7001[] = {91,92,93,94,95,96,97,98,99,100,101,102,103,104,105};
    int listFloor8001[] = {106,107,108,109,110,111,112,113,114,115,116,117,118,119,120};
    int listFloor9001[] = {121,122,123,124,125,126,127,128,129,130,131,132,133,134,135};
    int listFloor10001[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int listFloor12001[] = {136,137,138,139,140,141,142,143,144,145,146,147,148,149,150};
    int listFloor14001[] = {151,152,153,154,155,156,157,158,159,160,161,162,163,164,165};
    int listFloor16001[] = {166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
    int listFloor18001[] = {181,182,183,184,185,186,187,188,189,190,191,192,193,194,195};
    int listFloor20001[] = {196,197,198,199,200,201,202,203,204,205,206,207,208,209,210};
    int listFloor30001[] = {211,212,213,214,215,216,217,218,219,220,221,222,223,224,225};
    
    if ( auto itr = std::find(listFloor1, listFloor1 + sizeof(listFloor1)/sizeof(listFloor1[0]), idx); itr != std::end(listFloor1) )
    {
        return "1~1000";
    }
    
    if ( auto itr = std::find(listFloor1001, listFloor1001 + sizeof(listFloor1001)/sizeof(listFloor1001[0]), idx); itr != std::end(listFloor1001) )
    {
        return "1001~2000";
    }
    
    if ( auto itr = std::find(listFloor2001, listFloor2001 + sizeof(listFloor2001)/sizeof(listFloor2001[0]), idx); itr != std::end(listFloor2001) )
    {
        return "2001~3000";
    }
    
    if ( auto itr = std::find(listFloor3001, listFloor3001 + sizeof(listFloor3001)/sizeof(listFloor3001[0]), idx); itr != std::end(listFloor3001) )
    {
        return "3001~4000";
    }
    
    if ( auto itr = std::find(listFloor4001, listFloor4001 + sizeof(listFloor4001)/sizeof(listFloor4001[0]), idx); itr != std::end(listFloor4001) )
    {
        return "4001~5000";
    }
    
    if ( auto itr = std::find(listFloor5001, listFloor5001 + sizeof(listFloor5001)/sizeof(listFloor5001[0]), idx); itr != std::end(listFloor5001) )
    {
        return "5001~6000";
    }
    
    if ( auto itr = std::find(listFloor6001, listFloor6001 + sizeof(listFloor6001)/sizeof(listFloor6001[0]), idx); itr != std::end(listFloor6001) )
    {
        return "6001~7000";
    }
    
    if ( auto itr = std::find(listFloor7001, listFloor7001 + sizeof(listFloor7001)/sizeof(listFloor7001[0]), idx); itr != std::end(listFloor7001) )
    {
        return "7001~8000";
    }
    
    if ( auto itr = std::find(listFloor8001, listFloor8001 + sizeof(listFloor8001)/sizeof(listFloor8001[0]), idx); itr != std::end(listFloor8001) )
    {
        return "8001~9000";
    }
    
    if ( auto itr = std::find(listFloor9001, listFloor9001 + sizeof(listFloor9001)/sizeof(listFloor9001[0]), idx); itr != std::end(listFloor9001) )
    {
        return "9001~10000";
    }
    
    if ( auto itr = std::find(listFloor10001, listFloor10001 + sizeof(listFloor10001)/sizeof(listFloor10001[0]), idx); itr != std::end(listFloor10001) )
    {
        return "10001~12000";
    }
    
    if ( auto itr = std::find(listFloor12001, listFloor12001 + sizeof(listFloor12001)/sizeof(listFloor12001[0]), idx); itr != std::end(listFloor12001) )
    {
        return "12001~13000";
    }
    
    if ( auto itr = std::find(listFloor14001, listFloor14001 + sizeof(listFloor14001)/sizeof(listFloor14001[0]), idx); itr != std::end(listFloor14001) )
    {
        return "14001~16000";
    }
    
    if ( auto itr = std::find(listFloor16001, listFloor16001 + sizeof(listFloor16001)/sizeof(listFloor16001[0]), idx); itr != std::end(listFloor16001) )
    {
        return "16001~18000";
    }
    
    if ( auto itr = std::find(listFloor18001, listFloor18001 + sizeof(listFloor18001)/sizeof(listFloor18001[0]), idx); itr != std::end(listFloor18001) )
    {
        return "18001~20000";
    }
    
    if ( auto itr = std::find(listFloor20001, listFloor20001 + sizeof(listFloor20001)/sizeof(listFloor20001[0]), idx); itr != std::end(listFloor20001) )
    {
        return "20001~30000";
    }
    
    if ( auto itr = std::find(listFloor30001, listFloor30001 + sizeof(listFloor30001)/sizeof(listFloor30001[0]), idx); itr != std::end(listFloor30001) )
    {
        return "30001~";
    }
 
    return "";
}
void LayerDexMonster::setMonster()
{
    _listMonster.clear();
    
    int listFloor1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int listFloor1001[] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    int listFloor2001[] = {31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};
    int listFloor3001[] = {61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor4001[] = {61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor5001[] = {46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75};
    int listFloor6001[] = {76,77,78,79,80,81,82,83,84,85,86,87,88,89,90};
    int listFloor7001[] = {91,92,93,94,95,96,97,98,99,100,101,102,103,104,105};
    int listFloor8001[] = {106,107,108,109,110,111,112,113,114,115,116,117,118,119,120};
    int listFloor9001[] = {121,122,123,124,125,126,127,128,129,130,131,132,133,134,135};
    int listFloor10001[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int listFloor12001[] = {136,137,138,139,140,141,142,143,144,145,146,147,148,149,150};
    int listFloor14001[] = {151,152,153,154,155,156,157,158,159,160,161,162,163,164,165};
    int listFloor16001[] = {166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
    int listFloor18001[] = {181,182,183,184,185,186,187,188,189,190,191,192,193,194,195};
    int listFloor20001[] = {196,197,198,199,200,201,202,203,204,205,206,207,208,209,210};
    int listFloor30001[] = {211,212,213,214,215,216,217,218,219,220,221,222,223,224,225};
    
    switch (_nFloorSelectIdx) {
        case 0:
        {
            for ( int i = 1; i <= 225; i++)
            {
                _listMonster.push_back(i);
            }
            break;
        }
        case 1:     _listMonster.assign(std::begin(listFloor1), std::end(listFloor1));              break;
        case 2:     _listMonster.assign(std::begin(listFloor1001), std::end(listFloor1001));        break;
        case 3:     _listMonster.assign(std::begin(listFloor2001), std::end(listFloor2001));        break;
        case 4:     _listMonster.assign(std::begin(listFloor3001), std::end(listFloor3001));        break;
        case 5:     _listMonster.assign(std::begin(listFloor4001), std::end(listFloor4001));        break;
        case 6:     _listMonster.assign(std::begin(listFloor5001), std::end(listFloor5001));        break;
        case 7:     _listMonster.assign(std::begin(listFloor6001), std::end(listFloor6001));        break;
        case 8:     _listMonster.assign(std::begin(listFloor7001), std::end(listFloor7001));        break;
        case 9:     _listMonster.assign(std::begin(listFloor8001), std::end(listFloor8001));        break;
        case 10:    _listMonster.assign(std::begin(listFloor9001), std::end(listFloor9001));        break;
        case 11:    _listMonster.assign(std::begin(listFloor10001), std::end(listFloor10001));      break;
        case 12:    _listMonster.assign(std::begin(listFloor12001), std::end(listFloor12001));      break;
        case 13:    _listMonster.assign(std::begin(listFloor14001), std::end(listFloor14001));      break;
        case 14:    _listMonster.assign(std::begin(listFloor16001), std::end(listFloor16001));      break;
        case 15:    _listMonster.assign(std::begin(listFloor18001), std::end(listFloor18001));      break;
        case 16:    _listMonster.assign(std::begin(listFloor20001), std::end(listFloor20001));      break;
        case 17:    _listMonster.assign(std::begin(listFloor30001), std::end(listFloor30001));      break;
        default:
            break;
    }
}

int LayerDexMonster::getEffectValue(int level)
{
    return level;
}
