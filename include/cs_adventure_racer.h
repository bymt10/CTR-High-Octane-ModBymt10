#ifndef CTR_NATIVE_CS_ADVENTURE_RACER_H
#define CTR_NATIVE_CS_ADVENTURE_RACER_H

#include <stdint.h>

enum
{
	CS_ADVENTURE_CHARACTER_COUNT = 16,
	CS_ADVENTURE_BUTTON_R1 = 0x400,
	CS_ADVENTURE_BUTTON_L1 = 0x800,
	CS_ADVENTURE_BUTTON_CONFIRM = 0x50,
	CS_ADVENTURE_BUTTON_DPAD = 0x0c,
	CS_ADVENTURE_PHYS_GRAVITY = 0,
	CS_ADVENTURE_PHYS_ACCEL = 9,
	CS_ADVENTURE_PHYS_SPEED = 11,
	CS_ADVENTURE_PHYS_ACCEL_SPEED = 12,
	CS_ADVENTURE_PHYS_TURN = 18,
	CS_ADVENTURE_MAX_STAT_TARGET = 0x50,
};

int CS_AdventureRacer_IsUnlocked(int characterID, const uint32_t *progressUnlocks);
int CS_AdventureRacer_CycleFromShoulders(int characterID, uint32_t tappedButtons, const uint32_t *progressUnlocks);
int CS_AdventureRacer_ResolveSavedID(int characterID, const uint32_t *progressUnlocks);
int CS_AdventureRacer_StatTarget(int characterID, int normalTarget);
int CS_AdventureRacer_PhysicsValue(int characterID, int driverID, int engineID, int physicsField, const int classValues[4]);

#endif
