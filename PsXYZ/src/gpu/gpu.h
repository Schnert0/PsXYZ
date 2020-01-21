#ifndef gpu_h
#define gpu_h

#include <SDL.h>
#include <GL/glfw.h>
#include <stdio.h>
#include <string.h>

class c_gpu{
private:
    Uint32 cmdBuffer[16];
    Uint8  bufferLen;
    Uint8  cmdLen;
public:
     c_gpu();
    ~c_gpu();

    void reset();

    void gp0Write(Uint32 data);

    void setLong(Uint32 data, Uint8 words);
    void drawMonochromeQuad();
    void drawShadedQuad();
    void drawShadedTri();
    void drawTexturedQuad();
};

#endif // gpu_h
