//
//  PopupPurchaseBuy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseBuy.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPurchaseBuy* PopupPurchaseBuy::create(int nIdx)
{
    PopupPurchaseBuy* pRet = new(std::nothrow) PopupPurchaseBuy();
    if ( pRet && pRet->init(nIdx) )
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

PopupPurchaseBuy::PopupPurchaseBuy(void) :
_objProduct(nullptr),
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupPurchaseBuy::~PopupPurchaseBuy(void)
{
    if ( _objProduct != nullptr )
    {
        _objProduct->release();
    }
}

bool PopupPurchaseBuy::init(int nIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
     
    //
    _objProduct = CashManager::getInstance()->getProduct(nIdx);
    _objProduct->retain();
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupPurchaseBuy::initVar()
{

}

void PopupPurchaseBuy::initUi()
{
    Size size = Size(500, 450);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(_objProduct->getName(), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupPurchaseBuy::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
 
    // item
    Vector<InfoItems*> listItemsAll;
    auto listItems = _objProduct->getItems();
    auto listItemsDaily = _objProduct->getItemsOther();
    
    listItemsAll.pushBack(listItems);
    listItemsAll.pushBack(listItemsDaily);
    
    int paddingX = 5;
    int paddingY = 5;
    
    int drawCol = (int)listItemsAll.size();
    int drawRow = 1;
    float nWidth = 94.5 * drawCol + paddingX * (drawCol - 1);
    float nHeight = 94.5 * drawRow + paddingY * (drawRow - 1);
    
    auto layerItems = Layer::create();
    layerItems->setContentSize(Size(nWidth, nHeight));
    layerItems->setIgnoreAnchorPointForPosition(false);
    layerItems->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerItems->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2 + 35);
    _layerContainerContents->addChild(layerItems);
    for ( int i = 0; i < listItemsAll.size(); i++ )
    {
        auto obj = listItemsAll.at(i);
        
        int x = i % drawCol;
        int y = i / drawCol;
        double posX = x * 94.5 + x * paddingX;
        double posY = layerItems->getContentSize().height - y * 63 - y * paddingY;

        // item bg
        auto spriteBg = Sprite::create("Assets/ui/purchase/shop_item.png");
        spriteBg->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        spriteBg->setPosition(posX, posY);
        spriteBg->setScale(1.5);
        layerItems->addChild(spriteBg);
        
        // item image
        std::string strPathIcon = obj->getPath();
        if ( strPathIcon.length() != 0 )
        {
            auto spriteIcon = Sprite::create(strPathIcon);
            spriteIcon->setPosition(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2 + 8);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteBg->addChild(spriteIcon);
        }
        
        // item daily
        if ( i >= listItems.size() )
        {
            auto spriteDaily = Sprite::create("Assets/ui/purchase/shop_daily.png");
            spriteDaily->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            spriteDaily->setPosition(spriteBg->getContentSize().width, spriteBg->getContentSize().height - 8);
            spriteDaily->getTexture()->setTexParameters(texParams);
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
   
    // 설명
    std::string strContents = "";
    if ( _objProduct->getSaleLimit() > 0 )
    {
        strContents = GAME_TEXTFORMAT("t_ui_shop_limit", _objProduct->getSaleLimit());
    }
    else if ( _objProduct->getShopType() == E_PRODUCT::DAILY )
    {
        strContents = GAME_TEXT("t_ui_shop_daily_info");
    }
    
    if ( strContents.length() != 0 )
    {
        strContents.append("\n");
    }
    strContents.append(GAME_TEXT("t_ui_cash_msg_10"));
    
    if ( strContents.length() != 0 )
    {
        auto label = Label::createWithTTF(strContents, GAME_FONT, 18);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        label->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
        label->setDimensions(_layerContainerContents->getContentSize().width * 0.95, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(label);
    }
}

void PopupPurchaseBuy::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    

    auto spriteClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(33,33,2,2));
    spriteClose->setContentSize(Size(158, 92));
    
    auto itemClose = MafNode::MafMenuItemSprite::create(spriteClose, nullptr, CC_CALLBACK_1(PopupPurchaseBuy::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 24);
        labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(labelText);
    }
    
    auto spriteBuy = ui::Scale9Sprite::create("Assets/ui_common/btn_1_4_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteBuy->setContentSize(Size(158, 92));
    
    auto itemBuy = MafNode::MafMenuItemSprite::create(spriteBuy, nullptr, CC_CALLBACK_1(PopupPurchaseBuy::onClickBuy, this));
    menu->addChild(itemBuy);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_buy"), GAME_FONT, 24);
        labelText->setPosition(itemBuy->getContentSize().width / 2, itemBuy->getContentSize().height / 2);
        itemBuy->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}


#pragma mark - set, get, sort, add
void PopupPurchaseBuy::setCallbackResult(const std::function<void(int)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupPurchaseBuy::onClickClose(Ref* sender)
{
    hide();
}

void PopupPurchaseBuy::onClickBuy(cocos2d::Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult(_objProduct->getIdx());
    }
    
    hide();
}
