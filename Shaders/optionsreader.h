#ifndef _OPTIONSREADER_H
#define _OPTIONSREADER_H

#include <string>
#include <map>

namespace options_reader
{
	class ApplicationWindow
	{
	public:

		ApplicationWindow():x(0), y(0), width(1920), height(1080) {}
		ApplicationWindow(int x_in, int y_in, int width_in, int height_in) : x(x_in), y(y_in), width(width_in), height(height_in) {}
		int GetX() const {return x;}
		int GetY() const { return y;}
		int GetWidth() const { return width;}
		int GetHeight() const { return height;}
	private:
		int x;
		int y;
		int width;
		int height;
	};

	struct Part
	{
	public:
		Part(std::string file) : fileName(file) {}
		std::string GetFileName() const { return fileName;};
	private:
		std::string		fileName;
	};

	struct Pointer
	{
	public:
		Pointer(std::string file) : fileName(file) {}
		std::string GetFileName() const { return fileName;}
	private:
		std::string		fileName;
	};

	class Defects
	{
	public:
		Defects() {}
		bool FindByKey(std::string, std::string &);
		void Add(std::string key, std::string value);
	private:
		std::map<std::string, std::string>  defects_list;
	};

	struct Legends
	{
	public:
		Legends(std::string file) : fileName(file) {}
		std::string GetFileName() const { return fileName;}
	private:
		std::string		fileName;
	};


	class OptionsReader
	{
	public:
		static OptionsReader* Instance() { if (optionsReader) return optionsReader; else return optionsReader =  new OptionsReader();}
		~OptionsReader();

		static const ApplicationWindow* GetApplicationWindowOptions() { return Instance()->applicationWindow;}
		static const Part* GetPartOptions() { return Instance()->part;}
		static const Pointer* GetPointerOptions() { return Instance()->pointer;}
		static const Legends* GetLegendsOptions() { return Instance()->legends;}
		static Defects* GetDefectsOptions() { return Instance()->defects;}
	private:
		OptionsReader();
		void Init();
		static OptionsReader *optionsReader;

		ApplicationWindow *applicationWindow;
		Part	*part;
		Pointer *pointer;
		Defects *defects;
		Legends *legends;
	};

}

#endif