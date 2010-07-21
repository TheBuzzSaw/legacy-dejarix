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

#ifndef TABLEMODULE_H
#define TABLEMODULE_H

#include "Module.h"
#include "CardProgram.h"
#include "CardModel.h"
#include "CardInstance.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "Vector2D.h"

#include <list>

class TableModule : public Module
{
    public:
        TableModule();
        virtual ~TableModule();

        enum MouseModes { NONE, DRAGGING };

        /// module operation
        virtual void onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
        void processPosition();

        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);

        static bool unproject(float inX, float inY, float inZ,
            const Matrix3D& inMVM, const Matrix3D& inPM,
            const Vector3D<int>& inViewport, Vector3D<float>& inResult);
        static void transformPoint(const Matrix3D& inMatrix,
            const Vector3D<float>& inVector, Vector3D<float>& inResult);

    private:
        void loadCardImage(const char* inFile, GLuint inTexture);
        GLuint generateTexture();

        GLuint mTextures[200];
        size_t mLoadedTextures;

        Camera mCamera;
        Vector3D<GLint> mViewport;
        MatrixStack mModelView;
        Matrix3D mProjection;
        Matrix3D mProjectionHUD;
        Matrix3D mMVPM;

        CardProgram mCardProgram;
        ShaderVBO mTableVBO;
        GLuint mTableTexture;
        CardModel mCardModel;
        std::list<CardInstance*> mCards;
        CardInstance* mCardSelect;

        int mFixedY;
        Pixel mMouseCoordinates;
        Pixel mWindowCenter;
        Point mSmartPanning;
        Vector3D<GLfloat> mPointer;
        Vector3D<GLfloat> mDragAnchor;
        Vector3D<GLfloat> mCardAnchor;
        MouseModes mMouseMode;
        GLfloat mDepthZ;

        Uint8* mKeyState;
};

#endif
