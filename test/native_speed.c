#include "global.h"
#include "native_speed.h"
#include "event_data.h"
#include "main.h"
#include "load_save.h"
#include "constants/vars.h"
#include "test/test.h"
#include "test/battle.h"

#if IS_HNS
SINGLE_BATTLE_TEST("Native speed: a battle turn completes at each multiplier")
{
    u32 speed = 1;
    PARAMETRIZE { speed = 1; }
    PARAMETRIZE { speed = 2; }
    PARAMETRIZE { speed = 3; }
    PARAMETRIZE { speed = 4; }
    GIVEN {
        SetNativeGameSpeed(speed);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    } FINALLY {
        SetNativeGameSpeed(1);
    }
}

TEST("Native speed: legacy values, four multipliers and audio layout")
{
    u16 old = VarGet(VAR_NATIVE_GAME_SPEED);
    struct ChallengeSettings audio = gSaveBlock3Ptr->challengeSettings;
    u32 speed;
    VarSet(VAR_NATIVE_GAME_SPEED, 0);
    EXPECT_EQ(GetNativeGameSpeed(), 1);
    VarSet(VAR_NATIVE_GAME_SPEED, 0xffff);
    EXPECT_EQ(GetNativeGameSpeed(), 1);
    for (speed = 1; speed <= 4; speed++)
    {
        SetNativeGameSpeed(speed);
        EXPECT_EQ(GetNativeGameSpeed(), speed);
    }
    SetNativeGameSpeed(5);
    EXPECT_EQ(GetNativeGameSpeed(), 1);
    EXPECT_EQ(sizeof(struct ChallengeSettings), 32);
    EXPECT_EQ(memcmp(&audio, &gSaveBlock3Ptr->challengeSettings, sizeof(audio)), 0);
    VarSet(VAR_NATIVE_GAME_SPEED, old);
}

TEST("Native speed: extra ticks cannot reuse button presses or repeats")
{
    u16 savedNew = gMain.newKeys;
    u16 savedRaw = gMain.newKeysRaw;
    u16 savedRepeated = gMain.newAndRepeatedKeys;
    u16 savedHeld = gMain.heldKeys;
    gMain.newKeys = gMain.newKeysRaw = gMain.newAndRepeatedKeys = A_BUTTON;
    gMain.heldKeys = DPAD_RIGHT;
    NativeSpeed_ClearInputEdges();
    EXPECT_EQ(gMain.newKeys, 0);
    EXPECT_EQ(gMain.newKeysRaw, 0);
    EXPECT_EQ(gMain.newAndRepeatedKeys, 0);
    EXPECT_EQ(gMain.heldKeys, DPAD_RIGHT);
    gMain.newKeys = savedNew;
    gMain.newKeysRaw = savedRaw;
    gMain.newAndRepeatedKeys = savedRepeated;
    gMain.heldKeys = savedHeld;
}
#endif
