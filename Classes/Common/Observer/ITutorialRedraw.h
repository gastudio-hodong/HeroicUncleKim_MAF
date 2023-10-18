//
//  ITutorialRedraw.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 04/12/2019.
//

#ifndef ITutorialRedraw_h
#define ITutorialRedraw_h

#include "ManagerGame/TutorialManager.h"

class ITutorialRedraw
{
public:
    ITutorialRedraw() :
    _bNowTutorial(false),
    _progress(PROGRESS_NONE,SUB_NONE)
    {
        
    }
    
public:
    virtual void redrawTutorialObject() = 0;
    
public:
    std::pair<PROGRESS_MAIN,PROGRESS_SUB> _progress;
    bool _bNowTutorial;
    
};
#endif /* ITutorialRedraw_h */
