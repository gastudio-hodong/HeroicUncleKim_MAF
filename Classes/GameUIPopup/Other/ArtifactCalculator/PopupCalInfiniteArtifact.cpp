//
//  PopupCalInfiniteArtifact.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/03.
//

#include "PopupCalInfiniteArtifact.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

PopupCalInfiniteArtifact* PopupCalInfiniteArtifact::create(InfiniteArtifactInfo *info)
{
    PopupCalInfiniteArtifact* pRet = new(std::nothrow) PopupCalInfiniteArtifact();
    if ( pRet && pRet->init(info) )
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

PopupCalInfiniteArtifact::PopupCalInfiniteArtifact(void) :
_info(nullptr),
_levelNow(0),
_levelAfter(0),
_onCallback(nullptr),
_valueLongShow(false),

_layerContainerCurrency(nullptr),
_labelLevel(nullptr),
_labelEffect(nullptr),
_labelLevelPrev(nullptr),
_labelLevelAfter(nullptr),
_labelLevelDiff(nullptr),
_labelNeedKey(nullptr),
_labelNeedPoint(nullptr),
_itemKey(nullptr),
_itemPoint(nullptr)
{
    
}

PopupCalInfiniteArtifact::~PopupCalInfiniteArtifact(void)
{
    
}

bool PopupCalInfiniteArtifact::init(InfiniteArtifactInfo *info)
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // info
    _info = info;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupCalInfiniteArtifact::onEnter()
{
    PopupBase::onEnter();
    
    drawArtifactInfo();
    drawArtifactLevel();
}

#pragma mark - init
void PopupCalInfiniteArtifact::initVar()
{
    _levelNow = _info->getCurLv();
    _levelAfter = _levelNow;
}

void PopupCalInfiniteArtifact::initUi()
{
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_2.png", Rect::ZERO, Rect(35,170,10,10));
    _spriteContainer->setContentSize(Size(740, 484));
    _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    _spriteContainer->setTag(PopupCalInfiniteArtifact::tag_container);
    addChild(_spriteContainer);
    
    _layerContainerCurrency = Layer::create();
    _layerContainerCurrency->setContentSize( Size(_spriteContainer->getContentSize().width, 60) );
    _layerContainerCurrency->setIgnoreAnchorPointForPosition(false);
    _layerContainerCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerCurrency->setPosition( _spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height + 5);
    _spriteContainer->addChild(_layerContainerCurrency);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - 35, _spriteContainer->getContentSize().height - 27) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    itemClose->setScale(0.6f);
    menu->addChild(itemClose);
    
    auto itemReset = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/calculator/btn_refresh.png"), nullptr, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickReset, this));
    itemReset->setPosition( Vec2(_spriteContainer->getContentSize().width - 95, _spriteContainer->getContentSize().height - 27) );
    itemReset->setTouchRect( itemReset->getContentSize() * 2 );
    menu->addChild(itemReset);
    
    _itemMagnifier = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/review/icon_magnifier.png"), nullptr, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickCurrency, this));
     _itemMagnifier->setPosition(Vec2(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 90));
     _itemMagnifier->setScale(1.5f);
     _itemMagnifier->setTouchRect(_itemMagnifier->getContentSize() * 1.5);
     menu->addChild(_itemMagnifier);
    
    uiCurrency();
    uiTop();
    uiBottom();
}

#pragma mark - ui
void PopupCalInfiniteArtifact::uiCurrency()
{
    _layerContainerCurrency->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_4.png", Rect::ZERO, Rect(27,27,6,6));
    spriteBG->setContentSize(_layerContainerCurrency->getContentSize());
    spriteBG->setPosition(_layerContainerCurrency->getContentSize().width / 2, _layerContainerCurrency->getContentSize().height / 2);
    _layerContainerCurrency->addChild(spriteBG);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GOLD);
    list.push_back(E_ITEMS::KEY);
    list.push_back(E_ITEMS::POINT);
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, _layerContainerCurrency->getContentSize());
    uiCurrency->setPosition(Vec2(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2));
    spriteBG->addChild(uiCurrency);
}

