#ifndef CARDMODEL_H
#define CARDMODEL_H

#include "Vector3D.h"
#include "ShaderVBO.h"
#include "IndexVBO.h"
#include "CardProgram.h"

class CardModel
{
    public:
        CardModel(CardProgram& inProgram);
        ~CardModel();

        void build();
        void display(GLuint inFront);
        void display(GLuint inFront, GLuint inBack);

    private:
        Vector3D<float> mSize;
        unsigned short mDetail;
        ShaderVBO mVertices;
        IndexVBO mIndices[3];
        CardProgram* mCardProgram;
};

#endif
