#include "SoundManager.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

#define DATA_KEY_BGM    "DATA_BGM"
#define DATA_KEY_EFF    "DATA_EFFECT"

static float            _EffVolume = 1;
static float            _BgmVolume = 1;

static bool                                     _AutoPreload = false;       //메모리 로드가 안되었을 경우 먼저 로드 후 재생하는 변수
static std::unordered_map<std::string, bool>    _preloadMap;                //메모리에 로드된 오디오 파일 맵

static std::unordered_map<int, std::string> _effectList;                    //재생 중인 효과음들

static int _SceneBgmId           = AudioEngine::INVALID_AUDIO_ID;                //재생 중인 씬 BGM Id
static std::string _SceneBgmPath = "";                                           //재생 중인 씬 BGM Path

static int _ContnetsBgmId           = AudioEngine::INVALID_AUDIO_ID;                //재생 중인 컨텐츠 BGM Id
static std::string _ContnetsBgmPath = "";                                           //재생 중인 컨텐츠 BGM Path

static E_SOUND_TYPE _SoundState = E_SOUND_TYPE::SCENE;                            //현재 사운드 타입 상태

static std::vector<std::string> _BgmList;                                   //연속 재생을 위한 BGM 리스트
static int  _BgmListIndex;                                                  //연속 재생 BGM 리스트 중 현재 재생중인 인덱스

void SoundManager::InitSoundConfig()
{
    _EffVolume = UserDefault::getInstance()->getFloatForKey(DATA_KEY_EFF, 1.0f);
    _BgmVolume = UserDefault::getInstance()->getFloatForKey(DATA_KEY_BGM, 1.0f);
}

void SoundManager::SetSoundEffectVol(float volume)
{
    auto itEnd = _effectList.end();
    for (auto it = _effectList.begin(); it != itEnd; ++it)
    {
        AudioEngine::setVolume(it->first, volume);
    }
    
    _EffVolume = volume;
    
    UserDefault::getInstance()->setFloatForKey(DATA_KEY_EFF, volume);
    UserDefault::getInstance()->flush();
    
    if (!IsSoundEffect()) {
        SoundEffectStop();
    }
}
void SoundManager::SetSoundBgmVol(float volume)
{
    
    AudioEngine::setVolume(_SceneBgmId, volume);
    AudioEngine::setVolume(_ContnetsBgmId, volume);
    
    
    _BgmVolume = volume;
    
    UserDefault::getInstance()->setFloatForKey(DATA_KEY_BGM, volume);
    UserDefault::getInstance()->flush();
    
    if (!IsSoundBgm()) {
        SoundBgmPause();
    }else{
        if (!IsBgmPlaying()) {
            SoundBgmResume();
        }
    }
}
float SoundManager::GetSoundEffectVol()
{
    return _EffVolume;
}
float SoundManager::GetSoundBgmVol()
{
    return _BgmVolume;
}
bool SoundManager::IsSoundEffect()
{
    return GetSoundEffectVol() == 0 ? false : true;
}
bool SoundManager::IsSoundBgm()
{
    return GetSoundBgmVol() == 0 ? false : true;
}
AudioEngine::AudioState SoundManager::getState(int id)
{
    return AudioEngine::getState(id);
}
bool SoundManager::IsPlaying(int id)
{
    if (getState(id) == AudioEngine::AudioState::PLAYING) {
        return true;
    }
    return false;
}
bool SoundManager::IsBgmPlaying()
{
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            if (_SceneBgmId != AudioEngine::INVALID_AUDIO_ID) {
                if (getState(_SceneBgmId) == AudioEngine::AudioState::PLAYING) {
                    return true;
                }
            }
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            if (_ContnetsBgmId != AudioEngine::INVALID_AUDIO_ID) {
                if (getState(_ContnetsBgmId) == AudioEngine::AudioState::PLAYING) {
                    return true;
                }
            }
        }break;
        default:  break;
    }
    return false;
}
void SoundManager::SetAutoPreload(bool value)
{
    _AutoPreload = value;
}
bool SoundManager::IsAutoPreload()
{
    return _AutoPreload;
}

