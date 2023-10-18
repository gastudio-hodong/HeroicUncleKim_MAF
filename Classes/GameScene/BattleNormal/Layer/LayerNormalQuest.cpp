//
//  LayerNormalQuest.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerNormalQuest.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUI/UICell/UILayoutQuestCell.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Equip/PopupWeaponCraft.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

LayerNormalQuest* LayerNormalQuest::create()
{
    LayerNormalQuest* pRet = new(std::nothrow) LayerNormalQuest();
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

LayerNormalQuest::LayerNormalQuest(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),

_levelAdd(1),
_goldps("0"),

// widget
_uiContainerInfo(nullptr),
_uiContainerList(nullptr),

_uiGoldPS(nullptr)

{
    
}

LayerNormalQuest::~LayerNormalQuest(void)
{
    
}

bool LayerNormalQuest::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    
    // init
    initVar();
    initUi();

    //
    RefreshManager::getInstance()->addUI(E_REFRESH::QUEST_INFO, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::QUEST_BUY, this);
    
    return true;
}

void LayerNormalQuest::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        
        this->schedule(schedule_selector(LayerNormalQuest::scheduleGoldPS), 3.0, kRepeatForever, 0);
    }
    
    //
    drawInfo();
}

#pragma mark - refresh
void LayerNormalQuest::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::QUEST_INFO || eType == E_REFRESH::QUEST_BUY )
    {
        drawInfo();
    }
}
  

#pragma mark - init
void LayerNormalQuest::initVar()
{
    
}

void LayerNormalQuest::initUi()
{

}

#pragma mark - ui : defalut
void LayerNormalQuest::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, 120));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContainerInfo);
    
    _uiContainerList = UILayoutQuestCell::create();
    _uiContainerList->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height));
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContainerList);
}

void LayerNormalQuest::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    //
    int listLevelAdd[] = {C_MAX_QUEST, 100, 10, 1};
    std::string listLevelText[] = {"+Lv.Max", "+Lv.100", "+Lv.10", "+Lv.1"};
     
    auto uiLevelAdds = ui::Layout::create();
    uiLevelAdds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLevelAdds->setPosition(Vec2(size.width / 2, size.height));
    uiLevelAdds->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLevelAdds->setName("UI_LEVELADDS");
    layout->addChild(uiLevelAdds);
    for ( int i = 0; i < sizeof(listLevelAdd) / sizeof(int); i++ )
    {
        auto levelAdd = listLevelAdd[i];
        auto levelName = listLevelText[i];
        
        auto uiBtn = ui::Button::create("Assets/ui_common/quest_btn_on.png");
        uiBtn->addClickEventListener(CC_CALLBACK_1(LayerNormalQuest::onClickLevelAdd, this));
        uiBtn->setTag(levelAdd);
        uiLevelAdds->addChild(uiBtn);
        
        auto uiText = ui::CText::create(levelName, GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2));
        uiText->setName("UI_TEXT");
        uiBtn->addChild(uiText);
    }
    
    auto sizeLevelAdds = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLevelAdds);
    uiLevelAdds->setContentSize(sizeLevelAdds);
    uiLevelAdds->requestDoLayout();
    
    
    // GoldPS
    {
        auto uiIcon = Sprite::create("Assets/icon/menu_coin.png");
        uiIcon->setPosition(Vec2(40, 23));
        layout->addChild(uiIcon);

        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            ((ui::CText*)sender)->setString(_goldps);
        });

        _uiGoldPS = ui::CText::create(_goldps, GAME_FONT, 24);
        _uiGoldPS->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _uiGoldPS->setPosition(Vec2(70, 23));
        _uiGoldPS->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        layout->addChild(_uiGoldPS);
    }
}

#pragma mark - ui : draw
void LayerNormalQuest::drawInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
   
    //
    if ( auto uiLevelAdds = layout->getChildByName<ui::Layout*>("UI_LEVELADDS"); uiLevelAdds != nullptr )
    {
        auto listChild = uiLevelAdds->getChildren();
        for ( ssize_t i = 0; i < listChild.size(); i++ )
        {
            auto uiBtn = dynamic_cast<ui::Button *>(listChild.at(i));
            if ( _levelAdd == uiBtn->getTag() )
            {
                uiBtn->loadTextureNormal("Assets/ui_common/quest_btn_on.png");
            }
            else
            {
                uiBtn->loadTextureNormal("Assets/ui_common/quest_btn_off.png");
            }
            
            auto uiText = utils::findChild<ui::CText*>(uiBtn, "UI_TEXT");
            if ( _levelAdd == uiBtn->getTag() )
            {
                uiText->setTextColor(Color4B::WHITE);
            }
            else
            {
                uiText->setTextColor(Color4B(115, 116, 118, 255));
            }
        }
    }
    
    //
    scheduleGoldPS(0);
    _uiGoldPS->setString(_goldps);
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerNormalQuest::onClickLevelAdd(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int levelAdd = uiBtn->getTag();
    if ( levelAdd == _levelAdd )
    {
        return;
    }

    //
    _levelAdd = levelAdd;
    
    //
    drawInfo();
    
    //
    auto uiList = dynamic_cast<UILayoutQuestCell*>(_uiContainerList);
    if ( uiList != nullptr )
    {
        uiList->setLevelAdd(_levelAdd);
    }
}
 

#pragma mark - callback

#pragma mark - schedule
void LayerNormalQuest::scheduleGoldPS(float dt)
{
    std::string goldPS = UserInfoManager::getInstance()->getSecondEarnGoldNormal();
    goldPS = MafUtils::convertNumberToShort(goldPS);
    
    _goldps = GAME_TEXTFORMAT("t_ui_quest_gold", goldPS.c_str());
}

#pragma mark - event
void LayerNormalQuest::onListMove()
{
    auto uiList = dynamic_cast<UILayoutQuestCell*>(_uiContainerList);
    if ( uiList != nullptr )
    {
        uiList->onListMove();
    }
}
