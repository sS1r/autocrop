#include <iostream>

#include "autocrop.h"
#include "parse_options.h"
#include "run.h"

int main(int argc, char *argv[])
{
    try
    {
		const programOptions opts = parse_options(argc, argv);
		int status = run(opts);
		
		if(status >= 0)
		{
			std::cout << "Success!" << std::endl;
		}
		else
		{
			std::cout << "Fail!" << std::endl;
		}
    }

    catch(const char* text)
    {
        std::cerr << "Error: " << text << std::endl;
    }
	catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }    
    catch(...)
    {
        std::cout << "Unknown error occured.\n";
    }

    return 0;
}
