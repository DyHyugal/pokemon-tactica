#ifndef GUARD_NATIVE_SPEED_H
#define GUARD_NATIVE_SPEED_H

// Encoded in a previously unused HnS save variable; zero/legacy values mean x1.
u32 GetNativeGameSpeed(void);
void SetNativeGameSpeed(u32 multiplier);
bool32 NativeSpeed_CanRunExtraTick(void);
void NativeSpeed_ClearInputEdges(void);

#endif
