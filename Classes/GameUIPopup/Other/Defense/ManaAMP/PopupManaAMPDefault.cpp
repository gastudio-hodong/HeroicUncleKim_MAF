//
//  PopupManaAMPDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#include "PopupManaAMPDefault.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Common/PopupLoading.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPReset.h"

#include "GameObject/InfoItems.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/Defense/InfoDefenseManaAMP.h"
#include "GameObject/Defense/InfoDefenseManaAmpStone.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/SaveManager.h"


USING_NS_CC;

PopupManaAMPDefault* PopupManaAMPDefault::create(int number)
{
    PopupManaAMPDefault* pRet = new(std::nothrow) PopupManaAMPDefault();
    if ( pRet && pRet->init(number) )
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

PopupManaAMPDefault::PopupManaAMPDefault():
_lyContainerParent(nullptr)
,_number(0)
,_AMPData(nullptr)
,_currentStone(nullptr)
,_lyBG(nullptr)
,_lyTop(nullptr)
,_lyAMPInfo(nullptr)
,_lyMain(nullptr)
,_lyBottom(nullptr)
{
}

PopupManaAMPDefault::~PopupManaAMPDefault(void)
{
    DefenseManaAMPManager::getInstance()->_onServerInfo.Clear(this);
    DefenseManaAMPManager::getInstance()->_onServerUpdateAMP.Clear(this);
    DefenseManaAMPManager::getInstance()->_onServerEquipStone.Clear(this);
    _onReddotUpdate.Clear();
}

bool PopupManaAMPDefault::init(int number)
{
    if (!PopupBase::init())
    {
        return false;
    }
    _number = number;
    _bHideBackKey = false;
    updateAMPData();
    initUI();
    initVar();
    
    DefenseManaAMPManager::getInstance()->_onServerInfo += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupManaAMPDefault::onServerUpdate, this), this);
    DefenseManaAMPManager::getInstance()->_onServerUpdateAMP += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupManaAMPDefault::onServerUpdate, this), this);
    DefenseManaAMPManager::getInstance()->_onServerEquipStone += new MafFunction<void(bool, int, bool, E_DF_AMP_STONE_Type)>(CC_CALLBACK_4(PopupManaAMPDefault::onServerEquipStone, this), this);
    
    return true;
}

void PopupManaAMPDefault::onEnter()
{
    PopupBase::onEnter();
}

void PopupManaAMPDefault::hide()
{
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](bool){
        PopupLoading::hide();
        PopupBase::hide();
    });
}
void PopupManaAMPDefault::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupManaAMPDefault::initUI()
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
    
    _lyBG = LayerColor::create();
    _lyBG->setContentSize(Size(_lyContainerParent->getContentSize()));
    _lyBG->setIgnoreAnchorPointForPosition(false);
    _lyBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->setPosition(_lyContainerParent->getContentSize()/2);
    _lyContainerParent->addChild(_lyBG);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_lyContainerParent->getContentSize().width, 86));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_lyContainerParent->getContentSize().width/2, _lyContainerParent->getContentSize().height);
    _lyContainerParent->addChild(_lyTop);
    
    _lyAMPInfo = LayerColor::create();
    _lyAMPInfo->setContentSize(Size(_lyContainerParent->getContentSize().width, 156));
    _lyAMPInfo->setIgnoreAnchorPointForPosition(false);
    _lyAMPInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyAMPInfo->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _lyContainerParent->addChild(_lyAMPInfo);
    
    _lyMain = LayerColor::create();
    _lyMain->setContentSize(Size(_lyContainerParent->getContentSize().width,
                                 708));
    _lyMain->setIgnoreAnchorPointForPosition(false);
    _lyMain->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyMain->setPosition(_lyAMPInfo->getPosition() + Vec2::DOWN * _lyAMPInfo->getContentSize().height);
    _lyContainerParent->addChild(_lyMain);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_lyContainerParent->getContentSize().width, 384));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(_lyContainerParent->getContentSize().width/2, 36);
    _lyContainerParent->addChild(_lyBottom);
    
    
    updateUI();
}