void PopupCalInfiniteArtifact::uiTop()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalInfiniteArtifact::tag_container);
    
    std::string pathBG = "Assets/ui_common/box_transcendence1_1.png";
    if ( _info->getGrade() > 7 )
    {
        pathBG = "Assets/ui_common/box_transcendence2_1.png";
    }
    auto spriteIconBg = Sprite::create(pathBG);
    spriteIconBg->setPosition( Vec2(90, spriteContainer->getContentSize().height - 80) );
    spriteContainer->addChild(spriteIconBg);
    
    auto sprGrade = Sprite::create("Assets/ui_common/icon_star1_1.png");
    sprGrade->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprGrade->setPosition(0, spriteIconBg->getContentSize().height);
    spriteIconBg->addChild(sprGrade);
    
    auto lbGrade = MafNode::createBmFont(MafUtils::toString(_info->getGrade()), GAME_BM_FONT_AL, 22);
    lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbGrade->setPosition(sprGrade->getContentSize().width, sprGrade->getContentSize().height/2);
    lbGrade->setColor(Color3B(255,229,0));
    sprGrade->addChild(lbGrade);
    
    // 제목, 설명, 경로
    std::string strTitle = _info->getName();
    std::string strContent = _info->getDesc();
    std::string strPath = _info->getIconPath();
    
    auto spriteIcon = Sprite::create(strPath);
    spriteIcon->setPosition( Vec2(spriteIconBg->getContentSize().width / 2, spriteIconBg->getContentSize().height / 2) );
    spriteIcon->getTexture()->setTexParameters(texParams);
    spriteIcon->setScale(spriteIconBg->getContentSize().width / spriteIcon->getContentSize().width);
    spriteIconBg->addChild(spriteIcon);
    
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 27);
    labelTitle->setColor(COLOR_COIN);
    labelTitle->setPosition( Vec2(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteIconBg->getPositionY() + spriteIconBg->getContentSize().height / 2) );
    labelTitle->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    spriteContainer->addChild(labelTitle);
    
    _labelLevel = MafNode::createBmFont(MafUtils::format("Lv. %d (MAX %d)", _levelNow, _info->getMaxLv()), GAME_BM_FONT_AL, 22);
    _labelLevel->setColor(Color3B::WHITE);
    _labelLevel->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    _labelLevel->setPosition( Vec2(labelTitle->getPositionX() + 2, labelTitle->getPositionY() - labelTitle->getContentSize().height) );
    spriteContainer->addChild(_labelLevel);
    
    auto labelText = Label::createWithTTF(strContent, GAME_FONT, 22);
    labelText->setColor(Color3B::WHITE);
    labelText->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    labelText->setPosition( Vec2(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteIconBg->getPositionY() - spriteIconBg->getContentSize().height / 4 - 5) );
    spriteContainer->addChild(labelText);
    
    _labelEffect = MafNode::createBmFont("", GAME_BM_FONT_AL, 24);
    _labelEffect->setColor(Color3B::WHITE);
    _labelEffect->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
    _labelEffect->setPosition( Vec2(spriteContainer->getContentSize().width - 40, spriteIconBg->getPositionY() - spriteIconBg->getContentSize().height/2) );
    spriteContainer->addChild(_labelEffect);
}

