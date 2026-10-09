#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_message.h"
#include "menu.h"
#include "text.h"
#include "window.h"

extern const struct WindowTemplate *const gBattleWindowTemplates[];
extern const u8 gText_BattleYesNoChoice[];

TEST("Tactica battle menu: moving the Yes No cursor preserves text and clears the old cursor")
{
    u32 type;
    PARAMETRIZE { type = B_WIN_TYPE_NORMAL; }
    PARAMETRIZE { type = B_WIN_TYPE_KANTO_TUTORIAL; }
    PARAMETRIZE { type = B_WIN_TYPE_ARENA; }
    SetDefaultFontsPointer();
    gBattleScripting.windowsType = type;
    InitWindows(gBattleWindowTemplates[type]);
    EXPECT_EQ(GetWindowAttribute(B_WIN_YESNO, WINDOW_WIDTH), 4);
    const u8 colors[] = {8, 2, 3};
    FillWindowPixelBuffer(B_WIN_YESNO, PIXEL_FILL(8));
    AddTextPrinterParameterized4(B_WIN_YESNO, FONT_NORMAL, 8, 1, 0, 0, colors,
        TEXT_SKIP_DRAW, gText_BattleYesNoChoice);
    u8 before[4 * 4 * 32];
    const u8 *pixels = (const u8 *)GetWindowAttribute(B_WIN_YESNO, WINDOW_TILE_DATA);
    memcpy(before, pixels, sizeof(before));
    BattleDrawYesNoWindowCursor(0, TRUE);
    BattleDrawYesNoWindowCursor(0, FALSE);
    BattleDrawYesNoWindowCursor(1, TRUE);
    bool32 cursorDrawn = FALSE;
    for (u32 tile = 0; tile < 16; tile++)
        for (u32 byte = 0; byte < 32; byte++)
        {
            u32 index = tile * 32 + byte;
            if (tile % 4 != 0)
                EXPECT_EQ(pixels[index], before[index]);
            else if (tile < 8)
                EXPECT_EQ(pixels[index], 0x88);
            else if (pixels[index] != 0x88)
                cursorDrawn = TRUE;
        }
    EXPECT(cursorDrawn);
    FreeAllWindowBuffers();
}
