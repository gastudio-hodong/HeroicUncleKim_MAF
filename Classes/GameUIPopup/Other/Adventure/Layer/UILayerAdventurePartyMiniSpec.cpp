//
//  UILayerAdventurePartyMiniSpec.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/06.
//

#include "UILayerAdventurePartyMiniSpec.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureParty.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

UILayerAdventurePartyMiniSpec* UILayerAdventurePartyMiniSpec::create(const cocos2d::Size size)
{
    UILayerAdventurePartyMiniSpec* pRet = new(std::nothrow) UILayerAdventurePartyMiniSpec();
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

UILayerAdventurePartyMiniSpec::UILayerAdventurePartyMiniSpec(void):
_isFriend(false)
,_isPopup(false)
,_nCurrentAdventureID(0)
,_lyFriend(nullptr)
,_lyTop(nullptr)
,_lyMiddle(nullptr)
,_lyTable(nullptr)
,_table(nullptr)
{
    _listEffect.clear();
}

UILayerAdventurePartyMiniSpec::~UILayerAdventurePartyMiniSpec(void)
{
    _listEffect.clear();
}

bool UILayerAdventurePartyMiniSpec::init(const cocos2d::Size size)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    
    // init
    initVar();
    return true;
}

#pragma mark - init
void UILayerAdventurePartyMiniSpec::initVar()
{
}

void UILayerAdventurePartyMiniSpec::initUi()
{
    setEffectList();
    if(_isFriend)
    {
        _lyFriend = LayerColor::create();
        _lyFriend->setContentSize(Size(getContentSize().width, 166));
        _lyFriend->setIgnoreAnchorPointForPosition(false);
        _lyFriend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _lyFriend->setPosition(getContentSize().width/2, getContentSize().height);
        addChild(_lyFriend);
        
        _lyTable = LayerColor::create();
        _lyTable->setContentSize(Size(getContentSize().width, getContentSize().height - _lyFriend->getContentSize().height - 30));
        _lyTable->setIgnoreAnchorPointForPosition(false);
        _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _lyTable->setPosition(_lyFriend->getPosition() + Vec2::DOWN * (_lyFriend->getContentSize().height + 10));
        addChild(_lyTable);
        
        uiFriend();
    }
    else
    {
        // UI
        _lyTop = LayerColor::create();
        _lyTop->setContentSize(Size(getContentSize().width, 166));
        _lyTop->setIgnoreAnchorPointForPosition(false);
        _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _lyTop->setPosition(getContentSize().width/2, getContentSize().height);
        addChild(_lyTop);
        
        _lyMiddle = LayerColor::create();
        _lyMiddle->setContentSize(Size(getContentSize().width - 44, 70));
        _lyMiddle->setIgnoreAnchorPointForPosition(false);
        _lyMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _lyMiddle->setPosition(_lyTop->getPosition() + Vec2::DOWN * (_lyTop->getContentSize().height + 10));
        addChild(_lyMiddle);
        
        _lyTable = LayerColor::create();
        _lyTable->setContentSize(Size(getContentSize().width, getContentSize().height - _lyTop->getContentSize().height - _lyMiddle->getContentSize().height - 20));
        _lyTable->setIgnoreAnchorPointForPosition(false);
        _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _lyTable->setPosition(_lyMiddle->getPosition() + Vec2::DOWN * (_lyMiddle->getContentSize().height + 10));
        addChild(_lyTable);
        
        uiTop();
        uiMiddle();
    }
    
    uiTable();
    //
}

#pragma mark - override
void UILayerAdventurePartyMiniSpec::refreshUI()
{
    if(_isFriend)
    {
        uiFriend();
    }
    else
    {
        uiTop();
        uiMiddle();
    }
    
    if(_table != nullptr)
        _table->reloadData();
}

#pragma mark - table
void UILayerAdventurePartyMiniSpec::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerAdventurePartyMiniSpec::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 30);
}

