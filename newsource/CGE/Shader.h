#ifndef SHADER_H
#define SHADER_H

#include "Graphics.h"

namespace CGE
{
    class Shader
    {
        public:
            Shader();
            Shader(const char* inFile, GLenum inType);
            ~Shader();

            inline GLuint handle() const { return mHandle; }
            void loadFromFile(const char* inFile, GLenum inType);
            void loadFromBuffer(const char* inBuffer, GLenum inType);
            void unload();

        private:
            Shader(const Shader& inShader) {} // no copying

            GLuint mHandle;

            static char* fileToBuffer(const char* inFile);
    };
}

#endif
