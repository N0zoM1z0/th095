#include "inttypes.hpp"
#include <string.h>

namespace th095
{

// FUNCTION: TH095 0x0041B920.
u8 *__fastcall ReadResultHelpLine(char *destination, u8 *source, i32 maxLength)
{
    i32 outputLength = 0;
    memset(destination, 0, maxLength);

    while ((i8)*source != '\n' && (i8)*source != '\r' && maxLength != 0)
    {
        destination[outputLength] = *source;
        outputLength++;

        if ((((u8)*source >= 0x81) && ((u8)*source <= 0x9f)) ||
            (((u8)*source >= 0xe0) && ((u8)*source <= 0xfc)))
        {
            source++;
            maxLength--;
            destination[outputLength] = *source;
            outputLength++;
        }
        source++;
        maxLength--;
    }

    while ((i8)*source == '\n' || (i8)*source == '\r')
    {
        source++;
    }
    return source;
}

} // namespace th095
