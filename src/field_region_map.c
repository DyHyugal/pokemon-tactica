#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "field_move.h"
#include "follower_npc.h"
#include "field_effect.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "region_map.h"
#include "sound.h"
#include "strings.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

/*
 *  This is the type of map shown when interacting with the metatiles for
 *  a wall-mounted Region Map (on the wall of the Pokemon Centers near the PC)
 *  HnS also opens this map from Start: A flies to a visited destination, B returns.
 *  Other builds retain the read-only A/B behavior and optional R travel.
 *
 *  For the region map in the pokenav, see pokenav_region_map.c
 *  For the region map in the pokedex, see pokdex_area_screen.c/pokedex_area_region_map.c
 *  For the fly map, and utility functions all of the maps use, see region_map.c
 */

enum {
    WIN_MAPSEC_NAME,
    WIN_TITLE,
};

enum {
    TAG_PLAYER_ICON,
    TAG_CURSOR,
};

static EWRAM_DATA struct {
    MainCallback callback;
    bool32 choseFly;
    struct RegionMap regionMap;
    u16 state;
} *sFieldRegionMapHandler = NULL;

static void MCB2_InitRegionMapRegisters(void);
static void VBCB_FieldUpdateRegionMap(void);
static void MCB2_FieldUpdateRegionMap(void);
static void FieldUpdateRegionMap(void);
static void PrintRegionMapSecName();
static void PrintTitleWindowText();

static const struct BgTemplate sFieldRegionMapBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    }, {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 1,
        .priority = 2,
        .baseTile = 0
    }
};

static const struct WindowTemplate sFieldRegionMapWindowTemplates[] =
{
    [WIN_MAPSEC_NAME] = {
        .bg = 0,
        .tilemapLeft = IS_HNS ? 0 : 17,
        .tilemapTop = 17,
        .width = IS_HNS ? 30 : 12,
        .height = IS_HNS ? 3 : 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = IS_HNS ? 0 : 22,
        .tilemapTop = IS_HNS ? 0 : 1,
        .width = IS_HNS ? 30 : 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = IS_HNS ? 91 : 25
    },
    DUMMY_WIN_TEMPLATE
};

