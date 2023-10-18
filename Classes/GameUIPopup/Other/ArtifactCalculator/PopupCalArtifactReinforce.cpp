//
//  PopupCalArtifactReinforce.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCalArtifactReinforce.h"

#include "GameObject/InfoArtifact.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupCalArtifactReinforce* PopupCalArtifactReinforce::create(InfoArtifact *info)
{
    PopupCalArtifactReinforce* pRet = new(std::nothrow) PopupCalArtifactReinforce();
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

PopupCalArtifactReinforce::PopupCalArtifactReinforce(void) :
_info(nullptr),
_reinforceNow(0),
_reinforceAfter(0),
_onCallback(nullptr),
_valueLongShow(false),

_layerContainerCurrency(nullptr),
_labelReinforce(nullptr),
_labelLevel(nullptr),
_labelEffect(nullptr),
_labelLevelPrev(nullptr),
_labelLevelAfter(nullptr),
_labelLevelDiff(nullptr),
_labelNeedCube(nullptr),
_itemCube(nullptr)
{
    
}

PopupCalArtifactReinforce::~PopupCalArtifactReinforce(void)
{
    
}

bool PopupCalArtifactReinforce::init(InfoArtifact *info)
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

void PopupCalArtifactReinforce::onEnter()
{
    PopupBase::onEnter();
    
    drawArtifactInfo();
    drawArtifactLevel();
}

#pragma mark - init
void PopupCalArtifactReinforce::initVar()
{
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    _reinforceAfter = _reinforceNow;
}

void PopupCalArtifactReinforce::initUi()
{
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_2.png", Rect::ZERO, Rect(35,170,10,10));
    _spriteContainer->setContentSize(Size(740, 484));
    _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    _spriteContainer->setTag(PopupCalArtifactReinforce::tag_container);
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - 35, _spriteContainer->getContentSize().height - 27) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    itemClose->setScale(0.6f);
    menu->addChild(itemClose);
    
    auto itemReset = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/calculator/btn_refresh.png"), nullptr, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickReset, this));
    itemReset->setPosition( Vec2(_spriteContainer->getContentSize().width - 95, _spriteContainer->getContentSize().height - 27) );
    itemReset->setTouchRect( itemReset->getContentSize() * 2 );
    menu->addChild(itemReset);
    
    _itemMagnifier = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/review/icon_magnifier.png"), nullptr, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickCurrency, this));
    _itemMagnifier->setPosition(Vec2(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 90));
    _itemMagnifier->setScale(1.5f);
    _itemMagnifier->setTouchRect(_itemMagnifier->getContentSize() * 1.5);
    menu->addChild(_itemMagnifier);
    
    
    uiCurrency();
    uiTop();
    uiBottom();
}

#pragma mark - ui
void PopupCalArtifactReinforce::uiCurrency()
{
    _layerContainerCurrency->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_4.png", Rect::ZERO, Rect(27,27,6,6));
    spriteBG->setContentSize(_layerContainerCurrency->getContentSize());
    spriteBG->setPosition(_layerContainerCurrency->getContentSize().width / 2, _layerContainerCurrency->getContentSize().height / 2);
    _layerContainerCurrency->addChild(spriteBG);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::CUBE);
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, _layerContainerCurrency->getContentSize());
    uiCurrency->setPosition(Vec2(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2));
    spriteBG->addChild(uiCurrency);
}

void PopupCalArtifactReinforce::uiTop()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalArtifactReinforce::tag_container);
    
    auto spriteIconBg = Sprite::create("Assets/ui_common/box_quest.png");
    spriteIconBg->setPosition( Vec2(90, spriteContainer->getContentSize().height - 80) );
    spriteContainer->addChild(spriteIconBg);
    
    auto spriteIcon = Sprite::create(_info->getIconPath());
    spriteIcon->setPosition( Vec2(spriteIconBg->getContentSize().width / 2, spriteIconBg->getContentSize().height / 2) );
    spriteIcon->getTexture()->setTexParameters(texParams);
    spriteIcon->setScale(spriteIconBg->getContentSize().width / spriteIcon->getContentSize().width);
    spriteIconBg->addChild(spriteIcon);
    
    _labelReinforce = Label::createWithTTF("+0", GAME_FONT, 20);
    _labelReinforce->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _labelReinforce->setPosition(spriteIconBg->getContentSize().width - 5, spriteIconBg->getContentSize().height);
    _labelReinforce->setColor(Color3B::MAGENTA);
    _labelReinforce->enableOutline(Color4B::BLACK, 2);
    spriteIconBg->addChild(_labelReinforce);
    
    auto labelTitle = Label::createWithTTF(_info->getTitle(), GAME_FONT, 27);
    labelTitle->setColor(COLOR_COIN);
    labelTitle->setPosition( Vec2(spriteIconBg->getPositionX() + spriteIconBg->getContentSize().width / 2 + 10, spriteIconBg->getPositionY() + spriteIconBg->getContentSize().height / 2) );
    labelTitle->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    spriteContainer->addChild(labelTitle);
    
    int nLevel = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    _labelLevel = MafNode::createBmFont(MafUtils::format("Lv. %d (MAX %d)", nLevel, _info->getLevelMax()), GAME_BM_FONT_AL, 22);
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

