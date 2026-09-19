// Compiler-emission adapter for the historical PhotoCamera Player receiver
// and global spelling. It deliberately owns no Player or camera storage.
struct PhotoGameStateView
{
    f32 AngleToPoint(const Float3 *point);
};

extern PhotoGameStateView *g_PhotoGame;
