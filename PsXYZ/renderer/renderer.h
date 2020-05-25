#ifndef renderer_h
#define renderer_h

#include <SDL.h>
#include <GL/glfw3.h>

class Renderer{
private:
  SDL_Window* window;

  bool wireframeEnabled;

public:
   Renderer();
  ~Renderer();

  void update();

  void drawMonochromeQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint8 r, Uint8 g, Uint8 b);
  void drawMonochromeTri(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b);
  void drawShadedQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 r3, Uint8 g3, Uint8 b3, Uint8 r4, Uint8 g4, Uint8 b4);
  void drawShadedTri(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 r3, Uint8 g3, Uint8 b3);
  void drawBlendedTexturedQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

  void drawVRAM(Uint16* vram);
};

#endif // renderer
