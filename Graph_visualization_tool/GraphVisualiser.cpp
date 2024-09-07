#include "GraphVisualiser.h"
#include <fstream>
#include <streambuf>
#include <string>

static std::string header = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>GraphViz with Viz.js</title>
    </head>
    <body>
        <div id="graph" style="width: 100%; height: 500px;"></div>

        <!-- Include Viz.js from a CDN -->
        <script src="https://cdnjs.cloudflare.com/ajax/libs/viz.js/2.1.2/viz.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/viz.js/2.1.2/full.render.js"></script>

        <script>
            // Your GraphViz dot string
            var dotString = `)";



static std::string footer = R"(
        `;


        // create an instance of Viz
        var viz = new Viz();

        // Render the dot string as an SVG and append it to the div
        viz.renderSVGElement(dotString)
            .then(function(element) {
                document.getElementById('graph').appendChild(element);
            })
            .catch(error => {
                console.error(error);
            });
    </script>
    </body>
    </html>
        )";

bool visualize(const AutomationBase* fsa, const std::string& filePath)
{
	std::ofstream f(filePath);

	if (!f.is_open())
		return false;
	std::string serialized = fsa->getVisualizeString();
	f << header << serialized << footer;

	f.close();

	return true;
}
