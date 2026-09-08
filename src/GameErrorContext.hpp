#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"

namespace th095
{

#ifdef TH095_MATCH_GAME_ERROR_CONTEXT_AS_CLASS
class GameErrorContext
#else
struct GameErrorContext
#endif
{
  public:
    GameErrorContext();
    ~GameErrorContext();

    void ResetContext()
    {
        this->bufferEnd = this->buffer;
        this->bufferEnd[0] = '\0';
    }

    void Flush();

    const char *Log(const char *fmt, ...);
    const char *Fatal(const char *fmt, ...);

  private:
    char buffer[0x2000];
    char *bufferEnd;
    i8 showMessageBox;
};

DIFFABLE_EXTERN(GameErrorContext, g_GameErrorContext);

} // namespace th095