void PopupCalArtifactReinforce::uiBottom()
{
    auto spriteContainer  = (Sprite*)this->getChildByTag(PopupCalArtifactReinforce::tag_container);
    
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

        _labelLevelPrev = MafNode::createBmFont(MafUtils::format("%d", _reinforceNow), GAME_BM_FONT_AL, 24);
        _labelLevelPrev->setColor(Color3B::GRAY);
        _labelLevelPrev->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        _labelLevelPrev->setPosition( Vec2(260, 140) );
        spriteBox1->addChild(_labelLevelPrev);
        
        _labelLevelAfter = MafNode::createBmFont(MafUtils::format("%d", _reinforceAfter), GAME_BM_FONT_AL, 24);
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
        
        //
        auto spriteIconKey = Sprite::create("Assets/icon/menu_cube.png");
        spriteIconKey->setPosition( Vec2(30, 60) );
        spriteBox1->addChild(spriteIconKey);
        
        _labelNeedCube = MafNode::createBmFont("0", GAME_BM_FONT_AL, 24);
        _labelNeedCube->setColor(Color3B::MAGENTA);
        _labelNeedCube->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        _labelNeedCube->setPosition( Vec2(50, 60) );
        spriteBox1->addChild(_labelNeedCube);
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint( Vec2::ZERO );
    menu->setPosition( Vec2::ZERO );
    spriteContainer->addChild(menu);
    
    int listBtn1Tag[] = {PopupCalArtifactReinforce::tag_btn1_1, PopupCalArtifactReinforce::tag_btn1_10, PopupCalArtifactReinforce::tag_btn1_100, PopupCalArtifactReinforce::tag_btn1_1000};
       std::string listBtn1Text[] = {"+1", "+10", "+100", "+1000"};
       

       for (int i = 0; i < 4; i++ )
       {
           auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
           spriteNormal->setContentSize(Size(76, 76));
           
           auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
           spriteDisable->setContentSize(Size(76, 76));
           
           auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickLevel, this));
           item->setPosition( Vec2(300 + item->getContentSize().width / 2 + item->getContentSize().width * i + 5 * i, 268) );
           item->setLongClick(true);
           item->setTag(listBtn1Tag[i]);
           menu->addChild(item);
           
           auto label = Label::createWithTTF(listBtn1Text[i], GAME_FONT, 22);
           label->setPosition( Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2) );
           item->addChild(label);
       }
    
    int listBtnMinusTag[] = {PopupCalArtifactReinforce::tag_btn1_m1, PopupCalArtifactReinforce::tag_btn1_m10, PopupCalArtifactReinforce::tag_btn1_m100, PopupCalArtifactReinforce::tag_btn1_m1000};
    std::string listBtnMinusText[] = {"-1", "-10", "-100", "-1000"};
    
    for (int i = 0; i < 4; i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_on.png", Rect::ZERO, Rect(5,20,10,10));
        spriteNormal->setContentSize(Size(76, 76));
        
        auto spriteDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_3_1_off.png", Rect::ZERO, Rect(5,20,10,10));
        spriteDisable->setContentSize(Size(76, 76));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickLevel, this));
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
    
    auto maxBtnItem = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteDisable, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickLevel, this));
    maxBtnItem->setPosition( Vec2(300 + maxBtnItem->getContentSize().width / 2 + maxBtnItem->getContentSize().width * 4 + 5 * 4 + 20, 218) );
    maxBtnItem->setTag(PopupCalArtifactReinforce::tag_btn1_max);
    menu->addChild(maxBtnItem);
    
    auto label = Label::createWithTTF("MAX", GAME_FONT, 22);
    label->setPosition( Vec2(maxBtnItem->getContentSize().width / 2, maxBtnItem->getContentSize().height / 2) );
    maxBtnItem->addChild(label);
    
    //
    int listBtn2Tag[] = {PopupCalArtifactReinforce::tag_btn2_cube};
    std::string listBtn2Icon[] = {"Assets/icon/menu_cube.png"};
    
    for (int i = 0; i < sizeof(listBtn2Tag)/sizeof(int); i++ )
    {
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO, Rect(15,25,10,10));
        spriteNormal->setContentSize(Size(194, 76));
        
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_off.png", Rect::ZERO, Rect(15,25,10,10));
        spriteOff->setContentSize(Size(194, 76));
        
        
        auto item = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, spriteOff, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickConfirm, this));
        item->setPosition( Vec2(273 + item->getContentSize().width / 2 + item->getContentSize().width * i + 12 * i, 65) );
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
        
        if ( listBtn2Tag[i] ==  PopupCalArtifactReinforce::tag_btn2_cube )
        {
            _itemCube = item;
        }
    }
    
    // 초기화
    {
        auto spriteReset = ui::Scale9Sprite::create("Assets/ui_common/btn_2_2_on.png", Rect::ZERO, Rect(15,25,10,10));
        spriteReset->setContentSize(Size(116, 65));
        
        auto itemReset = MafNode::MafMenuItemSprite::create(spriteReset, nullptr, nullptr, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickReinforceReset, this));
        itemReset->setPosition( Vec2(itemReset->getContentSize().width/2 + 20, 70) );
        menu->addChild(itemReset);
        
        auto labelReset = MafNode::createBmFont("RESET", GAME_BM_FONT_AL, 24);
        labelReset->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelReset->setPosition( itemReset->getContentSize().width / 2, itemReset->getContentSize().height / 2 );
        labelReset->setColor(Color3B(181, 0, 148));
        itemReset->addChild(labelReset);
        
        auto spriteCurrency = Sprite::create("Assets/icon/icon_jewel.png");
        spriteCurrency->setPosition(25, itemReset->getContentSize().height * 0.25);
        itemReset->addChild(spriteCurrency);
        
        auto labelCurrency = MafNode::createBmFont("4000", GAME_BM_FONT_AL, 22);
        labelCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        labelCurrency->setPosition(40, itemReset->getContentSize().height * 0.25 );
        itemReset->addChild(labelCurrency);
    }
    
}

