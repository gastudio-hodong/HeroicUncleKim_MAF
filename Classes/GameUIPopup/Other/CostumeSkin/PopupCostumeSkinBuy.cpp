//
//  PopupCostumeSkinBuy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupCostumeSkinBuy.h"

#include "Common/ConfigGameNormal.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUI/UI/UICurrency.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

PopupCostumeSkinBuy* PopupCostumeSkinBuy::create()
{
    PopupCostumeSkinBuy* pRet = new(std::nothrow) PopupCostumeSkinBuy();
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

PopupCostumeSkinBuy::PopupCostumeSkinBuy() :
_skinShopIdx(0),
_objSkinShop(nullptr),
_objSkin(nullptr),

// widget
_uiContainerInfo(nullptr),
_uiContainerButtons(nullptr)
{
    
}

PopupCostumeSkinBuy::~PopupCostumeSkinBuy()
{
    
}

void PopupCostumeSkinBuy::onEnter()
{
    PopupBase::onEnter();
     
    //
    initVar();
    initUI();
}

#pragma mark - init
void PopupCostumeSkinBuy::initVar()
{
    _objSkinShop = CostumeSkinManager::getInstance()->getInfoSkinShop(_skinShopIdx);
    _objSkin = CostumeSkinManager::getInstance()->getInfoSkin(_objSkinShop->getSkinIdx());
}

void PopupCostumeSkinBuy::initUI()
{
    // size
    Size size = Size(536, 762);
    
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(_objSkin->getName(), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    labelTitle->setDimensions(400, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCostumeSkinBuy::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiContainer();
    uiInfo();
    uiButtons();
}

#pragma mark - ui
void PopupCostumeSkinBuy::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.height -= 75;
    
    _uiContainerButtons = ui::Layout::create();
    _uiContainerButtons->setContentSize( Size(size.width, 118) );
    _uiContainerButtons->setIgnoreAnchorPointForPosition(false);
    _uiContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    _spriteContainer->addChild(_uiContainerButtons);
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, size.height - _uiContainerButtons->getContentSize().height) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    _spriteContainer->addChild(_uiContainerInfo);
}

void PopupCostumeSkinBuy::uiInfo()
{
    _uiContainerInfo->removeAllChildren();
    
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/costume/paid_skin_01.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiBG);
    
    auto uiLight = Sprite::create("Assets/ui_common/effect_central1_1.png");
    uiLight->setPosition(Vec2(size.width / 2, size.height - 206));
    uiLight->runAction(RepeatForever::create(RotateBy::create(2.0f, 40.0f)));
    layout->addChild(uiLight);
    
    auto unitHero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
    unitHero->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    unitHero->setPosition(Vec2(size.width / 2 + 135, size.height - 206 + 40));
    unitHero->setSkin(_objSkin->getIdx());
    unitHero->actionMove();
    unitHero->getLayerCharacter()->setScale(2);
    layout->addChild(unitHero);
    
    // label
    if ( _objSkinShop->getLabel() != 0 )
    {
        /*
         0 : 없음
         1 : 패키지
         2 : 할인
         3 : 이벤트
         4 : 기간 한정판매
         */
        std::string pathLabel = "Assets/ui/costume/costume_label_event.png";
        switch (_objSkinShop->getLabel()) {
            case 1: pathLabel = "Assets/ui/costume/costume_label_package.png"; break;
            case 2: pathLabel = "Assets/ui/costume/costume_label_sale.png"; break;
            case 3: pathLabel = "Assets/ui/costume/costume_label_event.png"; break;
            case 4: pathLabel = "Assets/ui/costume/costume_label_limited.png"; break;
                
            default:
                break;
        }
        
        auto uiLabel = ui::ImageView::create(pathLabel);
        uiLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiLabel->setPosition(Vec2(size.width, size.height));
        uiLabel->setScale(3);
        ((ui::Scale9Sprite*)uiLabel->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        layout->addChild(uiLabel);
    }
    
    // currency
    if ( _objSkinShop->getBuyType().compare("EVENT") == 0 || _objSkinShop->getBuyType().compare("CASH_EVENT") == 0 )
    {
        auto objItem = ItemsManager::getInstance()->getInfoItems(_objSkinShop->getBuyItemIdx());
        if ( objItem != nullptr )
        {
            std::vector<E_ITEMS> list;
            list.push_back(enumFromInt<E_ITEMS>(objItem->getIdx()));
            
            auto uiCurrency = UICurrency::create(list, Size(160, 40));
            uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiCurrency->setPosition(Vec2(10, size.height - 30));
            layout->addChild(uiCurrency);
            {
                auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
                uiBG->setScale9Enabled(true);
                uiBG->setContentSize(Size(uiCurrency->getContentSize().width, uiCurrency->getContentSize().height));
                uiBG->setPosition(Vec2(uiCurrency->getContentSize().width / 2, uiCurrency->getContentSize().height / 2));
                uiCurrency->addChild(uiBG, -1);
            }
            
        }
    }
        
            
            
    
    // item
    auto uiReward = ui::Layout::create();
    uiReward->setContentSize(Size(size.height * 0.9, 66));
    uiReward->setIgnoreAnchorPointForPosition(false);
    uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiReward->setPosition(Vec2(size.width / 2, size.height - 350));
    layout->addChild(uiReward);
    uiPurchase(uiReward);
    
    // name
    auto uiNameBG = ui::ImageView::create("Assets/ui/costume/paid_skin_02.png");
    uiNameBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiNameBG->setPosition(Vec2(size.width / 2, size.height - 400));
    layout->addChild(uiNameBG);
    {
        auto uiName = ui::CText::create(_objSkin->getName(), GAME_FONT, 24);
        uiName->setPosition(Vec2(uiNameBG->getContentSize().width / 2, uiNameBG->getContentSize().height / 2));
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        uiName->setTextAreaSize(Size(uiNameBG->getContentSize().width - 10, uiName->getContentSize().height));
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiName->enableShadow();
        uiNameBG->addChild(uiName);
    }
    
    // info
    auto uiInfo = ui::CText::create(_objSkinShop->getComment(), GAME_FONT, 24);
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiInfo->setPosition(Vec2(size.width / 2, 0));
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setTextAreaSize(Size(size.width - 10, size.height - 412));
    uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiInfo);
    
}

