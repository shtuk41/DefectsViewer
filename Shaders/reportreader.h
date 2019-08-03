#ifndef _REPOPRTREADER_H
#define _REPOPRTREADER_H

#include <string>
#include <vector>

namespace report_reader
{

class Defect
{
	public:
		Defect(int n, std::string fov_in, int x_in, int y_in, std::string k, double score_in) : fov(fov_in), number(n), x(x_in), y(y_in), kind(k), score(score_in) {}

		int GetNumber() const {return number;}
		int GetX() const {return x;}
		int GetY() const {return y;}
		double GetScore() const {return score;}
		std::string GetKind() const { return kind;}
		std::string GetFov() const {return fov;}
	private:
		int number;
		int	x;
		int y;
		double score;
		std::string kind;
		std::string fov;
};

class ReportReader
	{
	public:
		enum FIELDS {NUMBER = 0, FOV, X, Y, KIND, SCORE, NORMALIZED, CLASSIFICATION, BAR_CODE};
		static ReportReader* Instance() { if (reportReader) return reportReader; else return reportReader =  new ReportReader();}
		~ReportReader();

		static bool LoadFile(std::string fileName) {return Instance()->ReadFile(fileName);}
		static std::vector<Defect> & GetData() { return Instance()->data;}
		
	private:
		ReportReader() {}

		bool ReadFile(std::string fileName);
		
		static ReportReader *reportReader;
		
		std::vector<Defect> data;
	};
}

#endif
