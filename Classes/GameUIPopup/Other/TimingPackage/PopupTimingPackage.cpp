//
//  PopupTimingPackage.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/12.
//

#include "PopupTimingPackage.h"

#include "ManagerGame/UserInfoManager.h"
#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/Buff/PopupBuffNewbie.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

PopupTimingPackage* PopupTimingPackage::create(InfoTimingPackage* infoTimingPackage, bool isScaedule)
{
    PopupTimingPackage* pRet = new(std::nothrow) PopupTimingPackage();
    if ( pRet && pRet->init(infoTimingPackage, isScaedule) )
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

PopupTimingPackage::PopupTimingPackage(void) :
_fTimer(0),
_fHideTimer(0),
_layerContainerReward(nullptr),
_layerContainerContents(nullptr),
_layerContainerButton(nullptr),

_lbContent(nullptr),

_callbackHide(nullptr),

_infoTimingPackage(nullptr)
{
    
}

PopupTimingPackage::~PopupTimingPackage(void)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupTimingPackage::scheduleUpdate), this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupTimingPackage::scheduleTimer), this);
}

bool PopupTimingPackage::init(InfoTimingPackage* infoTimingPackage, bool isScaedule)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _infoTimingPackage = infoTimingPackage;
    
    // init
    initVar();
    initUi();

    if(isScaedule == true)
    {
        if(!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(PopupTimingPackage::scheduleUpdate), this))
        {
            Director::getInstance()->getScheduler()->schedule(schedule_selector(PopupTimingPackage::scheduleUpdate), this, 1 , kRepeatForever, 1, false);
        }
    }
    
    return true;
}

void PopupTimingPackage::scheduleUpdate(float dt)
{
    _fTimer += dt;
    
    if(_fTimer >= 1)
    {
        switch(_infoTimingPackage->getType())
        {
            case E_TIMING_PACKAGE_TYPE::FREE:
            case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
            {
                bool isSchedule = TimingPackageManager::getInstance()->isEndTiming(_infoTimingPackage);
                if(isSchedule)
                {
                    uiButton();
                    Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupTimingPackage::scheduleUpdate), this);
                    
                    if(!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(PopupTimingPackage::scheduleTimer), this))
                    {
                        Director::getInstance()->getScheduler()->schedule(schedule_selector(PopupTimingPackage::scheduleTimer), this, 1 , kRepeatForever, 1, false);
                    }
                }
    
            }break;
            case E_TIMING_PACKAGE_TYPE::PURCHASE:
            {
                if(_infoTimingPackage->getEndTimer() <= 0 )
                {
                    Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupTimingPackage::scheduleUpdate), this);
                    onHide();
                    return;
                }
            }break;
        }
            setContentText();
    }
}

void PopupTimingPackage::scheduleTimer(float dt)
{
    _fHideTimer += dt;
    
    if(_fHideTimer >= 5)
    {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupTimingPackage::scheduleTimer), this);
        
        onHide();
    }
}
#pragma mark -init

void PopupTimingPackage::initVar()
{
    Size size(500,564);
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(_infoTimingPackage->getName(), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setDimensions(_spriteContainer->getContentSize().width - 100, labelTitle->getContentSize().height);
    labelTitle->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
    labelTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize(Size(_spriteContainer->getContentSize().width, 110));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerContents);
    
    _layerContainerReward = LayerColor::create();
    _layerContainerReward->setContentSize(Size(_spriteContainer->getContentSize().width, 275));
    _layerContainerReward->setIgnoreAnchorPointForPosition(false);
    _layerContainerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerReward->setPosition(Vec2(_layerContainerContents->getPosition()) + Vec2::DOWN * _layerContainerContents->getContentSize().height);
    _spriteContainer->addChild(_layerContainerReward);
    
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize(Size(_spriteContainer->getContentSize().width, 100));
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerButton->setPosition(Vec2(_layerContainerReward->getPosition()) + Vec2::DOWN * _layerContainerReward->getContentSize().height);
    _spriteContainer->addChild(_layerContainerButton);
    
    // 125 * 57(500 * 228)
    auto uiBanner = ui::Layout::create();
    uiBanner->setContentSize(Size(500, 228));
    uiBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBanner->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height));
    _spriteContainer->addChild(uiBanner);
    {
        auto uiBG = ui::ImageView::create(_infoTimingPackage->getBannerPath());
        uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBG->setPosition(Vec2(uiBanner->getContentSize().width / 2, 0));
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiBG->setScale(4);
        uiBanner->addChild(uiBG);
        
        if ( _infoTimingPackage->getStartConditionTypeParams() != 0 )
        {
            auto uiTypeBG = ui::ImageView::create("Assets/ui_common/icon_timing_box_2.png");
            uiTypeBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiTypeBG->setPosition(Vec2(uiBanner->getContentSize().width - 10, 10));
            uiBanner->addChild(uiTypeBG);
            
            auto uiType = ui::CText::create(MafUtils::format("%dth", _infoTimingPackage->getStartConditionTypeParams()), GAME_FONT, 24);
            uiType->setPositionType(ui::Widget::PositionType::PERCENT);
            uiType->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiType->setTextColor(Color4B::YELLOW);
            uiTypeBG->addChild(uiType);
        }
    }
    
}

