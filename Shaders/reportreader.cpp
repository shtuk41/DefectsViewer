#include "reportreader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <cctype>

using namespace report_reader;

ReportReader *ReportReader::reportReader = 0;

bool string_contains_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || *it == '.')) ++it;
		return !s.empty() && it == s.end();
}

bool ReportReader::ReadFile(std::string fileName)
{
	data.clear();

	std::ifstream fileRead;
	fileRead.open(fileName);

	std::string line;

	std::getline(fileRead, line);

	std::istringstream ss(line);

	FIELDS field = NUMBER;

	while (std::getline(ss, line, ','))
	{
		switch(field)
		{
		case NUMBER:
			if (line.compare("Number") != 0)
				return false;
			break;
		case FOV:
			if (line.compare("FOV Name") != 0)
				return false;
			break;
		case X:
			if (line.compare(" Defect Location on FOV X") != 0)
				return false;
			break;
		case Y:
			if (line.compare(" Defect Location on FOV Y") != 0)
				return false;
			break;
		case KIND:
			if (line.compare("Kind of Defect") != 0)
				return false;
			break;
		case SCORE:
			if (line.compare(" SurfMax Score") != 0)
				return false;
			break;
		case NORMALIZED:
			if (line.compare(" Normalized Score") != 0)
				return false;
			break;
		case CLASSIFICATION:
			if (line.compare(" Classification") != 0)
				return false;
			break;
		}

		field=(FIELDS)((int)field +  1);
	}
	
	while (std::getline(fileRead, line))
	{
		std::istringstream iss(line);
		FIELDS record = NUMBER;
		int number;
		int	x;
		int y;
		double score;
		std::string kind;
		std::string fov;
		std::string classification;

		while (std::getline(iss, line, ','))
		{
			std::stringstream ss(line);
			if (record == NUMBER)
			{
				if (string_contains_number(line))
				{
					ss >> number;
				}
				else
				{
					break;
				}
			}
			else if (record == FOV)
			{
				if (!line.empty())
				{
					fov = line;
				}
				else
				{
					break;
				}
			}
			else if (record == X)
			{
				if (string_contains_number(line))
				{
					ss >> x;
				}
				else
				{
					break;
				}
			}
			else if (record == Y)
			{
				if (string_contains_number(line))
				{
					ss >> y;
				}
				else
				{
					break;
				}
			}
			else if (record == KIND)
			{
				if (!line.empty())
				{
					kind = line;
				}
				else
				{
					break;
				}
			}
			else if (record == SCORE)
			{
				if (string_contains_number(line))
				{
					ss >> score;
				}
				else
				{
					break;
				}
			}
			else if (record == CLASSIFICATION)
			{
				if (!line.empty())
				{
					classification = line;
				}
				else
				{
					break;
				}
			}
		
			if (record == CLASSIFICATION && classification.compare("IsNotOk") == 0)
			{
				Defect d = Defect(number,fov,x,y,kind, score);
				data.push_back(d);
			}

			record=(FIELDS)((int)record +  1);
		}

		
	}

	return true;


	//
}