#include "gpu.h"

GPU::GPU(){
  memset(&transfer, 0, sizeof(transfer));
  memset(&vram.b, 0, sizeof(vram.b));
}

GPU::~GPU(){
}


void GPU::getComponents(Renderer* rendererPtr){
  renderer = rendererPtr;
}


void GPU::reset(){
    cmdLen = 0;
}


void GPU::gp0Write(Uint32 data){
    //printf("[DMA]\t0x%08x\n", data);
    if(cmdLen > 0){
      cmdBuffer[bufferLen] = data;
      bufferLen++;
      if(cmdLen == bufferLen){
          switch(cmdBuffer[0] >> 24){
          case 0x28: drawMonochromeQuad(); break;
          case 0x2c: drawTexturedQuad();   break;
          case 0x30: drawShadedTri();      break;
          case 0x38: drawShadedQuad();     break;
          case 0xa0: transferToVRAM();     break;
          case 0xc0: VRAMStore();       break;
          default: printf("[GPU]\terror - undefined GP0 draw opcode 0x%02x\n", cmdBuffer[0] >> 24);
          }
          cmdLen = 0;
      }
    }
    else if(transfer.remaining > 0){
        transfer.buffer[transfer.current] = data;
        transfer.current++;
        transfer.remaining--;
        if(transfer.remaining <= 0){
          Uint32 i = 0;
          for(int y = transfer.y; y < transfer.y+transfer.h; y++){
            for(int x = transfer.x/2; x < (transfer.x/2)+(transfer.w/2); x++){
              vram.w[y][x] = transfer.buffer[i];
              i++;
            }
          }
          delete transfer.buffer;
          renderer->drawVRAM(vram.a);
        }
    }
    else{
      switch(data >> 24){
      case 0x00: break;
      case 0x01: break;
      case 0x28: setLong(data, 5);       break;
      case 0x2c: setLong(data, 9);       break;
      case 0x30: setLong(data, 6);       break;
      case 0x38: setLong(data, 8);       break;
      case 0xa0: setLong(data, 3);       break;
      case 0xc0: setLong(data, 3);       break;
      case 0xe1: gp0DrawMode(data);      break;
      case 0xe2: gp0TextureWindow(data); break;
      case 0xe3: gp0SetTopLeft(data);    break;
      case 0xe4: gp0SetBotRight(data);   break;
      case 0xe5: gp0SetDrawOffset(data); break;
      case 0xe6: gp0MaskBit(data);       break;
      default: printf("[GPU]\terror - undefined GP0 command 0x%02x\n", data >> 24);
      }
    }
}


void GPU::gp1Write(Uint32 data){
  switch(data >> 24){
    case 0x00: gp1Reset();             break;
    case 0x01:                         break;
    case 0x02:                         break;
    case 0x03: gp1DisplayEnable(data); break;
    case 0x04: gp1DMADirection(data);  break;
    case 0x05: gp1VRAMStart(data);     break;
    case 0x06: gp1DisplayHRange(data); break;
    case 0x07: gp1DisplayVRange(data); break;
    case 0x08: gp1DisplayMode(data);   break;
    default: printf("[GPU]\terror - undefined GP1 command 0x%02x\n", data >> 24);
  }
}


void GPU::setLong(Uint32 data, Uint8 words){
    cmdLen = words;
    bufferLen = 1;
    cmdBuffer[0] = data;
}


void GPU::gp0DrawMode(Uint32 data){
  stat.pageX = data & 0xf;
  stat.pageY = (data>>4) & 1;
  stat.transparency = (data>>5) & 3;
  stat.colors = (data>>7) & 3;
  stat.dither = (data >> 9) & 1;
  stat.drawToDisplay = (data >> 10) & 1;
  stat.texDisable = (data >> 11) & 1;
  state.texFlipX = (data>>12) & 1;
  state.texFlipY = (data>>13) & 1;
 }


void GPU::gp0SetTopLeft(Uint32 data){
   state.clipX1 = data & 0x3ff;
   state.clipY1 = (data>>10) & 0x3ff;
}


