
#include "MICAudioManagerJNI.h"
#include "MICAudioManager.h"

void MICAudioManager::requestPermision()
{
}

bool MICAudioManager::deviceReady()
{
    return MICAudioManagerJNI::getInstance()->deviceReady();
}

void MICAudioManager::startRecord()
{
    MICAudioManagerJNI::getInstance()->startRecord();
}
void MICAudioManager::stopRecord()
{
    MICAudioManagerJNI::getInstance()->stopRecord();
}

int MICAudioManager::getPower()
{
    return MICAudioManagerJNI::getInstance()->getPower();
}