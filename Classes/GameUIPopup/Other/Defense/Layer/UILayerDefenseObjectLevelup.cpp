//
//  UILayerDefenseObjectLevelup.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/07.
//

#include "UILayerDefenseObjectLevelup.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerDefenseObjectLevelup* UILayerDefenseObjectLevelup::create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    UILayerDefenseObjectLevelup* pRet = new(std::nothrow) UILayerDefenseObjectLevelup();
    if ( pRet && pRet->init(size, type, idx) )
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

UILayerDefenseObjectLevelup::UILayerDefenseObjectLevelup(void):
_type(E_DF_Trench_SettingType::NONE),
_idx(0),
_lbNowCnt(nullptr),
_lbNextCnt(nullptr)
{
}

UILayerDefenseObjectLevelup::~UILayerDefenseObjectLevelup(void)
{
    DefenseManager::getInstance()->_onServerLevelup.Clear(this);
    _onHide.Clear();
}

bool UILayerDefenseObjectLevelup::init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    if ( !LayerColor::init())
    {
        return false;
    }
    
    setContentSize(size);
    _type = type;
    _idx = idx;
    //
    
    
    DefenseManager::getInstance()->_onServerLevelup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(UILayerDefenseObjectLevelup::callbackLevelup, this), this);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerDefenseObjectLevelup::initVar()
{
}

void UILayerDefenseObjectLevelup::initUi()
{
    // UI
    //
    uiMain();
}

#pragma mark - ui
void UILayerDefenseObjectLevelup::uiMain()
{
    removeAllChildren();
    
    auto checkCostSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_cost");
    auto checkGapSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_gap");
    
    
    auto characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    auto weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    bool isServerCheck = false;
    int maxLv = 0;
    int currentLv = 0;
    int cost = 0;
    int checkCost = 10;
    int checkGap = 10;
    double currentDFDamage = 0;
    double nextDFDamage = 0;
    
    if(auto checkCostSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_cost"))
    {
        checkCost = checkCostSetting->getValueToInt();
    }
    if(auto checkGapSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_gap"))
    {
        checkGap = checkGapSetting->getValueToInt();
    }
    
    switch (_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            maxLv = characterData->getMaxLv();
            currentLv = characterData->getCurrentLv();
            currentDFDamage = characterData->getDFAttack();
            nextDFDamage = characterData->getDFAttack(characterData->getCurrentLv() + 1);
            cost = characterData->getLvUpCost();
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            maxLv = weaponData->getMaxLv();
            currentLv = weaponData->getCurrentLv();
            currentDFDamage = weaponData->getDFAttack();
            nextDFDamage = weaponData->getDFAttack(weaponData->getCurrentLv() + 1);
            cost = weaponData->getLvUpCost();
        }break;
            
        default:
            break;
    }
    
    if((currentLv + 1) / checkGap >= 1 &&
       (currentLv + 1) % checkGap == 0)
    {
        isServerCheck = true;
    }
    
    auto sprCurrencyBox = Sprite::create("Assets/ui_common/base_text_2_4.png");
    sprCurrencyBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprCurrencyBox->setPosition(5, getContentSize().height - 5);
    addChild(sprCurrencyBox);
    {
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::DEFENSE_COIN);

        auto uiCurrency = UICurrency::create(list, Size(getContentSize().width/2, 40));
        uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCurrency->setPosition(Vec2(10, sprCurrencyBox->getContentSize().height / 2));
        sprCurrencyBox->addChild(uiCurrency, E_ZORER::UI+1);
        
        list.clear();
        list.push_back(E_ITEMS::COSTUME_COIN);
        auto uiCurrency2 = UICurrency::create(list, Size(250, 40));
        uiCurrency2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCurrency2->setPosition(Vec2(sprCurrencyBox->getContentSize().width + 200, sprCurrencyBox->getContentSize().height / 2));
        sprCurrencyBox->addChild(uiCurrency2, E_ZORER::UI+1);
        
    }
    
    auto sprStatBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprStatBox->setContentSize(Size(674, 50));
    sprStatBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprStatBox->setPosition(getContentSize().width/2, sprCurrencyBox->getPosition().y - sprCurrencyBox->getContentSize().height - 10);
    addChild(sprStatBox);
    
    
    if(currentLv < maxLv)
    {
        auto sprAfterBox = Sprite::create("Assets/ui_common/base_text_3_1.png");
        sprAfterBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        sprAfterBox->setPosition(getContentSize().width - 12, sprStatBox->getPositionY() - sprStatBox->getContentSize().height - 10);
        addChild(sprAfterBox);
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
    
    std::vector<std::pair<std::string, std::pair<double, double>>> listEffect;
    
    switch (_type) {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            listEffect = {
                std::make_pair(GAME_TEXT("t_ui_defense_setting_character_total"), std::make_pair(currentDFDamage, nextDFDamage))
            };
        }
            break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            listEffect = {
                std::make_pair(GAME_TEXT("t_ui_defense_setting_weapon_total"), std::make_pair(currentDFDamage, nextDFDamage))
            };
        }break;
        default:
            break;
    }
    
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
        
        if(nEffect >= 1000000)
            MafUtils::convertNumberToShort(strEffect);
        if(nPlusEffect >= 1000000)
            MafUtils::convertNumberToShort(strPlusEffect);
        
        auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 26);
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
                Vec2 posToast = getParent()->convertToWorldSpace(getPosition());
                posToast.y += getContentSize().height / 2.2f;
                
                std::string strEffect = MafUtils::doubleToString(nEffect);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }
        
        if(currentLv < maxLv)
        {
            auto lbPlusEffect = Label::createWithTTF(strPlusEffect, GAME_FONT, 26);
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
                    Vec2 posToast = getParent()->convertToWorldSpace(getPosition());
                    posToast.y += getContentSize().height / 2.2f;
                    
                    std::string strPlusEffect = MafUtils::doubleToString(nPlusEffect);
                    strPlusEffect = MafUtils::addCommaToString(strPlusEffect, 3, ',');
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
    
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_gift_bg.png");
    btnRankUp->setScale9Enabled(true);
    btnRankUp->setContentSize(Size(248,110));
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnRankUp->setPosition(Vec2(getContentSize().width/2, 25));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(UILayerDefenseObjectLevelup::onClickLevelUp, this));
    addChild(btnRankUp);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_level_up_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnRankUp->getContentSize().width/2, btnRankUp->getContentSize().height - btnRankUp->getContentSize().height/4);
        lbText->setDimensions(btnRankUp->getContentSize().width - 10, btnRankUp->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbText);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_COIN);
        auto sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(sprCurrencyIcon->getContentSize().width, btnRankUp->getContentSize().height/4);
        btnRankUp->addChild(sprCurrencyIcon);
        
        auto lbCount = Label::createWithTTF(std::to_string(cost), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnRankUp->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbCount);
        
        if(!isServerCheck)
            checkCost = 0;
        
        item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
        sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprCurrencyIcon->setPosition(btnRankUp->getContentSize().width/2 + 5, btnRankUp->getContentSize().height/4);
        btnRankUp->addChild(sprCurrencyIcon);
        
        lbCount = Label::createWithTTF(std::to_string(checkCost), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width);
        lbCount->setDimensions(100, btnRankUp->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbCount);
        
    }
    
    if(currentLv >= maxLv)
    {
        btnRankUp->setVisible(false);
    }
}

