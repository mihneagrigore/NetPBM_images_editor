
# Image Processing Algorithm Documentation

This document describes the implementation of an image processing algorithm, detailing the functions and their specific roles in processing images as per the given requirements.

## Table of Contents
1. [Overview](#overview)
2. [Functions Overview](#functions-overview)
    - [image_load](#image_load)
    - [select_zone & select_all](#select_zone--select_all)
    - [alloc_color, alloc_nocolor, alloc_double](#alloc_color-alloc_nocolor-alloc_double)
    - [read_color & read_nocolor](#read_color--read_nocolor)
    - [crop_color & crop_nocolor](#crop_color--crop_nocolor)
    - [rotate](#rotate)
    - [save](#save)
    - [apply](#apply)
    - [histogram](#histogram)
    - [equalize](#equalize)
3. [Conclusion](#conclusion)

## Overview

The implemented algorithm processes images according to the specified requirements. The program reads commands from the user, processes them, and modifies images as needed. It handles edge cases such as comments in commands, and ensures proper memory management when loading, modifying, and saving images.

## Functions Overview

### image_load

The `image_load` function is responsible for loading the image from a file. It opens and closes the file properly, displays error messages when needed, and sets the `loaded` flag according to the result of the operation.

### select_zone & select_all

The `select_zone` and `select_all` functions are used to select a specific area from the image or the entire image, respectively. They display error messages if the commands are invalid or improperly formatted.

### alloc_color, alloc_nocolor, alloc_double

These functions are used to allocate memory for different types of bidimensional data:

- **`alloc_color`**: Allocates a matrix of RGB structures.
- **`alloc_nocolor`**: Allocates a matrix of integers for black-and-white pixel values.
- **`alloc_double`**: Allocates a matrix for double-precision floating-point numbers used in calculations.

The corresponding deallocation functions, **`free_color`**, **`free_nocolor`**, and **`free_double`**, free the memory for these data structures when no longer needed.

### read_color & read_nocolor

The `read_color` and `read_nocolor` functions read pixel data from the image file that was loaded by the `image_load` function. If any errors occur during this process, they display appropriate messages to the user.

### crop_color & crop_nocolor

The `crop_color` and `crop_nocolor` functions implement the **CROP** command. They create an auxiliary matrix based on the dimensions specified in the command and overwrite the original matrix with the cropped version after freeing the original memory.

### rotate

The `rotate` function rotates the image by the number of degrees specified in the **ROTATE** command. It handles any errors appropriately. If the degree value is `+/-360` or `0`, the image remains unchanged. Negative degree values are converted to their positive equivalents.

### save

The `save` function saves the image's pixels into a specified file. It also ensures that the image is saved in the correct format (either ASCII or binary), converting the image type if necessary.

### apply

The `apply` function handles the **APPLY** command. It first displays error messages if necessary and then invokes the `apply_command` function. This function assigns an integer value to each argument in the **APPLY** command, which is used to determine the appropriate apply function to call. All calculations are performed using `double` data types and converted to `int` using `floor` and `clamp`.

### histogram

The `histogram` function prints the histogram of a black-and-white image or displays an error message if the image is incompatible. A dynamically allocated array is used to store the values of each histogram bin. The maximum value is determined, and the number of stars to display is calculated using a formula. The memory for the auxiliary histogram array is then freed.

### equalize

Similar to the **histogram** function, the `equalize` function calculates the histogram and then applies a formula to equalize the pixel values in the image. It frees memory for both the auxiliary histogram array and the auxiliary array used in the calculations.

## Conclusion

This image processing algorithm provides a set of functions to manipulate images by applying various commands such as loading, cropping, rotating, saving, and applying filters. Proper memory allocation and deallocation are implemented throughout to ensure efficiency and avoid memory leaks. Error handling is built into each function to guide the user through any issues that may arise during the execution of the program.
