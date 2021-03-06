#include "GraphVisualiser.h"
#include <fstream>
#include <streambuf>
#include <string>

bool readFile(const std::string& filePath, std::string& str)
{

	std::ifstream t(filePath);
	if (!t.is_open())
		return false;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	t.close();

	return true;
}

GraphVisualiser::GraphVisualiser(const std::string& headerTemplatePath, const std::string& footerTemplatePath) : headerPath(headerTemplatePath), footerPath(footerTemplatePath), isOkay(false)
{
	
}

bool GraphVisualiser::init()
{
	bool res = readFile(headerPath, header);
	if (!res)
		return isOkay = false;
	return isOkay = readFile(footerPath, footer);
}
bool GraphVisualiser::visualize(const AutomationBase* fsa, const std::string& filePath)
{
	if (!isOkay)
		return false;

	std::ofstream f(filePath);

	if (!f.is_open())
		return false;
	std::string serialized = fsa->getVisualizeString();
	f << header << serialized << footer;

	f.close();

	return true;
}
bool GraphVisualiser::isOkey()
{
	return isOkay;
}
