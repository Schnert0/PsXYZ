#include "renderer.h"

Renderer::Renderer(){
  window = SDL_CreateWindow("PsXYZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
  SDL_GL_CreateContext(window);
  glfwInit();
  glOrtho(0, 1024, 512, 0, -1, 1);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  wireframeEnabled = true;
}


Renderer::~Renderer(){
  SDL_DestroyWindow(window);
  SDL_Quit();
}


void Renderer::update(){
  SDL_GL_SwapWindow(window);
  //glClear(GL_COLOR_BUFFER_BIT);
}


void Renderer::drawMonochromeQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint8 r, Uint8 g, Uint8 b){
  if(wireframeEnabled){
    glBegin(GL_LINES);
      glColor3f((float)r/255, (float)g/255, (float)b/255);
      glVertex2i(x1, y1);
      glVertex2i(x2, y2);

      glVertex2i(x2, y2);
      glVertex2i(x3, y3);

      glVertex2i(x3, y3);
      glVertex2i(x4, y4);

      glVertex2i(x4, y4);
      glVertex2i(x1, y1);

      glVertex2i(x2, y2);
      glVertex2i(x4, y4);
    glEnd();

    return;
  }
  glBegin(GL_QUADS);
    glColor3f((float)r/255, (float)g/255, (float)b/255);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glVertex2i(x4, y4);
  glEnd();
}


void Renderer::drawMonochromeTri(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b){
  if(wireframeEnabled){
    glBegin(GL_LINES);
      glColor3f((float)r/255, (float)g/255, (float)b/255);
      glVertex2i(x1, y1);
      glVertex2i(x2, y2);

      glVertex2i(x2, y2);
      glVertex2i(x3, y3);

      glVertex2i(x3, y3);
      glVertex2i(x1, y1);
    glEnd();

    return;
  }

  glBegin(GL_TRIANGLES);
    glColor3f((float)r/255, (float)g/255, (float)b/255);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
  glEnd();
}


void Renderer::drawShadedQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 r3, Uint8 g3, Uint8 b3, Uint8 r4, Uint8 g4, Uint8 b4){
  if(wireframeEnabled){
    glBegin(GL_LINES);
      glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
      glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);

      glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);
      glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);

      glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);
      glColor3f((float)r4/255, (float)g4/255, (float)b4/255); glVertex2i(x4, y4);

      glColor3f((float)r4/255, (float)g4/255, (float)b4/255); glVertex2i(x4, y4);
      glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);

      glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);
      glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
    glEnd();

    return;
  }

  glBegin(GL_QUADS);
    glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
    glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);
    glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);
    glColor3f((float)r4/255, (float)g4/255, (float)b4/255); glVertex2i(x4, y4);
  glEnd();
}


void Renderer::drawShadedTri(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 r3, Uint8 g3, Uint8 b3){
    if(wireframeEnabled){
      glBegin(GL_LINES);
        glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
        glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);

        glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);
        glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);

        glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);
        glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
      glEnd();

      return;
    }

  glBegin(GL_TRIANGLES);
    glColor3f((float)r1/255, (float)g1/255, (float)b1/255); glVertex2i(x1, y1);
    glColor3f((float)r2/255, (float)g2/255, (float)b2/255); glVertex2i(x2, y2);
    glColor3f((float)r3/255, (float)g3/255, (float)b3/255); glVertex2i(x3, y3);
  glEnd();
}


void Renderer::drawBlendedTexturedQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
  if(wireframeEnabled){
    glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(x1, y1);
      glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(x2, y2);

      glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(x2, y2);
      glColor3f(0.0f, 0.0f, 1.0f); glVertex2i(x3, y3);

      glColor3f(0.0f, 0.0f, 1.0f); glVertex2i(x3, y3);
      glColor3f(0.0f, 0.0f, 0.0f); glVertex2i(x4, y4);

      glColor3f(0.0f, 0.0f, 0.0f); glVertex2i(x4, y4);
      glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(x1, y1);

      glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(x2, y2);
      glColor3f(0.0f, 0.0f, 0.0f); glVertex2i(x4, y4);
    glEnd();

    return;
  }

  glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(x1, y1);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(x2, y2);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2i(x3, y3);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex2i(x4, y4);
  glEnd();
}


void Renderer::drawVRAM(Uint16* vram){
  GLuint t;
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 512, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, vram);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glBindTexture(GL_TEXTURE_2D, t);
  glEnable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(   0,   0);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(1024,   0);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(1024, 512);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(   0, 512);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}
