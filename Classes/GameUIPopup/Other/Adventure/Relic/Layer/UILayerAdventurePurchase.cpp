//
//  UILayerAdventurePurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#include "UILayerAdventurePurchase.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"
#include "GameUIPopup/Other/PurchaseCash/UI/UICashBox.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

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

UILayerAdventurePurchase* UILayerAdventurePurchase::create(cocos2d::Size size)
{
    UILayerAdventurePurchase* pRet = new(std::nothrow) UILayerAdventurePurchase();
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

UILayerAdventurePurchase::UILayerAdventurePurchase(void):
_lyTopUI(nullptr)
,_lyProduct(nullptr)
,_layerContainerContentsScrollView(nullptr)

,_onCallbackInfo(nullptr)
{
}

UILayerAdventurePurchase::~UILayerAdventurePurchase(void)
{
}

bool UILayerAdventurePurchase::init(cocos2d::Size size)
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
void UILayerAdventurePurchase::refreshUI()
{
    uiTop();
    uiList();
}

#pragma mark - init
void UILayerAdventurePurchase::initVar()
{
    
}

void UILayerAdventurePurchase::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 140));
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
void UILayerAdventurePurchase::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_shop_box1_1.png");
    sprBg->setContentSize(Size(_lyTopUI->getContentSize().width - 5, sprBg->getContentSize().height));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBg->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 5);
    _lyTopUI->addChild(sprBg);
}

void UILayerAdventurePurchase::uiList()
{
    auto offset = _layerContainerContentsScrollView->getInnerContainerPosition();
    
    bool first = false;
    if ( _layerContainerContentsScrollView->getChildrenCount() <= 0 )
    {
        first = true;
    }
    
    _layerContainerContentsScrollView->removeAllChildren();
    
    double width = _layerContainerContentsScrollView->getInnerContainerSize().width;
    _layerContainerContentsScrollView->addChild(getUiGroup(400, width));
    
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

cocos2d::ui::Layout* UILayerAdventurePurchase::getUiGroup(int group, double width)
{
    std::string pathBox = "Assets/ui/purchase/shop_box4_2.png";
    
    //
    auto layoutInner = ui::Layout::create();
    
    auto listProduct = CashManager::getInstance()->getProductGroup(group);
    for ( auto& product : listProduct )
    {
        auto uiBox = UICashBox::create(product->getIdx(), pathBox);
        uiBox->setTouchEnabled(true);
        uiBox->addClickEventListener(CC_CALLBACK_1(UILayerAdventurePurchase::onClickProduct, this));
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
void UILayerAdventurePurchase::callbackInfo(bool bResult)
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
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void UILayerAdventurePurchase::onClickProduct(cocos2d::Ref* sender)
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
                CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerAdventurePurchase::callbackPurchase, this));
            });
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
        else
        {
            auto popup = PopupPurchaseBuy::create(nIdx);
            popup->setCallbackResult(CC_CALLBACK_1(UILayerAdventurePurchase::callbackProductOk, this));
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
    });
}

void UILayerAdventurePurchase::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerAdventurePurchase::callbackPurchase, this));
}
void UILayerAdventurePurchase::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayerAdventurePurchase::callbackInfo, this));
        
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
        AdventureRelicManager::getInstance()->requestInfoCore([=](bool, int){
            PopupLoading::hide();
            
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE_TUTORIAL_RUNNING);
            else
                RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
            
        });
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
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
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

#pragma mark - click
