//
//  UILayerRaidPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#include "UILayerRaidPurchase.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"
#include "GameUIPopup/Other/PurchaseCash/UI/UICashBox.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

static bool sortList(InfoProduct *a, InfoProduct *b)
{
    bool result = false;
    
    if(b->getSaleLimit() > 0 && CashManager::getInstance()->getPurchaseRemaining(b->getIdx()) <= 0)
    {
        if(a->getSaleLimit() > 0 && CashManager::getInstance()->getPurchaseRemaining(a->getIdx()) <= 0)
            result = a->getOrder() < b->getOrder();
        else
            result = true;
    }
    else
        result = a->getOrder() < b->getOrder();
    
    return result;
}

UILayerRaidPurchase* UILayerRaidPurchase::create(cocos2d::Size size)
{
    UILayerRaidPurchase* pRet = new(std::nothrow) UILayerRaidPurchase();
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

UILayerRaidPurchase::UILayerRaidPurchase(void):
_lyTopUI(nullptr)
,_lyProduct(nullptr)
,_layerContainerContentsScrollView(nullptr)

,_onCallbackInfo(nullptr)
{
}

UILayerRaidPurchase::~UILayerRaidPurchase(void)
{
}

bool UILayerRaidPurchase::init(cocos2d::Size size)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    setContentSize(size);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - override
void UILayerRaidPurchase::refreshUI()
{
    uiTop();
    uiList();
}

#pragma mark - init
void UILayerRaidPurchase::initVar()
{
    
}

void UILayerRaidPurchase::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 230));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI);
    
    _lyProduct = LayerColor::create();
    _lyProduct->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - 10));
    _lyProduct->setIgnoreAnchorPointForPosition(false);
    _lyProduct->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyProduct->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyProduct);
        
    _layerContainerContentsScrollView = ui::ScrollView::create();
    _layerContainerContentsScrollView->setContentSize(_lyProduct->getContentSize());
    _layerContainerContentsScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerContentsScrollView->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsScrollView->setPosition(Vec2(_lyProduct->getContentSize().width / 2 , 0));
    _layerContainerContentsScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _layerContainerContentsScrollView->setScrollBarPositionFromCorner(Vec2(4, 4));
    _layerContainerContentsScrollView->setScrollBarColor(Color3B::YELLOW);
    _layerContainerContentsScrollView->setScrollBarAutoHideEnabled(false);
    _layerContainerContentsScrollView->setBounceEnabled(false);
    _layerContainerContentsScrollView->setLayoutType(ui::Layout::Type::VERTICAL);
    _lyProduct->addChild(_layerContainerContentsScrollView);
    
    //
    uiTop();
    uiList();
}

#pragma mark - init
void UILayerRaidPurchase::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui/raid/raid_shop/raid_shop_bg_3.png");
    sprBg->setContentSize(Size(getContentSize().width, sprBg->getContentSize().height));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(_lyTopUI->getContentSize()/2);
    _lyTopUI->addChild(sprBg);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_purchase_double_ticket"), GAME_FONT, 40);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTitle->setPosition(10, _lyTopUI->getContentSize().height - 10);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
    lbTitle->setDimensions(_lyTopUI->getContentSize().width - 200, lbTitle->getContentSize().height);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setColor(Color3B(255,253,95));
    lbTitle->enableOutline(Color4B(73,43,24, 255), 1);
    _lyTopUI->addChild(lbTitle);
    
    auto lbMsg1 = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_purchase_double_ticket_mag"), GAME_FONT, 20);
    lbMsg1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMsg1->setPosition(lbTitle->getPosition() + Vec2::DOWN * (lbTitle->getContentSize().height + 5));
    lbMsg1->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
    lbMsg1->setDimensions(_lyTopUI->getContentSize().width - 200, lbMsg1->getContentSize().height);
    lbMsg1->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbMsg1);
    
    auto lbMsg2 = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_purchase_double_ticket_mag2"), GAME_FONT, 16);
    lbMsg2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMsg2->setPosition(lbMsg1->getPosition() + Vec2::DOWN * (lbMsg1->getContentSize().height + 5));
    lbMsg2->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
    lbMsg2->setDimensions(_lyTopUI->getContentSize().width - 200, lbMsg2->getContentSize().height);
    lbMsg2->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbMsg2);
    
    auto lbWarning = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_purchase_double_ticket_mag3"), GAME_FONT, 16);
    lbWarning->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbWarning->setPosition(10, 10);
    lbWarning->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
    lbWarning->setDimensions(_lyTopUI->getContentSize().width - 20, lbWarning->getContentSize().height);
    lbWarning->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbWarning);
    
    auto objProduct = CashManager::getInstance()->getProduct(70000000);
    
    auto btnTicketBuy = ui::Button::create("Assets/ui_common/btn_1_5_on.png");
    btnTicketBuy->setScale9Enabled(true);
    btnTicketBuy->setContentSize(Size(180, 126));
    btnTicketBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnTicketBuy->setPosition(Vec2(_lyTopUI->getContentSize().width - 10, _lyTopUI->getContentSize().height/2 + 18));
    btnTicketBuy->setZoomScale(0.05f);
    btnTicketBuy->addClickEventListener(CC_CALLBACK_1(UILayerRaidPurchase::onClickDoubleTicket, this));
    btnTicketBuy->setTitleText(MafUtils::format("%s\n%s", objProduct->getPrice().c_str(), GAME_TEXT("t_ui_buy").c_str()));
    btnTicketBuy->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnTicketBuy->getTitleLabel()->setDimensions(btnTicketBuy->getContentSize().width, btnTicketBuy->getContentSize().height);
    btnTicketBuy->setTitleFontSize(24);
    btnTicketBuy->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(btnTicketBuy);
    
    if(NewRaidManager::getInstance()->isTicket())
    {
        btnTicketBuy->setTitleText(GAME_TEXT("t_ui_raid_tickt_noti"));
    }
}