void PopupCalInfiniteArtifact::uiBottom()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalInfiniteArtifact::tag_container);
    
    auto spriteBox1 = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png", Rect::ZERO, Rect(0,0,20,20));
    spriteBox1->setContentSize(Size(268, 178));
    spriteBox1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteBox1->setPosition(20, 130);
    spriteContainer->addChild(spriteBox1);
    {
        auto spriteBoxIn = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png", Rect::ZERO, Rect(0,0,20,20));
        spriteBoxIn->setContentSize(Size(268, 34));
        spriteBoxIn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteBoxIn->setPosition(0, 110);
        spriteBox1->addChild(spriteBoxIn);
        
        
        auto labelTextNow = Label::createWithTTF(GAME_TEXT("t_ui_calculator_msg_1"), GAME_FONT, 22);
        labelTextNow->setColor(Color3B::GRAY);
        labelTextNow->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        labelTextNow->setPosition( Vec2(10, 140) );
        spriteBox1->addChild(labelTextNow);
        
        auto labelTextAfter = Label::createWithTTF(GAME_TEXT("t_ui_calculator_msg_2"), GAME_FONT, 22);
        labelTextAfter->setColor(Color3B::WHITE);
        labelTextAfter->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        labelTextAfter->setPosition( Vec2(10, 110) );
        spriteBox1->addChild(labelTextAfter);
        
        _labelLevelPrev = MafNode::createBmFont(MafUtils::format("%d", _levelNow), GAME_BM_FONT_AL, 24);
        _labelLevelPrev->setColor(Color3B::GRAY);
        _labelLevelPrev->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        _labelLevelPrev->setPosition( Vec2(260, 140) );
        spriteBox1->addChild(_labelLevelPrev);
        
        _labelLevelAfter = MafNode::createBmFont(MafUtils::format("%d", _levelAfter), GAME_BM_FONT_AL, 24);
        _labelLevelAfter->setColor(Color3B::WHITE);
        _labelLevelAfter->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        _labelLevelAfter->setPosition( Vec2(260, 110) );
        spriteBox1->addChild(_labelLevelAfter);
        
        _labelLevelDiff = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
        _labelLevelDiff->setColor(COLOR_COIN);
        _labelLevelDiff->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        _labelLevelDiff->setPosition( Vec2(260, 80) );
        _labelLevelDiff->setVisible(false);
        spriteBox1->addChild(_labelLevelDiff);
        
        
        std::string strIconKeyPath = "Assets/icon/menu_key.png";
        
        auto spriteIconKey = Sprite::create(strIconKeyPath.c_str());
        spriteIconKey->setPosition( Vec2(30, 60) );
        spriteBox1->addChild(spriteIconKey);
        
        auto spriteIconPoint = Sprite::create("Assets/icon/icon_point1.png");
        spriteIconPoint->setPosition( Vec2(30, 20) );
        spriteBox1->addChild(spriteIconPoint);
        
        _labelNeedKey = MafNode::createBmFont("0", GAME_BM_FONT_AL, 24);
        _labelNeedKey->setColor(COLOR_KEY);
        _labelNeedKey->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        _labelNeedKey->setPosition( Vec2(50, 60) );
        spriteBox1->addChild(_labelNeedKey);
        
        
        _labelNeedPoint = MafNode::createBmFont("0", GAME_BM_FONT_AL, 24);
        _labelNeedPoint->setColor(COLOR_POINT);
        _labelNeedPoint->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        _labelNeedPoint->setPosition( Vec2(50, 20) );
        spriteBox1->addChild(_labelNeedPoint);
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint( Vec2::ZERO );
    menu->setPosition( Vec2::ZERO );
    spriteContainer->addChild(menu);
    
    int listBtn1Tag[] = {PopupCalInfiniteArtifact::tag_btn1_1, PopupCalInfiniteArtifact::tag_btn1_10, PopupCalInfiniteArtifact::tag_btn1_100, PopupCalInfiniteArtifact::tag_btn1_1000};
    std::string listBtn1Text[] = {"+1", "+10", "+100", "+1000"};
    

    for (int i = 0; i < 4; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
        spriteNormal->setContentSize(Size(76, 76));
        
        auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
        spriteDisable->setContentSize(Size(76, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickLevel, this));
        item->setPosition( Vec2(300 + item->getContentSize().width / 2 + item->getContentSize().width * i + 5 * i, 268) );
        item->setLongClick(true);
        item->setTag(listBtn1Tag[i]);
        menu->addChild(item);
        
        auto label = Label::createWithTTF(listBtn1Text[i], GAME_FONT, 22);
        label->setPosition( Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2) );
        item->addChild(label);
    }
    
    int listBtnMinusTag[] = {PopupCalInfiniteArtifact::tag_btn1_m1, PopupCalInfiniteArtifact::tag_btn1_m10, PopupCalInfiniteArtifact::tag_btn1_m100, PopupCalInfiniteArtifact::tag_btn1_m1000};
    std::string listBtnMinusText[] = {"-1", "-10", "-100", "-1000"};
    
    for (int i = 0; i < 4; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
        spriteNormal->setContentSize(Size(76, 76));
        
        auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
        spriteDisable->setContentSize(Size(76, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickLevel, this));
        item->setPosition( Vec2(300 + item->getContentSize().width / 2 + item->getContentSize().width * i + 5 * i, 170) );
        item->setLongClick(true);
        item->setTag(listBtnMinusTag[i]);
        menu->addChild(item);
        
        auto label = Label::createWithTTF(listBtnMinusText[i], GAME_FONT, 22);
        label->setPosition( Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2) );
        item->addChild(label);
    }
    
    auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
    spriteNormal->setContentSize(Size(76, 174));
    
    auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
    spriteDisable->setContentSize(Size(76, 174));
    
    auto maxBtnItem = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickLevel, this));
    maxBtnItem->setPosition( Vec2(300 + maxBtnItem->getContentSize().width / 2 + maxBtnItem->getContentSize().width * 4 + 5 * 4 + 20, 218) );
    maxBtnItem->setTag(PopupCalInfiniteArtifact::tag_btn1_max);
    menu->addChild(maxBtnItem);
    
    auto label = Label::createWithTTF("MAX", GAME_FONT, 22);
    label->setPosition( Vec2(maxBtnItem->getContentSize().width / 2, maxBtnItem->getContentSize().height / 2) );
    maxBtnItem->addChild(label);
    
    
    int listBtn2Tag[] = {PopupCalInfiniteArtifact::tag_btn2_key, PopupCalInfiniteArtifact::tag_btn2_point};
    std::string listBtn2Icon[] = {"Assets/icon/menu_key.png", "Assets/icon/icon_point1.png"};
    
    for (int i = 0; i < 2; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO, Rect(15,25,10,10));
        spriteNormal->setContentSize(Size(194, 76));
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_off.png", Rect::ZERO, Rect(15,25,10,10));
        spriteOff->setContentSize(Size(194, 76));

        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteOff, CC_CALLBACK_1(PopupCalInfiniteArtifact::onClickConfirm, this));
        item->setPosition( Vec2(170 + item->getContentSize().width / 2 + item->getContentSize().width * i + 12 * i, 65) );
        item->setTag(listBtn2Tag[i]);
        menu->addChild(item);
        
        auto sprite = Sprite::create(listBtn2Icon[i]);
        sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        sprite->setPosition( Vec2(30, item->getContentSize().height / 2) );
        item->addChild(sprite);
        
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_apply"), GAME_FONT, 22);
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        label->setPosition( Vec2(100, item->getContentSize().height / 2) );
        item->addChild(label);
        
        if ( listBtn2Tag[i] ==  PopupCalInfiniteArtifact::tag_btn2_key )
        {
            _itemKey = item;
        }
        else if ( listBtn2Tag[i] ==  PopupCalInfiniteArtifact::tag_btn2_point )
        {
            _itemPoint = item;
        }
    }
    
    
}