#pragma mark - ui
void PopupManaAMPDefault::uiBG()
{
    _lyBG->removeAllChildren();
    
    auto bg1 = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bg1.png");
    bg1->getTexture()->setTexParameters(texParams);
    bg1->setScale(3);
    bg1->setPosition(_lyContainerParent->getContentSize()/2);
    bg1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->addChild(bg1);
    
    auto bg2 = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bg2.png");
    bg2->getTexture()->setTexParameters(texParams);
    bg2->setScale(3);
    bg2->setPosition(_lyContainerParent->getContentSize()/2);
    bg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->addChild(bg2);
    
    auto bg3 = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bg3.png");
    bg3->getTexture()->setTexParameters(texParams);
    bg3->setScale(3);
    bg3->setPosition(Vec2(125, 252) * 3);
    bg3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->addChild(bg3);
    
    auto bg4 = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bg4.png");
    bg4->getTexture()->setTexParameters(texParams);
    bg4->setScale(3);
    bg4->setPosition(_lyContainerParent->getContentSize()/2);
    bg4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->addChild(bg4);
    
    if(_AMPData == nullptr)
        return;
    
    if(_AMPData->isComplete())
    {
        auto action1 = FadeOut::create(1.5f);
        auto action2 = FadeIn::create(1.5f);
        
        bg2->runAction(RepeatForever::create(Sequence::create(action1, action2,  nullptr)));
        
        bg3->runAction(RepeatForever::create(RotateBy::create(10, 360)));
    }
    else
        bg2->setVisible(false);
    
}
void PopupManaAMPDefault::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto bg = Sprite::create("Assets/ui/defense/Horse_power_amplifier_top_box0.png");
    bg->getTexture()->setTexParameters(texParams);
    bg->setScale(3);
    bg->setPosition(_lyTop->getContentSize()/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyTop->addChild(bg);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTop->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/defense/Btn_defense_close.png"), nullptr, CC_CALLBACK_1(PopupManaAMPDefault::onClickClose, this));
    itemClose->setPosition(Vec2(_lyTop->getContentSize().width - 42, _lyTop->getContentSize().height/2));
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    auto btnHelp = ui::Button::create("Assets/ui/defense/Btn_defense_help.png");
    btnHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnHelp->setPosition(Vec2(50, _lyTop->getContentSize().height/2 + 3));
    btnHelp->setZoomScale(0.02f);
    btnHelp->addClickEventListener(CC_CALLBACK_1(PopupManaAMPDefault::onClickHelp, this));
    _lyTop->addChild(btnHelp);
    
    auto btnLeft = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/defense/Btn_yellow_arrow_l.png"), nullptr, nullptr,  CC_CALLBACK_1(PopupManaAMPDefault::onClickAnotherAMP, this));
    btnLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnLeft->setPosition(Vec2(180, _lyTop->getContentSize().height/2));
    btnLeft->setName("LEFT");
    btnLeft->setTouchRect(btnLeft->getContentSize()*2);
    menu->addChild(btnLeft);
    
    auto btnRight = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/defense/Btn_yellow_arrow_r.png"), nullptr, nullptr,  CC_CALLBACK_1(PopupManaAMPDefault::onClickAnotherAMP, this));
    btnRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnRight->setPosition(Vec2(_lyTop->getContentSize().width - 180, _lyTop->getContentSize().height/2));
    btnRight->setName("RIGHT");
    btnRight->setTouchRect(btnRight->getContentSize()*2);
    menu->addChild(btnRight);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp") + MafUtils::format(" %d", _number), GAME_FONT, 30);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbTitle->setPosition(Vec2(_lyTop->getContentSize()/2) + Vec2::UP * 3);
    lbTitle->setDimensions(340, _lyTop->getContentSize().height - 5);
    lbTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->enableBold();
    _lyTop->addChild(lbTitle);
}
void PopupManaAMPDefault::uiAMPInfo()
{
    if(_AMPData == nullptr)
        return;
    
    if(_AMPData->isReddotCondition())
    {
        _AMPData->completeReddotCondition();
        _onReddotUpdate.Invoke();
    }
    
    _lyAMPInfo->removeAllChildren();
    
    auto minimap = Sprite::create("Assets/ui/defense/Horse_power_amplifier_top_box1.png");
    minimap->getTexture()->setTexParameters(texParams);
    minimap->setScale(3);
    minimap->setPosition(_lyAMPInfo->getContentSize().width/2, _lyAMPInfo->getContentSize().height);
    minimap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyAMPInfo->addChild(minimap);
    
    auto ampInfo = Sprite::create("Assets/ui/defense/Horse_power_amplifier_top_box2.png");
    ampInfo->getTexture()->setTexParameters(texParams);
    ampInfo->setScale(3);
    ampInfo->setPosition(minimap->getPosition() + Vec2::DOWN * (minimap->getContentSize().height * minimap->getScale() - 72));
    ampInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyAMPInfo->addChild(ampInfo);
    
    auto lyMinimap = LayerColor::create();
    lyMinimap->setContentSize(Size(600, 90));
    lyMinimap->setIgnoreAnchorPointForPosition(false);
    lyMinimap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lyMinimap->setPosition(minimap->getPosition());
    _lyAMPInfo->addChild(lyMinimap);
    
    auto lyInfo = LayerColor::create();
    lyInfo->setContentSize(ampInfo->getContentSize() * ampInfo->getScale());
    lyInfo->setIgnoreAnchorPointForPosition(false);
    lyInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lyInfo->setPosition(ampInfo->getPosition());
    _lyAMPInfo->addChild(lyInfo);
    
    if(_AMPData->isComplete())
    {
        auto lbPowerText = Label::createWithTTF(GAME_TEXT("t_ui_town_msg_2") , GAME_FONT, 22);
        lbPowerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbPowerText->setPosition(Vec2(120, lyInfo->getContentSize().height/2));
        lbPowerText->setDimensions(lyInfo->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPowerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbPowerText->setOverflow(Label::Overflow::SHRINK);
        lyInfo->addChild(lbPowerText);
        
        std::string strEffect = "%s%%";
        
        std::string strEffectTmp = MafUtils::doubleToString(_AMPData->getManaAMPPower() * 100);
        //strEffectTmp = MafUtils::addCommaToString(strEffectTmp, 3, ',');
        
        MafUtils::convertNumberToShort(strEffectTmp);
        strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
        
        auto lbPower = Label::createWithTTF(strEffect, GAME_FONT, 22);
        lbPower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbPower->setPosition(Vec2(lyInfo->getContentSize().width - 120, lyInfo->getContentSize().height/2));
        lbPower->setDimensions(lyInfo->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPower->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        lbPower->setOverflow(Label::Overflow::SHRINK);
        lyInfo->addChild(lbPower);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbPower->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbPower->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyAMPInfo->getParent()->convertToWorldSpace(_lyAMPInfo->getPosition());
                posToast.y -= _lyAMPInfo->getContentSize().height / 2.f;
                
                std::string strEffect = MafUtils::doubleToString(_AMPData->getManaAMPPower() * 100);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbPower->getContentSize()/2);
            menu->addChild(btn);
        }
    }
    else
    {
        auto lbIncomplete = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_incomlete_1") , GAME_FONT, 22);
        lbIncomplete->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbIncomplete->setPosition(lyInfo->getContentSize()/2);
        lbIncomplete->setDimensions(lyInfo->getContentSize().width - 10, lbIncomplete->getContentSize().height);
        lbIncomplete->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbIncomplete->setOverflow(Label::Overflow::SHRINK);
        lyInfo->addChild(lbIncomplete);
    }
    
    Vec2 location = Vec2(22.5f, lyMinimap->getContentSize().height/2 + 8);
    bool isLeftFail = false;
    for(int i = 0; i < 10; ++i)
    {
        std::string stdValue = "";
        std::string iconPath = "";
        bool isSuccess = false;
        bool isCurrentNumber = false;
        E_DF_AMP_STONE_Type stoneType = (E_DF_AMP_STONE_Type)_AMPData->getStoneIndexByOrder(i+1);
        
        switch(stoneType)
        {
            case E_DF_AMP_STONE_Type::NONE:
            {
                if(i == _AMPData->getStoneCount())
                    isCurrentNumber = true;
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_off.png";
            }break;
            case E_DF_AMP_STONE_Type::BLACK:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_gray.png";
                stdValue = "+";
            }break;
            case E_DF_AMP_STONE_Type::RED:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_red.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::RED))
                    stdValue = MafUtils::format("%d", (int)data->getOutputQuantity());
            }break;
            case E_DF_AMP_STONE_Type::YELLO:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_yellow.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::YELLO))
                    stdValue = MafUtils::format("%.1f", data->getOutputQuantity());
            }break;
        }
        
        auto icon = Sprite::create(iconPath);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        icon->setPosition(location);
        lyMinimap->addChild(icon);
        
        if(isCurrentNumber)
        {
            auto currentEffect = Sprite::create("Assets/ui/defense/Horse_power_stone_mini_select.png");
            currentEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            currentEffect->setPosition(icon->getContentSize()/2);
            icon->addChild(currentEffect);
            
            auto action1 = FadeOut::create(0.5f);
            auto action2 = FadeIn::create(0.5f);
            
            currentEffect->runAction(RepeatForever::create(Sequence::create(action1, action2,  nullptr)));
        }
        if(isSuccess)
        {
            auto successEffect = Sprite::create("Assets/ui/defense/Horse_power_stone_mini_blue.png");
            successEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            successEffect->setPosition(icon->getContentSize()/2);
            icon->addChild(successEffect);
            
            if(_AMPData->getRedStoneCount() == 10)
            {
                successEffect->setColor(Color3B(255,240,0));
            }
            else
            {
                successEffect->setColor(Color3B(0,225,255));
            }
            
            if(i != 0 && !isLeftFail)
            {
                auto successLine = Sprite::create("Assets/ui/defense/Horse_power_stone_line_blue.png");
                successLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                successLine->setPosition(icon->getPosition());
                successLine->getTexture()->setTexParameters(texParams);
                lyMinimap->addChild(successLine);
                
                
                if(_AMPData->getRedStoneCount() == 10)
                {
                    successLine->setColor(Color3B(255,240,0));
                }
                else
                {
                    successLine->setColor(Color3B(0,225,255));
                }
                
                auto lbOperator = MafNode::createBmFont("X", GAME_BM_FONT_AL, 20);
                lbOperator->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                lbOperator->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
                lbOperator->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height/2);
                lyMinimap->addChild(lbOperator);
                
            }
            if(isLeftFail)
                isLeftFail = false;
        }
        
        auto lbValue = MafNode::createBmFont(stdValue, GAME_BM_FONT_AL, 20);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbValue->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbValue->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height/2 + Vec2::RIGHT * icon->getContentSize().width/2);
        lyMinimap->addChild(lbValue);
        
        if(stoneType == E_DF_AMP_STONE_Type::BLACK)
            isLeftFail = true;
                
        location += Vec2::RIGHT * (icon->getContentSize().width + 15);
    }
}
void PopupManaAMPDefault::uiMain()
{
    if(_AMPData == nullptr)
        return;
    
    _lyMain->removeAllChildren();
    _currentStone = nullptr;
    
    Size size = _lyMain->getContentSize();
    //216 147 132 291 162 246 288 138
    std::vector<Vec2> posList =
    {
        //1
        Vec2(size.width/2, size.height - 88),
        //2
        Vec2(size.width - 216, size.height - 147),
        //3
        Vec2(size.width - 132, size.height - 291),
        //4
        Vec2(size.width - 162, 246),
        //5
        Vec2(size.width - 288, 138),
        //6
        Vec2(288, 138),
        //7
        Vec2(162, 246),
        //8
        Vec2(132, size.height - 291),
        //9
        Vec2(216, size.height - 147),
        //10
        Vec2(size.width/2, size.height - 336),
    };
    
    bool isLeftFail = false;
    for(int i = 0; i < posList.size(); ++i)
    {
        Vec2 pos = posList[i];
        std::string iconPath = "";
        bool isSuccess = false;
        bool isCurrentNumber = false;
        E_DF_AMP_STONE_Type stoneType = (E_DF_AMP_STONE_Type)_AMPData->getStoneIndexByOrder(i+1);
        
        switch(stoneType)
        {
            case E_DF_AMP_STONE_Type::NONE:
            {
                if(i == _AMPData->getStoneCount())
                    isCurrentNumber = true;
                if(isCurrentNumber)
                    iconPath = "Assets/ui/defense/Horse_power_stone_select.png";
                else
                    iconPath = "Assets/ui/defense/Horse_power_amplifier_lock.png";
            }break;
            case E_DF_AMP_STONE_Type::BLACK:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_big_gray.png";
                isLeftFail = true;
            }break;
            case E_DF_AMP_STONE_Type::RED:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_big_red.png";
                isSuccess = true;
            }break;
            case E_DF_AMP_STONE_Type::YELLO:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_big_yellow.png";
                isSuccess = true;
            }break;
        }
        
        auto icon = Sprite::create(iconPath);
        icon->setPosition(pos);
        _lyMain->addChild(icon);
        
        auto lbNumber = Label::createWithTTF(MafUtils::format("%d", i+1), GAME_FONT, 26);
        lbNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbNumber->setPosition(icon->getPosition() + Vec2::DOWN * 10);
        lbNumber->setColor(Color3B(250,210,160));
        lbNumber->enableBold();
        _lyMain->addChild(lbNumber);
        
        if(stoneType != E_DF_AMP_STONE_Type::NONE || isCurrentNumber)
            lbNumber->setVisible(false);
        
        if(isCurrentNumber)
        {
            auto action1 = FadeOut::create(0.5f);
            auto action2 = FadeIn::create(0.5f);
            
            icon->runAction(RepeatForever::create(Sequence::create(action1, action2,  nullptr)));
            _currentStone = icon;
        }
        if(isSuccess)
        {
            auto successCircle = Sprite::create("Assets/ui/defense/Horse_power_circle_blue.png");
            successCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            successCircle->setPosition(icon->getContentSize()/2);
            icon->addChild(successCircle);
            if(_AMPData->getRedStoneCount() == 10)
            {
                successCircle->setColor(Color3B(255,240,0));
            }
            else
            {
                successCircle->setColor(Color3B(0,225,255));
            }
            if(i != 0 && !isLeftFail)
            {
                auto successLine = Sprite::create(MafUtils::format("Assets/ui/defense/Horse_power_line_blue%d.png", i));
                successLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                successLine->setPosition(size.width/2, size.height - 339);
                successLine->getTexture()->setTexParameters(texParams);
                successLine->setScale(3);
                _lyMain->addChild(successLine);
                
                if(_AMPData->getRedStoneCount() == 10)
                {
                    successLine->setColor(Color3B(255,240,0));
                }
                else
                {
                    successLine->setColor(Color3B(0,225,255));
                }
            }
            if(isLeftFail)
                isLeftFail = false;
        }
    }
}
void PopupManaAMPDefault::uiBottom()
{
    if(_AMPData == nullptr)
        return;
    
    _lyBottom->removeAllChildren();
    
    auto currencyBox = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bottom_box1.png");
    currencyBox->getTexture()->setTexParameters(texParams);
    currencyBox->setScale(3);
    currencyBox->setPosition(_lyBottom->getContentSize().width/2, _lyBottom->getContentSize().height);
    currencyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->addChild(currencyBox);
    
    auto buttonBox = Sprite::create("Assets/ui/defense/Horse_power_amplifier_bottom_box0.png");
    buttonBox->getTexture()->setTexParameters(texParams);
    buttonBox->setScale(3);
    buttonBox->setPosition(currencyBox->getPosition() + Vec2::DOWN * currencyBox->getContentSize().height * currencyBox->getScale());
    buttonBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->addChild(buttonBox);
    
    auto lyCurrencyBox = LayerColor::create();
    lyCurrencyBox->setContentSize(currencyBox->getContentSize() * currencyBox->getScale());
    lyCurrencyBox->setIgnoreAnchorPointForPosition(false);
    lyCurrencyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lyCurrencyBox->setPosition(_lyBottom->getContentSize().width/2, _lyBottom->getContentSize().height);
    _lyBottom->addChild(lyCurrencyBox);
    
    auto lyButtonBox = LayerColor::create();
    lyButtonBox->setContentSize(buttonBox->getContentSize() * buttonBox->getScale());
    lyButtonBox->setIgnoreAnchorPointForPosition(false);
    lyButtonBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lyButtonBox->setPosition(lyCurrencyBox->getPosition() + Vec2::DOWN * lyCurrencyBox->getContentSize().height);
    lyButtonBox->setName("BUTTON_LAYER");
    _lyBottom->addChild(lyButtonBox);
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::DEFENSE_COIN);
    list.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);

    auto uiCurrency = UICurrency::create(list, Size(lyCurrencyBox->getContentSize().width - 300, lyCurrencyBox->getContentSize().height));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCurrency->setPosition(lyCurrencyBox->getContentSize()/2);
    lyCurrencyBox->addChild(uiCurrency, E_ZORER::UI+1);
    
    for(int i = 0; i < 2; ++i)
    {
        E_DF_AMP_STONE_Type stonType = (E_DF_AMP_STONE_Type) (i+2);
        Vec2 btnLocation = Vec2();
        Vec2 rateLocation = Vec2();
        std::string iconPath = "";
        InfoDefenseManaAmpStone* ston = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)stonType);
        Color3B rateColor = Color3B(255,240,160);
        int rate = 0;
        switch(stonType)
        {
            case E_DF_AMP_STONE_Type::RED:
            {
                btnLocation = Vec2(220, lyButtonBox->getContentSize().height - 24);
                rate = _AMPData->getCurrentSuccessRate();
                iconPath = "Assets/ui/defense/Horse_power_stone_middle_red.png";
                rateLocation = Vec2(339, 142);
                
                int maxRate = 100;
                int minRate = 0;
                if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_max_success_rate"))
                    maxRate = setting->getValueToInt();
                if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_min_success_rate"))
                    minRate = setting->getValueToInt();
                
                if(rate >= maxRate)
                    rateColor = Color3B(30, 230, 30);
                else if(rate <= minRate)
                    rateColor = Color3B(240, 30, 30);
            }break;
            case E_DF_AMP_STONE_Type::YELLO:
            {
                btnLocation = Vec2(lyButtonBox->getContentSize().width - 220, lyButtonBox->getContentSize().height - 24);
                rate = ston->getBonusSuccessRate();
                iconPath = "Assets/ui/defense/Horse_power_stone_middle_yellow.png";
                rateLocation = Vec2(lyButtonBox->getContentSize(). width - 103, 142);
                rateColor = Color3B(30, 230, 30);
            }break;
        }
        
        auto btnStone = ui::Button::create("Assets/ui/defense/Btn_defense_basic_blue.png");
        btnStone->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnStone->setPosition(btnLocation);
        btnStone->setTag((int)stonType);
        btnStone->addClickEventListener(CC_CALLBACK_1(PopupManaAMPDefault::onClickEquipStone, this));
        btnStone->setName("EQUIP");
        lyButtonBox->addChild(btnStone);
        
        auto sprStone = Sprite::create(iconPath);
        sprStone->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprStone->setPosition(Vec2(50, btnStone->getContentSize().height - 10));
        btnStone->addChild(sprStone);
        
        auto sprCostBox = Sprite::create("Assets/ui/defense/Btn_defense_basic_blue_box.png");
        sprCostBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprCostBox->setPosition(Vec2(btnStone->getContentSize().width - 15, 15));
        btnStone->addChild(sprCostBox);
        
        std::string strPower = "";
        strPower = MafUtils::format("x%d", (int)ston->getOutputQuantity());
        
        auto lbEquip = Label::createWithTTF(strPower + " " + GAME_TEXT("t_ui_defense_manaamp_equip_stone"), GAME_FONT, 22);
        lbEquip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbEquip->setPosition(sprCostBox->getPosition().x - sprCostBox->getContentSize().width/2, btnStone->getContentSize().height - 7);
        lbEquip->setDimensions(130, lbEquip->getContentSize().height);
        lbEquip->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        lbEquip->setOverflow(Label::Overflow::SHRINK);
        btnStone->addChild(lbEquip);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_COIN);
        
        auto costIcon = Sprite::create(item->getPath());
        costIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        costIcon->setPosition(Vec2(0, sprCostBox->getContentSize().height/2));
        sprCostBox->addChild(costIcon);
        
        auto lbCost = Label::createWithTTF(std::to_string(_AMPData->getEquipCost()), GAME_FONT, 22);
        lbCost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbCost->setPosition(sprCostBox->getContentSize().width - 10, sprCostBox->getContentSize().height/2 + 2);
        lbCost->setDimensions(sprCostBox->getContentSize().width - costIcon->getContentSize().width - 10, lbCost->getContentSize().height);
            lbCost->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
            lbCost->setOverflow(Label::Overflow::SHRINK);
            sprCostBox->addChild(lbCost);
            
        auto lbSuccessRate = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_per"), GAME_FONT, 24);
        lbSuccessRate->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbSuccessRate->setPosition(btnStone->getPosition() + Vec2::LEFT * (btnStone->getContentSize().width/2 - 5)
                                       + Vec2::DOWN * (btnStone->getContentSize().height));
        lbSuccessRate->setDimensions(140, lbSuccessRate->getContentSize().height);
        lbSuccessRate->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
        lbSuccessRate->setOverflow(Label::Overflow::SHRINK);
        lbSuccessRate->setColor(Color3B(150,100,70));
        lyButtonBox->addChild(lbSuccessRate);
        
        auto lbRate = Label::createWithTTF(MafUtils::format("%d%%",rate), GAME_FONT, 35);
        lbRate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbRate->setPosition(rateLocation);
        lbRate->setDimensions(120, lbRate->getContentSize().height);
        lbRate->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        lbRate->setOverflow(Label::Overflow::SHRINK);
        lbRate->setColor(rateColor);
        lyButtonBox->addChild(lbRate);
        
        if(_AMPData->isComplete())
        {
            lbRate->setVisible(false);
            btnStone->setEnabled(false);
        }
    }
    
    auto btnReset = ui::Button::create("Assets/ui/defense/Btn_defense_bottom_off.png");
    btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnReset->setPosition(Vec2(lyButtonBox->getContentSize().width/2, 0));
    btnReset->addClickEventListener(CC_CALLBACK_1(PopupManaAMPDefault::onClickReset, this));
    btnReset->setName("RESET");
    btnReset->setZoomScale(0);
    lyButtonBox->addChild(btnReset);
    
    auto lbReset = Label::createWithTTF("RESET", GAME_FONT, 28);
    lbReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbReset->setPosition(btnReset->getContentSize(). width/2 + 10, 60);
    lbReset->setDimensions(135, lbReset->getContentSize().height);
    lbReset->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbReset->setOverflow(Label::Overflow::SHRINK);
    lbReset->setColor(Color3B(135,135,135));
    btnReset->addChild(lbReset);
    
    if(_AMPData->isComplete())
    {
        auto sprOn = Sprite::create("Assets/ui/defense/Btn_defense_bottom_on.png");
        sprOn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprOn->setPosition(Vec2(btnReset->getContentSize()/2));
        btnReset->addChild(sprOn);
        
        lbReset->setColor(Color3B::WHITE);
    }
}


