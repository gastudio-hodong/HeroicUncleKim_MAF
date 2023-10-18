//
//  PopupWeaponCraft.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/10/2019.
//

#include "PopupWeaponCraft.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

PopupWeaponCraft* PopupWeaponCraft::create(int weaponIdx)
{
    PopupWeaponCraft* pRet = new(std::nothrow) PopupWeaponCraft();
    if ( pRet && pRet->init(weaponIdx) )
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

PopupWeaponCraft::PopupWeaponCraft() :
_bInitUI(false),

_weaponIdx(0),

_layerContainerInfo(nullptr),
_layerContainerMine(nullptr),
_layerContainerButtons(nullptr)
{
    
}

PopupWeaponCraft::~PopupWeaponCraft(void)
{
    
}
bool PopupWeaponCraft::init(int weaponIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _weaponIdx = weaponIdx;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupWeaponCraft::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiMine();
        uiButtons();
    }
    
    //
    drawInfo();
    drawMine();
}

#pragma mark - init
void PopupWeaponCraft::initVar()
{
    
}

void PopupWeaponCraft::initUi()
{
    Size size = Size(729, 960);
    
    // size
    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(25);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_equip_legend_making"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupWeaponCraft::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupWeaponCraft::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25));
    _spriteContainer->addChild(layout);
    
    _layerContainerButtons = ui::Layout::create();
    _layerContainerButtons->setContentSize( Size(size.width, 89 + 25) );
    _layerContainerButtons->setIgnoreAnchorPointForPosition(false);
    _layerContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_layerContainerButtons);
    
    _layerContainerMine = ui::Layout::create();
    _layerContainerMine->setContentSize( Size(size.width, 350) );
    _layerContainerMine->setIgnoreAnchorPointForPosition(false);
    _layerContainerMine->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerMine->setPosition( Vec2(size.width / 2, _layerContainerButtons->getContentSize().height) );
    layout->addChild(_layerContainerMine);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, size.height - _layerContainerButtons->getContentSize().height - _layerContainerMine->getContentSize().height) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
}

void PopupWeaponCraft::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiIconBG = ui::ImageView::create("Assets/ui/mine/week_weaponbox.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiIconBG->setPosition(Vec2(size.width / 2, size.height));
    uiIconBG->setName("UI_ICONBG");
    layout->addChild(uiIconBG);
    
    // 라인
    auto uiLine = ui::ImageView::create("Assets/ui/mine/week_weaponline.png");
    uiLine->setPosition(Vec2(size.width/2, size.height - 200));
    layout->addChild(uiLine);
    
    //
    auto uiDesc = ui::CText::create(" ", GAME_FONT, 24);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiDesc->setPosition(Vec2(size.width/2, size.height - 220));
    uiDesc->setTextAreaSize(Size(size.width - 200, 150));
    uiDesc->setTextVerticalAlignment(TextVAlignment::TOP);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setName("UI_DESC");
    layout->addChild(uiDesc);
}

void PopupWeaponCraft::uiMine()
{
    auto layout = _layerContainerMine;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(570, size.height));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    uiBG->setName("UI_BG");
    layout->addChild(uiBG);
    {
        auto uiNameBG = ui::ImageView::create(PIXEL);
        uiNameBG->setScale9Enabled(true);
        uiNameBG->setContentSize(Size(566, 36));
        uiNameBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiNameBG->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 2));
        uiNameBG->setColor(Color3B(40, 49, 58));
        uiBG->addChild(uiNameBG);
        {
            auto uiName = Label::createWithTTF(GAME_TEXT("t_ui_equip_legend_meterial"), GAME_FONT, 24);
            uiName->setPosition(Vec2(uiNameBG->getContentSize().width / 2, uiNameBG->getContentSize().height / 2));
            uiName->setColor(COLOR_TITLE);
            uiNameBG->addChild(uiName);
        }
        
        auto uiMine = ui::Layout::create();
        uiMine->setContentSize(Size(550, 310));
        uiMine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiMine->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2 - uiNameBG->getContentSize().height / 2));
        uiMine->setName("UI_MINE");
        uiBG->addChild(uiMine);
    }
}

void PopupWeaponCraft::uiButtons()
{
    auto layout = _layerContainerButtons;
    Size size = layout->getContentSize();
    
    //
    auto uiBtnBuy = ui::Button::create("Assets/ui_common/btn_on.png");
    uiBtnBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnBuy->setPosition(Vec2(size.width / 2, 0));
    uiBtnBuy->addClickEventListener(CC_CALLBACK_1(PopupWeaponCraft::onClickCraft, this));
    layout->addChild(uiBtnBuy);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_making"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiBtnBuy->getContentSize().width / 2, uiBtnBuy->getContentSize().height / 2));
        uiBtnBuy->addChild(uiText);
    }
}

