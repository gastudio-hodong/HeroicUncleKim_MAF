//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupEventRaceReset.h"

#include "Common/ConfigGameNormal.h"

#include "GameScene/Base/BaseGameScene.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceTitle.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/RankingManager.h"
#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

PopupEventRaceReset* PopupEventRaceReset::create()
{
    PopupEventRaceReset* pRet = new(std::nothrow) PopupEventRaceReset();
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

PopupEventRaceReset::PopupEventRaceReset(void) :
_bInitUI(false)
{
    
}

PopupEventRaceReset::~PopupEventRaceReset(void)
{
    
}

bool PopupEventRaceReset::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupEventRaceReset");
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupEventRaceReset::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiButtons();
    }
    
    //
    drawInfo();
    drawButtons();
}

#pragma mark - init
void PopupEventRaceReset::initVar()
{
    
}

void PopupEventRaceReset::initUi()
{
    Size size = Size(578, 608);
    
    // size
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_race_reset_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventRaceReset::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupEventRaceReset::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 28 - 28;
    size.height = size.height - 75 - 28;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(layout);
    
    _layerContainerButtons = ui::Layout::create();
    _layerContainerButtons->setContentSize( Size(size.width, 92) );
    _layerContainerButtons->setIgnoreAnchorPointForPosition(false);
    _layerContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    _layerContainerButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(_layerContainerButtons);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, size.height - _layerContainerButtons->getContentSize().height) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
}

void PopupEventRaceReset::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    
    // 나의 순위
    auto uiInfoRank = ui::Layout::create();
    uiInfoRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoRank->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiInfoRank->setName("UI_INFO_RANK");
    layout->addChild(uiInfoRank);
    {
        auto uiNameBG = ui::ImageView::create("Assets/ui_common/base_box_1_17.png");
        uiNameBG->setScale9Enabled(true);
        uiNameBG->setContentSize(Size(138, 48));
        uiInfoRank->addChild(uiNameBG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_race_reset_rank_1"), GAME_FONT, 24);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextAreaSize(Size(uiNameBG->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiNameBG->addChild(uiText);
        }
        
        auto uiInfoBG = ui::ImageView::create("Assets/ui_common/base_box_1_18.png");
        uiInfoBG->setScale9Enabled(true);
        uiInfoBG->setContentSize(Size(320, 48));
        uiInfoBG->setName("UI_INFO_BG");
        uiInfoRank->addChild(uiInfoBG);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiInfoBG->setLayoutParameter(params);
        }
        
        //
        Size sizeLayout = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiInfoRank);
        uiInfoRank->setContentSize(sizeLayout);
        uiInfoRank->setPosition(Vec2(size.width / 2, size.height - 25));
        uiInfoRank->requestDoLayout();
    }
    
    // 보유
    auto uiInfoItem = ui::Layout::create();
    uiInfoItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoItem->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiInfoItem->setName("UI_INFO_ITEM");
    layout->addChild(uiInfoItem);
    {
        auto uiNameBG = ui::ImageView::create("Assets/ui_common/base_box_1_17.png");
        uiNameBG->setScale9Enabled(true);
        uiNameBG->setContentSize(Size(138, 48));
        uiInfoItem->addChild(uiNameBG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_race_reset_rank_2"), GAME_FONT, 24);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextAreaSize(Size(uiNameBG->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiNameBG->addChild(uiText);
        }
        
        auto uiInfoBG = ui::ImageView::create("Assets/ui_common/base_box_1_18.png");
        uiInfoBG->setScale9Enabled(true);
        uiInfoBG->setContentSize(Size(320, 48));
        uiInfoBG->setName("UI_INFO_BG");
        uiInfoItem->addChild(uiInfoBG);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiInfoBG->setLayoutParameter(params);
        }
        
        //
        Size sizeLayout = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiInfoItem);
        uiInfoItem->setContentSize(sizeLayout);
        uiInfoItem->setPosition(Vec2(size.width / 2, size.height - 83));
        uiInfoItem->requestDoLayout();
    }
    
    // desc
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_event_race_reset_msg_1"), GAME_FONT, 30);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiDesc->setPosition(Vec2(size.width / 2, 25));
    uiDesc->setTextAreaSize(Size(size.width - 50, 220));
    uiDesc->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiDesc);
}

