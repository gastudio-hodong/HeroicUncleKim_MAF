//
//  PopupBuff.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupBuff.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Pass/DungeonPass/PopupPurchaseDungeonPass.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupBuff* PopupBuff::create(cocos2d::ValueVector list)
{
    PopupBuff* pRet = new(std::nothrow) PopupBuff();
    if ( pRet && pRet->init(list) )
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

PopupBuff::PopupBuff(void) :
_eTypeShop(E_SHOP::SHOP_ATTACK_BONUS),
_eAdsBuffType(E_BUFF_TYPE::BUFF_ATTACK_BONUS),
_nTimer(0),
_isReward(false),

_layerContainerDungeonPass(nullptr)
{
    
}

PopupBuff::~PopupBuff(void)
{
    
}

bool PopupBuff::init(cocos2d::ValueVector list)
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    _list = list;
    
    // touch
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        hide();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // init
    initVar();
    initUi();
    
    scheduleUpdate();
    
    return true;
}

void PopupBuff::update(float dt)
{
    _nTimer += dt;
    
    
    if(_nTimer >= 1.f)
    {
        _nTimer = 0;
        reDrawAdsButton();
    }
}

#pragma mark - init
void PopupBuff::initVar()
{
 
     
}

void PopupBuff::initUi()
{
    _layerContainerDungeonPass = Layer::create();
    _layerContainerDungeonPass->setContentSize( Size(728, 174) );
    _layerContainerDungeonPass->setIgnoreAnchorPointForPosition(false);
    _layerContainerDungeonPass->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerDungeonPass->setPosition(getContentSize().width / 2, getContentSize().height - 90 - UserInfoManager::getInstance()->_fHeightIphoneX);
    addChild(_layerContainerDungeonPass);
    
    //
    float width = 700;
    
    if(getContentSize().width <= width)
        width = getContentSize().width;
    Size sizeBuffBox = Size(width, 153);
    
    int nCount = (int)_list.size();
    double nPosStartY = getContentSize().height / 2 + (nCount * sizeBuffBox.height / 2) + ((double)(nCount - 1) / 2 * 5);
    for ( int i = 0; i < nCount; i++ )
    {
        double nPosX = getContentSize().width / 2;
        double nPosY = nPosStartY - sizeBuffBox.height * i - 5 * i;
        
        auto layer = uiContent((E_SHOP)_list.at(i).asInt(), sizeBuffBox);
        layer->setIgnoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        layer->setPosition(nPosX, nPosY);
        addChild(layer);
    }
     
    
    //
    uiDungeonPass();
    reDrawAdsButton();
}

