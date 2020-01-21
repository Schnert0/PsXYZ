#include "gpu.h"

c_gpu::c_gpu(){
}

c_gpu::~c_gpu(){
}

void c_gpu::reset(){
    cmdLen = 0;
}

void c_gpu::gp0Write(Uint32 data){
    if(cmdLen == 0){
        switch(data >> 24){
        case 0x28: setLong(data, 5); break;
        case 0x2c: setLong(data, 9); break;
        case 0x30: setLong(data, 6); break;
        case 0x38: setLong(data, 8); break;
        //default: printf("GPU: error - undefined GPU opcode 0x%02x\n", data >> 24); getchar();
        }
    }
    else{
        cmdBuffer[bufferLen] = data;
        bufferLen++;
        if(cmdLen == bufferLen){
            switch(cmdBuffer[0] >> 24){
            case 0x28: drawMonochromeQuad(); break;
            case 0x2c: drawTexturedQuad(); break;
            case 0x30: drawShadedTri(); break;
            case 0x38: drawShadedQuad(); break;
            default: printf("GPU: error - undefined GP0 draw opcode 0x%02x\n", cmdBuffer[0] >> 24);
            }
            cmdLen = 0;
        }
    }
}


void c_gpu::setLong(Uint32 data, Uint8 words){
    cmdLen = words;
    bufferLen = 1;
    cmdBuffer[0] = data;
}

void c_gpu::drawMonochromeQuad(){
    glBegin(GL_QUADS);
        glColor3f((float)(cmdBuffer[0] & 0xff) / 255, (float)((cmdBuffer[0] >> 8) & 0xff)/255, (float)((cmdBuffer[0] >> 16) & 0xff) / 255);
        glVertex2i((cmdBuffer[1] & 0xffff), ((cmdBuffer[1] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[2] & 0xffff), ((cmdBuffer[2] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[4] & 0xffff), ((cmdBuffer[4] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[3] & 0xffff), ((cmdBuffer[3] >> 16) & 0xffff));
    glEnd();
}


void c_gpu::drawShadedQuad(){
        /*
        printf("(%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
               (cmdBuffer[7] & 0xffff), ((cmdBuffer[7] >> 16) & 0xffff),
               (cmdBuffer[3] & 0xffff), ((cmdBuffer[3] >> 16) & 0xffff),
               (cmdBuffer[1] & 0xffff), ((cmdBuffer[1] >> 16) & 0xffff),
               (cmdBuffer[5] & 0xffff), ((cmdBuffer[5] >> 16) & 0xffff)
               );
        */
    glBegin(GL_QUADS);
        glColor3f((float)(cmdBuffer[6] & 0xff) / 255, (float)((cmdBuffer[6] >> 8) & 0xff)/255, (float)((cmdBuffer[6] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[7] & 0xffff), ((cmdBuffer[7] >> 16) & 0xffff));
        glColor3f((float)(cmdBuffer[2] & 0xff) / 255, (float)((cmdBuffer[2] >> 8) & 0xff)/255, (float)((cmdBuffer[2] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[3] & 0xffff), ((cmdBuffer[3] >> 16) & 0xffff));
        glColor3f((float)(cmdBuffer[0] & 0xff) / 255, (float)((cmdBuffer[0] >> 8) & 0xff)/255, (float)((cmdBuffer[0] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[1] & 0xffff), ((cmdBuffer[1] >> 16) & 0xffff));
        glColor3f((float)(cmdBuffer[4] & 0xff) / 255, (float)((cmdBuffer[4] >> 8) & 0xff)/255, (float)((cmdBuffer[4] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[5] & 0xffff), ((cmdBuffer[5] >> 16) & 0xffff));
    glEnd();
}


void c_gpu::drawShadedTri(){
    glBegin(GL_TRIANGLES);
        glColor3f((float)(cmdBuffer[0] & 0xff) / 255, (float)((cmdBuffer[0] >> 8) & 0xff)/255, (float)((cmdBuffer[0] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[1] & 0xffff), ((cmdBuffer[1] >> 16) & 0xffff));
        glColor3f((float)(cmdBuffer[2] & 0xff) / 255, (float)((cmdBuffer[2] >> 8) & 0xff)/255, (float)((cmdBuffer[2] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[3] & 0xffff), ((cmdBuffer[3] >> 16) & 0xffff));
        glColor3f((float)(cmdBuffer[4] & 0xff) / 255, (float)((cmdBuffer[4] >> 8) & 0xff)/255, (float)((cmdBuffer[4] >> 16) & 0xff) / 255); glVertex2i((cmdBuffer[5] & 0xffff), ((cmdBuffer[5] >> 16) & 0xffff));
    glEnd();
}

void c_gpu::drawTexturedQuad(){
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.75f, 0.0f);
        glVertex2i((cmdBuffer[7] & 0xffff), ((cmdBuffer[7] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[3] & 0xffff), ((cmdBuffer[3] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[1] & 0xffff), ((cmdBuffer[1] >> 16) & 0xffff));
        glVertex2i((cmdBuffer[5] & 0xffff), ((cmdBuffer[5] >> 16) & 0xffff));
    glEnd();
}
