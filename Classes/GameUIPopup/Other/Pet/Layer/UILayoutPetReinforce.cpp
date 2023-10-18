//
//  UILayoutPetReinforce.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetReinforce.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

UILayoutPetReinforce* UILayoutPetReinforce::create(E_PET ePet)
{
    UILayoutPetReinforce* pRet = new(std::nothrow) UILayoutPetReinforce();
    if ( pRet && pRet->init(ePet) )
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

UILayoutPetReinforce::UILayoutPetReinforce(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_ePet(E_PET::CHICK_1)
{
    
}

UILayoutPetReinforce::~UILayoutPetReinforce(void)
{
    
}

bool UILayoutPetReinforce::init(E_PET ePet)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayoutPetReinforce::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiStep();
    }
    
    // draw
    drawInfo();
    drawStep();
}

#pragma mark - init
void UILayoutPetReinforce::initVar()
{
    
}

void UILayoutPetReinforce::initUi()
{
    
}



#pragma mark - ui
void UILayoutPetReinforce::uiContainer()
{
    Size size = this->getContentSize();
    Size sizeLayout = size;
    sizeLayout.width -= 10;
    
    _uiContentsUIInfo = ui::Layout::create();
    _uiContentsUIInfo->setContentSize(Size(sizeLayout.width, 164));
    _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContentsUIInfo);
    
    _uiContentsUIStep = ui::Layout::create();
    _uiContentsUIStep->setContentSize(Size(sizeLayout.width, sizeLayout.height - _uiContentsUIInfo->getContentSize().height));
    _uiContentsUIStep->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIStep->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIStep->setPosition(Vec2(size.width / 2, 0));
    _uiContentsUIStep->setLayoutType(ui::Layout::Type::VERTICAL);
    addChild(_uiContentsUIStep);
}

void UILayoutPetReinforce::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    // 82 * 4(328) + 5 * 3(15)
    auto uiMine = ui::Layout::create();
    uiMine->setContentSize(Size(343, 84));
    uiMine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiMine->setPosition(Vec2(0, size.height / 2));
    uiMine->setName("UI_MINE");
    layout->addChild(uiMine);
    
    //
    auto uiArrow = Sprite::create("Assets/ui/pet/pet_arrow.png");
    uiArrow->setPosition(450, size.height / 2);
    layout->addChild(uiArrow);
    
    //
    auto uiReinforce = ui::Button::create("Assets/ui_common/btn_2_1_on.png");
    uiReinforce->setScale9Enabled(true);
    uiReinforce->setCapInsets(Rect(15,25,10,10));
    uiReinforce->setContentSize(Size(140,82));
    uiReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiReinforce->setPosition(Vec2(size.width, size.height / 2));
    uiReinforce->addClickEventListener(CC_CALLBACK_1(UILayoutPetReinforce::onClickReinforce, this));
    layout->addChild(uiReinforce);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_upgrade"), GAME_FONT, 32);
        uiText->setPosition(Vec2(uiReinforce->getContentSize().width / 2, uiReinforce->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiReinforce->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiReinforce->addChild(uiText);
    }
     
}

void UILayoutPetReinforce::uiStep()
{
    auto layout = _uiContentsUIStep;
    Size size = layout->getContentSize();
    
    
    auto uiStepNow = ui::ImageView::create("Assets/ui_common/box.png");
    uiStepNow->setScale9Enabled(true);
    uiStepNow->setContentSize(Size(size.width, 128));
    uiStepNow->setName("UI_STEP_NOW");
    layout->addChild(uiStepNow);
    
    auto uiStepNext = ui::ImageView::create("Assets/ui_common/box.png");
    uiStepNext->setScale9Enabled(true);
    uiStepNext->setContentSize(Size(size.width, 128));
    uiStepNext->setName("UI_STEP_NEXT");
    layout->addChild(uiStepNext);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(0, 25, 0, 0));
        uiStepNext->setLayoutParameter(params);
    }
    
    auto uiStepArrow = ui::Layout::create();
    uiStepArrow->setContentSize(Size(size.width, 72));
    uiStepArrow->setName("UI_STEP_ARROW");
    layout->addChild(uiStepArrow);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(0, 50, 0, 0));
        uiStepArrow->setLayoutParameter(params);
    }
}

