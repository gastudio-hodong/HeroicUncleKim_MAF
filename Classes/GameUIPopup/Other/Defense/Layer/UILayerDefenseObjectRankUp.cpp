//
//  UILayerDefenseRankUp.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/07.
//

#include "UILayerDefenseObjectRankUp.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerDefenseObjectRankUp* UILayerDefenseObjectRankUp::create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    UILayerDefenseObjectRankUp* pRet = new(std::nothrow) UILayerDefenseObjectRankUp();
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

UILayerDefenseObjectRankUp::UILayerDefenseObjectRankUp(void):
_nMakeCount(0)
,_type(E_DF_Trench_SettingType::NONE)
,_idx(0)
,_nMaxMakeCount(0)
,_objRankUpSetting(nullptr)
,_lbNowCnt(nullptr)
,_lbNextCnt(nullptr)
,_sprEffect(nullptr)
,_characterData(nullptr)
,_weaponData(nullptr)
,_lbRankUp(nullptr)
{
}

UILayerDefenseObjectRankUp::~UILayerDefenseObjectRankUp(void)
{
    DefenseManager::getInstance()->_onServerRankup.Clear(this);
    _onHide.Clear();
}

bool UILayerDefenseObjectRankUp::init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    if ( !LayerColor::init())
    {
        return false;
    }
    
    setContentSize(size);
    _type = type;
    _idx = idx;
    
    DefenseManager::getInstance()->_onServerRankup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(UILayerDefenseObjectRankUp::callbackRankup, this), this);
    
    _characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    _weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    
    calculateMaxMakeCount();
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerDefenseObjectRankUp::initVar()
{
}

void UILayerDefenseObjectRankUp::initUi()
{
    //
    uiMain();
}

