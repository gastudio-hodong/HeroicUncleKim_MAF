//
//  PopupReviveDetail.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 28/06/2019.
//

#include "PopupReviveDetail.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupReviveDetail* PopupReviveDetail::create(int nType)
{
    PopupReviveDetail* pRet = new(std::nothrow) PopupReviveDetail();
    if ( pRet && pRet->init(nType) )
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

PopupReviveDetail::PopupReviveDetail(void) :
_bAutoSave(false),
_nType(0),

_onCallbackResult(nullptr),

_layerContainerContent(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupReviveDetail::~PopupReviveDetail(void)
{
    
}

bool PopupReviveDetail::init(int nType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nType = nType;
    
    _bAutoSave = UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_NORMAL",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_REVIVE_NORMAL", _bAutoSave);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupReviveDetail::initVar()
{
    
}

void PopupReviveDetail::initUi()
{
    
    //
    Size size = Size(550, 75);
    size.height += 370;     // content
    size.height += 88;     // content ads
    size.height += 88;     // button
    size.height += 25;      // ton padding
    size.height += 25;      // middle padding
    size.height += 25;      // buttom padding
    _spriteContainer->setContentSize( size );
    
    //
    std::string strTitle = "";
    if ( _nType == 0 )
    {
        strTitle = GAME_TEXT("t_ui_revival_1");
    }
    else if ( _nType == 1 )
    {
        strTitle = GAME_TEXT("t_ui_revival_2");
    }
    else
    {
        strTitle = GAME_TEXT("t_ui_revival_3");
    }
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupReviveDetail::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    menu->addChild(itemClose);
    
    //
    _layerContainerContent = Layer::create();
    _layerContainerContent->setContentSize( Size(size.width - 50, 390) );
    _layerContainerContent->setIgnoreAnchorPointForPosition(false);
    _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 5) );
    _spriteContainer->addChild(_layerContainerContent);
    
    //
    _layerContainerContentAds = Layer::create();
    _layerContainerContentAds->setContentSize( Size(498, 88) );
    _layerContainerContentAds->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentAds->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContentAds->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _layerContainerContent->getPositionY() - _layerContainerContent->getContentSize().height) );
    _spriteContainer->addChild(_layerContainerContentAds);
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 88) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    
    //
    uiContent();
    uiContentAds();
    uiButton();
    uiAutoSave();
}

