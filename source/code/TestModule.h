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
#include "Vector2D.h"

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
        static bool unproject(float inX, float inY, float inZ,
            const Matrix3D& inMVM, const Matrix3D& inPM,
            const Vector3D<int>& inViewport, Vector3D<float>& inResult);
        static void transformPoint(const Matrix3D& inMatrix,
            const Vector3D<float>& inVector, Vector3D<float>& inResult);

        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);

    private:
        void loadCardImage(const char* inFile, GLuint inTexture);
        void processPosition();

        Camera mCamera;
        MatrixStack mModelView;
        Matrix3D mProjection;
        GLdouble mProjectionArray[16];
        Matrix3D mMVPM;
        ShaderProgram mCardProgram;
        GLint mUniformUseTexture;
        GLint mUniformCardColor;
        ShaderVBO mTable;
        CardModel mCard;
        GLuint mTextures[3];
        Pixel mMouseCoordinates;
        Vector3D<GLfloat> mPointer;
        Vector3D<GLfloat> mDragAnchor;
        Vector3D<GLfloat> mCardDragSource;
        Vector3D<GLfloat> mCardTranslate;
        Vector3D<GLint> mViewport;
        MouseModes mMouseMode;
        bool mSpin;
};

#endif
