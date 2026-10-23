# Window module width and DirectInput initialization

This is the bounded original Japanese TH095 v1.02a SetupDInput client path.
One shared SupervisorSetupDInput.inl serves both existing Main emission routes.
The real input backend, callbacks, parent layouts and full 64-bit game remain
separate work.

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
modeled traces. Native tests stage controller availability at the backend
boundary and check callback identity; they do not execute the callback bodies.

## Ownership and build boundary

Main still owns TU emission. The receiver supplies its HWND, options and input/
keyboard/controller slots; controller capabilities are deliberately written to
the global Supervisor. The module value is a transient window-associated input
to the factory. Existing release order and publication behavior are retained.
No canonical Supervisor layout, COM ownership model or real OS backend is newly
accepted by fixture layouts. Both exact and normal production now use the same
function body, while their surrounding declarations remain separate debt.

## Independent oracles

- Fresh attested target initialization, window creation, input-worker caller
  and controller callback evidence establish the target client boundary.
- Actual baseline/candidate pinned-VC7.1 objects preserve all 689 target bytes
  and all 28 relocation identities, positions, types and addends. A minimal
  pinned getter probe emits the same 23 bytes/relocation for legacy and native
  SDK spellings on x86. Exact matching alone is not normal-body acceptance.
- Independent normal Main graph comparison covers every emitted function and
  its referenced code/data/EH. Aggregate and whole-build results are recorded
  separately in SEM-342.
- SupervisorSetupDInputSdkTest.cpp compiles the actual shared body with real
  Windows and DirectInput headers under VC7.1 x86 and MinGW x86/x64, also with
  the target's _WIN32_WINNT=0x0500 setting. Receiver/logger bindings are explicit
  fixtures, but the API types, COM methods and callback declarations are real
  SDK declarations. ABI receipts verify native pointers, four-byte LONG,
  44-byte DIDEVCAPS and index -6. These are compile-only checks.
- The thirteenth native x86-64/UBSan program passes 429 sequential fixture
  scenarios. Actual allocated module pointers exceed 32 bits and reach the
  factory intact; deliberate narrowing loses their identity. The tests cover
  null/high module values, disable/failure paths, exact release/log order,
  nonnegative HRESULTs, all ignored-failure masks, controller absence/presence
  and receiver/global capability ownership. Not every injected combination is
  claimed reachable through the real OS or real enumeration callback.

The private source/command/COFF/SDK/control receipts are below .analysis/sem342/.
For the SDK compile-only test use compile-probe.sh with Main's normal flags
and /I src, or each MinGW compiler with -std=c++98 -O2 -Wall -Wextra -Werror
-pedantic -D_WIN32_WINNT=0x0500 -Isrc -c and an output path below build/.

TH08 corroborates the window-associated retrieval/client shape. Its current
32-bit source also uses GetWindowLongA; it supplies no authority for the native
width correction or TH095 parent layout.
