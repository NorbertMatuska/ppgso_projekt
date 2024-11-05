// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <limits.h>

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
  // TODO: Define correct data type for r, g, b channel
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

int main()
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        std::cout << "Current Path: " << buffer << std::endl;
    } else {
        std::cerr << "Error getting current path" << std::endl;
    }
    // Initialize a framebuffer
  auto framebuffer = new Pixel[SIZE][SIZE];

  // TODO: Open file lena.raw (this is 512x512 RAW GRB format)

  std::ifstream file;
  file.open("lena.raw", std::ios::binary);
  if (!file) {
      std::cout << "File not created\n";
  } else {
      std::cout << "File created\n";
  }

  // TODO: Read data to framebuffer and close the file

  file.read(reinterpret_cast<char *>((unsigned char *) framebuffer), SIZE * SIZE * 3);
  file.close();

  // Traverse the framebuffer
  for (unsigned int y = 0; y < SIZE; y++) {
    for (unsigned int x = 0; x < SIZE; x++) {
        // TODO: Apply pixel operation
        if(y < SIZE/2) {
            unsigned char grayscale = (unsigned char)(framebuffer[y][x].r*0.299 + framebuffer[y][x].g*0.587 + framebuffer[y][x].b * 0.114);

            framebuffer[y][x].r = grayscale;
            framebuffer[y][x].g = grayscale;
            framebuffer[y][x].b = grayscale;
        }
        else {
            framebuffer[y][x].r *= (float)rand() / (RAND_MAX);
            framebuffer[y][x].g *= (float)rand() / (RAND_MAX);
            framebuffer[y][x].b *= (float)rand() / (RAND_MAX);
        }
    }
  }

  // TODO: Open file result.raw
  std::cout << "Generating result.raw file ..." << std::endl;
  std::ofstream output;
  output.open("result.raw", std::ios::binary);
  if(!output) {
      std::cout << "fail" << std::endl;
  }
  // TODO: Write the framebuffer to the file and close it
  output.write(reinterpret_cast<const char *>((unsigned char *) framebuffer), SIZE * SIZE * 3);

  output.close();

  std::cout << "Done." << std::endl;
  delete[] framebuffer;
  return EXIT_SUCCESS;
}