#pragma mark - set, get, sort, add
void PopupCalArtifactReinforce::drawArtifactInfo()
{
    // reinforce
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    
    std::string strReinforce = "+";
    strReinforce.append(MafUtils::toString(_reinforceNow));
    _labelReinforce->setString(strReinforce);
    
    // effect
    int nLevel = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _info->getIdx());
    double effect = DataManager::GetArtifactEffect(_info->getIdx(), nLevel, _reinforceAfter);
    
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

void PopupCalArtifactReinforce::drawArtifactLevel()
{
    // reinforce
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    
    // need cube
    int diff = _reinforceAfter - _reinforceNow;
    int needCube = diff * DataManager::GetArtifactReinforcePrice(_info->getIdx());
    
    
    // text print
    _labelNeedCube->setString(MafUtils::format("%d", needCube));
    
    _labelLevelPrev->setString(MafUtils::format("%d", _reinforceNow));
    _labelLevelAfter->setString(MafUtils::format("%d", _reinforceAfter));
    _labelLevelDiff->setString(MafUtils::format("+%d", diff));
    if ( diff > 0)
    {
        _labelLevelDiff->setVisible(true);
    }
    
    // button enable
    _itemCube->setEnabled(true);
    
    if ( _reinforceNow == _reinforceAfter )
    {
        _itemCube->setEnabled(false);
    }
    
    if ( _info->isReinforce() == 0 || ItemsMoneyManager::getInstance()->getCube() < needCube )
    {
        _itemCube->setEnabled(false);
    }
}

void PopupCalArtifactReinforce::setCallback(const std::function<void(void)>& callback)
{
    _onCallback = callback;
}

#pragma mark - schedule
void PopupCalArtifactReinforce::buyArtifact(float dt)
{
    // reinforce
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    
    bool isError = false;
    if ( _reinforceNow >= _info->getLevelReinforceMax() )
    {
        _reinforceNow = _info->getLevelReinforceMax();
        _reinforceAfter = _info->getLevelReinforceMax();

        isError = true;
    }

    if ( _reinforceNow >= _reinforceAfter )
    {
        _reinforceAfter = _reinforceNow;

        isError = true;
    }

    if ( isError == false )
    {
        do {

            

        } while (false);
    }


    if ( isError == false )
    {
        drawArtifactInfo();
        drawArtifactLevel();
    }
    else
    {
        unschedule(CC_SCHEDULE_SELECTOR(PopupCalArtifactReinforce::buyArtifact));

        

        //
        PopupLoading::hide();
    }
}


#pragma mark - click
void PopupCalArtifactReinforce::onClickClose(Ref* sender)
{
    hide();
}

void PopupCalArtifactReinforce::onClickReset(cocos2d::Ref* sender)
{
    _reinforceAfter = _reinforceNow;

    if ( _reinforceAfter > _info->getLevelReinforceMax() )
    {
        _reinforceAfter = _info->getLevelReinforceMax();
    }

    if ( _reinforceNow > _reinforceAfter )
    {
        _reinforceAfter = _reinforceNow;
    }

    drawArtifactInfo();
    drawArtifactLevel();
}