void PopupEventRaceReset::uiButtons()
{
    auto layout = _layerContainerButtons;
    Size size = layout->getContentSize();

    
    _uiBtnResetPayment = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    _uiBtnResetPayment->setScale9Enabled(true);
    _uiBtnResetPayment->setContentSize(Size(210, 92));
    _uiBtnResetPayment->setCapInsets(Rect(33,33,2,2));
    _uiBtnResetPayment->addClickEventListener(CC_CALLBACK_1(PopupEventRaceReset::onClickResetPayment, this));
    layout->addChild(_uiBtnResetPayment);
    {
        auto action1 = CallFuncN::create( [=]( Ref* sender ) {
            
            auto obj = CashManager::getInstance()->getProduct(E_CASH::CASH_EVENT_RACE_RESET);
            if ( obj == nullptr )
            {
                return;
            }
            
            std::string price = obj->getPrice();
            if ( price.length() != 0 )
            {
                ((ui::CText*)sender)->setString(price);
                ((ui::CText*)sender)->stopAllActions();
                return;
            }
        });
        auto action2 = DelayTime::create(5.0);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_shop_price_loading"), GAME_FONT, 28);
        uiText->setPosition(Vec2(_uiBtnResetPayment->getContentSize().width / 2, _uiBtnResetPayment->getContentSize().height / 2));
        uiText->setName("UI_TEXT");
        uiText->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
        _uiBtnResetPayment->addChild(uiText);
    }
    
    _uiBtnReset = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    _uiBtnReset->setScale9Enabled(true);
    _uiBtnReset->setContentSize(Size(210, 92));
    _uiBtnReset->setCapInsets(Rect(33,33,2,2));
    _uiBtnReset->addClickEventListener(CC_CALLBACK_1(PopupEventRaceReset::onClickReset, this));
    layout->addChild(_uiBtnReset);
    {
        auto uiText = ui::CText::create(" ", GAME_FONT, 28);
        uiText->setPosition(Vec2(_uiBtnReset->getContentSize().width / 2, _uiBtnReset->getContentSize().height / 2));
        uiText->setName("UI_TEXT");
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setLineSpacing(-10);
        _uiBtnReset->addChild(uiText);
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        _uiBtnReset->setLayoutParameter(params);
    }
}

#pragma mark - ui
void PopupEventRaceReset::drawInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    
    auto uiInfoRank = layout->getChildByName<ui::Layout*>("UI_INFO_RANK");
    if ( uiInfoRank != nullptr )
    {
        int rank = 0;
        auto objRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::EVENT_RACE);
        if ( objRank != nullptr )
        {
            rank = objRank->getRank();
        }
        
        if ( auto uiInfoBG = uiInfoRank->getChildByName<ui::ImageView*>("UI_INFO_BG"); uiInfoBG != nullptr )
        {
            uiInfoBG->removeAllChildren();
            
            auto uiRank = ui::CText::create(MafUtils::toString(rank), GAME_FONT, 24);
            uiRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiRank->setPosition(Vec2(30, uiInfoBG->getContentSize().height / 2));
            uiInfoBG->addChild(uiRank);
        }
    }
    
    auto uiInfoItem = layout->getChildByName<ui::Layout*>("UI_INFO_ITEM");
    if ( uiInfoItem != nullptr )
    {
        if ( auto uiInfoBG = uiInfoItem->getChildByName<ui::ImageView*>("UI_INFO_BG"); uiInfoBG != nullptr )
        {
            uiInfoBG->removeAllChildren();
            
            //
            std::vector<E_ITEMS> list;
            list.push_back(E_ITEMS::EVENT_RACE_TICKET);
            list.push_back(E_ITEMS::EVENT_RACE_COIN);
            
            auto uiCurrency = UICurrency::create(list, Size(uiInfoBG->getContentSize().width - 60, uiInfoBG->getContentSize().height));
            uiCurrency->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, uiInfoBG->getContentSize().height / 2));
            uiInfoBG->addChild(uiCurrency);
        }
    }
    
}