#pragma mark - ui
void PopupReviveDetail::uiContent()
{
    _layerContainerContent->removeAllChildren();
    
    std::string strTitle = "";
    if ( _nType == 0 )
    {
        strTitle = GAME_TEXT("t_ui_revival_1");
    }
    else if ( _nType == 1 )
    {
        strTitle = GAME_TEXT("t_ui_revival_2");
    }
    else
    {
        strTitle = GAME_TEXT("t_ui_revival_3");
    }

    auto labelText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_msg_12", strTitle.c_str()), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height);
    labelText->setDimensions(_layerContainerContent->getContentSize().width, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelText);
    
    Vec2 posBG = Vec2::ZERO;
    // 만능 포인트
    {
        int levelPoint = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_50);
        int levelPointAdd = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_51);

        int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_50);
        int reinforceAdd = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_51);

        std::string strTempMsg = GAME_TEXT("t_ui_revive_msg_16");;
        if ( levelPoint != 0 )
        {
            int gain = UserInfoManager::getInstance()->getFloor(_ePlace) / DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_50, levelPoint, reinforce);
            int gainToday = UserInfoManager::getInstance()->getArtifactPointGain(_ePlace);
            int gainTodayMax = C_MAX_PRESTIGE_POINT + DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_51, levelPointAdd, reinforceAdd);
            
            //모험 유물 적용
            double advtRelicBuff = AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::REVIVE_POINT_INCREASE);
            int gainBonus = gainTodayMax * advtRelicBuff * 0.01;
            
            gainTodayMax += gainBonus;
            
            int gainTemp = gainTodayMax - gainToday;
            if ( gain > gainTemp )
                gain = gainTemp;
            
            gain = MAX(gain, 0);
            
            strTempMsg = GAME_TEXTFORMAT("t_ui_revive_msg_14", gain, gainToday, gainTodayMax);
        }
            
        auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_7.png");
        sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBG->setContentSize(Size(498, 100));
        sprBG->setPosition(labelText->getPosition() + Vec2::DOWN * (labelText->getContentSize().height + 5));
        _layerContainerContent->addChild(sprBG);
        posBG = sprBG->getPosition();
        {
            auto btnIcon = ui::Button::create("Assets/icon_artifact/artifact_50.png");
            btnIcon->setScale(2);
            btnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            btnIcon->setPosition(Vec2(0, sprBG->getContentSize().height/2));
            btnIcon->setZoomScale(0.05f);
            btnIcon->addClickEventListener([=](Ref*){
                PopupToast::show(GAME_TEXT("t_ui_revive_detail_toast_1"), getContentSize().width/2, getContentSize().height/2);
            });
            sprBG->addChild(btnIcon);
            
            
            auto labelTempMsg = Label::createWithTTF(strTempMsg, GAME_FONT, 20);
            labelTempMsg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            labelTempMsg->setPosition(btnIcon->getPosition() + Vec2::RIGHT * (btnIcon->getContentSize().width * btnIcon->getScale()));
            labelTempMsg->setLineSpacing(-5);
            labelTempMsg->setDimensions(sprBG->getContentSize().width - btnIcon->getContentSize().width * btnIcon->getScale() - 10, sprBG->getContentSize().height - 10);
            labelTempMsg->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelTempMsg->setOverflow(Label::Overflow::SHRINK);
            sprBG->addChild(labelTempMsg);
        }
    }

    // 피버타임
    {
        int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_52);
        int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_52);
        float feverTime = C_MAX_FEVER_TIME + DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_52, level, reinforce);
        
        std::string strTempMsg = GAME_TEXTFORMAT("t_ui_revive_msg_18", (int)feverTime, UserInfoManager::getInstance()->getFeverDay(), C_MAX_FEVER_DAY);
        
        auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_7.png");
        sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBG->setContentSize(Size(498, 100));
        sprBG->setPosition(posBG + Vec2::DOWN * (sprBG->getContentSize().height + 15));
        posBG = sprBG->getPosition();
        _layerContainerContent->addChild(sprBG);
        {
            auto btnIcon = ui::Button::create("Assets/icon_artifact/artifact_52.png");
            btnIcon->setScale(2);
            btnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            btnIcon->setPosition(Vec2(0, sprBG->getContentSize().height/2));
            btnIcon->setZoomScale(0.05f);
            btnIcon->addClickEventListener([=](Ref*){
                PopupToast::show(GAME_TEXT("t_ui_revive_detail_toast_2"), getContentSize().width/2, getContentSize().height/2);
            });
            sprBG->addChild(btnIcon);
            
            auto labelTempMsg = Label::createWithTTF(strTempMsg, GAME_FONT, 20);
            labelTempMsg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            labelTempMsg->setPosition(btnIcon->getPosition() + Vec2::RIGHT * (btnIcon->getContentSize().width * btnIcon->getScale()));
            labelTempMsg->setLineSpacing(-5);
            labelTempMsg->setDimensions(sprBG->getContentSize().width - btnIcon->getContentSize().width * btnIcon->getScale() - 10, sprBG->getContentSize().height - 10);
            labelTempMsg->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelTempMsg->setOverflow(Label::Overflow::SHRINK);
            sprBG->addChild(labelTempMsg);
        }
    }

    // 강화 큐브
    {
        int per = 30;
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_8))
            per = 35;
        std::string strTempMsg = GAME_TEXTFORMAT("t_ui_revive_msg_33", per);
        bool isCondition = true;
        if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_1_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_3_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_4_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_8_EVOL1) < 99999 )
        {
            strTempMsg = GAME_TEXT("t_ui_revive_msg_32");
            isCondition = false;
        }
        else
        {
            float per = 0.7f;
            if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_8))
                per = 0.65f;
            
            int nFloorCondition = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) * per;
            strTempMsg += "\n(" + GAME_TEXTFORMAT("t_ui_revive_msg_35", nFloorCondition) + ")";
            strTempMsg += "\n" + GAME_TEXTFORMAT("t_ui_revive_msg_30", UserInfoManager::getInstance()->getCubeDay(), C_MAX_CUBE_DAY);
        }
        
        auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_7.png");
        sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBG->setContentSize(Size(498, 100));
        sprBG->setPosition(posBG + Vec2::DOWN * (sprBG->getContentSize().height + 15));
        _layerContainerContent->addChild(sprBG);
        {
            auto btnIcon = ui::Button::create("Assets/icon/menu_cube.png");
            btnIcon->setScale(2);
            btnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            btnIcon->setPosition(Vec2(0, sprBG->getContentSize().height/2));
            btnIcon->setZoomScale(0.05f);
            btnIcon->addClickEventListener([=](Ref*){
                PopupToast::show(GAME_TEXT("t_ui_revive_detail_toast_3"), getContentSize().width/2, getContentSize().height/2);
            });
            sprBG->addChild(btnIcon);
            if(isCondition && UserInfoManager::getInstance()->getCubeDay() > 0)
            {
                auto lbCount = Label::createWithTTF(std::to_string(UserInfoManager::getInstance()->getCubeTotalGain()), GAME_FONT, 20);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbCount->setPosition(btnIcon->getContentSize().width/2 * btnIcon->getScale(), -5);
                sprBG->addChild(lbCount);
            }
            

            auto labelTempMsg = Label::createWithTTF(strTempMsg, GAME_FONT, 20);
            labelTempMsg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            labelTempMsg->setPosition(btnIcon->getPosition() + Vec2::RIGHT * (btnIcon->getContentSize().width * btnIcon->getScale()));
            labelTempMsg->setLineSpacing(-5);
            labelTempMsg->setDimensions(sprBG->getContentSize().width - btnIcon->getContentSize().width * btnIcon->getScale() - 10, sprBG->getContentSize().height - 10);
            labelTempMsg->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelTempMsg->setOverflow(Label::Overflow::SHRINK);
            sprBG->addChild(labelTempMsg);
        }
    }
}

