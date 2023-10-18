//
//  PopupTicket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupTicket.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

PopupTicket* PopupTicket::create()
{
    PopupTicket* pRet = new(std::nothrow) PopupTicket();
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

PopupTicket::PopupTicket(void) :
_nTicket(0),

_onCallbackUse(nullptr),
_onCallbackBuy(nullptr),

_layerContainerTop(nullptr),
_layerContainerList(nullptr)
{
    
}

PopupTicket::~PopupTicket(void)
{
    
}

bool PopupTicket::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }

    
    // init
    initVar();
    initUi();
    
    
    return true;
}

#pragma mark - init
void PopupTicket::initVar()
{
    _nTicket = PrisonManager::getInstance()->getTicketCount();
}

void PopupTicket::initUi()
{
    Size size = Size(556, 974);
    
    //
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_pticket"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupTicket::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerTop = LayerColor::create();
    _layerContainerTop->setContentSize( Size(size.width - 50, 440) );
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTop->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_layerContainerTop);
    
    //
    _layerContainerList = LayerColor::create();
    _layerContainerList->setContentSize( Size(size.width - 50, 435) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerList);
    
    
    //
    uiTop();
    uiList();
    
    // draw
    drawInfo();
}


#pragma mark - ui
void PopupTicket::uiTop()
{
    _layerContainerTop->removeAllChildren();
    
    _uiTextPTicket = ui::CText::create(GAME_TEXTFORMAT("t_ui_prison_msg_27", _nTicket), GAME_FONT, 33);
    _uiTextPTicket->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiTextPTicket->setPosition(Vec2(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height - 30));
    _layerContainerTop->addChild(_uiTextPTicket);
    
    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_prison_msg_28"), GAME_FONT, 28);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height - 100);
    labelText->setColor(COLOR_KEY);
    labelText->setLineSpacing(-5);
    labelText->setDimensions(_layerContainerTop->getContentSize().width, 90);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerTop->addChild(labelText);
    
    //
    _uiRewardContainer = ui::ImageView::create("Assets/ui/prison/p_ticket_buff_box.png");
    _uiRewardContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiRewardContainer->setPosition(Vec2(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height - 200));
    _layerContainerTop->addChild(_uiRewardContainer);
    {
        //
        auto uiRewardPos = ui::ImageView::create("Assets/ui/prison/p_ticket_buff_select.png");
        uiRewardPos->setPosition(Vec2(120, _uiRewardContainer->getContentSize().height / 2));
        uiRewardPos->setTag(100);
        uiRewardPos->setVisible(false);
        _uiRewardContainer->addChild(uiRewardPos);
        
        //
        auto uiIconTicket = ui::ImageView::create("Assets/icon/icon_p_ticket.png");
        uiIconTicket->setPosition(Vec2(45, _uiRewardContainer->getContentSize().height / 2 + 27));
        _uiRewardContainer->addChild(uiIconTicket);
        
        auto uiIconDevilCoin = ui::ImageView::create("Assets/icon/icon_coin_2.png");
        uiIconDevilCoin->setPosition(Vec2(45, _uiRewardContainer->getContentSize().height / 2 - 23));
        _uiRewardContainer->addChild(uiIconDevilCoin);
        
        //
        // {"use_max":5,"use_ratio":[2,3,4,5,7]}
        std::string strText = ParameterManager::getInstance()->getParameterString("p_ticket_value");
        
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strText.c_str());
        if ( strText.length() != 0 && jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser["use_ratio"].Size(); i++ )
            {
                double ratio = jsonParser["use_ratio"][i].GetDouble();
                
                Vec2 pos = Vec2(120 + i * 62, _uiRewardContainer->getContentSize().height / 2);
                
                auto uiTextIdx = ui::CText::create(MafUtils::format("%d", i+1), GAME_FONT, 24);
                uiTextIdx->setPosition(Vec2(pos.x, pos.y + 27));
                _uiRewardContainer->addChild(uiTextIdx);
                
                auto uiTextRatio = ui::CText::create(MafUtils::format("x%.0f", ratio), GAME_FONT, 24);
                uiTextRatio->setPosition(Vec2(pos.x, pos.y - 23));
                _uiRewardContainer->addChild(uiTextRatio);
            }
        }
    }
    
    //
    auto uiBtnUse = ui::Button::create("Assets/ui_common/btn_purple.png");
    uiBtnUse->setScale9Enabled(true);
    uiBtnUse->setContentSize(Size(210, 96));
    uiBtnUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnUse->setPosition(Vec2(_layerContainerTop->getContentSize().width / 2, 25));
    uiBtnUse->addClickEventListener(CC_CALLBACK_1(PopupTicket::onClickUse, this));
    _layerContainerTop->addChild(uiBtnUse);
    {
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_p_ticket.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiIcon->setPosition(Vec2(85, uiBtnUse->getContentSize().height / 2));
        uiBtnUse->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_use"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(100, uiBtnUse->getContentSize().height / 2) );
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextAreaSize(Size(90, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBtnUse->addChild(uiText);
    }
}

void PopupTicket::uiList()
{
    _layerContainerList->removeAllChildren();
    
    int nSellIdx[] = {E_CASH::CASH_PTICKET_1, E_CASH::CASH_PTICKET_2, E_CASH::CASH_PTICKET_3, E_CASH::CASH_PTICKET_4};
    int nSellCount[] = {1, 3, 10, 30};
    int nSellBonusCount[] = {0, 1, 2, 13};
    for ( int i = 0; i < 4; i++ )
    {
        auto objProduct = CashManager::getInstance()->getProduct(nSellIdx[i]);
        if ( objProduct == nullptr )
        {
            continue;
        }
        
        auto spriteBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
        spriteBG->setScale9Enabled(true);
        spriteBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        spriteBG->setContentSize(Size(506, 100));
        spriteBG->setPosition(Vec2(_layerContainerList->getContentSize().width / 2, _layerContainerList->getContentSize().height - i * spriteBG->getContentSize().height - i * 10));
        _layerContainerList->addChild(spriteBG);
        
        auto spriteSell = Sprite::create("Assets/ui_common/package_ticket.png");
        spriteSell->setPosition( Vec2(55, spriteBG->getContentSize().height / 2) );
        spriteBG->addChild(spriteSell);
        {
            auto labelText = Label::createWithTTF(MafUtils::toString(nSellCount[i]), GAME_FONT, 19);
            labelText->setPosition(spriteSell->getContentSize().width / 2, 5);
            labelText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
            labelText->enableBold();
            spriteSell->addChild(labelText);
        }
        
        if ( nSellBonusCount[i] > 0 )
        {
            auto spritePlus = Sprite::create("Assets/ui/prison/costume_icon_puls.png");
            spritePlus->setPosition(spriteSell->getPositionX() + spriteSell->getContentSize().width / 2 + 40, spriteBG->getContentSize().height / 2);
            spriteBG->addChild(spritePlus);
            
            auto spriteSellBonus = Sprite::create("Assets/ui_common/package_ticket.png");
            spriteSellBonus->setPosition(spritePlus->getPositionX() + 40 + spriteSellBonus->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
            spriteBG->addChild(spriteSellBonus);
            {
                auto labelText = Label::createWithTTF(MafUtils::toString(nSellBonusCount[i]), GAME_FONT, 19);
                labelText->setPosition( Vec2(spriteSellBonus->getContentSize().width / 2, 5) );
                labelText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
                labelText->enableBold();
                spriteSellBonus->addChild(labelText);
            }
        }
        
        auto menuCell = MafNode::MafMenu::create();
        menuCell->setPosition(Vec2::ZERO);
        menuCell->setAnchorPoint(Vec2::ZERO);
        spriteBG->addChild(menuCell);
        
        auto itemUpgrade = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/prison/btn_p_ticket_buy.png"), Sprite::create("Assets/ui/prison/btn_p_ticket_buy.png"), nullptr, CC_CALLBACK_1(PopupTicket::onClickBuy, this));
        itemUpgrade->setPosition(spriteBG->getContentSize().width - itemUpgrade->getContentSize().width / 2 - 10, spriteBG->getContentSize().height / 2);
        itemUpgrade->setTag(nSellIdx[i]);
        menuCell->addChild(itemUpgrade);
        {
            auto labelPrice = Label::createWithSystemFont(objProduct->getPrice(), SYSTEM_FONT, 28);
            labelPrice->setPosition(itemUpgrade->getContentSize().width / 2, itemUpgrade->getContentSize().height / 2);
            itemUpgrade->addChild(labelPrice);
            
            std::string strPrice = objProduct->getPrice();
            if ( strPrice.length() == 0 )
            {
                labelPrice->setString(GAME_TEXT("t_ui_shop_price_loading"));
                
                auto action1 = CallFuncN::create( [=]( Ref* sender ) {
                    
                    auto obj = CashManager::getInstance()->getProduct(objProduct->getIdx());
                    
                    std::string strPrice = obj->getPrice();
                    if ( strPrice.length() != 0 )
                    {
                        ((Label*)sender)->setString(strPrice);
                        ((Label*)sender)->stopAllActions();
                        return;
                    }
                });
                auto action2 = DelayTime::create(5.0);
                auto seq = Sequence::create(action1, action2, nullptr);
                labelPrice->runAction( RepeatForever::create(seq) );
            }
        }
    }
}

#pragma mark - draw
void PopupTicket::drawInfo()
{
    //
    _uiTextPTicket->setString(GAME_TEXTFORMAT("t_ui_prison_msg_27", _nTicket));
    
    //
    auto uiRewardPos = _uiRewardContainer->getChildByTag<ui::ImageView*>(100);
    if ( uiRewardPos != nullptr )
    {
        int idx = PrisonManager::getInstance()->getTicketUseIdx();
        
        if ( idx == -1 )
        {
            uiRewardPos->setVisible(false);
        }
        else
        {
            Vec2 pos = Vec2(120 + idx * 62, uiRewardPos->getPositionY());
            if ( uiRewardPos->isVisible() == false )
            {
                uiRewardPos->setPosition(pos);
            }
            else
            {
                uiRewardPos->runAction(MoveTo::create(0.5, pos));
            }
            uiRewardPos->setVisible(true);
        }
    }
}

#pragma mark - set, get, other
void PopupTicket::setCallbackUse(const std::function<void(void)>& callback)
{
    _onCallbackUse = callback;
}

void PopupTicket::setCallbackBuy(const std::function<void(void)>& callback)
{
    _onCallbackBuy = callback;
}

#pragma mark - click
void PopupTicket::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupTicket::onClickUse(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    double ticketUseMax = PrisonManager::getInstance()->getTicketUseMax();
    double ticketUseRatio = PrisonManager::getInstance()->getTicketUseRatio();
    if ( _nTicket <= 0 )
    {
        PopupToast::show(GAME_TEXT("t_ui_prison_msg_45"));
        return;
    }
        
    if ( ticketUseRatio >= ticketUseMax )
    {
        PopupToast::show(GAME_TEXT("t_ui_prison_msg_46"));
        return;
    }
    
    //
    if ( PrisonManager::getInstance()->getResetTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_reset"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){

            PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::getInstance()->getPrisonType(), nullptr);
            
        });
        popup->setHideBackKey(false);
        popup->show();
        
        hide();
        return;
    }
    
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_msg_p_ticket"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(PopupTicket::onClickUseOk, this));
    popup->show();
}