void PopupCostumeSkinBuy::uiButtons()
{
    _uiContainerButtons->removeAllChildren();
    
    auto layout = _uiContainerButtons;
    auto size = layout->getContentSize();
    
    if ( _objSkinShop->getBuyType().compare("CASH") == 0 || _objSkinShop->getBuyType().compare("CASH_EVENT") == 0 )
    {
        auto uiBtnBuyCash = ui::Button::create("Assets/ui_common/ui_pet_tempered_btn.png");
        uiBtnBuyCash->addClickEventListener(CC_CALLBACK_1(PopupCostumeSkinBuy::onClickBuyCash, this));
        layout->addChild(uiBtnBuyCash);
        
        auto objProduct = CashManager::getInstance()->getProduct(_objSkinShop->getBuyCashIdx());
        if ( objProduct != nullptr )
        {
            auto uiPrice = ui::CText::create(objProduct->getPrice(), GAME_FONT, 28);
            uiPrice->setPosition(Vec2(uiBtnBuyCash->getContentSize().width / 2, uiBtnBuyCash->getContentSize().height / 2));
            uiPrice->enableShadow();
            uiBtnBuyCash->addChild(uiPrice);
        }
    }
    
    if ( _objSkinShop->getBuyType().compare("EVENT") == 0 || _objSkinShop->getBuyType().compare("CASH_EVENT") == 0 )
    {
        auto uiBtnBuyItem = ui::Button::create("Assets/ui_common/ui_pet_tempered_btn.png");
        uiBtnBuyItem->addClickEventListener(CC_CALLBACK_1(PopupCostumeSkinBuy::onClickBuyItem, this));
        layout->addChild(uiBtnBuyItem);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(_objSkinShop->getBuyItemIdx());
        if ( objItem != nullptr )
        {
            objItem->setTypeParams(_objSkinShop->getBuyItemParams());
            objItem->setCount(_objSkinShop->getBuyItemCount());
            
            
            auto uiTest = UtilsWidget::getInstance()->createIconText(objItem->getPath(), objItem->getCount(), uiBtnBuyItem->getContentSize().width - 10, 28, 0);
            uiTest->setPosition(Vec2(uiBtnBuyItem->getContentSize().width / 2, uiBtnBuyItem->getContentSize().height / 2));
            uiBtnBuyItem->addChild(uiTest);
        }
    }
    
    
    // align
    double padding = 10;
    double widthButtons = -padding;
    for ( auto& uiBtn : layout->getChildren() )
    {
        widthButtons += uiBtn->getContentSize().width * uiBtn->getScaleX() + padding;
    }
    
    float posX = (size.width / 2.0f) - (widthButtons / 2.0f);
    float posY = size.height / 2.0f;
    for (const auto &child : layout->getChildren())
    {
        child->setPosition(posX + child->getContentSize().width * child->getScaleX() / 2.0f, posY);
        posX += child->getContentSize().width * child->getScaleX() + padding;
    }
    
}

