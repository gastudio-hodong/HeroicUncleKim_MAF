//
//  PopupPurchaseReturn.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseReturn.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPurchaseReturn* PopupPurchaseReturn::create()
{
    PopupPurchaseReturn* pRet = new(std::nothrow) PopupPurchaseReturn();
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

PopupPurchaseReturn::PopupPurchaseReturn(void) :
_nItemIdx(0),

_layerContainerContents(nullptr),
_layerContainerContentsTop(nullptr),
_layerContainerContentsMiddle(nullptr)
{
    
}

PopupPurchaseReturn::~PopupPurchaseReturn(void)
{
    
}

bool PopupPurchaseReturn::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    return true;
}

void PopupPurchaseReturn::onEnter()
{
    PopupBase::onEnter();
    
    // init
    initVar();
    initUi();
    
    //
    PopupLoading::show();
    CashManager::getInstance()->requestPurchaseRemainingAll(CC_CALLBACK_1(PopupPurchaseReturn::callbackPurchaseRemaining, this));
}

#pragma mark - init
void PopupPurchaseReturn::initVar()
{
    
}

void PopupPurchaseReturn::initUi()
{
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(Size(750, 1176));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layerContainerContents->setPosition( getContentSize().width / 2, getContentSize().height / 2);
    addChild(_layerContainerContents);
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/shop_return1_1.png");
        spriteBG->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
        _layerContainerContents->addChild(spriteBG);
    }
    
    //
    _layerContainerContentsTop = Layer::create();
    _layerContainerContentsTop->setContentSize( Size(_layerContainerContents->getContentSize().width, 360) );
    _layerContainerContentsTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContentsTop->setPosition( _layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    _layerContainerContents->addChild(_layerContainerContentsTop);
    
    //
    _layerContainerContentsMiddle = Layer::create();
    _layerContainerContentsMiddle->setContentSize( Size(746, 748) );
    _layerContainerContentsMiddle->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsMiddle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContentsMiddle->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 360);
    _layerContainerContents->addChild(_layerContainerContentsMiddle);
     
    
    //
    uiContentsTop();
    uiContentsMiddle();
}

#pragma mark - ui
void PopupPurchaseReturn::uiContentsTop()
{
    //
    _layerContainerContentsTop->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContentsTop->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x_1.png"), nullptr, CC_CALLBACK_1(PopupPurchaseReturn::onClickClose, this));
    itemClose->setPosition(_layerContainerContentsTop->getContentSize().width - 10 - itemClose->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 10 - itemClose->getContentSize().height / 2);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    auto labelText1 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_1"), GAME_FONT, 70);
    labelText1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText1->setPosition(_layerContainerContentsTop->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 90);
    labelText1->setDimensions(_layerContainerContentsTop->getContentSize().width * 0.9, labelText1->getContentSize().height);
    labelText1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText1->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContentsTop->addChild(labelText1);
    
    auto labelText2 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_2"), GAME_FONT, 30);
    labelText2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText2->setPosition(_layerContainerContentsTop->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 170);
    labelText2->setColor(Color3B(255, 212, 0));
    labelText2->enableShadow(Color4B(166, 55, 0, 255));
    labelText2->setDimensions(_layerContainerContentsTop->getContentSize().width * 0.9, labelText2->getContentSize().height);
    labelText2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText2->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContentsTop->addChild(labelText2);
    
    
    auto labelInfo1 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_3"), GAME_FONT, 20);
    labelInfo1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelInfo1->setPosition(_layerContainerContentsTop->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 295);
    labelInfo1->setDimensions(_layerContainerContentsTop->getContentSize().width * 0.9, labelInfo1->getContentSize().height);
    labelInfo1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelInfo1->setOverflow(Label::Overflow::SHRINK);
    labelInfo1->setColor(Color3B(204, 204, 204));
    _layerContainerContentsTop->addChild(labelInfo1);
    

    auto objEvent = EventManager::getInstance()->getActivatedEvent(14);
    int64_t nTimeDiff = 0;
    if ( objEvent != nullptr )
    {
        nTimeDiff = objEvent->getEventEnd() - UtilsDate::getInstance()->getTime();
        if ( nTimeDiff <= 0 )
        {
            nTimeDiff = 0;
        }
    }
    int32_t h = (int)nTimeDiff / 3600;
    int32_t m = (int)nTimeDiff % 3600 / 60;
    int32_t s = (int)nTimeDiff % 3600 % 60;
    
    auto labelInfo2 = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_purchase_return_msg_4", h, m, s), GAME_FONT, 24);
    labelInfo2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelInfo2->setPosition(_layerContainerContentsTop->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 295);
    labelInfo2->setDimensions(_layerContainerContentsTop->getContentSize().width * 0.9, labelInfo2->getContentSize().height);
    labelInfo2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelInfo2->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContentsTop->addChild(labelInfo2);
    
    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
        
        auto objEvent = EventManager::getInstance()->getActivatedEvent(14);
        
        int64_t nTimeDiff = objEvent->getEventEnd() - UtilsDate::getInstance()->getTime();
        if ( nTimeDiff <= 0 )
        {
            nTimeDiff = 0;
            ((Label*)sender)->setString("00:00:00");
            ((Label*)sender)->stopAllActions();
            return;
        }
        
        int32_t h = (int)nTimeDiff / 3600;
        int32_t m = (int)nTimeDiff % 3600 / 60;
        int32_t s = (int)nTimeDiff % 3600 % 60;

        ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_purchase_return_msg_4", h, m, s));
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    labelInfo2->runAction( RepeatForever::create(seq) );
}

