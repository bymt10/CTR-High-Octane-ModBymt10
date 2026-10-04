#ifndef CTR_NATIVE_CS_OXIDE_RACER_H
#define CTR_NATIVE_CS_OXIDE_RACER_H

#include <stdint.h>

enum
{
	CS_OXIDE_BOSS_HIT_SPIN_COOLDOWN = 0x300,
	CS_OXIDE_BOSS_HIT_SPIN_STEP = 0xc,
	CS_OXIDE_BOSS_HIT_SPIN_END = 0x100,
	CS_OXIDE_BOSS_DAMAGE_STATE_SPIN = 1,
};

static inline int CS_OxideRacer_ShouldUseBossHitReaction(int isOxidePlayer, int damageType)
{
	return isOxidePlayer && (damageType >= 1) && (damageType <= 4);
}

static inline int CS_OxideRacer_AdvanceBossHitSpin(int cooldown, int spinAngle, int *nextCooldown, int *nextSpinAngle)
{
	int cooldownAfterFrame = cooldown - CS_OXIDE_BOSS_HIT_SPIN_STEP;
	if (cooldownAfterFrame < CS_OXIDE_BOSS_HIT_SPIN_END)
	{
		*nextCooldown = 0;
		*nextSpinAngle = 0;
		return 0;
	}

	*nextCooldown = cooldownAfterFrame;
	*nextSpinAngle = (int)(int16_t)((uint16_t)spinAngle + (uint16_t)cooldown);
	return 1;
}

#endif
