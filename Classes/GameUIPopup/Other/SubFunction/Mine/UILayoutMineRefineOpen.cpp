//
//  UILayoutMineRefineOpen.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutMineRefineOpen.h"

#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

UILayoutMineRefineOpen* UILayoutMineRefineOpen::create()
{
    UILayoutMineRefineOpen* pRet = new(std::nothrow) UILayoutMineRefineOpen();
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

UILayoutMineRefineOpen::UILayoutMineRefineOpen():
_bInitUI(false),

_ePlace(E_PLACE::DG_NORMAL),

//
_uiContainerInfo(nullptr),
_uiContainerMine(nullptr),
_uiContainerButtons(nullptr),

_uiPet(nullptr)
{
    
}

bool UILayoutMineRefineOpen::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setTouchEnabled(true);
    setBackGroundColor(Color3B(18, 25, 31));
    setBackGroundColorType(BackGroundColorType::SOLID);
        
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutMineRefineOpen::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiMine();
        uiButtons();
        
        _bInitUI = true;
    }
    
    // draw
    drawInfo();
    drawInfoPet();
    drawMine();
    drawButtons();
}

#pragma mark - init
void UILayoutMineRefineOpen::initVar()
{
    
}

void UILayoutMineRefineOpen::initUI()
{
    
}

#pragma mark - ui
void UILayoutMineRefineOpen::uiContainer()
{
    Size size = getContentSize();
    
    //
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(getContentSize().width / 2, getContentSize().height));
    addChild(layout);
    
    _uiContainerButtons = ui::Layout::create();
    _uiContainerButtons->setContentSize(Size(size.width, 116));
    _uiContainerButtons->setIgnoreAnchorPointForPosition(false);
    _uiContainerButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerButtons->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContainerButtons);
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, 400));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    _uiContainerInfo->setClippingEnabled(true);
    layout->addChild(_uiContainerInfo);
    
    _uiContainerMine = ui::Layout::create();
    _uiContainerMine->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height - _uiContainerButtons->getContentSize().height));
    _uiContainerMine->setIgnoreAnchorPointForPosition(false);
    _uiContainerMine->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerMine->setPosition( Vec2(size.width / 2, size.height - _uiContainerInfo->getContentSize().height) );
    layout->addChild(_uiContainerMine);
}

void UILayoutMineRefineOpen::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto uiTopBG = ui::ImageView::create("Assets/ui/mine/mine_bg2.png");
    uiTopBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTopBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiTopBG);
    {
        // pet
        auto animationIcon = Animation::create();
        animationIcon->setDelayPerUnit(0.15);
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strTemp = MafUtils::format("Assets/character_pet/normal/mole/pet_mole_attack_%d.png",i);
            animationIcon->addSpriteFrameWithFile(strTemp);
        }
        
        _uiPet = ui::ImageView::create("Assets/character_pet/normal/mole/pet_mole_attack_1.png");
        _uiPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiPet->setPosition(Vec2(uiTopBG->getContentSize().width * 0.45 + 10, -28));
        _uiPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
        uiTopBG->addChild(_uiPet);
        
        {
            auto animationIcon = Animation::create();
            animationIcon->setDelayPerUnit(0.15);
            for ( int i = 1; i <= 4; i++ )
            {
                std::string strTemp = MafUtils::format("Assets/ui/mine/mole_smith_idle_%d.png",i);
                animationIcon->addSpriteFrameWithFile(strTemp);
            }
            
            auto uiPetSmith = ui::ImageView::create("Assets/ui/mine/mole_smith_idle_1.png");
            uiPetSmith->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiPetSmith->setPosition(Vec2(uiTopBG->getContentSize().width * 0.6, 6));
            uiPetSmith->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
            uiTopBG->addChild(uiPetSmith);
        }
    }
    
    auto uiText1 = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_unlock_msg_1"), GAME_FONT, 38);
    uiText1->setPosition(Vec2(size.width / 2, size.height - 200));
    uiText1->setTextColor(Color4B(246, 203, 28, 255));
    layout->addChild(uiText1);
    
    auto uiText2 = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_unlock_msg"), GAME_FONT, 28);
    uiText2->setPosition(Vec2(size.width / 2, size.height - 280));
    uiText2->setTextColor(Color4B(128, 128, 128, 255));
    uiText2->setTextAreaSize(Size(size.width, uiText2->getContentSize().height));
    uiText2->setTextOverflow(Label::Overflow::SHRINK);
    uiText2->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText2);
}
void UILayoutMineRefineOpen::uiMine()
{
    auto layout = _uiContainerMine;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(570, 350));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
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
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_unlock_autosave"), GAME_FONT, 28);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiText->setPosition(Vec2(uiBG->getContentSize().width / 2, -50));
    uiBG->addChild(uiText);
}
void UILayoutMineRefineOpen::uiButtons()
{
    auto layout = _uiContainerButtons;
    auto size = layout->getContentSize();
    
    auto uiBtn = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
    uiBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtn->setPosition(Vec2(size.width / 2, 0));
    uiBtn->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefineOpen::onClickOpen, this));
    uiBtn->setName("UI_BTN");
    layout->addChild(uiBtn);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_unlock_bt"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, 32.5));
        uiText->setTextAreaSize(Size(uiBtn->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiBtn->addChild(uiText);
    }
}

