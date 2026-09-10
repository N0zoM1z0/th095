# Runtime issue ledger

This ledger tracks whole-program failures separately from function-level exact
matching. A successful link or exact function comparison does not close a
runtime issue; closure requires a reproduced cause plus a relevant build and
runtime check. Conversely, a runtime repair does not grant exact credit.

Status meanings:

- `closed`: the cause is established and the repaired path was exercised;
- `fixed / confirmation pending`: the evidence-backed repair is built, but the
  exact user-observed interaction still needs a post-fix manual replay;
- `open`: no accepted cause or repair yet.

## Issue index

| ID | Status | User-visible symptom | Established cause / disposition |
| --- | --- | --- | --- |
| RT-001 | closed | Startup could exit before reaching a usable title screen. | The production front-end callback ran before asynchronous title loading cleared its real bit-0 barrier. Production now waits for that barrier; exact compilation retains the target wrapper. |
| RT-002 | closed | Mission Select could not enumerate/start the real scenes. | Production lacked backing storage for the 93 scene records, group tables, and unlock/capture tables. The canonical target data is reconstructed. |
| RT-003 | closed | Entering gameplay faulted. | Background/ANM read a null duplicate of target `0x004C4A34`; both now use `g_Supervisor.currentBackgroundViewport @ +0x3C4`. |
| RT-004 | closed | Retry/result transitions could exit. | The runnable link did not consistently publish the live task through `g_Supervisor.photoGameTask @ 0x004C4DF4`, and one attempted repair incorrectly collapsed it with standalone slot `0x004BDEC8`. Production now preserves both target slots and their distinct lifetimes. |
| RT-005 | closed | Gameplay UI and bullets rendered, but the playfield background was black. | Stage scripts updated duplicate camera globals while rendering used Supervisor viewport 0. All background camera/script fields now share that embedded viewport owner. |
| RT-006 | closed | BGM was absent although graphics worked. | SoundPlayer used a legacy Supervisor layout and interpreted unrelated bytes as music disabled/preload enabled. Production accessors now use the TH095 Supervisor layout and configuration offsets. |
| RT-007 | closed | An unattended demo could exit after the player was hit; similar transitions were intermittent. | The startup worker and main thread could postload the same ANM entry concurrently. Production serializes the two consumers through Supervisor critical section 6. Three fresh-prefix demo runs passed all 21 checkpoints, and the gameplay escape/retry scenario passed all five. |
| RT-008 | closed | ESC back to title produced a title background with no six menu labels and a stray Japanese scene-description fragment. | SceneSelect and MusicRoom cleared/wrote `title.anm` entry 0 instead of the shared writable text ANM at `g_Supervisor.textAnm` (`0x004C4AAC`). Result replay-label VMs had the same wrong-owner family. All production consumers now use the embedded text owner; paired original/reconstruction return scenarios render the menu again and remain alive. |
| RT-009 | closed | The title screen's third and fourth entries opened Options and Music Room in the wrong order. | The source cases and private switch-table labels were paired incorrectly. Target row 2 requests state 8 (Music Room), while row 3 requests state 7 (Options). The linked routing and relocation manifest now encode that mapping; paired entry tests open the same screens as the original. |
| RT-010 | fixed / confirmation pending | After saving a replay and choosing the keyboard's Finish entry, the user's build exited. | `ResultScreen::Update` states 13/14/15 are exact and state 15 writes the replay then returns to the slot list. Four replay-label VM initializations instead used the result-screen ANM rather than target `g_Supervisor.textAnm`; that owner defect is now repaired and built. The precise save -> Finish interaction has not yet been repeated post-fix, so do not record it as runtime-closed. |

## Shared text ANM incident

Target `0x004C4AAC` is `g_Supervisor.textAnm`. A hash-attested Ghidra xref
review found thirteen references:

- PhotoCard initialization at `0x004086FC`;
- startup publication/use at `0x004242FE/0x00424303`;
- ResultScreen replay-label initialization at
  `0x00428B1C/0x00428B36/0x00428DC0/0x00428DD9`;
- SceneSelect preview construction at `0x0044861D` and its expanded texture
  clears at `0x0044BF17/0x0044C188/0x0044C3F9`; and
- Music Room text use at `0x004510E1/0x0045134E`.

PhotoCard was already routed correctly. Production SceneSelect, MusicRoom, and
ResultScreen were not. The repair is intentionally one owner family:

- `SceneSelectUpdate.cpp` clears `g_Supervisor.textAnm->textures[0]`;
- `SceneSelectController.cpp` creates scene-description VMs from that owner;
- `MusicRoom.cpp` clears it and creates its eight description VMs from it;
- `ResultScreen.cpp` initializes replay-label VMs 21/22 from it in both replay
  initializer paths.

This was not an error in the accepted state-machine semantics. It was a
runnable-link receiver/owner error: several ANM objects expose the same API, so
a method call can compile, link, and even look structurally plausible while
mutating the wrong texture atlas.

## Switch-table comparison blind spot

`SceneSelectControllerView::UpdateMainMenu @ 0x00446A50` demonstrates that a
relocation-normalized object comparison is not sufficient when two private
labels have the same relocation type but different semantic destinations. The
target emits the state-7 block before state 8, but routes menu row 2 to state 8
and row 3 to state 7. The source now preserves both facts, and the adjacent
private-label entries in `config/match-units.toml` name their real targets.

Future switch reviews must verify the linked jump-table destination for each
case, not only masked bytes, mnemonic equality, or relocation counts.

## Current verification checkpoint

The repaired production build is a 780,288-byte PE32 i386 GUI executable with
SHA-256
`5cb15a02c5f787f64475e9b600ff5f82b0e4fd4fd1d895d09d7c88b24ecb95dc`.
The five affected sources replay 37/37 configured exact units.

Paired original/reconstruction runtime scenarios passed as follows:

| Scenario | Relevant reconstructed result |
| --- | --- |
| Mission Select -> ESC -> title | Six menu labels restored, stray preview text absent, process alive; returned-title RMSE `0.0183221`. |
| Title row 3 | Music Room opens in both processes; process alive; opened-screen RMSE `0.0328018`. |
| Title row 4 | Options opens in both processes; process alive; opened-screen RMSE `0.0200367`. |
| Music Room -> ESC -> title | Both return to title alive; returned-title RMSE `0.020138`. |
| Options -> ESC -> title | Both return to title alive; returned-title RMSE `0.021678`. |

The remaining required manual check is RT-010: reproduce replay save -> Finish
with the newly deployed build and confirm that the process returns to the
replay-slot list instead of exiting.

