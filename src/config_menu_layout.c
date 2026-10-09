#include "global.h"
#include "config_menu_layout.h"
#include "text.h"
#include "menu.h"
#include "window.h"

u32 DrawConfigMenuLabel(u8 window, const u8 *label, u8 y, bool32 active)
{
    u32 font = FONT_NORMAL;
    if (GetStringWidth(font, label, 0) > 88)
        font = FONT_NARROWER;
    const u8 colors[] = {TEXT_COLOR_TRANSPARENT,
        active ? TEXT_COLOR_OPTIONS_ORANGE_FG : TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG,
        active ? TEXT_COLOR_OPTIONS_ORANGE_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW};
    AddTextPrinterParameterized4(window, font, 8, y, 0, 0, colors, TEXT_SKIP_DRAW, label);
    return max(104, 16 + GetStringWidth(font, label, 0));
}

void DrawConfigMenuChoices(u8 window, const u8 *const *choices, u32 count, u32 selected, u8 y, bool32 active, u32 left)
{
    // Keep the selected value visible, adding neighbours only when their text
    // and gaps fit. This applies to every tab, not just the difficulty row.
    u32 first = selected, last = selected;
    s32 total = GetStringWidth(FONT_NORMAL, choices[selected], 0);
    u32 font = FONT_NORMAL;
    if (total > 198 - left)
    {
        font = FONT_NARROWER;
        total = GetStringWidth(font, choices[selected], 0);
    }
    while (last - first < 2)
    {
        if (first > 0 && total + 8 + GetStringWidth(font, choices[first - 1], 0) <= 198 - left)
        {
            first--;
            total += 8 + GetStringWidth(font, choices[first], 0);
        }
        else if (last + 1 < count && total + 8 + GetStringWidth(font, choices[last + 1], 0) <= 198 - left)
        {
            last++;
            total += 8 + GetStringWidth(font, choices[last], 0);
        }
        else
            break;
    }
    u32 x = last == first ? 198 - total : left;
    for (u32 i = first; i <= last; i++)
    {
        u8 colors[] = {TEXT_COLOR_TRANSPARENT,
            active ? (i == selected ? TEXT_COLOR_OPTIONS_RED_FG : TEXT_COLOR_OPTIONS_GRAY_FG)
                   : (i == selected ? TEXT_COLOR_OPTIONS_RED_DARK_FG : TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG),
            active ? (i == selected ? TEXT_COLOR_OPTIONS_RED_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW)
                   : (i == selected ? TEXT_COLOR_OPTIONS_RED_DARK_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW)};
        if (i == last && last != first)
            x = 198 - GetStringWidth(font, choices[i], 0);
        AddTextPrinterParameterized4(window, font, x, y, 0, 0, colors, TEXT_SKIP_DRAW, choices[i]);
        x += GetStringWidth(font, choices[i], 0) + 8;
    }
}