void PopupPurchaseReturn::uiContentsMiddle()
{
    _layerContainerContentsMiddle->removeAllChildren();
     
    // 왼쪽 상품
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/shop_return_box_1_1.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        spriteBG->setPosition(_layerContainerContentsMiddle->getContentSize().width / 2 - 2.5, _layerContainerContentsMiddle->getContentSize().height);
        _layerContainerContentsMiddle->addChild(spriteBG);
        
        auto labelTitle1 = Label::createWithTTF(GAME_TEXT("t_cash_title_373"), GAME_FONT, 28);
        labelTitle1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelTitle1->setPosition(20, spriteBG->getContentSize().height - 45);
        labelTitle1->setDimensions(230, labelTitle1->getContentSize().height);
        labelTitle1->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelTitle1->setOverflow(Label::Overflow::SHRINK);
        labelTitle1->setColor(Color3B(255, 229, 0));
        spriteBG->addChild(labelTitle1);
        
        auto labelTitle2 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_5"), GAME_FONT, 24);
        labelTitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelTitle2->setPosition(20, spriteBG->getContentSize().height - 45);
        labelTitle2->setDimensions(230, labelTitle1->getContentSize().height);
        labelTitle2->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelTitle2->setOverflow(Label::Overflow::SHRINK);
        labelTitle2->setColor(Color3B(226, 169, 0));
        spriteBG->addChild(labelTitle2);
        
        auto spriteRateBG = Sprite::create("Assets/ui/purchase/shop_value1_1.png");
        spriteRateBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        spriteRateBG->setPosition(spriteBG->getContentSize().width, spriteBG->getContentSize().height + 5);
        spriteBG->addChild(spriteRateBG);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_7"), GAME_FONT, 24);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            labelText->setPosition(spriteRateBG->getContentSize().width / 2, spriteRateBG->getContentSize().height - 5);
            labelText->setDimensions(spriteRateBG->getContentSize().width * 0.9, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setLineSpacing(-10);
            spriteRateBG->addChild(labelText);
        }
        
        {
            double nPosX = spriteBG->getContentSize().width / 2;
            double nPosY = 280;
            
            std::string listString[] = {GAME_TEXT("t_ui_purchase_return_msg_9_1"), GAME_TEXT("t_ui_purchase_return_msg_9_2"), GAME_TEXT("t_ui_purchase_return_msg_9_3")};
            for ( int i = 0; i < sizeof(listString) / sizeof(std::string); i++ )
            {
                auto labelText = Label::createWithTTF(listString[i], GAME_FONT, 30);
                labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                labelText->setPosition(nPosX, nPosY);
                labelText->setDimensions(spriteBG->getContentSize().width * 0.8, labelText->getContentSize().height);
                labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelText->setOverflow(Label::Overflow::SHRINK);
                spriteBG->addChild(labelText);
                
                nPosY -= (labelText->getContentSize().height * 0.8);
            }
        }
        
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteBG->addChild(menu);
        
        // item close
        auto spritePurchase = ui::Scale9Sprite::create("Assets/ui_common/btn_4_2.png", Rect::ZERO, Rect(40,48,10,2));
        spritePurchase->setContentSize(Size(294, 100));
        
        auto spritePurchaseDisabled = ui::Scale9Sprite::create("Assets/ui_common/btn_4_off.png", Rect::ZERO, Rect(40,48,10,2));
        spritePurchaseDisabled->setContentSize(Size(294, 100));
        
        auto itemPurchase = MafNode::MafMenuItemSprite::create(spritePurchase, nullptr, spritePurchaseDisabled, CC_CALLBACK_1(PopupPurchaseReturn::onClickPurchase, this));
        itemPurchase->setTag(E_CASH::CASH_RETURN_1);
        itemPurchase->setPosition(spriteBG->getContentSize().width / 2, itemPurchase->getContentSize().height / 2 + 10);
        menu->addChild(itemPurchase);
        
        auto labelBuy = Label::createWithSystemFont(GAME_TEXT("t_ui_shop_price_loading"), SYSTEM_FONT, 32);
        labelBuy->setPosition(itemPurchase->getContentSize().width / 2, itemPurchase->getContentSize().height / 2);
        labelBuy->setTextColor(Color4B(255, 255, 255, 255));
        labelBuy->enableOutline(Color4B(203, 110, 64, 255), 4);
        itemPurchase->addChild(labelBuy);
        
        int nPurchaseRemaining = CashManager::getInstance()->getPurchaseRemaining(E_CASH::CASH_RETURN_1);
        if ( nPurchaseRemaining <= 0 )
        {
            itemPurchase->setEnabled(false);
            labelBuy->setString(GAME_TEXT("t_ui_popshop_stepdone"));
        }
        else
        {
            auto objProduct = CashManager::getInstance()->getProduct(E_CASH::CASH_RETURN_1);
            if ( objProduct != nullptr )
            {
                std::string strPrice = objProduct->getPrice();
                if ( strPrice.length() != 0 )
                {
                    labelBuy->setString(strPrice);
                }
                else
                {
                    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
                        
                        auto objProduct = CashManager::getInstance()->getProduct(E_CASH::CASH_RETURN_1);
                        std::string strPrice = objProduct->getPrice();
                        if ( strPrice.length() != 0 )
                        {
                            ((Label*)sender)->setString(strPrice);
                            ((Label*)sender)->stopAllActions();
                            return;
                        }
                    });
                    auto action2 = DelayTime::create(5.0);
                    auto seq = Sequence::create(action1, action2, nullptr);
                    labelBuy->runAction( RepeatForever::create(seq) );
                }
            }
        }
    }
    
    // 오른쪽 상품
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/shop_return_box_1_2.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteBG->setPosition(_layerContainerContentsMiddle->getContentSize().width / 2 + 2.5, _layerContainerContentsMiddle->getContentSize().height);
        _layerContainerContentsMiddle->addChild(spriteBG);
        
        auto labelTitle1 = Label::createWithTTF(GAME_TEXT("t_cash_title_374"), GAME_FONT, 28);
        labelTitle1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelTitle1->setPosition(20, spriteBG->getContentSize().height - 45);
        labelTitle1->setDimensions(230, labelTitle1->getContentSize().height);
        labelTitle1->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelTitle1->setOverflow(Label::Overflow::SHRINK);
        labelTitle1->setColor(Color3B(255, 229, 0));
        spriteBG->addChild(labelTitle1);
        
        auto labelTitle2 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_6"), GAME_FONT, 24);
        labelTitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelTitle2->setPosition(20, spriteBG->getContentSize().height - 45);
        labelTitle2->setDimensions(230, labelTitle1->getContentSize().height);
        labelTitle2->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelTitle2->setOverflow(Label::Overflow::SHRINK);
        labelTitle2->setColor(Color3B(226, 169, 0));
        spriteBG->addChild(labelTitle2);
        
        auto spriteRateBG = Sprite::create("Assets/ui/purchase/shop_value1_1.png");
        spriteRateBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        spriteRateBG->setPosition(spriteBG->getContentSize().width, spriteBG->getContentSize().height + 5);
        spriteBG->addChild(spriteRateBG);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_8"), GAME_FONT, 24);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            labelText->setPosition(spriteRateBG->getContentSize().width / 2, spriteRateBG->getContentSize().height - 5);
            labelText->setDimensions(spriteRateBG->getContentSize().width * 0.9, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setLineSpacing(-10);
            spriteRateBG->addChild(labelText);
        }
        
        {
            double nPosX = spriteBG->getContentSize().width / 2;
            double nPosY = 315;
            
            std::string listString[] = {
                GAME_TEXT("t_ui_purchase_return_msg_10_1"), GAME_TEXT("t_ui_purchase_return_msg_10_2"), GAME_TEXT("t_ui_purchase_return_msg_10_3"),
                GAME_TEXT("t_ui_purchase_return_msg_10_5"), GAME_TEXT("t_ui_purchase_return_msg_10_4"), GAME_TEXT("t_ui_purchase_return_msg_10_6")
            };
            for ( int i = 0; i < sizeof(listString) / sizeof(std::string); i++ )
            {
                int nFontSize = 30;
                Color3B colorFont = Color3B::WHITE;
                if ( i == 3 || i == 5 )
                {
                    nFontSize = 20;
                    colorFont = Color3B(96, 214, 0);
                }
                
                auto labelText = Label::createWithTTF(listString[i], GAME_FONT, nFontSize);
                labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                labelText->setPosition(nPosX, nPosY);
                labelText->setDimensions(spriteBG->getContentSize().width * 0.75, labelText->getContentSize().height);
                labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelText->setOverflow(Label::Overflow::SHRINK);
                labelText->setColor(colorFont);
                spriteBG->addChild(labelText);
                
                
                nPosY -= (labelText->getContentSize().height * 0.8);
            }
        }
        
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteBG->addChild(menu);
        
        // item close
        auto spritePurchase = ui::Scale9Sprite::create("Assets/ui_common/btn_4_2.png", Rect::ZERO, Rect(40,48,10,2));
        spritePurchase->setContentSize(Size(294, 100));
        
        auto spritePurchaseDisabled = ui::Scale9Sprite::create("Assets/ui_common/btn_4_off.png", Rect::ZERO, Rect(40,48,10,2));
        spritePurchaseDisabled->setContentSize(Size(294, 100));
        
        auto itemPurchase = MafNode::MafMenuItemSprite::create(spritePurchase, nullptr, spritePurchaseDisabled, CC_CALLBACK_1(PopupPurchaseReturn::onClickPurchase, this));
        itemPurchase->setPosition(spriteBG->getContentSize().width / 2, itemPurchase->getContentSize().height / 2 + 10);
        itemPurchase->setTag(E_CASH::CASH_RETURN_2);
        menu->addChild(itemPurchase);
        
        auto labelBuy = Label::createWithSystemFont(GAME_TEXT("t_ui_shop_price_loading"), SYSTEM_FONT, 32);
        labelBuy->setPosition(itemPurchase->getContentSize().width / 2, itemPurchase->getContentSize().height / 2);
        labelBuy->setTextColor(Color4B(255, 255, 255, 255));
        labelBuy->enableOutline(Color4B(203, 110, 64, 255), 4);
        itemPurchase->addChild(labelBuy);
        
        int nPurchaseRemaining = CashManager::getInstance()->getPurchaseRemaining(E_CASH::CASH_RETURN_2);
        if ( nPurchaseRemaining <= 0 )
        {
            itemPurchase->setEnabled(false);
            labelBuy->setString(GAME_TEXT("t_ui_popshop_stepdone"));
        }
        else
        {
            auto objProduct = CashManager::getInstance()->getProduct(E_CASH::CASH_RETURN_2);
            if ( objProduct != nullptr )
            {
                std::string strPrice = objProduct->getPrice();
                if ( strPrice.length() != 0 )
                {
                    labelBuy->setString(strPrice);
                }
                else
                {
                    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
                        
                        auto objProduct = CashManager::getInstance()->getProduct(E_CASH::CASH_RETURN_2);
                        std::string strPrice = objProduct->getPrice();
                        if ( strPrice.length() != 0 )
                        {
                            ((Label*)sender)->setString(strPrice);
                            ((Label*)sender)->stopAllActions();
                            return;
                        }
                    });
                    auto action2 = DelayTime::create(5.0);
                    auto seq = Sequence::create(action1, action2, nullptr);
                    labelBuy->runAction( RepeatForever::create(seq) );
                }
            }
        }
        
        //
        auto spriteInfoBG = Sprite::create("Assets/ui/purchase/shop_return_box_1_3.png");
        spriteInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteInfoBG->setPosition(spriteBG->getContentSize().width / 2, -5);
        spriteBG->addChild(spriteInfoBG);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_11"), GAME_FONT, 24);
            labelText->setPosition(spriteInfoBG->getContentSize().width / 2, spriteInfoBG->getContentSize().height / 2);
            labelText->setDimensions(spriteInfoBG->getContentSize().width * 0.9, spriteInfoBG->getContentSize().height * 0.9);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            spriteInfoBG->addChild(labelText);
        }
    }
}
 