#pragma mark - click
void PopupManaAMPDefault::onClickClose(Ref* sender)
{
    hide();
}
void PopupManaAMPDefault::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_defense_manaamp_info_title_%d", "t_ui_defense_manaamp_info_msg_%d");
    popup->show();
}
void PopupManaAMPDefault::onClickEquipStone(Ref* sender)
{
    auto node = (Node*)sender;
    
    E_DF_AMP_STONE_Type type = (E_DF_AMP_STONE_Type)node->getTag();
    int cost = _AMPData->getEquipCost();
    
    if(DefenseManager::getInstance()->getDefenseCoin() < cost)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(auto layer = _lyBottom->getChildByName("BUTTON_LAYER"))
    {
        for(auto node : layer->getChildren())
        {
            if(auto btn = dynamic_cast<ui::Button*>(node))
            {
                if(btn->getName() == "RESET" || btn->getName() == "EQUIP")
                    btn->setEnabled(false);
            }
        }
    }
    
    
    DefenseManaAMPManager::getInstance()->requestChoiceStone(_number, type);
}
void PopupManaAMPDefault::onClickReset(Ref* sender)
{
    if(!_AMPData->isComplete())
        return;
    
    int cost = 100;
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_reset"))
        cost = setting->getValueToInt();
    
    if(NewRaidManager::getInstance()->getDragonBall() < cost)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupManaAMPReset::create(_number);
    popup->_onReset += new MafFunction<void(void)>([=](){
        DefenseManaAMPManager::getInstance()->requestResetAMP(_number);
    }, this);
    popup->show();
}
void PopupManaAMPDefault::onClickAnotherAMP(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getName() == "LEFT")
        _number--;
    if(node->getName() == "RIGHT")
        _number++;
    
    if(_number <= 0)
        _number = DefenseManaAMPManager::getInstance()->getMaxManaAMPCount();
    if(_number > DefenseManaAMPManager::getInstance()->getMaxManaAMPCount())
        _number = 1;
    
    updateAMPData();
    updateUI();
}
#pragma mark - utils