#pragma mark - show, hide

#pragma mark - set, get, sort, add
void PopupCalInfiniteArtifact::drawArtifactInfo()
{
    // level
    _levelNow = _info->getCurLv();
    _labelLevel->setString(MafUtils::format("Lv. %d (MAX %d)", _levelNow, _info->getMaxLv()));
    
    // effect
    auto effect = _info->getEfficiency(_levelAfter - _info->getCurLv());
    std::string strValue = "";
    
    if (effect < 1000)  strValue = MafUtils::format("%.2lf",effect);
    else                strValue = MafUtils::format("%.0lf", effect);
    
    if(!_valueLongShow)
    {
         if (effect > 1000)
             MafUtils::convertNumberToShort(strValue);
        else
            strValue = MafUtils::addCommaToString(strValue, 3, ',');
    }
    else
        strValue = MafUtils::addCommaToString(strValue, 3, ',');
    
    _labelEffect->setString( MafUtils::format("%s%%",strValue.c_str()) );
}

void PopupCalInfiniteArtifact::drawArtifactLevel()
{
    _levelNow = _info->getCurLv();
    
    // need point
    int diff = _levelAfter - _levelNow;
    int needPoint = diff * _info->getDemandPoint();
    
    // need key
    double nKey = 0.0f;
    
    for ( int i = _levelNow; i < _levelAfter; i++ )
    {
        nKey += InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_info->getGrade(), _info->getIncreaseA(), _info->getIncreaseB(), i+1, _info->getType(), true);
    }
    
    // text print
    std::string strNeedKeyShort = MafUtils::doubleToString(nKey);
    strNeedKeyShort = MafUtils::convertNumberToShort(strNeedKeyShort);
    _labelNeedKey->setString(strNeedKeyShort);
    _labelNeedPoint->setString(MafUtils::format("%d", needPoint));
    
    _labelLevelPrev->setString(MafUtils::format("%d", _levelNow));
    _labelLevelAfter->setString(MafUtils::format("%d", _levelAfter));
    _labelLevelDiff->setString(MafUtils::format("+%d", diff));
    if ( diff > 0)
    {
        _labelLevelDiff->setVisible(true);
    }
    
    
    // button enable
    _itemKey->setEnabled(true);
    _itemPoint->setEnabled(true);
    
    if ( _levelNow == _levelAfter )
    {
        _itemKey->setEnabled(false);
        _itemPoint->setEnabled(false);
    }
    
    std::string strUserKey = ItemsMoneyManager::getInstance()->getKey();

    if ( MafUtils::compareStr(strUserKey, MafUtils::doubleToString(nKey)) == false )
    {
        _itemKey->setEnabled(false);
    }
    
    if ( _info->getDemandPoint() == 0 || ItemsMoneyManager::getInstance()->getPoint() < needPoint )
    {
        _itemPoint->setEnabled(false);
    }
}

