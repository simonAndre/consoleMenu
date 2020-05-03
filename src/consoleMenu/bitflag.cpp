#include "bitflag.h"

namespace CONSOLEMENU_NAMESPACE
{

bitflag::bitflag() {}
bitflag::bitflag(unsigned char init) { _store = init; }

bool bitflag::Get(char iflag)
{
    return (((_store >> iflag) & 1) == 0x1);
}
void bitflag::Set(char iflag, bool val)
{
    if (val)
        _store |= 1 << iflag;
    else
        _store &= ~(1 << iflag);
}
bool bitflag::Switch(char iflag)
{
    _store ^= 1 << iflag;
    return (((_store >> iflag) & 1) == 0x1);
}
} // namespace CONSOLEMENU_NAMESPACE