void PopupManaAMPDefault::updateUI()
{
    uiBG();
    uiTop();
    uiAMPInfo();
    uiMain();
    uiBottom();
}
void PopupManaAMPDefault::updateAMPData()
{
    _AMPData = DefenseManaAMPManager::getInstance()->getManaAmpData(_number);
}

void PopupManaAMPDefault::playEquipEffect(const bool isSuccess, const E_DF_AMP_STONE_Type type)
{
    if(_currentStone == nullptr)
    {
        updateUI();
        return;
    }
    
    int rand = random(0, 100);
    bool isRevertEffect = false;
    if(rand <= 10)
        isRevertEffect = true;
    
    std::string iconPath = "";
    switch (type) {
        case E_DF_AMP_STONE_Type::RED:
        {
            iconPath = "Assets/ui/defense/Horse_power_stone_big_red.png";
        }break;
        case E_DF_AMP_STONE_Type::YELLO:
        {
            iconPath = "Assets/ui/defense/Horse_power_stone_big_yellow.png";
        }break;
            
        default:
            break;
    }
    
    if(isSuccess)
    {
        if(isRevertEffect)
        {
            _currentStone->setTexture("Assets/ui/defense/Horse_power_stone_big_gray.png");
        }
        else
        {
            _currentStone->setTexture(iconPath);
        }
    }
    else
    {
        if(isRevertEffect)
        {
            _currentStone->setTexture(iconPath);
        }
        else
        {
            _currentStone->setTexture("Assets/ui/defense/Horse_power_stone_big_gray.png");
        }
    }
    
    auto successAnim = Animation::create();
    for(int i = 0; i <= 4; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Horse_power_stone_success%02d.png", i);
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        successAnim->addSpriteFrame(sprite->getSpriteFrame());
    }
    successAnim->setDelayPerUnit(0.05f);
    auto successAnimate = Animate::create(successAnim);
    
    auto failAnim = Animation::create();
    for(int i = 0; i <= 4; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Horse_power_stone_fail%02d.png", i);
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        failAnim->addSpriteFrame(sprite->getSpriteFrame());
    }
    failAnim->setDelayPerUnit(0.05f);
    auto failAnimate = Animate::create(failAnim);
    
    auto funcUpdateUI = CallFunc::create(CC_CALLBACK_0(PopupManaAMPDefault::updateUI, this));
    
    auto successSeq = Sequence::create(successAnimate, funcUpdateUI, RemoveSelf::create(), nullptr);
    auto failSeq = Sequence::create(failAnimate, funcUpdateUI, RemoveSelf::create(), nullptr);
    
    auto funcChangeIcon = CallFunc::create([=](){
        
        if(isSuccess)
        {
            _currentStone->setTexture(iconPath);
        }
        else
        {
            _currentStone->setTexture("Assets/ui/defense/Horse_power_stone_big_gray.png");
        }
    });
    
    auto successRevertSeq = Sequence::create(failAnimate, funcChangeIcon, successAnimate->reverse(), funcUpdateUI, RemoveSelf::create(),nullptr);
    
    auto failRevertSeq = Sequence::create(successAnimate, funcChangeIcon, failAnimate, funcUpdateUI, RemoveSelf::create(),nullptr);
    
    auto effect = Sprite::create("Assets/ui/defense/Horse_power_stone_success00.png");
    effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    effect->setScale(3);
    effect->setPosition(_currentStone->getContentSize()/2);
    _currentStone->addChild(effect);
    
    if(isSuccess)
    {
        if(type == E_DF_AMP_STONE_Type::YELLO)
        {
            effect->runAction(successSeq);
            return;
        }
            
        if(isRevertEffect)
        {
            effect->runAction(successRevertSeq);
        }
        else
        {
            effect->runAction(successSeq);
        }
    }
    else
    {
        if(isRevertEffect)
        {
            effect->runAction(failRevertSeq);
        }
        else
        {
            effect->runAction(failSeq);
        }
    }
}

#pragma mark - callback
void PopupManaAMPDefault::onServerUpdate(const bool bResult, const int nResult)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    
    updateAMPData();
    updateUI();
}
void PopupManaAMPDefault::onServerEquipStone(const bool bResult, const int nResult, const bool isSuccess, const E_DF_AMP_STONE_Type type)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    
    updateAMPData();
    playEquipEffect(isSuccess, type);
}