void PopupTimingPackage::initUi()
{
    uiContents();
    uiReward();
    uiButton();
}

void PopupTimingPackage::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    _lbContent = Label::createWithTTF("", GAME_FONT, 22);
    _lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbContent->setPosition(Vec2(_layerContainerContents->getContentSize()/2));
    _lbContent->setDimensions(_layerContainerContents->getContentSize().width - 10, _layerContainerContents->getContentSize().height - 10);
    _lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
    _lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _layerContainerContents->addChild(_lbContent);
    setContentText();
    
    
    
}

void PopupTimingPackage::uiReward()
{
    _layerContainerReward->removeAllChildren();
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png");
    sprBg->setContentSize(Size(422,_layerContainerReward->getContentSize().height));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBg->setPosition(Vec2(_layerContainerReward->getContentSize().width / 2, _layerContainerReward->getContentSize().height));
    _layerContainerReward->addChild(sprBg);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _layerContainerReward->addChild(menu);
    
    auto listOriginItems = _infoTimingPackage->getListItems();
    Vector<InfoItems*> listItems1;
    Vector<InfoItems*> listItems2;
    listItems1.clear();
    listItems2.clear();
    
    if(_infoTimingPackage->getIdx() == 1)
    {
        auto item = InfoItems::create(-1);
        item->setPath("Assets/icon/icon_newbie_1_1.png");
        item->setTextKey("t_ui_newbie_buff");
        listOriginItems.insert(0, item);
        
    }
    
    for(int i = 0; i < listOriginItems.size(); ++i)
    {
        if(i < 3)listItems1.pushBack(listOriginItems.at(i));
        else listItems2.pushBack(listOriginItems.at(i));
    }
    
    
    float posY = _layerContainerReward->getContentSize().height - _layerContainerReward->getContentSize().height / 4;
    
    if(listItems2.size() == 0)
        posY = _layerContainerReward->getContentSize().height/2;
    
    float posX = _layerContainerReward->getContentSize().width/4;
    if(listItems1.size() % 2 == 0)
    {
        posX = _layerContainerReward->getContentSize().width/3;
    }
    float posXInterval = posX;
    
    for(auto item : listItems1)
    {
        if(listItems1.size() == 1)
            posX = _layerContainerReward->getContentSize().width/2;
        
        auto sprIcon = Sprite::create(item->getPath());
        sprIcon->getTexture()->setTexParameters(texParams);
        sprIcon->setScale(58 / sprIcon->getContentSize().width);
        
        auto itemSprIcon = MafNode::MafMenuItemSprite::create(sprIcon, nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
        itemSprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemSprIcon->setPosition(posX, posY);
        itemSprIcon->setTouchRect(itemSprIcon->getContentSize() * 1.5f);
        if(item->getIdx() == -1)
            itemSprIcon->setName("NEWBIE");
        menu->addChild(itemSprIcon);
        
        auto lbContent = Label::createWithTTF(item->getText(), GAME_FONT, 15);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbContent->setPosition(itemSprIcon->getPosition() + Vec2(0, itemSprIcon->getContentSize().height / 2));
        lbContent->setDimensions(itemSprIcon->getContentSize().width + 40, lbContent->getContentSize().height);
        lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
        lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _layerContainerReward->addChild(lbContent);
        
        std::string strCount = item->getCount();
        if(MafUtils::toInt(strCount) > 10000)
             MafUtils::convertNumberToShort(strCount);
        auto lbCount = MafNode::createBmFont(strCount, GAME_BM_FONT_AL, 20);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbCount->setPosition(itemSprIcon->getPosition() - Vec2(0, itemSprIcon->getContentSize().height / 2 - 5));
        _layerContainerReward->addChild(lbCount);
        
        posX += posXInterval;
    }
    
    posX = _layerContainerReward->getContentSize().width/4;
    if(listItems2.size() % 2 == 0)
    {
        posX = _layerContainerReward->getContentSize().width/3;
    }
    posXInterval = posX;
    
    posY = _layerContainerReward->getContentSize().height / 4;
    
    for(auto item : listItems2)
    {
        if(listItems2.size() == 1)
            posX = _layerContainerReward->getContentSize().width/2;
        
        auto sprIcon = Sprite::create(item->getPath());
        sprIcon->getTexture()->setTexParameters(texParams);
        sprIcon->setScale(58 / sprIcon->getContentSize().width);
        
        auto itemSprIcon = MafNode::MafMenuItemSprite::create(sprIcon, nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
        itemSprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemSprIcon->setPosition(posX, posY);
        itemSprIcon->setTouchRect(itemSprIcon->getContentSize() * 1.5f);
        if(item->getIdx() == -1)
            itemSprIcon->setName("NEWBIE");
        menu->addChild(itemSprIcon);
        
        auto lbContent = Label::createWithTTF(item->getText(), GAME_FONT, 15);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbContent->setPosition(itemSprIcon->getPosition() + Vec2(0, itemSprIcon->getContentSize().height / 2));
        lbContent->setDimensions(itemSprIcon->getContentSize().width + 40, lbContent->getContentSize().height);
        lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
        lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _layerContainerReward->addChild(lbContent);
        
        std::string strCount = item->getCount();
        if(MafUtils::toInt(strCount) > 10000)
             MafUtils::convertNumberToShort(strCount);
        auto lbCount = MafNode::createBmFont(strCount, GAME_BM_FONT_AL, 20);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbCount->setPosition(itemSprIcon->getPosition() - Vec2(0, itemSprIcon->getContentSize().height / 2 - 5));
        _layerContainerReward->addChild(lbCount);
        
        posX += posXInterval;
    }
}

void PopupTimingPackage::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _layerContainerButton->addChild(menu);
    
    if(TimingPackageManager::getInstance()->isEndTiming(_infoTimingPackage)
       || _infoTimingPackage->isPurchase())
    {
        auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_5.png");
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBox->setPosition(Vec2(_layerContainerButton->getContentSize().width/2, _layerContainerButton->getContentSize().height));
        sprBox->setContentSize(Size(500, 90));
        _layerContainerButton->addChild(sprBox);
        {
            auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_timing_reward_1"), GAME_FONT, 24);
            
            lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbContent->setPosition(Vec2(sprBox->getContentSize()/2));
            lbContent->setDimensions(sprBox->getContentSize().width - 10, sprBox->getContentSize().height - 10);
            lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
            lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
            sprBox->addChild(lbContent);
        }
    }
    else
    {
        switch (_infoTimingPackage->getType())
        {
            case E_TIMING_PACKAGE_TYPE::FREE:
            {
                MafNode::MafMenuItemSprite* itemBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_off.png"), nullptr, [&, itemBtn](Ref*){
                    
                    Vec2 posToast = _layerContainerButton->getParent()->convertToWorldSpace(_layerContainerButton->getPosition());
                    posToast.y += _layerContainerButton->getContentSize().height * 0.5f ;
                    PopupToast::showImmediately(GAME_TEXT("t_ui_timing_reward_2"), posToast.x, posToast.y);
                });
                itemBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                itemBtn->setPosition(_layerContainerButton->getContentSize()/2);
                menu->addChild(itemBtn);
                {
                    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_5"), GAME_FONT, 24);
                    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbContent->setPosition(itemBtn->getContentSize()/2);
                    lbContent->setDimensions(itemBtn->getContentSize().width - 10, itemBtn->getContentSize().height - 10);
                    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemBtn->addChild(lbContent);
                }
            }
                break;
            case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
            {
                auto itemBuy = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_on.png"), nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
                itemBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                itemBuy->setPosition(30, _layerContainerButton->getContentSize().height/2);
                itemBuy->setName("BUY");
                menu->addChild(itemBuy);
                {
                    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_13"), GAME_FONT, 24);
                    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbContent->setPosition(Vec2(itemBuy->getContentSize().width/2, itemBuy->getContentSize().height - itemBuy->getContentSize().height/3));
                    lbContent->setDimensions(itemBuy->getContentSize().width - 10, lbContent->getContentSize().height);
                    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemBuy->addChild(lbContent);
                    
                    auto lbPrice = Label::createWithTTF("$40", GAME_FONT, 24);
                    lbPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbPrice->setPosition(Vec2(itemBuy->getContentSize().width/2, itemBuy->getContentSize().height/3));
                    lbPrice->setDimensions(itemBuy->getContentSize().width - 10, lbContent->getContentSize().height);
                    lbPrice->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemBuy->addChild(lbPrice);
                    
                    auto objProduct = CashManager::getInstance()->getProduct(_infoTimingPackage->getCashIdx());
                    if ( objProduct != nullptr )
                    {
                        lbPrice->setString(objProduct->getPrice());
                    }
                }
                
                auto sprClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_9_on.png");
                sprClose->setContentSize(Size(212, 89));
                auto itemClose = MafNode::MafMenuItemSprite::create(sprClose, nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
                itemClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                itemClose->setPosition(_layerContainerButton->getContentSize().width - 30, _layerContainerButton->getContentSize().height/2);
                itemClose->setName("CLOSE");
                menu->addChild(itemClose);
                {
                    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_timing_reward_2"), GAME_FONT, 24);
                    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbContent->setPosition(Vec2(itemClose->getContentSize()/2));
                    lbContent->setDimensions(itemClose->getContentSize().width - 10, itemClose->getContentSize().height - 10);
                    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemClose->addChild(lbContent);
                }
            }
                break;
            case E_TIMING_PACKAGE_TYPE::PURCHASE:
            {
                auto itemBuy = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_on.png"), nullptr, CC_CALLBACK_1(PopupTimingPackage::onClick, this));
                itemBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                itemBuy->setPosition(_layerContainerButton->getContentSize().width/2, _layerContainerButton->getContentSize().height/2);
                itemBuy->setName("BUY");
                menu->addChild(itemBuy);
                {
                    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_artifact_title_discount_1"), GAME_FONT, 24);
                    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbContent->setPosition(Vec2(itemBuy->getContentSize().width/2, itemBuy->getContentSize().height - itemBuy->getContentSize().height/3));
                    lbContent->setDimensions(itemBuy->getContentSize().width - 10, lbContent->getContentSize().height);
                    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemBuy->addChild(lbContent);
                    
                    auto lbPrice = Label::createWithTTF("$40", GAME_FONT, 24);
                    lbPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    lbPrice->setPosition(Vec2(itemBuy->getContentSize().width/2, itemBuy->getContentSize().height/3));
                    lbPrice->setDimensions(itemBuy->getContentSize().width - 10, lbContent->getContentSize().height);
                    lbPrice->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
                    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
                    itemBuy->addChild(lbPrice);
                    
                    auto objProduct = CashManager::getInstance()->getProduct(_infoTimingPackage->getCashIdx());
                    if ( objProduct != nullptr )
                    {
                        lbPrice->setString(objProduct->getPrice());
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}

void PopupTimingPackage::setContentText()
{
    std::string strContent = "";
    
    switch (_infoTimingPackage->getType()) {
        case E_TIMING_PACKAGE_TYPE::FREE:
        case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
        {
            switch (_infoTimingPackage->getEndConditionType())
            {
                case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
                {
                    strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getHighFloor(DG_NORMAL), _infoTimingPackage->getCompleteValue());
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
                {
                    strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), PrisonManager::getInstance()->getDungeonCount(), _infoTimingPackage->getCompleteValue());
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
                {
                    strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER), _infoTimingPackage->getCompleteValue());
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
                {
                    auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, _infoTimingPackage->getEndConditionTypeParams());
                    if ( objArtifact != nullptr )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
                {
                    auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, _infoTimingPackage->getEndConditionTypeParams());
                    if ( objArtifact != nullptr )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
                {
                    auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, _infoTimingPackage->getEndConditionTypeParams());
                    if ( objArtifact != nullptr )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
                {
                    auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, _infoTimingPackage->getEndConditionTypeParams());
                    if ( objArtifact != nullptr )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
                {
                    auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, _infoTimingPackage->getEndConditionTypeParams());
                    if ( objArtifact != nullptr )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
                {
                    auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
                    if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), objArtifact->getCurLv(), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
                {
                    auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
                    if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), objArtifact->getCurLv(), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
                {
                    auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
                    if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), objArtifact->getCurLv(), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
                {
                    auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
                    if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), objArtifact->getCurLv(), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
                {
                    auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
                    if ( objArtifact != nullptr && objArtifact->getGrade() == _infoTimingPackage->getEndConditionTypeParams() )
                    {
                        strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), objArtifact->getCurLv(), _infoTimingPackage->getCompleteValue());
                    }
                    break;
                }
                    
                default:
                    break;
            }
        }
            break;
        case E_TIMING_PACKAGE_TYPE::PURCHASE:
        {
            int time = _infoTimingPackage->getEndTimer();
            int hour = time / 3600;
            int min = (time % 3600) / 60;
            int second = time % 3600 % 60;
            
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            if(second < 0)
                second = 0;
            
            strContent = GAME_TEXTFORMAT(_infoTimingPackage->getDescKey(), hour, min, second);
        }
            break;
            
        default:
            break;
    }
    
    _lbContent->setString(strContent);
}

void PopupTimingPackage::onClick(cocos2d::Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getName().compare("CLOSE") == 0)
    {
        onHide();
    }
    else if(node->getName().compare("NEWBIE") == 0)
    {
        auto popup = PopupBuffNewbie::create();
        popup->show();
    }
    else if(node->getName().compare("BUY") == 0)
    {
        
        if(_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE)
        {
            PopupLoading::show();
            CashManager::getInstance()->purchaseInitiate(_infoTimingPackage->getCashIdx(), CC_CALLBACK_4(PopupTimingPackage::callbackInAppPurchasePass, this));
        }
        else if(_infoTimingPackage->getType() == E_TIMING_PACKAGE_TYPE::TERMS_FREE)
        {
            auto popup = PopupDefault::create(GAME_TEXT(_infoTimingPackage->getName()), GAME_TEXT("t_ui_timing_desc_5"));
            popup->addButton(GAME_TEXT("t_ui_artifact_title_discount_1"), false, [=](Ref*){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(_infoTimingPackage->getCashIdx(), CC_CALLBACK_4(PopupTimingPackage::callbackInAppPurchasePass, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
        }
    }
}

void PopupTimingPackage::onHide()
{
    if(_callbackHide != nullptr)
        _callbackHide();
    
    hide();
}

#pragma mark -callback
void PopupTimingPackage::callbackInAppPurchasePass(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        // redraw
        PopupLoading::show();
        TimingPackageManager::getInstance()->requestTimingPackageInfo(CC_CALLBACK_1(PopupTimingPackage::callbackPurchase, this));
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

void PopupTimingPackage::callbackPurchase(bool bResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        //
        onHide();
        
        return;
    }
    
    
    ValueMap listData;
    listData["KIND"] = Value("PURCHASE");
    TimingPackageManager::getInstance()->invokeEventTimingPackage(listData);
    onHide();
    
}

void PopupTimingPackage::setCallbackHide(std::function<void(void)> pCallback)
{
    _callbackHide = pCallback;
}