void PopupCalInfiniteArtifact::setCallback(const std::function<void(void)>& callback)
{
    _onCallback = callback;
}


#pragma mark - click
void PopupCalInfiniteArtifact::onClickClose(Ref* sender)
{
    hide();
}

void PopupCalInfiniteArtifact::onClickReset(cocos2d::Ref* sender)
{
    _levelAfter = _levelNow;
    
    if ( _levelAfter > _info->getMaxLv() )
    {
        _levelAfter = _info->getMaxLv();
    }
    
    if ( _levelNow > _levelAfter )
    {
        _levelAfter = _levelNow;
    }

    drawArtifactInfo();
    drawArtifactLevel();
}

void PopupCalInfiniteArtifact::onClickLevel(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    if ( item->getTag() == PopupCalInfiniteArtifact::tag_btn1_max )
    {
        PopupLoading::show();
        std::thread([this]() {
 
            // need key
            
            std::string needKey = "0";
            double nKey = 0.0f;
            std::string strUserKey = ItemsMoneyManager::getInstance()->getKey();
            for ( int i = _levelNow; i < _info->getMaxLv(); i++ )
            {
                
                nKey += InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_info->getGrade(), _info->getIncreaseA(), _info->getIncreaseB(), i+1, _info->getType(), true);
                
                needKey = MafUtils::doubleToString(nKey);
                if ( MafUtils::compareStr(strUserKey, needKey) == false )
                {
                    break;
                }
                
                _levelAfter = i+1;
            }
            
            if ( _levelAfter > _info->getMaxLv() )
            {
                _levelAfter = _info->getMaxLv();
            }
                
            if ( _levelNow > _levelAfter )
            {
                _levelAfter = _levelNow;
            }
            
            if ( _levelAfter < 0 )
            {
                _levelAfter = 0;
            }
            
            //
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                PopupLoading::hide();
                
                drawArtifactInfo();
                drawArtifactLevel();
            });
             
        }).detach();
    }
    else
    {
        switch (item->getTag()) {
            case PopupCalInfiniteArtifact::tag_btn1_1:      _levelAfter += 1;       break;
            case PopupCalInfiniteArtifact::tag_btn1_10:     _levelAfter += 10;      break;
            case PopupCalInfiniteArtifact::tag_btn1_100:    _levelAfter += 100;     break;
            case PopupCalInfiniteArtifact::tag_btn1_1000:   _levelAfter += 1000;    break;
            case PopupCalInfiniteArtifact::tag_btn1_m1:     _levelAfter -= 1;       break;
            case PopupCalInfiniteArtifact::tag_btn1_m10:    _levelAfter -= 10;      break;
            case PopupCalInfiniteArtifact::tag_btn1_m100:   _levelAfter -= 100;     break;
            case PopupCalInfiniteArtifact::tag_btn1_m1000:  _levelAfter -= 1000;    break;
        }
    
        if ( _levelAfter > _info->getMaxLv() )
        {
            _levelAfter = _info->getMaxLv();
        }
            
        if ( _levelNow > _levelAfter )
        {
            _levelAfter = _levelNow;
        }
        
        if ( _levelAfter < 0 )
        {
            _levelAfter = 0;
        }
        
        drawArtifactInfo();
        drawArtifactLevel();
    }
}

