//
//  PopupBonsikSoulChange.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupBonsikSoulChange.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/BonsikDex/PopupBonsikSoulChange.h"
#include "GameUIPopup/Other/Story/PopupStory.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupBonsikSoulChange* PopupBonsikSoulChange::create()
{
    PopupBonsikSoulChange* pRet = new(std::nothrow) PopupBonsikSoulChange();
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

PopupBonsikSoulChange::PopupBonsikSoulChange(void) :
_onCallbackResult(nullptr),

_layerContainerContents(nullptr)
{
    
}

PopupBonsikSoulChange::~PopupBonsikSoulChange(void)
{
    
}

bool PopupBonsikSoulChange::init()
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
void PopupBonsikSoulChange::initVar()
{
    E_ITEMS listTradeForType[] = {E_ITEMS::MONSTER_SOUL_PIECE, E_ITEMS::MONSTER_SOUL_PIECE};
    int listTradeFor[] = {10, 100};
    
    E_ITEMS listTradeToType[] = {E_ITEMS::MONSTER_SOUL, E_ITEMS::MONSTER_SOUL};
    int listTradeTo[] = {1, 10};
    
    int listTradeLimit[] = {0, 0};
    
    for ( int i = 0; i < sizeof(listTradeForType)/sizeof(E_ITEMS); i++ )
    {
        auto obj = InfoTradeItems::create();
        obj->setIdx(i);
        
        obj->setTradeForType(listTradeForType[i]);
        obj->setTradeForCount(listTradeFor[i]);
        
        obj->setTradeToType(listTradeToType[i]);
        obj->setTradeToCount(listTradeTo[i]);
        
        obj->setTradeLimit(listTradeLimit[i]);
        
        //
        _listTrade.pushBack(obj);
    }
}

void PopupBonsikSoulChange::initUi()
{
    Size size = Size(750, 419);

    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - 40) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_bonsikdex_msg_2"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBonsikSoulChange::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::MONSTER_SOUL_PIECE);
    
    auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25));
    _spriteContainer->addChild(uiCurrency, E_ZORER::UI);
    
    //
    uiContents();
}


