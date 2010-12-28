#ifndef TOOLS_H
#define TOOLS_H

#include "Graphics.h"

namespace CGE
{
    Surface loadImage(const char* inFile);
    void loadTexture(Surface inSurface, GLuint inTexture);
    void loadCubeMap(Surface inSurface[], GLuint inTexture);
}

#endif