void FieldInitRegionMap(MainCallback callback)
{
    SetVBlankCallback(NULL);
    sFieldRegionMapHandler = Alloc(sizeof(*sFieldRegionMapHandler));
    if (sFieldRegionMapHandler == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    sFieldRegionMapHandler->state = 0;
    sFieldRegionMapHandler->choseFly = FALSE;
    sFieldRegionMapHandler->callback = callback;
    SetMainCallback2(MCB2_InitRegionMapRegisters);
}

static void MCB2_InitRegionMapRegisters(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(1, sFieldRegionMapBgTemplates, ARRAY_COUNT(sFieldRegionMapBgTemplates));
    InitWindows(sFieldRegionMapWindowTemplates);
    DeactivateAllTextPrinters();
    if (IS_HNS)
    {
        static const u16 palette[16] = {
            RGB(0,0,0), RGB(3,5,8), RGB(30,30,31), RGB(1,2,3),
            RGB(6,24,28), RGB(19,22,25), RGB(29,19,10), RGB(27,28,29)
        };
        LoadPalette(palette, BG_PLTT_ID(15), sizeof(palette));
    }
    else
        LoadUserWindowBorderGfx(0, 0x27, BG_PLTT_ID(13));
    ClearScheduledBgCopiesToVram();
    SetMainCallback2(MCB2_FieldUpdateRegionMap);
    SetVBlankCallback(VBCB_FieldUpdateRegionMap);
}

static void VBCB_FieldUpdateRegionMap(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MCB2_FieldUpdateRegionMap(void)
{
    FieldUpdateRegionMap();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    DoScheduledBgTilemapCopiesToVram();
}

static void FieldUpdateRegionMap(void)
{
    switch (sFieldRegionMapHandler->state)
    {
    case 0:
        InitRegionMap(&sFieldRegionMapHandler->regionMap, FALSE);
        CreateRegionMapPlayerIcon(TAG_PLAYER_ICON, TAG_PLAYER_ICON);
        CreateRegionMapCursor(TAG_CURSOR, TAG_CURSOR);
        sFieldRegionMapHandler->state++;
        break;
    case 1:
#if !IS_HNS
        DrawStdFrameWithCustomTileAndPalette(WIN_TITLE, FALSE, 0x27, 0xd);
        FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));
        PrintTitleWindowText();
        ScheduleBgCopyTilemapToVram(0);
#endif
        if (IS_HNS)
            PrintTitleWindowText();
        else
            DrawStdFrameWithCustomTileAndPalette(WIN_MAPSEC_NAME, FALSE, 0x27, 0xd);
        PrintRegionMapSecName();
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        sFieldRegionMapHandler->state++;
        break;
    case 2:
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(2);
        sFieldRegionMapHandler->state++;
        break;
    case 3:
        if (!gPaletteFade.active)
        {
            sFieldRegionMapHandler->state++;
        }
        break;
    case 4:
        switch (DoRegionMapInputCallback())
        {
        case MAP_INPUT_MOVE_END:
                PrintRegionMapSecName();
#if !IS_HNS
                PrintTitleWindowText();
#endif
                break;
        case MAP_INPUT_A_BUTTON:
        case MAP_INPUT_R_BUTTON:
                if ((IS_HNS || JOY_NEW(R_BUTTON))
                 && CanFlyFromRegionMap(&sFieldRegionMapHandler->regionMap))
                {
                    PlaySE(SE_SELECT);
                    sFieldRegionMapHandler->choseFly = TRUE;
                    sFieldRegionMapHandler->state++;
                }
                else if (IS_HNS)
                    PlaySE(SE_FAILURE);
                else if (JOY_NEW(A_BUTTON))
                    sFieldRegionMapHandler->state++;
                break;
        case MAP_INPUT_B_BUTTON:
                sFieldRegionMapHandler->state++;
                break;
        }
        break;
    case 5:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sFieldRegionMapHandler->state++;
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            MainCallback callback = sFieldRegionMapHandler->callback;
            bool32 fly = sFieldRegionMapHandler->choseFly;
            if (fly)
                SetFlyDestination(&sFieldRegionMapHandler->regionMap);
            FreeRegionMapIconResources();
            FreeAllWindowBuffers();
            TRY_FREE_AND_SET_NULL(sFieldRegionMapHandler);
            if (fly)
            {
                gSkipShowMonAnim = TRUE;
                ReturnToFieldFromFlyMapSelect();
            }
            else
                SetMainCallback2(callback);
        }
        break;
    }
}

static void PrintRegionMapSecName(void)
{
    if (IS_HNS)
    {
        const struct RegionMap *map = &sFieldRegionMapHandler->regionMap;
        bool32 canFly = CanFlyFromRegionMap(map);
        const u8 textColors[] = {1, 2, 3};
        const u8 hintColors[] = {1, 5, 3};
        const u8 actionColors[] = {1, 4, 3};
        static const u8 select[] = _("Choose a visited destination.");
        static const u8 locked[] = _("Fly unlocks after badge 5.");
        static const u8 unavailable[] = _("Fly unavailable here.");
        static const u8 unvisited[] = _("Visit this destination first.");
        static const u8 action[] = _("A FLY");
        static const u8 empty[] = _("REGION MAP");
        const u8 *hint = select;
        if (!IsFieldMoveUnlocked(FIELD_MOVE_FLY))
            hint = locked;
        else if (!Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType)
              || !CheckFollowerNPCFlag(FOLLOWER_NPC_FLAG_CAN_LEAVE_ROUTE))
            hint = unavailable;
        else if (map->mapSecType == MAPSECTYPE_CITY_CANTFLY)
            hint = unvisited;
        FillWindowPixelBuffer(WIN_MAPSEC_NAME, PIXEL_FILL(1));
        FillWindowPixelRect(WIN_MAPSEC_NAME, PIXEL_FILL(4), 0, 0, 240, 1);
        AddTextPrinterParameterized3(WIN_MAPSEC_NAME, FONT_SMALL, 8, 2,
            textColors, TEXT_SKIP_DRAW, map->mapSecType == MAPSECTYPE_NONE ? empty : map->mapSecName);
        if (canFly)
            AddTextPrinterParameterized3(WIN_MAPSEC_NAME, FONT_SMALL, 232 - GetStringWidth(FONT_SMALL, action, 0),
                2, actionColors, TEXT_SKIP_DRAW, action);
        AddTextPrinterParameterized3(WIN_MAPSEC_NAME, FONT_SMALL_NARROW, 8, 16,
            hintColors, TEXT_SKIP_DRAW, hint);
        PutWindowTilemap(WIN_MAPSEC_NAME);
        CopyWindowToVram(WIN_MAPSEC_NAME, COPYWIN_FULL);
        return;
    }
    if (sFieldRegionMapHandler->regionMap.mapSecType != MAPSECTYPE_NONE)
    {
        FillWindowPixelBuffer(WIN_MAPSEC_NAME, PIXEL_FILL(1));
        AddTextPrinterParameterized(WIN_MAPSEC_NAME, FONT_NORMAL, sFieldRegionMapHandler->regionMap.mapSecName, 0, 1, 0, NULL);
        ScheduleBgCopyTilemapToVram(WIN_MAPSEC_NAME);
    }
    else
    {
        FillWindowPixelBuffer(WIN_MAPSEC_NAME, PIXEL_FILL(1));
        CopyWindowToVram(WIN_MAPSEC_NAME, COPYWIN_FULL);
    }
}

