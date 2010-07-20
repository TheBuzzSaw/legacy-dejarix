#ifndef CARDPROGRAM_H
#define CARDPROGRAM_H

#include "ShaderProgram.h"
#include "Vector3D.h"

class CardProgram : public ShaderProgram
{
    public:
        CardProgram();
        virtual ~CardProgram();

        enum Bindings
        {
            VERTEX = 0,
            TEXTURE = 1
        };

        inline void setColor(const Vector3D<float>& inVector)
        {
            glUniform4fv(mUniformCardColor, 1, inVector.array());
        }

        inline void useTexture(bool inUse)
        {
            glUniform1i(mUniformUseTexture, inUse);
        }

    protected:
        virtual void bindUniforms();

    private:
        GLint mUniformUseTexture;
        GLint mUniformCardColor;
        GLint mUniformCardTexture;
};

#endif
