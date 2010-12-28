#ifndef CARDOBJECT_H
#define CARDOBJECT_H

#include <CGE/Graphics.h>
#include <CGE/Matrix4x4.h>

class CardObject
{
    public:
        static inline void setMatrixUniform(GLuniform inUniform)
        {
            mMatrixUniform = inUniform;
        }

        CardObject();
        virtual ~CardObject();

        virtual void display() = 0;
        virtual void update(const mat4f& inMatrix);

    protected:
        void sendMatrix();

        mat4f mMatrix;
        float mRotate; // Z-axis rotation
        float mPosition[3];

    private:
        static GLuniform mMatrixUniform;
};

#endif
