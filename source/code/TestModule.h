/**
 *  This file is part of Dejarix.
 *
 *  Dejarix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Dejarix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dejarix.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "Module.h"
#include "ShaderProgram.h"
#include "CardModel.h"
#include "MatrixStack.h"
#include "Camera.h"

class TestModule : public Module
{
    public:
        TestModule();
        virtual ~TestModule();

        enum MouseModes { NONE, DRAGGING };

        /// module operation
        virtual void onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);

    private:
        void loadCardImage(const char* inFile, GLuint inTexture);

        Camera mCamera;
        MatrixStack mModelView;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        ShaderProgram mCardProgram;
        GLint mUniformUseTexture;
        GLint mUniformCardColor;
        ShaderVBO mTable;
        CardModel mCard;
        GLuint mTextures[3];
        Vector3D<GLdouble> mPointer;
        Vector3D<GLdouble> mDragAnchor;
        Vector3D<GLfloat> mCardDragSource;
        Vector3D<GLfloat> mCardTranslate;
        Vector3D<GLint> mViewport;
        Matrix3D mMV;
        MouseModes mMouseMode;
};

#endif