#pragma mark - ui
void PopupBuff::uiDungeonPass()
{
    _layerContainerDungeonPass->removeAllChildren();
    
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( objPass == nullptr )
    {
        return;
    }
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_2.png",Rect::ZERO,Rect(0,0,336,40));
    spriteBG->setContentSize(_layerContainerDungeonPass->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteBG->setPosition(_layerContainerDungeonPass->getContentSize().width / 2, 0);
    _layerContainerDungeonPass->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerDungeonPass->addChild(menu);
    
    auto spriteTitle = Sprite::create("Assets/ui/purchase/dungeonpass_title_1.png");
    spriteTitle->setPosition(_layerContainerDungeonPass->getContentSize().width / 2, _layerContainerDungeonPass->getContentSize().height - 40);
    _layerContainerDungeonPass->addChild(spriteTitle);
    
    // item purchase
    {
        auto itemPurchase = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/title_sparkle_bar_1_1.png"), nullptr, CC_CALLBACK_1(PopupBuff::onClickDungeonPass, this));
        itemPurchase->setPosition(_layerContainerDungeonPass->getContentSize().width / 2, _layerContainerDungeonPass->getContentSize().height - 100);
        menu->addChild(itemPurchase);
        {
            auto aniframeRun = Animation::create();
            aniframeRun->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 5; i++ )
            {
                aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/purchase/title_sparkle_bar_1_%d.png", i));
            }
            aniframeRun->addSpriteFrameWithFile("Assets/ui/purchase/title_sparkle_bar_1_1.png");
            
            auto action1 = Animate::create(aniframeRun);
            auto action2 = DelayTime::create(0.5);
            auto seq = Sequence::create(action1, action2, NULL);
            itemPurchase->getNormalImage()->runAction(RepeatForever::create(seq));
        }
        
        time_t nDiffPurchase = objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime();
        if ( nDiffPurchase < 0 ) nDiffPurchase = 0;
        
        std::string strText = GAME_TEXT("t_ui_dungeonpass_btn_purchase");
        if ( objPass->isBuyPass() == true )
            strText = GAME_TEXT("t_ui_dungeonpass_purchase_title_2");
        else if ( objPass->isBuyPass() == false && nDiffPurchase <= 0 )
            strText = GAME_TEXT("t_ui_cash_msg_9_0");
        
        auto labelPurchase = Label::createWithTTF(strText, GAME_FONT, 24);
        labelPurchase->setPosition(itemPurchase->getContentSize()/2);
        labelPurchase->setName("LABEL_TEXT");
        itemPurchase->addChild(labelPurchase);
    }
    
    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_7"), GAME_FONT, 20);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelText->setPosition(_layerContainerDungeonPass->getContentSize().width / 2, 0);
    labelText->setDimensions(_layerContainerDungeonPass->getContentSize().width - 20, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerDungeonPass->addChild(labelText);
    
}

Layer* PopupBuff::uiContent(E_SHOP eType, Size sizeBox)
{
    E_BUFF_TYPE eTypeBuff;
    switch (eType) {
        case E_SHOP::SHOP_ATTACK_BONUS:     eTypeBuff = E_BUFF_TYPE::BUFF_ATTACK_BONUS;     break;
        case E_SHOP::SHOP_COIN_BONUS:       eTypeBuff = E_BUFF_TYPE::BUFF_COIN_BONUS;       break;
        case E_SHOP::SHOP_MOVE_BONUS:       eTypeBuff = E_BUFF_TYPE::BUFF_MOVE_BONUS;       break;
            
        case E_SHOP::SHOP_D_MOVE_SPD:       eTypeBuff = E_BUFF_TYPE::BUFF_D_MOVE_SPD;       break;
        case E_SHOP::SHOP_D_ATK_SPD:        eTypeBuff = E_BUFF_TYPE::BUFF_D_ATK_SPD;        break;
        case E_SHOP::SHOP_D_ATK_DMG:        eTypeBuff = E_BUFF_TYPE::BUFF_D_ATK_DMG;        break;
        case E_SHOP::SHOP_D_QUEST_GOLD:     eTypeBuff = E_BUFF_TYPE::BUFF_D_QUEST_GOLD;     break;
        default:
            break;
    }
    
    std::string strTitle = DataManager::getInstance()->GetShopName(eType);
    std::string strContents = DataManager::getInstance()->GetShopExpain(eType);
    Color3B colorTitle = COLOR_COIN;
    Color3B colorContents = Color3B::WHITE;
    
    bool bPass = false;
    if ( eType == E_SHOP::SHOP_ATTACK_BONUS )
    {
        auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
        if ( objPass != nullptr && objPass->isBuyPass() == true )
        {
            time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( nDiffTime > 0 )
            {
                strTitle = GAME_TEXT("t_item_d1_title_pass_1");
                strContents = GAME_TEXT("t_item_d1_contents_pass_1");
                colorTitle = Color3B(255, 115, 0);
                colorContents = Color3B(255, 115, 0);
                
                bPass = true;
            }
        }
    }
    
    //
    auto layerBox = Layer::create();
    layerBox->setContentSize(sizeBox);
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_5.png", Rect::ZERO, Rect(35,60,10,10));
    spriteBG->setContentSize(layerBox->getContentSize());
    spriteBG->setPosition(layerBox->getContentSize().width / 2, layerBox->getContentSize().height / 2);
    layerBox->addChild(spriteBG);
    
    //
    auto spriteIconBg = Sprite::create("Assets/ui_common/base_box_2.png");
    spriteIconBg->setPosition(79, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(spriteIconBg);
    {
        if ( bPass == true )
        {
            auto spriteIconBorder = ui::Scale9Sprite::create("Assets/ui_common/box_item_1_2.png", Rect::ZERO, Rect(41,41,1,1));
            spriteIconBorder->setContentSize(Size(118, 126));
            spriteIconBorder->setPosition(spriteIconBg->getContentSize().width / 2, spriteIconBg->getContentSize().height / 2);
            spriteIconBg->addChild(spriteIconBorder);
        }
        
        std::string strPath = "";
        switch(eType)
        {
            case E_SHOP::SHOP_HERO_GEM :        strPath = "Assets/ui/shop/shop_new_1.png"; break;
            case E_SHOP::SHOP_FEVER :           strPath = "Assets/ui/shop/shop_fever_1.png"; break;
            case E_SHOP::SHOP_ATTACK_BONUS :    strPath = "Assets/ui/shop/shop_new_2.png"; break;
            case E_SHOP::SHOP_MOVE_BONUS :      strPath = "Assets/ui/shop/shop_new_3.png"; break;
            case E_SHOP::SHOP_COIN_BONUS :      strPath = "Assets/ui/shop/shop_new_4.png"; break;
            case E_SHOP::SHOP_GOLD_GEM :        strPath = "Assets/ui/shop/shop_new_5.png"; break;
            case E_SHOP::SHOP_GOLD_GEM_BIG :    strPath = "Assets/ui/shop/shop_new_6.png"; break;
            case E_SHOP::SHOP_D_MOVE_SPD :    strPath = "Assets/ui/shop/shop_new_7.png"; break;
            case E_SHOP::SHOP_D_ATK_SPD :    strPath = "Assets/ui/shop/shop_new_8.png"; break;
            case E_SHOP::SHOP_D_ATK_DMG :    strPath = "Assets/ui/shop/shop_new_9.png"; break;
            case E_SHOP::SHOP_D_QUEST_GOLD :    strPath = "Assets/ui/shop/shop_new_10.png"; break;
            case E_SHOP::SHOP_D_GOLD_GEM :    strPath = "Assets/ui/shop/shop_new_11.png"; break;
            default: break;
        }
        auto spriteIcon = Sprite::create(strPath);
        spriteIcon->setPosition( Vec2(spriteIconBg->getContentSize().width / 2, spriteIconBg->getContentSize().height / 2) );
        spriteIcon->getTexture()->setTexParameters(texParams);
        spriteIcon->setScale(2);
        spriteIconBg->addChild(spriteIcon);
    }
    
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 27);
    labelTitle->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    labelTitle->setPosition(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteBG->getContentSize().height - 54);
    labelTitle->setColor(colorTitle);
    labelTitle->setDimensions(220, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    spriteBG->addChild(labelTitle);
    
    auto labelContents = Label::createWithTTF(strContents, GAME_FONT, 22);
    labelContents->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    labelContents->setPosition(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteBG->getContentSize().height - 54);
    labelContents->setDimensions(220, labelTitle->getContentSize().height);
    labelContents->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelContents->setOverflow(Label::Overflow::SHRINK);
    labelContents->setColor(colorContents);
    spriteBG->addChild(labelContents);
    
    auto labelTime = Label::createWithTTF("", GAME_FONT, 16);
    labelTime->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    labelTime->setPosition(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, 10);
    labelTime->setDimensions(220, labelTitle->getContentSize().height);
    labelTime->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelTime->setOverflow(Label::Overflow::SHRINK);
    labelTime->setTag(eTypeBuff);
    spriteBG->addChild(labelTime);
    auto action1 = CallFuncN::create([=](Ref* sender){
        
        E_BUFF_TYPE eTypeBuff = (E_BUFF_TYPE)((Label*)sender)->getTag();
        int nTime = UserInfoManager::getInstance()->getBuffTime(eTypeBuff);
        if ( nTime <= 0 )
        {
            ((Label*)sender)->setString("");
            return;
        }
        
        int nHour = nTime / 3600;
        int nMin = nTime % 3600 / 60;
        int nSec = nTime % 60;
        
        std::string strTime = "";
        if ( nHour != 0 )
        {
            strTime.append(GAME_TEXTFORMAT("t_ui_hour", nHour));
            strTime.append(" ");
            strTime.append(GAME_TEXTFORMAT("t_ui_minute", nMin));
            strTime.append(" ");
            strTime.append(GAME_TEXTFORMAT("t_ui_second", nSec));
        }
        else if ( nMin != 0 )
        {
            strTime.append(GAME_TEXTFORMAT("t_ui_minute", nMin));
            strTime.append(" ");
            strTime.append(GAME_TEXTFORMAT("t_ui_second", nSec));
        }
        else
        {
            strTime.append(GAME_TEXTFORMAT("t_ui_second", nSec));
        }
        ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_buff_newbie_time", strTime.c_str()));
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    labelTime->runAction(RepeatForever::create(seq));
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    auto itemBuy = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"), nullptr, CC_CALLBACK_1(PopupBuff::onClickBuy, this));
    itemBuy->setPosition(spriteBG->getContentSize().width - itemBuy->getContentSize().width / 2 - 10, spriteBG->getContentSize().height / 2 );
    itemBuy->setTag(eType);
    menu->addChild(itemBuy);
    {
        int nPrice = getPrice(eType);
        int nGem = ItemsMoneyManager::getInstance()->getGem();
        if ( nGem < nPrice )
        {
            ((Sprite*)itemBuy->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
        
        auto spritePrice = Sprite::create("Assets/icon/icon_jewel.png");
        spritePrice->setPosition(itemBuy->getContentSize().width * 0.175, itemBuy->getContentSize().height * 0.3);
        itemBuy->addChild(spritePrice);
        
        auto labelPrice = MafNode::createBmFont(MafUtils::toString(nPrice), GAME_BM_FONT_AL, 28);
        labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPrice->setPosition(itemBuy->getContentSize().width*0.35, itemBuy->getContentSize().height * 0.3);
        labelPrice->setColor(COLOR_COIN);
        itemBuy->addChild(labelPrice);
        
        auto spriteTime = Sprite::create("Assets/icon/icon_time_1_1.png");
        spriteTime->setPosition(itemBuy->getContentSize().width * 0.175, itemBuy->getContentSize().height * 0.75);
        itemBuy->addChild(spriteTime);
        
        auto labelTime = MafNode::createBmFont("+30M", GAME_BM_FONT_AL, 28);
        labelTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTime->setPosition(itemBuy->getContentSize().width*0.35, itemBuy->getContentSize().height * 0.75);
        itemBuy->addChild(labelTime);
    }
    
    if(eTypeBuff <= E_BUFF_TYPE::BUFF_D_ATK_SPD)
    {
        auto sprAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_7_on.png");
        sprAds->setContentSize(itemBuy->getContentSize());
        
        auto sprOffAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png");
        sprOffAds->setContentSize(itemBuy->getContentSize());
        
        auto itemAds = MafNode::MafMenuItemSprite::create(sprAds, nullptr, sprOffAds, CC_CALLBACK_1(PopupBuff::onClickAds, this));
        itemAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        itemAds->setPosition(itemBuy->getPosition() + Vec2::LEFT * (itemBuy->getContentSize().width/2 + 10));
        
        itemAds->setTag(eTypeBuff);
        menu->addChild(itemAds);
        {
            auto sprAds1 = Sprite::create("Assets/icon/icon_ad_1_1.png");
            sprAds1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAds1->setPosition(itemAds->getContentSize().width / 3.5, itemAds->getContentSize().height * 0.3);
            sprAds1->setName("ADS_1");
            itemAds->addChild(sprAds1);
            
            auto sprAds2 = Sprite::create("Assets/icon/icon_ad_1_2.png");
            sprAds2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAds2->setPosition(itemAds->getContentSize().width - itemAds->getContentSize().width / 3.5, itemAds->getContentSize().height * 0.3);
            sprAds2->setName("ADS_2");
            itemAds->addChild(sprAds2);
            
            auto spriteTime = Sprite::create("Assets/icon/icon_time_1_1.png");
            spriteTime->setPosition(itemAds->getContentSize().width / 3.5, itemAds->getContentSize().height * 0.75);
            itemAds->addChild(spriteTime);
            
            auto labelTime = MafNode::createBmFont("+15M", GAME_BM_FONT_AL, 28);
            labelTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelTime->setPosition(spriteTime->getPositionX() + 25, itemAds->getContentSize().height * 0.75);
            itemAds->addChild(labelTime);
            
            auto lbCooldown = Label::createWithTTF("", GAME_FONT, 25);
            lbCooldown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbCooldown->setPosition(itemAds->getContentSize().width - itemAds->getContentSize().width / 2, itemAds->getContentSize().height * 0.3);
            lbCooldown->setName("ADS_COOLDOWN");
            lbCooldown->setDimensions(itemAds->getContentSize().width - 10, lbCooldown->getContentSize().height);
            lbCooldown->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbCooldown->setOverflow(Label::Overflow::SHRINK);
            itemAds->addChild(lbCooldown);
            
            lbCooldown->setVisible(false);
            
        }
        
        _vecAdsItem.pushBack(itemAds);
        
    }
    
    
    return layerBox;
}
 
void PopupBuff::reDrawAdsButton()
{
    for(auto item : _vecAdsItem)
    {
        E_BUFF_TYPE eBuffType = (E_BUFF_TYPE)item->getTag();
        
        int nAdsTime = 0;
        
        switch (eBuffType)
        {
            case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK);
                break;
            case E_BUFF_TYPE::BUFF_COIN_BONUS:
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD);
                break;
            case E_BUFF_TYPE::BUFF_MOVE_BONUS:
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE);
                break;
            case E_BUFF_TYPE::BUFF_D_ATK_DMG:
            {
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_ATK_SPD:
            {
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
            {
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
            {
                nAdsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD);
            }
                break;
        }

        auto sprAds1 = item->getChildByName("ADS_1");
        auto sprAds2 = item->getChildByName("ADS_2");
        auto lbAdsCool = (Label*) item->getChildByName("ADS_COOLDOWN");
        
        if(nAdsTime > 0)
        {
            item->setEnabled(false);
            
            if(sprAds1 != nullptr)
                sprAds1->setVisible(false);
            if(sprAds2 != nullptr)
                sprAds2->setVisible(false);
            if(lbAdsCool != nullptr)
            {
                lbAdsCool->setVisible(true);
                
                lbAdsCool->setString(MafUtils::convertNumberToTime(nAdsTime, false));
            }
        }
        else
        {
            item->setEnabled(true);
            
            if(sprAds1 != nullptr)
                sprAds1->setVisible(true);
            if(sprAds2 != nullptr)
                sprAds2->setVisible(true);
            if(lbAdsCool != nullptr)
            {
                lbAdsCool->setVisible(false);
            }
            
        }
    }
}

#pragma mark - set, get, sort, add
int PopupBuff::getPrice(E_SHOP eType)
{
    int nPrice = 0;
    
    if( eType == E_SHOP::SHOP_COIN_BONUS )          nPrice = MafRemoteConfig::GetRemoteValueInt("BUY_COIN_BONUS");
    else if( eType == E_SHOP::SHOP_ATTACK_BONUS )   nPrice = MafRemoteConfig::GetRemoteValueInt("BUY_ATTACK_BONUS");
    else if( eType == E_SHOP::SHOP_MOVE_BONUS )     nPrice = MafRemoteConfig::GetRemoteValueInt("BUY_MOVE_BONUS");
    else if( eType == E_SHOP::SHOP_D_MOVE_SPD )     nPrice = BUY_D_MOVE_SPD;
    else if( eType == E_SHOP::SHOP_D_ATK_SPD )      nPrice = BUY_D_ATK_SPD;
    else if( eType == E_SHOP::SHOP_D_ATK_DMG )      nPrice = BUY_D_ATK_DMG;
    else if( eType == E_SHOP::SHOP_D_QUEST_GOLD )   nPrice = BUY_D_QUEST_GOLD;
    
    return nPrice;
}

#pragma mark - click
void PopupBuff::onClickBuy(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    _eTypeShop = (E_SHOP)item->getTag();
    
    int nPrice = getPrice(_eTypeShop);
    int nGem = ItemsMoneyManager::getInstance()->getGem();
    if ( nGem < nPrice )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        
        hide();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(PopupBuff::onClickBuyOk, this));
    popup->show();
}

void PopupBuff::onClickBuyOk(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_BUY_GOLD);
    
    int nPrice = getPrice(_eTypeShop);
    E_BUFF_TYPE eTypeBuff = E_BUFF_TYPE::BUFF_ATTACK_BONUS;
    if( _eTypeShop == E_SHOP::SHOP_COIN_BONUS )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_COIN_BONUS;
    }
    else if( _eTypeShop == E_SHOP::SHOP_ATTACK_BONUS )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_ATTACK_BONUS;
    }
    else if( _eTypeShop == E_SHOP::SHOP_MOVE_BONUS )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_MOVE_BONUS;
        SpecialMissionManager::getInstance()->clearSpecialMission(93);
    }
    else if( _eTypeShop == E_SHOP::SHOP_D_MOVE_SPD )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_D_MOVE_SPD;
    }
    else if( _eTypeShop == E_SHOP::SHOP_D_ATK_SPD )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_D_ATK_SPD;
    }
    else if( _eTypeShop == E_SHOP::SHOP_D_ATK_DMG )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_D_ATK_DMG;
    }
    else if( _eTypeShop == E_SHOP::SHOP_D_QUEST_GOLD )
    {
        eTypeBuff = E_BUFF_TYPE::BUFF_D_QUEST_GOLD;
    }
        
    int nGem = ItemsMoneyManager::getInstance()->getGem();
    if ( nGem < nPrice )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        return;
    }
    
    //
    ItemsMoneyManager::getInstance()->setGem(-nPrice);
    
    //
    int nTime = TIME_BUFF_BONUS;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_21) )
    {
        nTime += 600;
    }
    UserInfoManager::getInstance()->setBuffTime(eTypeBuff, nTime);
     
    //
    int buffCount = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_BUFF_COUNT);
    SaveManager::setHistoryData(E_HISTORY_DATA::K_BUFF_COUNT, (++buffCount));
    
    // analytics
    if ( _eTypeShop == E_SHOP::SHOP_ATTACK_BONUS || _eTypeShop == E_SHOP::SHOP_MOVE_BONUS || _eTypeShop == E_SHOP::SHOP_COIN_BONUS )
    {
        int nItem = 0;
        if ( _eTypeShop == E_SHOP::SHOP_ATTACK_BONUS )     nItem = 1;
        else if ( _eTypeShop == E_SHOP::SHOP_MOVE_BONUS )       nItem = 2;
        else if ( _eTypeShop == E_SHOP::SHOP_COIN_BONUS )       nItem = 3;
        
        std::string strEventName = MafUtils::format(kAnalEventItemBuy, nItem);
        MafAnalyticsManager::LogEvent(strEventName.c_str(),kRepeatTrue);
    }
}