void SoundManager::SoundUnload(const std::string& path)
{
    if (_preloadMap.find(path) != _preloadMap.end()) {
        _preloadMap.erase(path);
    }
    AudioEngine::uncache(path);
}
void SoundManager::SoundUnload()
{
    _preloadMap.clear();
    AudioEngine::uncacheAll();
}
void SoundManager::SoundPreload(const std::string& path)
{
    AudioEngine::preload(path, [path](bool isSuccess){
        if (isSuccess)
        {
            _preloadMap[path] = true;
        }
        else
        {
            
        }
    });
}
void SoundManager::SoundPreloadBgmStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType)
{
    AudioEngine::preload(path, [path,loop,volume, eType](bool isSuccess){
        if (isSuccess)
        {
            _preloadMap[path] = true;

            SoundBgmStart(path,loop,volume, eType);
        }
        else
        {
            
        }
    });
}
void SoundManager::SoundPreloadEffectStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType)
{
    AudioEngine::preload(path, [path,loop,volume, eType](bool isSuccess){
        if (isSuccess)
        {
            _preloadMap[path] = true;
            
            SoundEffectStart(path,loop,volume, eType);
        }
        else
        {
            
        }
    });
}

int SoundManager::SoundEffectStart(const std::string& path, const E_SOUND_TYPE eType)
{
    return SoundEffectStart(path, false, _EffVolume, eType);
}
int SoundManager::SoundEffectStart(const std::string& path, bool loop, const E_SOUND_TYPE eType)
{
    return SoundEffectStart(path, loop, _EffVolume, eType);
}
int SoundManager::SoundEffectStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType)
{
    if (_SoundState != eType || !IsSoundEffect()) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    if (IsAutoPreload() && _preloadMap.find(path) == _preloadMap.end())
    {
        SoundPreloadEffectStart(path, loop, volume, eType);
        return -2;
    }
    else
    {
        int id = AudioEngine::play2d(path, loop, volume);
        if (id != AudioEngine::INVALID_AUDIO_ID)
        {
            if (_effectList.find(id) == _effectList.end()) {
                _effectList[id] = path;
            }
            
            AudioEngine::setFinishCallback(id, [](int finishAudioID, const std::string& finishAudioFile)
                                           {
                                               if (_effectList.find(finishAudioID) != _effectList.end()) {
                                                   _effectList.erase(finishAudioID);
                                               }
                                           });
        }
        return id;
    }
}

int SoundManager::SoundBgmStart(const std::string& path, const E_SOUND_TYPE eType)
{
    return SoundBgmStart(path, true, _BgmVolume, eType);
}
int SoundManager::SoundBgmStart(const std::string& path, bool loop, const E_SOUND_TYPE eType)
{
    return SoundBgmStart(path, loop, _BgmVolume, eType);
}
int SoundManager::SoundBgmStart(const std::string& path, bool loop, float volume, const E_SOUND_TYPE eType)
{
    if(eType != _SoundState)
    {
        switch (_SoundState)
        {
            case E_SOUND_TYPE::SCENE: return _SceneBgmId; break;
            case E_SOUND_TYPE::CONTENTS:  return _ContnetsBgmId; break;
            default:  break;
        }
        SoundBgmStop();
    }
    
    if (IsBgmPlaying())
    {
        switch (_SoundState)
        {
            case E_SOUND_TYPE::SCENE:
            {
                if (_SceneBgmPath.compare(path) == 0)
                {
                    return _SceneBgmId;
                }
            }break;
            case E_SOUND_TYPE::CONTENTS:
            {
                if (_ContnetsBgmPath.compare(path) == 0)
                {
                    return _ContnetsBgmId;
                }
            }break;
            default:  break;
        }
        SoundBgmStop();
    }
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE: _SceneBgmPath = path; break;
        case E_SOUND_TYPE::CONTENTS: _ContnetsBgmPath = path; break;
        default:  break;
    }
    //

    //
    if ( !IsSoundBgm() )
    {
        switch (_SoundState)
        {
            case E_SOUND_TYPE::SCENE: _SceneBgmId = AudioEngine::INVALID_AUDIO_ID; break;
            case E_SOUND_TYPE::CONTENTS: _ContnetsBgmId = AudioEngine::INVALID_AUDIO_ID; break;
            default:  break;
        }
        return AudioEngine::INVALID_AUDIO_ID;
    }

    if (IsAutoPreload() && _preloadMap.find(path) == _preloadMap.end())
    {
        SoundPreloadBgmStart(path, loop, volume, eType);
        return -2;
    }
    else
    {
        int bgmId = 0;
        switch (_SoundState)
        {
            case E_SOUND_TYPE::SCENE:
            {
                _SceneBgmId = AudioEngine::play2d(path, loop, volume);
                bgmId = _SceneBgmId;
            }break;
            case E_SOUND_TYPE::CONTENTS:
            {
                _ContnetsBgmId = AudioEngine::play2d(path, loop, volume);
                bgmId = _ContnetsBgmId;
            }break;
            default:  break;
        }
        
        
        if (bgmId != AudioEngine::INVALID_AUDIO_ID)
        {
            AudioEngine::setFinishCallback(bgmId, [](int finishAudioID, const std::string& finishAudioFile)
                                           {
                                               //BGM 순서 재생 기능
                                               if (!_BgmList.empty()) {
                                                   _BgmListIndex++;
                                                   SoundBgmSequenceStart();
                                               }
                                           });
        }
        return bgmId;
    }
}