#pragma mark - draw
void UILayoutPetReinforce::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    auto uiMine = layout->getChildByName<ui::ImageView*>("UI_MINE");
    if ( uiMine != nullptr )
    {
        uiMine->removeAllChildren();
        
        //
        int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(_ePet);
        
        //
        std::vector<int> listMatter = PetNewManager::getInstance()->getPetReinforceUpgradePrice(_ePet, reinforceLevel);
        for ( int i = 0; i < listMatter.size(); i++ )
        {
            E_MINERAL eMineral = (E_MINERAL)i;
            
            int currencyNeed = listMatter.at(i);
            int currencyNow = MineralManager::getInstance()->getCount(eMineral);
            if ( currencyNeed <= 0 )
            {
                continue;
            }
            
            size_t count = uiMine->getChildrenCount();
            
            double posX = count * 82 + count * 5;
            double posY = uiMine->getContentSize().height / 2;
            
            //
            auto uiBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiBG->setScale9Enabled(true);
            uiBG->setCapInsets(Rect(5,30,10,10));
            uiBG->setContentSize(Size(82, 82));
            uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBG->setPosition(Vec2(posX, posY));
            uiMine->addChild(uiBG);
            
            //
            auto uiIcon= ui::ImageView::create(DataManager::GetMineMineralPath(eMineral));
            uiIcon->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
            uiIcon->setScale(1.4);
            uiBG->addChild(uiIcon);

            //
            auto uiProgressBG = ui::ImageView::create("Assets/ui/pet/box_item_progressbar_1.png");
            uiProgressBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiProgressBG->setPosition(Vec2(0,0));
            uiBG->addChild(uiProgressBG);
            {
                double percent = (double)currencyNow / currencyNeed * 100;
                if ( percent > 100 )
                    percent = 100;

                std::string currencyNowText = MafUtils::toString(currencyNow);
                MafUtils::convertNumberToShort(currencyNowText);

                std::string currencyNeedText = MafUtils::toString(currencyNeed);
                MafUtils::convertNumberToShort(currencyNeedText);

                auto progress = ProgressTimer::create(Sprite::create("Assets/ui/pet/box_item_progressbar_2.png"));
                progress->setPosition(uiProgressBG->getContentSize()/2);
                progress->setMidpoint(Vec2(0,0.5));
                progress->setBarChangeRate(Vec2(1,0));
                progress->setType(ProgressTimer::Type::BAR);
                progress->setPercentage(percent);
                uiProgressBG->addChild(progress);

                auto uiCount = ui::CText::create(MafUtils::format("%s/%s", currencyNowText.c_str(), currencyNeedText.c_str()), GAME_FONT, 14);
                uiCount->setPosition(Vec2(uiProgressBG->getContentSize().width / 2, 10.5));
                uiProgressBG->addChild(uiCount);
                if ( reinforceLevel >= C_MAX_PET_GRADE )
                {
                    uiCount->setString("-");
                }
            }
            
            if ( reinforceLevel < C_MAX_PET_GRADE && currencyNow < currencyNeed )
            {
                auto uiCash = ui::Button::create("Assets/ui_common/box_item_1_2.png");
                uiCash->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
                uiCash->setTag(eMineral);
                uiCash->addClickEventListener(CC_CALLBACK_1(UILayoutPetReinforce::onClickMine, this));
                uiBG->addChild(uiCash);

                auto uiReddot = ui::ImageView::create("Assets/ui/exclamation_mark_icon_2.png");
                uiReddot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(Vec2(0, uiBG->getContentSize().height));
                uiBG->addChild(uiReddot);
            }
        }
    }
}

