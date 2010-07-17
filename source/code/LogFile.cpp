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

#include "LogFile.h"
#include "Config.h"

using namespace std;

LogFile::LogFile()
{
}

void LogFile::start(const char* inTitle)
{
    if (mStream.is_open()) return;

    char buffer[15];
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    strftime(buffer, 15, "%Y%m%d%H%M%S", timeinfo);

    string s(Config::getUserFolder());
    s += "logs/";
    s += inTitle;
    s += '-';
    s += buffer;
    s += ".txt";

    mStream.open(s.c_str());
}

LogFile::~LogFile()
{
    if (mStream.is_open()) mStream.close();
}

void LogFile::addLine(const string& inText)
{
    mStream << inText << endl;
}
