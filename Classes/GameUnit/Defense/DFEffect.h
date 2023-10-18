//
//  DFHitEffect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/11.
//

#ifndef DFHitEffect_hpp
#define DFHitEffect_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ObjectPool/MafPoolableObject.h"

class DFEffect : public MafPoolableObject
{
public:
    static DFEffect* create();
    
    DFEffect(void);
    virtual ~DFEffect(void);
    bool init(void) override;
public:
    void activateObject() override;
    void deactivateObject() override;
    
    void setNum(int num);
    void setPlaySpeed(float value);
    void setPlayScale(float value);
    void setPath(std::string path);
protected:
    void updateEffect();
public://override
    Rect GetBoundingBox() override;
    
protected:
    int _num;
    float _playSpeed;
    float _playScale;
    std::string _path;
    Sprite* _effect;
};

#endif /* DFHitEffect_hpp */
