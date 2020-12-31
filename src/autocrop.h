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

#endif /* AUTOCROP_H */