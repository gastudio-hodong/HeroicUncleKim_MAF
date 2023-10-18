//
//  PopupCostumeSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#include "PopupCostumeSkin.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CostumeSkin/Layer/UILayerCostumeSkinList.h"
#include "GameUIPopup/Other/CostumeSkin/Layer/UILayerCostumeSkinShop.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

PopupCostumeSkin* PopupCostumeSkin::create()
{
    PopupCostumeSkin* pRet = new(std::nothrow) PopupCostumeSkin();
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

PopupCostumeSkin::PopupCostumeSkin(void) :
_bInitUI(false),

_eType(E_TYPE::LIST),

_uiContainerInfo(nullptr),
_uiContainerInfoUnit(nullptr),
_uiContainerTab(nullptr),
_uiContainerTabContents(nullptr),
_uiContainerStats(nullptr),

_uiInfoBtnSkinEquip(nullptr),
_uiInfoNameBG(nullptr),
_uiInfoBuffBG(nullptr),

_uiStats(nullptr)
{
    
}

PopupCostumeSkin::~PopupCostumeSkin(void)
{
    
}

bool PopupCostumeSkin::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME_BUY, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME_DUMMY, this);
    
    return true;
}

void PopupCostumeSkin::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiTab();
        uiTabContents();
        uiStats();
        
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawTab();
    drawTabContents();
    drawStats();
    
    PopupLoading::show();
    CostumeSkinManager::getInstance()->requestSkinIsBuy(CC_CALLBACK_1(PopupCostumeSkin::callbackInfo, this));
}

void PopupCostumeSkin::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( _bInitUI == false )
    {
        return;
    }
    
    if ( eType == E_REFRESH::COSTUME || eType == E_REFRESH::COSTUME_BUY || eType == E_REFRESH::COSTUME_DUMMY )
    {
        drawInfo();
        drawTabContents();
        drawStats();
    }
}

#pragma mark - init
void PopupCostumeSkin::initVar()
{
    CostumeSkinManager::getInstance()->setCostumeSkinEquipDummy(0);
}

void PopupCostumeSkin::initUi()
{
    
}

#pragma mark - ui
void PopupCostumeSkin::uiContainer()
{
    Size size = getContentSize();
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, 432 + UserInfoManager::getInstance()->_fHeightIphoneX) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    _uiContainerInfo->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerInfo->setBackGroundColor(Color3B::YELLOW);
    addChild(_uiContainerInfo);
    
    _uiContainerStats = ui::Layout::create();
    _uiContainerStats->setContentSize( Size(size.width, 68) );
    _uiContainerStats->setIgnoreAnchorPointForPosition(false);
    _uiContainerStats->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerStats->setPosition( Vec2(size.width / 2, 0) );
    addChild(_uiContainerStats);
    
    _uiContainerTab = ui::Layout::create();
    _uiContainerTab->setContentSize( Size(size.width, 84) );
    _uiContainerTab->setIgnoreAnchorPointForPosition(false);
    _uiContainerTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerTab->setPosition( Vec2(size.width / 2, size.height - _uiContainerInfo->getContentSize().height - 20) );
    addChild(_uiContainerTab);
    
    _uiContainerTabContents = ui::Layout::create();
    _uiContainerTabContents->setContentSize( Size(size.width, size.height - _uiContainerInfo->getContentSize().height - 20 - _uiContainerStats->getContentSize().height) );
    _uiContainerTabContents->setIgnoreAnchorPointForPosition(false);
    _uiContainerTabContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerTabContents->setPosition( Vec2(size.width / 2, _uiContainerStats->getContentSize().height) );
    _uiContainerTabContents->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerTabContents->setBackGroundColor(Color3B(19, 26, 33));
    addChild(_uiContainerTabContents);
}

