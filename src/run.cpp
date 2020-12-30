#include <iostream>

#include "run.h"
#include "parse_options.h"
#include "autocrop.h"

// Global flag for verbose output
bool gVerboseOutput;

int run(const programOptions& opts)
{
	
	if(opts.help)
	{
		std::cout << opts.helpmsg << std::endl;
		return 0;
	}
	
	gVerboseOutput = opts.verbose;
	
	if(opts.read_stdin)
	{
		std::string fname;
		while(std::getline(std::cin, fname))
		{
			autocrop(fname.c_str(), generate_output_filename(fname).c_str(), opts.crop);
		}
	}
	else
	{
		for(auto i = opts.input_files.begin();i != opts.input_files.end();i++)
		{			
			if(opts.verbose)
			{
				std::cout << "Processing file " << *i << std::endl;
			}
			autocrop((*i).c_str(), generate_output_filename(*i).c_str(), opts.crop);
		}
	}
	
	return 1;
}

	
std::string generate_output_filename(std::string input_fname)
{
	// Split the filename at extension
	unsigned ext_loc = input_fname.rfind('.');
	if (ext_loc == std::string::npos)
	{
		return input_fname + "_cropped";
	}
	std::string ext = input_fname.substr(ext_loc + 1);
	std::string base = input_fname.substr(0, ext_loc);
	
	return base + "_cropped." + ext;
}