void GPU::gp0SetBotRight(Uint32 data){
   state.clipX2 = data & 0x3ff;
   state.clipY2 = (data>>10) & 0x3ff;
}


void GPU::gp0SetDrawOffset(Uint32 data){
  Uint16 x = data & 0x7ff;
  Uint16 y = (data >> 11) & 0x7ff;
  state.drawX = ((Sint16)(x << 5)) >> 5;
  state.drawY = ((Sint16)(y << 5)) >> 5;
}


void GPU::gp0TextureWindow(Uint32 data){
  state.texWinXMask = data & 0x1f;
  state.texWinYMask = (data >> 5) & 0x1f;
  state.texWinX = (data >> 10) & 0x1f;
  state.texWinY = (data >> 15) & 0x1f;
}


void GPU::gp0MaskBit(Uint32 data){
  stat.forceWriteMask = data & 1;
  stat.checkMask = (data >> 1) & 1;
}


void GPU::gp1Reset(){
 memset(&stat,  0, sizeof(stat));
 memset(&state, 0, sizeof(state));

 stat.raw = 0x1c000000;

 stat.displayDisable = true;
 stat.interlace = true;
 state.pixelStart = 0x200;
 state.pixelEnd = 0xc00;
 state.lineStart = 0x10;
 state.lineEnd = 0x100;
}


void GPU::gp1DisplayMode(Uint32 data){
 stat.hres = data & 3;
 stat.vres = (data>>2) & 1;
 stat.pal = (data>>3) & 1;
 stat.outDepth = (data>>4) & 1;
 stat.interlace = (data >> 5) & 1;
 stat.hres2 = (data >> 6) & 1;
}


void GPU::gp1DMADirection(Uint32 data){
 stat.DMADir = data & 3;
}


void GPU::gp1VRAMStart(Uint32 data){
  state.displayX = data & 0x3fe;
  state.displayY = (data >> 10) & 0x1ff;
}


void GPU::gp1DisplayHRange(Uint32 data){
  state.pixelStart = data & 0xfff;
  state.pixelEnd = (data>>12) & 0xfff;
}


void GPU::gp1DisplayVRange(Uint32 data){
  state.lineStart = data & 0x3ff;
  state.lineEnd = (data >>10) & 0x3ff;
}


void GPU::gp1DisplayEnable(Uint32 data){
  stat.displayDisable = data & 1;
}


void GPU::drawMonochromeQuad(){
  int x1 = cmdBuffer[1] & 0xffff;
  int y1 = (cmdBuffer[1] >> 16) & 0xffff;
  int x2 = cmdBuffer[2] & 0xffff;
  int y2 = (cmdBuffer[2] >> 16) & 0xffff;
  int x3 = cmdBuffer[4] & 0xffff;
  int y3 = (cmdBuffer[4] >> 16) & 0xffff;
  int x4 = cmdBuffer[3] & 0xffff;
  int y4 = (cmdBuffer[3] >> 16) & 0xffff;

  Uint8 r = (cmdBuffer[0]      ) & 0xff;
  Uint8 g = (cmdBuffer[0] >>  8) & 0xff;
  Uint8 b = (cmdBuffer[0] >> 16) & 0xff;

  renderer->drawMonochromeQuad(x1, y1, x2, y2, x3, y3, x4, y4, r, g, b);
}


