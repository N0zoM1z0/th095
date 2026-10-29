# Kaze no Gogai: Sakuya revision play guide

This is a fan expansion made with the reconstructed TH095 engine. It is not
unreleased ZUN content.

Double-click `launch-gogai.bat` in the standalone play directory. Choose Game
Start, then press Left once from LEVEL 1 to select the reserved extra issue
(group 12). All eight scenes are immediately available.

Arrow keys move. Shift focuses. Shift+Z charges quickly; when fully charged,
release Shift, hold Z to aim, then release Z to take the photograph. Escape
pauses. Retry or return to scene selection after failure. During the opening
preparation marker, the subject cannot yet count as a successful photograph.

For no-death testing, double-click `run-no-death-test.bat` and keep its
`run-no-death-test.ps1` helper beside it. The launcher disables collision
death only in the new game process. It never modifies the executable on disk.
Close the game and use `launch-gogai.bat` to restore ordinary play. The timer
still runs and saves use the same play directory. A replay containing
collisions requires the no-death launcher for playback. The launcher accepts
only the verified Sakuya-revision executable; other rebuilds are rejected.

| Scene | Subject | Theme | Required photos |
| --- | --- | --- | --- |
| 1 | Sakuya | Silver Stage in Frozen Time: crossed knives, three frozen-time dashes | 6 |
| 2 | Sakuya | The Last Shutter: evade the camera, throw knives during the dash | 8 |
| 3 | Youmu | Rotating deadline (prototype) | 6 |
| 4 | Youmu | Shutter margin (prototype) | 6 |
| 5 | Yukari | Moving interview (prototype) | 6 |
| 6 | Yukari | Double printing (prototype) | 6 |
| 7 | Kaguya | Press conference (prototype) | 7 |
| 8 | Kaguya | Final edition (prototype) | 8 |

Photography, collisions, scoring, charging, Best Shot and replays use the native
engine. Scores, photographs and replays are stored in this standalone play
directory. Original-installation saves are separate. See [README](README.md)
for source builds and validation.


Sakuya's first spell stops time on a regular rhythm. Position before the stop,
read the split knife fans, then photograph her during the recovery window.
Her second spell reacts when an aimed frame contains her. Bait the dash,
follow the blue silhouettes, and re-aim at her destination during the evasion
cooldown. Ghosts do not count as photographic subjects. The other six scenes
have the requested cast but retain their earlier prototype attacks.
