//
//  UILayerRaidStatus.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#include "UILayerRaidStatus.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

UILayerRaidStatus* UILayerRaidStatus::create(cocos2d::Size size)
{
    UILayerRaidStatus* pRet = new(std::nothrow) UILayerRaidStatus();
    if ( pRet && pRet->init(size) )
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

UILayerRaidStatus::UILayerRaidStatus(void):
_lyStatus(nullptr)
,_table(nullptr)
,_lbBongLevel(nullptr)
,_callbackLevelup(nullptr)
,_callbackHide(nullptr)
{
    _listInfoBongStat.clear();
    _mapLevelupIdx.clear();
}

UILayerRaidStatus::~UILayerRaidStatus(void)
{
    _listInfoBongStat.clear();
    _mapLevelupIdx.clear();
}

bool UILayerRaidStatus::init(cocos2d::Size size)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    setContentSize(size);
    
    //
    
    _listInfoBongStat = NewRaidManager::getInstance()->getInfoBongStatList();
    
    for(auto obj : _listInfoBongStat)
    {
        _mapLevelupIdx.insert(std::make_pair(obj->getIdx(), false));
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}


#pragma mark - table
void UILayerRaidStatus::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerRaidStatus::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 105);
}

extension::TableViewCell* UILayerRaidStatus::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    auto infoStatus = _listInfoBongStat.at(idx);
    
    if(infoStatus == nullptr)
        return cell;
    
    auto iterLevelup = _mapLevelupIdx.find((int)infoStatus->getIdx());
    bool isLevelup = false;
    
    if(iterLevelup != _mapLevelupIdx.end())
        isLevelup = iterLevelup->second;
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_1.png");
    sprBg->setContentSize(Size(730, 102));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBg);
    
    auto sprIconBox = ui::Scale9Sprite::create("Assets/ui_common/box_02.png");
    sprIconBox->setContentSize(Size(82,82));
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(Vec2(10, cell->getContentSize().height/2));
    sprBg->addChild(sprIconBox);
    {
        std::string strPath = "Assets/ui/raid/raid_ability_01.png";
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(infoStatus->getIconPath());
        
        if (fullpath.size() != 0)
        {
            strPath = infoStatus->getIconPath();
        }
        auto sprIcon = Sprite::create(strPath);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprIconBox->getContentSize()/2);
        sprIconBox->addChild(sprIcon);
    }
    float width = sprBg->getContentSize().width - 192 - sprIconBox->getContentSize().width - 80;
    
    auto lbContent = Label::createWithTTF(infoStatus->getText(), GAME_FONT, 30);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbContent->setPosition(sprIconBox->getPositionX() + 20 + sprIconBox->getContentSize().width, sprBg->getContentSize().height - sprBg->getContentSize().height/4);
    
    if(lbContent->getContentSize().width < width)
        width = lbContent->getContentSize().width;
    
    lbContent->setDimensions(width, lbContent->getContentSize().height);
    lbContent->setContentSize(Size(width, lbContent->getContentSize().height));
    
    lbContent->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBg->addChild(lbContent);
    
    if(isLevelup)
    {
        actionBongLevelup(lbContent);
        
        if(iterLevelup != _mapLevelupIdx.end())
            iterLevelup->second = false;
    }
    
    double fEffect = infoStatus->getEffect();
    
    std::string strEffect = MafUtils::doubleToString(fEffect);
    
    MafUtils::convertNumberToShort(strEffect);
    
    if(infoStatus->getIdx() != 1)
    {
        strEffect += "%";
    }
    
    auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 30);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbEffect->setPosition(lbContent->getPositionX(), sprBg->getContentSize().height/3);
    lbEffect->setDimensions(sprBg->getContentSize().width - 192 - sprIconBox->getContentSize().width - 40, lbEffect->getContentSize().height);
    lbEffect->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbEffect->setColor(Color3B(128,128,128));
    sprBg->addChild(lbEffect);
    
    std::string path = "Assets/ui_common/btn_1_1_on.png";
    
    if(NewRaidManager::getInstance()->getAbilityPoint() < 1)
        path = "Assets/ui_common/btn_1_off.png";
    
    auto btnLvUp = ui::Button::create(path);
    btnLvUp->setScale9Enabled(true);
    btnLvUp->setContentSize(Size(192, 94));
    btnLvUp->setTag(infoStatus->getIdx());
    btnLvUp->addClickEventListener(CC_CALLBACK_1(UILayerRaidStatus::onClickStatLevelUp, this));
    btnLvUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLvUp->setZoomScale(0.05f);
    btnLvUp->setPosition(Vec2(sprBg->getContentSize().width - 10, sprBg->getContentSize().height/2));
    sprBg->addChild(btnLvUp);
    {
        auto sprPlus = Sprite::create("Assets/ui_common/plus_2.png");
        sprPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprPlus->setPosition(Vec2(25, btnLvUp->getContentSize().height/2));
        btnLvUp->addChild(sprPlus);
        
        auto lbText = Label::createWithTTF("Skill Up", GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setDimensions(btnLvUp->getContentSize().width - 25 - sprPlus->getContentSize().width - 20, lbText->getContentSize().height);
        lbText->setPosition(sprPlus->getPosition() + Vec2::RIGHT * (sprPlus->getContentSize().width + 5));
        btnLvUp->addChild(lbText);
        
    }
    
    if(infoStatus->getCurrentLv() == infoStatus->getMaxLv())
    {
        btnLvUp->setVisible(false);
    }
    
    return cell;
}

