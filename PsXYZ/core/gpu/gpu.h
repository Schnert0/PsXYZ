#ifndef gpu_h
#define gpu_h

#include <SDL.h>
#include <GL/glfw3.h>
#include <stdio.h>
#include <string.h>

#include "../../renderer/renderer.h"

class GPU{
private:
  Renderer* renderer;

  union{
    Uint32 raw;
    struct {
  		Uint32 pageX:4;
  		Uint32 pageY:1;
  		Uint32 transparency:2;
  		Uint32 colors:2;
  		Uint32 dither:1;
  		Uint32 drawToDisplay:1;
  		Uint32 forceWriteMask:1;
  		Uint32 checkMask:1;
  		Uint32 interlaceField:1;
  		Uint32 reverse:1;
  		Uint32 texDisable:1;
  		Uint32 hres2:1;
  		Uint32 hres:2;
  		Uint32 vres:1;
  		Uint32 pal:1;
  		Uint32 outDepth:1;
  		Uint32 interlace:1;
  		Uint32 displayDisable:1;
  		Uint32 DMARequest:1;
  		Uint32 cmdReady:1;
  		Uint32 vramReady:1;
  		Uint32 DMAReady:1;
  		Uint32 DMADir:2;
  		Uint32 field:1;
     };
  }stat;

  struct{
    Uint8 texFlipX;
  	Uint8 texFlipY;
  	Uint8 texWinXMask;
  	Uint8 texWinYMask;
  	Uint8 texWinX;
  	Uint8 texWinY;
  	Uint16 clipX1;
  	Uint16 clipY1;
  	Uint16 clipX2;
  	Uint16 clipY2;
  	Sint16 drawX;
  	Sint16 drawY;
  	Uint16 displayX;
  	Uint16 displayY;
  	Uint16 pixelStart;
  	Uint16 pixelEnd;
  	Uint16 lineStart;
  	Uint16 lineEnd;
  }state;

  struct{
    Uint16  x, y, w, h;
    Uint32  size, remaining;
    Uint32  current;
    Uint32* buffer;
  }transfer;

  Uint32 cmdBuffer[16];
  Uint8  bufferLen;
  Uint8  cmdLen;

  union{
    Uint8  b[512][2048];
    Uint16 h[512][1024];
    Uint32 w[512][512];

    Uint16 a[512*1024];
  }vram;

  void setLong(Uint32 data, Uint8 words);

  void gp0DrawMode(Uint32 data);
  void gp0SetTopLeft(Uint32);
  void gp0SetBotRight(Uint32);
  void gp0SetDrawOffset(Uint32);
  void gp0TextureWindow(Uint32 data);
  void gp0MaskBit(Uint32 data);

  void gp1Reset();
  void gp1DisplayMode(Uint32 data);
  void gp1DMADirection(Uint32 data);
  void gp1VRAMStart(Uint32 data);
  void gp1DisplayHRange(Uint32 data);
  void gp1DisplayVRange(Uint32 data);
  void gp1DisplayEnable(Uint32 data);

  void drawMonochromeQuad();
  void drawShadedQuad();
  void drawShadedTri();
  void drawTexturedQuad();
  void transferToVRAM();
  void VRAMStore();

public:
   GPU();
  ~GPU();
  void getComponents(Renderer* rendererPtr);

  void reset();

  void gp0Write(Uint32 data);
  void gp1Write(Uint32 data);
};

#endif // gpu_h
