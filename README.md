# autocrop -- automatic image cropper

autocrop is a simple command line utility that allows you to crop images automatically based on colors.
The background color is given as a command line argument, and borders of the image with this color are cropped.


## Example usage:
Say we have a photo like this, and we want to remove the borders:
![Before](/img/potato_before.png)
The background color on this image is (0, 162, 232), so running the following command:
```
./autocrop -c 0,162,232 [image_file_name]
```
Will procude this:
![After](/img/potato_after.png)