void PopupCostumeSkin::uiInfo()
{
    _uiContainerInfo->removeAllChildren();
    
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/battle_bg/prison/dressroom_2_bg.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width / 2, 0));
    uiBG->setScale(MAX(size.width / uiBG->getContentSize().width, size.height / uiBG->getContentSize().height));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    auto uiAltar = ui::ImageView::create("Assets/battle_bg/prison/dressroom_2_altar.png");
    uiAltar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiAltar->setPosition(Vec2(size.width / 2, 20));
    uiAltar->setScale(3);
    ((ui::Scale9Sprite*)uiAltar->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiAltar);
    
    _uiContainerInfoUnit = ui::Layout::create();
    _uiContainerInfoUnit->setContentSize(Size(232, 232));
    _uiContainerInfoUnit->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfoUnit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerInfoUnit->setPosition(Vec2(size.width / 2, 70));
    _uiContainerInfoUnit->setTouchEnabled(true);
    _uiContainerInfoUnit->addClickEventListener([=](Ref* sender){
        
        auto unitHero = _uiContainerInfoUnit->getChildByTag<UnitBehaviorHeroAnother*>(1000);
        if ( unitHero != nullptr )
        {
            unitHero->actionAttack(true);
        }
    });
    layout->addChild(_uiContainerInfoUnit);
    
    auto uiLayoutInfo = ui::Layout::create();
    uiLayoutInfo->setContentSize(Size(size.width, size.height - UserInfoManager::getInstance()->_fHeightIphoneX));
    uiLayoutInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutInfo->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(uiLayoutInfo);
    {
        auto sizeInfo = uiLayoutInfo->getContentSize();
        double posX = 0;
        double posY = 0;
        
        // rigth top
        {
            posX = sizeInfo.width - 10;
            posY = sizeInfo.height - 43;

            // close
            auto uiBtnClose = ui::Button::create("Assets/ui/costume/costume_btn_exit.png");
            uiBtnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiBtnClose->setPosition(Vec2(posX, posY));
            uiBtnClose->addClickEventListener(CC_CALLBACK_1(PopupCostumeSkin::onClickClose, this));
            uiLayoutInfo->addChild(uiBtnClose);
            
            //
            posY -= 80;
            _uiInfoBtnSkinEquip = ui::Button::create("Assets/ui/costume/costume_btn_wear_off.png");
            _uiInfoBtnSkinEquip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _uiInfoBtnSkinEquip->setPosition(Vec2(posX, posY));
            _uiInfoBtnSkinEquip->addClickEventListener(CC_CALLBACK_1(PopupCostumeSkin::onClickSkinEquip, this));
            uiLayoutInfo->addChild(_uiInfoBtnSkinEquip);
        }
        
        // left top
        {
            posX = 10;
            posY = sizeInfo.height - 43;
            
            {
                std::vector<E_ITEMS> list;
                list.push_back(E_ITEMS::COSTUME_COIN);
                
                auto uiCurrency = UICurrency::create(list, Size(160, 40));
                uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiCurrency->setPosition(Vec2(posX, posY));
                uiLayoutInfo->addChild(uiCurrency);
                {
                    auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
                    uiBG->setScale9Enabled(true);
                    uiBG->setContentSize(Size(uiCurrency->getContentSize().width, uiCurrency->getContentSize().height));
                    uiBG->setPosition(Vec2(uiCurrency->getContentSize().width / 2, uiCurrency->getContentSize().height / 2));
                    uiCurrency->addChild(uiBG, -1);
                }
            }
            
            {
                posY -= 50;
                std::vector<E_ITEMS> list;
                list.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);
                
                auto uiCurrency = UICurrency::create(list, Size(160, 40));
                uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiCurrency->setPosition(Vec2(posX, posY));
                uiLayoutInfo->addChild(uiCurrency);
                {
                    auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
                    uiBG->setScale9Enabled(true);
                    uiBG->setContentSize(Size(uiCurrency->getContentSize().width, uiCurrency->getContentSize().height));
                    uiBG->setPosition(Vec2(uiCurrency->getContentSize().width / 2, uiCurrency->getContentSize().height / 2));
                    uiCurrency->addChild(uiBG, -1);
                }
            }
            
            {
                posY -= 50;
                std::vector<E_ITEMS> list;
                list.push_back(E_ITEMS::EVENT_RACE_SKIN);
                
                auto uiCurrency = UICurrency::create(list, Size(160, 40));
                uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiCurrency->setPosition(Vec2(posX, posY));
                uiLayoutInfo->addChild(uiCurrency);
                {
                    auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
                    uiBG->setScale9Enabled(true);
                    uiBG->setContentSize(Size(uiCurrency->getContentSize().width, uiCurrency->getContentSize().height));
                    uiBG->setPosition(Vec2(uiCurrency->getContentSize().width / 2, uiCurrency->getContentSize().height / 2));
                    uiCurrency->addChild(uiBG, -1);
                }
            }
        }
        
        // middle top
        {
            posX = sizeInfo.width / 2;
            posY = sizeInfo.height - 10;
            
            //
            _uiInfoNameBG = ui::ImageView::create("Assets/ui/costume/costume_name_box.png");
            _uiInfoNameBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            _uiInfoNameBG->setPosition(Vec2(posX, posY));
            layout->addChild(_uiInfoNameBG);
            
            //
            posY -= 50;
            _uiInfoBuffBG = ui::ImageView::create("Assets/ui/costume/costume_name_attkup_box.png");
            _uiInfoBuffBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            _uiInfoBuffBG->setPosition(Vec2(posX, posY));
            layout->addChild(_uiInfoBuffBG);
        }
    }
}

