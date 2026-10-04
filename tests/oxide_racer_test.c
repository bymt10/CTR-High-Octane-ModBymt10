#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <cs_oxide_racer.h>

static void TestBossHitReactionReplacesAllImpactStunsOnlyForOxide(void)
{
	assert(CS_OxideRacer_ShouldUseBossHitReaction(1, 1));
	assert(CS_OxideRacer_ShouldUseBossHitReaction(1, 2));
	assert(CS_OxideRacer_ShouldUseBossHitReaction(1, 3));
	assert(CS_OxideRacer_ShouldUseBossHitReaction(1, 4));
	assert(!CS_OxideRacer_ShouldUseBossHitReaction(1, 0));
	assert(!CS_OxideRacer_ShouldUseBossHitReaction(1, 5));
	assert(!CS_OxideRacer_ShouldUseBossHitReaction(0, 2));
	assert(!CS_OxideRacer_ShouldUseBossHitReaction(0, 3));
}

static void TestBossSpinUsesOriginalDurationAndReset(void)
{
	int cooldown = CS_OXIDE_BOSS_HIT_SPIN_COOLDOWN;
	int spinAngle = 0;
	int steps = 0;

	while (cooldown != 0)
	{
		int nextCooldown;
		int nextSpinAngle;
		int active = CS_OxideRacer_AdvanceBossHitSpin(cooldown, spinAngle, &nextCooldown, &nextSpinAngle);

		steps++;
		if (active)
		{
			assert(nextCooldown == cooldown - 0xc);
			assert(nextSpinAngle == spinAngle + cooldown);
		}
		else
		{
			assert(nextCooldown == 0);
			assert(nextSpinAngle == 0);
		}

		cooldown = nextCooldown;
		spinAngle = nextSpinAngle;
	}

	assert(steps == 43);
	assert(spinAngle == 0);
}

int main(void)
{
	TestBossHitReactionReplacesAllImpactStunsOnlyForOxide();
	TestBossSpinUsesOriginalDurationAndReset();
	puts("Oxide racer behavior tests passed");
	return 0;
}
