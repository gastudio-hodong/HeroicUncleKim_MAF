#ifndef __TapRed__SoundManager__
#define __TapRed__SoundManager__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

enum class E_SOUND_TYPE
{
    NONE = 0,
    
    SCENE,
    CONTENTS
};

class SoundManager
{
public:
    static void InitSoundConfig();
    static void SetSoundBgmVol(float volume);
    static void SetSoundEffectVol(float volume);
    static float GetSoundEffectVol();
    static float GetSoundBgmVol();
    static bool IsSoundEffect();
    static bool IsSoundBgm();
    
    static bool IsBgmPlaying();
    static bool IsPlaying(int soundId);
    static cocos2d::experimental::AudioEngine::AudioState getState(int soundId);
    
    static void SetAutoPreload(bool value);
    static bool IsAutoPreload();
    ///
    
    static void SoundPreloadBgmStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static void SoundPreloadEffectStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static void SoundPreload(const std::string& path);
    static void SoundUnload(const std::string& path);
    static void SoundUnload();
    
    static int SoundEffectStart(const std::string& path, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static int SoundEffectStart(const std::string& path, bool loop, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static int SoundEffectStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    
    static int SoundBgmStart(const std::string& path, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static int SoundBgmStart(const std::string& path, bool loop, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    static int SoundBgmStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType = E_SOUND_TYPE::SCENE);
    
    static void SoundBgmSequenceAdd(const std::string& path);
    static void SoundBgmSequenceClear();
    static void SoundBgmSequenceStart();
    
    static void SoundEffectStop();
    static void SoundEffectPause();
    static void SoundEffectResume();
    static void SoundBgmStop();
    static void SoundBgmPause();
    static void SoundBgmResume();
    
    static void SoundStop();
    static void SoundPause();
    static void SoundResume();
    static void SoundStop(int soundId);
    static void SoundPause(int soundId);
    static void SoundResume(int soundId);
    
    static void ChangeSoundState(E_SOUND_TYPE eType);
};

/* SOUND */
#define SOUND_BGM           "Sound/bgm_96.mp3"
#define SOUND_BGM_BOSS      "Sound/bgm_boss.mp3"
#define SOUND_BGM_DUNGEON   "Sound/bgm_prison.mp3"
#define SOUND_BGM_DUNGEON_EVENT   "Sound/s-w-1.mp3"
#define SOUND_BGM_RAID      "Sound/raid_bg.mp3"
#define SOUND_BGM_HELL      "Sound/bgm_hell.mp3"
#define SOUND_COSTUME_BGM   "Sound/bgm_costume.mp3"

#define SOUND_EVENT_FISHING_BGM   "Sound/bgm_fishing.mp3"

#define SOUND_SELECT    "Sound/button2.mp3"
#define SOUND_SELECT_3  "Sound/button3.mp3"
#define SOUND_BUY_GOLD  "Sound/buy_gold.mp3"
#define SOUND_UPGRADE   "Sound/upgrade2.mp3"

#define SOUND_RUN       "Sound/run2.mp3"

#define SOUND_ATTACK_1     "Sound/hit_1.mp3"
#define SOUND_ATTACK_2     "Sound/hit_2.mp3"
#define SOUND_ATTACK_3     "Sound/hit_3.mp3"
#define SOUND_ATTACK_4     "Sound/hit_4.mp3"
#define SOUND_ATTACK_5     "Sound/hit_5.mp3"
#define SOUND_ATTACK_EVENT "Sound/water-splash-1.mp3"
#define SOUND_CRI_1     "Sound/crit1.mp3"
#define SOUND_CRI_2     "Sound/crit2.mp3"
#define SOUND_REVIVAL   "Sound/revival.mp3"

#define SOUND_DIE_MONEY "Sound/die.mp3"
#define SOUND_DIE_1     "Sound/monsterdie_1.mp3"
#define SOUND_DIE_2     "Sound/monsterdie_2.mp3"
#define SOUND_DIE_3     "Sound/monsterdie_3.mp3"
#define SOUND_DIE_DRAGON "Sound/dragon_die.mp3"

#define SOUND_MONSTER_HIT_1  "Sound/monsterhit_1.mp3"
#define SOUND_MONSTER_HIT_2  "Sound/monsterhit_2.mp3"
#define SOUND_MONSTER_HIT_3  "Sound/monsterhit_3.mp3"

#define SOUND_PET_KRRR_ATTACK   "Sound/krr_attack.mp3"
#define SOUND_PET_HAMSTER       "Sound/pururu_voice.mp3"
#define SOUND_PET_DRAGON        "Sound/gochujang.mp3"
#define SOUND_PET_PRINCESS      "Sound/gongju.mp3"
#define SOUND_BUFF              "Sound/booster.mp3"
#define SOUND_BOX_CREATE        "Sound/create_box.mp3"

#define SOUND_ROCKET_START        "Sound/rocket-start.mp3"

#define SOUND_RAID_ATTACK    "Sound/raid_normal.mp3"


#define SOUND_BGM_D_1           "Sound/devil/bgm_d_1.mp3"
#define SOUND_BGM_D_BOSS_1      "Sound/devil/boss_d_1.mp3"

#define SOUND_PET_9             "Sound/devil/pet_9.mp3"
#define SOUND_PET_10            "Sound/devil/pet_10.mp3"
#define SOUND_PET_11            "Sound/devil/pet_11.mp3"
#define SOUND_PET_11_BUFF       "Sound/devil/pet_11_buff.mp3"
#define SOUND_PET_12            "Sound/devil/pet_11.mp3"

#define SOUND_EVENT_RACE_BGM_1            "Sound/event/event_race_bgm_1.mp3"
#define SOUND_PISTOL_SHOT            "Sound/pistol_shot_sound.mp3"
#define SOUND_FLASH_BGM_1               "Sound/bgm_flashman.mp3"

#define SOUND_FROST_CUTSCENE "Sound/skill/cutscene.mp3"
#define SOUND_FROST_ATTACK_SUCCCESS "Sound/skill/attack_success.mp3"
#define SOUND_FROST_ATTACK_FAIL "Sound/skill/attack_fail.mp3"
#define SOUND_FROST_ATTACK_SKILL_20005 "Sound/skill/attack_20005.mp3"
#define SOUND_FROST_ATTACK_SKILL_20006 "Sound/skill/attack_20006.mp3"

#define SOUND_EFFECT_EVENT_LUCKYBAG_FAIL "Sound/failed_1.mp3"

#endif /* defined(__TapRed__SoundManager__) */