void UILayoutPetReinforce::drawStep()
{
    auto layout = _uiContentsUIStep;
    Size size = layout->getContentSize();
    
    int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(_ePet);
    int reinforceLevelNext = reinforceLevel + 1;
    
    //
    auto uiStepNow = layout->getChildByName<ui::ImageView*>("UI_STEP_NOW");
    if ( uiStepNow != nullptr )
    {
        uiStepNow->removeAllChildren();
        if ( reinforceLevel > 0 )
        {
            uiStepNow->setVisible(true);
            
            //
            std::string stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7", reinforceLevel);
            if ( reinforceLevel == 1 )          stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_1", reinforceLevel);
            else if ( reinforceLevel == 2 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_2", reinforceLevel);
            else if ( reinforceLevel == 3 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_3", reinforceLevel);
            stepText = GAME_TEXTFORMAT("t_ui_pet_msg_13", stepText.c_str());
            
            auto uiStep = ui::CText::create(stepText, GAME_FONT, 22);
            uiStep->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiStep->setPosition(Vec2(uiStepNow->getContentSize().width / 2, uiStepNow->getContentSize().height / 2 + 2.5));
            uiStep->setTextAreaSize(Size(uiStepNow->getContentSize().width - 10, uiStep->getContentSize().height));
            uiStep->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiStep->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiStep->setTextOverflow(Label::Overflow::SHRINK);
            uiStepNow->addChild(uiStep);
            
            //
            std::string effectText = PetNewManager::getInstance()->getPetReinforceExplain(_ePet, reinforceLevel);
            effectText = GAME_TEXTFORMAT("t_ui_pet_msg_15", effectText.c_str());
            
            auto uiEffect = ui::CText::create(effectText, GAME_FONT, 26);
            uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiEffect->setPosition(Vec2(uiStepNow->getContentSize().width / 2, uiStepNow->getContentSize().height / 2 - 2.5));
            uiEffect->setTextAreaSize(Size(uiStepNow->getContentSize().width - 10, uiEffect->getContentSize().height));
            uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiEffect->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiEffect->setTextOverflow(Label::Overflow::SHRINK);
            uiEffect->setTextColor(Color4B(COLOR_TITLE));
            uiStepNow->addChild(uiEffect);
        }
        else
        {
            uiStepNow->setVisible(false);
        }
    }
    
    //
    auto uiStepNext = layout->getChildByName<ui::ImageView*>("UI_STEP_NEXT");
    if ( uiStepNext != nullptr )
    {
        uiStepNext->removeAllChildren();
        if ( reinforceLevel < C_MAX_PET_GRADE )
        {
            uiStepNext->setVisible(true);
            
            //
            std::string stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7", reinforceLevelNext);
            if ( reinforceLevelNext == 1 )          stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_1", reinforceLevelNext);
            else if ( reinforceLevelNext == 2 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_2", reinforceLevelNext);
            else if ( reinforceLevelNext == 3 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_3", reinforceLevelNext);
            stepText = GAME_TEXTFORMAT("t_ui_pet_msg_14", stepText.c_str());
            
            auto uiStep = ui::CText::create(stepText, GAME_FONT, 22);
            uiStep->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiStep->setPosition(Vec2(uiStepNext->getContentSize().width / 2, uiStepNext->getContentSize().height / 2 + 2.5));
            uiStep->setTextAreaSize(Size(uiStepNext->getContentSize().width - 10, uiStep->getContentSize().height));
            uiStep->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiStep->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiStep->setTextOverflow(Label::Overflow::SHRINK);
            uiStep->setTextColor(Color4B(128, 128, 128, 255));
            uiStepNext->addChild(uiStep);
            
            //
            std::string effectText = PetNewManager::getInstance()->getPetReinforceExplain(_ePet, reinforceLevelNext);
            effectText = GAME_TEXTFORMAT("t_ui_pet_msg_15", effectText.c_str());
            
            auto uiEffect = ui::CText::create(effectText, GAME_FONT, 26);
            uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiEffect->setPosition(Vec2(uiStepNext->getContentSize().width / 2, uiStepNext->getContentSize().height / 2 - 2.5));
            uiEffect->setTextAreaSize(Size(uiStepNext->getContentSize().width - 10, uiEffect->getContentSize().height));
            uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiEffect->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiEffect->setTextOverflow(Label::Overflow::SHRINK);
            uiEffect->setTextColor(Color4B(128, 128, 128, 255));
            uiStepNext->addChild(uiEffect);
        }
        else
        {
            uiStepNext->setVisible(false);
        }
    }
    
    //
    auto uiStepArrow = layout->getChildByName<ui::ImageView*>("UI_STEP_ARROW");
    if ( uiStepArrow != nullptr )
    {
        uiStepArrow->removeAllChildren();
        
        //
        int listLevel[] = {1, 2, 3, 4, 5};
        for ( int i = 0; i < sizeof(listLevel)/sizeof(int); i++ )
        {
            double posY = uiStepArrow->getContentSize().height / 2;
            double posX = i * 144;
            if ( i > 0 )
                posX -= 16;
                
            std::string pathBG = "Assets/ui/bonsikdex/book_step1_2.png";
            if ( i == 0 )
                pathBG = "Assets/ui/bonsikdex/book_step1_1.png";
            
            auto uiBG = ui::ImageView::create(pathBG);
            uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBG->setPosition(Vec2(posX, posY));
            uiStepArrow->addChild(uiBG);
            
            //
            std::string stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7", listLevel[i]);
            if ( listLevel[i] == 1 )          stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_1", listLevel[i]);
            else if ( listLevel[i] == 2 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_2", listLevel[i]);
            else if ( listLevel[i] == 3 )     stepText = GAME_TEXTFORMAT("t_ui_pet_msg_7_3", listLevel[i]);

            auto uiStep = ui::CText::create(stepText, GAME_FONT, 22);
            uiStep->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiStep->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
            uiStep->setTextAreaSize(Size(110, uiStep->getContentSize().height));
            uiStep->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiStep->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiStep->setTextOverflow(Label::Overflow::SHRINK);
            uiBG->addChild(uiStep);
            if ( i == 0 )
            {
                uiStep->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                uiStep->setPositionX(10);
            }

            if ( reinforceLevel >= listLevel[i] )
            {
                uiBG->loadTexture("Assets/ui/bonsikdex/book_step2_2.png");
                if ( i == 0 )
                    uiBG->loadTexture("Assets/ui/bonsikdex/book_step2_1.png");
            }
        }
    }
    
    //
    layout->requestDoLayout();
}
 
#pragma mark - set, get, add


#pragma mark - callback
void UILayoutPetReinforce::callbackPurchaseOk(int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayoutPetReinforce::callbackPurchase, this));
}

void UILayoutPetReinforce::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
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
        drawInfo();
        
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
void UILayoutPetReinforce::onClickMine(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_MINERAL eMineral = (E_MINERAL)uiBtn->getTag();
    E_CASH eCash = E_CASH::CASH_MINE_1;
    switch (eMineral) {
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
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(UILayoutPetReinforce::callbackPurchase, this));
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
            popup->setCallbackResult(CC_CALLBACK_1(UILayoutPetReinforce::callbackPurchaseOk, this));
            popup->show();
        }
    });
}

void UILayoutPetReinforce::onClickReinforce(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int result = PetNewManager::getInstance()->onPetReinforce(_ePet);
    if ( result != 0 )
    {
        /*
         0 : 성공
         1 : 조건 안됨
         2 : 보유하지 않음
         3 : 최대 레벨
         4 : 재화 부족
         */
        if ( result == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_pet_msg_9"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else if ( result == 3 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_pet_msg_10"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else if ( result == 4 )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::MINE_STONE_IRON);
            popup->show();
        }
        return;
    }
    
    //
    drawInfo();
    drawStep();
}
