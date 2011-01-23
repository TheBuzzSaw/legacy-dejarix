#include "TestModule.h"

#include <CGE/Vectors.h>

#define FOV 30.0f
#define NCC 1.0f
#define FCC 100.0f

GLfloat points[24] = {
    +1.0f, +1.0f, +1.0f,
    +1.0f, -1.0f, +1.0f,
    -1.0f, -1.0f, +1.0f,
    -1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, -1.0f,
    +1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, +1.0f, -1.0f
};

GLfloat colors[32] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLuint indices[36] = {
    0, 1, 2, 0, 2, 3, // top
    7, 6, 5, 7, 5, 4, // bottom
    1, 0, 4, 1, 4, 5, // right
    3, 2, 6, 3, 6, 7, // left
    2, 1, 5, 2, 5, 6, // front
    0, 3, 7, 0, 7, 4  // back
};

const GLfloat a = 1.0f / sqrt(3.0f);

GLfloat normals[24] = {
    +a, +a, +a,
    +a, -a, +a,
    -a, -a, +a,
    -a, +a, +a,
    +a, +a, -a,
    +a, -a, -a,
    -a, -a, -a,
    -a, +a, -a,
};

TestModule::TestModule() : mRotate(0.0f)
{
    {
        CGE::Image i("data/images/wood.jpg");
        const CGE::Image* ip[] = { &i, &i, &i, &i, &i, &i };
        mCubeMap.loadImages(ip);
    }

    mVS.loadFromFile("data/shaders/test.vs");
    mFS.loadFromFile("data/shaders/test.fs");
    mProgram.attachShader(mVS);
    mProgram.attachShader(mFS);
    mProgram.bindAttribLocation(0, "in_Position");
    mProgram.bindAttribLocation(1, "in_Color");
    mProgram.bindAttribLocation(2, "in_Normal");
    mProgram.link();
    mUniMVPM = mProgram.getUniformLocation("MVPM");
    mUniMVM = mProgram.getUniformLocation("MVM");
    mUniNM = mProgram.getUniformLocation("NM");

    GLint u;

    u = mProgram.getUniformLocation("cubeMap");
    glUniform1i(u, 0);

    vec4f v = {0.1f, 0.1f, 0.1f, 1.0f};
    u = mProgram.getUniformLocation("ambientColor");
    glUniform4fv(u, 1, v);

    u = mProgram.getUniformLocation("diffuseColor");
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    glUniform4fv(u, 1, v);

    u = mProgram.getUniformLocation("specularColor");
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    v[3] = 0.0f;
    glUniform4fv(u, 1, v);

    u = mProgram.getUniformLocation("lightPosition");
    v[0] = 5.0f;
    v[2] = 100.0f;
    glUniform3fv(u, 1, v);

    mVBO.loadVAA(0, 3, 8, points);
    mVBO.loadVAA(1, 4, 8, colors);
    mVBO.loadVAA(2, 3, 8, normals);
    mIVBO.loadData(GL_TRIANGLES, 36, indices);
}

TestModule::~TestModule()
{
}

void TestModule::onOpen()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE0);
    mCubeMap.bind();

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
}

void TestModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void TestModule::onFrame()
{
    mRotate += 1.0f;
    if (mRotate > 180.0f) mRotate -= 360.0f;
}

void TestModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.loadIdentity();
    mModelView.translate(0.0f, 0.0f, -10.0f);
    mModelView.rotateX(mRotate);
    mModelView.rotateZ(mRotate);

    mNormalView.loadIdentity();
    mNormalView.rotateX(mRotate);
    mNormalView.rotateZ(mRotate);

    mat4f mvp(mProjection, mModelView);
    glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, mvp);
    glUniformMatrix4fv(mUniMVM, 1, GL_FALSE, mModelView);
    glUniformMatrix4fv(mUniNM, 1, GL_FALSE, mNormalView);
    mVBO.display(mIVBO);
}

void TestModule::onResize(int inWidth, int inHeight)
{
    float ratio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(FOV, ratio, NCC, FCC, true);
    glViewport(0, 0, inWidth, inHeight);
}
