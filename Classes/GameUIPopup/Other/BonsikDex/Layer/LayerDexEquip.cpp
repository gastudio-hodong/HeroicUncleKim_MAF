//
//  LayerDexEquip.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerDexEquip.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUnit/UnitBehaviorHero.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/Equip/PopupWeaponReinforce.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

LayerDexEquip::LayerDexEquip(void) :
_bInitUI(false),

//
_nWeaponSelectIdx(1),

//
_uiContentsInfo(nullptr),
_uiContentsList(nullptr),
_uiContentsEffect(nullptr),

_uiList(nullptr)
{
    
}

LayerDexEquip::~LayerDexEquip(void)
{
    
}

bool LayerDexEquip::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void LayerDexEquip::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        // init
        initVar();
        initUi();
        
        //
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_REINFORCE, this);
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawList();
    drawEffect();
    
    //
    _uiList->jumpToItem(_nWeaponSelectIdx / 5, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

#pragma mark - override DelegateListView
void LayerDexEquip::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerDexEquip::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 180;

    return Size(width, height);
}

void LayerDexEquip::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
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
    if ( nIdxEnd > _listWeapon.size() )
    {
        nIdxEnd = _listWeapon.size();
    }
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int idxWeapon = (int)nIdxStart + i + 1;
        
        auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(idxWeapon);
        if ( objWeapon == nullptr )
        {
            continue;
        }
        
        int reinforceNow = WeaponManager::getInstance()->getReinforce(idxWeapon);
        int reinforceMax = objWeapon->getEnhanceMax();
            
        double posX = 138 * i + 10 * i;
        double posY = sizeLayout.height / 2;
        
        auto pathBG = "Assets/ui/bonsikdex/book_box_1_1.png";
        if ( objWeapon->getLegend() > 1 )
        {
            pathBG = "Assets/ui/bonsikdex/book_box_3_2.png";
        }
        else if ( objWeapon->getLegend() > 0 )
        {
            pathBG = "Assets/ui/bonsikdex/book_box_3_1.png";
        }
        
        //
        auto uiItemBG = ui::Button::create(pathBG);
        uiItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemBG->setPosition(Vec2(posX, posY));
        uiItemBG->addClickEventListener(CC_CALLBACK_1(LayerDexEquip::onClickWeapon, this));
        uiItemBG->setTag(idxWeapon);
        uiLayout->addChild(uiItemBG);
        
        //
        auto uiIcon = ui::ImageView::create(WeaponManager::getInstance()->getImagePath(idxWeapon));
        uiIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 16.5));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiItemBG->addChild(uiIcon, 1);
        
        double scaleIcon = MIN((uiItemBG->getContentSize().width * 0.6) / uiIcon->getContentSize().width, (uiItemBG->getContentSize().height * 0.8) / uiIcon->getContentSize().height);
        if ( scaleIcon > 1 )
        {
            uiIcon->setScale(scaleIcon);
        }
        
        if ( objWeapon->getLegend() > 1 )
        {
            auto uiEffect = ui::ImageView::create("Assets/ui/mine/week_weaponbox_2_2.png");
            uiEffect->setPosition(uiIcon->getPosition());
            uiEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
            uiItemBG->addChild(uiEffect);
        }
        
        //
        std::string pathCollect = "Assets/ui/bonsikdex/book_box_1_3.png";
        if( idxWeapon <= WeaponManager::getInstance()->getEquipPermanent() )
            pathCollect = "Assets/ui/bonsikdex/book_box_1_4.png";
        
        auto uiCollect = Sprite::create(pathCollect);
        uiCollect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiCollect->setPosition(uiItemBG->getContentSize().width / 2, 0);
        uiItemBG->addChild(uiCollect);
        
   
        auto uiDisable = ui::ImageView::create(PIXEL);
        uiDisable->setScale9Enabled(true);
        uiDisable->setContentSize(uiItemBG->getContentSize());
        uiDisable->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
        uiDisable->setColor(Color3B::BLACK);
        uiDisable->setOpacity(150);
        uiDisable->setVisible(false);
        uiItemBG->addChild(uiDisable, 1);
        
        if( idxWeapon > WeaponManager::getInstance()->getEquipPermanent() + 1 )
        {
            uiDisable->setVisible(true);
        }
        
        //
        auto uiIdx = ui::CText::create(MafUtils::toString(idxWeapon), GAME_FONT, 20);
        uiIdx->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiIdx->setPosition(Vec2(15, uiItemBG->getContentSize().height - 5));
        uiIdx->enableOutline(Color4B::BLACK, 2);
        uiItemBG->addChild(uiIdx, 1);
        
        //
        if ( idxWeapon == _nWeaponSelectIdx )
        {
            auto uiItemSelect = ui::ImageView::create("Assets/ui/bonsikdex/book_box_select.png");
            uiItemSelect->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
            uiItemBG->addChild(uiItemSelect, 10);
        }
        
        //
        std::string pathReinforce = "Assets/ui/bonsikdex/book_rating_icon_1_1.png";
        if ( reinforceNow >= reinforceMax )
        {
            pathReinforce = "Assets/ui/bonsikdex/book_rating_icon_1_2.png";
        }
        auto uiReinforce = ui::ImageView::create(pathReinforce);
        uiReinforce->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiReinforce->setPosition(Vec2(uiItemBG->getContentSize().width, uiItemBG->getContentSize().height + 2));
        uiItemBG->addChild(uiReinforce, 10);
        {
            auto uiText = ui::CText::create(MafUtils::toString(reinforceNow), GAME_FONT, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiText->setPosition(Vec2(uiReinforce->getContentSize().width - 10, uiReinforce->getContentSize().height / 2));
            uiText->setTextColor(Color4B::YELLOW);
            uiReinforce->addChild(uiText);
        }
    }
    
}

