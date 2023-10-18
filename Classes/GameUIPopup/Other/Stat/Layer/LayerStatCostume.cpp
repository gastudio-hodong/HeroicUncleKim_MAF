//
//  LayerStatCostume.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerStatCostume.h"

#include "ManagerGame/UserInfoManager.h"
#include "GameObject/InfoCostume.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

static bool sortIdx(InfoCostume *t1, InfoCostume *t2)
{
    return (t1->getIdx() < t2->getIdx());
}

StatCostume* StatCostume::create()
{
    StatCostume* pRet = new(std::nothrow) StatCostume();
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

StatCostume::StatCostume(void):
_layerContainerCostume(nullptr)
{
    
}

StatCostume::~StatCostume(void)
{
    
}
bool StatCostume::init()
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    
    // init
    initUI();
    return true;
}
void StatCostume::initUI()
{
    Size size = Size(750, Director::getInstance()->getVisibleSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    
    _layerContainerCostume = Layer::create();
    _layerContainerCostume->setContentSize( size );
    _layerContainerCostume->setIgnoreAnchorPointForPosition(false);
    _layerContainerCostume->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    addChild(_layerContainerCostume);
      
    initCostume();
}

void StatCostume::initVar()
{
    _lbCurrentValue.clear();
}

void StatCostume::initCostume()
{
    auto vecCostume = CostumeManager::getInstance()->getCostumeAll();
    E_COSTUME costumeType;
    InfoCostume *infoLegendSwap;
    
    int widthCount = 13;
    float startHeight = _layerContainerCostume->getContentSize().height*0.71;
    float bgScale= 0.65;
    int boxSize = 80*bgScale;
    
    
    for (int i=0; i<C_COUNT_COSTUME_TYPE; i++) {
        
        costumeType = (E_COSTUME)(i+1);
        Vector<InfoCostume*> vec;
        for(auto costume : vecCostume)
        {
            if(costume->getType() == costumeType)
            {
                vec.pushBack(costume);
            }
        }
        
        std::sort(vec.begin(), vec.end(), sortIdx);
        
        //전설 장신구를 찾아서 맨뒤로 옮겨준다.
        if(costumeType==E_COSTUME::JEWEL)
        {
            infoLegendSwap = vec.at(vec.size()-4);
            vec.eraseObject(infoLegendSwap);
            vec.pushBack(infoLegendSwap);
        }
        
        for (int j=0; j<vec.size(); j++)
        {
            auto info = vec.at(j);
            int costumeIdx = info->getIdx();
            
            std::string str = DataManager::GetCostumeIconPath(costumeType,(E_COSTUME_IDX)costumeIdx);
            int x = j%widthCount;
            int y = i + (j/widthCount)*5;
            
            if (costumeIdx == E_COSTUME_IDX::IDX_LEGEND)
            {
                y = -1;
                x = 4 + costumeType-1;
            }
            else if ( (i+1) == E_COSTUME::JEWEL)
            {
                int val = (40-1)/widthCount; //40 is setcount;
                int rest = (40-1)%widthCount;
                if(rest != 0)
                    val++;
                
                y = val*5;//y = 20;
                if(j>=widthCount){
                    y++;
                    x = j-widthCount;
                }
            }
            
            
            int gap = 0;
            if(y>=5)
                gap+=10;
            if(y>=10)
                gap+=10;
            if(y>=15)
                gap+=10;
            if(y==-1){
                gap=-5;
            }
            
            Sprite *bg = Sprite::create("Assets/ui/stat/box_3.png");
            bg->setAnchorPoint(Vec2(0, 0.5f));
            bg->setPosition(_layerContainerCostume->getContentSize().width*0.03 + x*boxSize+10, startHeight - (y*boxSize) -bg->getContentSize().height*0.5 - gap);
            bg->setScale(bgScale);
            _layerContainerCostume->addChild(bg);
            
            if (costumeType != E_COSTUME::JEWEL) {
                if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, (E_COSTUME_IDX)costumeIdx)) {
                    bg->setTexture("Assets/ui/stat/box_3_1.png");
                }
            }
            
            if (costumeType == E_COSTUME::JEWEL && costumeIdx >= E_COSTUME_IDX::IDX_LEGEND)
            {
                if (costumeIdx == E_COSTUME_IDX::IDX_LEGEND)
                {
                    Sprite *sprIcon = Sprite::create(str);
                    sprIcon->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
                    bg->addChild(sprIcon);
                    sprIcon->setScale(bg->getBoundingBox().size.width / sprIcon->getContentSize().width);
                    
                    if (!CostumeManager::getInstance()->getExist(costumeType, (E_COSTUME_IDX)costumeIdx)) {
                        sprIcon->setColor(Color3B(125, 125, 125));
                        Sprite *black = Sprite::create("Assets/ui/stat/box_3_off.png");
                        black->setPosition(bg->getContentSize()/2);
                        bg->addChild(black);
                    }
                }
                else
                {
                    int level=0;
                    if (costumeIdx == E_COSTUME_IDX::IDX_HONOR_1001) {
                        level = UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_1);
                        str = "Assets/icon_costume/jewel_1001.png";
                    }else if(costumeIdx == E_COSTUME_IDX::IDX_HONOR_1002){
                        level = UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_2);
                        str = "Assets/icon_costume/jewel_1002.png";
                    }else{
                        level = UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_3);
                        str = "Assets/icon_costume/jewel_1003.png";
                    }
                    
                    Sprite *sprIcon = Sprite::create(str);
                    sprIcon->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
                    bg->addChild(sprIcon);
                    sprIcon->setScale(bg->getBoundingBox().size.width / sprIcon->getContentSize().width);
                    
                    if (level == 0) {
                        sprIcon->setColor(Color3B(125, 125, 125));
                        Sprite *black = Sprite::create("Assets/ui/stat/box_3_off.png");
                        black->setPosition(bg->getContentSize()/2);
                        bg->addChild(black);
                    }else{
                        //                    Label* lbLevel = Label::createWithTTF("", GAME_FONT, 16);
                        Label *lbLevel = Label::createWithSystemFont(MafUtils::toString(level), SYSTEM_FONT, 15);
                        lbLevel->enableBold();
                        lbLevel->setAnchorPoint(Vec2(1, 0));
                        lbLevel->setPosition(bg->getContentSize().width-2, 2);
                        bg->addChild(lbLevel);
                    }
                }
            }
            else
            {
                Sprite *sprIcon = Sprite::create(str);
                sprIcon->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
                bg->addChild(sprIcon);
                
                sprIcon->setScale(bg->getBoundingBox().size.width / sprIcon->getContentSize().width);
                
                
                if (!CostumeManager::getInstance()->getExist(costumeType, (E_COSTUME_IDX)costumeIdx)) {
                    sprIcon->setColor(Color3B(125, 125, 125));
                    Sprite *black = Sprite::create("Assets/ui/stat/box_3_off.png");
                    black->setPosition(bg->getContentSize()/2);
                    bg->addChild(black);
                }
            }
        }
    }
}
