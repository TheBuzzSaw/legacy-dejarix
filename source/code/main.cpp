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
#include "DisplayEngine.h"
#include "SoundEngine.h"
#include "TestModule.h"
#include "TableModule.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    Config::initialize(argc, argv);
    DisplayEngine::initialize();
    //SoundEngine::initialize();
    //Config::outputSettings();
    Module* m;

    try
    {
        m = new TableModule;
    }
    catch (const Shader::Exception& se)
    {
        cerr << "shader exception -- " << se.reason << endl;
        m = NULL;
    }
    catch (...)
    {
        cerr << "unknown exception" << endl;
        m = NULL;
    }

    if (m) DisplayEngine::start(m);
    //SoundEngine::cleanup();
    Config::finalize();
    return 0;
}
