#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <iterator>

namespace gil = boost::gil;

enum direction
{
	up =    0,
	down =  1,
	left =  2,
	right = 3
};

template <typename iter_type>
bool _check_line(iter_type iter, unsigned iter_lim)
{
	for (unsigned i = 0; i < iter_lim; i++)
	{
		uint8_t r = gil::get_color(iter[i], gil::red_t());
		uint8_t g = gil::get_color(iter[i], gil::green_t());
		uint8_t b = gil::get_color(iter[i], gil::blue_t());
		
		if (r != 255 or g != 255 or b != 255)
		{
			return false;
		}
	}
	return true;
}

int _crop_x(const gil::rgb8_view_t& view, direction dir)
{
	unsigned i     = 0;
	int i_change = 0;
	
	if(dir == up || dir == left)
	{
		i = 0;
		i_change = 1;
	}
	else if(dir == down)
	{
		i = view.height() - 1;
		i_change = -1;
	}
	else if(dir == right)
	{
		i = view.width() - 1;
		i_change = -1;
	}
	else
	{
		throw "_x_crop: invalid direction";
	}
	
	bool same_color = true;
	bool lines_left = true;

    while(lines_left && same_color)
	{
		if(dir == up || dir == down)
		{
			same_color = _check_line<gil::rgb8_view_t::x_iterator>(view.row_begin(i), view.width());
		}
		if(dir == left || dir == right)
		{
			same_color = _check_line<gil::rgb8_view_t::y_iterator>(view.col_begin(i), view.height());
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

void autocrop(const char* input_fname, const char* output_fname)
{
	unsigned crop_top = 0;
	unsigned crop_bottom = 0;
	unsigned crop_left = 100;
	unsigned crop_right = 100;
	
	gil::rgb8_image_t img;
	gil::png_read_and_convert_image(input_fname, img);
	gil::rgb8_view_t view = gil::view(img);

	crop_top = _crop_x(view, up);
	std::cout << "Would remove " << crop_top << " rows from top." << std::endl;
	
	crop_bottom = _crop_x(view, down);
	std::cout << "Would remove " << crop_bottom << " rows from bottom." << std::endl;

	crop_left = _crop_x(view, left);
	std::cout << "Would remove " << crop_left << " rows from left." << std::endl;
	
	crop_right = _crop_x(view, right);
	std::cout << "Would remove " << crop_right << " rows from right." << std::endl;

	// Crop the original image and write to file
	unsigned new_w = view.width() - crop_left - crop_right;
	unsigned new_h = view.height() - crop_top - crop_bottom;
	gil::rgb8_view_t new_view = gil::subimage_view(view, crop_left, crop_top, new_w, new_h);
	gil::png_write_view(output_fname, new_view);
}
