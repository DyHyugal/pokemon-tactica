#include "global.h"
#include "gba/m4a_internal.h"
#include "m4a.h"
#include "load_save.h"
#include "gbs.h"
#include "constants/songs.h"
#include "constants/cries.h"
#include "sound.h"
#include "test/test.h"

extern struct ToneData gCryTable[];

static void SetVolumes(u32 music, u32 sfx)
{
    gSaveblock3.challengeSettings.audioVolumeInitialized = TRUE;
    gSaveblock3.challengeSettings.musicOnOff = FALSE;
    gSaveblock3.challengeSettings.musicVolume = music;
    gSaveblock3.challengeSettings.sfxVolume = sfx;
}

TEST("Audio volume: six independent mixer gains survive track initialization")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    u32 level;
    const u8 expected[] = {0, 25, 51, 77, 102, 128};
    for (level = 0; level < ARRAY_COUNT(expected); level++)
    {
        struct MusicPlayerTrack track = {0};
        track.flags = MPT_FLG_VOLSET;
        track.vol = 128;
        track.volX = 64; // The value written by MPlayMain's START path.
        SetVolumes(level, 5);
        TrkVolPitSet(&gMPlayInfo_BGM, &track);
        EXPECT_EQ(track.volMR, expected[level]);
        track.flags = MPT_FLG_VOLSET;
        TrkVolPitSet(&gMPlayInfo_SE1, &track);
        EXPECT_EQ(track.volMR, 128);
        SetVolumes(5, level);
        track.flags = MPT_FLG_VOLSET;
        TrkVolPitSet(&gMPlayInfo_SE1, &track);
        EXPECT_EQ(track.volMR, expected[level]);
        track.flags = MPT_FLG_VOLSET;
        TrkVolPitSet(&gPokemonCryMusicPlayers[0], &track);
        EXPECT_EQ(track.volMR, expected[level]);
    }
    gSaveblock3.challengeSettings = saved;
}

TEST("Audio volume: ducking and fades multiply rather than replace user gain")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    struct MusicPlayerTrack track = {0};
    SetVolumes(1, 5);
    track.flags = MPT_FLG_VOLSET;
    track.vol = 128;
    track.volX = 32;
    TrkVolPitSet(&gMPlayInfo_BGM, &track);
    EXPECT_EQ(track.volMR, 12);
    track.flags = MPT_FLG_VOLSET;
    track.volX = 64;
    TrkVolPitSet(&gMPlayInfo_BGM, &track);
    EXPECT_EQ(track.volMR, 25);
    gSaveblock3.challengeSettings.musicOnOff = TRUE;
    track.flags = MPT_FLG_VOLSET;
    TrkVolPitSet(&gMPlayInfo_BGM, &track);
    EXPECT_EQ(track.volMR, 0);
    gSaveblock3.challengeSettings = saved;
}

TEST("Audio volume: legacy and invalid saves use the quiet default")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    SetVolumes(7, 6);
    EXPECT_EQ(GetUserMusicVolume(), 51);
    EXPECT_EQ(GetUserSfxVolume(), 51);
    SetVolumes(0, 0);
    gSaveblock3.challengeSettings.audioVolumeInitialized = FALSE;
    EXPECT_EQ(GetUserMusicVolume(), 51);
    EXPECT_EQ(GetUserSfxVolume(), 51);
    EXPECT_EQ(sizeof(struct ChallengeSettings), 32);
    gSaveblock3.challengeSettings = saved;
}

