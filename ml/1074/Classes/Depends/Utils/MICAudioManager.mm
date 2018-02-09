
#include "MICAudioManager.h"
#include "MICAudioManager_ios.h"

void MICAudioManager::requestPermision()
{
    [[MICAudioManager_ios sharedManager]requestPermision];
}
bool MICAudioManager::deviceReady()
{
    return [[MICAudioManager_ios sharedManager]deviceReady];    
}

void MICAudioManager::startRecord()
{
    [[MICAudioManager_ios sharedManager]startRecord];
}
void MICAudioManager::stopRecord()
{
    [[MICAudioManager_ios sharedManager]stopRecord];
}

int MICAudioManager::getPower()
{
    return [[MICAudioManager_ios sharedManager]getPower];
}