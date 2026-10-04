#include "cs_adventure_racer.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct RacerUnlock
{
	int characterID;
	int unlockBit;
};

static const struct RacerUnlock s_unlocks[] = {
	{8, 10},
	{9, 8},
	{10, 7},
	{11, 9},
	{12, 5},
	{13, 6},
	{14, 11},
	{15, 37},
};

static void TestUnlockPolicy(void)
{
	uint32_t unlocks[2] = {0, 0};

	for (int characterID = 0; characterID < 8; characterID++)
	{
		assert(CS_AdventureRacer_IsUnlocked(characterID, unlocks));
	}
	for (int characterID = 8; characterID < CS_ADVENTURE_CHARACTER_COUNT; characterID++)
	{
		assert(!CS_AdventureRacer_IsUnlocked(characterID, unlocks));
	}

	for (size_t i = 0; i < sizeof(s_unlocks) / sizeof(s_unlocks[0]); i++)
	{
		const int characterID = s_unlocks[i].characterID;
		const int unlockBit = s_unlocks[i].unlockBit;
		assert(!CS_AdventureRacer_IsUnlocked(characterID, unlocks));
		unlocks[unlockBit >> 5] |= UINT32_C(1) << (unlockBit & 31);
		assert(CS_AdventureRacer_IsUnlocked(characterID, unlocks));
		assert(CS_AdventureRacer_ResolveSavedID(characterID, unlocks) == characterID);
		unlocks[unlockBit >> 5] = 0;
	}

	assert(!CS_AdventureRacer_IsUnlocked(-1, unlocks));
	assert(!CS_AdventureRacer_IsUnlocked(CS_ADVENTURE_CHARACTER_COUNT, unlocks));
	assert(CS_AdventureRacer_ResolveSavedID(-1, unlocks) == 0);
	assert(CS_AdventureRacer_ResolveSavedID(8, unlocks) == 0);
	assert(CS_AdventureRacer_ResolveSavedID(37, unlocks) == 0);
	assert(CS_AdventureRacer_ResolveSavedID(0, NULL) == 0);
}

static void TestShoulderCycle(void)
{
	uint32_t unlocks[2] = {0, 0};
	assert(CS_AdventureRacer_CycleFromShoulders(7, CS_ADVENTURE_BUTTON_R1, unlocks) == 0);
	assert(CS_AdventureRacer_CycleFromShoulders(0, CS_ADVENTURE_BUTTON_L1, unlocks) == 7);
	assert(CS_AdventureRacer_CycleFromShoulders(4, CS_ADVENTURE_BUTTON_L1 | CS_ADVENTURE_BUTTON_R1, unlocks) == 4);
	assert(CS_AdventureRacer_CycleFromShoulders(4, CS_ADVENTURE_BUTTON_L1 | CS_ADVENTURE_BUTTON_R1 | CS_ADVENTURE_BUTTON_CONFIRM, unlocks) == 4);
	assert(CS_AdventureRacer_CycleFromShoulders(4, CS_ADVENTURE_BUTTON_CONFIRM, unlocks) == 4);
	assert(CS_AdventureRacer_CycleFromShoulders(4, CS_ADVENTURE_BUTTON_DPAD, unlocks) == 4);
	assert(CS_AdventureRacer_CycleFromShoulders(-1, CS_ADVENTURE_BUTTON_L1 | CS_ADVENTURE_BUTTON_R1, unlocks) == 0);

	unlocks[10 >> 5] |= UINT32_C(1) << (10 & 31);
	assert(CS_AdventureRacer_CycleFromShoulders(7, CS_ADVENTURE_BUTTON_R1, unlocks) == 8);
	assert(CS_AdventureRacer_CycleFromShoulders(8, CS_ADVENTURE_BUTTON_L1, unlocks) == 7);

	for (size_t i = 0; i < sizeof(s_unlocks) / sizeof(s_unlocks[0]); i++)
	{
		const int bit = s_unlocks[i].unlockBit;
		unlocks[bit >> 5] |= UINT32_C(1) << (bit & 31);
	}
	assert(CS_AdventureRacer_CycleFromShoulders(7, CS_ADVENTURE_BUTTON_R1, unlocks) == 8);
	assert(CS_AdventureRacer_CycleFromShoulders(0, CS_ADVENTURE_BUTTON_L1, unlocks) == 15);
	assert(CS_AdventureRacer_CycleFromShoulders(15, CS_ADVENTURE_BUTTON_R1, unlocks) == 0);
	for (int characterID = 0; characterID < CS_ADVENTURE_CHARACTER_COUNT; characterID++)
	{
		assert(CS_AdventureRacer_CycleFromShoulders(characterID, CS_ADVENTURE_BUTTON_R1, unlocks) ==
		       (characterID + 1) % CS_ADVENTURE_CHARACTER_COUNT);
		assert(CS_AdventureRacer_CycleFromShoulders(characterID, CS_ADVENTURE_BUTTON_L1, unlocks) ==
		       (characterID + CS_ADVENTURE_CHARACTER_COUNT - 1) % CS_ADVENTURE_CHARACTER_COUNT);
	}
}

static void TestOxideStatsAndPhysics(void)
{
	static const int speed[4] = {13140, 13520, 13900, 12950};
	static const int accel[4] = {480, 544, 448, 512};
	static const int accelSpeed[4] = {14640, 15020, 15400, 14450};
	static const int turn[4] = {28, 26, 24, 30};

	assert(CS_AdventureRacer_StatTarget(15, 0x37) == 0x50);
	assert(CS_AdventureRacer_StatTarget(0, 0x37) == 0x37);
	assert(CS_AdventureRacer_StatTarget(13, 0x50) == 0x50);

	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_SPEED, speed) == 13900);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_ACCEL, accel) == 544);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_ACCEL_SPEED, accelSpeed) == 15400);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_TURN, turn) == 30);

	assert(CS_AdventureRacer_PhysicsValue(15, 1, 1, CS_ADVENTURE_PHYS_SPEED, speed) == speed[1]);
	assert(CS_AdventureRacer_PhysicsValue(0, 0, 1, CS_ADVENTURE_PHYS_SPEED, speed) == speed[1]);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_GRAVITY, turn) == turn[1]);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 4, CS_ADVENTURE_PHYS_TURN, turn) == 0);
	assert(CS_AdventureRacer_PhysicsValue(15, 0, 1, CS_ADVENTURE_PHYS_TURN, NULL) == 0);
}

int main(void)
{
	TestUnlockPolicy();
	TestShoulderCycle();
	TestOxideStatsAndPhysics();
	puts("Adventure racer policy tests passed");
	return 0;
}
