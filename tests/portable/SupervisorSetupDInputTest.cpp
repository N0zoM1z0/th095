// Actual shared initialization body; native pointer, COM-operation, callback
// identity and logger bindings below are explicit fixtures, not SDK/parent ABI.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include "inttypes.hpp"

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "input check line %d: %s\n", __LINE__, #x); std::abort(); } } while (0)
namespace th095
{
struct InputWindowFixture { int tag; } windows[2];
struct InputModuleFixture { int tag; } modules[2];
typedef InputModuleFixture *InputModuleHandleFixture;
struct InputGuidFixture { int tag; };
struct InputFormatFixture { int tag; };
struct InputCapsFixture { u32 dwSize; u32 marker; };
const InputGuidFixture IID_IDirectInput8A = {1}, GUID_SysKeyboard = {2};
const InputFormatFixture c_dfDIKeyboard = {1}, c_dfDIJoystick2 = {2};
enum { DIRECTINPUT_VERSION=0x800, GWLP_HINSTANCE=-6,
       DISCL_NONEXCLUSIVE=2, DISCL_FOREGROUND=4, DISCL_BACKGROUND=8, DISCL_NOWINKEY=16,
       DI8DEVCLASS_GAMECTRL=4, DIEDFL_ATTACHEDONLY=1 };
enum { WINDOW_READ=1, CREATE_INPUT, CREATE_KEYBOARD, KEY_FORMAT, KEY_COOP, KEY_ACQUIRE,
       LOG_MESSAGE, ENUM_DEVICES, CTRL_FORMAT, CTRL_COOP, CTRL_CAPS, CTRL_OBJECTS,
       KEY_RELEASE, INPUT_RELEASE };
enum { FAIL_NONE=0, DISABLED, FAIL_FACTORY, FAIL_KEYBOARD, FAIL_FORMAT, FAIL_COOP };
struct InputDeviceFixture;
struct InputInterfaceFixture;
struct InputSupervisorFixture
{
    InputWindowFixture *gameWindow;
    struct Configuration { struct Options { u32 disableDirectInput; } options; } config;
    InputInterfaceFixture *directInput;
    InputDeviceFixture *keyboard, *controller;
    InputCapsFixture controllerCaps;
    i32 SetupDInput();
    static i32 EnumGameControllersCb(const void *, void *) { std::abort(); }
    static i32 ControllerCallback(const void *, void *) { std::abort(); }
};
struct InputScenario
{
    i32 stopAt;
    u32 ignoredFailures;
    i32 successfulHresult;
    bool publishController;
    InputModuleHandleFixture module;
    InputSupervisorFixture *receiver;
    std::vector<int> events;
    i32 inputReleases, keyReleases;
} scenario;
InputSupervisorFixture g_Supervisor;

struct InputDeviceFixture
{
    bool isKeyboard;
    i32 SetDataFormat(const InputFormatFixture *format)
    {
        CHECK(format == (isKeyboard ? &c_dfDIKeyboard : &c_dfDIJoystick2));
        scenario.events.push_back(isKeyboard ? KEY_FORMAT : CTRL_FORMAT);
        if (isKeyboard) return scenario.stopAt == FAIL_FORMAT ? -1 : scenario.successfulHresult;
        return scenario.ignoredFailures & 4u ? -1 : 0;
    }
    i32 SetCooperativeLevel(InputWindowFixture *window, u32 flags)
    {
        CHECK(window == scenario.receiver->gameWindow);
        CHECK(flags == static_cast<u32>(isKeyboard ? DISCL_FOREGROUND|DISCL_NONEXCLUSIVE|DISCL_NOWINKEY : DISCL_BACKGROUND|DISCL_NONEXCLUSIVE));
        scenario.events.push_back(isKeyboard ? KEY_COOP : CTRL_COOP);
        if (isKeyboard) return scenario.stopAt == FAIL_COOP ? -1 : scenario.successfulHresult;
        return scenario.ignoredFailures & 8u ? -1 : 0;
    }
    i32 Acquire()
    {
        CHECK(isKeyboard); scenario.events.push_back(KEY_ACQUIRE);
        return scenario.ignoredFailures & 1u ? -1 : 0;
    }
    u32 Release()
    {
        CHECK(isKeyboard); ++scenario.keyReleases; scenario.events.push_back(KEY_RELEASE); return 0;
    }
    i32 GetCapabilities(InputCapsFixture *caps)
    {
        CHECK(!isKeyboard && caps == &g_Supervisor.controllerCaps);
        CHECK(caps->dwSize == sizeof(InputCapsFixture));
        scenario.events.push_back(CTRL_CAPS); caps->marker = 42;
        return scenario.ignoredFailures & 16u ? -1 : 0;
    }
    i32 EnumObjects(i32 (*callback)(const void *, void *), void *context, u32 flags)
    {
        CHECK(!isKeyboard && callback == &InputSupervisorFixture::ControllerCallback);
        CHECK(context == NULL && flags == 0); scenario.events.push_back(CTRL_OBJECTS);
        return scenario.ignoredFailures & 32u ? -1 : 0;
    }
} keyboardDevice={true}, controllerDevice={false}, oldKeyboard={true}, oldController={false};

struct InputInterfaceFixture
{
    i32 CreateDevice(const InputGuidFixture &guid, InputDeviceFixture **output, void *outer)
    {
        CHECK(&guid == &GUID_SysKeyboard && outer == NULL);
        CHECK(output == &scenario.receiver->keyboard); scenario.events.push_back(CREATE_KEYBOARD);
        *output = scenario.stopAt == FAIL_KEYBOARD ? NULL : &keyboardDevice;
        return scenario.stopAt == FAIL_KEYBOARD ? -1 : scenario.successfulHresult;
    }
    u32 Release()
    {
        ++scenario.inputReleases; scenario.events.push_back(INPUT_RELEASE); return 0;
    }
    i32 EnumDevices(u32 kind, i32 (*callback)(const void *, void *), void *context, u32 flags)
    {
        CHECK(kind == DI8DEVCLASS_GAMECTRL && flags == DIEDFL_ATTACHEDONLY);
        CHECK(callback == &InputSupervisorFixture::EnumGameControllersCb && context == NULL);
        scenario.events.push_back(ENUM_DEVICES);
        // Explicit backend staging: callback identity is checked, not executed.
        // Enumeration/callback implementation is outside this component test.
        if (scenario.publishController) scenario.receiver->controller = &controllerDevice;
        return scenario.ignoredFailures & 2u ? -1 : 0;
    }
} inputInterface, oldInput;

std::intptr_t ReadWindowModuleFixture(InputWindowFixture *window, int index)
{
    CHECK(window == scenario.receiver->gameWindow && index == GWLP_HINSTANCE);
    scenario.events.push_back(WINDOW_READ);
    return reinterpret_cast<std::intptr_t>(scenario.module);
}
i32 CreateInputFixture(InputModuleHandleFixture module, u32 version,
                       const InputGuidFixture &iid, void **output, void *outer)
{
    CHECK(module == scenario.module); // detects any loss of high module bits
    CHECK(version == DIRECTINPUT_VERSION && &iid == &IID_IDirectInput8A && outer == NULL);
    CHECK(output == reinterpret_cast<void **>(&scenario.receiver->directInput));
    scenario.events.push_back(CREATE_INPUT);
    void *value = scenario.stopAt == FAIL_FACTORY ? NULL : &inputInterface;
    std::memcpy(output, &value, sizeof(value));
    return scenario.stopAt == FAIL_FACTORY ? -1 : scenario.successfulHresult;
}
struct InputLogFixture
{
    void Log(const char *message)
    {
        CHECK(message != NULL && std::strlen(message) > 0); scenario.events.push_back(LOG_MESSAGE);
    }
} g_GameErrorContext;

#define HINSTANCE InputModuleHandleFixture
#define DIDEVCAPS InputCapsFixture
#define Supervisor InputSupervisorFixture
#define GetWindowLongPtrA ReadWindowModuleFixture
#define DirectInput8Create CreateInputFixture
#include "SupervisorSetupDInput.inl"
#undef DirectInput8Create
#undef GetWindowLongPtrA
#undef Supervisor
#undef DIDEVCAPS
#undef HINSTANCE
}
using namespace th095;
static int cases;

