#include <fstream>
#include <sstream>
#include <vector>
#include "optionsreader.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using namespace options_reader;

OptionsReader *OptionsReader::optionsReader = 0;

bool Defects::FindByKey(std::string key, std::string &value)
{
	std::map<std::string, std::string>::iterator it = defects_list.begin();

	while (it != defects_list.end())
	{
		if ((it->first).compare(key) == 0)
		{
			value =  it->second;
			return true;
		}

		++it;
	}

	value = "";

	return false;
}

void Defects::Add(std::string key, std::string value)
{
	defects_list.insert(std::make_pair(key, value));	
}

OptionsReader::OptionsReader()
{
	applicationWindow = 0;
	part = 0;
	pointer = 0;
	defects = 0;

	Init();
}

int ConvertCharToInt(char *v)
{
	std::stringstream ss((std::string(v)));
	int output;
	ss >> output;
	return output;
}

void OptionsReader::Init()
{
	xml_document<> doc;
	xml_node<> * root_node;
	std::ifstream file("options.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	root_node = doc.first_node("application");

	xml_node<> * application_node = root_node->first_node("windows");

	int x = ConvertCharToInt(application_node->first_attribute("x")->value());
	int y = ConvertCharToInt(application_node->first_attribute("y")->value());
	int width = ConvertCharToInt(application_node->first_attribute("width")->value());
	int height = ConvertCharToInt(application_node->first_attribute("height")->value());

	applicationWindow = new ApplicationWindow(x,y,width,height);

	xml_node<> * part_node = root_node->first_node("part");
	part = new Part(part_node->first_attribute("file")->value());

	xml_node<> * pointer_node = root_node->first_node("pointer");
	pointer = new Pointer(pointer_node->first_attribute("file")->value());

	xml_node<> * legends_node = root_node->first_node("legends");
	legends = new Legends(legends_node->first_attribute("file")->value());

	defects = new Defects();

	xml_node<> *defects_node = root_node->first_node("defects");

	for (xml_node<> *defect_node = defects_node->first_node("defect"); defect_node; defect_node = defect_node->next_sibling())
	{
		defects->Add(defect_node->first_attribute("name")->value(),defect_node->first_attribute("color")->value());
	}
}

OptionsReader::~OptionsReader()
{
	if (applicationWindow)
		delete applicationWindow;

	applicationWindow = 0;

	if (part)
		delete part;

	part = 0;

	if (pointer)
		delete pointer;

	pointer = 0;

	if (defects)
		delete defects;

	defects = 0;

	if (legends)
		delete legends;

	legends = 0;
}



