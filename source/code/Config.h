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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string>
#include <sstream>
#include <map>

#define UNIX_HOME_FOLDER getenv("HOME")

#define DEFAULT_VALUE ""

class Config
{
    public:
        static void initialize(int inArgc, char** inArgv);
        static void finalize();
        static void loadFromFile(const char* inFile);
        static void outputSettings();
        static void outputSettings(std::ostream& inStream);
        static const char* getRaw(const char* inKey, const char* inDefault);
        static const char* getRaw(const char* inKey);
        static void set(const char* inKey, const char* inValue);

        static inline const std::string& getUserFolder()
        {
            return mUserFolder;
        }

        template<class T>
        static T get(const char* inKey, T inDefault)
        {
            T outValue;

            std::stringstream ss;
            ss << inDefault;
            std::string s(ss.str());

            ss.str(std::string());
            ss.clear();

            ss << getRaw(inKey, s.c_str());
            if(ss.str().length() > 0) ss >> outValue;
            return outValue;
        }

    private:
        static std::map<std::string, std::string> mSettings;
        static std::string mUserFolder;
        static bool mCreateSettingsFile;
};

void trim(std::string& inString);

#endif