int64_t LayerDexEquip::listviewNumberOfCells(ui::CListView *listview)
{
    size_t size = _listWeapon.size() / 5;
    if ( _listWeapon.size() % 5 != 0 )
        size++;
    
    return size;
}

#pragma mark - refresh
void LayerDexEquip::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::WEAPON_BUY_REINFORCE )
    {
        drawInfo();
        drawList();
        drawEffect();
    }
}

#pragma mark - init
void LayerDexEquip::initVar()
{
    _listWeapon = WeaponManager::getInstance()->getListWeaponAll();
    
    _nWeaponSelectIdx = WeaponManager::getInstance()->getEquipPermanent() + 1;
    _nWeaponSelectIdx = MIN(_nWeaponSelectIdx, (int)_listWeapon.size());
}

void LayerDexEquip::initUi()
{
    uiContainer();
    uiInfo();
    uiList();
    uiEffect();
}

#pragma mark - ui
void LayerDexEquip::uiContainer()
{
    Size size = getContentSize();
    
    // 182 * 100
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, 400 + 20));
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
void LayerDexEquip::uiInfo()
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
        
        auto uiNameBG = ui::ImageView::create("Assets/ui_common/base_text_1_5.png");
        uiNameBG->setScale9Enabled(true);
        uiNameBG->setContentSize(Size(uiNameBG->getContentSize().width, 44));
        uiNameBG->setPosition(Vec2(170, uiInfoBG->getContentSize().height - 70));
        uiInfoBG->addChild(uiNameBG);
        
        auto uiName = ui::CText::create(" ", GAME_FONT, 28);
        uiName->setPosition(Vec2(uiNameBG->getContentSize().width / 2, uiNameBG->getContentSize().height / 2));
        uiName->setTextAreaSize(Size(320, uiName->getContentSize().height));
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiName->enableOutline(Color4B::BLACK, 2);
        uiName->setName("UI_NAME");
        uiNameBG->addChild(uiName);
        
        auto unitHero = UnitBehaviorHero::create(E_PLACE::DG_NORMAL);
        unitHero->setSkinUse(false);
        unitHero->setMotion(0, _nWeaponSelectIdx);
        unitHero->setTalkShow(false);
        unitHero->actionRun();
        unitHero->setPosition(170 + 175, 10);
        unitHero->setName("UNIT_HERO");
        uiInfoBG->addChild(unitHero);
        
        auto uiPermanentBG = ui::ImageView::create("Assets/ui/bonsikdex/book_bg2_3.png");
        uiPermanentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiPermanentBG->setPosition(Vec2(uiInfoBG->getContentSize().width - 175, uiInfoBG->getContentSize().height - 60));
        uiPermanentBG->setName("UI_PERMANENT");
        uiInfoBG->addChild(uiPermanentBG);
        
        //
        auto uiDesc = ui::CText::create("asfadf", GAME_FONT, 20);
        uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiDesc->setPosition(Vec2(uiInfoBG->getContentSize().width - 175, uiInfoBG->getContentSize().height - 170));
        uiDesc->setTextAreaSize(Size(300, 80));
        uiDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDesc->setTextColor(Color4B(72, 46, 33, 255));
        uiDesc->setName("UI_DESC");
        uiInfoBG->addChild(uiDesc);
        
 
        //
        auto uiReinforce = ui::Button::create("Assets/ui/bonsikdex/book_bg3_1.png");
        uiReinforce->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiReinforce->setPosition(Vec2(uiInfoBG->getContentSize().width - 10, 10));
        uiReinforce->addClickEventListener(CC_CALLBACK_1(LayerDexEquip::onClickReinforce, this));
        uiReinforce->setName("UI_REINFORCE");
        uiInfoBG->addChild(uiReinforce);
        {
            auto uiEffectLevel = ui::CText::create(GAME_TEXT("t_ui_upgrade"), GAME_FONT, 24);
            uiEffectLevel->setPosition(Vec2(uiReinforce->getContentSize().width / 2, uiReinforce->getContentSize().height / 2 + 20));
            uiReinforce->addChild(uiEffectLevel);
            
            auto uiCost = ui::Layout::create();
            uiCost->setContentSize(Size(uiReinforce->getContentSize().width - 10, 38));
            uiCost->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiCost->setPosition(Vec2(uiReinforce->getContentSize().width / 2, 25));
            uiCost->setLayoutType(ui::Layout::Type::HORIZONTAL);
            uiCost->setName("UI_COST");
            uiReinforce->addChild(uiCost);
        }
    }
}
void LayerDexEquip::uiList()
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

