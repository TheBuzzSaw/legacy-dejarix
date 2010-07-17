#ifndef CARDMODEL_H
#define CARDMODEL_H

#include "Vector3D.h"
#include "ShaderVBO.h"
#include "IndexVBO.h"
#include "ShaderProgram.h"

class CardModel
{
    public:
        CardModel();
        ~CardModel();

        void build(const ShaderProgram& inProgram);
        void display(GLuint inFront, GLuint inBack);

        static void setUniforms(GLint inCardTexture, GLint inUseTexture,
            GLint inCardColor);

    private:
        Vector3D<float> mSize;
        unsigned short mDetail;
        ShaderVBO mVertices;
        IndexVBO mIndices[3];

        static GLint mCardTexture;
        static GLint mUseTexture;
        static GLint mCardColor;
};

#endif
