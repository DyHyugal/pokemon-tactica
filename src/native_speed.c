#include "global.h"
#include "native_speed.h"
#include "event_data.h"
#include "main.h"
#include "palette.h"
#include "sound.h"
#include "link.h"
#include "bg.h"
#include "task.h"
#include "constants/vars.h"

#define NATIVE_SPEED_SAVE_TAG 0x5300

u32 GetNativeGameSpeed(void)
{
#if IS_HNS
    u16 value = VarGet(VAR_NATIVE_GAME_SPEED);
    if (value >= NATIVE_SPEED_SAVE_TAG && value < NATIVE_SPEED_SAVE_TAG + 4)
        return value - NATIVE_SPEED_SAVE_TAG + 1;
#endif
    return 1;
}

void SetNativeGameSpeed(u32 multiplier)
{
#if IS_HNS
    if (multiplier < 1 || multiplier > 4)
        multiplier = 1;
    VarSet(VAR_NATIVE_GAME_SPEED, NATIVE_SPEED_SAVE_TAG + multiplier - 1);
#endif
}

bool32 NativeSpeed_CanRunExtraTick(void)
{
    // Audio and graphics transfers retain their real VBlank clock. Do not
    // shorten task-based fanfare/cry waits by running them multiple times.
    return !gLinkTransferringData && !gReceivedRemoteLinkPlayers
        && !gWirelessCommType && !gPaletteFade.active
        && !gPaletteFade.softwareFadeFinishing
        && !IsDma3ManagerBusyWithBgCopy()
        && IsFanfareTaskInactive() && !IsCryPlaying()
        && !FuncIsActiveTask(Task_DuckBGMForPokemonCry);
}

void NativeSpeed_ClearInputEdges(void)
{
    // A press/repeat belongs to the physical input frame, not each simulation
    // tick. Held directions still work; an A press cannot confirm twice.
    gMain.newKeys = 0;
    gMain.newKeysRaw = 0;
    gMain.newAndRepeatedKeys = 0;
}
