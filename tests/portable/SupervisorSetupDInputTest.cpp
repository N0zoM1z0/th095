// Actual setup, device selection, availability and normal axis callback bodies.
// Native pointers, COM operations, object records and logging use explicit
// fixtures, not SDK/parent ABI. The exact wrapper's unused copy is separate debt.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include "inttypes.hpp"
#include "SupervisorFlags.hpp"

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "input check line %d: %s\n", __LINE__, #x); std::abort(); } } while (0)
namespace th095
{
struct InputWindowFixture { int tag; } windows[2];
struct InputModuleFixture { int tag; } modules[2];
typedef InputModuleFixture *InputModuleHandleFixture;
struct InputGuidFixture { int tag; };
struct InputDeviceInstanceFixture { InputGuidFixture guidInstance; };
struct InputObjectInstanceFixture { u32 dwType; };
struct InputPropertyHeaderFixture { u32 dwSize, dwHeaderSize, dwObj, dwHow; };
struct InputPropertyRangeFixture { InputPropertyHeaderFixture diph; i32 lMin, lMax; };
struct InputFormatFixture { int tag; };
struct InputCapsFixture { u32 dwSize; u32 marker; };
const InputGuidFixture IID_IDirectInput8A = {1}, GUID_SysKeyboard = {2};
const InputGuidFixture DIPROP_RANGE = {3};
const InputFormatFixture c_dfDIKeyboard = {1}, c_dfDIJoystick2 = {2};
enum { DIRECTINPUT_VERSION=0x800, GWLP_HINSTANCE=-6,
       DISCL_NONEXCLUSIVE=2, DISCL_FOREGROUND=4, DISCL_BACKGROUND=8, DISCL_NOWINKEY=16,
       DI8DEVCLASS_GAMECTRL=4, DIEDFL_ATTACHEDONLY=1,
       DIENUM_STOP=0, DIENUM_CONTINUE=1, DIDFT_AXIS=3, DIPH_BYID=2 };
enum { WINDOW_READ=1, CREATE_INPUT, CREATE_KEYBOARD, KEY_FORMAT, KEY_COOP, KEY_ACQUIRE,
       LOG_MESSAGE, ENUM_DEVICES, CTRL_FORMAT, CTRL_COOP, CTRL_CAPS, CTRL_OBJECTS,
       KEY_RELEASE, INPUT_RELEASE, CREATE_CONTROLLER, SET_AXIS_RANGE };
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
    SupervisorFlags flags;
    i32 SetupDInput();
    static void InitializeInput(void *);
    static i32 EnumGameControllersCb(const InputDeviceInstanceFixture *, void *);
    static i32 ControllerCallback(const InputObjectInstanceFixture *, void *);
};
struct InputControllerResultFixture { i32 result; InputDeviceFixture *output; };
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
    bool executeDeviceCallback, checkAvailabilityClear;
    std::vector<InputDeviceInstanceFixture> devices;
    std::vector<InputControllerResultFixture> controllerResults;
    std::vector<const InputGuidFixture *> createdGuids;
    std::vector<int> visitedDevices;
    bool executeObjectCallback;
    i32 propertyResult;
    std::vector<InputObjectInstanceFixture> objects;
    std::vector<u32> visitedObjects;
    std::vector<InputPropertyRangeFixture> ranges;
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
    i32 SetProperty(const InputGuidFixture &property, const InputPropertyHeaderFixture *header)
    {
        CHECK(this == g_Supervisor.controller && !isKeyboard && &property == &DIPROP_RANGE);
        CHECK(header->dwSize == sizeof(InputPropertyRangeFixture));
        CHECK(header->dwHeaderSize == sizeof(InputPropertyHeaderFixture) && header->dwHow == DIPH_BYID);
        const InputPropertyRangeFixture *range=reinterpret_cast<const InputPropertyRangeFixture *>(header);
        CHECK(range->lMin == -1000 && range->lMax == 1000);
        scenario.ranges.push_back(*range); scenario.events.push_back(SET_AXIS_RANGE);
        return scenario.propertyResult;
    }
    i32 EnumObjects(i32 (*callback)(const InputObjectInstanceFixture *, void *), void *context, u32 flags)
    {
        CHECK(!isKeyboard && callback == &InputSupervisorFixture::ControllerCallback);
        CHECK(context == NULL && flags == 0); scenario.events.push_back(CTRL_OBJECTS);
        if (scenario.executeObjectCallback)
        {
            for (const InputObjectInstanceFixture &object : scenario.objects)
            {
                scenario.visitedObjects.push_back(object.dwType);
                const i32 result=callback(&object,context);
                CHECK(result == DIENUM_STOP || result == DIENUM_CONTINUE);
                if (result == DIENUM_STOP) break;
            }
        }
        return scenario.ignoredFailures & 32u ? -1 : 0;
    }
} keyboardDevice={true}, controllerDevice={false}, oldKeyboard={true}, oldController={false};

