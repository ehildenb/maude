//
//	Banner gets recompiled every build to update time; so keep it small.
//
#include <time.h>
#include <sys/time.h>
#include <ostream>
using namespace std;
#include "tty.hh"

void
printBanner(std::ostream& s)
{
  struct timeval t;
  gettimeofday(&t, 0);
  time_t secs = t.tv_sec;
 
  s << "\t\t     \\||||||||||||||||||/\n";
  s << "\t\t   --- Welcome to " <<
    Tty(Tty::RED) << 'M' <<
    Tty(Tty::CYAN) << 'a' <<
    Tty(Tty::BLUE) << 'u' <<
    Tty(Tty::MAGENTA) << 'd' <<
    Tty(Tty::GREEN) << 'e' <<
    Tty(Tty::RESET) << " ---\n";
  s << "\t\t     /||||||||||||||||||\\\n";
  s << "\tMaude alpha 80a built: " <<
    __DATE__ << ' ' << __TIME__ << '\n';
  s << "\t     Copyright 1997-2003 SRI International\n";
  s << "\t\t   " << ctime(&secs);
}