#pragma mark - set, get, sort, add


#pragma mark - click
void PopupPurchaseReturn::onClickClose(Ref* sender)
{
    hide();
}

void PopupPurchaseReturn::onClickPurchase(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
    auto item = (MafNode::MafMenuItem*)sender;
    

    _nItemIdx = item->getTag();
    auto objProduct = CashManager::getInstance()->getProduct(_nItemIdx);
    if ( objProduct == nullptr )
    {
        return;
    }
    
    PopupLoading::show();
    CashManager::getInstance()->requestPurchaseRemainingAll(CC_CALLBACK_1(PopupPurchaseReturn::callbackPurchaseRemainingInitiate, this));

}
   

#pragma mark - callback
void PopupPurchaseReturn::callbackPurchaseRemaining(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    uiContentsMiddle();
}

void PopupPurchaseReturn::callbackPurchaseRemainingInitiate(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    uiContentsMiddle();
    
    //
    int nPurchaseRemaining = CashManager::getInstance()->getPurchaseRemaining(_nItemIdx);
    if ( nPurchaseRemaining > 0 )
    {
        /*
        0 : 오류 없음
        1 : 다른상품 결제 오류 있음.
        2 : 같은상품 결제 오류 있음.
        */
        CashManager::getInstance()->purchaseHas(_nItemIdx, [=](int nHas){
            if ( nHas == 2 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
                popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                    CashManager::getInstance()->purchaseConsumeAll();
                });
                popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                    PopupLoading::show();
                    CashManager::getInstance()->purchaseInitiate(_nItemIdx, CC_CALLBACK_4(PopupPurchaseReturn::callbackPurchase, this));
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
                CashManager::getInstance()->purchaseInitiate(_nItemIdx, CC_CALLBACK_4(PopupPurchaseReturn::callbackPurchase, this));
            }
        });
    }
}

void PopupPurchaseReturn::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        // 상점 정보 다시 가져오기
        PopupLoading::show();
        CashManager::getInstance()->requestDailyInfo(CC_CALLBACK_1(PopupPurchaseReturn::callbackCashPension, this));
        
        
        // ui 새로고침
        uiContentsMiddle();
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

void PopupPurchaseReturn::callbackCashPension(bool bResult)
{
    //
    PopupLoading::hide();
    
    PopupLoading::show();
    CashManager::getInstance()->requestPurchaseRemainingAll(CC_CALLBACK_1(PopupPurchaseReturn::callbackPurchaseRemaining, this));
}
