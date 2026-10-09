#ifndef GUARD_CONFIG_MENU_LAYOUT_H
#define GUARD_CONFIG_MENU_LAYOUT_H

#define TEXT_COLOR_OPTIONS_WHITE              1
#define TEXT_COLOR_OPTIONS_GRAY_FG            2
#define TEXT_COLOR_OPTIONS_GRAY_SHADOW        3
#define TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG      4
#define TEXT_COLOR_OPTIONS_ORANGE_FG          5
#define TEXT_COLOR_OPTIONS_ORANGE_SHADOW      6
#define TEXT_COLOR_OPTIONS_RED_FG             7
#define TEXT_COLOR_OPTIONS_RED_SHADOW         8
#define TEXT_COLOR_OPTIONS_GREEN_FG           9
#define TEXT_COLOR_OPTIONS_GREEN_SHADOW      10
#define TEXT_COLOR_OPTIONS_GREEN_DARK_FG     11
#define TEXT_COLOR_OPTIONS_GREEN_DARK_SHADOW 12
#define TEXT_COLOR_OPTIONS_RED_DARK_FG       13
#define TEXT_COLOR_OPTIONS_RED_DARK_SHADOW   14

u32 DrawConfigMenuLabel(u8 window, const u8 *label, u8 y, bool32 active);
void DrawConfigMenuChoices(u8 window, const u8 *const *choices, u32 count, u32 selected, u8 y, bool32 active, u32 left);

#if TESTING
bool32 GetOptionMenuTestRow(u32 index, const u8 **label, const u8 *const **choices, u32 *count);
bool32 GetChallengeMenuTestRow(u32 index, const u8 **label, const u8 *const **choices, u32 *count);
#endif

#endif
