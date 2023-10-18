//
//  PopupCalArtifact.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCalArtifact.h"

#include "GameObject/InfoArtifact.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupCalArtifact* PopupCalArtifact::create(InfoArtifact *info)
{
    PopupCalArtifact* pRet = new(std::nothrow) PopupCalArtifact();
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

PopupCalArtifact::PopupCalArtifact(void) :
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

PopupCalArtifact::~PopupCalArtifact(void)
{
    
}

bool PopupCalArtifact::init(InfoArtifact *info)
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

void PopupCalArtifact::onEnter()
{
    PopupBase::onEnter();
    
    drawArtifactInfo();
    drawArtifactLevel();
}

#pragma mark - init
void PopupCalArtifact::initVar()
{
    _levelNow = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    _levelAfter = _levelNow;
}

void PopupCalArtifact::initUi()
{
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_2.png", Rect::ZERO, Rect(35,170,10,10));
    _spriteContainer->setContentSize(Size(740, 484));
    _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    _spriteContainer->setTag(PopupCalArtifact::tag_container);
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCalArtifact::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - 35, _spriteContainer->getContentSize().height - 27) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    itemClose->setScale(0.6f);
    menu->addChild(itemClose);
    
    auto itemReset = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/calculator/btn_refresh.png"), nullptr, CC_CALLBACK_1(PopupCalArtifact::onClickReset, this));
    itemReset->setPosition( Vec2(_spriteContainer->getContentSize().width - 95, _spriteContainer->getContentSize().height - 27) );
    itemReset->setTouchRect( itemReset->getContentSize() * 2 );
    menu->addChild(itemReset);
    
    _itemMagnifier = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/review/icon_magnifier.png"), nullptr, CC_CALLBACK_1(PopupCalArtifact::onClickCurrency, this));
     _itemMagnifier->setPosition(Vec2(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 90));
     _itemMagnifier->setScale(1.5f);
     _itemMagnifier->setTouchRect(_itemMagnifier->getContentSize() * 1.5);
     menu->addChild(_itemMagnifier);
    
    uiCurrency();
    uiTop();
    uiBottom();
}

#pragma mark - ui
void PopupCalArtifact::uiCurrency()
{
    _layerContainerCurrency->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_4.png", Rect::ZERO, Rect(27,27,6,6));
    spriteBG->setContentSize(_layerContainerCurrency->getContentSize());
    spriteBG->setPosition(_layerContainerCurrency->getContentSize().width / 2, _layerContainerCurrency->getContentSize().height / 2);
    _layerContainerCurrency->addChild(spriteBG);
    
    //
    std::vector<E_ITEMS> list;
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
    {
        list.push_back(E_ITEMS::D_GOLD);
        list.push_back(E_ITEMS::D_KEY);
        list.push_back(E_ITEMS::COSTUME_COIN);
        list.push_back(E_ITEMS::POINT);
        list.push_back(E_ITEMS::GEM);
    }
    else
    {
        list.push_back(E_ITEMS::GOLD);
        list.push_back(E_ITEMS::KEY);
        list.push_back(E_ITEMS::POINT);
        list.push_back(E_ITEMS::GEM);
    }
    
    auto uiCurrency = UICurrency::create(list, _layerContainerCurrency->getContentSize());
    uiCurrency->setPosition(Vec2(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2));
    spriteBG->addChild(uiCurrency);
}

void PopupCalArtifact::uiTop()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalArtifact::tag_container);
    
    auto spriteIconBg = Sprite::create("Assets/ui_common/box_quest.png");
    spriteIconBg->setPosition( Vec2(90, spriteContainer->getContentSize().height - 80) );
    spriteContainer->addChild(spriteIconBg);
    
    auto spriteIcon = Sprite::create(_info->getIconPath());
    spriteIcon->setPosition( Vec2(spriteIconBg->getContentSize().width / 2, spriteIconBg->getContentSize().height / 2) );
    spriteIcon->getTexture()->setTexParameters(texParams);
    spriteIcon->setScale(spriteIconBg->getContentSize().width / spriteIcon->getContentSize().width);
    spriteIconBg->addChild(spriteIcon);
    
    auto labelTitle = Label::createWithTTF(_info->getTitle(), GAME_FONT, 27);
    labelTitle->setColor(COLOR_COIN);
    labelTitle->setPosition( Vec2(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteIconBg->getPositionY() + spriteIconBg->getContentSize().height / 2) );
    labelTitle->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    spriteContainer->addChild(labelTitle);
    
    _labelLevel = MafNode::createBmFont(MafUtils::format("Lv. %d (MAX %d)", _levelNow, _info->getLevelMax()), GAME_BM_FONT_AL, 22);
    _labelLevel->setColor(Color3B::WHITE);
    _labelLevel->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    _labelLevel->setPosition( Vec2(labelTitle->getPositionX() + 2, labelTitle->getPositionY() - labelTitle->getContentSize().height) );
    spriteContainer->addChild(_labelLevel);
    
    auto labelText = Label::createWithTTF(_info->getDesc(), GAME_FONT, 22);
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