void UILayerRaidPurchase::uiList()
{
    auto offset = _layerContainerContentsScrollView->getInnerContainerPosition();
    
    bool first = false;
    if ( _layerContainerContentsScrollView->getChildrenCount() <= 0 )
    {
        first = true;
    }
    
    _layerContainerContentsScrollView->removeAllChildren();
    
    double width = _layerContainerContentsScrollView->getInnerContainerSize().width;
    _layerContainerContentsScrollView->addChild(getUiGroup(300, width));
    
    //
    auto sizeInner = UtilsWidget::getInstance()->getLayoutAccumulatedSize(_layerContainerContentsScrollView->getInnerContainer());
    _layerContainerContentsScrollView->setInnerContainerSize(sizeInner);
    _layerContainerContentsScrollView->getInnerContainer()->requestDoLayout();
    
    if ( first == true )
    {
        offset.y = (sizeInner.height - _layerContainerContentsScrollView->getContentSize().height) * -1;
    }
    _layerContainerContentsScrollView->setInnerContainerPosition(offset);
}

cocos2d::ui::Layout* UILayerRaidPurchase::getUiGroup(int group, double width)
{
    std::string pathBox = "Assets/ui/purchase/shop_box4_2.png";

    //
    auto layoutInner = ui::Layout::create();
    
    auto listProduct = CashManager::getInstance()->getProductGroup(group);
    for ( auto& product : listProduct )
    {
        auto uiBox = UICashBox::create(product->getIdx(), pathBox);
        uiBox->setTouchEnabled(true);
        uiBox->addClickEventListener(CC_CALLBACK_1(UILayerRaidPurchase::onClickProduct, this));
        uiBox->setTag(product->getIdx());
        layoutInner->addChild(uiBox);
    }
    UtilsWidget::getInstance()->sortWidgetGrid(layoutInner, 2, width, 0, 40, false, true);
    
    //
    auto size = Size(width, layoutInner->getContentSize().height);
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    
    //
    layout->addChild(layoutInner);
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

#pragma mark - callback
void UILayerRaidPurchase::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        std::string strMsg = GAME_TEXT("t_ui_error_2");
        if ( AccountManager::getInstance()->getUserID().length() == 0 )
        {
            strMsg = GAME_TEXT("t_ui_error_1");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void UILayerRaidPurchase::onClickProduct(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    int nIdx = item->getTag();
    
    auto objProduct = CashManager::getInstance()->getProduct(nIdx);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(nIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerRaidPurchase::callbackPurchase, this));
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
            auto popup = PopupPurchaseBuy::create(nIdx);
            popup->setCallbackResult(CC_CALLBACK_1(UILayerRaidPurchase::callbackProductOk, this));
            popup->show();
        }
    });
}

void UILayerRaidPurchase::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerRaidPurchase::callbackPurchase, this));
}
void UILayerRaidPurchase::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayerRaidPurchase::callbackInfo, this));
        
        PopupLoading::show();
        CashManager::getInstance()->requestPurchaseRemainingAll([=](bool bResult){
            PopupLoading::hide();
            uiList();
        });
        
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        PopupLoading::show();
        NewRaidManager::getInstance()->requestInfo([=](bool, int){
            PopupLoading::hide();
            
            RefreshManager::getInstance()->refreshUI(E_REFRESH::RAID_ALL);
        });
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        if(nIdx == 70000000)
            strText = GAME_TEXT("t_ui_new_raid_ticket_pop_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
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
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

#pragma mark - click
void UILayerRaidPurchase::onClickDoubleTicket(cocos2d::Ref* sender)
{
    if(NewRaidManager::getInstance()->isTicket())
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_new_raid_ticket_pop_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(70000000);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(70000000, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidPurchase::callbackPurchase, this));
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
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            
            std::string strMsg = GAME_TEXT("t_ui_new_raid_ticket_pop_1") + "\n" + GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min);
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidPurchase::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
        }
    });
}