void SoundManager::SoundBgmSequenceAdd(const std::string& path)
{
    _BgmListIndex = 0;
    _BgmList.push_back(path);
}
void SoundManager::SoundBgmSequenceClear()
{
    _BgmListIndex = 0;
    _BgmList.clear();
}
void SoundManager::SoundBgmSequenceStart()
{
    if (_BgmList.empty()) {
        return;
    }
    
    if (_BgmListIndex >= _BgmList.size()) {
        _BgmListIndex = 0;
    }else if (_BgmListIndex < 0) {
        _BgmListIndex = 0;
    }
    
    SoundBgmStart(_BgmList.at(_BgmListIndex), false);
}

void SoundManager::SoundEffectStop()
{
    auto itEnd = _effectList.end();
    for (auto it = _effectList.begin(); it != itEnd; ++it)
    {
        AudioEngine::stop(it->first);
    }
    _effectList.clear();
}
void SoundManager::SoundEffectPause()
{
    auto itEnd = _effectList.end();
    for (auto it = _effectList.begin(); it != itEnd; ++it)
    {
        AudioEngine::pause(it->first);
    }
}
void SoundManager::SoundEffectResume()
{
    auto itEnd = _effectList.end();
    for (auto it = _effectList.begin(); it != itEnd; ++it)
    {
        AudioEngine::resume(it->first);
    }
}

void SoundManager::SoundBgmStop()
{
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            if (_SceneBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::stop(_SceneBgmId);
                _SceneBgmId = AudioEngine::INVALID_AUDIO_ID;
            }
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            if (_ContnetsBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::stop(_ContnetsBgmId);
                _ContnetsBgmId = AudioEngine::INVALID_AUDIO_ID;
            }
        }break;
        default:  break;
    }
}
void SoundManager::SoundBgmPause()
{
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            if (_SceneBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::pause(_SceneBgmId);
            }
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            if (_ContnetsBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::pause(_ContnetsBgmId);
            }
        }break;
        default:  break;
    }
}

void SoundManager::SoundBgmResume()
{
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            if (_SceneBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::resume(_SceneBgmId);
            }
            else
            {
                SoundBgmStart(_SceneBgmPath);
            }
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            if (_ContnetsBgmId != AudioEngine::INVALID_AUDIO_ID)
            {
                AudioEngine::resume(_ContnetsBgmId);
            }
            else
            {
                SoundBgmStart(_ContnetsBgmPath);
            }
        }break;
        default:  break;
    }
}

void SoundManager::SoundStop()
{
    AudioEngine::stopAll();
    
    _effectList.clear();
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            _SceneBgmId = AudioEngine::INVALID_AUDIO_ID;
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            _ContnetsBgmId = AudioEngine::INVALID_AUDIO_ID;
        }break;
        default:  break;
    }
}
void SoundManager::SoundPause()
{
    AudioEngine::pauseAll();
}
void SoundManager::SoundResume()
{
    AudioEngine::resumeAll();
}
void SoundManager::SoundStop(int soundid)
{
    AudioEngine::stop(soundid);
    
    if (_effectList.find(soundid) != _effectList.end()) {
        _effectList.erase(soundid);
    }
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            if (soundid == _SceneBgmId)
            {
                _SceneBgmId = AudioEngine::INVALID_AUDIO_ID;
            }
        }break;
        case E_SOUND_TYPE::CONTENTS:
        {
            if (soundid == _ContnetsBgmId)
            {
                _ContnetsBgmId = AudioEngine::INVALID_AUDIO_ID;
            }
            
        }break;
        default:  break;
    }
    
}
void SoundManager::SoundPause(int soundid)
{
    AudioEngine::pause(soundid);
}
void SoundManager::SoundResume(int soundid)
{
    AudioEngine::resume(soundid);
}

void SoundManager::ChangeSoundState(E_SOUND_TYPE eType)
{
    if(_SoundState == eType)
        return;
    else
        SoundStop();
    
    _SoundState = eType;
    
    switch (_SoundState)
    {
        case E_SOUND_TYPE::SCENE:
        {
            SoundBgmStart(_SceneBgmPath);
            
            _ContnetsBgmId = AudioEngine::INVALID_AUDIO_ID;
            _ContnetsBgmPath = "";
        }break;
        default:  break;
    }
}
