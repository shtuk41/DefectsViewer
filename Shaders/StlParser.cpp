#include "StlParser.h"

output_data StlParser::ParseToTriangleMesh(const char *stlFileName, unsigned int &number_of_triangles, float xoffset, float yoffset, float zoffset)
{
	std::ifstream stlFile;
	//std::ofstream dataFile;

	stlFile.open(stlFileName,std::ios::binary);
	//dataFile.open("stlDataFileDebug.txt",std::ios::trunc);

	output_data data;

	data.output = 0;
	data.normal = 0;
	number_of_triangles = 0;
	unsigned int numberTriangles;

	STL_HEADER	partHolderHeader;
	
	if (stlFile.is_open() /*&& dataFile.is_open()*/)
	{
		stlFile.seekg (0, std::ios::beg);
		stlFile.read ((char *)&partHolderHeader, sizeof(STL_HEADER));
		stlFile.read ((char *)&numberTriangles, sizeof(unsigned int));

		//dataFile << "Number of Triangles:	" <<  numberTriangles << std::endl;
		//dataFile << "Size of Triangle:	" <<  sizeof(triangle) << std::endl;
		//dataFile << "Header size" << sizeof(STL_HEADER) << std::endl;

		data.output = new vec4[numberTriangles * 3];
		data.normal = new vec3[numberTriangles * 3];
		number_of_triangles = numberTriangles;

		char *c = strstr((char*)partHolderHeader.data,"COLOR");

		if (c)
		{
			//dataFile << "COLOR FOUND" << std::endl;
		}

		c = strstr((char*)partHolderHeader.data,"MATERIAL");

		if (c)
		{
			//dataFile << "MATERIAL FOUND" << std::endl;
		}

		for (unsigned int ii = 0; ii < numberTriangles; ii++)
		{
			unsigned int index = ii * 3;
			triangle next;
			size_t sizeofTriangle = sizeof(triangle);
			stlFile.read((char*)&next,50);

			//dataFile << "Normal vector: " << std::endl;
			//dataFile << next.norm.data[0] << " " << next.norm.data[1] << " " << next.norm.data[2] << std::endl;
			//dataFile << "Triangle" << std::endl;

			data.normal[index]		=	
			data.normal[index + 1]	= 
			data.normal[index + 2]	=	vec3(next.norm.data[0],next.norm.data[1],next.norm.data[2]);


			//dataFile << next.points[0].data[0] << " " << next.points[0].data[1] << " " << next.points[0].data[2] << std::endl;
			
			data.output[index] = vec4(next.points[0].data[0] + xoffset,next.points[0].data[1] + yoffset,next.points[0].data[2] + zoffset, 1.0f);
			
			//dataFile << next.points[1].data[0] << " " << next.points[1].data[1] << " " << next.points[1].data[2] << std::endl;
			
			data.output[index + 1] = vec4(next.points[1].data[0] + xoffset,next.points[1].data[1] + yoffset,next.points[1].data[2] + zoffset, 1.0f);

			//dataFile << next.points[2].data[0] << " " << next.points[2].data[1] << " " << next.points[2].data[2] << std::endl;
			
			data.output[index + 2] = vec4(next.points[2].data[0] + xoffset,next.points[2].data[1] + yoffset,next.points[2].data[2] + zoffset, 1.0f);

			/*int blue	=	(next.atribute_byte_count &  0x001f);
			int green	=	(next.atribute_byte_count &  0x03e0) >> 5;
			int red		=	(next.atribute_byte_count &  0x7c00) >> 10;
			int valid	=	(next.atribute_byte_count &  0x8000) >> 15;

			dataFile << "Blue:	" << blue << endl;
			dataFile << "Green	" << green << endl;
			dataFile << "Red:	" << red << endl;
			dataFile << "Valid:	" << valid << endl;

			int iRedWrite = 20 << 10;
			int iGreenWrite = 20 << 5;

			next.atribute_byte_count |= iRedWrite;
			next.atribute_byte_count |= iGreenWrite;
			next.atribute_byte_count |= 1 << 15;

			blue	=	(next.atribute_byte_count &  0x001f);
			green	=	(next.atribute_byte_count &  0x03e0) >> 5;
			red		=	(next.atribute_byte_count &  0x7c00) >> 10;
			valid	=	(next.atribute_byte_count &  0x8000) >> 15;

			dataFileNew << "Blue:	" << blue << endl;
			dataFileNew << "Green	" << green << endl;
			dataFileNew << "Red:	" << red << endl;
			dataFileNew << "Valid:	" << valid << endl;*/
		}
	}

		return data;
}