void LayerDexEquip::uiEffect()
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
        uiHelp->addClickEventListener(CC_CALLBACK_1(LayerDexEquip::onClickHelp, this));
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
        
        double effect = 1.0;
        
        effect *= WeaponManager::getInstance()->getDexEffect() + 1;
        effect *= WeaponManager::getInstance()->getReinforceEffect() + 1;
    
        effect -= 1;
        effect *= 100;
        
        std::string effectString = MafUtils::doubleToString(effect);
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
void LayerDexEquip::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_nWeaponSelectIdx);
    if ( objWeapon == nullptr )
    {
        return;
    }
    
    auto weaponPermanent = WeaponManager::getInstance()->getEquipPermanent();
    
    //
    if ( auto unitHero = utils::findChild<UnitBehaviorHero*>(layout, "UNIT_HERO"); unitHero != nullptr )
    {
        unitHero->setMotion(0, _nWeaponSelectIdx);
        unitHero->actionRun();
    }
    
    //
    if ( auto uiName = utils::findChild<ui::CText*>(layout, "UI_NAME"); uiName != nullptr )
    {
        uiName->setString(MafUtils::format("%d.%s", objWeapon->getIdx(), objWeapon->getName().c_str()));
    }
    
    //
    if ( auto uiDesc = utils::findChild<ui::CText*>(layout, "UI_DESC"); uiDesc != nullptr )
    {
        uiDesc->setString(objWeapon->getComment());
    }
    
    //
    if ( auto uiPermanentBG = utils::findChild<ui::ImageView*>(layout, "UI_PERMANENT"); uiPermanentBG != nullptr )
    {
        uiPermanentBG->removeAllChildren();
        
        if( _nWeaponSelectIdx <= weaponPermanent )
        {
            auto itemStemp = ui::ImageView::create("Assets/ui/bonsikdex/book_bg2_2.png");
            itemStemp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            itemStemp->setPosition(Vec2(10, uiPermanentBG->getContentSize().height / 2));
            uiPermanentBG->addChild(itemStemp);
        }
        else
        {
            auto uiIconGold = ui::ImageView::create("Assets/icon/menu_coin.png");
            uiIconGold->setPosition(Vec2(30, uiPermanentBG->getContentSize().height * 0.70));
            uiPermanentBG->addChild(uiIconGold);
            {
                auto price = WeaponManager::getInstance()->getBuyPrice(_nWeaponSelectIdx + BUY_WEAPON_PERMANENT_GOLD_GRADE, 0);
                MafUtils::convertNumberToShort(price);
                
                auto uiCount = ui::CText::create(price, GAME_FONT_AL, 25);
                uiCount->setPosition(Vec2(uiIconGold->getContentSize().width, uiIconGold->getContentSize().height/2));
                uiCount->setColor(COLOR_COIN);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiIconGold->addChild(uiCount);
            }
            
            auto uiIconGem = ui::ImageView::create("Assets/icon/icon_jewel.png");
            uiIconGem->setPosition(Vec2(30, uiPermanentBG->getContentSize().height * 0.30));
            uiPermanentBG->addChild(uiIconGem);
            {
                auto uiCount = ui::CText::create(MafUtils::format("%d",BUY_WEAPON_PERMANENT_GEM), GAME_FONT_AL, 25);
                uiCount->setPosition(Vec2(uiIconGem->getContentSize().width , uiIconGem->getContentSize().height/2));
                uiCount->setColor(COLOR_GEM);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiIconGem->addChild(uiCount);
            }
        }
        
        auto uiPermanentBuy = ui::Button::create("Assets/ui_common/btn_5_2.png");
        uiPermanentBuy->setScale9Enabled(true);
        uiPermanentBuy->setContentSize(Size(110, 80));
        uiPermanentBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiPermanentBuy->setPosition(Vec2(uiPermanentBG->getContentSize().width - 10, uiPermanentBG->getContentSize().height / 2));
        uiPermanentBuy->addClickEventListener(CC_CALLBACK_1(LayerDexEquip::onClickPermanent, this));
        uiPermanentBG->addChild(uiPermanentBuy);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_weapon_permanent_1"), GAME_FONT, 20);
            uiText->setPosition(Vec2(uiPermanentBuy->getContentSize().width / 2, uiPermanentBuy->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiPermanentBuy->getContentSize().height - 10, uiPermanentBuy->getContentSize().height - 10));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiPermanentBuy->addChild(uiText);
        }
        
        if( _nWeaponSelectIdx != weaponPermanent + 1 || WeaponManager::getInstance()->getLevel(_nWeaponSelectIdx) < WeaponManager::getInstance()->getLevelMax() )
        {
            uiPermanentBuy->setEnabled(false);
        }
    }
    
    //
    if ( auto uiReinforce = utils::findChild<ui::Button*>(layout, "UI_REINFORCE"); uiReinforce != nullptr )
    {
        auto uiCost = uiReinforce->getChildByName<ui::Layout*>("UI_COST");
        uiCost->removeAllChildren();
        
        int reinforceNow = WeaponManager::getInstance()->getReinforce(_nWeaponSelectIdx);
        int reinforceMax = objWeapon->getEnhanceMax();
        
        if ( reinforceNow < reinforceMax )
        {
            auto listCost = objWeapon->getEnhanceCost();
            
            for ( int i = 0; i < listCost.size(); i++ )
            {
                auto objItem = listCost.at(i);
                
                auto uiItemBG = ui::Layout::create();
                uiItemBG->setContentSize(Size(100, uiCost->getContentSize().height));
                uiItemBG->setLayoutType(ui::Layout::Type::HORIZONTAL);
                uiCost->addChild(uiItemBG);
                {
                    if ( i != 0 )
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setMargin(ui::Margin(20, 0, 0, 0));
                        uiItemBG->setLayoutParameter(params);
                    }
                    
                    auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                    uiItemBG->addChild(uiItemIcon);
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                        uiItemIcon->setLayoutParameter(params);
                    }
                    
                    std::string rewardCount = objItem->getCount();
                    if ( objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(rewardCount);
                    }
                    
                    auto uiItemCount = ui::CText::create(rewardCount, GAME_FONT, 20);
                    uiItemCount->enableOutline(Color4B::BLACK, 2);
                    uiItemBG->addChild(uiItemCount);
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                        uiItemCount->setLayoutParameter(params);
                    }
                }
                
                auto sizeItem = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiItemBG);
                uiItemBG->setContentSize(sizeItem);
                uiItemBG->requestDoLayout();
            }
            
            auto sizeCost = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiCost);
            uiCost->setContentSize(sizeCost);
            uiCost->requestDoLayout();
            
            if ( weaponPermanent < _nWeaponSelectIdx )
            {
                uiReinforce->setBright(false);
            }
            else
            {
                uiReinforce->setBright(true);
            }
        }
        else
        {
            auto uiText = ui::CText::create("MAX", GAME_FONT, 20);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextColor(Color4B(160, 148, 109, 255));
            uiCost->setContentSize(uiText->getContentSize());
            uiCost->addChild(uiText);
        }
    }
}