void PopupReviveDetail::uiContentAds()
{
    _layerContainerContentAds->removeAllChildren();
    
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png", Rect::ZERO, Rect(0,0,20,20));
    spriteBg->setContentSize(Size(498, 88));
    spriteBg->setPosition(_layerContainerContentAds->getContentSize().width / 2, _layerContainerContentAds->getContentSize().height / 2);
    _layerContainerContentAds->addChild(spriteBg);
    
    //
    int nMaxAds = 5;
    int nCountAds = UserInfoManager::getInstance()->getRevivalAds(E_PLACE::DG_NORMAL);
    if ( nCountAds > nMaxAds)
    {
        nCountAds = nMaxAds;
    }
    
    double nPercent = (double)nCountAds / nMaxAds * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_34"), GAME_FONT, 16);
    labelText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    labelText->setPosition(10, spriteBg->getContentSize().height / 2);
    labelText->setDimensions(280, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    spriteBg->addChild(labelText);
    
    auto spriteProgressBg = Sprite::create("Assets/ui/revive/revive_btn_progress1_1.png");
    spriteProgressBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    spriteProgressBg->setPosition(10, spriteBg->getContentSize().height / 2);
    spriteBg->addChild(spriteProgressBg);
    {
        ProgressTimer* progress = ProgressTimer::create(Sprite::create("Assets/ui/revive/revive_btn_progress1_2.png"));
        progress->setPosition(spriteProgressBg->getContentSize()/2);
        progress->setMidpoint(Vec2(0,0.5));
        progress->setBarChangeRate(Vec2(1,0));
        progress->setType(ProgressTimer::Type::BAR);
        progress->setPercentage(nPercent);
        spriteProgressBg->addChild(progress);
        
        auto label = Label::createWithTTF(MafUtils::format("%d/%d", nCountAds, nMaxAds), GAME_FONT, 20);
        label->setPosition(spriteProgressBg->getContentSize().width / 2, spriteProgressBg->getContentSize().height / 2);
        spriteProgressBg->addChild(label);
    }
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBg->addChild(menu);
    
    auto itemFree = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/revive/revive_btn_play_3.png"), Sprite::create("Assets/ui/revive/revive_btn_play_3.png"), nullptr, CC_CALLBACK_1(PopupReviveDetail::onClickFree, this));
    itemFree->setPosition(spriteBg->getContentSize().width - itemFree->getContentSize().width / 2, spriteBg->getContentSize().height / 2);
    menu->addChild(itemFree);
    {
        auto label1 = Label::createWithTTF(GAME_TEXT("t_ui_revive_free"), GAME_FONT, 20);
        label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        label1->setPosition(itemFree->getContentSize().width / 2, itemFree->getContentSize().height / 2);
        label1->setColor(COLOR_COIN);
        itemFree->addChild(label1);
        
        auto label2 = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_key", 3), GAME_FONT, 18);
        label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        label2->setPosition(itemFree->getContentSize().width / 2, itemFree->getContentSize().height / 2);
        itemFree->addChild(label2);
    }
    
    if ( nCountAds < nMaxAds)
    {
        itemFree->setEnabled(false);
        itemFree->getNormalImage()->setColor(Color3B(100, 100, 100));
    }
    
}

void PopupReviveDetail::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);

    //
    auto spriteYes = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteYes->setContentSize(Size(240, 88));
    auto spriteSelectYes = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectYes->setContentSize(Size(240, 88));
    
    auto itemYes = MafNode::MafMenuItemSprite::create(spriteYes, spriteSelectYes, nullptr, CC_CALLBACK_1(PopupReviveDetail::onClickYes, this));
    menu->addChild(itemYes);
    {
        if ( _nType == 2 || _nType == 1 )
        {
            auto objItems = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
            if ( _nType == 2 )
                objItems->setCount(1000);
            else if ( _nType == 1 )
                objItems->setCount(500);
            
            auto spriteIcon = Sprite::create(objItems->getPath());
            spriteIcon->setPosition(itemYes->getContentSize().width * 0.3, itemYes->getContentSize().height / 2);
            itemYes->addChild(spriteIcon);
            
            auto labelCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 24);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCount->setPosition(itemYes->getContentSize().width * 0.47, itemYes->getContentSize().height / 2);
            itemYes->addChild(labelCount);
        }
        else
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
            label->setPosition( Vec2(itemYes->getContentSize().width / 2, itemYes->getContentSize().height / 2) );
            itemYes->addChild(label);
        }
    }
    
    //
    auto spriteAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_7_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteAds->setContentSize(Size(240, 88));
    auto spriteSelectAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_7_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectAds->setContentSize(Size(240, 88));
    
    auto itemAds = MafNode::MafMenuItemSprite::create(spriteAds, spriteSelectAds, nullptr, CC_CALLBACK_1(PopupReviveDetail::onClickAds, this));
    menu->addChild(itemAds);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_ad_1_1.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteIcon->setPosition(30, spriteAds->getContentSize().height);
        itemAds->addChild(spriteIcon);
        
        auto sprMemo = Sprite::create("Assets/ui_common/base_memo_1_1.png");
        sprMemo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprMemo->setPosition(Vec2(itemAds->getContentSize().width - 15, itemAds->getContentSize().height/2));
        itemAds->addChild(sprMemo);
        {
            auto spriteIconKey = Sprite::create("Assets/icon/menu_key.png");
            spriteIconKey->setPosition(40, sprMemo->getContentSize().height - sprMemo->getContentSize().height/4);
            sprMemo->addChild(spriteIconKey);

            std::string strMsg1 = "+10%";
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_14) )
            {
                strMsg1 = "+30%";
            }

            auto label1 = Label::createWithTTF(strMsg1, GAME_FONT, 16);
            label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label1->setPosition(spriteIconKey->getPosition() + Vec2::RIGHT * spriteIconKey->getContentSize().width/3);
            label1->setDimensions(sprMemo->getContentSize().width/2, label1->getContentSize().height);
            label1->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            label1->setOverflow(cocos2d::Label::Overflow::SHRINK);
            label1->setColor(Color3B::BLACK);
            sprMemo->addChild(label1);

            auto spriteIconCube = Sprite::create("Assets/icon/menu_cube.png");
            spriteIconCube->setPosition(40, sprMemo->getContentSize().height/4);
            sprMemo->addChild(spriteIconCube);
            
            auto label2 = Label::createWithTTF("+100%", GAME_FONT, 16);
            label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label2->setPosition(spriteIconCube->getPosition() + Vec2::RIGHT * spriteIconCube->getContentSize().width/3);
            label2->setDimensions(sprMemo->getContentSize().width/2, label2->getContentSize().height);
            label2->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            label2->setOverflow(cocos2d::Label::Overflow::SHRINK);
            label2->setColor(Color3B::BLACK);
            sprMemo->addChild(label2);
        }
        
        if ( _nType == 2 || _nType == 1 )
        {
            auto objItems = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
            if ( _nType == 2 )
                objItems->setCount(1000);
            else if ( _nType == 1 )
                objItems->setCount(500);
            
            auto sprGem = Sprite::create(objItems->getPath());
            sprGem->setPosition(30, itemAds->getContentSize().height / 4);
            itemAds->addChild(sprGem);
            
            auto labelCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 24);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCount->setPosition(sprGem->getPosition() + Vec2::RIGHT * sprGem->getContentSize().width/3 + Vec2::UP * 5);
            labelCount->setDimensions(40, labelCount->getContentSize().height);
            labelCount->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            labelCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
            itemAds->addChild(labelCount);
        }
        else
        {
            auto labelCount = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCount->setPosition(50, itemAds->getContentSize().height / 4 + 5);
            labelCount->setDimensions(40, labelCount->getContentSize().height);
            labelCount->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            labelCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
            itemAds->addChild(labelCount);
        }
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}
void PopupReviveDetail::uiAutoSave()
{
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
    }
    
    auto btnAutoSaveCheck = ui::Button::create(strCheckPath);
    btnAutoSaveCheck->addClickEventListener(CC_CALLBACK_1(PopupReviveDetail::onClickAutoSaveCheck, this));
    btnAutoSaveCheck->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnAutoSaveCheck->setScale(0.8f);
    btnAutoSaveCheck->setZoomScale(0.05f);
    btnAutoSaveCheck->setPosition(Vec2(_spriteContainer->getContentSize().width, -10));
    _spriteContainer->addChild(btnAutoSaveCheck);
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_auto_save_check"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    labelText->setPosition(btnAutoSaveCheck->getPosition() + Vec2::LEFT * (btnAutoSaveCheck->getContentSize().width * btnAutoSaveCheck->getScale() + 5));
    labelText->setDimensions(labelText->getContentSize().width, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::RIGHT, TextVAlignment::TOP);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelText);
}

