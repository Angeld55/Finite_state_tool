#include <iostream>
#include "../Finate_state_automation/FiniteStateAutomation.h"
class GraphVisualiser
{
	std::string headerPath;
	std::string footerPath;
	std::string header;
	std::string footer;
	bool isOkay;

public:
	GraphVisualiser(const std::string& headerTemplatePath, const std::string& footerTemplatePath);

	bool init();
	bool isOkey() const;
	bool visualize(const AutomationBase* fsa, const std::string& filePath);

};
