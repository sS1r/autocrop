#include <iostream>

#include "run.h"
#include "parse_options.h"
#include "autocrop.h"

int run(const programOptions& opts)
{
	
	if(opts.help)
	{
		std::cout << opts.helpmsg << std::endl;
		return 0;
	}
	
	if(opts.input_files.empty())
	{
		std::cerr << "No input files given! Exiting now." << std::endl;
		return -1;
	}
	else
	{
		for(auto i = opts.input_files.begin();i != opts.input_files.end();i++)
		{			
			if(opts.verbose)
			{
				std::cout << "Processing file " << *i << std::endl;
			}
			autocrop((*i).c_str(), "cropped.png");
		}
	}
	
	return 1;
}