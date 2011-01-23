#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <CGE/Module.h>
#include <CGE/Program.h>
#include <CGE/ClusterVBO.h>
#include <CGE/Matrix4x4.h>
#include <CGE/CubeMap.h>
#include <CGE/VertexShader.h>
#include <CGE/FragmentShader.h>

class TestModule : public CGE::Module
{
    public:
        TestModule();
        virtual ~TestModule();

        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();

        virtual void onResize(int inWidth, int inHeight);

    private:
        CGE::Program<2> mProgram;
        CGE::VertexShader mVS;
        CGE::FragmentShader mFS;
        CGE::ClusterVBO<3> mVBO;
        CGE::IndexVBO mIVBO;

        CGE::CubeMap mCubeMap;

        GLint mUniMVPM;
        GLint mUniMVM;
        GLint mUniNM;
        mat4f mProjection;
        mat4f mModelView;
        mat4f mNormalView;
        float mRotate;
};

#endif