void PopupEventRaceReset::drawButtons()
{
    auto layout = _layerContainerButtons;
    
    //
    if ( auto uiText = _uiBtnResetPayment->getChildByName<ui::CText*>("UI_TEXT"); uiText != nullptr )
    {
        auto obj = CashManager::getInstance()->getProduct(E_CASH::CASH_EVENT_RACE_RESET);
        if ( obj != nullptr )
        {
            std::string price = obj->getPrice();
            if ( price.empty() == true )
            {
                price = GAME_TEXT("t_ui_shop_price_loading");
            }
            
            uiText->setString(price);
            uiText->stopAllActions();
        }
    }
    
    //
    if ( getResetCount() <= 0 )
    {
        _uiBtnReset->loadTextureNormal("Assets/ui_common/btn_1_off.png");
    }
    else
    {
        _uiBtnReset->loadTextureNormal("Assets/ui_common/btn_1_1_on.png");
    }
    
    if ( auto uiText = _uiBtnReset->getChildByName<ui::CText*>("UI_TEXT"); uiText != nullptr )
    {
        uiText->setString(GAME_TEXTFORMAT("t_ui_event_race_reset_bt_1", getResetCount(), getResetCountMax()));
        uiText->setTextAreaSize(Size(_uiBtnReset->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        
        
    }
    
    //
    Size size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->setPositionX(layout->getParent()->getContentSize().width / 2);
    layout->requestDoLayout();
}

#pragma mark - set, get, other

int PopupEventRaceReset::getResetCount()
{
    return EventRaceManager::getInstance()->getGameResetCount();
}
int PopupEventRaceReset::getResetCountMax()
{
    int count = 3;
    
    std::string json = EventManager::getInstance()->getEventOhterDataByEventIdx(33);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)json.c_str());
    if ( json.empty() == false && jsonParser.HasParseError() == false )
    {
        
        if ( jsonParser.HasMember("reset_count") == true )
        {
            count = jsonParser["reset_count"].GetInt();
        }
    }
    
    return count;
}

#pragma mark - callback
void PopupEventRaceReset::callbackReset(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    auto sceneLayer = UtilsScene::getInstance()->getRunningLayer();
    dynamic_cast<BaseGameScene *>(sceneLayer)->hidePopupAll();
    
    auto popup = PopupEventRaceTitle::create();
    popup->show();
}

void PopupEventRaceReset::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        PopupLoading::show();
        EventRaceManager::getInstance()->requestReset(CC_CALLBACK_2(PopupEventRaceReset::callbackReset, this), true);
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
         2 # 이벤트 중이 아닙니다. ( 복귀자 )
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
void PopupEventRaceReset::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventRaceReset::onClickResetPayment(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( EventManager::getInstance()->isActivatedEvent(33) == false )
    {
        hide();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_event_race_reset_title"), GAME_TEXT("t_ui_event_race_reset_check_msg_1"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
        
        /*
        0 : 오류 없음
        1 : 다른상품 결제 오류 있음.
        2 : 같은상품 결제 오류 있음.
        */
        E_CASH eCash = E_CASH::CASH_EVENT_RACE_RESET;
        CashManager::getInstance()->purchaseHas(eCash, [=](int nHas){
            if ( nHas == 2 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
                popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                    CashManager::getInstance()->purchaseConsumeAll();
                });
                popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                    PopupLoading::show();
                    CashManager::getInstance()->purchaseInitiate((int)eCash, CC_CALLBACK_4(PopupEventRaceReset::callbackPurchase, this));
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
                CashManager::getInstance()->purchaseInitiate((int)eCash, CC_CALLBACK_4(PopupEventRaceReset::callbackPurchase, this));
            }
        });
    });
    popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
    popup->show();
}

void PopupEventRaceReset::onClickReset(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( EventManager::getInstance()->isActivatedEvent(33) == false )
    {
        hide();
        return;
    }
    
    if ( getResetCount() <= 0 )
    {
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_event_race_reset_title"), GAME_TEXT("t_ui_event_race_reset_check_msg_2"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
        
        PopupLoading::show();
        EventRaceManager::getInstance()->requestReset(CC_CALLBACK_2(PopupEventRaceReset::callbackReset, this), false);
    });
    popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
    popup->show();

}
