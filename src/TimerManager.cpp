#include "TimerManager.h"
#include "Globals.h"
#include <Arduino.h>
#include <ArduinoJson.h>

TimerManager_ &TimerManager_::getInstance()
{
  static TimerManager_ instance;
  return instance;
}

TimerManager_ &TimerManager = TimerManager.getInstance();

void TimerManager_::rightButton()
{
  if (isEditMode())   
  {
    increasePosValue();
  }
}

void TimerManager_::rightButtonLong()
{
  if (isEditMode())
  {
    decreaseEditPos();
  }
  else
  {
    reset();
  }    
}

void TimerManager_::leftButton()
{
  if (isEditMode())
  {
    if (isEditMode())
    {
       decreasePosValue();
    }
  }
}

void TimerManager_::leftButtonLong()
{
  if (isEditMode())
  {
    increaseEditPos();
  }
}

void TimerManager_::selectButton()
{
  if (isEditMode())
  {
    // leave edit mode
    toggleEditMode();
  }
  else
  {
    // start or stop timer
    if (isActive())
    {
      _isActive = false;
      //PeripheryManager.stopSound();
    }	
    else
    {
      _isActive = true;
      setEndTime();
    }
  }
}

void TimerManager_::selectButtonLong()
{
  toggleEditMode();
}

void TimerManager_::configureTimer(String Payload)
{
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, Payload);
  if (error)
    return;

  if (doc.containsKey("duration"))
  {
    _configuredTotalSeconds = doc["duration"] | TIMERMANAGER_DEFAULT_DURATION;
    _elapsedTotalSeconds = 0;
    _isActive = false;
  }
  //TODO: bool showProgress = doc["showProgress"] | true;
  TIMER_SOUND = doc.containsKey("sound") ? doc["sound"].as<String>() : "";
}

time_t TimerManager_::getEndTime() const
{
  return _timerEndTime;
}

void TimerManager_::setEndTime()
{
  auto duration = _configuredTotalSeconds - _elapsedTotalSeconds;
  time_t now = time(nullptr);
  struct tm futureTimeinfo = *localtime(&now);
  futureTimeinfo.tm_hour += duration / 3600;
  futureTimeinfo.tm_min += (duration % 3600) / 60;
  futureTimeinfo.tm_sec += duration % 60;
  _timerEndTime = mktime(&futureTimeinfo);
}

void TimerManager_::updateElapsedSecs()
{
}

int TimerManager_::calcAndUpdateRemainingSecs()
{
	if (isEditMode())
  {
    return _configuredTotalSeconds;
  }

  int remainingSecs{ 0 };
    
    if (!isActive())
    {
        remainingSecs = TimerManager.getConfiguredSecs() - TimerManager.getElapsedSecs();
    }
    else
    {
        time_t endTime = TimerManager.getEndTime();
        time_t now = time(nullptr);
        remainingSecs = difftime(endTime, now);
        _elapsedTotalSeconds = _configuredTotalSeconds - remainingSecs;
    }

    return remainingSecs;
}

void TimerManager_::toggleEditMode()
{
  if (isEditMode())
  {
    // leave edit mode
    _isEditMode = false;
  }
  else
  {
    // enter edit mode
    reset();
    _isEditMode = true;
    _editPos = 2;
  }
}

void TimerManager_::reset()
{
  _isActive = false;
  _elapsedTotalSeconds = 0;
  _wasSoundPlayed = false;
  _isEditMode = false;
}

void TimerManager_::decreasePosValue()
{
  // decrease the digit at at position _editPos
  if (0 == _editPos) // seconds
  {
    _configuredTotalSeconds--;
  }      
  else if (1 == _editPos) // tens of seconds
  {
    _configuredTotalSeconds -= 10;
  }
  else if (2 == _editPos) // minutes
  {
    _configuredTotalSeconds -= 60;
  }
  else if (3 == _editPos) // tens of minutes
  {
    _configuredTotalSeconds -= 600;
  }
  else if (4 == _editPos) // hours, but only up to 9 hours max
  {
    _configuredTotalSeconds -= 3600;
  }

  if (_configuredTotalSeconds < 0)
  {
    _configuredTotalSeconds = 0;
  }
}

void TimerManager_::increasePosValue()
{
  // increase the digit at position _editPos
  
  if (0 == _editPos) // seconds
  {
    _configuredTotalSeconds++;
  }      
  else if (1 == _editPos) // tens of seconds
  {
    _configuredTotalSeconds += 10;
  }
  else if (2 == _editPos) // minutes
  {
    _configuredTotalSeconds += 60;
  }
  else if (3 == _editPos) // tens of minutes
  {
    _configuredTotalSeconds += 600;
  }
  else if (4 == _editPos) // hours, but only up to 9 hours max
  {
    _configuredTotalSeconds += 3600;
  }

  if (_configuredTotalSeconds >= 36000)
  {
    _configuredTotalSeconds -= 3600;
  }
}

void TimerManager_::increaseEditPos()
{
  if (_editPos >= 4)
  {
    _editPos = 0;
  }
  else
  {
    ++_editPos;
  }
}

void TimerManager_::decreaseEditPos()
{
  if (_editPos == 0)
  {
    _editPos = 4;
  }
  else
  {
    --_editPos;
  }
}
