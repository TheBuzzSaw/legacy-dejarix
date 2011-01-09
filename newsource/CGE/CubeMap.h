#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"
#include "Image.h"

namespace CGE
{
    class CubeMap : public Texture
    {
        public:
            CubeMap();
            virtual ~CubeMap();

            void loadImages(const Image* inImages[]);
            void loadFiles(const char* inFiles[]);
    };
}

#endif