#pragma mark - draw
void UILayoutMineRefineOpen::drawInfo()
{
    auto layout = _uiContainerInfo;
}

void UILayoutMineRefineOpen::drawInfoPet()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7) > 0 )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("mole_skin_%d", PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(E_PET::MOLE_7) > 0 )
        {
            bSkin = true;
            strPath.append("mole_evolution/");
        }
    }
    
    if ( bSkin == false )
    {
        return;
    }
    
    //
    auto animationIcon = Animation::create();
    animationIcon->setDelayPerUnit(0.15);
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        animationIcon->addSpriteFrameWithFile(strTemp);
    }
    _uiPet->getVirtualRenderer()->stopAllActions();
    _uiPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
}

void UILayoutMineRefineOpen::drawMine()
{
    auto layout = _uiContainerMine;
    
    auto listCostMine = MineralManager::getInstance()->getRefineOpenCostMine();
    
    //
    auto uiMine = (ui::Layout*)utils::findChild(layout, "UI_MINE");
    if ( uiMine != nullptr )
    {
        uiMine->removeAllChildren();
        
        for ( int i = 0; i < listCostMine.size(); i++ )
        {
            auto objItem = listCostMine.at(i);
             
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
                uiMineCell->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefineOpen::onClickMine, this));
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
void UILayoutMineRefineOpen::drawButtons()
{
    auto layout = _uiContainerButtons;
    
    auto objCost = MineralManager::getInstance()->getRefineOpenCost();
    
    //
    auto uiBtn = layout->getChildByName<ui::Button*>("UI_BTN");
    if ( uiBtn != nullptr )
    {
        uiBtn->removeChildByName("UI_COST");
        
        if ( objCost != nullptr )
        {
            auto uiCost = UtilsWidget::getInstance()->createIconText(objCost->getPath(), objCost->getCount(), uiBtn->getContentSize().width - 10, 24, 15);
            uiCost->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height - 30));
            uiCost->setName("UI_COST");
            uiBtn->addChild(uiCost);
        }
        
        bool bLock = false;
        
        // cost
        auto itemNeed = objCost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)objCost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            bLock = true;
        }
        
        if ( bLock == true )
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_max.png");
        }
        else
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_bg.png");
        }
    }
}

#pragma mark - set, get

#pragma mark - callback
void UILayoutMineRefineOpen::callbackPurchaseOk(int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayoutMineRefineOpen::callbackPurchase, this));
}

void UILayoutMineRefineOpen::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
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
        drawButtons();
        
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

#pragma mark - click
void UILayoutMineRefineOpen::onClickMine(cocos2d::Ref* sender)
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
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(UILayoutMineRefineOpen::callbackPurchase, this));
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
            popup->setCallbackResult(CC_CALLBACK_1(UILayoutMineRefineOpen::callbackPurchaseOk, this));
            popup->show();
        }
    });
}

void UILayoutMineRefineOpen::onClickOpen(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    int result = MineralManager::getInstance()->onRefineOpen();
    if ( result != 0 )
    {
        return;
    }
    
    removeFromParent();
}
