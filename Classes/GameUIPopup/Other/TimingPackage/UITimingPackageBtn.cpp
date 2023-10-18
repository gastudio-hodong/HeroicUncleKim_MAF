//
//  UITimingPackageBtn.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#include "UITimingPackageBtn.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"

#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

UITimingPackageBtn* UITimingPackageBtn::create()
{
    UITimingPackageBtn* pRet = new(std::nothrow) UITimingPackageBtn();
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

UITimingPackageBtn::UITimingPackageBtn():
_isActive(false),
_isVisible(false),

_infoTimingPackage(nullptr),

_callbackReload(nullptr),
_callbackShowPopup(nullptr),

//
_itemIcon(nullptr),
_uiCondition(nullptr),
_uiConditionType(nullptr),

_reddot(nullptr)
{
}

UITimingPackageBtn::~UITimingPackageBtn()
{
    deActiveUITimingPackageBtn();
    TimingPackageManager::getInstance()->unSubEventTimingPackage(this);
}

bool UITimingPackageBtn::init()
{
    if (!Node::init()) {
        return false;
    }
    setContentSize(Size(72, 100));
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    _itemIcon = MafNode::MafMenuItemSprite::create(nullptr, nullptr, CC_CALLBACK_1(UITimingPackageBtn::onClick, this));
    _itemIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _itemIcon->setPosition(getContentSize()/2);
    menu->addChild(_itemIcon);
    
    _uiCondition = Label::createWithTTF("", GAME_FONT, 20);
    _uiCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiCondition->setPosition(_itemIcon->getPosition() + Vec2::DOWN * 25);
    _uiCondition->setDimensions(getContentSize().width + 20, _uiCondition->getContentSize().height);
    _uiCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _uiCondition->setOverflow(Label::Overflow::SHRINK);
    _uiCondition->enableOutline(Color4B::BLACK, 1);
    _uiCondition->setLineSpacing(-10);
    addChild(_uiCondition);
    
    _uiConditionType = ui::ImageView::create("Assets/ui_common/icon_timing_box_1.png");
    _uiConditionType->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiConditionType->setPosition(Vec2(0, getContentSize().height - 50));
    addChild(_uiConditionType);
    {
        auto uiText = ui::CText::create("", GAME_FONT, 18);
        uiText->setPosition(Vec2(_uiConditionType->getContentSize().width / 2, _uiConditionType->getContentSize().height / 2 + 5));
        uiText->setTextColor(Color4B::YELLOW);
        uiText->setName("UI_TEXT");
        _uiConditionType->addChild(uiText);
    }
    
    _reddot = UIReddot::create(E_REDDOT_TYPE::NOMAL);
    _reddot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reddot->setPosition(getContentSize().width - 25, getContentSize().height - 50);
    _reddot->setScale(0.7);
    _reddot->setName("UIReddot");
    _reddot->setVisible(false);
    addChild(_reddot);
    
    
    
    setVisibleTimingPackageBtn(false);
    
    return true;
}

void UITimingPackageBtn::update(float dt)
{
    if(_infoTimingPackage == nullptr)
    {
        unscheduleUpdate();
        return;
    }
    
    if( _infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE)
    {
        _infoTimingPackage->setEndTimer(_infoTimingPackage->getEndTimer() - dt);
        
        if(_infoTimingPackage->getEndTimer() <= 0)
        {
            unscheduleUpdate();
            
            ValueMap listData;
            listData["KIND"] = Value("END");
            TimingPackageManager::getInstance()->invokeEventTimingPackage(listData);
            return;
        }
    }
    
    setConditionText();
}

void UITimingPackageBtn::onNotifyTimingPackge(cocos2d::ValueMap listData)
{
    if(!_isActive)
        return;
    
    std::string strKind = "NONE";
    
    setConditionText();
    
    auto iter = listData.find("KIND");
    if(iter != listData.end())
    {
        strKind = iter->second.asString();
    }
    
    if(strKind.compare("END") == 0)
    {
        if(TimingPackageManager::getInstance()->isEndTiming(_infoTimingPackage))
        {
            _infoTimingPackage->setIsActivate(false);
            deActiveUITimingPackageBtn();
        }
    }
    else if(strKind.compare("REWARD") == 0)
    {
        if(TimingPackageManager::getInstance()->isRewardTiming(_infoTimingPackage))
        {
            TimingPackageManager::getInstance()->requestReward(_infoTimingPackage);
            _infoTimingPackage->setIsActivate(false);
            deActiveUITimingPackageBtn();
            
            if(_callbackReload != nullptr)
                _callbackReload();
        }
    }
    else if(strKind.compare("PURCHASE") == 0)
    {
        if(_infoTimingPackage->isPurchase() == true)
        {
            _infoTimingPackage->setIsActivate(false);
            deActiveUITimingPackageBtn();
            
            if(_callbackReload != nullptr)
                _callbackReload();
        }
    }
}

void UITimingPackageBtn::setInfoTimingPackage(InfoTimingPackage* infoTimingPackage)
{
    if(_infoTimingPackage != nullptr)
        _infoTimingPackage->setIsBtnActive(false);
    
    _infoTimingPackage = infoTimingPackage;
    
    if((_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE && _infoTimingPackage->isActivate() == false) ||
       (_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::TERMS_FREE && _infoTimingPackage->isActivate() == false))
    {
        if( AccountManager::getInstance()->getUserIdx() != 0)
        {
            this->retain();
            TimingPackageManager::getInstance()->requestTimingPackageActivate(CC_CALLBACK_2(UITimingPackageBtn::callbackActiveTimingPackage, this), _infoTimingPackage);
        }
        return;
    }
    else if(_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::FREE)
    {
        _infoTimingPackage->setIsActivate(true);
    }
    
    this->retain();
    callbackActiveTimingPackage(true, 1);
}

void UITimingPackageBtn::deActiveUITimingPackageBtn()
{
    unscheduleUpdate();
    if(_infoTimingPackage != nullptr)
    {
        _infoTimingPackage->setIsBtnActive(false);
        _infoTimingPackage = nullptr;
    }
    _isActive = false;
    setVisible(false);
}

void UITimingPackageBtn::onClick(cocos2d::Ref* pSender)
{
    if(_isVisible == false)
        return;
    
    showPopup();
    if(_reddot != nullptr)
        _reddot->removeFromParent();
    _reddot = nullptr;
}

void UITimingPackageBtn::showPopup()
{
    TimingPackageManager::getInstance()->showTimingPackagePopup(_infoTimingPackage);
}

void UITimingPackageBtn::setConditionText()
{
    std::string strCondition = "";
    int nCurrentValue = 0;
    if(_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE)
    {
        auto time = _infoTimingPackage->getEndTimer();
        
        int hour = (int)time / 3600;
        int min = ((int)time % 3600) / 60;
        
        if(hour < 0)
            hour = 0;
        if(min < 0)
            min = 0;
        
        strCondition = "%02d:%02d";
        strCondition = MafUtils::format(strCondition.c_str(), hour, min);
    }
    else
    {
        switch (_infoTimingPackage->getEndConditionType()) {
            case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
            {
                nCurrentValue = UserInfoManager::getInstance()->getHighFloor(DG_NORMAL);
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
            {
                nCurrentValue = PrisonManager::getInstance()->getDungeonCount();
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
            {
                nCurrentValue = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER);
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, _infoTimingPackage->getEndConditionTypeParams());
                if ( objArtifact != nullptr )
                {
                    nCurrentValue = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx());
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, _infoTimingPackage->getEndConditionTypeParams());
                if ( objArtifact != nullptr )
                {
                    nCurrentValue = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx());
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, _infoTimingPackage->getEndConditionTypeParams());
                if ( objArtifact != nullptr )
                {
                    nCurrentValue = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx());
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, _infoTimingPackage->getEndConditionTypeParams());
                if ( objArtifact != nullptr )
                {
                    nCurrentValue = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx());
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, _infoTimingPackage->getEndConditionTypeParams());
                if ( objArtifact != nullptr )
                {
                    nCurrentValue = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx());
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
            {
                auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
                if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                {
                    nCurrentValue = objArtifact->getCurLv();
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
            {
                auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
                if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                {
                    nCurrentValue = objArtifact->getCurLv();
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
            {
                auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
                if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                {
                    nCurrentValue = objArtifact->getCurLv();
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
            {
                auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
                if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                {
                    nCurrentValue = objArtifact->getCurLv();
                }
                break;
            }
            case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
            {
                auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
                if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                {
                    nCurrentValue = objArtifact->getCurLv();
                }
                break;
            }
                
            default:
                break;
        }
        strCondition = MafUtils::format("%d/%d", nCurrentValue, _infoTimingPackage->getCompleteValue());
    }
    
    _uiCondition->setString(strCondition);
}

#pragma mark callback
void UITimingPackageBtn::callbackActiveTimingPackage(bool bResult, int nResult)
{
    this->release();
    
    if(!bResult)
        return;
    
    if(nResult != 1)
        return;
    
    if ( _infoTimingPackage == nullptr )
    {
        return;
    }
    
    _infoTimingPackage->setIsBtnActive(true);
    _isActive = true;
    
    TimingPackageManager::getInstance()->subEventTimingPackage(new MafFunction<void(cocos2d::ValueMap )>(CC_CALLBACK_1(UITimingPackageBtn::onNotifyTimingPackge, this), this));
    
    _itemIcon->setNormalImage(Sprite::create(_infoTimingPackage->getIconPath()));
    if ( auto uiText = _uiConditionType->getChildByName<ui::CText*>("UI_TEXT"); uiText != nullptr )
    {
        uiText->setString(MafUtils::format("%dth", _infoTimingPackage->getStartConditionTypeParams()));
    }
    
    setConditionText();
    
    if(_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE)
        scheduleUpdate();
    
    setVisibleTimingPackageBtn(true);
    
    if(!_infoTimingPackage->isOpenPopup())
    {
        if(_callbackShowPopup != nullptr)
            _callbackShowPopup(_infoTimingPackage);
        if(_reddot != nullptr)
            _reddot->setVisible(true);
    }
    _infoTimingPackage->setIsOpenPopup(true);
    
}

void UITimingPackageBtn::callbackReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
        return;
    
    if(nResult != 1)
        return;
    
    showPopup();
    deActiveUITimingPackageBtn();
}


void UITimingPackageBtn::setCallbackReload(const std::function<void()>& pCallback)
{
    _callbackReload = pCallback;
}

void UITimingPackageBtn::setCallbackShowPopup(const std::function<void(InfoTimingPackage*)>& pCallback)
{
    _callbackShowPopup = pCallback;
}

InfoTimingPackage* UITimingPackageBtn::getTimingPackageInfo()
{
    return _infoTimingPackage;
}

const bool UITimingPackageBtn::isPurchase() const
{
    bool result = false;
    
    if ( _infoTimingPackage != nullptr )
    {
        if ( _infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE )
            result = true;
    }
    
    return result;
}

void UITimingPackageBtn::setVisibleTimingPackageBtn(bool isVisible)
{
    _isVisible = isVisible;
    
    _itemIcon->setVisible(_isVisible);
    _uiCondition->setVisible(_isVisible);
    
    _uiConditionType->setVisible(false);
    if ( isVisible == true && _infoTimingPackage != nullptr && _infoTimingPackage->getStartConditionTypeParams() != 0 )
    {
        _uiConditionType->setVisible(_isVisible);
    }
    
    if(_reddot != nullptr)
        _reddot->setVisible(_isVisible);
}