void PopupCostumeSkinBuy::uiPurchase(cocos2d::ui::Layout* uiParent)
{
    auto objProduct = CashManager::getInstance()->getProduct(_objSkinShop->getBuyCashIdx());
    if ( objProduct == nullptr )
    {
        return;
    }
    
    //
    Vector<InfoItems*> listItemsAll;
    auto listItems = objProduct->getItems();
    auto listItemsDaily = objProduct->getItemsOther();
    
    listItemsAll.pushBack(listItems);
    listItemsAll.pushBack(listItemsDaily);
    
    int paddingX = 5;
    int paddingY = 5;
    
    int drawCol = (int)listItemsAll.size();
    int drawRow = 1;
    float nWidth = 63 * drawCol + paddingX * (drawCol - 1);
    float nHeight = 63 * drawRow + paddingY * (drawRow - 1);
    
    auto layerItems = Layer::create();
    layerItems->setContentSize(Size(nWidth, nHeight));
    layerItems->setIgnoreAnchorPointForPosition(false);
    layerItems->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerItems->setPosition(Vec2(uiParent->getContentSize().width / 2, uiParent->getContentSize().height / 2));
    uiParent->addChild(layerItems);
    for ( int i = 0; i < listItemsAll.size(); i++ )
    {
        auto obj = listItemsAll.at(i);
        
        int x = i % drawCol;
        int y = i / drawCol;
        double posX = x * 63 + x * paddingX;
        double posY = layerItems->getContentSize().height - y * 63 - y * paddingY;

        // item bg
        auto spriteBg = Sprite::create("Assets/ui/purchase/shop_item.png");
        spriteBg->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        spriteBg->setPosition(posX, posY);
        layerItems->addChild(spriteBg);
        
        // item image
        std::string strPathIcon = obj->getPath();
        if ( strPathIcon.length() != 0 )
        {
            auto spriteIcon = Sprite::create(strPathIcon);
            spriteIcon->setPosition(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2 + 8);
            spriteBg->addChild(spriteIcon);
        }
        
        // item daily
        if ( i >= listItems.size() )
        {
            auto spriteDaily = Sprite::create("Assets/ui/purchase/shop_daily.png");
            spriteDaily->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            spriteDaily->setPosition(spriteBg->getContentSize().width, spriteBg->getContentSize().height - 8);
            spriteBg->addChild(spriteDaily);
        }
        
        // item count
        std::string strCount = obj->getCount();
        if ( obj->getType().compare("BUFF") == 0 )
        {
            if ( obj->getTypeDetail().compare("REVIVAL") == 0 )
            {
                strCount = GAME_TEXTFORMAT("t_ui_hour", atoi(obj->getCount().c_str()));
            }
            else
            {
                strCount = GAME_TEXTFORMAT("t_ui_second", atoi(obj->getCount().c_str()));
            }
        }
        if ( obj->getType().compare("ADS") == 0 )
        {
            if ( obj->getIdx() == 48 )
            {
                strCount.append("H");
            }
            else if ( obj->getIdx() == 53 )
            {
                strCount.append("D");
            }
        }
        
        auto labelCount = Label::createWithTTF(strCount, GAME_FONT, 14);
        labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelCount->setPosition(spriteBg->getContentSize().width / 2, 4);
        spriteBg->addChild(labelCount);
    }
}
#pragma mark - draw

#pragma mark - ui : set, get
void PopupCostumeSkinBuy::setSkinShopIdx(int idx)
{
    _skinShopIdx = idx;
}

#pragma mark - callback
void PopupCostumeSkinBuy::callbackBuyPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_skin_msg_4"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        //
        PopupLoading::show();
        CostumeSkinManager::getInstance()->requestSkinIsBuy([=](bool bResult){
            PopupLoading::hide();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
        });
        
        hide();
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

void PopupCostumeSkinBuy::callbackBuyItem(bool bResult, int nResult, int nSkin)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )
        {
            strError = GAME_TEXT("t_ui_error_31");
        }
        else if ( nResult == 4 )
        {
            strError = GAME_TEXT("t_ui_event_halloween_msg_3");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_skin_msg_4"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    hide();
}

#pragma mark - click
void PopupCostumeSkinBuy::onClickClose(Ref* sender)
{
    hide();
}

void PopupCostumeSkinBuy::onClickBuyCash(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto objProduct = CashManager::getInstance()->getProduct(_objSkinShop->getBuyCashIdx());
    if ( objProduct == nullptr )
    {
        return;
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(objProduct->getIdx(), [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                
                auto objProduct = CashManager::getInstance()->getProduct(_objSkinShop->getBuyCashIdx());
                
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(objProduct->getIdx(), CC_CALLBACK_4(PopupCostumeSkinBuy::callbackBuyPurchase, this));
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
            CashManager::getInstance()->purchaseInitiate(objProduct->getIdx(), CC_CALLBACK_4(PopupCostumeSkinBuy::callbackBuyPurchase, this));
        }
    });
}

void PopupCostumeSkinBuy::onClickBuyItem(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto objItem = ItemsManager::getInstance()->getInfoItems(_objSkinShop->getBuyItemIdx());
    if ( objItem == nullptr )
    {
        return;
    }
    
    PopupLoading::show();
    CostumeSkinManager::getInstance()->requestSkinBuy(_objSkinShop->getIdx(), CC_CALLBACK_3(PopupCostumeSkinBuy::callbackBuyItem, this));
}


#pragma mark - game
