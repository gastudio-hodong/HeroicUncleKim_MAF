//
//  UIBuff.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UIBuff.h"

#include "GameUIPopup/Other/Buff/PopupBuff.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

UIBuff* UIBuff::create(std::vector<E_BUFF_TYPE> listBuff)
{
    UIBuff* pRet = new(std::nothrow) UIBuff();
    if ( pRet && pRet->init(listBuff) )
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

UIBuff::UIBuff(void)
{
    
}

UIBuff::~UIBuff(void)
{
    
}

bool UIBuff::init(std::vector<E_BUFF_TYPE> listBuff)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _listBuff = listBuff;
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    setContentSize(Size(41 * _listBuff.size(), 41));
    
    // init
    initVar();
    initUi();
     
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::BUFF_BUY, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::BUFF_BUY_DEVIL, this);
    
    return true;
}

#pragma mark - override : refresh
void UIBuff::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::BUFF_BUY || eType == E_REFRESH::BUFF_BUY_DEVIL )
    {
        drawContainer();
    }
}
 

#pragma mark - init
void UIBuff::initVar()
{
    
}

void UIBuff::initUi()
{
    Size size = getContentSize();
    
    _uiContainer = ui::Layout::create();
    _uiContainer->setContentSize(size);
    _uiContainer->setAnchorPoint(Vec2::ZERO);
    _uiContainer->setPosition(Vec2::ZERO);
    _uiContainer->setLayoutType(ui::Layout::Type::HORIZONTAL);
    addChild(_uiContainer);
   
    //
    uiContainer();
    
    //
    schedule(schedule_selector(UIBuff::drawContainer), 1.0, kRepeatForever, 0);
}

// ui
void UIBuff::uiContainer()
{
    _uiContainer->removeAllChildren();
    
    auto layout = _uiContainer;
    
    //
    for ( int i = 0; i < _listBuff.size(); i++ )
    {
        E_BUFF_TYPE eType = _listBuff.at(i);
        
        std::string path = getPath(eType);
        bool bAds = isAdsAvailable(eType);
        
        auto uiButton = ui::Button::create(path);
        uiButton->addClickEventListener(CC_CALLBACK_1(UIBuff::onClickBuff, this));
        uiButton->setTag((int)eType);
        layout->addChild(uiButton);
        {
            auto uiTextCount = ui::CText::create("", GAME_FONT_AL, 18);
            uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextCount->setPosition(Vec2(uiButton->getContentSize().width / 2, 0));
            uiTextCount->setName("UI_TEXT_COUNT");
            uiButton->addChild(uiTextCount);
        }
        
        if ( bAds == true )
        {
            auto sprAdsFx = Sprite::create("Assets/ui_common/icon_buff_fx_1_01.png");
            sprAdsFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAdsFx->setPosition(uiButton->getContentSize()/2);
            sprAdsFx->setName("ADS_FX");
            uiButton->addChild(sprAdsFx);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 16; i++ )
            {
                auto sprFrame = Sprite::create(MafUtils::format("Assets/ui_common/icon_buff_fx_1_%02d.png", i));
                sprFrame->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(sprFrame->getSpriteFrame());
            }
            sprAdsFx->runAction(RepeatForever::create(Animate::create(animation)));
            sprAdsFx->setVisible(false);
        }
        
    }
    
    // draw
    drawContainer();
}

#pragma mark - ui
void UIBuff::drawContainer(float dt/* = 0*/)
{
    auto layout = _uiContainer;
    
    //
    bool bRequestLayout = false;
    for ( int i = 0; i < _listBuff.size(); i++ )
    {
        E_BUFF_TYPE eType = _listBuff.at(i);
        
        auto uiButton = layout->getChildByTag<ui::Button*>((int)eType);
        auto uiTextCount = uiButton->getChildByName<ui::CText*>("UI_TEXT_COUNT");
        
        bool bVisible = isVisible(eType);
        int time = UserInfoManager::getInstance()->getBuffTime(eType);
        int adsTime = 0;
        
        switch (eType)
        {
            case E_BUFF_TYPE::BUFF_ATTACK_BONUS: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK); break;
            case E_BUFF_TYPE::BUFF_COIN_BONUS: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD); break;
            case E_BUFF_TYPE::BUFF_MOVE_BONUS: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE); break;
                
            default:
                break;
        }
        
        
        if ( time > 0 )
        {
            if ( eType == E_BUFF_TYPE::BUFF_KEY_BONUS )
            {
                uiButton->setOpacity(255);
                uiTextCount->setString(MafUtils::toString(time));
            }
            else
            {
                int nDay = time / 86400;
                int nHour = time % 86400 / 3600;
                int nMin = time % 3600 / 60;
                int nSec = time % 60;
                
                bool bSec = false;
                std::string strText = "";
                if ( nDay != 0 )        strText = MafUtils::format("%dD", nDay);
                else if ( nHour != 0 )  strText = MafUtils::format("%dH", nHour);
                else if ( nMin != 0 )   strText = MafUtils::format("%dM", nMin);
                else
                {
                    bSec = true;
                    strText = MafUtils::format("%dS", nSec);
                }
                uiTextCount->setString(strText);
                if ( bSec == true )
                    uiTextCount->setColor(COLOR_GEM);
                else
                    uiTextCount->setColor(Color3B::WHITE);
                
                //
                uiButton->setOpacity(255);
                if ( uiButton->isVisible() == false )
                {
                    bRequestLayout = true;
                    uiButton->setVisible(true);
                }
            }
            
            
            if ( eType == E_BUFF_TYPE::BUFF_ATTACK_BONUS )
            {
                std::string strAttackIconPath = "Assets/ui_common/icon_rage.png";
                
                auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
                if ( objPass != nullptr && objPass->isBuyPass() == true )
                {
                    time_t nDiffTime = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
                    if ( nDiffTime > 0 )
                    {
                        strAttackIconPath = "Assets/ui_common/icon_rage_2.png";
                    }
                }
                uiButton->loadTextureNormal(strAttackIconPath);
            }
        }
        else
        {
            if ( bVisible == true )
            {
                uiButton->setOpacity(100);
                uiTextCount->setString("");
            }
            else
            {
                uiButton->setVisible(false);
            }
            
        }
        
        auto adsFx = uiButton->getChildByName("ADS_FX");
        if ( adsFx != nullptr )
        {
            if ( adsTime <= 0 )
            {
                adsFx->setVisible(true);
            }
            else
            {
                adsFx->setVisible(false);
            }
        }
    }
    
    if ( bRequestLayout == true )
    {
        layout->requestDoLayout();
    }
}
 
