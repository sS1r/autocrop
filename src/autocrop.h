#ifndef AUTOCROP_H
#define AUTOCROP_H

#include "parse_options.h"

void autocrop(const char* input_fname, const char* output_fname, const cropOptions& options);

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
	unsigned r, g, b, a;
};

#endif /* AUTOCROP_H */