#include "global.h"
#include "test/test.h"
#include "config_menu_layout.h"
#include "text.h"
#include "window.h"

static u32 ConfigPixel(const u8 *tiles, u32 x, u32 y)
{
    u32 index = (y / 8 * 26 + x / 8) * 32 + y % 8 * 4 + x % 8 / 2;
    return (tiles[index] >> (x % 2 * 4)) & 15;
}

TEST("Tactica configuration: every option and challenge tab renders choices after its label")
{
    bool32 active;
    bool32 (*getRow)(u32, const u8 **, const u8 *const **, u32 *);
    PARAMETRIZE { getRow = GetOptionMenuTestRow; active = TRUE; }
    PARAMETRIZE { getRow = GetOptionMenuTestRow; active = FALSE; }
    PARAMETRIZE { getRow = GetChallengeMenuTestRow; active = TRUE; }
    PARAMETRIZE { getRow = GetChallengeMenuTestRow; active = FALSE; }
    const struct WindowTemplate windows[] = {
        {.bg = 0, .width = 26, .height = 4, .paletteNum = 1, .baseBlock = 0},
        DUMMY_WIN_TEMPLATE
    };
    SetDefaultFontsPointer();
    InitWindows(windows);
    const u8 *label;
    const u8 *const *choices;
    u32 count, cases = 0;
    for (u32 row = 0; getRow(row, &label, &choices, &count); row++)
    {
        if (choices == NULL || count == 0)
            continue;
        for (u32 selected = 0; selected < count; selected++)
        {
            FillWindowPixelBuffer(0, PIXEL_FILL(1));
            u32 left = DrawConfigMenuLabel(0, label, 0, active);
            DrawConfigMenuChoices(0, choices, count, selected, 1, active, left);
            const u8 *pixels = (const u8 *)GetWindowAttribute(0, WINDOW_TILE_DATA);
            u32 labelRight = 0, selectedLeft = 208, selectedRight = 0;
            for (u32 x = 0; x < 208; x++)
                for (u32 y = 0; y < 17; y++)
                {
                    u32 color = ConfigPixel(pixels, x, y);
                    if (color == (active ? TEXT_COLOR_OPTIONS_ORANGE_FG : TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG) || color == (active ? TEXT_COLOR_OPTIONS_ORANGE_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW))
                        if (x < left)
                            labelRight = max(labelRight, x);
                    if (color == (active ? TEXT_COLOR_OPTIONS_RED_FG : TEXT_COLOR_OPTIONS_RED_DARK_FG) || color == (active ? TEXT_COLOR_OPTIONS_RED_SHADOW : TEXT_COLOR_OPTIONS_RED_DARK_SHADOW))
                    {
                        selectedLeft = min(selectedLeft, x);
                        selectedRight = max(selectedRight, x);
                    }
                }
            EXPECT(selectedLeft < 208);
            EXPECT_GT(selectedLeft, labelRight);
            EXPECT(selectedRight < 199);
            cases++;
        }
    }
    EXPECT(cases > 30);
    FreeAllWindowBuffers();
}
