#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"

namespace th095
{

class GameErrorContext
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
