//
//  UILayoutCashBase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutCashBase.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"
#include "GameUIPopup/Other/PurchaseCash/UI/UICashBox.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseDailyReward.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayoutCashBase::UILayoutCashBase()
{
    
}

bool UILayoutCashBase::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    
    return true;
}

#pragma mark - init
void UILayoutCashBase::initVar()
{
    
}

void UILayoutCashBase::initUI()
{
    
}

#pragma mark - ui

#pragma mark - draw

#pragma mark - set, get

#pragma mark - callback
void UILayoutCashBase::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        return;
    }
    
    // info daily network
    PopupLoading::show();
    CashManager::getInstance()->requestDailyInfo(CC_CALLBACK_1(UILayoutCashBase::callbackDailyInfo, this));
}

void UILayoutCashBase::callbackDailyInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        return;
    }
 
    // info Remaining
    PopupLoading::show();
    CashManager::getInstance()->requestPurchaseRemainingAll(CC_CALLBACK_1(UILayoutCashBase::callbackRemainingInfo, this));
}

void UILayoutCashBase::callbackRemainingInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        return;
    }
    
    //
    onDraw();
}

void UILayoutCashBase::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayoutCashBase::callbackInfo, this));
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        
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

void UILayoutCashBase::callbackPurchaseDailyReceive(bool bResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    // 재화 다시 가져오기.
    PopupLoading::show();
    CashManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayoutCashBase::callbackInfo, this));
    
    auto objProduct = CashManager::getInstance()->getProduct(nIdx);
    if ( objProduct != nullptr )
    {
        auto popup = PopupPurchaseDailyReward::create(objProduct->getName());
        popup->show();
    }
}


#pragma mark - click

#pragma mark - game
void UILayoutCashBase::onPurchase(int idx)
{
    auto objProduct = CashManager::getInstance()->getProduct(idx);
    if ( objProduct == nullptr )
    {
        return;
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(idx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(idx, CC_CALLBACK_4(UILayoutCashBase::callbackPurchase, this));
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
            auto popup = PopupPurchaseBuy::create(idx);
            popup->setCallbackResult([=](int idx){
                
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(idx, CC_CALLBACK_4(UILayoutCashBase::callbackPurchase, this));
            });
            popup->show();
        }
    });
}

void UILayoutCashBase::onPurchaseDailyReceive(int idx)
{
    auto objProduct = CashManager::getInstance()->getProduct(idx);
    if ( objProduct == nullptr )
    {
        return;
    }
    
    if ( objProduct->getShopType() != E_PRODUCT::DAILY )
    {
        return;
    }
    
    if ( CashManager::getInstance()->getDailyDate(idx) > 0 && CashManager::getInstance()->isDailyReceive(idx) == true )
    {
        PopupLoading::show();
        CashManager::getInstance()->requestDailyReceive(idx, CC_CALLBACK_3(UILayoutCashBase::callbackPurchaseDailyReceive, this));
    }
}

