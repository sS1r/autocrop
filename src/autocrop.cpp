#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

#include <cmath>
#include <algorithm>

#include "autocrop.h"
#include "run.h"

// Using Boost GIL for handling images
namespace gil = boost::gil;


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


/*
	Iterates a line of pixels (row or column) and checks if the color is
	same in all of them
*/
template <typename iter_type>
bool _check_line(iter_type iter, unsigned iter_lim, const cropOptions& options)
{
	for (unsigned i = 0; i < iter_lim; i++)
	{
		uint8_t r = gil::get_color(iter[i], gil::red_t());
		uint8_t g = gil::get_color(iter[i], gil::green_t());
		uint8_t b = gil::get_color(iter[i], gil::blue_t());
		
		uint8_t base_r = options.color_r;
		uint8_t base_g = options.color_g;
		uint8_t base_b = options.color_b;

		int diff_r = base_r - r;
		int diff_g = base_g - g;
		int diff_b = base_b - b;
		
		double max_len =  sqrt(255*255 + 255*255 + 255*255);
		double diff_len = sqrt(diff_r*diff_r + diff_g*diff_g + diff_b*diff_b);
		unsigned relative_len = 100.0 * diff_len / max_len;
		
		if (relative_len > options.threshold)
		{
			return false;
		}
	}
	return true;
}

/*
	Calculates the number of rows or columns to be removed
	when cropping the image on given direction.
*/
int _crop(const gil::rgb8_view_t& view, direction dir, const cropOptions& options)
{
	unsigned i = 0;
	int i_change = 0;

	switch(dir)
	{
		case up:   
			i = 0; i_change = 1; break;
		case down:  
			i = view.height() - 1; i_change = -1; break;
		case left:  
			i = 0; i_change = 1; break;
		case right: 
			i = view.width() - 1; i_change = -1; break;
		default: 
			throw "_x_crop: invalid direction";
	}

	bool same_color = true;
	bool lines_left = true;

    while(lines_left && same_color)
	{
		if(dir == up || dir == down)
		{
			same_color = _check_line<gil::rgb8_view_t::x_iterator>(view.row_begin(i), view.width(), options);
		}
		if(dir == left || dir == right)
		{
			same_color = _check_line<gil::rgb8_view_t::y_iterator>(view.col_begin(i), view.height(), options);
		}
		
		i += i_change;
		
		switch(dir)
		{
			case up:    lines_left = i < view.height(); break;
			case down:  lines_left = i >= 0; break;
			case left:  lines_left = i < view.width(); break;
			case right: lines_left = i >= 0; break;
			default: break;
		}

	}
	
	switch(dir)
	{
		case up:    return i - 1;
		case down:  return view.height() - i - 1;
		case left:  return i - 1;
		case right: return view.width() - i - 1;
		default: break;
	}
	return -1;
}

/*
	Reads the input file into a GIL image
	The file type is deduced from the file extension
*/
void _read_image(std::string input_fname, gil::rgb8_image_t& img)
{
	// Find the file extension
	unsigned ext_loc = input_fname.rfind('.');
	if (ext_loc == std::string::npos)
	{
		throw "Input file type not recognized";
	}
	std::string ext = input_fname.substr(ext_loc + 1);
	
	// Transform to lower case
	auto tolower = [](unsigned char c) -> unsigned char {return std::tolower(c);};
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
	
	// Read the image
	if(ext == "png")
	{
		gil::png_read_and_convert_image(input_fname.c_str(), img);
	}
	else if(ext == "jpg" || ext == "jpeg")
	{	
		gil::jpeg_read_and_convert_image(input_fname.c_str(), img);
	}
	else if(ext == "tif" || ext == "tiff")
	{
		gil::tiff_read_and_convert_image(input_fname.c_str(), img);
	}
	else
	{
		throw "Input file type not supported";
	}	
}


/*
	Crops the given input file and saves the result in the output file
*/
void autocrop(const char* input_fname, const char* output_fname, const cropOptions& options)
{
	// Read the input file into a 'view'
	gil::rgb8_image_t img;
	_read_image(input_fname, img);
	gil::rgb8_view_t view = gil::view(img);

	// Calculate the amount of crop on all directions
	unsigned crop_top = _crop(view, up, options);
	if(gVerboseOutput)
	{
		std::cout << "Removing " << crop_top << " rows from top." << std::endl;
	}
	
	unsigned crop_bottom = _crop(view, down, options);
	if(gVerboseOutput)
	{
		std::cout << "Removing " << crop_bottom << " rows from bottom." << std::endl;
	}
	
	unsigned crop_left = _crop(view, left, options);
	if(gVerboseOutput)
	{
		std::cout << "Removing " << crop_left << " rows from left." << std::endl;
	}
	
	unsigned crop_right = _crop(view, right, options);
	if(gVerboseOutput)
	{
		std::cout << "Removing " << crop_right << " rows from right." << std::endl;
	}
	
	// Crop the original image
	unsigned new_w = view.width() - crop_left - crop_right;
	unsigned new_h = view.height() - crop_top - crop_bottom;
	gil::rgb8_view_t cropped_view = gil::subimage_view(view, crop_left, crop_top, new_w, new_h);	
	
	// Generate output image
	gil::rgb8_image_t output_img(new_w + 2 * options.border_x, new_h + 2 * options.border_y);
	
	// Fill the output image with the cropping base color
	gil::rgb8_pixel_t fill_color(options.color_r, options.color_g, options.color_b);
	gil::fill_pixels(gil::view(output_img), fill_color);

	// Copy the cropped section of the original image into the output image
	gil::rgb8_view_t output_view = gil::subimage_view(gil::view(output_img), options.border_x, options.border_y, new_w, new_h);
	gil::copy_pixels(cropped_view, output_view);
	
	// Write output
	gil::png_write_view(output_fname, gil::const_view(output_img));
}
