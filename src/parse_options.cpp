#include "parse_options.h"

#include <boost/program_options.hpp>

#include <sstream>
#include <iostream>

namespace po = boost::program_options;        

const char* PROGRAM_HELP_DESC = 
"autocrop -- automatic image cropper.\n\n"
"Usage: autocrop [options] INPUT_FILES\n";	

programOptions parse_options(int argc, char *argv[])
{
	programOptions po;
	
	// Option descriptions (separate for hidden and visible opts)
	po::options_description visible("Options");
	po::options_description hidden;
	po::options_description all;
	
	// Options adders (again separate for hidden and visible)
	po::options_description_easy_init visible_add = visible.add_options();
	po::options_description_easy_init hidden_add = hidden.add_options();
	po::positional_options_description p;
	
	// Add all options here
	visible_add("help,h", "Print this help and exit");
	visible_add("verbose,v", "Print verbose output");
	visible_add("threshold,t", po::value<double>(), "Cropping threshold [0 ... 100].");
	hidden_add("input-files", po::value<std::vector<std::string>>(), "Input file");
	p.add("input-files", -1);
	all.add(visible).add(hidden);
	
	// Parse options and add them to a map
	po::variables_map vm;        
	po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);
	po::notify(vm);
	
	// Get the option values
	po.help = bool(vm.count("help"));
	po.verbose = bool(vm.count("verbose"));
	if(vm.count("threshold"))
	{
		double th = vm["threshold"].as<double>();
		if(th < 0 || th > 100)
		{
			th = th < 0 ? 0 : 100;
			std::cerr << "Warning: threshold out of limits; set to " << th << "." << std::endl;
		}
		po.threshold = th;
	}
	if(vm.count("input-files")) po.input_files = vm["input-files"].as<std::vector<std::string>>();

	// Generate the help message
	std::stringstream ss;
	ss << PROGRAM_HELP_DESC << visible;
	po.helpmsg = ss.str();

	return po;
}