void PopupCostumeSkin::uiTab()
{
    _uiContainerTab->removeAllChildren();
    
    auto layout = _uiContainerTab;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_menu_bar.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, 20));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width / 2, 0));
    uiBG->setCapInsets(Rect(4, 10, 1, 1));
    layout->addChild(uiBG);
    
    //
    auto layoutButtons = ui::Layout::create();
    layoutButtons->setContentSize( Size(size.width, size.height - uiBG->getContentSize().height + 3) );
    layoutButtons->setIgnoreAnchorPointForPosition(false);
    layoutButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    layoutButtons->setPosition( Vec2(size.width / 2, uiBG->getContentSize().height - 3) );
    layoutButtons->setName("UI_BUTTONS");
    layout->addChild(layoutButtons);
    {
        double padding = 10 ;
        double widthButtons = -padding;
        
        E_TYPE listType[] = {E_TYPE::LIST, E_TYPE::SHOP};
        for ( int i = 0; i < sizeof(listType)/sizeof(E_TYPE); i++ )
        {
            auto uiBtn = ui::Button::create("Assets/ui/costume/costume_menu_btn_disable.png");
            uiBtn->setScale9Enabled(true);
            uiBtn->setContentSize(Size(310, 64));
            uiBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiBtn->addClickEventListener(CC_CALLBACK_1(PopupCostumeSkin::onClickTab, this));
            uiBtn->setTag(enumToInt(listType[i]));
            layoutButtons->addChild(uiBtn);
            
            std::string textKey = "";
            if ( listType[i] == E_TYPE::LIST )
                textKey = "t_ui_menu_bonsikdex";
            else if ( listType[i] == E_TYPE::SHOP )
                textKey = "t_ui_shop_gem";
                
            auto uiText = ui::CText::create(GAME_TEXT(textKey), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2));
            uiText->setName("UI_TEXT");
            uiBtn->addChild(uiText);
            
            widthButtons += uiBtn->getContentSize().width * uiBtn->getScaleX() + padding;
        }
        
        float posX = (layoutButtons->getContentSize().width / 2.0f) - (widthButtons / 2.0f);
        float posY = 0;
        for (const auto &child : layoutButtons->getChildren())
        {
            child->setPosition(posX + child->getContentSize().width * child->getScaleX() / 2.0f, posY);
            posX += child->getContentSize().width * child->getScaleX() + padding;
        }
    }
    
    //
    
}

void PopupCostumeSkin::uiTabContents()
{
    _uiContainerTabContents->removeAllChildren();
    
    auto layout = _uiContainerTabContents;
    auto size = layout->getContentSize();
    
    auto uiContentList = UILayerCostumeSkinList::create();
    uiContentList->setContentSize(size);
    uiContentList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiContentList->setPosition(Vec2(size.width / 2, 0));
    uiContentList->setTag(enumToInt(E_TYPE::LIST));
    uiContentList->setVisible(false);
    layout->addChild(uiContentList);
    
    auto uiContentShop = UILayerCostumeSkinShop::create();
    uiContentShop->setContentSize(size);
    uiContentShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiContentShop->setPosition(Vec2(size.width / 2, 0));
    uiContentShop->setTag(enumToInt(E_TYPE::SHOP));
    uiContentShop->setVisible(false);
    layout->addChild(uiContentShop);
}

