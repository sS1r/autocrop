#ifndef AUTOCROP_H
#define AUTOCROP_H

#include <cmath>
#include <ostream>

#include "parse_options.h"

void autocrop(const char* input_fname, const char* output_fname, cropOptions options);

// Supported image types
enum IMG_TYPE
{
	IMG_PNG,
	IMG_JPG,
	IMG_TIF,
};

/*
	Direction enumeration.
	Defines the part of the image that will be cropped.
*/
enum direction
{
	up =    0,
	down =  1,
	left =  2,
	right = 3
};

struct Color
{
	uint8_t r, g, b;
	
	// Needed for map
	bool operator<(const Color& c) const
    { 
		double abs_this =  sqrt(r*r + g*g + b*b);
		double abs_other = sqrt(c.r*c.r + c.g*c.g + c.b*c.b);
        return abs_this < abs_other;
    }
	
	// Needed for debugging
	friend std::ostream& operator<<(std::ostream& os, const Color& c)
	{
		os << "[" << (unsigned)c.r << "," << (unsigned)c.g << "," << (unsigned)c.b << "]";
		return os;
	}
};

#endif /* AUTOCROP_H */