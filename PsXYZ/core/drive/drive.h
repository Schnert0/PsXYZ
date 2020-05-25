#ifndef drive_h
#define drive_h

#include <SDL.h>
#include <stdio.h>
#include <string.h>

class Drive{
private:

public:
   Drive();
  ~Drive();

  Uint32 read(Uint32 offset);
  void  write(Uint32 offset, Uint32 data);
};

#endif // drive_h