extension::TableViewCell* UILayerAdventurePartyMiniSpec::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    
    auto objFriend = AdventureManager::getInstance()->getInfoFriendFromUserIdx(_nCurrentAdventureID);
    
    if(_isFriend)
    {
        if(objFriend == nullptr)
            return cell;
    }
    
    // obj
    
    auto eType = _listEffect[idx];
    
    std::string strEffect = "-" + GAME_TEXT("t_ui_advt_property_type_reduction_" + std::to_string((int)eType));
    
    double dTmpEffect = 0;
    double dTmpPublicEffect = 0;
    if(_isFriend)
    {
        dTmpEffect = objFriend->getAbilityEffect(eType);
    }
    else
    {
        switch (eType)
        {
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE:
            {
                dTmpEffect = AdventureManager::getInstance()->getEffectDecreseTime(_nCurrentAdventureID, true);
            } break;
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_GRASS_REWARD_INCREASE:
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_SNOW_REWARD_INCREASE:
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_DESERT_REWARD_INCREASE:
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_VOLCANO_REWARD_INCREASE:
            {
                dTmpPublicEffect = AdventureManager::getInstance()->getAdventureTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_PARTY_REWARD_INCREASE, _nCurrentAdventureID, true);
                dTmpEffect = dTmpPublicEffect + AdventureManager::getInstance()->getAdventureTotalEffect(eType, _nCurrentAdventureID, true);
            } break;
            case E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE:
            {
                dTmpEffect = AdventureManager::getInstance()->getAdventureTotalEffect(eType, _nCurrentAdventureID, true);
            } break;
            default:
            {
                dTmpEffect = AdventureManager::getInstance()->getAdventureTotalEffect(eType, _nCurrentAdventureID, true);
            }
                break;
        }
    }
    strEffect += " " + AdventureManager::getInstance()->getPropertyEffectText(eType, dTmpEffect);
    
    auto lbTextEffect = Label::createWithTTF(strEffect, GAME_FONT, 24);
    lbTextEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTextEffect->setPosition(10, cell->getContentSize().height);
    lbTextEffect->setDimensions(cell->getContentSize().width/2 - 20, cell->getContentSize().height);
    lbTextEffect->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextEffect->setOverflow(Label::Overflow::SHRINK);
    cell->addChild(lbTextEffect);
    
    if(dTmpEffect <= 0)
        lbTextEffect->setColor(Color3B(90,90,90));
    
    return cell;
}

ssize_t UILayerAdventurePartyMiniSpec::numberOfCellsInTableView(extension::TableView *table)
{
    return _listEffect.size();
}

