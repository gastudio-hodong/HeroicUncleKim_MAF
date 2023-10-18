//
//  PopupAdsSkip.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupAdsSkip.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupAdsSkip* PopupAdsSkip::create()
{
    PopupAdsSkip* pRet = new(std::nothrow) PopupAdsSkip();
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

PopupAdsSkip::PopupAdsSkip():

_uiContainerContent(nullptr)
{
    
}

PopupAdsSkip::~PopupAdsSkip()
{
    
}

bool PopupAdsSkip::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupAdsSkip::initVar()
{
    
}

void PopupAdsSkip::initUI()
{
    // size
    Size size = Size(620, 431);
    
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_adskip"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdsSkip::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _uiContainerContent = ui::Layout::create();
    _uiContainerContent->setContentSize(Size(size.width - 25, size.height - 75 - 25 - 25));
    _uiContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_uiContainerContent);
    
    
    //
    uiContent();
}
 
 
#pragma mark - ui
void PopupAdsSkip::uiContent()
{
    _uiContainerContent->removeAllChildren();
    
    auto layout = _uiContainerContent;
    
    auto uiImgBG = ui::ImageView::create("Assets/ui/ads/adskip_1.png");
    uiImgBG->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
    layout->addChild(uiImgBG);
    
    auto uiImgIcon = ui::ImageView::create("Assets/ui/ads/adsfree_img.png");
    uiImgIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiImgIcon->setPosition(Vec2(0, uiImgBG->getContentSize().height));
    uiImgBG->addChild(uiImgIcon);
    
    // title
    auto uiTextTitle = ui::CText::create(GAME_TEXT("t_ui_adskip_title"), GAME_FONT, 26);
    uiTextTitle->setPosition(Vec2(uiImgBG->getContentSize().width - 183, uiImgBG->getContentSize().height - 22.5));
    uiTextTitle->enableOutline(Color4B(0,22,56,255), 2);
    uiTextTitle->setTextAreaSize(Size(230, uiTextTitle->getContentSize().height));
    uiTextTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTextTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiImgBG->addChild(uiTextTitle);
    
    // desc
    _uiTextDesc = ui::CText::create(GAME_TEXT("t_ui_adskip_msg_1"), GAME_FONT, 21);
    _uiTextDesc->setPosition(Vec2(uiImgBG->getContentSize().width - 110, uiImgBG->getContentSize().height - 100));
    _uiTextDesc->enableOutline(Color4B(0,22,56,255), 2);
    _uiTextDesc->setTextAreaSize(Size(180, _uiTextDesc->getContentSize().height));
    _uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
    _uiTextDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiTextDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTextDesc->setLineSpacing(-10);
    uiImgBG->addChild(_uiTextDesc);
    
    auto uiBtnPurchase = ui::Button::create("Assets/ui/ads/adskip_btn.png");
    uiBtnPurchase->setPosition(Vec2(uiImgBG->getContentSize().width - 183, uiImgBG->getContentSize().height - 190));
    uiBtnPurchase->addClickEventListener(CC_CALLBACK_1(PopupAdsSkip::onClickPurchase, this));
    uiImgBG->addChild(uiBtnPurchase);
    {
        _uiTextActivePlus1 = ui::CText::create(GAME_TEXT("t_ui_adskip_msg_7"), GAME_FONT, 21);
        _uiTextActivePlus1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiTextActivePlus1->setPosition(Vec2(uiBtnPurchase->getContentSize().width / 2, uiBtnPurchase->getContentSize().height));
        _uiTextActivePlus1->enableOutline(Color4B(0,22,56,255), 2);
        _uiTextActivePlus1->setTextAreaSize(Size(300, _uiTextActivePlus1->getContentSize().height));
        _uiTextActivePlus1->setTextOverflow(Label::Overflow::SHRINK);
        _uiTextActivePlus1->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnPurchase->addChild(_uiTextActivePlus1);
        
        auto uiTextPrice = ui::CText::create("", GAME_FONT, 21);
        uiTextPrice->setPosition(Vec2(uiBtnPurchase->getContentSize().width / 2, uiBtnPurchase->getContentSize().height / 2));
        uiTextPrice->enableOutline(Color4B(0,22,56,255), 2);
        uiBtnPurchase->addChild(uiTextPrice);
        {
            auto action1 = CallFuncN::create([=](Ref* sender){
                
                auto obj = CashManager::getInstance()->getProduct(E_CASH::CASH_DAILY_ADS_NEW);
                std::string strProductID = obj->getProductIdReal();
                std::string strPrice = obj->getPrice();
                
                if ( strPrice.length() != 0 )
                {
                    ((ui::CText*)sender)->setString(strPrice);
                    ((ui::CText*)sender)->stopAllActions();
                    return;
                }
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            uiTextPrice->runAction(RepeatForever::create(seq));
        }
    }
    
    _uiTextActive = ui::CText::create(GAME_TEXT("t_ui_ad_deactivated"), GAME_FONT, 30);
    _uiTextActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiTextActive->setPosition(Vec2(uiImgBG->getContentSize().width - 183, uiImgBG->getContentSize().height - 270));
    _uiTextActive->enableOutline(Color4B(0,22,56,255), 2);
    _uiTextActive->setTextAreaSize(Size(270, _uiTextActive->getContentSize().height));
    _uiTextActive->setTextOverflow(Label::Overflow::SHRINK);
    _uiTextActive->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiImgBG->addChild(_uiTextActive);
    
    _uiTextActivePlus2 = ui::CText::create(GAME_TEXT("t_ui_adskip_msg_8"), GAME_FONT, 18);
    _uiTextActivePlus2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiTextActivePlus2->setPosition(Vec2(uiImgBG->getContentSize().width - 183, uiImgBG->getContentSize().height - 270));
    _uiTextActivePlus2->enableOutline(Color4B(0,22,56,255), 2);
    _uiTextActivePlus2->setTextAreaSize(Size(270, _uiTextActivePlus2->getContentSize().height));
    _uiTextActivePlus2->setTextOverflow(Label::Overflow::SHRINK);
    _uiTextActivePlus2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiImgBG->addChild(_uiTextActivePlus2);
        
    
    //
    drawContent();
}

#pragma mark - draw
void PopupAdsSkip::drawContent()
{
    std::string desc = GAME_TEXT("t_ui_adskip_msg_1");
    
    time_t adsFreeTime = CashManager::getInstance()->getAdsfreeTime();
    if ( adsFreeTime > 0)
    {
        desc = GAME_TEXT("t_ui_adskip_msg_2");
        
        int day = (int)(adsFreeTime / (24 * 3600));
        if ( day > 0 )
        {
            desc.append(GAME_TEXTFORMAT("t_ui_adskip_msg_3", day));
            desc.append(" ");
            
            adsFreeTime = adsFreeTime % (24 * 3600);
        }
        
        int hour = (int)(adsFreeTime / 3600);
        if ( hour > 0)
        {
            desc.append(GAME_TEXTFORMAT("t_ui_adskip_msg_4", hour));
        }
        else
        {
            desc.append(GAME_TEXT("t_ui_adskip_msg_5"));
        }
    }
    
    
    if( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        _uiTextDesc->setString(desc);
        _uiTextActive->setString(GAME_TEXT("t_ui_ad_activated"));
        _uiTextActivePlus1->setVisible(true);
        _uiTextActivePlus2->setVisible(true);
    }
    else
    {
        _uiTextDesc->setString(desc);
        _uiTextActive->setString(GAME_TEXT("t_ui_ad_deactivated"));
        _uiTextActivePlus1->setVisible(false);
        _uiTextActivePlus2->setVisible(false);
    }
        
}


#pragma mark - set, get

#pragma mark - callback
void PopupAdsSkip::callbackBuyPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        drawContent();
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
void PopupAdsSkip::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdsSkip::onClickPurchase(Ref* sender)
{
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    int nItemIdx = E_CASH::CASH_DAILY_ADS_NEW;
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        nItemIdx = E_CASH::CASH_DAILY_ADS_CONTINUE;
    }
    
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupAdsSkip::callbackBuyPurchase, this));
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
            PopupLoading::show();
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupAdsSkip::callbackBuyPurchase, this));
        }
    });
    
}
 
