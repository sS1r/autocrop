#ifndef RUN_H
#define RUN_H

#include "parse_options.h"

/*
	Global flag for verbose output
*/
extern bool gVerboseOutput;


/*
	Runs the main program.
	
	Return value:
		< 0 : Program terminated in controlled fashion due to a non-fatal error.
		0   : Program exited without errors but didn't do anything.
		> 0 : Program exited without errors and finished its work.
		
	May also throw exceptions. In this case, a fatal error occured and the program
	will be terminated in non-controlled fashion.
	
*/
int run(const programOptions& opts);

/*
	Processes a single file
*/
void process_image(std::string fname, const programOptions& opts);

/*
	Generates a output filename from the given input filename
*/
std::string generate_output_filename(std::string input_fname);

#endif /* RUN_H */