#pragma mark - ui
void PopupBonsikSoulChange::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    for ( int i = 0; i < _listTrade.size(); i++ )
    {
        double paddingY = 10;
        double posX = _layerContainerContents->getContentSize().width / 2;
        double posY = _layerContainerContents->getContentSize().height - (i * 118 + i * paddingY);

        //
        auto obj = _listTrade.at(i);

        //
        auto spriteBG = Sprite::create("Assets/ui/prison/beelzebub/box_beelzebub_list1.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBG->setPosition(posX, posY);
        _layerContainerContents->addChild(spriteBG);

        auto spriteDirectionBG = Sprite::create("Assets/ui/prison/beelzebub/box_beelzebub_direction.png");
        spriteDirectionBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteDirectionBG->setPosition(0, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteDirectionBG);
        
        //
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        spriteBG->addChild(menu);

        //
        {
            auto spriteIconBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
            spriteIconBG->setContentSize(Size(82, 82));
            spriteIconBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            spriteIconBG->setPosition(10, spriteBG->getContentSize().height / 2);
            spriteBG->addChild(spriteIconBG);
            {
                auto objItems = ItemsManager::getInstance()->getInfoItems(obj->getTradeForType());
                if ( objItems != nullptr )
                {
                    auto spriteIcon = Sprite::create(objItems->getPath());
                    spriteIcon->setPosition(spriteIconBG->getContentSize().width / 2, spriteIconBG->getContentSize().height / 2);
                    spriteIcon->getTexture()->setTexParameters(texParams);
                    spriteIcon->setScale(spriteIconBG->getContentSize().width / spriteIcon->getContentSize().width);
                    spriteIconBG->addChild(spriteIcon);
                }
            }
        }
        
        
        //
        {
            auto spriteIconBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
            spriteIconBG->setContentSize(Size(82, 82));

            auto itemIconBG = MafNode::MafMenuItemSprite::create(spriteIconBG, nullptr, [=](cocos2d::Ref* sender){
                auto item = (MafNode::MafMenuItemSprite*)sender;

                auto objItems = ItemsManager::getInstance()->getInfoItems(obj->getTradeToType());
                if ( objItems == nullptr )
                {
                    return;
                }
                
                if ( objItems->getText().length() == 0 )
                {
                    return;
                }
                
                Vec2 posToast = item->getParent()->convertToWorldSpace(item->getPosition());
                posToast.y += item->getContentSize().height / 2;
                posToast.y += 10;

                PopupToast::showImmediately(objItems->getText(), posToast.x, posToast.y);
            });
            itemIconBG->setPosition(spriteBG->getContentSize().width - 250, spriteBG->getContentSize().height / 2 + 10);
            menu->addChild(itemIconBG);
            {
                auto objItems = ItemsManager::getInstance()->getInfoItems(obj->getTradeToType());
                if ( objItems != nullptr && objItems->getPath().length() != 0 )
                {
                    auto spriteIcon = Sprite::create(objItems->getPath());
                    spriteIcon->setPosition(spriteIconBG->getContentSize().width / 2, spriteIconBG->getContentSize().height / 2);
                    spriteIcon->getTexture()->setTexParameters(texParams);
                    spriteIcon->setScale(spriteIconBG->getContentSize().width / spriteIcon->getContentSize().width);
                    spriteIconBG->addChild(spriteIcon);

                    std::string strText = MafUtils::toString(obj->getTradeToCount() * obj->getTradeProceedingCount());
                    if ( objItems->getType().compare("BUFF") == 0 )
                    {
                        int nMin = obj->getTradeToCount() * obj->getTradeProceedingCount() / 60;
                        strText = GAME_TEXTFORMAT("t_ui_minute", nMin);
                    }

                    auto labelText = Label::createWithTTF(strText, GAME_FONT, 20);
                    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    labelText->setPosition(spriteIconBG->getContentSize().width / 2, 5);
                    spriteIconBG->addChild(labelText);
                }
            }
        }
        
        

        //
        auto itemArrowL = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_quantity_down.png"), nullptr, CC_CALLBACK_1(PopupBonsikSoulChange::onClickArrowL, this));
        itemArrowL->setPosition(200 - 80, spriteBG->getContentSize().height / 2);
        itemArrowL->setTouchRect(itemArrowL->getContentSize() * 2);
        itemArrowL->setTag(i);
        menu->addChild(itemArrowL);
        if ( i != 0 )
            itemArrowL->setVisible(false);

        auto itemArrowR = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_quantity_up.png"), nullptr, CC_CALLBACK_1(PopupBonsikSoulChange::onClickArrowR, this));
        itemArrowR->setPosition(200 + 80, spriteBG->getContentSize().height / 2);
        itemArrowR->setTouchRect(itemArrowR->getContentSize() * 2);
        itemArrowR->setTag(i);
        menu->addChild(itemArrowR);
        if ( i != 0 )
            itemArrowR->setVisible(false);

        auto labelCount = Label::createWithTTF(MafUtils::format("x %d", obj->getTradeForCount() * obj->getTradeProceedingCount()), GAME_FONT, 24);
        labelCount->setPosition(200, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(labelCount);

        //
        auto spriteTrade = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteTrade->setContentSize(Size(142, 86));

        auto itemTrade = MafNode::MafMenuItemSprite::create(spriteTrade, nullptr, CC_CALLBACK_1(PopupBonsikSoulChange::onClickTrade, this));
        itemTrade->setPosition(spriteBG->getContentSize().width - 10 - itemTrade->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
        itemTrade->setTag(i);
        menu->addChild(itemTrade);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_prison_msg_35"), GAME_FONT, 24);
            labelText->setPosition(itemTrade->getContentSize().width / 2, itemTrade->getContentSize().height / 2);
            itemTrade->addChild(labelText);

            if ( obj->getTradeLimit() > 0 )
            {
                auto labelLimit = Label::createWithTTF(MafUtils::format("(%d/%d)", obj->getTradeCount(), obj->getTradeLimit()), GAME_FONT, 16);
                labelLimit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                labelLimit->setPosition(itemTrade->getContentSize().width / 2, 5);
                itemTrade->addChild(labelLimit);
            }
        }


        
    }
}

#pragma mark - set, get, other
void PopupBonsikSoulChange::setCallbackResult(const std::function<void(void)>& callback)
{
    _onCallbackResult = callback;
}

#pragma mark - click
void PopupBonsikSoulChange::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupBonsikSoulChange::onClickArrowL(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    auto objTradeSelect = _listTrade.at(item->getTag());
    int nCount = objTradeSelect->getTradeProceedingCount() - 1;
    if ( nCount < 1 )
    {
        nCount = 10;
    }
    
    objTradeSelect->setTradeProceedingCount(nCount);
    
    //
    uiContents();
}

void PopupBonsikSoulChange::onClickArrowR(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    auto objTradeSelect = _listTrade.at(item->getTag());
    int nCount = objTradeSelect->getTradeProceedingCount() + 1;
    if ( nCount > 10 )
    {
        nCount = 1;
    }
    
    objTradeSelect->setTradeProceedingCount(nCount);
    
    //
    uiContents();
}

void PopupBonsikSoulChange::onClickTrade(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    auto objTradeSelect = _listTrade.at(item->getTag());
    
    int nCurrencyNow = ItemsMoneyManager::getInstance()->getMonsterSoulPiece();
    int nCurrencyNeed = objTradeSelect->getTradeForCount() * objTradeSelect->getTradeProceedingCount();
    int nCount = objTradeSelect->getTradeToCount() * objTradeSelect->getTradeProceedingCount();
    if ( nCurrencyNeed > nCurrencyNow || nCount <= 0  )
    {
        return;
    }
    
    auto objItems = ItemsManager::getInstance()->getInfoItems(objTradeSelect->getTradeToType());
    if ( objItems == nullptr )
    {
        return;
    }
    
    // 지급
    cocos2d::Vector<InfoItems*> listReward;
    if ( (E_ITEMS)objItems->getIdx() == E_ITEMS::MONSTER_SOUL )
    {
        for ( int i = 0; i < nCount; i++ )
        {
            int nTypeParams = random(1, C_COUNT_MONSTER);
            
            InfoItems* objTemp = nullptr;
            for ( auto& obj : listReward )
            {
                if ( obj->getTypeParams() == nTypeParams )
                {
                    objTemp = obj;
                    break;
                }
            }
            
            if ( objTemp != nullptr )
            {
                objTemp->setCount(std::atoi(objTemp->getCount().c_str()) + 1);
            }
            else
            {
                auto objTemp = ItemsManager::getInstance()->getInfoItems(E_ITEMS::MONSTER_SOUL);
                objTemp->setCount(1);
                objTemp->setTypeParams(nTypeParams);
                listReward.pushBack(objTemp);
            }
        }
    }
    else
    {
        objItems->setCount(nCount);
        listReward.pushBack(objItems);
    }
    ItemsManager::getInstance()->addItems(listReward);
    
    // 감소
    ItemsMoneyManager::getInstance()->setMonsterSoulPiece(-nCurrencyNeed);
    objTradeSelect->setTradeProceedingCount(1);
    
    // analytics
    MafAnalyticsManager::LogEvent(kAnalPediaChange, kRepeatTrue);
    
    // draw
    uiContents();
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult();
    }
}
 