#pragma mark - click
void PopupReviveDetail::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupReviveDetail::onClickYes(cocos2d::Ref* sender)
{
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult(_nType, 1);
    }
    
    if ( _bAutoSave )
    {
        SaveManager::getInstance()->requestSave(nullptr);
    }
    
    hide();
}

void PopupReviveDetail::onClickAds(cocos2d::Ref* sender)
{
    //광고 쿨타임 진행중 체크
    if(AdsUtilsManager::getInstance()->getAdsCooldown() > 0 && CashManager::getInstance()->isAdsfreeTime() == false)
    {
        return;
    }
    
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult(_nType, 2);
    }
    
    if ( _bAutoSave )
    {
        SaveManager::getInstance()->requestSave(nullptr);
    }
    
    hide();
}

void PopupReviveDetail::onClickFree(cocos2d::Ref* sender)
{
    if ( _onCallbackResult != nullptr )
    {//1 : 강화환생
        _onCallbackResult(1, 3);
    }
    
    if ( _bAutoSave )
    {
        SaveManager::getInstance()->requestSave(nullptr);
    }
    
    hide();
}

void PopupReviveDetail::onClickAutoSaveCheck(Ref *sender)
{
    auto btn = (ui::Button*)sender;
    _bAutoSave = !UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_NORMAL",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_REVIVE_NORMAL", _bAutoSave);
    
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_auto_save_pop_reconfirmation"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    btn->getRendererNormal()->setTexture(strCheckPath);
    
}

void PopupReviveDetail::setCallbackResult(const std::function<void(int, int)>& callback)
{
    _onCallbackResult = callback;
}


#pragma mark - callback