#pragma mark - ui
void UILayerDefenseObjectRankUp::uiMain()
{
    removeAllChildren();
    
    auto sprArrow = Sprite::create("Assets/ui_common/arrow_R02.png");
    sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprArrow->setPosition(Vec2(getContentSize()/2) + Vec2::UP * 70);
    addChild(sprArrow);
    
    auto nextCharacterData = DefenseManager::getInstance()->getCharacterData(_idx+1);
    auto nextWeaponData = DefenseManager::getInstance()->getWeaponData(_idx+1);
    
    std::string strBox = "Assets/ui/defense/Ui_list_ch_smelting_box%02d.png";
    std::string strBoxNext = "Assets/ui/defense/Ui_list_ch_smelting_box%02d.png";
    std::string strNextName = "";
    std::string strName = "";
    std::string strIconPath = "";
    std::string strNextIconPath = "";
    bool isHave = false;
    bool isNextHave = false;
    bool isNextExist = false;
    int nextCount = 0;
    int currentCount = 0;
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER :
        {
            if(nextCharacterData == nullptr)
            {
                strBoxNext = MafUtils::format(strBoxNext.c_str(), DefenseManager::getInstance()->getCharacterDataCount()/5);
            }
            else
            {
                isNextExist = true;
                isNextHave = nextCharacterData->isHave();
                strBoxNext = MafUtils::format(strBoxNext.c_str(), nextCharacterData->getGrade());
                strNextIconPath = nextCharacterData->getResourcePath();
                strNextName = "";
                nextCount = nextCharacterData->getCurrentCount();
            }
            strBox = MafUtils::format(strBox.c_str(), _characterData->getGrade());
            isHave = _characterData->isHave();
            strName = "";
            strIconPath = _characterData->getResourcePath();
            currentCount = _characterData->getCurrentCount();
            
        }break;
        case E_DF_Trench_SettingType::WEAPON :
        {
            if(nextWeaponData == nullptr)
            {
                strBoxNext = MafUtils::format(strBoxNext.c_str(), DefenseManager::getInstance()->getWeaponDataCount()/5);
            }
            else
            {
                isNextExist = true;
                isNextHave = nextWeaponData->isHave();
                strBoxNext = MafUtils::format(strBoxNext.c_str(), nextWeaponData->getGrade());
                strNextIconPath = nextWeaponData->getResourceIconPath();
                strNextName = "";
                nextCount = nextWeaponData->getCurrentCount();
            }
            strBox = MafUtils::format(strBox.c_str(), _weaponData->getGrade());
            isHave = _weaponData->isHave();
            strName = "";
            strIconPath = _weaponData->getResourceIconPath();
            currentCount = _weaponData->getCurrentCount();
            
        }break;
        default: break;
    }
    
    auto sprBox = Sprite::create(strBox);
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprBox->setPosition(80, getContentSize().height/2 + 70);
    addChild(sprBox);
    {
        auto lbName = Label::createWithTTF(strName, GAME_FONT, 24);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbName->setPosition(sprBox->getContentSize().width/2, 10);
        lbName->setDimensions(sprBox->getContentSize().width - 15, lbName->getContentSize().height);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        lbName->setOverflow(Label::Overflow::SHRINK);
        lbName->enableBold();
        sprBox->addChild(lbName);
        
        auto lyIcon = LayerColor::create();
        lyIcon->setIgnoreAnchorPointForPosition(false);
        lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lyIcon->setContentSize(sprBox->getContentSize() - Size(20));
        
        auto clipper = ClippingNode::create(lyIcon);
        clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        clipper->setPosition(sprBox->getContentSize()/2);
        clipper->setIgnoreAnchorPointForPosition(false);
        sprBox->addChild(clipper);
        
        auto sprIcon = Sprite::create(strIconPath);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(clipper->getContentSize()/2);
        clipper->addChild(sprIcon);
        
        if(!isHave)
        {
            sprIcon->setColor(Color3B(15, 15, 15));
            lbName->setString("???");
            sprBox->setColor(Color3B(158,158,158));
        }
        
        auto sprCountBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
        sprCountBox->setContentSize(Size(140, 43));
        sprCountBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCountBox->setPosition(sprBox->getPosition().x + sprBox->getContentSize().width/2, 140);
        addChild(sprCountBox);
        
        std::string strCount = "%d{#FF1E1E:(-%d)}";
        
        strCount = MafUtils::format(strCount.c_str(), currentCount, _nMakeCount * _objRankUpSetting->getValueToInt());
        
        _lbNowCnt = ui::CText::create("", GAME_FONT, 23);
        _lbNowCnt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _lbNowCnt->setPosition(sprCountBox->getContentSize()/2);
        _lbNowCnt->setTextAreaSize(Size(sprCountBox->getContentSize().width - 5, sprCountBox->getContentSize().height));
        _lbNowCnt->setTextVerticalAlignment(TextVAlignment::CENTER);
        _lbNowCnt->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        _lbNowCnt->setTextOverflow(Label::Overflow::SHRINK);
        _lbNowCnt->enableBold();
        sprCountBox->addChild(_lbNowCnt);
        
        _lbNowCnt->setString(strCount);
    }
    
    
    auto sprBoxNext = Sprite::create(strBoxNext);
    sprBoxNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprBoxNext->setPosition(getContentSize().width - 60, getContentSize().height/2 + 75);
    sprBoxNext->getTexture()->setTexParameters(texParams);
    sprBoxNext->setScale(1.1f);
    addChild(sprBoxNext);
    {
        if(isNextExist)
        {
            auto lbName = Label::createWithTTF(strNextName, GAME_FONT, 24);
            lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbName->setPosition(sprBox->getContentSize().width/2, 10);
            lbName->setDimensions(sprBoxNext->getContentSize().width - 15, lbName->getContentSize().height);
            lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbName->setOverflow(Label::Overflow::SHRINK);
            lbName->enableBold();
            sprBoxNext->addChild(lbName);
            
            auto lyIcon = LayerColor::create();
            lyIcon->setIgnoreAnchorPointForPosition(false);
            lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lyIcon->setContentSize(sprBoxNext->getContentSize() - Size(20));
            
            auto clipper = ClippingNode::create(lyIcon);
            clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            clipper->setPosition(sprBoxNext->getContentSize()/2);
            clipper->setIgnoreAnchorPointForPosition(false);
            sprBoxNext->addChild(clipper);
            
            auto sprIcon = Sprite::create(strNextIconPath);
            sprIcon->getTexture()->setTexParameters(texParams);
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(clipper->getContentSize()/2);
            clipper->addChild(sprIcon);
            
            auto sprCountBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
            sprCountBox->setContentSize(Size(140, 43));
            sprCountBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCountBox->setPosition(sprBoxNext->getPosition().x - sprBoxNext->getContentSize().width * sprBoxNext->getScale() / 2, 140);
            addChild(sprCountBox);
            
            std::string strCount = "%d{#33FF00:(+%d)}";
            
            strCount = MafUtils::format(strCount.c_str(), nextCount, _nMakeCount);
            _lbNextCnt = ui::CText::create("", GAME_FONT, 23);
            _lbNextCnt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _lbNextCnt->setPosition(sprCountBox->getContentSize()/2);
            _lbNextCnt->setTextAreaSize(Size(sprCountBox->getContentSize().width - 5, sprCountBox->getContentSize().height));
            _lbNextCnt->setTextVerticalAlignment(TextVAlignment::CENTER);
            _lbNextCnt->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            _lbNextCnt->setTextOverflow(Label::Overflow::SHRINK);
            _lbNextCnt->enableBold();
            sprCountBox->addChild(_lbNextCnt);
            
            _lbNextCnt->setString(strCount);
            
            if(!isNextHave)
            {
                sprIcon->setColor(Color3B(15, 15, 15));
                lbName->setString("???");
                sprBoxNext->setColor(Color3B(158,158,158));
            }
        }
        else
        {
            auto lbText = Label::createWithTTF("CONTINUED...", GAME_FONT, 22);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(sprBoxNext->getContentSize()/2);
            lbText->setDimensions(sprBoxNext->getContentSize().width - 20, sprBoxNext->getContentSize().height - 5);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setColor(Color3B(227,199,110));
            lbText->setOverflow(Label::Overflow::SHRINK);
            
            sprBoxNext->setColor(Color3B(158,158,158));
            sprBoxNext->addChild(lbText);
        }
        
        _sprEffect = Sprite::create("Assets/ui/defense/Ui_list_ch_smelting_box_edge.png");
        _sprEffect->getTexture()->setTexParameters(texParams);
        _sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _sprEffect->setPosition(sprBoxNext->getContentSize().width/2, sprBoxNext->getContentSize().height + 5);
        _sprEffect->setVisible(false);
        sprBoxNext->addChild(_sprEffect);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
        _sprEffect->runAction(RepeatForever::create(sequience));
        
        if(isNextExist && _nMakeCount >= 1)
            _sprEffect->setVisible(true);
    }
    //
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnRankUp->setPosition(Vec2(getContentSize().width/2, 25));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(UILayerDefenseObjectRankUp::onClickRankUp, this));
    btnRankUp->setTitleText(GAME_TEXTFORMAT("t_ui_spirit_rank_up_count_bt", _nMakeCount));
    btnRankUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnRankUp->getTitleLabel()->setDimensions(btnRankUp->getContentSize().width, btnRankUp->getContentSize().height);
    btnRankUp->setTitleFontSize(30);
    addChild(btnRankUp);
    
    _lbRankUp = btnRankUp->getTitleLabel();
    
    auto btnPlus = ui::Button::create("Assets/ui_common/btn_updown_1_1.png");
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(btnRankUp->getPosition() + Vec2::UP * btnRankUp->getContentSize().height/2 + Vec2::RIGHT * (btnRankUp->getContentSize().width/2 + 15));
    btnPlus->setZoomScale(0.05f);
    btnPlus->setName("PLUS");
    btnPlus->addClickEventListener(CC_CALLBACK_1(UILayerDefenseObjectRankUp::onClickFluctuation, this));
    addChild(btnPlus);
    
    auto btnMinus = ui::Button::create("Assets/ui_common/btn_updown_1_2.png");
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(btnRankUp->getPosition() + Vec2::UP * btnRankUp->getContentSize().height/2 + Vec2::LEFT * (btnRankUp->getContentSize().width/2 + 15));
    btnMinus->setZoomScale(0.05f);
    btnMinus->setName("MINUS");
    btnMinus->addClickEventListener(CC_CALLBACK_1(UILayerDefenseObjectRankUp::onClickFluctuation, this));
    addChild(btnMinus);
    
    
    
    if(!isHave || !isNextExist)
    {
        btnPlus->setEnabled(false);
        btnMinus->setEnabled(false);
        btnRankUp->setEnabled(false);
    }
}