ssize_t UILayerRaidStatus::numberOfCellsInTableView(extension::TableView *table)
{
    
    return _listInfoBongStat.size();
}

void UILayerRaidStatus::refreshUI()
{
    uiTop();
    _table->reloadData();
}
void UILayerRaidStatus::setCallbackLevelup(const std::function<void(void)>& callback)
{
    _callbackLevelup = callback;
}
void UILayerRaidStatus::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
#pragma mark - init
void UILayerRaidStatus::initVar()
{
}

void UILayerRaidStatus::initUi()
{
    
    _lyTop = Layer::create();
    _lyTop->setContentSize(Size(getContentSize().width, 120));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTop);
    
    // Table
    _lyStatus = LayerColor::create(Color4B::BLACK);
    _lyStatus->setContentSize(Size(getContentSize().width, getContentSize().height - _lyTop->getContentSize().height));
    _lyStatus->setIgnoreAnchorPointForPosition(false);
    _lyStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyStatus->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    addChild(_lyStatus);
   
    
    //
    uiTop();
    uiList();
}

#pragma mark - init
void UILayerRaidStatus::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_02.png");
    sprBox->setContentSize(_lyTop->getContentSize());
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(sprBox);
    
    
    int nLv = NewRaidManager::getInstance()->getLevel();
    auto infoLv = NewRaidManager::getInstance()->getInfoBongLevel(nLv + 1);
    int nTotalEXP = 0;
    int nPercent = 0;
    bool isMaxLv = false;
    
    if(infoLv != nullptr)
    {
        nTotalEXP = infoLv->getTotalEXP();
    }
    nPercent = (double)NewRaidManager::getInstance()->getEXP() / nTotalEXP * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    if(nLv >= NewRaidManager::getInstance()->getMaxLevel())
    {
        nPercent = 0;
        isMaxLv = true;
    }
    
    float width = getContentSize().width - 470;
    
    _lbBongLevel = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_raid_stat_exp", nLv), GAME_FONT, 30);
    _lbBongLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbBongLevel->setPosition(15, _lyTop->getContentSize().height - 38);
    _lbBongLevel->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
    _lbBongLevel->setOverflow(cocos2d::Label::Overflow::SHRINK);
    
    if(_lbBongLevel->getContentSize().width < width)
        width = _lbBongLevel->getContentSize().width;
    _lbBongLevel->setDimensions(width, _lbBongLevel->getContentSize().height);
    _lbBongLevel->setContentSize(Size(width, _lbBongLevel->getContentSize().height));
    
    _lyTop->addChild(_lbBongLevel);
    
    
    
    
    auto spriteTimerBg = Sprite::create("Assets/ui/raid/raid_progress_1_1.png");
    spriteTimerBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteTimerBg->setPosition( 15, 10);
    _lyTop->addChild(spriteTimerBg);
        
    std::string strProgressPath = "Assets/ui/raid/raid_progress_1_2.png";
        
    auto timerCount = ProgressTimer::create(Sprite::create(strProgressPath));
    timerCount->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    timerCount->setPosition( spriteTimerBg->getPosition() );
    timerCount->setMidpoint(Vec2(0, 0.5));
    timerCount->setBarChangeRate(Vec2(1,0));
    timerCount->setType(ProgressTimer::Type::BAR);
    timerCount->setPercentage(nPercent);
    _lyTop->addChild(timerCount);
    {
        std::string strTimer = MafUtils::format("%d/%d", NewRaidManager::getInstance()->getEXP(), nTotalEXP);
        
        if(isMaxLv == true)
            strTimer = "MAX";
        
        auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 16);
        labelTimer->setPosition( Vec2(timerCount->getContentSize().width / 2, timerCount->getContentSize().height / 2) );
        timerCount->addChild(labelTimer);
    }
    
    std::string path = "Assets/ui_common/btn_on.png";
    
    if(nPercent < 100)
        path = "Assets/ui_common/btn_1_off.png";
    auto btnLvUp = ui::Button::create(path);
    btnLvUp->setScale9Enabled(true);
    btnLvUp->setContentSize(Size(212, 90));
    btnLvUp->addClickEventListener(CC_CALLBACK_1(UILayerRaidStatus::onClickBongLevelUp, this));
    btnLvUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLvUp->setZoomScale(0.05f);
    btnLvUp->setPosition(Vec2(_lyTop->getContentSize().width - 10, _lyTop->getContentSize().height/2));
    btnLvUp->setTitleText(GAME_TEXT("t_ui_raid_p_lvup_bt"));
    btnLvUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnLvUp->setTitleFontName(GAME_FONT);
    btnLvUp->setTitleFontSize(26);
    _lyTop->addChild(btnLvUp);
    {
        auto sprFX = ui::Scale9Sprite::create("Assets/ui/raid/raid_lvup_on.png");
        sprFX->setContentSize(btnLvUp->getContentSize());
        sprFX->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprFX->setPosition(btnLvUp->getContentSize()/2);
        btnLvUp->addChild(sprFX);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto seq = Sequence::create(fadeOut,fadeIn, NULL);
        sprFX->runAction(RepeatForever::create(seq));
        
        if(nPercent >= 100)
        {
            sprFX->setVisible(true);
        }
        else
        {
            sprFX->setVisible(false);
        }
    }
    
    if(isMaxLv == true)
        btnLvUp->setVisible(false);
    
    
    auto sprStatBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprStatBox->setContentSize(Size(156,40));
    sprStatBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    sprStatBox->setPosition(spriteTimerBg->getPosition().x + spriteTimerBg->getContentSize().width,
                            spriteTimerBg->getPosition().y + spriteTimerBg->getContentSize().height + 15);
    _lyTop->addChild(sprStatBox);
    {
        std::string strStat = MafUtils::format("POINT : %d", NewRaidManager::getInstance()->getAbilityPoint());
        auto labelStat = Label::createWithTTF(strStat, GAME_FONT, 24);
        labelStat->setPosition( sprStatBox->getContentSize()/2 );
        labelStat->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        labelStat->setOverflow(cocos2d::Label::Overflow::SHRINK);
        labelStat->setDimensions(sprStatBox->getContentSize().width - 10, sprStatBox->getContentSize().height - 5);
        
        sprStatBox->addChild(labelStat);
    }
    
}