void PopupBuff::onClickDungeonPass(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_BUY_GOLD);
    
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( objPass == nullptr )
    {
        return;
    }
    
    if ( objPass->isBuyPass() || objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime() <= 600 )
    {
        return;
    }
    
    auto popup = PopupPurchaseDungeonPass::create();
    popup->show();
    
    hide();
}

void PopupBuff::onClickAds(Ref* sender)
{
    auto item = (Node*)sender;
    if(item == nullptr)
        return;
    
    E_ADS_WATCH eType = E_ADS_WATCH::NONE;
    
    _eAdsBuffType = (E_BUFF_TYPE)item->getTag();
    
    std::string strCount = MafUtils::toString(ParameterManager::getInstance()->getParameterInteger("ads_buff_count"));
    
    switch (_eAdsBuffType)
    {
        case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
        {
            eType = E_ADS_WATCH::BUFF_ATK;
        }
            break;
        case E_BUFF_TYPE::BUFF_COIN_BONUS:
        {
            eType = E_ADS_WATCH::BUFF_COIN;
        }
            break;
        case E_BUFF_TYPE::BUFF_MOVE_BONUS:
        {
            eType = E_ADS_WATCH::BUFF_SPD;
        }
            break;
        case E_BUFF_TYPE::BUFF_D_ATK_DMG:
        {
            eType = E_ADS_WATCH::BUFF_D_ATK_DMG;
        }
            break;
        case E_BUFF_TYPE::BUFF_D_ATK_SPD:
        {
            eType = E_ADS_WATCH::BUFF_D_ATK_SPD;
        }
            break;
        case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
        {
            eType = E_ADS_WATCH::BUFF_D_MOVE_SPD;
        }
            break;
        case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
        {
            eType = E_ADS_WATCH::BUFF_D_QUEST_GOLD;
        }
            break;
    }
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        ResultAdsCallBack(ADS_RESULT::SUCCESS);
    }
    else
    {
        auto popup = PopupAdsWatch::create(eType, strCount);
        popup->setCallbackResult(CC_CALLBACK_0(PopupBuff::onClickAdsOk, this));
        popup->show();
    }
}