void PopupCalArtifact::uiBottom()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalArtifact::tag_container);
    
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
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            strIconKeyPath = "Assets/icon/icon_gold_2.png";
        
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
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            _labelNeedKey->setColor(COLOR_D_KEY);
        
        
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
    
    int listBtn1Tag[] = {PopupCalArtifact::tag_btn1_1, PopupCalArtifact::tag_btn1_10, PopupCalArtifact::tag_btn1_100, PopupCalArtifact::tag_btn1_1000};
    std::string listBtn1Text[] = {"+1", "+10", "+100", "+1000"};
    

    for (int i = 0; i < 4; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
        spriteNormal->setContentSize(Size(76, 76));
        
        auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
        spriteDisable->setContentSize(Size(76, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifact::onClickLevel, this));
        item->setPosition( Vec2(300 + item->getContentSize().width / 2 + item->getContentSize().width * i + 5 * i, 268) );
        item->setLongClick(true);
        item->setTag(listBtn1Tag[i]);
        menu->addChild(item);
        
        auto label = Label::createWithTTF(listBtn1Text[i], GAME_FONT, 22);
        label->setPosition( Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2) );
        item->addChild(label);
    }
    
    int listBtnMinusTag[] = {PopupCalArtifact::tag_btn1_m1, PopupCalArtifact::tag_btn1_m10, PopupCalArtifact::tag_btn1_m100, PopupCalArtifact::tag_btn1_m1000};
    std::string listBtnMinusText[] = {"-1", "-10", "-100", "-1000"};
    
    for (int i = 0; i < 4; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
        spriteNormal->setContentSize(Size(76, 76));
        
        auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
        spriteDisable->setContentSize(Size(76, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifact::onClickLevel, this));
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
    
    auto maxBtnItem = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifact::onClickLevel, this));
    maxBtnItem->setPosition( Vec2(300 + maxBtnItem->getContentSize().width / 2 + maxBtnItem->getContentSize().width * 4 + 5 * 4 + 20, 218) );
    maxBtnItem->setTag(PopupCalArtifact::tag_btn1_max);
    menu->addChild(maxBtnItem);
    
    auto label = Label::createWithTTF("MAX", GAME_FONT, 22);
    label->setPosition( Vec2(maxBtnItem->getContentSize().width / 2, maxBtnItem->getContentSize().height / 2) );
    maxBtnItem->addChild(label);
    
    
    int listBtn2Tag[] = {PopupCalArtifact::tag_btn2_key, PopupCalArtifact::tag_btn2_point};
    std::string listBtn2Icon[] = {"Assets/icon/menu_key.png", "Assets/icon/icon_point1.png"};
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
        listBtn2Icon[0] = "Assets/icon/icon_gold_2.png";
    
    for (int i = 0; i < 2; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO, Rect(15,25,10,10));
        spriteNormal->setContentSize(Size(194, 76));
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_off.png", Rect::ZERO, Rect(15,25,10,10));
        spriteOff->setContentSize(Size(194, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteOff, CC_CALLBACK_1(PopupCalArtifact::onClickConfirm, this));
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
        
        if ( listBtn2Tag[i] ==  PopupCalArtifact::tag_btn2_key )
        {
            _itemKey = item;
        }
        else if ( listBtn2Tag[i] ==  PopupCalArtifact::tag_btn2_point )
        {
            _itemPoint = item;
        }
    }
    
    
}

#pragma mark - show, hide

