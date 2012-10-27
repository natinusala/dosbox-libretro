#include <string.h>

#include "libretro.h"

#include "dosbox.h"
#include "video.h"

// GFX
Bit8u RDOSGFXbuffer[1024*768*4];
Bitu RDOSGFXwidth, RDOSGFXheight, RDOSGFXpitch;
unsigned RDOSGFXcolorMode = RETRO_PIXEL_FORMAT_0RGB1555;
bool RDOSGFXhaveFrame;
static Bitu (*getRGB)(Bit8u red, Bit8u green, Bit8u blue);

// Color Getters
template<int R, int G, int B, int RLOSS, int GLOSS, int BLOSS>
static Bitu getRGB_t(Bit8u red, Bit8u green, Bit8u blue)
{
    return ((red >> RLOSS) << R) | ((green >> GLOSS) << G) | ((blue >> BLOSS) << B);
}

// GFX
Bitu GFX_GetBestMode(Bitu flags)
{
    switch(RDOSGFXcolorMode)
    {
        case RETRO_PIXEL_FORMAT_0RGB1555: getRGB = &getRGB_t<10, 5, 0, 3, 3, 3>; return GFX_CAN_15 | GFX_RGBONLY;
        case RETRO_PIXEL_FORMAT_RGB565:   getRGB = &getRGB_t<11, 6, 0, 3, 2, 3>; return GFX_CAN_16 | GFX_RGBONLY;
        case RETRO_PIXEL_FORMAT_XRGB8888: getRGB = &getRGB_t<16, 8, 0, 0, 0, 0>; return GFX_CAN_32 | GFX_RGBONLY;
    }
    
    return 0;
}

Bitu GFX_GetRGB(Bit8u red,Bit8u green,Bit8u blue)
{
    return getRGB(red, green, blue);
}

Bitu GFX_SetSize(Bitu width,Bitu height,Bitu flags,double scalex,double scaley,GFX_CallBack_t cb)
{
    memset(RDOSGFXbuffer, 0, sizeof(RDOSGFXbuffer));
    
    RDOSGFXwidth = width;
    RDOSGFXheight = height;
    RDOSGFXpitch = width * ((RETRO_PIXEL_FORMAT_XRGB8888 == RDOSGFXcolorMode) ? 4 : 2);
    
    if(RDOSGFXwidth > 1024 || RDOSGFXheight > 768)
    {
        return 0;
    }
    
    return GFX_GetBestMode(0);
}

bool GFX_StartUpdate(Bit8u * & pixels,Bitu & pitch)
{
    pixels = RDOSGFXbuffer;
    pitch = RDOSGFXpitch;
    
    return true;
}

void GFX_EndUpdate( const Bit16u *changedLines )
{
    RDOSGFXhaveFrame = true;
}

// Stubs
void GFX_SetTitle(Bit32s cycles,Bits frameskip,bool paused){}
void GFX_ShowMsg(char const* format,...){}
void GFX_Events(){}
void GFX_SetPalette(Bitu start,Bitu count,GFX_PalEntry * entries){}