void UILayerRaidStatus::uiList()
{
    _table = extension::TableView::create(this, Size(_lyStatus->getContentSize().width, _lyStatus->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _table->setPosition(_lyStatus->getContentSize().width / 2, _lyStatus->getContentSize().height / 2);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyStatus->addChild(_table);
    
    //
    _table->reloadData();
}

#pragma mark - click
void UILayerRaidStatus::onClickBongLevelUp(Ref* pSender)
{
    int nLv = NewRaidManager::getInstance()->getLevel();
    auto infoLv = NewRaidManager::getInstance()->getInfoBongLevel(nLv+1);
    int nTotalEXP = 0;
    
    if(infoLv == nullptr)
    {
        return;
    }
    
    nTotalEXP = infoLv->getTotalEXP();
    if(NewRaidManager::getInstance()->getEXP() < nTotalEXP)
    {
        Vec2 posToast = _lyStatus->getParent()->convertToWorldSpace(_lyStatus->getPosition());
        posToast.y -= _lyStatus->getContentSize().height - 100;

        PopupToast::showImmediately(GAME_TEXT("t_ui_level_up_pop"), posToast.x, posToast.y);
        return;
    }
    if(!NewRaidManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        NewRaidManager::getInstance()->requestLevelup(CC_CALLBACK_3(UILayerRaidStatus::callbackLevelup, this), 0);
    }
}

void UILayerRaidStatus::onClickStatLevelUp(Ref* pSender)
{
    auto node = (Node*)pSender;
    
    if(NewRaidManager::getInstance()->getAbilityPoint() < 1)
    {
        Vec2 posToast = _lyStatus->getParent()->convertToWorldSpace(_lyStatus->getPosition());
        posToast.y -= _lyStatus->getContentSize().height / 2;

        PopupToast::showImmediately(GAME_TEXT("t_ui_raid_stat_toast_error"), posToast.x, posToast.y);
        return;
    }
    if(!NewRaidManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        NewRaidManager::getInstance()->requestLevelup(CC_CALLBACK_3(UILayerRaidStatus::callbackLevelup, this), node->getTag());
    }
}


#pragma mark - action
void UILayerRaidStatus::actionBongLevelup(Node* pNode)
{
    auto spriteEffect = Sprite::create("Assets/ui_common/ef_level_1.png");
    spriteEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteEffect->setPosition(pNode->getContentSize().width + 75, pNode->getContentSize().height / 2);
    pNode->addChild(spriteEffect);
        
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.05f);
    animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_1.png");
    animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_2.png");
    animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_3.png");
    animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_4.png");
        
    auto action1 = Animate::create(animation);
    auto action2 = RemoveSelf::create();
    auto seq = Sequence::create(action1, action2, NULL);
    spriteEffect->runAction(seq);
}

#pragma mark - callback
void UILayerRaidStatus::callbackLevelup(bool bResult, int nResult, int idx)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        else if( nResult == 8 )
        {
            strMessageKey = "t_ui_new_raid_error_1";
        }
        else if( nResult == 9 )
        {
            strMessageKey = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(nResult == 8 || nResult == 9)
        {
            if(_callbackHide != nullptr)
                _callbackHide();
        }
        return;
    }
    
    auto iter = _mapLevelupIdx.find(idx);
    if(iter != _mapLevelupIdx.end())
    {
        iter->second = true;
    }
    
    refreshUI();
    if(idx == 0)
    {
        if(_callbackLevelup != nullptr)
            _callbackLevelup();
        actionBongLevelup(_lbBongLevel);
    }
}