static void Reset(InputSupervisorFixture *receiver, InputModuleHandleFixture module,
                  int stage, bool controller, u32 ignored=0)
{
    scenario=InputScenario(); scenario.receiver=receiver;scenario.module=module;
    scenario.stopAt=stage;scenario.publishController=controller;scenario.ignoredFailures=ignored;
    std::memset(&g_Supervisor,0,sizeof(g_Supervisor));
    if (receiver != &g_Supervisor) std::memset(receiver,0,sizeof(*receiver));
    receiver->gameWindow=&windows[receiver == &g_Supervisor ? 0 : 1];
    receiver->config.options.disableDirectInput=stage == DISABLED;
    if (stage == DISABLED)
    {
        receiver->directInput=&oldInput;receiver->keyboard=&oldKeyboard;receiver->controller=&oldController;
    }
}
static void Verify(InputSupervisorFixture *receiver)
{
    const int stage=scenario.stopAt;
    CHECK(receiver->SetupDInput() == (stage == FAIL_NONE ? 0 : -1)); ++cases;
    std::vector<int> expected={WINDOW_READ};
    if (stage == DISABLED)
    {
        CHECK(receiver->directInput == &oldInput && receiver->keyboard == &oldKeyboard && receiver->controller == &oldController);
    }
    else
    {
        expected.push_back(CREATE_INPUT);
        if (stage != FAIL_FACTORY)
        {
            expected.push_back(CREATE_KEYBOARD);
            if (stage != FAIL_KEYBOARD)
            {
                expected.push_back(KEY_FORMAT);
                if (stage != FAIL_FORMAT) expected.push_back(KEY_COOP);
                if (stage == FAIL_FORMAT || stage == FAIL_COOP) expected.push_back(KEY_RELEASE);
                else
                {
                    expected.push_back(KEY_ACQUIRE);expected.push_back(LOG_MESSAGE);expected.push_back(ENUM_DEVICES);
                    if (scenario.publishController)
                    {
                        const int controllerEvents[]={CTRL_FORMAT,CTRL_COOP,CTRL_CAPS,CTRL_OBJECTS,LOG_MESSAGE};
                        expected.insert(expected.end(),controllerEvents,controllerEvents+5);
                    }
                }
            }
            if (stage != FAIL_NONE) expected.push_back(INPUT_RELEASE);
        }
        if (stage != FAIL_NONE) expected.push_back(LOG_MESSAGE);
        CHECK(receiver->directInput == (stage == FAIL_NONE ? &inputInterface : NULL));
        CHECK(receiver->keyboard == (stage == FAIL_NONE ? &keyboardDevice : NULL));
        CHECK(receiver->controller == (stage == FAIL_NONE && scenario.publishController ? &controllerDevice : NULL));
    }
    CHECK(expected == scenario.events);
    CHECK(scenario.inputReleases == (stage >= FAIL_KEYBOARD ? 1 : 0));
    CHECK(scenario.keyReleases == (stage >= FAIL_FORMAT ? 1 : 0));
    if (stage == FAIL_NONE && scenario.publishController)
    {
        CHECK(g_Supervisor.controllerCaps.marker == 42);
        CHECK(g_Supervisor.controllerCaps.dwSize == sizeof(InputCapsFixture));
        if (receiver != &g_Supervisor) CHECK(receiver->controllerCaps.dwSize == 0);
    }
}
int main()
{
    static_assert(sizeof(void *) == 8 && sizeof(std::intptr_t) == 8,"native pointer transport");
    InputModuleHandleFixture values[]={NULL,&modules[0],&modules[1]};
    for (InputModuleHandleFixture module : values)
    {
        if (module != NULL)
        {
            CHECK(reinterpret_cast<std::uintptr_t>(module) > UINT32_MAX);
            const std::int32_t oldResult=static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(module));
            CHECK(reinterpret_cast<InputModuleHandleFixture>(static_cast<std::intptr_t>(oldResult)) != module);
        }
        for (int stage=FAIL_NONE;stage<=FAIL_COOP;++stage)
            for (int controller=0;controller<2;++controller)
            { Reset(&g_Supervisor,module,stage,controller!=0);Verify(&g_Supervisor); }
        for (u32 ignored=0;ignored<64;++ignored)
            for (int controller=0;controller<2;++controller)
            { Reset(&g_Supervisor,module,FAIL_NONE,controller!=0,ignored);Verify(&g_Supervisor); }
        const i32 nonnegativeResults[]={1,INT32_MAX};
        for (i32 result : nonnegativeResults)
        { Reset(&g_Supervisor,module,FAIL_NONE,true);scenario.successfulHresult=result;Verify(&g_Supervisor); }
        InputSupervisorFixture receiver;
        Reset(&receiver,module,FAIL_NONE,true);Verify(&receiver);
    }
    std::printf("shared input setup: %d scenarios passed; high module bits retained\n",cases);
}
