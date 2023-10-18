//
//  PopupDefenseDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#include "PopupDefenseDefault.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "GameUIPopup/Other/Defense/Layer/LayerDefenseMain.h"
#include "GameUIPopup/Other/Defense/PopupDefenseTrenchSetting.h"
#include "GameUIPopup/Other/Defense/PopupDefenseObjectList.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseShopDefault.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;

PopupDefenseDefault* PopupDefenseDefault::create()
{
    PopupDefenseDefault* pRet = new(std::nothrow) PopupDefenseDefault();
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

PopupDefenseDefault::PopupDefenseDefault():
_lyContainerParent(nullptr)
,_lyMainGame(nullptr)
,_lyBottom(nullptr)
{
}

PopupDefenseDefault::~PopupDefenseDefault(void)
{
    DefenseManager::getInstance()->setOpenContentsNow(false);
    DefenseManager::getInstance()->setIsBackground(false);
    DefenseManager::getInstance()->clearEvents();
}

bool PopupDefenseDefault::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    DefenseManager::getInstance()->setOpenContentsNow(true);
    DefenseManager::getInstance()->clearEvents();
    
    DefenseManager::getInstance()->_onUpdateBackgound += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseDefault::onBackground, this), this);
    
    _bHideBackKey = false;
    initUI();
    initVar();
    
    return true;
}

void PopupDefenseDefault::onEnter()
{
    PopupBase::onEnter();
    
    DefenseManager::getInstance()->_onServerInfo += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupDefenseDefault::onServerInfo, this), this);
    
    DefenseManager::getInstance()->_onServerUpdateFloor += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupDefenseDefault::onServerUpdateFloor, this), this);
    
    
    DefenseManager::getInstance()->requestInfo();
}

void PopupDefenseDefault::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupDefenseDefault::initUI()
{
    Size size = getContentSize();
    bool isSizeOver = false;
    if(size.width >= 750)
    {
        size.width = 750;
        isSizeOver = true;
    }
    if(size.height >= 1335)
    {
        size.height = 1335;
        isSizeOver = true;
    }
    
    _lyContainerParent = Layer::create();
    _lyContainerParent->setContentSize(size);
    _lyContainerParent->setIgnoreAnchorPointForPosition(false);
    _lyContainerParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyContainerParent->setPosition(getContentSize().width / 2, 0);
    addChild(_lyContainerParent);
    
    _lyMainGame = LayerColor::create();
    _lyMainGame->setContentSize(_lyContainerParent->getContentSize());
    _lyMainGame->setIgnoreAnchorPointForPosition(false);
    _lyMainGame->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    _lyMainGame->setPosition(_lyContainerParent->getContentSize()/2);
    _lyContainerParent->addChild(_lyMainGame);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_lyContainerParent->getContentSize().width, 200));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(_lyContainerParent->getContentSize().width / 2, 0);
    _lyContainerParent->addChild(_lyBottom);
    
    uiBottom();
}