static void PrintTitleWindowText(void)
{
    if (IS_HNS)
    {
        static const u8 johto[] = _("JOHTO");
        static const u8 combined[] = _("JOHTO / KANTO");
        static const u8 back[] = _("B BACK");
        const u8 colors[] = {1, 2, 3};
        const u8 accent[] = {1, 4, 3};
        FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));
        AddTextPrinterParameterized3(WIN_TITLE, FONT_SMALL, 8, 1, colors, TEXT_SKIP_DRAW,
            FlagGet(FLAG_VISITED_KANTO) ? combined : johto);
        AddTextPrinterParameterized3(WIN_TITLE, FONT_SMALL_NARROW, 232 - GetStringWidth(FONT_SMALL_NARROW, back, 0),
            4, accent, TEXT_SKIP_DRAW, back);
        FillWindowPixelRect(WIN_TITLE, PIXEL_FILL(4), 0, 15, 240, 1);
        PutWindowTilemap(WIN_TITLE);
        CopyWindowToVram(WIN_TITLE, COPYWIN_FULL);
        return;
    }
    static const u8 FlyPromptText[] = _("{R_BUTTON} FLY");
    const u8 *region;
    if (IS_HNS)
        region = gText_Johto;
    else if (IS_FRLG)
        region = gText_Kanto;
    else
        region = gText_Hoenn;
    u32 hoennOffset = GetStringCenterAlignXOffset(FONT_NORMAL, region, 0x38);
    u32 flyOffset = GetStringCenterAlignXOffset(FONT_NORMAL, FlyPromptText, 0x38);

    FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));

    if (sFieldRegionMapHandler->regionMap.mapSecType == MAPSECTYPE_CITY_CANFLY
        && FlagGet(OW_FLAG_POKE_RIDER) && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
    {
        AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, FlyPromptText, flyOffset, 1, 0, NULL);
        ScheduleBgCopyTilemapToVram(WIN_TITLE);
    }
    else
    {
        AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, region, hoennOffset, 1, 0, NULL);
        CopyWindowToVram(WIN_TITLE, COPYWIN_FULL);
    }
}

#if TESTING
void TestTacticaMapPanels(const struct RegionMap *map)
{
    typeof(*sFieldRegionMapHandler) local = {0};
    typeof(sFieldRegionMapHandler) saved = sFieldRegionMapHandler;
    local.regionMap = *map;
    sFieldRegionMapHandler = &local;
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(1, sFieldRegionMapBgTemplates, ARRAY_COUNT(sFieldRegionMapBgTemplates));
    InitWindows(sFieldRegionMapWindowTemplates);
    PrintTitleWindowText();
    PrintRegionMapSecName();
    sFieldRegionMapHandler = saved;
}
#endif