void LayerDexEquip::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void LayerDexEquip::drawEffect()
{
    auto layout = _uiContentsEffect;
    
    //
    if ( auto uiBonusTotal = utils::findChild<ui::CText*>(layout, "UI_BONUS_TOTAL"); uiBonusTotal != nullptr )
    {
        int dexBonusTotal = WeaponManager::getInstance()->getDexBonusEffect() * 100;
        uiBonusTotal->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_monster_msg_1", dexBonusTotal));
    }
    
    //
    if ( auto uiEffectPrevBG = utils::findChild<ui::ImageView*>(layout, "UI_BONUS_PREV"); uiEffectPrevBG != nullptr )
    {
        uiEffectPrevBG->removeAllChildren();
        
        int dexBonusLevelNow = WeaponManager::getInstance()->getDexBonusLevelNow();
        int dexBonusNeedCount = WeaponManager::getInstance()->getDexBonusInfoNeedCount(dexBonusLevelNow);
        int dexBonusEffect = WeaponManager::getInstance()->getDexBonusInfoEffect(dexBonusLevelNow) * 100;
        
        std::string title = GAME_TEXT("t_ui_bonsikdex_weapon_enhance_msg_1_non");
        if ( dexBonusLevelNow > 0 )
            title = GAME_TEXTFORMAT("t_ui_bonsikdex_weapon_enhance_msg_1", dexBonusLevelNow, dexBonusNeedCount);
        
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
        
        int dexBonusLevelMax = WeaponManager::getInstance()->getDexBonusLevelMax();
        int dexBonusLevelNext = WeaponManager::getInstance()->getDexBonusLevelNow() + 1;
        int dexBonusNeedCount = WeaponManager::getInstance()->getDexBonusInfoNeedCount(dexBonusLevelNext);
        int dexBonusEffect = WeaponManager::getInstance()->getDexBonusInfoEffect(dexBonusLevelNext) * 100;
        
        if ( dexBonusLevelNext <= dexBonusLevelMax )
        {
            auto uiEffectTitle = ui::CText::create(GAME_TEXTFORMAT("t_ui_bonsikdex_weapon_enhance_msg_1", dexBonusLevelNext, dexBonusNeedCount), GAME_FONT, 20);
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
        double effect = 1.0;
        
        effect *= WeaponManager::getInstance()->getDexEffect() + 1;
        effect *= WeaponManager::getInstance()->getReinforceEffect() + 1;
    
        effect -= 1;
        effect *= 100;
        
        std::string effectString = MafUtils::doubleToString(effect);
        effectString = MafUtils::convertNumberToShort(effectString);
        
        uiEffectTotal->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_total",  effectString.c_str()));
    }
}

#pragma mark - callback
void LayerDexEquip::callbackPermanent(int idx)
{
    int result = WeaponManager::getInstance()->onWeaponBuyPermanent(idx);
    if ( result != 0 )
    {
        return;
    }
 
    //
    drawInfo();
    drawList();
    drawEffect();
}

#pragma mark - click
void LayerDexEquip::onClickHelp(cocos2d::Ref* sender)
{
    auto popup = PopupHelp::create(4, "t_ui_bonsikdex_info_title_%d", "t_ui_bonsikdex_info_msg_%d");
    popup->show();
}

void LayerDexEquip::onClickWeapon(cocos2d::Ref *sender)
{
    auto item = (MenuItem*)sender;
    
    //
    _nWeaponSelectIdx = item->getTag();
    
    //
    drawInfo();
    drawList();
}
  

void LayerDexEquip::onClickPermanent(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupWeaponPermanent::create(_nWeaponSelectIdx);
    popup->setCallbackResult(CC_CALLBACK_1(LayerDexEquip::callbackPermanent, this));
    popup->show();
}

void LayerDexEquip::onClickReinforce(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_nWeaponSelectIdx);
    if ( objWeapon == nullptr )
    {
        return;
    }
    
    int reinforceNow = WeaponManager::getInstance()->getReinforce(_nWeaponSelectIdx);
    int reinforceMax = objWeapon->getEnhanceMax();
    if ( reinforceNow >= reinforceMax )
    {
        return;
    }
        
    auto weaponPermanent = WeaponManager::getInstance()->getEquipPermanent();
    if ( weaponPermanent < _nWeaponSelectIdx )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_bonsikdex_weapon_error_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_180_L )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_bonsikdex_weapon_error_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    

    auto popup = PopupWeaponReinforce::create();
    popup->setWeaponIdx(_nWeaponSelectIdx);
    popup->show();
}

#pragma mark - set, get
