#include "parse_options.h"

#include <boost/program_options.hpp>

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

namespace po = boost::program_options;        

const char* PROGRAM_HELP_DESC = 
"autocrop -- automatic image cropper.\n\n"
"Usage: autocrop [options] [INPUT_FILES]\n"
"Reads file names from standard input if INPUT_FILES is empty.\n\n";	

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
	visible_add("auto-detect-bg", "Detects background color automatically (TBA)");
	visible_add("border_x", po::value<unsigned>(), "Leave border in X direction");
	visible_add("border_y", po::value<unsigned>(), "Leave border in Y direction");
	visible_add("threshold,t", po::value<double>(), "Cropping threshold [0 ... 100].");
	visible_add("color,c", po::value<std::string>(), "Base color, in format R,G,B.");
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
	if(vm.count("border_x"))
	{
		po.crop.border_x = vm["border_x"].as<unsigned>();
	}
	if(vm.count("border_y"))
	{
		po.crop.border_y = vm["border_y"].as<unsigned>();
	}
	if(vm.count("threshold"))
	{
		double th = vm["threshold"].as<double>();
		if(th < 0 || th > 100)
		{
			th = th < 0 ? 0 : 100;
			std::cerr << "Warning: threshold out of bounds; set to " << th << "." << std::endl;
		}
		po.crop.threshold = th;
	}
	if(vm.count("color"))
	{
		std::string color = vm["color"].as<std::string>();
		
		// Make sure there are exactly 2 commas in the string
		if(std::count(color.begin(), color.end(), ',') != 2)
		{
			throw "'color' option in wrong format!";
		}
		else
		{
			// Parse the color string
			int i1 = color.find(',');
			int i2 = color.find(',', i1 + 1);
			std::string r_str = color.substr(0, i1);
			std::string g_str = color.substr(i1 + 1, i2 - i1);
			std::string b_str = color.substr(i2 + 1);
			
			// Convert to int
			int r, g, b;
			try 
			{
				r = std::stoi(r_str);
				g = std::stoi(g_str);
				b = std::stoi(b_str);
			}
			catch(...)
			{
				throw "Could not convert color value to integer";
			}
			
			// Check bounds
			if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255)
			{
				throw "Color out of bounds [0 ... 255]";
			}
			po.crop.color_r = r;
			po.crop.color_g = g;
			po.crop.color_b = b;
		}
	}
	
	// Check the input files
	if(vm.count("input-files"))
	{		
		po.input_files = vm["input-files"].as<std::vector<std::string>>();
		po.read_stdin = false;
	}
	else
	{
		po.read_stdin = true;
	}
	
	// Generate the help message
	std::stringstream ss;
	ss << PROGRAM_HELP_DESC << visible;
	po.helpmsg = ss.str();

	return po;
}