#ifndef TH095_INPUT_RUNTIME_HPP
#define TH095_INPUT_RUNTIME_HPP

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// Target-backed semantic overlay for the shared input storage rooted at
// 0x004BE218.  ControllerInputSlotView describes the live-controller prefix;
// ReplayInputSource describes the replay/history interpretation used by
// ReplayManager and photo/front-end consumers.  Whether the original program
// expressed those overlapping interpretations as one C++ type is unknown.
struct ReplayInputSource
{
    u16 currentInput;          // +0x00
    u16 unknown002;
    u16 repeatOutput;          // +0x04
    u16 pressedInput;          // +0x06
    u8 unknown008[0x24];
    u16 historyCurrent;        // +0x2C
    u16 historyPrevious;       // +0x2E
    u16 historyRepeat;         // +0x30
    u16 historyPressed;        // +0x32
    u16 historyReleased;       // +0x34
    u16 unknown036;
    u16 heldFrames[16];        // +0x38

    void Update();
};

typedef char ReplayInputCurrentAt00[
    (offsetof(ReplayInputSource, currentInput) == 0x00) ? 1 : -1];
typedef char ReplayInputRepeatAt04[
    (offsetof(ReplayInputSource, repeatOutput) == 0x04) ? 1 : -1];
typedef char ReplayInputPressedAt06[
    (offsetof(ReplayInputSource, pressedInput) == 0x06) ? 1 : -1];
typedef char ReplayInputHistoryCurrentAt2C[
    (offsetof(ReplayInputSource, historyCurrent) == 0x2c) ? 1 : -1];
typedef char ReplayInputHistoryPreviousAt2E[
    (offsetof(ReplayInputSource, historyPrevious) == 0x2e) ? 1 : -1];
typedef char ReplayInputHistoryRepeatAt30[
    (offsetof(ReplayInputSource, historyRepeat) == 0x30) ? 1 : -1];
typedef char ReplayInputHistoryPressedAt32[
    (offsetof(ReplayInputSource, historyPressed) == 0x32) ? 1 : -1];
typedef char ReplayInputHistoryReleasedAt34[
    (offsetof(ReplayInputSource, historyReleased) == 0x34) ? 1 : -1];
typedef char ReplayInputHeldAt38[
    (offsetof(ReplayInputSource, heldFrames) == 0x38) ? 1 : -1];
typedef char ReplayInputSourceSizeIs58[
    (sizeof(ReplayInputSource) == 0x58) ? 1 : -1];

#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
struct ControllerInputSlotView;
extern ControllerInputSlotView g_ControllerInputSlots[];

static __forceinline ReplayInputSource *RuntimeReplayInputSource()
{
    return reinterpret_cast<ReplayInputSource *>(g_ControllerInputSlots);
}

static __forceinline u8 *RuntimeInputStorage()
{
    return reinterpret_cast<u8 *>(RuntimeReplayInputSource());
}

static __forceinline u16 &RuntimeInputCurrent()
{
    return RuntimeReplayInputSource()->currentInput;
}

static __forceinline u16 &RuntimeResultMenuInput()
{
    return RuntimeReplayInputSource()->repeatOutput;
}

static __forceinline u16 &RuntimePressedButtons()
{
    return RuntimeReplayInputSource()->pressedInput;
}

static __forceinline u16 &RuntimeHistoryCurrent()
{
    return RuntimeReplayInputSource()->historyCurrent;
}

static __forceinline u16 &RuntimeHistoryPrevious()
{
    return RuntimeReplayInputSource()->historyPrevious;
}

static __forceinline u16 &RuntimeHistoryRepeat()
{
    return RuntimeReplayInputSource()->historyRepeat;
}

static __forceinline u16 &RuntimeHistoryPressed()
{
    return RuntimeReplayInputSource()->historyPressed;
}

static __forceinline u16 &RuntimeHistoryReleased()
{
    return RuntimeReplayInputSource()->historyReleased;
}
#endif

} // namespace th095

#endif
