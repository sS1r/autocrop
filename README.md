# autocrop -- automatic image cropper

autocrop is a simple command line utility that allows you to crop images automatically based on colors.
The background color is given as a command line argument, and borders of the image with this color are cropped.

The program is especially useful for applications which produce a lot of image files that have unnecessarily thick borders. 
Removing those borders will make the images a lot nicer if they need to be included for example in a report.

## Example usage 1:
Say we have a photo like this, and we want to remove the borders:
![Before](/img/potato_before.png)
The background color on this image is (0, 162, 232), so running the following command:
```
./autocrop -c 0,162,232 [image_file_name]
```
Will procude this:
![After](/img/potato_after.png)

## Example usage 2:
If you have a folder with multiple PNG files with white background that you want to crop, you can do it like this on Linux:
```
ls *.png  | ./autocrop -c 255,255,255 --overwrite
```
or like this on Windows:
```
dir /B *.png  | autocrop.exe -c 255,255,255 --overwrite
```
Note that this will overwrite the existing files.
