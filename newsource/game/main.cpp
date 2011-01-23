#include <rapidxml/rapidxml.hpp>
#include <fstream>
#include <iostream>

#include <CGE/Engine.h>
#include <CGE/Vectors.h>
#include "TableModule.h"

using namespace std;

int main(int argc, char** argv)
{
    /// RapidXML demo
    if (false)
    {
        using namespace rapidxml;

        ifstream fin("data/cards/meta/1.xml");
        if (fin.fail()) return 1;

        fin.seekg(0, ios::end);
        size_t length = fin.tellg();
        fin.seekg(0, ios::beg);
        char* buffer = new char[length + 1];
        fin.read(buffer, length);
        buffer[length] = '\0';

        fin.close();

        xml_document<> doc;
        doc.parse<0>(buffer);

        delete [] buffer;

        cout << "The first node is '" << doc.first_node()->name() << "'\n";
        for (xml_node<>* n = doc.first_node("card")->first_node(); n;
            n = n->next_sibling())
        {
            const char* v = n->value();
            if (!v || !*v) v = "(empty)";
            cout << n->name() << " : " << v << '\n';
        }
    }

    /// Vectors test
    {
        vec3f a = {1, 1, 1};
        vec3f b, c;

        CGE::copyVectors<float, 3>(a, b);
        CGE::addVectors<float, 3>(a, b, c);

        CGE::normalize3(c, 1.5f);
        for (size_t i = 0; i < 3; ++i)
            cout << ' ' << c[i];

        cout << endl << CGE::length3(c) << endl;
    }

    CGE::Engine::Settings s;
    s.windowTitle = "DEJARIX alpha build";
    s.windowTitle2 = "DEJARIX";

    CGE::Engine e(s);
    e.manage(new TableModule);
    return 0;
}
