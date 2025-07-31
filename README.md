<p align="center">
  <img src="assets/logo.png" alt="Logo">
</p>

![License](https://img.shields.io/github/license/giovanni-iannaccone/raccoon)
[![Number of GitHub issues that are open](https://img.shields.io/github/issues/giovanni-iannaccone/raccoon)](https://github.com/giovanni-iannaccone/raccoon/issues)
![Number of GitHub closed issues](https://img.shields.io/github/issues-closed/giovanni-iannaccone/raccoon)
![Number of GitHub pull requests that are open](https://img.shields.io/github/issues-pr-raw/giovanni-iannaccone/raccoon)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/giovanni-iannaccone/raccoon)
![Number of GitHub contributors](https://img.shields.io/github/contributors/giovanni-iannaccone/raccoon)
[![Number of GitHub stars](https://img.shields.io/github/stars/giovanni-iannaccone/raccoon)](https://github.com/giovanni-iannaccone/raccoon/stargazers)

# 🦝 Raccoon - Image Manipulation Library

### "Make art with code."

Raccoon is a lightweight and efficient C++ library designed for those who want to handle JPEG, PNG, and PPM files without wading through a swamp of convoluted frameworks. It lets you load, tweak, and create images effortlessly, with built-in tools for adding geometric shapes. And for command-line lovers, there's a nifty utility that makes image manipulation refreshingly straightforward.

## ✨ Features
- **Read images**: Load JPEG/PNG/PPM files effortlessly into your application
- **Modify Images**: Perform image editing operations with ease
- **Insert Geometric Shapes**: Add lines, rectangles, circles, and more directly into your images
- **Command-Line Utility**: Use the library directly from the command line for quick tasks
- **Dependency-light**: Everything is implemented from scratch or is from the standard library

## 👨‍💻 Installation
Clone the repository:
```bash
git clone https://github.com/giovanni-iannaccone/raccoon
cd raccoon
```
>[!TIP]
>Refer to the <a href="https://github.com/giovanni-iannaccone/raccoon/releases/latest">latest release</a> to have a stable and fully-working version of raccoon

## 🎨 Usage 
- Library Integration <br/>
Include the library in your C++ project:
```c++
#include "raccoon/raccoon.hpp"
```

- Use predefined shapes <br/>
The library provides a collection of predefined shapes for use in your project:
```c++
RaccoonCanvas* canvas = new RaccoonCanvas(pixels, HEIGHT, WIDTH);

auto rec = new Rectangle(150, 200, 100, 200, 0xFFA1FF15);
canvas->draw(rec);
delete rec;
```

- Define your own shapes: <br/>
By using the dependency injection pattern, you can define custom shapes:
```c++

class MyShape: public Drawable {

public:
  void draw(uint32_t pixels[], size_t height, size_t width) override {
    // code
  }
}

auto myshape = new MyShape();
canvas->draw(myshape);
```

Refer to `./include/shapes.hpp` for the implementation details of the default shapes.

## 🧃 Command Line Utility
1. Compile the CLI tool with make:
```bash
make all
```

2. Interact directly with images using the Raccoon CLI tool:
```bash
./raccoon_cli ./outputs/raccoon_cli.ppm
```
```bash
██████╗  █████╗  ██████╗ ██████╗ ██████╗  ██████╗ ███╗   ██
██╔══██╗██╔══██╗██╔════╝██╔════╝██╔═══██╗██╔═══██╗████╗  ██║
██████╔╝███████║██║     ██║     ██║   ██║██║   ██║██╔██╗ ██║
██╔══██╗██╔══██║██║     ██║     ██║   ██║██║   ██║██║╚██╗██║
██║  ██║██║  ██║╚██████╗╚██████╗╚██████╔╝╚██████╔╝██║ ╚████║
╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝
File dimensions are 800x800
Type help to show the menu
raccoon@ppm:./outputs/raccoon_cli.ppm$
```

## 👀 Who can find this helpful ?
Everybody who...
- is willing to learn a new way to create art
- has tried lots of libraries but couldn’t find one that’s easy to use
- likes building things from scratch
- is working on a big project like an image viewer or image editor

## 🔭 Learn
Read <a href="/docs/">documentation</a> to learn more about raccoon implementation and how to properly use it in your C++ project.

- <a href="https://jpeg.org/jpeg/index.html"> JPEG specs </a>
- <a href="https://www.rfc-editor.org/rfc/rfc2083.html"> PNG specs </a>
- <a href="https://netpbm.sourceforge.net/doc/ppm.html"> PPM specs </a>

## 🧩 Contributing
We welcome contributing. See CONTRIBUTING.md file for details.

## ⚖️ License
This project is licensed under the GPL-3.0 License. See the LICENSE file for details.

## ⚔️ Contact
- For any inquiries or support, please contact <a href="mailto:iannacconegiovanni444@gmail.com"> iannacconegiovanni444@gmail.com </a>.
- Visit my site for more informations about me and my work <a href="https://giovanni-iannaccone.github.io" target=”_blank” rel="noopener noreferrer"> https://giovanni-iannaccone.github.io </a>

