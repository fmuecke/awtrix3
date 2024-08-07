#ifndef TimerManager_h
#define TimerManager_h

#include <Arduino.h>
#include <time.h>

#define TIMERMANAGER_DEFAULT_DURATION 300

class TimerManager_
{
    TimerManager_() = default;
    time_t _timerEndTime{ 0 };
    int _configuredTotalSeconds{ TIMERMANAGER_DEFAULT_DURATION };
    int _elapsedTotalSeconds{ 0 };
    bool _isActive{ false };
    bool _isEditMode{ false };
    bool _wasSoundPlayed{ false };
    uint8_t _editPos{ 2 };  // default position: minutes

    void _start();
    void _stop();

public:
    static TimerManager_& getInstance();

    void rightButton();
    void rightButtonLong();
    void leftButton();
    void leftButtonLong();
    void selectButton();
    void selectButtonLong();

    void configureTimer(String Payload);
    time_t getEndTime() const;
    void setEndTime();
    void updateElapsedSecs();
    int calcAndUpdateRemainingSecs();
    void toggleEditMode();
    bool isActive() const { return _isActive; }
    bool isEditMode() const { return _isEditMode; }
    bool wasSoundPlayed() const { return _wasSoundPlayed; }
    void setSoundPlayed(bool played) { _wasSoundPlayed = played; }
    uint8_t getEditPos() const { return _editPos; }
    int getElapsedSecs() const { return _elapsedTotalSeconds; }
    int getConfiguredSecs() const { return _configuredTotalSeconds; }
    void reset();
    void increaseEditPos();
    void decreasePosValue();
    void increasePosValue();
    void decreaseEditPos();
};

extern TimerManager_ &TimerManager;

#endif