struct InputInterfaceFixture
{
    i32 CreateDevice(const InputGuidFixture &guid, InputDeviceFixture **output, void *outer)
    {
        if (&guid != &GUID_SysKeyboard)
        {
            CHECK(scenario.executeDeviceCallback && outer == NULL);
            CHECK(this == g_Supervisor.directInput && output == &g_Supervisor.controller);
            const std::size_t index = scenario.createdGuids.size();
            CHECK(index < scenario.controllerResults.size());
            scenario.createdGuids.push_back(&guid); scenario.events.push_back(CREATE_CONTROLLER);
            *output = scenario.controllerResults[index].output;
            return scenario.controllerResults[index].result;
        }
        CHECK(&guid == &GUID_SysKeyboard && outer == NULL);
        CHECK(output == &scenario.receiver->keyboard); scenario.events.push_back(CREATE_KEYBOARD);
        *output = scenario.stopAt == FAIL_KEYBOARD ? NULL : &keyboardDevice;
        return scenario.stopAt == FAIL_KEYBOARD ? -1 : scenario.successfulHresult;
    }
    u32 Release()
    {
        ++scenario.inputReleases; scenario.events.push_back(INPUT_RELEASE); return 0;
    }
    i32 EnumDevices(u32 kind, i32 (*callback)(const InputDeviceInstanceFixture *, void *), void *context, u32 flags)
    {
        CHECK(kind == DI8DEVCLASS_GAMECTRL && flags == DIEDFL_ATTACHEDONLY);
        CHECK(callback == &InputSupervisorFixture::EnumGameControllersCb && context == NULL);
        scenario.events.push_back(ENUM_DEVICES);
        if (scenario.executeDeviceCallback)
        {
            for (std::size_t i=0; i<scenario.devices.size(); ++i)
            {
                scenario.visitedDevices.push_back(static_cast<int>(i));
                const i32 result=callback(&scenario.devices[i],context);
                CHECK(result == DIENUM_STOP || result == DIENUM_CONTINUE);
                if (result == DIENUM_STOP) break;
            }
        }
        else if (scenario.publishController) scenario.receiver->controller = &controllerDevice;
        return scenario.ignoredFailures & 2u ? -1 : 0;
    }
} inputInterface, oldInput;

