#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H

#include <string>
#include <vector>

class cropOptions
{
	public: 
	
	// Set the default options
	cropOptions()
	{
		color_r = 255;
		color_g = 255;
		color_b = 255;
		threshold = 0;
		border_x = 0;
		border_y = 0;
	}
	
	// Default destructor
	~cropOptions() 
	{
		
	}
	
	// Color to be removed
	uint8_t color_r;
	uint8_t color_g;
	uint8_t color_b;
	
	// Threshold value for cropping (0% -- 100%)
	unsigned threshold;
	
	// Border in x and y directions after cropping
	unsigned border_x;
	unsigned border_y;
};

class programOptions
{
	public:
	
	// Set the default options
	programOptions(): crop()
	{
		help = false;				
		verbose = false;
		read_stdin = false;
		input_files = std::vector<std::string>();
		output_file = "";
		helpmsg = "";
	}
	
	// Default destructor
	~programOptions() 
	{
		
	}
	
	// Options for cropping
	cropOptions crop;
	
	// Prints help
	bool help;
	std::string helpmsg;
	
	// Verbose printing
	bool verbose;
	
	// Read input files from stdin
	bool read_stdin;
	
	// Input and output filenames
	std::vector<std::string> input_files;
	std::string output_file;

};

programOptions parse_options(int argc, char *argv[]);


#endif /* PARSE_OPTIONS_H */