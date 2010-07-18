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

#include "Config.h"

#include <boost/filesystem.hpp>

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

map<string, string> Config::mSettings;
bool Config::mCreateSettingsFile = false;

#ifndef __WIN32__
string Config::mUserFolder(UNIX_HOME_FOLDER);
#else
string Config::mUserFolder("data/");
#endif

void trim(string& inString)
{
    size_t startPos = inString.find_first_not_of(" \t");
    size_t endPos = inString.find_last_not_of(" \t");

    if (startPos == string::npos || endPos == string::npos)
        inString = "";
    else
        inString = inString.substr(startPos, endPos - startPos + 1);
}

void Config::initialize(int inArgc, char** inArgv)
{
#ifndef __WIN32__
    {
        // UNIX home folder settings file
        using namespace boost::filesystem;
        string s(UNIX_HOME_FOLDER);
        s += "/.dejarix";
        create_directory(s.c_str());
        s += "/logs";
        create_directory(s.c_str());
        mUserFolder += "/.dejarix/";
    }
#endif

    string settingsFile(mUserFolder);
    settingsFile += "settings.txt";
    loadFromFile(inArgc > 1 ? inArgv[1] : settingsFile.c_str());
}

void Config::loadFromFile(const char* inFile)
{
    ifstream settingsFile;
    settingsFile.open(inFile);
    if (settingsFile.fail())
    {
        cerr << "failed to open " << inFile << endl;
        mCreateSettingsFile = true;
        return;
    }

    string s;

    while (getline(settingsFile, s))
    {
        size_t eq = s.find_first_of('=');
        if (s.length() < 2 || s[0] == '`' || eq == string::npos) continue;

        string key = s.substr(0, eq);
        string value = s.substr(eq + 1);
        trim(key);
        trim(value);

        if (key.length() < 2 || value.length() < 1) continue;

        mSettings[key] = value;
    }

    settingsFile.close();
}

void Config::outputSettings()
{
    outputSettings(cout);
}

void Config::outputSettings(ostream& inStream)
{
    for (map<string, string>::iterator i = mSettings.begin();
        i != mSettings.end(); ++i)
    {
        inStream << i->first << " = " << i->second << endl;
    }
}

const char* Config::getRaw(const char* inKey, const char* inDefault)
{
    map<string, string>::iterator i = mSettings.find(inKey);

    if (i == mSettings.end())
    {
        mSettings[inKey] = inDefault;
        return inDefault;
    }

    return i->second.c_str();
}

const char* Config::getRaw(const char* inKey)
{
    return getRaw(inKey, DEFAULT_VALUE);
}

void Config::set(const char* inKey, const char* inValue)
{
    mSettings[inKey] = inValue;
}

void Config::finalize()
{
    if (mCreateSettingsFile)
    {
        string s(getUserFolder());
        s += "settings.txt";
        ofstream settings(s.c_str());
        if (settings.fail()) return;
        outputSettings(settings);
        settings.close();
    }
}