std::intptr_t ReadWindowModuleFixture(InputWindowFixture *window, int index)
{
    CHECK(window == scenario.receiver->gameWindow && index == GWLP_HINSTANCE);
    if (scenario.checkAvailabilityClear)
        CHECK(g_Supervisor.flags.keyboardAvailable == 0 && g_Supervisor.flags.controllerAvailable == 0);
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
#define BOOL i32
#define CALLBACK
#define LPCDIDEVICEINSTANCEA const InputDeviceInstanceFixture *
#define LPVOID void *
#define HRESULT i32
#define __fastcall
#define LPCDIDEVICEOBJECTINSTANCEA const InputObjectInstanceFixture *
#define DIPROPRANGE InputPropertyRangeFixture
#define DIPROPHEADER InputPropertyHeaderFixture
#include "SupervisorInitializeInput.inl"
#include "SupervisorEnumGameControllers.inl"
#include "SupervisorControllerCallback.inl"
#undef DIPROPHEADER
#undef DIPROPRANGE
#undef LPCDIDEVICEOBJECTINSTANCEA
#undef __fastcall
#undef HRESULT
#undef LPVOID
#undef LPCDIDEVICEINSTANCEA
#undef CALLBACK
#undef BOOL
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
static u32 FlagWord(const SupervisorFlags &flags)
{
    u32 word;
    std::memcpy(&word,&flags,sizeof(word));
    return word;
}
static void Verify(InputSupervisorFixture *receiver, bool worker=false, void *argument=NULL)
{
    const int stage=scenario.stopAt;
    const u32 before=FlagWord(g_Supervisor.flags);
    if (worker) InputSupervisorFixture::InitializeInput(argument);
    else CHECK(receiver->SetupDInput() == (stage == FAIL_NONE ? 0 : -1));
    ++cases;
    if (worker)
    {
        CHECK(receiver == &g_Supervisor);
        const u32 expected=(before & ~0xC00u) | (receiver->keyboard != NULL ? 0x400u : 0)
                           | (receiver->controller != NULL ? 0x800u : 0);
        CHECK(FlagWord(g_Supervisor.flags) == expected);
    }
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
static void CheckDeviceCallbacks()
{
    InputDeviceInstanceFixture instance={{81}};
    InputSupervisorFixture unrelated;
    const i32 results[]={INT32_MIN,-1,0,1,INT32_MAX};
    InputDeviceFixture *outputs[]={NULL,&controllerDevice,&oldController};
    // The context is a valid, different owner; neither callback storage nor
    // lifetime comes from it. Out-parameter fault combinations are synthetic.
    for (i32 result : results)
        for (InputDeviceFixture *output : outputs)
        {
            Reset(&g_Supervisor,&modules[0],FAIL_NONE,false);
            unrelated=InputSupervisorFixture(); unrelated.controller=&oldKeyboard;
            scenario.executeDeviceCallback=true;
            scenario.controllerResults.push_back({result,output});
            g_Supervisor.directInput=&inputInterface;
            CHECK(InputSupervisorFixture::EnumGameControllersCb(&instance,&unrelated)
                  == (result < 0 ? DIENUM_CONTINUE : DIENUM_STOP));
            CHECK(g_Supervisor.controller == output && unrelated.controller == &oldKeyboard);
            CHECK(scenario.createdGuids.size() == 1 && scenario.createdGuids[0] == &instance.guidInstance);
            CHECK(reinterpret_cast<std::uintptr_t>(scenario.createdGuids[0]) > UINT32_MAX);
            CHECK(scenario.events == std::vector<int>({CREATE_CONTROLLER}));
            ++cases;
        }
    // A pre-existing device stops enumeration before touching a null instance
    // or input interface. The argument/context do not supply an alternate owner.
    Reset(&g_Supervisor,&modules[0],FAIL_NONE,false);
    g_Supervisor.controller=&oldController;
    CHECK(InputSupervisorFixture::EnumGameControllersCb(NULL,NULL) == DIENUM_STOP);
    CHECK(scenario.events.empty()); ++cases;

    // Execute the real callback through the shared setup's backend binding.
    // Trace 0: no devices. Trace 1: failures, then success, then an unvisited
    // device. Trace 2: nonnegative/null stops immediately. Trace 3: a failure
    // publishes a pointer; the next callback stops without another creation.
    // Trace 4: all attempts fail. Trace 5: callback writes global storage even
    // when SetupDInput runs on a different receiver/interface.
    for (int trace=0;trace<6;++trace)
    {
        InputSupervisorFixture receiver;
        Reset(trace == 5 ? &receiver : &g_Supervisor,&modules[1],FAIL_NONE,false);
        scenario.executeDeviceCallback=true;
        if (trace != 0) scenario.devices={{{11}},{{12}},{{13}},{{14}}};
        switch (trace)
        {
        case 1: scenario.controllerResults={{-1,NULL},{INT32_MIN,NULL},{1,&controllerDevice}};break;
        case 2: scenario.controllerResults={{0,NULL}};break;
        case 3: scenario.controllerResults={{-1,&controllerDevice}};break;
        case 4: scenario.controllerResults={{-1,NULL},{-1,NULL},{-1,NULL},{-1,NULL}};break;
        case 5: scenario.controllerResults={{0,&controllerDevice}};g_Supervisor.directInput=&oldInput;break;
        }
        CHECK(scenario.receiver->SetupDInput() == 0);
        const std::size_t visits[]={0,3,1,2,4,1},creates[]={0,3,1,1,4,1};
        CHECK(scenario.visitedDevices.size() == visits[trace]);
        CHECK(scenario.createdGuids.size() == creates[trace]);
        for (std::size_t i=0;i<scenario.createdGuids.size();++i)
            CHECK(scenario.createdGuids[i] == &scenario.devices[i].guidInstance);
        CHECK(g_Supervisor.controller == (trace == 1 || trace == 3 || trace == 5 ? &controllerDevice : NULL));
        if (trace == 5)
            CHECK(receiver.controller == NULL && g_Supervisor.directInput == &oldInput);
        CHECK(scenario.inputReleases == 0 && scenario.keyReleases == 0); ++cases;
    }
}
static void CheckAxisCallbacks()
{
    const struct ObjectCase { u32 type; bool axis; } objects[] = {
        {0,false},{1,true},{2,true},{3,true},{4,false},
        {0x100,false},{0x101,true},{0x102,true},{0x103,true},
        {0x7FFFFFFC,false},{0x7FFFFFFD,true},{0x80000002,true},
        {0xFFFFFFFC,false},{0xFFFFFFFD,true},{0xFFFFFFFE,true},{0xFFFFFFFF,true}
    };
    const i32 results[]={INT32_MIN,-1,0,1,INT32_MAX};
    for (const ObjectCase &item : objects)
        for (i32 result : results)
            for (int withContext=0;withContext<2;++withContext)
            {
                Reset(&g_Supervisor,&modules[0],FAIL_NONE,false);
                g_Supervisor.controller=&controllerDevice;
                InputSupervisorFixture unrelated={}; unrelated.controller=&oldController;
                InputObjectInstanceFixture object={item.type};
                scenario.propertyResult=result;
                CHECK(InputSupervisorFixture::ControllerCallback(&object,withContext ? &unrelated : NULL)
                      == (item.axis && result < 0 ? DIENUM_STOP : DIENUM_CONTINUE));
                CHECK(unrelated.controller == &oldController && g_Supervisor.controller == &controllerDevice);
                CHECK(scenario.ranges.size() == (item.axis ? 1u : 0u));
                if (item.axis)
                {
                    CHECK(scenario.ranges[0].diph.dwObj == item.type);
                    CHECK(scenario.events == std::vector<int>({SET_AXIS_RANGE}));
                }
                else CHECK(scenario.events.empty());
                ++cases;
            }
    for (u32 type : {0u,4u,0x100u,0xFFFFFFFCu})
    {
        Reset(&g_Supervisor,&modules[0],FAIL_NONE,false);
        InputObjectInstanceFixture object={type};
        CHECK(InputSupervisorFixture::ControllerCallback(&object,NULL) == DIENUM_CONTINUE);
        CHECK(g_Supervisor.controller == NULL && scenario.events.empty()); ++cases;
    }
    const struct Trace {
        std::vector<u32> objects, visited, configured;
        i32 propertyResult;
        u32 backendFailures;
        bool foreignReceiver;
    } traces[] = {
        {{},{},{},0,0,false},
        {{4,0x101,0x102},{4,0x101,0x102},{0x101,0x102},0,0,false},
        {{1,2,4},{1},{1},-1,32,false},
        {{4,1,2},{4,1},{1},INT32_MIN,32,false},
        {{0,4,0xFFFFFFFC},{0,4,0xFFFFFFFC},{},0,0,false},
        {{0x7FFFFFFD,0x80000002,0xFFFFFFFF},{0x7FFFFFFD,0x80000002,0xFFFFFFFF},
         {0x7FFFFFFD,0x80000002,0xFFFFFFFF},INT32_MAX,0,false},
        {{0x103},{0x103},{0x103},1,0,true}
    };
    for (const Trace &trace : traces)
    {
        InputSupervisorFixture receiver;
        Reset(trace.foreignReceiver ? &receiver : &g_Supervisor,&modules[1],FAIL_NONE,true,trace.backendFailures);
        if (trace.foreignReceiver) g_Supervisor.controller=&oldController;
        scenario.executeObjectCallback=true; scenario.propertyResult=trace.propertyResult;
        for (u32 type : trace.objects) scenario.objects.push_back({type});
        CHECK(scenario.receiver->SetupDInput() == 0);
        CHECK(scenario.visitedObjects == trace.visited);
        std::vector<u32> configured;
        for (const InputPropertyRangeFixture &range : scenario.ranges) configured.push_back(range.diph.dwObj);
        CHECK(configured == trace.configured);
        std::vector<int> expected={WINDOW_READ,CREATE_INPUT,CREATE_KEYBOARD,KEY_FORMAT,KEY_COOP,
            KEY_ACQUIRE,LOG_MESSAGE,ENUM_DEVICES,CTRL_FORMAT,CTRL_COOP,CTRL_CAPS,CTRL_OBJECTS};
        expected.insert(expected.end(),trace.configured.size(),SET_AXIS_RANGE); expected.push_back(LOG_MESSAGE);
        CHECK(scenario.events == expected);
        CHECK(scenario.inputReleases == 0 && scenario.keyReleases == 0);
        CHECK(scenario.receiver->controller == &controllerDevice);
        if (trace.foreignReceiver) CHECK(g_Supervisor.controller == &oldController);
        ++cases;
    }
}
int main()
{
    static_assert(sizeof(void *) == 8 && sizeof(std::intptr_t) == 8,"native pointer transport");
    static_assert(sizeof(InputPropertyHeaderFixture) == 16 && sizeof(InputPropertyRangeFixture) == 24,"fixture range representation");
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
    CheckDeviceCallbacks();
    CheckAxisCallbacks();
    InputSupervisorFixture ignoredOwner;
    std::memset(&ignoredOwner,0x5A,sizeof(ignoredOwner));
    unsigned char ignoredBefore[sizeof(ignoredOwner)];
    std::memcpy(ignoredBefore,&ignoredOwner,sizeof(ignoredOwner));
    void *arguments[]={NULL,&ignoredOwner,&modules[0]};
    for (void *argument : arguments)
        for (int stage=FAIL_NONE;stage<=FAIL_COOP;++stage)
            for (u32 availability=0;availability<4;++availability)
            {
                Reset(&g_Supervisor,&modules[0],stage,true);
                const u32 bits=(0xA5A55A5Au & ~0xC00u) | (availability << 10);
                std::memcpy(&g_Supervisor.flags,&bits,sizeof(bits));
                scenario.checkAvailabilityClear=true;
                Verify(&g_Supervisor,true,argument);
                CHECK(std::memcmp(ignoredBefore,&ignoredOwner,sizeof(ignoredOwner)) == 0);
            }
    std::printf("input setup/selection/publication/axis range: %d scenarios passed; high module bits retained\n",cases);
}
