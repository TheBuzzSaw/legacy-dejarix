#include "CardModel.h"

#include <fstream>
using namespace std;

GLint CardModel::mCardTexture = 0;
GLint CardModel::mUseTexture = 0;
GLint CardModel::mCardColor = 0;

CardModel::CardModel() : mSize(6.3f, 8.8f, 0.05f, 0.25f), mDetail(4)
{
}

CardModel::~CardModel()
{
}

void CardModel::display(GLuint inFront, GLuint inBack)
{
    Vector3D<float> c(0.25f);
    glUniform4fv(mCardColor, 1, c.array());
    glUniform1i(mUseTexture, 1);
    glBindTexture(GL_TEXTURE_2D, inFront);
    //mVertices.displayIndexed(mIndices[0]);
    glBindTexture(GL_TEXTURE_2D, inBack);
    //mVertices.displayIndexed(mIndices[2]);

    glUniform1i(mUseTexture, 0);
    mVertices.displayIndexed(mIndices[1]);
}

void CardModel::setUniforms(GLint inCardTexture, GLint inUseTexture,
    GLint inCardColor)
{
    mCardTexture = inCardTexture;
    mUseTexture = inUseTexture;
    mCardColor = inCardColor;
    glUniform1i(mCardTexture, 0);
}

void CardModel::build(const ShaderProgram& inProgram)
{
    float w = mSize[0] / 2.0f;
    float h = mSize[1] / 2.0f;
    float d = mSize[2] / 2.0f;

    GLfloat* vertices = NULL;
    GLfloat* textureCoordinates = NULL;
    GLuint* topIndices = NULL;
    GLuint* middleIndices = NULL;
    GLuint* bottomIndices = NULL;

    float ratio = mSize[0] / mSize[1];
    int current = 0;
    int currentTC = 0;

    int numVertices = 8 * mDetail + 16;
    vertices = new GLfloat[numVertices * 3];
    textureCoordinates = new GLfloat[numVertices * 2];

#define addVertex(x, y) vertices[current++] = (x); \
vertices[current++] = (y); \
vertices[current++] = d; \
vertices[current++] = (x); \
vertices[current++] = (y); \
vertices[current++] = -d

#define addTC(x, y) textureCoordinates[currentTC++] = (x); \
textureCoordinates[currentTC++] = (y); \
textureCoordinates[currentTC++] = 1.0f - (x); \
textureCoordinates[currentTC++] = (y)

    // upper left corner
    int previousCorner = 0;
    int previousCornerTC = 0;
    addVertex(mSize[3] - w, h - mSize[3]);
    addTC(mSize[3] / mSize[0], mSize[3] / mSize[1]);
    addVertex(-w, h - mSize[3]);
    addTC(0.0f, mSize[3] / mSize[1]);

    float theta = 90.0f / float(mDetail);
    for (int i = 1; i < mDetail; ++i)
    {
        float dx = mSize[3] * cos(TO_RADIANS(float(i) * theta));
        float dy = mSize[3] * sin(TO_RADIANS(float(i) * theta));
        addVertex(mSize[3] - w - dx, h - mSize[3] + dy);
        addTC((mSize[3] - dx) / mSize[0], (mSize[3] - dy) / mSize[1]);
    }

    addVertex(mSize[3] - w, h);
    addTC(mSize[3] / mSize[0], 0);

    // upper right corner
    addVertex(-vertices[previousCorner], vertices[previousCorner + 1]);
    addTC(1.0f - textureCoordinates[previousCornerTC],
        textureCoordinates[previousCornerTC + 1]);
    previousCorner = current - 6;
    previousCornerTC = currentTC - 4;
    int mirror = current - 12;
    int mirrorTC = currentTC - 8;
    for (int i = 0; i < mDetail + 1; ++i)
    {
        addVertex(-vertices[mirror], vertices[mirror + 1]);
        addTC(1.0f - textureCoordinates[mirrorTC],
            textureCoordinates[mirrorTC + 1]);
        mirror -= 6;
        mirrorTC -= 4;
    }

    // lower right corner
    addVertex(vertices[previousCorner], -vertices[previousCorner + 1]);
    addTC(textureCoordinates[previousCornerTC],
        1.0f - textureCoordinates[previousCornerTC + 1]);
    previousCorner = current - 6;
    previousCornerTC = currentTC - 4;
    mirror = current - 12;
    mirrorTC = currentTC - 8;
    for (int i = 0; i < mDetail + 1; ++i)
    {
        addVertex(vertices[mirror], -vertices[mirror + 1]);
        addTC(textureCoordinates[mirrorTC],
            1.0f - textureCoordinates[mirrorTC + 1]);
        mirror -= 6;
        mirrorTC -= 4;
    }

    // lower left corner
    addVertex(-vertices[previousCorner], vertices[previousCorner + 1]);
    addTC(1.0f - textureCoordinates[previousCornerTC],
        textureCoordinates[previousCornerTC + 1]);
    previousCorner = current - 6;
    previousCornerTC = currentTC - 4;
    mirror = current - 12;
    mirrorTC = currentTC - 8;
    for (int i = 0; i < mDetail + 1; ++i)
    {
        addVertex(-vertices[mirror], vertices[mirror + 1]);
        addTC(1.0f - textureCoordinates[mirrorTC],
            textureCoordinates[mirrorTC + 1]);
        mirror -= 6;
        mirrorTC -= 4;
    }

    float HTA = 490.0f / 512.0f; // Holotable Adjustment
    for (int i = 0; i < numVertices; ++i)
    {
        textureCoordinates[i * 2] *= ratio;

        textureCoordinates[i * 2] *= HTA;
        textureCoordinates[i * 2 + 1] *= HTA;
    }

    mVertices.loadVAA(inProgram.getBinding("CardVertex"), 3, numVertices,
        vertices);
    mVertices.loadVAA(inProgram.getBinding("CardTextureCoordinate"), 2,
        numVertices, textureCoordinates);

    ofstream fout("card.txt");
    for (int i = 0; i < numVertices; ++i)
    {
        fout << "v " << vertices[i * 3] << ' ' << vertices[i * 3 + 1] << ' '
            << vertices[i * 3 + 2];
        fout << " tc " << textureCoordinates[i * 2] << ' '
            << textureCoordinates[i * 2 + 1] << endl;
    }

    delete [] vertices;
    delete [] textureCoordinates;

    int numTriangles = 16 * mDetail + 28;
    int numMiddleTriangles = 8 * mDetail + 8;
    int numTopTriangles = (numTriangles - numMiddleTriangles) / 2;

    topIndices = new GLuint[numTopTriangles * 3];
    middleIndices = new GLuint[numMiddleTriangles * 3];
    bottomIndices = new GLuint[numTopTriangles * 3];

    current = 0;
    int topCurrent = 0;
    int middleCurrent = 0;
    int bottomCurrent = 0;
    GLuint corners[5];
    int cornerSize = 2 * mDetail + 4;
    for (int i = 0; i < 5; ++i) corners[i] = cornerSize * i;

#define addTriangle(a, b, c) middleIndices[middleCurrent++] = (a); \
middleIndices[middleCurrent++] = (b); \
middleIndices[middleCurrent++] = (c)

#define addTriangles(a, b, c) topIndices[topCurrent++] = (a); \
topIndices[topCurrent++] = (b); \
topIndices[topCurrent++] = (c); \
bottomIndices[bottomCurrent++] = (c) + 1; \
bottomIndices[bottomCurrent++] = (b) + 1; \
bottomIndices[bottomCurrent++] = (a) + 1

#define addQuad(a, b, c, d) addTriangle(a, b, c); addTriangle(a, c, d)
#define addQuads(a, b, c, d) addTriangles(a, b, c); addTriangles(a, c, d)

    // iterate through all for corners
    for (int i = 0; i < 4; ++i)
    {
        addQuads(corners[i], corners[i + 1] - 2, corners[(i + 1) % 4] + 2,
            corners[(i + 1) % 4]);
        addQuad(corners[i + 1] - 2, corners[i + 1] - 1, corners[(i + 1) % 4] + 3,
            corners[(i + 1) % 4] + 2);

        for (int j = 0; j < mDetail; ++j)
        {
            int k = 2 * (j + 1);
            addTriangles(corners[i], corners[i] + k, corners[i] + k + 2);
            addQuad(corners[i] + k, corners[i] + k + 1, corners[i] + k + 3,
                corners[i] + k + 2);
        }
    }

    addQuads(corners[0], corners[1], corners[2], corners[3]);

    mIndices[0].loadData(numTopTriangles * 3, topIndices);
    mIndices[1].loadData(numMiddleTriangles * 3, middleIndices);
    mIndices[2].loadData(numTopTriangles * 3, bottomIndices);

    fout << "\ntop indices\n";
    for (int i = 0; i < numTopTriangles; ++i)
    {
        fout << topIndices[i * 3] << ' ' << topIndices[i * 3 + 1]
            << ' ' << topIndices[i * 3 + 2] << endl;
    }

    fout << "\nmiddle indices\n";
    for (int i = 0; i < numMiddleTriangles; ++i)
    {
        fout << middleIndices[i * 3] << ' ' << middleIndices[i * 3 + 1]
            << ' ' << middleIndices[i * 3 + 2] << endl;
    }

    fout << "\nbottom indices\n";
    for (int i = 0; i < numTopTriangles; ++i)
    {
        fout << bottomIndices[i * 3] << ' ' << bottomIndices[i * 3 + 1]
            << ' ' << bottomIndices[i * 3 + 2] << endl;
    }
    fout.close();

    delete [] topIndices;
    delete [] middleIndices;
    delete [] bottomIndices;
}
