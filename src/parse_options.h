#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H

#include <string>
#include <vector>

class programOptions
{
	public:
	
	// Set the default options
	programOptions()
	{
		help = false;				
		verbose = false;
		threshold = 0;
		color_r = 255;
		color_g = 255;
		color_b = 255;
		input_files = std::vector<std::string>();
		output_file = "";
		helpmsg = "";
		border_x = 0;
		border_y = 0;
	}
	
	// Default destructor
	~programOptions() 
	{
		
	}
	
	// Prints help
	bool help;
	std::string helpmsg;
	
	// Verbose printing
	bool verbose;
	
	// Threshold value for cropping (0% -- 100%)
	unsigned threshold;
	
	// Color to be removed
	uint8_t color_r;
	uint8_t color_g;
	uint8_t color_b;
	
	// Input and output filenames
	std::vector<std::string> input_files;
	std::string output_file;
	
	// Border in x and y directions after cropping
	unsigned border_x;
	unsigned border_y;
};

programOptions parse_options(int argc, char *argv[]);


#endif /* PARSE_OPTIONS_H */