#pragma mark - ui
void PopupDefenseDefault::uiMain()
{
    _lyMainGame->removeAllChildren();
    
    _mainGame = LayerDefenseMain::create(_lyMainGame->getContentSize());
    _mainGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _mainGame->setPosition(_lyMainGame->getContentSize()/2);
    _mainGame->_onClose += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseDefault::hide, this) ,this);
    _lyMainGame->addChild(_mainGame, 2);
}
void PopupDefenseDefault::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto sprBg = Sprite::create("Assets/ui/defense/Ui_bottom_box.png");
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(_lyBottom->getContentSize()/2);
    _lyBottom->addChild(sprBg);
    
    Vec2 buttonLocation = Vec2(40, sprBg->getContentSize().height - 80);
    for(int i = 0; i < 4; ++i)
    {
        int idx = i+1;
        
        std::string path = "Assets/ui/defense/Ui_bottom_btn_off.png";
        if(idx == 4)
            path = "Assets/ui/defense/Ui_bottom_btn_on.png";
        auto btn = ui::Button::create(path);
        btn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        btn->setPosition(buttonLocation);
        btn->addClickEventListener(CC_CALLBACK_1(PopupDefenseDefault::onClickButton, this));
        btn->setTag(idx);
        btn->setZoomScale(0.05f);
        btn->setTitleFontSize(26);
        btn->setTitleText(GAME_TEXT(MafUtils::format("t_ui_defense_main_button%d",i+1)));
        btn->setTitleAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
        btn->getTitleLabel()->setDimensions(90, btn->getTitleLabel()->getContentSize().height);
        btn->getTitleLabel()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btn->getTitleLabel()->setPosition(Vec2(btn->getContentSize().width - 10, btn->getContentSize().height/2));
        btn->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprBg->addChild(btn);
        
        std::string iconPath = "";
        if(idx == 1)
            iconPath = "Assets/icon/icon_defense_party.png";
        else if(idx == 2)
            iconPath = "Assets/icon/icon_defense_mole.png";
        else if(idx == 3)
            iconPath = "Assets/icon/icon_defense_gun.png";
        else if(idx == 4)
            iconPath = "Assets/icon/icon_defense_shop.png";
        
        auto icon = Sprite::create(iconPath);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        icon->setPosition(0, btn->getContentSize().height/2);
        btn->addChild(icon);
        
        if(idx == 2 || idx == 3)
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(Vec2(15, btn->getContentSize().height) - Size(0, 15));
            uiReddot->setTag(idx);
            btn->addChild(uiReddot);
            uiReddot->setVisible(false);

            auto action1 = CallFuncN::create([=](Ref* sender){
                auto uiReddot = (UIReddot*)sender;
                
                if(uiReddot->getTag() == 2)
                {
                    if ( DefenseManager::getInstance()->isReddotCondition(E_DF_Trench_SettingType::CHARACTER))
                    {
                        uiReddot->setVisible(true);
                    }
                    else
                    {
                        uiReddot->setVisible(false);
                    }
                }
                else if(uiReddot->getTag() == 3)
                {
                    if ( DefenseManager::getInstance()->isReddotCondition(E_DF_Trench_SettingType::WEAPON))
                    {
                        uiReddot->setVisible(true);
                    }
                    else
                    {
                        uiReddot->setVisible(false);
                    }
                }
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
        
        buttonLocation += Vec2::RIGHT * (btn->getContentSize().width + 15);
    }
    
}

#pragma mark - click
void PopupDefenseDefault::onBackground()
{
    if(DefenseManager::getInstance()->isBackground())
        setLocalZOrder(-1);
    else
        setLocalZOrder(ZORDER_SCENE_POPUP);
}
#pragma mark - click
void PopupDefenseDefault::onClickClose(Ref* sender)
{
    hide();
}
void PopupDefenseDefault::onClickButton(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getTag() == 1)
    {
        auto popup = PopupDefenseTrenchSetting::create();
        popup->show();
    }
    if(node->getTag() == 2)
    {
        auto popup = PopupDefenseObjectList::create(E_DF_Trench_SettingType::CHARACTER);
        popup->show();
    }
    if(node->getTag() == 3)
    {
        auto popup = PopupDefenseObjectList::create(E_DF_Trench_SettingType::WEAPON);
        popup->show();
    }
    if(node->getTag() == 4)
    {
        auto popup = PopupDefenseShopDefault::create();
        popup->show();
    }
}
#pragma mark - callback
void PopupDefenseDefault::onServerInfo(const bool bResult, const int nResult)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    DialogManager::getInstance()->showDialog("DEFENSE_OPENNINIG");
    uiMain();
    
    if(!DefenseManager::getInstance()->isFirstGift())
        DefenseManager::getInstance()->requestFirstGift();
}

void PopupDefenseDefault::onServerUpdateFloor(const bool bResult, const int nResult)
{
    if(!bResult || nResult != 1)
    {
        if(nResult == 9)
            hide();
        return;
    }
}