#pragma mark - draw
void PopupWeaponCraft::drawInfo()
{
    auto layout = _layerContainerInfo;
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_weaponIdx);
    
    int weaponIdx = objWeapon->getIdx();
    std::string pathIcon = WeaponManager::getInstance()->getImagePath(_weaponIdx);
    std::string nameText = MafUtils::format("%d. %s", weaponIdx, objWeapon->getName().c_str());
    std::string descText = objWeapon->getComment();
    //
    if ( auto uiIconBG = layout->getChildByName<ui::ImageView*>("UI_ICONBG"); uiIconBG != nullptr )
    {
        uiIconBG->removeAllChildren();
        
        if ( objWeapon->getLegend() > 1 )
        {
            uiIconBG->loadTexture("Assets/ui/mine/week_weaponbox_2_1.png");
        }
        else if ( objWeapon->getLegend() > 0 )
        {
            uiIconBG->loadTexture("Assets/ui/mine/week_weaponbox.png");
        }
        
        if ( objWeapon->getLegend() > 1 )
        {
            auto uiEffect = ui::ImageView::create("Assets/ui/mine/week_weaponbox_2_2.png");
            uiEffect->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
            uiEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
            uiIconBG->addChild(uiEffect);
        }
            

        //
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale((uiIconBG->getContentSize().width - 20) / uiIcon->getContentSize().width);
        uiIconBG->addChild(uiIcon);
        
        
        //
        auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiName->setPosition(Vec2(uiIconBG->getContentSize().width / 2, -10));
        uiName->setTextColor(Color4B(COLOR_COIN));
        uiIconBG->addChild(uiName);
        if ( objWeapon->getLegend() > 1 )
        {
            uiName->setTextColor(Color4B(0, 245, 225, 255));
        }
        else if ( objWeapon->getLegend() > 0 )
        {
            uiName->setTextColor(Color4B(212, 34, 204, 255));
        }
    }
    
    if ( auto uiDesc = layout->getChildByName<ui::CText*>("UI_DESC"); uiDesc != nullptr )
    {
        uiDesc->setString(descText);
    }
}

void PopupWeaponCraft::drawMine()
{
    auto layout = _layerContainerMine;
    Size size = layout->getContentSize();
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_weaponIdx);
    
    auto listCost = objWeapon->getCost();
    
    //
    auto uiMine = (ui::Layout*)utils::findChild(layout, "UI_MINE");
    if ( uiMine != nullptr )
    {
        uiMine->removeAllChildren();
        
        for ( int i = 0; i < listCost.size(); i++ )
        {
            auto objItem = listCost.at(i);
             
            int itemNeed = atoi(objItem->getCount().c_str());
            int itemNow = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)objItem->getIdx()).c_str());
        
            double percent = 0;
            if ( itemNow > 0 && itemNeed > 0 )
            {
                percent = (double)itemNow / itemNeed * 100;
                if ( percent > 100 )
                    percent = 100;
            }
            
            // item bg
            auto uiMineCell = ui::Layout::create();
            uiMineCell->setContentSize(Size(112, 89));
            uiMine->addChild(uiMineCell);
            if ( objItem->getType().compare("MINE") == 0 && itemNeed != 0 && itemNow < itemNeed )
            {
                uiMineCell->setTouchEnabled(true);
                uiMineCell->addClickEventListener(CC_CALLBACK_1(PopupWeaponCraft::onClickMine, this));
                uiMineCell->setTag(objItem->getTypeParams());
            }
            {
                auto uiIcon = ui::ImageView::create(objItem->getPath());
                uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiIcon->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 24));
                uiIcon->setScale(1.2);
                uiMineCell->addChild(uiIcon);

                auto uiBarBG = ui::ImageView::create("Assets/ui/mine/week_weapon_bar1.png");
                uiBarBG->setScale9Enabled(true);
                uiBarBG->setContentSize(Size(uiMineCell->getContentSize().width, uiBarBG->getContentSize().height));
                uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBarBG->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 0));
                uiMineCell->addChild(uiBarBG);

                auto uiBar = ui::LoadingBar::create("Assets/ui/mine/week_weapon_bar2.png");
                uiBar->setScale9Enabled(true);
                uiBar->setContentSize(Size(uiMineCell->getContentSize().width, uiBarBG->getContentSize().height));
                uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBar->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 0));
                uiBar->setPercent(percent);
                uiMineCell->addChild(uiBar);
                
                auto uiCount = Label::createWithTTF("-", GAME_FONT, 16);
                uiCount->setPosition(uiBar->getContentSize().width / 2, uiBar->getContentSize().height / 2);
                uiBar->addChild(uiCount);
                if ( itemNeed > 0 )
                {
                    std::string itemNowText = MafUtils::toString(itemNow);
                    itemNowText = MafUtils::convertNumberToShort(itemNowText);
                    
                    std::string itemNeedText = MafUtils::toString(itemNeed);
                    itemNeedText = MafUtils::convertNumberToShort(itemNeedText);

                    uiCount->setString(MafUtils::format("%s/%s",  itemNowText.c_str(), itemNeedText.c_str()));
                }
                
                //
                if ( objItem->getType().compare("MINE") == 0 && itemNeed != 0 && itemNow < itemNeed )
                {
                    auto uiEffect = ui::ImageView::create("Assets/ui_common/box_item_1_3.png");
                    uiEffect->setPosition(Vec2(uiMineCell->getContentSize().width / 2, uiMineCell->getContentSize().height / 2));
                    uiMineCell->addChild(uiEffect);

                    auto uiReddot = ui::ImageView::create("Assets/ui/exclamation_mark_icon_2.png");
                    uiReddot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                    uiReddot->setPosition(Vec2(0, uiMineCell->getContentSize().height));
                    uiMineCell->addChild(uiReddot);
                }
                else if ( (E_ITEMS)objItem->getIdx() == E_ITEMS::MYTHRIL )
                {
                    auto uiEffect = ui::ImageView::create("Assets/ui_common/box_transcendence1_2.png");
                    uiEffect->setPosition(Vec2(uiMineCell->getContentSize().width / 2, uiMineCell->getContentSize().height / 2 + 10));
                    uiMineCell->addChild(uiEffect, -1);
                }
            }
        }
        
        //
        UtilsWidget::getInstance()->sortWidgetGrid(uiMine, 4, uiMine->getContentSize().width, uiMine->getContentSize().height, 10, false, false);
    }
}

