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

class TestModule : public Module
{
    public:
        TestModule();
        virtual ~TestModule();

        /// module operation
        virtual void onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:

    private:
        void loadCardImage(const char* inFile, GLuint inTexture);

        MatrixStack mModelView;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        ShaderProgram mCardProgram;
        CardModel mCard;
        GLuint mTextures[2];
        float mRotation;
};

#endif
