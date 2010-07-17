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

#ifndef SHADER_H
#define SHADER_H

#include "OGL.h"

#include <string>
#include <map>

class Shader
{
    public:
        virtual ~Shader();

        class Exception
        {
            public:
                Exception(const std::string& inReason) : reason(inReason) {}
                const std::string reason;
        };

        inline GLuint handle() { return mHandle; }

        static Shader* load(const char* inFile);
        static void unloadAll();

    private:
        Shader(const char* inFile);

        static char* fileToBuffer(const char* inFile);
        static std::map<std::string, Shader*> mShaders;

        GLuint mHandle;
};

#endif