void PopupTicket::onClickUseOk(cocos2d::Ref* sender)
{
    //
    PopupLoading::show();
    PrisonManager::getInstance()->requestTicketUse(CC_CALLBACK_2(PopupTicket::callbackTicketUse, this));
}

void PopupTicket::onClickBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    
    
    auto item = (MafNode::MafMenuItem*)sender;
    
    
    
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    int nItemIdx = item->getTag();
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupTicket::callbackInAppPurchaseTicket, this));
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
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupTicket::callbackInAppPurchaseTicket, this));
        }
    });
}

#pragma mark - callback
void PopupTicket::callbackInAppPurchaseTicket(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        // redraw
        if ( _onCallbackBuy != nullptr )
        {
            _onCallbackBuy();
        }
        
        //
        _nTicket = PrisonManager::getInstance()->getTicketCount();
        
        //
        drawInfo();
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

void PopupTicket::callbackTicketUse(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        //# 2 : 티켓 부족, 3 : 최대 사용
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_prison_msg_45");
        else if ( nResult == 3 )
            strMessage = GAME_TEXT("t_ui_prison_msg_46");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    else if ( nResult == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_prison_msg_44", PrisonManager::getInstance()->getTicketUseRatio()));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( _onCallbackUse != nullptr )
    {
        _onCallbackUse();
    }
    
    //
    _nTicket = PrisonManager::getInstance()->getTicketCount();
    
    //
    drawInfo();
}
