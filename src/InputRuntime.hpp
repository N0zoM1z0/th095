#ifndef TH095_INPUT_RUNTIME_HPP
#define TH095_INPUT_RUNTIME_HPP
#include "inttypes.hpp"
namespace th095
{
struct ControllerInputSlotView;
extern ControllerInputSlotView g_ControllerInputSlots[];
static __forceinline u8 *RuntimeInputStorage() { return reinterpret_cast<u8 *>(g_ControllerInputSlots); }
static __forceinline u16 &RuntimeInputCurrent() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x00); }
static __forceinline u16 &RuntimeResultMenuInput() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x04); }
static __forceinline u16 &RuntimePressedButtons() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x06); }
static __forceinline u16 &RuntimeHistoryCurrent() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x2c); }
static __forceinline u16 &RuntimeHistoryPrevious() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x2e); }
static __forceinline u16 &RuntimeHistoryRepeat() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x30); }
static __forceinline u16 &RuntimeHistoryPressed() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x32); }
static __forceinline u16 &RuntimeHistoryReleased() { return *reinterpret_cast<u16 *>(RuntimeInputStorage() + 0x34); }
} // namespace th095
#endif