void PopupCostumeSkin::uiStats()
{
    _uiContainerStats->removeAllChildren();
    
    auto layout = _uiContainerStats;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_menu_bar.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width / 2, 0));
    uiBG->setCapInsets(Rect(4, 10, 1, 1));
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_arti_d1_contents_12"), GAME_FONT, 26);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiText->setPosition(Vec2(50, size.height / 2));
    layout->addChild(uiText);
    
    _uiStats = ui::CText::create("0%", GAME_FONT, 26);
    _uiStats->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _uiStats->setPosition(Vec2(size.width - 50, size.height / 2));
    _uiStats->setTouchEnabled(true);
    _uiStats->addClickEventListener([=](Ref* sender){
        double skinBuff = CostumeSkinManager::getInstance()->getEffectOwned();
        skinBuff *= 100;
        
        std::string value = MafUtils::format("%.0f%%", skinBuff);
        
        Vec2 posToast = _uiStats->getParent()->convertToWorldSpace(_uiStats->getPosition());
        posToast.y += _uiStats->getContentSize().height / 2;
        posToast.y += 20;
        posToast.x = getContentSize().width/2;
            
        PopupToast::showImmediately(value, posToast.x, posToast.y);
    });
    layout->addChild(_uiStats);
}

#pragma mark - ui draw
void PopupCostumeSkin::drawInfo()
{
    int skinEquip = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    int skinEquipDummy = CostumeSkinManager::getInstance()->getCostumeSkinEquipDummy();
    if ( skinEquip == skinEquipDummy )
    {
        skinEquipDummy = 0;
    }
    
    int idx = skinEquip;
    if ( skinEquipDummy != 0 )
    {
        idx = skinEquipDummy;
    }
    
    auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(idx);
    if ( objSkin == nullptr )
    {
        _uiInfoBtnSkinEquip->setVisible(false);
        _uiInfoBuffBG->setVisible(false);
    }
    else
    {
        bool bBuy = CostumeSkinManager::getInstance()->isCostumeSkinBuy(objSkin->getIdx());
        
        //
        _uiInfoBtnSkinEquip->setVisible(false);
        _uiInfoBtnSkinEquip->removeAllChildren();
        if ( bBuy == true )
        {
            _uiInfoBtnSkinEquip->setVisible(true);
            
            if ( skinEquipDummy != 0 )
            {
                auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_equip"), GAME_FONT, 18);
                uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiText->setPosition(Vec2(_uiInfoBtnSkinEquip->getContentSize().width / 2, 5));
                uiText->setTextColor(Color4B(COLOR_TITLE));
                _uiInfoBtnSkinEquip->addChild(uiText);
                _uiInfoBtnSkinEquip->loadTextureNormal("Assets/ui/costume/costume_btn_wear_off.png");
            }
            else
            {
                auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_equip_now"), GAME_FONT, 18);
                uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiText->setPosition(Vec2(_uiInfoBtnSkinEquip->getContentSize().width / 2, 5));
                _uiInfoBtnSkinEquip->addChild(uiText);
                _uiInfoBtnSkinEquip->loadTextureNormal("Assets/ui/costume/costume_btn_wear_on.png");
            }
        }
        
        
        //
        _uiInfoBuffBG->setVisible(false);
        _uiInfoBuffBG->removeAllChildren();
        if ( objSkin->getBuffType().compare("ALL_DAMAGE") == 0 )
        {
            _uiInfoBuffBG->setVisible(true);
            
            int nRate = objSkin->getBuffStrength() * 100;
            auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_costume_skin_option", nRate), GAME_FONT, 24);
            uiText->setPosition(Vec2(_uiInfoBuffBG->getContentSize().width / 2, _uiInfoBuffBG->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(_uiInfoBuffBG->getContentSize().width - 20, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            _uiInfoBuffBG->addChild(uiText);
        }
    }
    
    //
    _uiInfoNameBG->setVisible(true);
    _uiInfoNameBG->removeAllChildren();
    {
        std::string name = GAME_TEXT("t_ui_costume_skin_msg_1");
        if ( objSkin != nullptr )
        {
            name = objSkin->getName();
        }
        
        auto uiText = ui::CText::create(name, GAME_FONT, 24);
        uiText->setPosition(Vec2(_uiInfoNameBG->getContentSize().width / 2, _uiInfoNameBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(_uiInfoNameBG->getContentSize().width - 20, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->enableShadow();
        _uiInfoNameBG->addChild(uiText);
    }
    
    //
    _uiContainerInfoUnit->removeAllChildren();
    {
        auto unitHero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
        unitHero->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        unitHero->setPosition(_uiContainerInfoUnit->getContentSize().width / 2 + 85, 120);
        unitHero->getLayerCharacter()->setScale(1.25);
        if ( skinEquipDummy == 0 && skinEquip == 0 )
        {
            unitHero->setImage(WeaponManager::getInstance()->getEquipVisual(),
                            CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD),
                            CostumeManager::getInstance()->getEquip(E_COSTUME::BODY),
                            CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE),
                            CostumeManager::getInstance()->getEquip(E_COSTUME::HAND),
                            CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT));
            
            unitHero->setMotion(WeaponManager::getInstance()->getEquipVisual(),
                             CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD),
                             CostumeManager::getInstance()->getEquip(E_COSTUME::BODY),
                             CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE),
                             CostumeManager::getInstance()->getEquip(E_COSTUME::HAND),
                             CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT));
        }
        else if ( skinEquipDummy != 0 )
        {
            unitHero->setSkin(skinEquipDummy);
        }
        else if ( skinEquip != 0 )
        {
            unitHero->setSkin(skinEquip);
        }
        unitHero->actionMove();
        unitHero->setTag(1000);
        _uiContainerInfoUnit->addChild(unitHero);
        
        
    }
}