#pragma mark -utils
void UILayerDefenseObjectRankUp::onClickRankUp(Ref* sender)
{
    if(_nMakeCount <= 0)
    {
        return;
    }
    
    DefenseManager::getInstance()->requestRankup(_type, _idx, _nMakeCount);
}

void UILayerDefenseObjectRankUp::onClickFluctuation(Ref* sender)
{
    auto node = (Node*)sender;
    
    
    if(node->getName().compare("PLUS") == 0)
    {
        _nMakeCount++;
        
        if(_nMakeCount > _nMaxMakeCount)
            _nMakeCount = 0;
    }
    else if(node->getName().compare("MINUS") == 0)
    {
        _nMakeCount--;
        
        if(_nMakeCount < 0)
            _nMakeCount = _nMaxMakeCount;
    }
    uiMain();
}
void UILayerDefenseObjectRankUp::calculateMaxMakeCount()
{
    int currentCount = 0;
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER : currentCount = _characterData->getCurrentCount(); break;
        case E_DF_Trench_SettingType::WEAPON : currentCount = _weaponData->getCurrentCount(); break;
        default: break;
    }
    _objRankUpSetting = DefenseManager::getInstance()->getInfoSetting("defense_promotion");
    _nMakeCount = _nMaxMakeCount = currentCount / _objRankUpSetting->getValueToInt();
}
#pragma mark -callback
void UILayerDefenseObjectRankUp::callbackRankup(bool bResult, int nResult)
{
    //
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
    calculateMaxMakeCount();
    uiMain();
}

void UILayerDefenseObjectRankUp::onChangeObject(int idx)
{
    _idx = idx;
    _characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    _weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    
    calculateMaxMakeCount();
    
    uiMain();
}