#pragma mark -utils
void UILayerDefenseObjectLevelup::onClickLevelUp(Ref* sender)
{
    auto characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    auto weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    int checkCost = 10;
    int checkGap = 10;
    int64_t cost = 0;
    bool isServerLevelup = false;
    
    if(auto checkCostSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_cost"))
    {
        checkCost = checkCostSetting->getValueToInt();
    }
    if(auto checkGapSetting = DefenseManager::getInstance()->getInfoSetting("defense_lvcheck_gap"))
    {
        checkGap = checkGapSetting->getValueToInt();
    }
    
    switch (_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            cost = characterData->getLvUpCost();
            
            isServerLevelup = (characterData->getCurrentLv() + 1) % checkGap == 0;
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            cost = weaponData->getLvUpCost();
            
            isServerLevelup = (characterData->getCurrentLv() + 1) % checkGap == 0;
        }break;
            
        default:
            break;
    }
    
    if(DefenseManager::getInstance()->getDefenseCoin() < cost)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(isServerLevelup &&
       std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) < checkCost)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    DefenseManager::getInstance()->requestLevelup(_type, _idx);
}

#pragma mark -callback
void UILayerDefenseObjectLevelup::callbackLevelup(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        if(nResult != 9)
        {
            std::string strMessageKey = "t_ui_error_30";
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        _onHide.Invoke();
        return;
    }
    uiMain();
    
//    if(_callbackRedraw != nullptr)
//        _callbackRedraw();
    //RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    //RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
}

void UILayerDefenseObjectLevelup::onChangeObject(int idx)
{
    _idx = idx;
    uiMain();
}
