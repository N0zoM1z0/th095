# Window module width and DirectInput initialization

This is the bounded original Japanese TH095 v1.02a SetupDInput client path.
SupervisorSetupDInput.inl, SupervisorInitializeInput.inl and
SupervisorEnumGameControllers.inl each serve both existing Main emission routes.
The axis-property callback, real input backend, parent layouts and full 64-bit
game remain separate work.

## Window-associated module value

Target SetupDInput at 0x00423960 is 689 bytes. It calls the GetWindowLongA IAT
slot at 0x004951DC with receiver gameWindow and index -6, then forwards the
returned module value as DirectInput8Create's first argument. CreateGameWindow
at 0x00420B90 supplies the HINSTANCE to window registration/creation. The input
initializer reads the window's associated module, rather than substituting the
global Supervisor instance. The client introduces no module-handle release.

GetWindowLongA returns a Windows LONG: four bytes even on Windows x64. Turning
that result into HINSTANCE afterward cannot recover high module bits. The
shared source uses reinterpret_cast<HINSTANCE>(GetWindowLongPtrA(...,
GWLP_HINSTANCE)). Actual VC7.1 and MinGW x86 SDK headers map this to the original
GetWindowLongA import and index -6. On x64 the getter returns LONG_PTR and uses
the native GetWindowLongPtrA import; HINSTANCE and LONG_PTR are both eight bytes,
while LONG remains four. No local platform/profile selector or fabricated SDK
type is needed.

The real MinGW x64 body moves the getter result from RAX to RCX as a full
64-bit value before DirectInput8Create. A controlled probe redirects only the
getter to the legacy API: the x86 SDK accepts its native-sized result, but the
x64 SDK rejects its LONG-returning function pointer where LONG_PTR is required.
This type failure and native high-address transport tests independently expose
the old width loss.

## Preserved initialization and failure transitions

The module query remains **before** the disableDirectInput test, including on
the disabled path. The target does not separately inspect a zero getter result
or last error; it forwards that value to the factory and follows its HRESULT.

| Step | Preserved behavior |
| --- | --- |
| Input disabled | Return -1 after the module query; existing input/device pointers remain unchanged |
| DirectInput8Create fails | Clear directInput, log, return -1 |
| Keyboard CreateDevice fails | Release and clear directInput, log, return -1 |
| Keyboard SetDataFormat fails | Release/clear keyboard, then directInput; log and return -1 |
| Keyboard SetCooperativeLevel fails | The same keyboard-before-interface cleanup; log and return -1 |
| Keyboard ready | Call Acquire, log success, then enumerate attached game controllers |
| Controller present after enumeration | Set format/cooperative level; set the **global** controllerCaps size; get capabilities, enumerate objects and log |

The checked HRESULTs use sign, so nonnegative results continue. Acquire,
EnumDevices and the controller configuration/capability/object-enumeration
results remain ignored as in the target. The body still returns 0 after these
calls. This batch does not add error handling, repeated-initialization safety,
controller teardown or assumptions about failure out-parameters beyond the
modeled traces. The original setup fixtures stage controller availability at
the backend boundary. Additional SEM-343 scenarios execute the actual shared
device-selection callback; the axis-property callback remains identity-only.

## Device selection and availability publication

The 68-byte device callback at 0x00423C20 ignores context and uses the global
Supervisor's directInput and controller slots. It passes the enumerated
instance's guidInstance by reference to CreateDevice, with the global controller
slot as output and a null outer object. The shared body uses SDK DIENUM names:

| Callback condition | Return and transition |
| --- | --- |
| A controller is already present | DIENUM_STOP (0), without reading the instance or calling CreateDevice |
| CreateDevice returns a negative HRESULT | DIENUM_CONTINUE (1), without clearing or releasing its output |
| CreateDevice returns any nonnegative HRESULT | DIENUM_STOP (0), without a second pointer-presence test |