#pragma mark - set, get
bool UIBuff::isAdsAvailable(E_BUFF_TYPE eType)
{
    bool bAds = false;
    switch (eType) {
        case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
        case E_BUFF_TYPE::BUFF_COIN_BONUS:
        case E_BUFF_TYPE::BUFF_MOVE_BONUS:
        case E_BUFF_TYPE::BUFF_D_ATK_DMG:
        case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
        case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
        case E_BUFF_TYPE::BUFF_D_ATK_SPD:
            bAds = true;
            break;
            
        default:
            break;
    }
    
    return bAds;
}

bool UIBuff::isVisible(E_BUFF_TYPE eType)
{
    bool bVisible = false;
    switch (eType) {
        case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
        case E_BUFF_TYPE::BUFF_COIN_BONUS:
        case E_BUFF_TYPE::BUFF_MOVE_BONUS:
        case E_BUFF_TYPE::BUFF_KEY_BONUS:
        case E_BUFF_TYPE::BUFF_D_ATK_DMG:
        case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
        case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
        case E_BUFF_TYPE::BUFF_D_ATK_SPD:
            bVisible = true;
            break;
            
        default:
            break;
    }
    
    return bVisible;
}

std::string UIBuff::getPath(E_BUFF_TYPE eType)
{
    std::string path = "";
    switch (eType) {
        case E_BUFF_TYPE::BUFF_ATTACK_BONUS:
            path = "Assets/ui_common/icon_rage.png";
            break;
        case E_BUFF_TYPE::BUFF_COIN_BONUS:
            path = "Assets/ui_common/icon_double.png";
            break;
        case E_BUFF_TYPE::BUFF_MOVE_BONUS:
            path = "Assets/ui_common/icon_speed.png";
            break;
        case E_BUFF_TYPE::BUFF_KEY_BONUS:
            path = "Assets/ui_common/icon_key.png";
            break;
        case E_BUFF_TYPE::BUFF_ATTACK_9_BONUS:
            path = "Assets/ui_common/icon_rage_3.png";
            break;
        case E_BUFF_TYPE::BUFF_ATTACK_10_BONUS:
            path = "Assets/ui_common/icon_rage_4.png";
            break;
        case E_BUFF_TYPE::BUFF_D_ATK_DMG:
            path = "Assets/ui_common/icon_buff_atk.png";
            break;
        case E_BUFF_TYPE::BUFF_D_QUEST_GOLD:
            path = "Assets/ui_common/icon_buff_quest.png";
            break;
        case E_BUFF_TYPE::BUFF_D_MOVE_SPD:
            path = "Assets/ui_common/icon_buff_spd.png";
            break;
        case E_BUFF_TYPE::BUFF_D_ATK_SPD:
            path = "Assets/ui_common/icon_buff_atkspd.png";
            break;
            
        default:
            break;
    }
    
    return path;
}

#pragma mark - schedule

#pragma mark - callback
void UIBuff::callbackRank(bool Result)
{
    drawContainer();
}

#pragma mark - click
void UIBuff::onClickBuff(cocos2d::Ref* sender)
{
    ValueVector list;
    
    if ( std::find(_listBuff.begin(), _listBuff.end(), E_BUFF_TYPE::BUFF_ATTACK_BONUS) != _listBuff.end() )
    {
        list.push_back(Value(E_SHOP::SHOP_ATTACK_BONUS));
        list.push_back(Value(E_SHOP::SHOP_COIN_BONUS));
        list.push_back(Value(E_SHOP::SHOP_MOVE_BONUS));
    }
    else
    {
        list.push_back(Value(E_SHOP::SHOP_D_ATK_DMG));
        list.push_back(Value(E_SHOP::SHOP_D_QUEST_GOLD));
        list.push_back(Value(E_SHOP::SHOP_D_MOVE_SPD));
        list.push_back(Value(E_SHOP::SHOP_D_ATK_SPD));
    }
    
    auto popup = PopupBuff::create(list);
    popup->show();
}


#pragma mark - event