void PopupBuff::onClickAdsOk()
{
    MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupBuff::ResultAdsCallBack, this));
}

void PopupBuff::ResultAdsCallBack(ADS_RESULT result)
{
    if ( _isReward == true )
        return;
    else
        _isReward = true;
    
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_ATTACK_BONUS )
        {
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotBuffAtk, CashManager::getInstance()->isAdsfreeTime());
        }
        else if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_COIN_BONUS )
        {
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotBuffGold, CashManager::getInstance()->isAdsfreeTime());
        }
        else if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_MOVE_BONUS )
        {
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotBuffMove, CashManager::getInstance()->isAdsfreeTime());
        }
        
        E_ITEMS eItems = E_ITEMS::NONE;
        int count = ParameterManager::getInstance()->getParameterInteger("ads_buff_count");
        switch (_eAdsBuffType)
        {
            case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
            {
                eItems = E_ITEMS::BUFF_ATK;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_COIN_BONUS:
            {
                eItems = E_ITEMS::BUFF_COIN;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_MOVE_BONUS:
            {
                eItems = E_ITEMS::BUFF_SPD;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_ATK_DMG:
            {
                eItems = E_ITEMS::BUFF_DEVIL_ATK;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_ATK_SPD:
            {
                eItems = E_ITEMS::BUFF_DEVIL_ASPD;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
            {
                eItems = E_ITEMS::BUFF_DEVIL_SPD;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE, count);
            }
                break;
            case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
            {
                eItems = E_ITEMS::BUFF_DEVIL_COIN;
                UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD, count);
            }
                break;
        }
        
        auto infoItems = ItemsManager::getInstance()->getInfoItems(eItems);
        infoItems->setCount(count);
        
        ItemsManager::getInstance()->addItem(infoItems);
        reDrawAdsButton();
        _isReward = false;
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupBuff::ResultAdsCallBack, this));
            if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_ATTACK_BONUS )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotBuffAtk);
            }
            else if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_COIN_BONUS )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotBuffGold);
            }
            else if ( _eAdsBuffType == E_BUFF_TYPE::BUFF_MOVE_BONUS )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotBuffMove);
            }
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        
        _isReward = false;
        reDrawAdsButton();
    }
}