Thus a synthetic nonnegative/null output still stops. A failure that publishes
a pointer continues once, but a later callback sees that existing pointer and
stops without another creation. These fault injections expose the actual branch
contract; they do not establish which failure out-parameters real COM permits.
Neither the callback context nor a different SetupDInput receiver redirects the
callback's global storage. No callback-side release is introduced.

The 127-byte fastcall InitializeInput at 0x004238E0 also ignores its argument.
It clears global keyboardAvailable and controllerAvailable, calls global
SetupDInput, then derives those bits from the final global keyboard/controller
pointers. It ignores the setup return code and preserves every other flag bit.
StartInputWorker at 0x00423CE0 supplies the global Supervisor argument to the
standalone worker, but the callback does not consume that argument. In
particular, disabling setup leaves existing pointers unchanged, so publication
can still report them available. This is sequential publication, not proof of
cross-thread synchronization or an OS join.

## Ownership and build boundary

Main still owns TU emission. The receiver supplies its HWND, options and input/
keyboard/controller slots; controller capabilities are deliberately written to
the global Supervisor. The module value is a transient window-associated input
to the factory. Existing release order and publication behavior are retained.
The global Supervisor owns device slots and availability bits; the enumerated
instance/GUID and callback context are borrowed arguments, with context unused.
No canonical Supervisor layout, COM ownership model or real OS backend is newly
accepted by fixture layouts. Exact and normal production use the same three
bodies, while their surrounding declarations remain separate debt.

## Independent oracles

- Fresh attested target initialization, window creation, input-worker caller
  and controller callback evidence establish the target client boundary.
- Actual baseline/candidate pinned-VC7.1 objects preserve all 884 target bytes
  and 44 relocation identities, positions, types and addends across setup,
  initialization and device selection. SEM-342's minimal pinned getter probe
  emits the same 23 bytes/relocation for legacy and native SDK spellings on x86.
  Exact matching alone is not normal-body acceptance.
- Independent normal Main graph comparison covers every emitted function and
  its referenced code/data/EH. Aggregate and whole-build results are recorded
  separately in the accepted batch history.
- SupervisorSetupDInputSdkTest.cpp compiles all three actual shared bodies with real
  Windows and DirectInput headers under VC7.1 x86 and MinGW x86/x64, also with
  the target's _WIN32_WINNT=0x0500 setting. Receiver/logger bindings are explicit
  fixtures, but the API types, COM methods and callback declarations are real
  SDK declarations. ABI receipts verify native pointers, four-byte LONG,
  44-byte DIDEVCAPS and index -6. Uncast function-pointer assignments check the
  SDK device callback and fastcall worker callback signatures. BOOL and the
  canonical SupervisorFlags remain four bytes; DIENUM_STOP/CONTINUE are 0/1.
  These are compile-only checks.
- The thirteenth native x86-64/UBSan program passes 523 sequential fixture
  scenarios. Actual allocated module pointers exceed 32 bits and reach the
  factory intact; deliberate narrowing loses their identity. The tests cover
  null/high module values, disable/failure paths, exact release/log order,
  nonnegative HRESULTs, all ignored-failure masks, controller absence/presence
  and receiver/global capability ownership. The 94 added cases cover sixteen
  direct device-callback cases, six enumerated traces through shared setup and
  72 availability-publication cases, including null/unrelated arguments and
  all initial availability-bit combinations. Only device-selection callbacks
  execute; the axis-property callback does not. Not every injected combination
  is claimed reachable through the real OS.

The private source/command/COFF/SDK/control receipts are below .analysis/sem342/
and .analysis/sem343/.
For the SDK compile-only test use compile-probe.sh with Main's normal flags
and /I src, or each MinGW compiler with -std=c++98 -O2 -Wall -Wextra -Werror
-pedantic -D_WIN32_WINNT=0x0500 -Isrc -c and an output path below build/.

TH08 corroborates the window-associated retrieval/client and device-selection
callback shape. Its current
32-bit source also uses GetWindowLongA; it supplies no authority for the native
width correction or TH095 parent layout.