void PopupCalInfiniteArtifact::onClickConfirm(cocos2d::Ref* sender)
{
    // 다시 한번 초기화
    _levelNow = _info->getCurLv();
    
    if ( _levelAfter > _info->getMaxLv() )
    {
        _levelAfter = _info->getMaxLv();
    }
    
    if ( _levelNow > _levelAfter )
    {
        _levelAfter = _levelNow;
    }
    
    if ( _levelNow == _levelAfter )
    {
        return;
    }
    
    auto item = (MenuItem*)sender;
    
    int levelPlus = 0;
    bool isLevelPlusApply = false;
    
    std::string needKeyBonus = "0";
    int needPointBonus = 0;
    
    
    // 코스튬 효과
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_10) && _info->getType() != INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS)
    {
        isLevelPlusApply = true;
    }
    if ( isLevelPlusApply == true )
    {
        for ( int i = _levelNow; i < _levelAfter; i++ )
        {
            if ( random(1, 100) <= 10 )
            {
                levelPlus += 1;
                
                std::string temp = MafUtils::doubleToString(InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_info->getGrade(), _info->getIncreaseA(), _info->getIncreaseB(), i+1, _info->getType(), true));
                
                needKeyBonus = MafUtils::bigAddNum(needKeyBonus, temp);
            }
        }
        
        needPointBonus = _info->getDemandPoint() * levelPlus;
    }
    
    
    
    if ( item->getTag() == PopupCalInfiniteArtifact::tag_btn2_key )
    {
        // need key
        std::string nKey = "0";
        double needKey = 0;
        for ( int i = _levelNow; i < _levelAfter; i++ )
        {
            needKey += InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_info->getGrade(), _info->getIncreaseA(), _info->getIncreaseB(), i+1, _info->getType(), true);
        }
        nKey = MafUtils::bigAddNum(nKey,MafUtils::doubleToString(needKey));
        auto nowCurrency = ItemsMoneyManager::getInstance()->getKey();
        auto needCurrency = nKey;
        if ( levelPlus > 0 )
        {
            needCurrency = MafUtils::bigSubNum(needCurrency, needKeyBonus);
        }
        
        
        if ( MafUtils::compareStr(nowCurrency, needCurrency) == false )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::KEY);
            popup->show();
            return;
        }
        
        // 재화 감소
        needCurrency.insert(0, "-");
        ItemsMoneyManager::getInstance()->setKey(needCurrency);
        // 레벨 적용
        InfiniteArtifactManager::getInstance()->setInfiniteArtifactLevel(_info, _levelAfter);
    }
    else if ( item->getTag() == PopupCalInfiniteArtifact::tag_btn2_point )
    {
        // need point
        int nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
        int needCurrency = (_levelAfter - _levelNow) * _info->getDemandPoint();
        
        if ( levelPlus > 0 )
        {
            needCurrency -= needPointBonus;
        }
        
        if ( _info->getDemandPoint() == 0 || nowCurrency < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
            popup->show();
            return;
        }
        
        // 재화 감소
        ItemsMoneyManager::getInstance()->setPoint(-needCurrency);
        
        // 레벨 적용
        InfiniteArtifactManager::getInstance()->setInfiniteArtifactLevel(_info, _levelAfter);
    }

    // 데이터 초기화
    _levelNow = _levelAfter;
    
    drawArtifactInfo();
    drawArtifactLevel();
    
    
    if ( levelPlus > 0 )
    {
        std::string strPlus = MafUtils::format("+%d", levelPlus);
        auto labelPlus = Label::createWithTTF(strPlus, GAME_FONT, 24);
        labelPlus->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelPlus->setPosition(_labelLevelPrev->getPositionX() - 50, _labelLevelPrev->getPositionY() + 10);
        labelPlus->setColor(COLOR_COIN);
        labelPlus->setOpacity(0);
        _labelLevelPrev->getParent()->addChild(labelPlus);
        
        auto action1_1 = FadeIn::create(0.2);
        auto action1_2 = DelayTime::create(0.3);
        auto action1_3 = FadeOut::create(0.2);
        auto action1_4 = RemoveSelf::create();
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, action1_4, nullptr);
        
        auto action2_1 = DelayTime::create(0.2);
        auto action2_2 = MoveBy::create(0.3, Vec2(0, 10));
        auto seq2 = Sequence::create(action2_1, action2_2, nullptr);
        labelPlus->runAction(seq1);
        labelPlus->runAction(seq2);
    }
    
    
    // 콜백
    if ( _onCallback != nullptr )
    {
        _onCallback();
    }
}

void PopupCalInfiniteArtifact::onClickCurrency(cocos2d::Ref* sender)
{
    if ( _valueLongShow == true )
    {
        return;
    }
        
    _valueLongShow = true;
    drawArtifactInfo();
    
    auto action1 = DelayTime::create(4.0);
    auto action2 = CallFunc::create([=](){
        _valueLongShow = false;
        drawArtifactInfo();
    });
    auto seq = Sequence::create(action1, action2, nullptr);
    runAction(seq);
}