#pragma mark - set, get, sort, add
void PopupCalArtifact::drawArtifactInfo()
{
    // level
    _levelNow = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    _labelLevel->setString(MafUtils::format("Lv. %d (MAX %d)", _levelNow, _info->getLevelMax()));
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    
    // effect
    double effect = DataManager::GetArtifactEffect(_info->getIdx(), _levelAfter, reinforce);
    std::string strValue = "";
    if ( _info->getIdx() == E_ARTIFACT::ARTI_17_CROSS_EVOL2 )
    {
        if (effect < 1000)  strValue = MafUtils::format("%.1lf",effect);
        else                strValue = MafUtils::format("%.0lf", effect);
    }
    else if( _info->getIdx() == E_ARTIFACT::ARTI_1_EVOL1 ||
             _info->getIdx() == E_ARTIFACT::ARTI_3_EVOL1 ||
             _info->getIdx() == E_ARTIFACT::ARTI_4_EVOL1 ||
             _info->getIdx() == E_ARTIFACT::ARTI_8_EVOL1 ||
             _info->getIdx() == E_ARTIFACT::ARTI_12_CROSS_EVOL1 )
    {
        strValue = MafUtils::format("%.0lf", effect);
    }
    else
    {
        if (effect < 1000)  strValue = MafUtils::format("%.2lf",effect);
        else                strValue = MafUtils::format("%.0lf", effect);
    }
    
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

void PopupCalArtifact::drawArtifactLevel()
{
    _levelNow = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    
    // need point
    int diff = _levelAfter - _levelNow;
    int needPoint = diff * _info->getPricePoint();
    
    // need key
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(UserInfoManager::getInstance()->getCurrentPlace());
    int floorTotal = UserInfoManager::getInstance()->getTotalFloor();
    
    double nKey = 0.0f;
    for ( int i = _levelNow; i < _levelAfter; i++ )
    {
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
            nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL, _info->getIdx(), i, floorHigh, floorTotal);
        else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM, _info->getIdx(), i, floorHigh, floorTotal);
    }
    std::string needKey = MafUtils::doubleToString(nKey);
    
    // text print
    std::string strNeedKeyShort = needKey;
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
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
        strUserKey = ItemsMoneyManager::getInstance()->getDevilKey();

    if ( MafUtils::compareStr(strUserKey, needKey) == false )
    {
        _itemKey->setEnabled(false);
    }
    
    if ( _info->getPricePoint() == 0 || ItemsMoneyManager::getInstance()->getPoint() < needPoint )
    {
        _itemPoint->setEnabled(false);
    }
}

void PopupCalArtifact::setCallback(const std::function<void(void)>& callback)
{
    _onCallback = callback;
}


#pragma mark - click
void PopupCalArtifact::onClickClose(Ref* sender)
{
    hide();
}

void PopupCalArtifact::onClickReset(cocos2d::Ref* sender)
{
    _levelAfter = _levelNow;
    
    if ( _levelAfter > _info->getLevelMax() )
    {
        _levelAfter = _info->getLevelMax();
    }
    
    if ( _levelNow > _levelAfter )
    {
        _levelAfter = _levelNow;
    }

    drawArtifactInfo();
    drawArtifactLevel();
}

