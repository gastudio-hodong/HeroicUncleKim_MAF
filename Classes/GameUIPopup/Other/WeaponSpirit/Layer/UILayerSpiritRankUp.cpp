//
//  UILayerSpiritRankUp.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#include "UILayerSpiritRankUp.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

UILayerSpiritRankUp* UILayerSpiritRankUp::create(cocos2d::Size size, InfoWeaponSpirit* info)
{
    UILayerSpiritRankUp* pRet = new(std::nothrow) UILayerSpiritRankUp();
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

UILayerSpiritRankUp::UILayerSpiritRankUp(void):
_nMakeCount(0)
,_nMaxMakeCount(0)
,_lyUI(nullptr)
,_infoSpirit(nullptr)
,_infoNextSpirit(nullptr)
,_objRankUpSetting(nullptr)
,_lbNowCnt(nullptr)
,_lbNextCnt(nullptr)
,_sprEffect(nullptr)
,_lbRankUp(nullptr)
{
}

UILayerSpiritRankUp::~UILayerSpiritRankUp(void)
{
}

bool UILayerSpiritRankUp::init(cocos2d::Size size, InfoWeaponSpirit* info)
{
    if ( !UILayerSpiritParent::init())
    {
        return false;
    }
    
    setContentSize(size);
    _infoSpirit = info;
    _infoNextSpirit = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(_infoSpirit->getIdx() + 1);
    _objRankUpSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_rank_up_cost");
    //
    
    _nMakeCount = _nMaxMakeCount = _infoSpirit->getCurrentCount() / _objRankUpSetting->getValueToInt();
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerSpiritRankUp::initVar()
{
}

void UILayerSpiritRankUp::initUi()
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

void UILayerSpiritRankUp::refreshUI(InfoWeaponSpirit* info)
{
    _infoSpirit = info;
    _infoNextSpirit = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(_infoSpirit->getIdx() + 1);
    _nMakeCount = _nMaxMakeCount = _infoSpirit->getCurrentCount() / _objRankUpSetting->getValueToInt();
    uiMain();
}

#pragma mark - ui
void UILayerSpiritRankUp::uiMain()
{
    _lyUI->removeAllChildren();
    
    auto sprArrow = Sprite::create("Assets/ui_common/arrow_R02.png");
    sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprArrow->setPosition(Vec2(_lyUI->getContentSize()/2) + Vec2::UP * 70);
    _lyUI->addChild(sprArrow);
    
    std::string strSpiritBoxMaterial = "Assets/ui/spirit/spirit_box1_%d.png";
    std::string strSpiritNameBoxMaterial = "Assets/ui/spirit/spirit_box2_%d.png";
    std::string strSpiritBoxNextBox = "Assets/ui/spirit/spirit_box1_%d.png";
    std::string strSpiritBoxNextNameBox = "Assets/ui/spirit/spirit_box2_%d.png";
    
    if(_infoNextSpirit != nullptr)
    {
        if(!_infoNextSpirit->isHas())
        {
            strSpiritBoxNextBox = "Assets/ui/spirit/spirit_box3_%d.png";
            strSpiritBoxNextNameBox = "Assets/ui/spirit/spirit_box4_%d.png";
        }
        
        strSpiritBoxNextBox = MafUtils::format(strSpiritBoxNextBox.c_str(), _infoNextSpirit->getGrade());
        strSpiritBoxNextNameBox = MafUtils::format(strSpiritBoxNextNameBox.c_str(), _infoNextSpirit->getGrade());
    }
    else
    {
        strSpiritBoxNextBox = "Assets/ui/spirit/spirit_box3_6.png";
        strSpiritBoxNextNameBox = "Assets/ui/spirit/spirit_box4_6.png";
    }
    if(!_infoSpirit->isHas())
    {
        strSpiritBoxMaterial = "Assets/ui/spirit/spirit_box3_%d.png";
        strSpiritNameBoxMaterial = "Assets/ui/spirit/spirit_box4_%d.png";
    }
    
    auto sprSpiritBoxMaterial = Sprite::create(MafUtils::format(strSpiritBoxMaterial.c_str(), _infoSpirit->getGrade()));
    sprSpiritBoxMaterial->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprSpiritBoxMaterial->setPosition(80, _lyUI->getContentSize().height/2 + 110);
    _lyUI->addChild(sprSpiritBoxMaterial);
    {
        
        auto sprNameBox = Sprite::create(MafUtils::format(strSpiritNameBoxMaterial.c_str(), _infoSpirit->getGrade()));
        sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprNameBox->setPosition(sprSpiritBoxMaterial->getContentSize().width/2, 0);
        sprSpiritBoxMaterial->addChild(sprNameBox);
        
        auto lbName = Label::createWithTTF(_infoSpirit->getText(), GAME_FONT, 24);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbName->setPosition(sprNameBox->getContentSize()/2);
        lbName->setDimensions(sprNameBox->getContentSize().width - 15, lbName->getContentSize().height);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        lbName->enableBold();
        sprNameBox->addChild(lbName);
        
        auto sprSpirit = Sprite::create(_infoSpirit->getResourcePath());
        sprSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprSpirit->setPosition(sprSpiritBoxMaterial->getContentSize()/2);
        sprSpiritBoxMaterial->addChild(sprSpirit);
        
        if(!_infoSpirit->isHas())
        {
            sprSpirit->setColor(Color3B(15, 15, 15));
            lbName->setString("???");
        }
        
        auto sprCountBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
        sprCountBox->setContentSize(Size(140, 43));
        sprCountBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCountBox->setPosition(sprSpiritBoxMaterial->getPosition().x + sprSpiritBoxMaterial->getContentSize().width/2, 140);
        _lyUI->addChild(sprCountBox);
        
        std::string strCount = "%d{#FF1E1E:(-%d)}";
        
        strCount = MafUtils::format(strCount.c_str(), _infoSpirit->getCurrentCount(), _nMakeCount * _objRankUpSetting->getValueToInt());
        
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
    
    
    auto sprSpiritBoxNext = Sprite::create(strSpiritBoxNextBox);
    sprSpiritBoxNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprSpiritBoxNext->setPosition(_lyUI->getContentSize().width - 60, _lyUI->getContentSize().height/2 + 105);
    sprSpiritBoxNext->getTexture()->setTexParameters(texParams);
    sprSpiritBoxNext->setScale(1.1f);
    _lyUI->addChild(sprSpiritBoxNext);
    {
        auto sprNameBox = Sprite::create(strSpiritBoxNextNameBox);
        sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprNameBox->setPosition(sprSpiritBoxNext->getContentSize().width/2, 0);
        sprSpiritBoxNext->addChild(sprNameBox);
        
        if(_infoNextSpirit != nullptr)
        {
            auto lbName = Label::createWithTTF(_infoNextSpirit->getText(), GAME_FONT, 24);
            lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbName->setPosition(sprNameBox->getContentSize()/2);
            lbName->setDimensions(sprNameBox->getContentSize().width - 15, lbName->getContentSize().height);
            lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbName->setOverflow(Label::Overflow::SHRINK);
            lbName->enableBold();
            sprNameBox->addChild(lbName);
            
            auto sprSpirit = Sprite::create(_infoNextSpirit->getResourcePath());
            sprSpirit->getTexture()->setTexParameters(texParams);
            sprSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprSpirit->setPosition(sprSpiritBoxNext->getContentSize()/2);
            sprSpiritBoxNext->addChild(sprSpirit);
            
            auto sprCountBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
            sprCountBox->setContentSize(Size(140, 43));
            sprCountBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCountBox->setPosition(sprSpiritBoxNext->getPosition().x - sprSpiritBoxNext->getContentSize().width * sprSpiritBoxNext->getScale() / 2, 140);
            _lyUI->addChild(sprCountBox);
            
            std::string strCount = "%d{#33FF00:(+%d)}";
            
            strCount = MafUtils::format(strCount.c_str(), _infoNextSpirit->getCurrentCount(), _nMakeCount);
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
            
            if(!_infoNextSpirit->isHas())
            {
                sprSpirit->setColor(Color3B(15, 15, 15));
                lbName->setString("???");
            }
        }
        else
        {
            auto lbText = Label::createWithTTF("CONTINUED...", GAME_FONT, 22);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(sprSpiritBoxNext->getContentSize()/2);
            lbText->setDimensions(sprSpiritBoxNext->getContentSize().width - 20, sprSpiritBoxNext->getContentSize().height - 5);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setColor(Color3B(227,199,110));
            lbText->setOverflow(Label::Overflow::SHRINK);
            
            sprSpiritBoxNext->addChild(lbText);
        }
        
        _sprEffect = Sprite::create("Assets/ui/spirit/spirit_box0_1.png");
        _sprEffect->getTexture()->setTexParameters(texParams);
        _sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _sprEffect->setPosition(sprSpiritBoxNext->getContentSize().width/2, sprSpiritBoxNext->getContentSize().height + 5);
        _sprEffect->setVisible(false);
        sprSpiritBoxNext->addChild(_sprEffect);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
        _sprEffect->runAction(RepeatForever::create(sequience));
        
        if(_infoNextSpirit != nullptr && _nMakeCount >= 1)
            _sprEffect->setVisible(true);
    }
    //
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnRankUp->setPosition(Vec2(_lyUI->getContentSize().width/2, 25));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(UILayerSpiritRankUp::onClickRankUp, this));
    btnRankUp->setTitleText(GAME_TEXTFORMAT("t_ui_spirit_rank_up_count_bt", _nMakeCount));
    btnRankUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnRankUp->getTitleLabel()->setDimensions(btnRankUp->getContentSize().width, btnRankUp->getContentSize().height);
    btnRankUp->setTitleFontSize(30);
    _lyUI->addChild(btnRankUp);
    
    _lbRankUp = btnRankUp->getTitleLabel();
    
    auto btnPlus = ui::Button::create("Assets/ui_common/btn_updown_1_1.png");
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(btnRankUp->getPosition() + Vec2::UP * btnRankUp->getContentSize().height/2 + Vec2::RIGHT * (btnRankUp->getContentSize().width/2 + 15));
    btnPlus->setZoomScale(0.05f);
    btnPlus->setName("PLUS");
    btnPlus->addClickEventListener(CC_CALLBACK_1(UILayerSpiritRankUp::onClickFluctuation, this));
    _lyUI->addChild(btnPlus);
    
    auto btnMinus = ui::Button::create("Assets/ui_common/btn_updown_1_2.png");
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(btnRankUp->getPosition() + Vec2::UP * btnRankUp->getContentSize().height/2 + Vec2::LEFT * (btnRankUp->getContentSize().width/2 + 15));
    btnMinus->setZoomScale(0.05f);
    btnMinus->setName("MINUS");
    btnMinus->addClickEventListener(CC_CALLBACK_1(UILayerSpiritRankUp::onClickFluctuation, this));
    _lyUI->addChild(btnMinus);
    
    
    
    if(!_infoSpirit->isHas() || _infoNextSpirit == nullptr)
    {
        btnPlus->setEnabled(false);
        btnMinus->setEnabled(false);
        btnRankUp->setEnabled(false);
    }
}

#pragma mark -utils
void UILayerSpiritRankUp::onClickRankUp(Ref* sender)
{
    if(_nMakeCount <= 0)
    {
        return;
    }
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestSpiritCompose(CC_CALLBACK_2(UILayerSpiritRankUp::callbackRankup, this),_infoSpirit->getIdx(), _nMakeCount);
    }
}

void UILayerSpiritRankUp::onClickFluctuation(Ref* sender)
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
    
    std::string strNowInfoCount = "%d{#EF1F1F:(-%d)}";
    std::string strNextInfoCount = "%d{#018123:(+%d)}";
    
    _lbRankUp->setString(GAME_TEXTFORMAT("t_ui_spirit_rank_up_count_bt", _nMakeCount));
    
    _lbNowCnt->setString(MafUtils::format(strNowInfoCount.c_str(), _infoSpirit->getCurrentCount(), _nMakeCount * _objRankUpSetting->getValueToInt()));
    if(_infoNextSpirit != nullptr && _lbNextCnt != nullptr)
    _lbNextCnt->setString(MafUtils::format(strNextInfoCount.c_str(), _infoNextSpirit->getCurrentCount(), _nMakeCount));
    
    if(_infoNextSpirit != nullptr && _nMakeCount >= 1)
        _sprEffect->setVisible(true);
    else
        _sprEffect->setVisible(false);
}

#pragma mark -callback
void UILayerSpiritRankUp::callbackRankup(bool bResult, int nResult)
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
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
}