#pragma mark - set, get

#pragma mark - click
void PopupWeaponCraft::onClickClose(Ref* sender)
{
    hide();
}
void PopupWeaponCraft::onClickCraft(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_weaponIdx);
    
    int result = WeaponManager::getInstance()->onWeaponBuyLegend(_weaponIdx);
    if ( result != 0 )
    {
        /*
         0 : 성공
         1 : 조건 안됨
         2 : 재화 부족
         */
        if ( result == 2 )
        {
            auto listCost = objWeapon->getCost();
            
            for ( int i = 0; i < listCost.size(); i++ )
            {
                auto cost = listCost.at(i);
                
                auto itemNeed = cost->getCount();
                auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
                if ( MafUtils::compareStr(itemNow, itemNeed) == false )
                {
                    auto popup = PopupCurrencyDetail::create((E_ITEMS)cost->getIdx());
                    popup->show();
                    break;
                }
            }
        }
        
        return;
    }
    
    if ( objWeapon->getIdx() == 180 )
    {
        // 시나리오 연출
        if(DialogManager::getInstance()->isViewDialog("SMELTING_WORKSHOP_EP") == 0)
        {
            DialogManager::getInstance()->showDialog("SMELTING_WORKSHOP_EP");
        }
    }
    
    //
    hide();
}

void PopupWeaponCraft::onClickMine(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    E_MINERAL eType = enumFromInt<E_MINERAL>(uiLayout->getTag());
    
    E_CASH eCash = E_CASH::CASH_MINE_1;
    switch (eType) {
        case E_MINERAL::MINERAL_1:     eCash = E_CASH::CASH_MINE_1;    break;
        case E_MINERAL::MINERAL_2:     eCash = E_CASH::CASH_MINE_2;    break;
        case E_MINERAL::MINERAL_3:     eCash = E_CASH::CASH_MINE_3;    break;
        case E_MINERAL::MINERAL_4:     eCash = E_CASH::CASH_MINE_4;    break;
        case E_MINERAL::MINERAL_5:     eCash = E_CASH::CASH_MINE_5;    break;
        case E_MINERAL::MINERAL_6:     eCash = E_CASH::CASH_MINE_6;    break;
        case E_MINERAL::MINERAL_7_SAPPHIRE:        eCash = E_CASH::CASH_MINE_7;        break;
        case E_MINERAL::MINERAL_8_OPAL:            eCash = E_CASH::CASH_MINE_8;        break;
        case E_MINERAL::MINERAL_9_AQUA:            eCash = E_CASH::CASH_MINE_9;        break;
        case E_MINERAL::MINERAL_10_EMERALD:        eCash = E_CASH::CASH_MINE_10;       break;
        case E_MINERAL::MINERAL_11_AMETHYST:       eCash = E_CASH::CASH_MINE_11;       break;
            
        default:
            break;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(eCash);
    if ( objProduct == nullptr )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }

    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(eCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(PopupWeaponCraft::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            auto popup = PopupPurchaseBuy::create(eCash);
            popup->setCallbackResult(CC_CALLBACK_1(PopupWeaponCraft::callbackPurchaseOk, this));
            popup->show();
        }
    });
}

#pragma mark - callback
void PopupWeaponCraft::callbackPurchaseOk(int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(PopupWeaponCraft::callbackPurchase, this));
}

void PopupWeaponCraft::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        
        //
        drawMine();
        
        
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}
