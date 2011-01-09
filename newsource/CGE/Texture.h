#ifndef TEXTURE_H
#define TEXTURE_H

#include "Graphics.h"

namespace CGE
{
    class Texture
    {
        public:
            Texture(GLenum inTarget);
            virtual ~Texture();

            inline void bind() const { glBindTexture(mTarget, mHandle); }

        protected:
            void processParams(const GLenum inParams[]);
            GLtexture mHandle;
            GLenum mTarget;

    };
}

#endif