#pragma mark ui
void UILayerAdventurePartyMiniSpec::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nCurrentAdventureID);
    if(objParty == nullptr)
        return;
    
    auto listMember = objParty->getListMemeber();
    auto objMember = listMember.at(0);
    std::string strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d.png", 1, objMember->getImageGrade());

    auto sprPartyBox = Sprite::create(strBoxPath);
    sprPartyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprPartyBox->setPosition(_lyTop->getContentSize().width/2, _lyTop->getContentSize().height - 10);
    sprPartyBox->getTexture()->setTexParameters(texParams);
    sprPartyBox->setScale(3);
    _lyTop->addChild(sprPartyBox);
    {
        Vec2 pos = Vec2(10, 2);
        
        for(int i = 1; i < listMember.size(); ++i)
        {
            auto objMember = listMember.at(i);
            if(objMember == nullptr || !objMember->isOpen())
                continue;
            
            std::string strPath = "Assets/ui/adventure/adventure_crew_%d_%d_1.png";
            auto sprMember = Sprite::create(MafUtils::format(strPath.c_str(), i+1, objMember->getImageGrade()));
            sprMember->setPosition(pos);
            sprMember->getTexture()->setTexParameters(texParams);
            sprMember->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprPartyBox->addChild(sprMember);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.25f);
                for ( int j = 1; j <= 2; j++ )
                {
                    auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d_%d.png", objMember->getImageIdx(), objMember->getImageGrade(), j));
                    sprFrame->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(sprFrame->getSpriteFrame());
                }
                sprMember->runAction(RepeatForever::create(Animate::create(animation)));
            }
            pos += Vec2::RIGHT * (sprMember->getContentSize().width / 2 + 12);
        }
    }
    
    auto btnTitleBox = ui::Button::create("Assets/ui_common/base_text_2_7.png");
    btnTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnTitleBox->setPosition(sprPartyBox->getPosition() + Vec2::LEFT * (sprPartyBox->getContentSize().width / 2 * sprPartyBox->getScale()));
    btnTitleBox->addClickEventListener(CC_CALLBACK_1(UILayerAdventurePartyMiniSpec::onClickParty, this));
    btnTitleBox->setZoomScale(0.03f);
    _lyTop->addChild(btnTitleBox);
    {
        auto sprPresetIcon =Sprite::create(MafUtils::format("Assets/ui/adventure/btn_preset_1_%d.png", objParty->getPresetIdx()));
        sprPresetIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprPresetIcon->setPosition(Vec2(5, btnTitleBox->getContentSize().height/2));
        btnTitleBox->addChild(sprPresetIcon);
        
        std::string strTitleText = GAME_TEXT("t_ui_advt_crew_title_1") + MafUtils::format(" %d", objParty->getIdx());
        
        auto lbText = Label::createWithTTF(strTitleText, GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprPresetIcon->getPosition() + Vec2::RIGHT * (sprPresetIcon->getContentSize().width + 10));
        lbText->setDimensions(btnTitleBox->getContentSize().width - 40 - btnTitleBox->getContentSize().width, btnTitleBox->getContentSize().height - 10);
        lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnTitleBox->addChild(lbText);
    }
    
    std::vector<E_ITEMS> list;
    list.clear();
    list.push_back(E_ITEMS::ADVENTURE_TICKET);
    
    auto uiCurrency = UICurrency::create(list, Size(100, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiCurrency->setPosition(Vec2(_lyTop->getContentSize().width - 30, _lyTop->getContentSize().height - 20));
    _lyTop->addChild(uiCurrency, E_ZORER::UI+1);
    
    
}
void UILayerAdventurePartyMiniSpec::uiMiddle()
{
    _lyMiddle->removeAllChildren();
    
    std::vector<E_ADVENTURE_PROPERTY_TYPE> listAreaEffect;
    listAreaEffect.clear();
    listAreaEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_GRASS_REWARD_INCREASE);
    listAreaEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_SNOW_REWARD_INCREASE);
    listAreaEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_DESERT_REWARD_INCREASE);
    listAreaEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_VOLCANO_REWARD_INCREASE);
    
    auto sprAreaBonusBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprAreaBonusBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprAreaBonusBox->setPosition(_lyMiddle->getContentSize()/2);
    sprAreaBonusBox->setContentSize(_lyMiddle->getContentSize());
    _lyMiddle->addChild(sprAreaBonusBox);
    {
        const double dTmpPublicEffect = AdventureManager::getInstance()->getAdventureTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_PARTY_REWARD_INCREASE, _nCurrentAdventureID, true);
        
        float posY = sprAreaBonusBox->getContentSize().height - sprAreaBonusBox->getContentSize().height /4;
        for(int i = 0; i < listAreaEffect.size(); i += 2)
        {
            auto eType = listAreaEffect[i];
            double dTmpEffect = dTmpPublicEffect + AdventureManager::getInstance()->getAdventureTotalEffect(eType, _nCurrentAdventureID, true);
            
            std::string strEffect = "-" + GAME_TEXT("t_ui_advt_property_type_reduction_" + std::to_string((int)eType));
            strEffect += " " + AdventureManager::getInstance()->getPropertyEffectText(eType, dTmpEffect);
            
            auto lbTextEffect = Label::createWithTTF(strEffect, GAME_FONT, 27);
            lbTextEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbTextEffect->setPosition(10, posY);
            lbTextEffect->setDimensions(sprAreaBonusBox->getContentSize().width/2 - 20, sprAreaBonusBox->getContentSize().height/2);
            lbTextEffect->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbTextEffect->setOverflow(Label::Overflow::SHRINK);
            sprAreaBonusBox->addChild(lbTextEffect);
            
            if(dTmpEffect <= 0)
                lbTextEffect->setColor(Color3B(90,90,90));
            
            eType = listAreaEffect[i + 1];
            dTmpEffect = dTmpPublicEffect + AdventureManager::getInstance()->getAdventureTotalEffect(eType, _nCurrentAdventureID, true);
            
            strEffect = "-" + GAME_TEXT("t_ui_advt_property_type_reduction_" + std::to_string((int)eType));
            strEffect += " " + AdventureManager::getInstance()->getPropertyEffectText(eType, dTmpEffect);
            
            lbTextEffect = Label::createWithTTF(strEffect, GAME_FONT, 27);
            lbTextEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            lbTextEffect->setPosition(sprAreaBonusBox->getContentSize().width - 10, posY);
            lbTextEffect->setDimensions(sprAreaBonusBox->getContentSize().width/2 - 20, sprAreaBonusBox->getContentSize().height/2);
            lbTextEffect->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
            lbTextEffect->setOverflow(Label::Overflow::SHRINK);
            sprAreaBonusBox->addChild(lbTextEffect);
            
            if(dTmpEffect <= 0)
                lbTextEffect->setColor(Color3B(90,90,90));
            
            posY = sprAreaBonusBox->getContentSize().height /4;
        }
    }

    
}
void UILayerAdventurePartyMiniSpec::uiTable()
{
    _lyTable->removeAllChildren();
    
    auto lyEffectInfo = LayerColor::create();
    lyEffectInfo->setContentSize( Size(_lyTable->getContentSize().width - 40, _lyTable->getContentSize().height - 10));
    lyEffectInfo->setIgnoreAnchorPointForPosition(false);
    lyEffectInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyEffectInfo->setPosition( Vec2(_lyTable->getContentSize().width / 2, 20));
    _lyTable->addChild(lyEffectInfo);
    
    _table = extension::TableView::create(this, Size(lyEffectInfo->getContentSize().width, lyEffectInfo->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(lyEffectInfo->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    lyEffectInfo->addChild(_table);
    _table->reloadData();

}

void UILayerAdventurePartyMiniSpec::uiFriend()
{
    auto objFriend = AdventureManager::getInstance()->getInfoFriendFromUserIdx(_nCurrentAdventureID);
    if(objFriend == nullptr)
        return;
    
    std::string strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d.png", 1, objFriend->getGrade());

    auto sprPartyBox = Sprite::create(strBoxPath);
    sprPartyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprPartyBox->setPosition(_lyFriend->getContentSize().width/2, _lyFriend->getContentSize().height - 10);
    sprPartyBox->getTexture()->setTexParameters(texParams);
    sprPartyBox->setScale(3);
    _lyFriend->addChild(sprPartyBox);
    {
        Vec2 pos = Vec2(10, 2);
        
        for(int i = 0; i < 3; ++i)
        {
            std::string strPath = "Assets/ui/adventure/adventure_crew_%d_%d_1.png";
            
            auto sprMember = Sprite::create(MafUtils::format(strPath.c_str(), i+2, objFriend->getGrade()));
            sprMember->setPosition(pos);
            sprMember->getTexture()->setTexParameters(texParams);
            sprMember->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprPartyBox->addChild(sprMember);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.25f);
                for ( int j = 1; j <= 2; j++ )
                {
                    auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d_%d.png", i+2, objFriend->getGrade(), j));
                    sprFrame->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(sprFrame->getSpriteFrame());
                }
                sprMember->runAction(RepeatForever::create(Animate::create(animation)));
            }
            pos += Vec2::RIGHT * (sprMember->getContentSize().width / 2 + 12);
        }
    }
    
    auto sprTitleBox = Sprite::create("Assets/ui_common/base_text_2_7.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprTitleBox->setPosition(sprPartyBox->getPosition() + Vec2::LEFT * (sprPartyBox->getContentSize().width / 2 * sprPartyBox->getScale()));
    _lyFriend->addChild(sprTitleBox);
    {
        std::string strFriendName = "Lv.%02d %s";
        strFriendName = MafUtils::format(strFriendName.c_str(), objFriend->getLevel(), objFriend->getNickName().c_str());
        
        auto lbText = Label::createWithTTF(strFriendName, GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(10, sprTitleBox->getContentSize().height/2);
        lbText->setDimensions(sprTitleBox->getContentSize().width - 30, sprTitleBox->getContentSize().height - 10);
        lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprTitleBox->addChild(lbText);
    }
}
#pragma mark click
void UILayerAdventurePartyMiniSpec::onClickParty(Ref* const pSender)
{
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nCurrentAdventureID);
    if(_isPopup || (!objParty->isOpen() || AdventureManager::getInstance()->getTutorialStep() < 6))
        return;
    
    auto popup = PopupAdventureParty::create(_nCurrentAdventureID);
    popup->setCallbackRefresh(CC_CALLBACK_0(UILayerAdventurePartyMiniSpec::refreshUI, this));
    popup->show();
}
#pragma mark set
void UILayerAdventurePartyMiniSpec::setIsFriend(const bool isFriend)
{
    _isFriend = isFriend;
}
void UILayerAdventurePartyMiniSpec::setIsPopup(const bool isPopup)
{
    _isPopup = isPopup;
}
void UILayerAdventurePartyMiniSpec::setAdventureID(const int nAdventureID)
{
    _nCurrentAdventureID = nAdventureID;
}

void UILayerAdventurePartyMiniSpec::setEffectList()
{
    _listEffect.clear();
    if(_isFriend)
    {
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_ALL_AREA_REWARD_INCREASE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::REWARD_BONUS_RATE);
    }
    else
    {
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::AREA_CORE_INCREASE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::REWARD_BONUS_RATE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::BASIC_REWARD_INCREASE);
        _listEffect.push_back(E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE);
    }
}

