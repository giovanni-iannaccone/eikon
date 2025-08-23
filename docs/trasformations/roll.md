# Roll

This method uses the `std::rotate` function from the C++ Standard Library to cyclically shift elements within a range. It takes as input the number of columns to roll and applies a rotation to the elements in the range [first, last). The rotation is performed around a midpoint called middle, which defines the new starting point of the sequence.

The behavior of std::rotate is as follows:
- The element at middle is moved to first
- The element at middle + 1 is moved to first + 1
- This continues until all elements in the range [middle, last) are placed at the beginning
- The elements from [first, middle) are then placed after them, preserving their relative order

In simpler terms: It rearranges the elements so that the sequence starting at middle becomes the new beginning of the range, and everything before middle is shifted to the end.

Example: Given an array [A, B, C, D, E] and middle = C, the result after rotation would be [C, D, E, A, B].

The roll method takes an integer representing the middle index and applies a rotation to each row of the canvas. 

```cpp
EikonCanvas *roll(size_t col) {

    for (size_t y = 0; y < this->height; y++)
        std::rotate(
            this->pixels[y], 
            &this->pixels[y][this->width - col], 
            &this->pixels[y][this->width]
        );

    return this;
}
```