void PopupCalArtifactReinforce::onClickLevel(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    if ( item->getTag() == PopupCalArtifactReinforce::tag_btn1_max )
    {
        int nCube = 0;
        int nUserCube = ItemsMoneyManager::getInstance()->getCube();
        
        for ( int i = _reinforceNow; i < _info->getLevelReinforceMax(); i++ )
        {
            nCube += DataManager::GetArtifactReinforcePrice(_info->getIdx());
        
            if ( nCube > nUserCube )
            {
                break;
            }
            
            _reinforceAfter = i + 1;
        }
    }
    else
    {
        switch (item->getTag()) {
            case PopupCalArtifactReinforce::tag_btn1_1:         _reinforceAfter += 1;       break;
            case PopupCalArtifactReinforce::tag_btn1_10:        _reinforceAfter += 10;      break;
            case PopupCalArtifactReinforce::tag_btn1_100:       _reinforceAfter += 100;     break;
            case PopupCalArtifactReinforce::tag_btn1_1000:      _reinforceAfter += 1000;    break;
            case PopupCalArtifactReinforce::tag_btn1_m1:        _reinforceAfter -= 1;       break;
            case PopupCalArtifactReinforce::tag_btn1_m10:       _reinforceAfter -= 10;      break;
            case PopupCalArtifactReinforce::tag_btn1_m100:      _reinforceAfter -= 100;     break;
            case PopupCalArtifactReinforce::tag_btn1_m1000:     _reinforceAfter -= 1000;    break;
        }
    }
        

    if ( _reinforceAfter > _info->getLevelReinforceMax() )
    {
        _reinforceAfter = _info->getLevelReinforceMax();
    }

    if ( _reinforceNow > _reinforceAfter )
    {
        _reinforceAfter = _reinforceNow;
    }

    if ( _reinforceAfter < 0 )
    {
        _reinforceAfter = 0;
    }

    drawArtifactInfo();
    drawArtifactLevel();
}

void PopupCalArtifactReinforce::onClickConfirm(cocos2d::Ref* sender)
{
    // reinforce
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());

    if ( _reinforceAfter > _info->getLevelReinforceMax() )
    {
        _reinforceAfter = _info->getLevelReinforceMax();
    }

    if ( _reinforceNow > _reinforceAfter )
    {
        _reinforceAfter = _reinforceNow;
    }

    if ( _reinforceNow == _reinforceAfter )
    {
        return;
    }


    auto item = (MenuItem*)sender;

    if ( item->getTag() == PopupCalArtifactReinforce::tag_btn2_cube )
    {
        int nowCurrency = ItemsMoneyManager::getInstance()->getCube();
        int needCurrency = (_reinforceAfter - _reinforceNow) * DataManager::GetArtifactReinforcePrice(_info->getIdx());
        
        if ( nowCurrency < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::CUBE);
            popup->show();
            return;
        }
        
        if ( _reinforceAfter >= _info->getLevelReinforceMax() )
        {
            _reinforceAfter = _info->getLevelReinforceMax();
        }
        
        // 재화 감소
        ItemsMoneyManager::getInstance()->setCube(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactReinforce(_ePlace, _info->getIdx(), _reinforceAfter);
    }

    
    // 데이터 초기화
    _reinforceNow = _reinforceAfter;
    
    //
    drawArtifactInfo();
    drawArtifactLevel();
    
    // 콜백
    if ( _onCallback != nullptr )
    {
        _onCallback();
    }
}

void PopupCalArtifactReinforce::onClickReinforceReset(cocos2d::Ref* sender)
{
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    if ( _reinforceNow <= 0 )
    {
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_arti_d1_cube_reset"));
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(PopupCalArtifactReinforce::onClickReinforceResetOk, this));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->show();
}

void PopupCalArtifactReinforce::onClickReinforceResetOk(cocos2d::Ref* sender)
{
    _reinforceNow = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _info->getIdx());
    if ( _reinforceNow <= 0 )
    {
        return;
    }
    
    auto nowCurrency = ItemsMoneyManager::getInstance()->getGem();
    auto needCurrency = 4000;
    
    if ( nowCurrency < needCurrency )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        return;
    }
    
    UserInfoManager::getInstance()->setArtifactReinforce(_ePlace, _info->getIdx(), 0);
    ItemsMoneyManager::getInstance()->setCube(_reinforceNow);
    ItemsMoneyManager::getInstance()->setGem(-needCurrency);
    _reinforceNow = 0;
    _reinforceAfter = 0;
    
    //
    drawArtifactInfo();
    drawArtifactLevel();
    
    // 콜백
    if ( _onCallback != nullptr )
    {
        _onCallback();
    }
}

void PopupCalArtifactReinforce::onClickCurrency(cocos2d::Ref* sender)
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




