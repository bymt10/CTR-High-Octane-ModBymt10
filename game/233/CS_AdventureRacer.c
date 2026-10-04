#include <cs_adventure_racer.h>

static int CS_AdventureRacer_UnlockBit(int characterID)
{
	static const signed char s_unlockBits[CS_ADVENTURE_CHARACTER_COUNT] = {
		-1, -1, -1, -1, -1, -1, -1, -1,
		10, 8, 7, 9, 5, 6, 11, 37,
	};

	if ((unsigned int)characterID >= CS_ADVENTURE_CHARACTER_COUNT)
	{
		return -1;
	}
	return s_unlockBits[characterID];
}

int CS_AdventureRacer_IsUnlocked(int characterID, const uint32_t *progressUnlocks)
{
	if ((unsigned int)characterID >= CS_ADVENTURE_CHARACTER_COUNT)
	{
		return 0;
	}

	const int unlockBit = CS_AdventureRacer_UnlockBit(characterID);
	if (unlockBit < 0)
	{
		return 1;
	}
	if (progressUnlocks == 0)
	{
		return 0;
	}
	return (progressUnlocks[unlockBit >> 5] & (UINT32_C(1) << (unlockBit & 31))) != 0;
}

int CS_AdventureRacer_CycleFromShoulders(int characterID, uint32_t tappedButtons, const uint32_t *progressUnlocks)
{
	const uint32_t shoulderButtons = CS_ADVENTURE_BUTTON_L1 | CS_ADVENTURE_BUTTON_R1;
	const uint32_t pressedShoulders = tappedButtons & shoulderButtons;
	if ((pressedShoulders == 0) || (pressedShoulders == shoulderButtons) ||
	    ((tappedButtons & CS_ADVENTURE_BUTTON_CONFIRM) != 0))
	{
		return CS_AdventureRacer_IsUnlocked(characterID, progressUnlocks) ? characterID : 0;
	}

	const int step = (pressedShoulders == CS_ADVENTURE_BUTTON_R1) ? 1 : -1;
	int candidate = characterID;
	if ((unsigned int)candidate >= CS_ADVENTURE_CHARACTER_COUNT)
	{
		candidate = 0;
	}

	for (int i = 0; i < CS_ADVENTURE_CHARACTER_COUNT; i++)
	{
		candidate = (candidate + step + CS_ADVENTURE_CHARACTER_COUNT) % CS_ADVENTURE_CHARACTER_COUNT;
		if (CS_AdventureRacer_IsUnlocked(candidate, progressUnlocks))
		{
			return candidate;
		}
	}
	return 0;
}

int CS_AdventureRacer_ResolveSavedID(int characterID, const uint32_t *progressUnlocks)
{
	return CS_AdventureRacer_IsUnlocked(characterID, progressUnlocks) ? characterID : 0;
}

int CS_AdventureRacer_StatTarget(int characterID, int normalTarget)
{
	return (characterID == 15) ? CS_ADVENTURE_MAX_STAT_TARGET : normalTarget;
}

int CS_AdventureRacer_PhysicsValue(int characterID, int driverID, int engineID, int physicsField, const int classValues[4])
{
	if ((classValues == 0) || ((unsigned int)engineID >= 4))
	{
		return 0;
	}

	const int isOxidePlayer = (characterID == 15) && (driverID == 0);
	const int useMaximum = isOxidePlayer &&
	                       ((physicsField == CS_ADVENTURE_PHYS_ACCEL) ||
	                        (physicsField == CS_ADVENTURE_PHYS_SPEED) ||
	                        (physicsField == CS_ADVENTURE_PHYS_ACCEL_SPEED) ||
	                        (physicsField == CS_ADVENTURE_PHYS_TURN));
	if (!useMaximum)
	{
		return classValues[engineID];
	}

	int maximum = classValues[0];
	for (int i = 1; i < 4; i++)
	{
		if (classValues[i] > maximum)
		{
			maximum = classValues[i];
		}
	}
	return maximum;
}
