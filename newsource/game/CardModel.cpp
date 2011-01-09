#include "CardModel.h"

CardModel::CardModel()
{
    build(6.3f, 8.8f, 0.05f, 0.25f);
}

CardModel::~CardModel()
{
}

void CardModel::display(GLuniform inSwitch, const CGE::Texture2D& inFront,
    const CGE::Texture2D& inBack)
{
    glUniform1i(inSwitch, 0);
    mVBO.display(mMiddleIVBO);

    glUniform1i(inSwitch, 1);
    inFront.bind();
    mVBO.display(mTopIVBO);
    inBack.bind();
    mVBO.display(mBottomIVBO);
}

void CardModel::build(float inWidth, float inHeight, float inDepth,
    float inRadius, int inDetail)
{
    //if (inDetail < 1 || inRadius <= 0.0f)
    {
        buildSimple(inWidth, inHeight, inDepth);
        return;
    }
}

void CardModel::buildSimple(float inWidth, float inHeight, float inDepth)
{
    float w = inWidth  / 2.0f;
    float h = inHeight / 2.0f;
    float d = inDepth  / 2.0f;

    GLfloat points[24] = {
        +w, +h, +d,
        +w, -h, +d,
        -w, -h, +d,
        -w, +h, +d,
        +w, +h, -d,
        +w, -h, -d,
        -w, -h, -d,
        -w, +h, -d
        };

    mVBO.loadVAA(0, 3, 8, points);

    GLfloat textureCoordinates[16] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
        };

    float ratio = inWidth / inHeight;
    float adjustment = 490.0f / 512.0f; // card image size adjustment
    for (size_t i = 0; i < 8; ++i)
    {
        // It is assumed that the card is taller than it is wide.
        textureCoordinates[i * 2] *= ratio * adjustment;
        textureCoordinates[i * 2 + 1] *= adjustment;
    }

    mVBO.loadVAA(1, 2, 8, textureCoordinates);

    GLuint topIndices[6] = { 0, 1, 2, 0, 2, 3 };
    GLuint bottomIndices[6] = { 7, 6, 5, 7, 5, 4 };
    GLuint middleIndices[24] = {
        1, 0, 4, 1, 4, 5, // right
        3, 2, 6, 3, 6, 7, // left
        2, 1, 5, 2, 5, 6, // front
        0, 3, 7, 0, 7, 4  // back
        };

    mTopIVBO.loadData(GL_TRIANGLES, 6, topIndices);
    mBottomIVBO.loadData(GL_TRIANGLES, 6, bottomIndices);
    mMiddleIVBO.loadData(GL_TRIANGLES, 24, middleIndices);
}