void PopupCostumeSkin::drawTab()
{
    auto layout = _uiContainerTab;
    auto size = layout->getContentSize();
    
    //
    auto layoutButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");
    {
        E_TYPE listType[] = {E_TYPE::LIST, E_TYPE::SHOP};
        for ( int i = 0; i < sizeof(listType)/sizeof(E_TYPE); i++ )
        {
            auto uiBtn = layoutButtons->getChildByTag<ui::Button*>(enumToInt(listType[i]));
            auto uiText = uiBtn->getChildByName<ui::CText*>("UI_TEXT");
            if ( listType[i] == _eType )
            {
                uiBtn->loadTextureNormal("Assets/ui/costume/costume_menu_btn_select.png");
                uiText->setTextColor(Color4B(255, 245, 212, 255));
            }
            else
            {
                uiBtn->loadTextureNormal("Assets/ui/costume/costume_menu_btn_disable.png");
                uiText->setTextColor(Color4B(130, 128, 120, 255));
            }
        }
    }
}

void PopupCostumeSkin::drawTabContents()
{
    auto layout = _uiContainerTabContents;
    auto size = layout->getContentSize();
    
    E_TYPE listType[] = {E_TYPE::LIST, E_TYPE::SHOP};
    for ( int i = 0; i < sizeof(listType)/sizeof(E_TYPE); i++ )
    {
        auto uiContent = layout->getChildByTag<UILayerCostumeSkinParent*>(enumToInt(listType[i]));
        
        if ( listType[i] == _eType )
        {
            uiContent->setVisible(true);
            uiContent->setContentSize(size);
            uiContent->refreshList();
        }
        else
        {
            uiContent->setVisible(false);
            uiContent->setContentSize(size);
        }
    }
}

void PopupCostumeSkin::drawStats()
{
    auto layout = _uiContainerTabContents;
    auto size = layout->getContentSize();
    
    double skinBuff = CostumeSkinManager::getInstance()->getEffectOwned();
    skinBuff *= 100;
    
    //
    std::string value = MafUtils::doubleToString(skinBuff);
    value = MafUtils::convertNumberToShort(value);
    _uiStats->setString(MafUtils::format("%s%%", value.c_str()));
}

#pragma mark - click
void PopupCostumeSkin::onClickClose(Ref *sender)
{
    hide();
}

void PopupCostumeSkin::onClickTab(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    E_TYPE eType = enumFromInt<E_TYPE>(uiBtn->getTag());
    
    if ( _eType == eType )
    {
        return;
    }
    
    _eType = eType;
    drawTab();
    drawTabContents();
}


void PopupCostumeSkin::onClickSkinEquip(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int skinEquip = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    int skinEquipDummy = CostumeSkinManager::getInstance()->getCostumeSkinEquipDummy();
    if ( skinEquip == 0 && skinEquipDummy == 0 )
    {
        return;
    }
    
    if ( skinEquip == skinEquipDummy )
    {
        skinEquipDummy = 0;
    }
    
    if ( skinEquipDummy != 0 )
    {
        CostumeSkinManager::getInstance()->onSkinEquip(skinEquipDummy);
    }
    else if ( skinEquip != 0 )
    {
        CostumeSkinManager::getInstance()->onSkinEquip(0);
    }
}

#pragma mark - callback
void PopupCostumeSkin::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    // draw
    drawInfo();
    drawTab();
    drawTabContents();
    drawStats();
}

#pragma mark -
void PopupCostumeSkin::setTabType(E_TYPE eType)
{
    _eType = eType;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawTab();
        drawTabContents();
        drawStats();
    }
}
