//
//  UILayerSpiritLevelup.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/24.
//

#include "UILayerSpiritLevelup.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

UILayerSpiritLevelup* UILayerSpiritLevelup::create(cocos2d::Size size, InfoWeaponSpirit* info)
{
    UILayerSpiritLevelup* pRet = new(std::nothrow) UILayerSpiritLevelup();
    if ( pRet && pRet->init(size, info) )
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

UILayerSpiritLevelup::UILayerSpiritLevelup(void):
_lyUI(nullptr)
,_infoSpirit(nullptr)
{
}

UILayerSpiritLevelup::~UILayerSpiritLevelup(void)
{
}

bool UILayerSpiritLevelup::init(cocos2d::Size size, InfoWeaponSpirit* info)
{
    if ( !UILayerSpiritParent::init())
    {
        return false;
    }
    
    setContentSize(size);
    _infoSpirit = info;
    //
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerSpiritLevelup::initVar()
{
}

void UILayerSpiritLevelup::initUi()
{
    // UI
    _lyUI = Layer::create();
    _lyUI->setContentSize(Size(getContentSize()));
    _lyUI->setIgnoreAnchorPointForPosition(false);
    _lyUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyUI->setPosition(getContentSize()/2);
    addChild(_lyUI);
    
    //
    uiMain();
}

void UILayerSpiritLevelup::refreshUI(InfoWeaponSpirit* info)
{
    _infoSpirit = info;
    uiMain();
}

#pragma mark - ui
void UILayerSpiritLevelup::uiMain()
{
    _lyUI->removeAllChildren();
    
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_max_lv");
    
    auto sprCurrencyBox = Sprite::create("Assets/ui_common/base_text_2_4.png");
    sprCurrencyBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprCurrencyBox->setPosition(5, _lyUI->getContentSize().height - 5);
    _lyUI->addChild(sprCurrencyBox);
    {
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::COSTUME_COIN);

        auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
        uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCurrency->setPosition(Vec2(10, sprCurrencyBox->getContentSize().height / 2));
        sprCurrencyBox->addChild(uiCurrency, E_ZORER::UI+1);
    }
    
    if(_infoSpirit->getCurrentLv() < objSetting->getValueToInt())
    {
        auto sprAfterBox = Sprite::create("Assets/ui_common/base_text_3_1.png");
        sprAfterBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        sprAfterBox->setPosition(_lyUI->getContentSize().width - 5, _lyUI->getContentSize().height - 5);
        _lyUI->addChild(sprAfterBox);
        {
            auto lbText = Label::createWithTTF("AFTER", GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(sprAfterBox->getContentSize()/2);
            lbText->setDimensions(sprAfterBox->getContentSize().width - 5, sprAfterBox->getContentSize().height - 5);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            lbText->setColor(Color3B(51,255,0));
            lbText->enableBold();
            sprAfterBox->addChild(lbText);
        }
    }
    
    auto sprStatBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprStatBox->setContentSize(Size(674, 130));
    sprStatBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprStatBox->setPosition(_lyUI->getContentSize().width/2, sprCurrencyBox->getPosition().y - sprCurrencyBox->getContentSize().height - 10);
    _lyUI->addChild(sprStatBox);
    
    
    std::vector<std::pair<std::string, std::pair<double, double>>> listEffect = {
        std::make_pair(GAME_TEXT("t_ui_town_msg_2"), std::make_pair(_infoSpirit->getEffectDmg(), _infoSpirit->getEffectDmg(_infoSpirit->getCurrentLv() + 1, _infoSpirit->getCurrentSmelting()))),
        std::make_pair(GAME_TEXT("t_ui_town_msg_5"), std::make_pair(_infoSpirit->getEffectGold(), _infoSpirit->getEffectGold(_infoSpirit->getCurrentLv() + 1, _infoSpirit->getCurrentSmelting()))),
        std::make_pair(GAME_TEXT("t_ui_town_msg_3"), std::make_pair(_infoSpirit->getEffectKey(), _infoSpirit->getEffectKey(_infoSpirit->getCurrentLv() + 1, _infoSpirit->getCurrentSmelting())))
    };
    
    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];
        
        double posY = (sprStatBox->getContentSize().height - 5) - 40 *i;
        
        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 26);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(15, posY);
        sprStatBox->addChild(lbTitle);
        
        double nEffect = obj.second.first;
        double nPlusEffect = obj.second.second;
        std::string strEffect = MafUtils::doubleToString(nEffect);
        std::string strPlusEffect = MafUtils::doubleToString(nPlusEffect);
        
        MafUtils::convertNumberToShort(strEffect);
        MafUtils::convertNumberToShort(strPlusEffect);
        
        auto lbEffect = Label::createWithTTF(strEffect + "%", GAME_FONT, 26);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(sprStatBox->getContentSize().width - 230, posY);
        lbEffect->setDimensions(120, lbEffect->getContentSize().height);
        lbEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbEffect->setOverflow(Label::Overflow::SHRINK);
        sprStatBox->addChild(lbEffect);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyUI->getParent()->convertToWorldSpace(_lyUI->getPosition());
                posToast.y += _lyUI->getContentSize().height / 2.2f;
                
                std::string strEffect = MafUtils::doubleToString(nEffect);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }
        
        if(_infoSpirit->getCurrentLv() < objSetting->getValueToInt())
        {
            auto lbPlusEffect = Label::createWithTTF(strPlusEffect + "%", GAME_FONT, 26);
            lbPlusEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            lbPlusEffect->setPosition(sprStatBox->getContentSize().width-15, posY);
            lbPlusEffect->setColor(Color3B(51,255,0));
            lbPlusEffect->setDimensions(120, lbPlusEffect->getContentSize().height);
            lbPlusEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbPlusEffect->setOverflow(Label::Overflow::SHRINK);
            sprStatBox->addChild(lbPlusEffect);
            {
                auto menu = MafNode::MafMenu::create();
                menu->setPosition(Vec2::ZERO);
                lbPlusEffect->addChild(menu);
                
                auto area = Layer::create();
                area->setContentSize(lbPlusEffect->getContentSize());
                
                auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                    Vec2 posToast = _lyUI->getParent()->convertToWorldSpace(_lyUI->getPosition());
                    posToast.y += _lyUI->getContentSize().height / 2.2f;
                    
                    std::string strPlusEffect = MafUtils::doubleToString(nPlusEffect);
                    strPlusEffect = MafUtils::addCommaToString(strPlusEffect, 3, ',');
                    strPlusEffect += "%";
                    PopupToast::show(strPlusEffect, posToast.x, posToast.y);
                });
                btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                btn->setPosition(lbPlusEffect->getContentSize()/2);
                menu->addChild(btn);
            }
            
            auto sprArrow = Sprite::create("Assets/ui_common/arrow_R04.png");
            sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            sprArrow->setPosition(sprStatBox->getContentSize().width - sprStatBox->getContentSize().width/3.7 ,posY - 10);
            sprStatBox->addChild(sprArrow);
        }
        else
        {
            lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            lbEffect->setPosition(sprStatBox->getContentSize().width-15, posY);
        }
        
    }
    
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnRankUp->setPosition(Vec2(_lyUI->getContentSize().width/2, 25));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(UILayerSpiritLevelup::onClickLevelUp, this));
    _lyUI->addChild(btnRankUp);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_level_up_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnRankUp->getContentSize().width/2, btnRankUp->getContentSize().height - btnRankUp->getContentSize().height/4);
        lbText->setDimensions(btnRankUp->getContentSize().width - 10, btnRankUp->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbText);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
        auto sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(btnRankUp->getContentSize().width/3.5, btnRankUp->getContentSize().height/4);
        btnRankUp->addChild(sprCurrencyIcon);
        
        auto lbCount = Label::createWithTTF(std::to_string(_infoSpirit->getLvUpCost()), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnRankUp->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbCount);
        
    }
    if(_infoSpirit->getCurrentLv() >= objSetting->getValueToInt())
    {
        btnRankUp->setVisible(false);
    }
}

#pragma mark -utils
void UILayerSpiritLevelup::onClickLevelUp(Ref* sender)
{
    if(_infoSpirit->getLvUpCost() > std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()))
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestSpiritLevelup(CC_CALLBACK_2(UILayerSpiritLevelup::callbackLevelup, this), _infoSpirit->getIdx());
    }
}

#pragma mark -callback
void UILayerSpiritLevelup::callbackLevelup(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    //
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 0 )
        {

        }
        else if( nResult == 2 )
        {
            
        }
        else if( nResult == 3 )
        {
            
        }
        else if( nResult == 4 )
        {
            
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    uiMain();
    
    if(_callbackRedraw != nullptr)
        _callbackRedraw();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
}