TEST("Audio volume: GBS mute disconnects only its own hardware channel")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    struct GBSTrack track = {0};
    m4aSoundInit();
    SetVolumes(5, 0);
    track.flags = MPT_FLG_EXIST;
    track.channelID = 2;
    track.noteLength1 = 10;
    track.volX = 64;
    track.velocity = 15;
    track.pan = 0xFF;
    track.isSFXChannel = TRUE;
    REG_NR51 = 0xFF;
    GBSMain(&gMPlayInfo_SE1, (struct MusicPlayerTrack *)&track);
    EXPECT_EQ(REG_NR51 & 0x22, 0);
    EXPECT_EQ(REG_NR51 & 0xDD, 0xDD);
    EXPECT_EQ(track.noteLength1, 9); // Muted sounds retain their timing.
    SetVolumes(0, 5);
    GBSMain(&gMPlayInfo_SE1, (struct MusicPlayerTrack *)&track);
    EXPECT_EQ(REG_NR51 & 0x22, 0x22);
    EXPECT_EQ(REG_NR50, 0x77); // Music mute cannot attenuate SFX globally.
    track.isSFXChannel = FALSE;
    GBSMain(&gMPlayInfo_BGM, (struct MusicPlayerTrack *)&track);
    EXPECT_EQ(REG_NR51 & 0x22, 0);
    gSaveblock3.challengeSettings = saved;
}

TEST("Audio volume: GBS tone steps change hardware amplitude")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    struct GBSTrack track = {0};
    const u8 expected[] = {0, 2, 4, 6, 8, 11};
    u32 level;
    m4aSoundInit();
    track.flags = MPT_FLG_EXIST;
    track.channelID = 2;
    track.noteLength1 = 20;
    track.volX = 64;
    track.velocity = 15;
    track.pan = 0xFF;
    for (level = 0; level < ARRAY_COUNT(expected); level++)
    {
        SetVolumes(level, 5);
        GBSMain(&gMPlayInfo_BGM, (struct MusicPlayerTrack *)&track);
        if (level != 0)
            EXPECT_EQ(REG_NR22 >> 4, expected[level]);
        else
            EXPECT_EQ(REG_NR51 & 0x22, 0);
    }
    gSaveblock3.challengeSettings = saved;
}

TEST("Audio volume: newly started effects and overlapping cries stay muted")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    struct MusicPlayerInfo *players[3];
    u32 i, j;
    m4aSoundInit();
    SetVolumes(5, 0);
    m4aSongNumStart(SE_SELECT, FALSE);
    players[0] = gMPlayTable[GetSong(SE_SELECT, FALSE)->ms].info;
    // GetCryIdBySpecies intentionally suppresses cries in headless tests.
    // Start the real cry player directly to exercise its audio path.
    SetPokemonCryVolume(120);
    players[1] = SetPokemonCryTone(&gCryTable[CRY_BULBASAUR - 1]);
    players[2] = SetPokemonCryTone(&gCryTable[CRY_CHARMANDER - 1]);
    EXPECT_EQ(players[1]->tracks[0].flags & MPT_FLG_EXIST, MPT_FLG_EXIST);
    EXPECT(players[1] != players[2]);
    for (i = 0; i < ARRAY_COUNT(players); i++)
    {
        MPlayMain(players[i]);
        EXPECT(players[i]->status & MUSICPLAYER_STATUS_TRACK);
        for (j = 0; j < players[i]->trackCount; j++)
        {
            EXPECT_EQ(players[i]->tracks[j].volMR, 0);
            EXPECT_EQ(players[i]->tracks[j].volML, 0);
        }
    }
    SetVolumes(5, 5);
    ApplyUserAudioVolumes();
    for (i = 0; i < ARRAY_COUNT(players); i++)
    {
        MPlayMain(players[i]);
        EXPECT(players[i]->tracks[0].volMR > 0);
    }
    SetVolumes(5, 0);
    ApplyUserAudioVolumes();
    for (i = 0; i < ARRAY_COUNT(players); i++)
    {
        MPlayMain(players[i]);
        EXPECT_EQ(players[i]->tracks[0].volMR, 0);
        EXPECT_EQ(players[i]->tracks[0].volML, 0);
    }
    m4aMPlayAllStop();
    gSaveblock3.challengeSettings = saved;
}
