#include <rapidxml/rapidxml.hpp>
#include <fstream>
#include <iostream>

#include <CGE/Engine.h>
#include "TestModule.h"

int main(int argc, char** argv)
{
    /// RapidXML demo
    {
        using namespace rapidxml;
        using namespace std;

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
            char* v = n->value();
            if (!v) v = "(empty)";
            cout << n->name() << " : " << v << '\n';
        }
    }

    CGE::Engine e;
    {
        TestModule tm;
        e.run(&tm);
    }
    return 0;
}