void PopupCalArtifact::onClickLevel(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    if ( item->getTag() == PopupCalArtifact::tag_btn1_max )
    {
        PopupLoading::show();
        std::thread([this]() {
 
            // need key
            int floorHigh = UserInfoManager::getInstance()->getHighFloor(UserInfoManager::getInstance()->getCurrentPlace());
            int floorTotal = UserInfoManager::getInstance()->getTotalFloor();
            
            double nKey = 0.0f;
            std::string strUserKey = ItemsMoneyManager::getInstance()->getKey();
            if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            {
                strUserKey = ItemsMoneyManager::getInstance()->getDevilKey();
            }
                
            for ( int i = _levelNow; i < _info->getLevelMax(); i++ )
            {
                if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
                    nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL, _info->getIdx(), i, floorHigh, floorTotal);
                else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
                    nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM, _info->getIdx(), i, floorHigh, floorTotal);
                
                std::string needKey = MafUtils::doubleToString(nKey);
                if ( MafUtils::compareStr(strUserKey, needKey) == false )
                {
                    break;
                }
                
                _levelAfter = i+1;
            }
            
            if ( _levelAfter > _info->getLevelMax() )
            {
                _levelAfter = _info->getLevelMax();
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
            case PopupCalArtifact::tag_btn1_1:      _levelAfter += 1;   	break;
            case PopupCalArtifact::tag_btn1_10:     _levelAfter += 10;      break;
            case PopupCalArtifact::tag_btn1_100:    _levelAfter += 100;     break;
            case PopupCalArtifact::tag_btn1_1000:   _levelAfter += 1000;    break;
            case PopupCalArtifact::tag_btn1_m1:     _levelAfter -= 1;       break;
            case PopupCalArtifact::tag_btn1_m10:    _levelAfter -= 10;      break;
            case PopupCalArtifact::tag_btn1_m100:   _levelAfter -= 100;     break;
            case PopupCalArtifact::tag_btn1_m1000:  _levelAfter -= 1000;    break;
        }
    
        if ( _levelAfter > _info->getLevelMax() )
        {
            _levelAfter = _info->getLevelMax();
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

void PopupCalArtifact::onClickConfirm(cocos2d::Ref* sender)
{
    // 다시 한번 초기화
    _levelNow = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    
    if ( _levelAfter > _info->getLevelMax() )
    {
        _levelAfter = _info->getLevelMax();
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
    
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(UserInfoManager::getInstance()->getCurrentPlace());
    int floorTotal = UserInfoManager::getInstance()->getTotalFloor();
    
    // 코스튬 효과
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
    {
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_10) && (DataManager::IsArtifactTypeNormal(_info->getIdx()) == true || DataManager::IsArtifactTypeSpecial(_info->getIdx()) == true))
        {
            isLevelPlusApply = true;
        }
    }
    
    if ( isLevelPlusApply == true )
    {
        for ( int i = _levelNow; i < _levelAfter; i++ )
        {
            if ( random(1, 100) <= 10 )
            {
                levelPlus += 1;
                
                double temp = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_info->getIdx(), i, floorHigh, floorTotal);
                needKeyBonus = MafUtils::bigAddNum(needKeyBonus, MafUtils::doubleToString(temp));
            }
        }
        
        needPointBonus = _info->getPricePoint() * levelPlus;
    }
    
    
    
    if ( item->getTag() == PopupCalArtifact::tag_btn2_key )
    {
        // need key
        double nKey = 0.0f;
        for ( int i = _levelNow; i < _levelAfter; i++ )
        {
            if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
                nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL, _info->getIdx(), i, floorHigh, floorTotal);
            else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
                nKey += DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM, _info->getIdx(), i, floorHigh, floorTotal);
        }
        auto nowCurrency = ItemsMoneyManager::getInstance()->getKey();
        auto needCurrency = MafUtils::doubleToString(nKey);
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
        {
            nowCurrency = ItemsMoneyManager::getInstance()->getDevilKey();
        }
        
        if ( levelPlus > 0 )
        {
            needCurrency = MafUtils::bigSubNum(needCurrency, needKeyBonus);
        }
        
        
        if ( MafUtils::compareStr(nowCurrency, needCurrency) == false )
        {
            if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::KEY);
                popup->show();
            }
            else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::D_KEY);
                popup->show();
            }
            return;
        }
        
        // 재화 감소
        needCurrency.insert(0, "-");
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
            ItemsMoneyManager::getInstance()->setKey(needCurrency);
        else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
            ItemsMoneyManager::getInstance()->setDevilKey(needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _info->getIdx(), _levelAfter);
    }
    else if ( item->getTag() == PopupCalArtifact::tag_btn2_point )
    {
        // need point
        int nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
        int needCurrency = (_levelAfter - _levelNow) * _info->getPricePoint();
        
        if ( levelPlus > 0 )
        {
            needCurrency -= needPointBonus;
        }
        
        if ( _info->getPricePoint() == 0 || nowCurrency < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
            popup->show();
            return;
        }
        
        // 재화 감소
        ItemsMoneyManager::getInstance()->setPoint(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _info->getIdx(), _levelAfter);
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
    
    
    // 스토리 업적
    if ( _info->getIdx() == E_ARTIFACT::ARTI_1_EVOL1 )
    {
        if ( _levelNow >= 25 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_RING);
        }
        
        if ( _levelNow >= 30000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_30000_ARTIFACT1, 1, 1);
        }
    }
    else if( _info->getIdx() == E_ARTIFACT::ARTI_3_EVOL1 )
    {
        if ( _levelNow >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_STORMY);
        }
    }
    else if( _info->getIdx() == E_ARTIFACT::ARTI_4_EVOL1 )
    {
        if ( _levelNow >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_EAGLE);
        }
        
        if ( _levelNow >= 20000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_20000_ARTIFACT4, 1, 1);
        }
    }
    
    
    
    // 콜백
    if ( _onCallback != nullptr )
    {
        _onCallback();
    }
}

void PopupCalArtifact::onClickCurrency(cocos2d::Ref* sender)
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