void GPU::drawShadedQuad(){
  int x1 =  (cmdBuffer[7] & 0xffff);
  int y1 = ((cmdBuffer[7] >> 16) & 0xffff);
  int x2 =  (cmdBuffer[3] & 0xffff);
  int y2 = ((cmdBuffer[3] >> 16) & 0xffff);
  int x3 =  (cmdBuffer[1] & 0xffff);
  int y3 = ((cmdBuffer[1] >> 16) & 0xffff);
  int x4 =  (cmdBuffer[5] & 0xffff);
  int y4 = ((cmdBuffer[5] >> 16) & 0xffff);

  Uint8 r1 = (float)((cmdBuffer[6]      ) & 0xff);
  Uint8 g1 = (float)((cmdBuffer[6] >>  8) & 0xff);
  Uint8 b1 = (float)((cmdBuffer[6] >> 16) & 0xff);

  Uint8 r2 = (float)((cmdBuffer[2]      ) & 0xff);
  Uint8 g2 = (float)((cmdBuffer[2] >>  8) & 0xff);
  Uint8 b2 = (float)((cmdBuffer[2] >> 16) & 0xff);

  Uint8 r3 = (float)((cmdBuffer[0]      ) & 0xff);
  Uint8 g3 = (float)((cmdBuffer[0] >>  8) & 0xff);
  Uint8 b3 = (float)((cmdBuffer[0] >> 16) & 0xff);

  Uint8 r4 = (float)((cmdBuffer[4]      ) & 0xff);
  Uint8 g4 = (float)((cmdBuffer[4] >>  8) & 0xff);
  Uint8 b4 = (float)((cmdBuffer[4] >> 16) & 0xff);

  renderer->drawShadedQuad(x1, y1, x2, y2, x3, y3, x4, y4, r1, b1, g1, r2, g2, b2, r3, g3, b3, r4, g4, b4);
}


void GPU::drawShadedTri(){
  int x1 =  (cmdBuffer[1] & 0xffff);
  int y1 = ((cmdBuffer[1] >> 16) & 0xffff);
  int x2 =  (cmdBuffer[3] & 0xffff);
  int y2 = ((cmdBuffer[3] >> 16) & 0xffff);
  int x3 =  (cmdBuffer[5] & 0xffff);
  int y3 = ((cmdBuffer[5] >> 16) & 0xffff);

  Uint8 r1 = ((cmdBuffer[0]      ) & 0xff);
  Uint8 g1 = ((cmdBuffer[0] >>  8) & 0xff);
  Uint8 b1 = ((cmdBuffer[0] >> 16) & 0xff);

  Uint8 r2 = ((cmdBuffer[2]      ) & 0xff);
  Uint8 g2 = ((cmdBuffer[2] >>  8) & 0xff);
  Uint8 b2 = ((cmdBuffer[2] >> 16) & 0xff);

  Uint8 r3 = ((cmdBuffer[4]      ) & 0xff);
  Uint8 g3 = ((cmdBuffer[4] >>  8) & 0xff);
  Uint8 b3 = ((cmdBuffer[4] >> 16) & 0xff);

  renderer->drawShadedTri(x1, y1, x2, y2, x3, y3, r1, g1, b1, r2, g2, b2, r3, g3, b3);
}


void GPU::drawTexturedQuad(){
  int x1 =  (cmdBuffer[7] & 0xffff);
  int y1 = ((cmdBuffer[7] >> 16) & 0xffff);
  int x2 =  (cmdBuffer[3] & 0xffff);
  int y2 = ((cmdBuffer[3] >> 16) & 0xffff);
  int x3 =  (cmdBuffer[1] & 0xffff);
  int y3 = ((cmdBuffer[1] >> 16) & 0xffff);
  int x4 =  (cmdBuffer[5] & 0xffff);
  int y4 = ((cmdBuffer[5] >> 16) & 0xffff);

  renderer->drawBlendedTexturedQuad(x1, y1, x2, y2, x3, y3, x4, y4);
}


void GPU::transferToVRAM(){
  transfer.x = cmdBuffer[1] & 0xffff;
  transfer.y = cmdBuffer[1] >> 16;
  transfer.w = cmdBuffer[2] & 0xffff;
  transfer.h = cmdBuffer[2] >> 16;

  transfer.size = transfer.w * transfer.h;
  transfer.size = (transfer.size+1) & ~1;

  transfer.buffer = new Uint32[transfer.size];

  transfer.current = 0;
  transfer.remaining = transfer.size / 2;
}


void GPU::VRAMStore(){
  transfer.w = cmdBuffer[2] & 0xffff;
  transfer.h = cmdBuffer[2] >> 16;
}
