# Kaze no Gogai: Issue 1 play guide

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
`run-no-death-test.ps1` helper beside it. The launcher disables bullet/laser
death only in the new game process. It never modifies the executable on disk.
Close the game and use `launch-gogai.bat` to restore ordinary play. The timer
still runs and saves use the same play directory. A replay containing
collisions requires the no-death launcher for playback. The launcher accepts
only the verified issue-1 executable; other rebuilds are rejected.

| Scene | Subject | Theme | Required photos |
| --- | --- | --- | --- |
| 1 | Sakuya | Morning interview: sparse rings | 5 |
| 2 | Sakuya | Crossed headlines: intersecting fans | 5 |
| 3 | Ran | Rotating deadline: turning rings | 6 |
| 4 | Ran | Shutter margin: gaps between bursts | 6 |
| 5 | Yukari | Moving interview: follow the subject | 6 |
| 6 | Yukari | Double printing: two emission centers | 6 |
| 7 | Suika | Press conference: movement and aimed shots | 7 |
| 8 | Suika | Final edition: rings and a moving deadline | 8 |

Photography, collisions, scoring, charging, Best Shot and replays use the native
engine. Scores, photographs and replays are stored in this standalone play
directory. Original-installation saves are separate. See [README](README.md)
for source builds and validation.
