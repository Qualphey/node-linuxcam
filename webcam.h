/** Small C++ wrapper around V4L example code to access the webcam
**/

#include <iostream>
#include <string>
#include <memory> // unique_ptr
#include <cstring>

#include <iostream>

#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <stdexcept>

#include <linux/videodev2.h>

using namespace std;

struct buffer {
  void   *data;
  size_t  size;
};

class Image {
  public:
    unsigned char *data = NULL;
    long unsigned int width = 0;
    long unsigned int height = 0;
    long unsigned int size = 0;

    Image(long unsigned int width, long unsigned int height) {
      this->width = width;
      this->height = height;
    }

    Image(long unsigned int width, long unsigned int height,
                                      long unsigned int size) {
      data = (unsigned char *) malloc(size);
      this->width = width;
      this->height = height;
      this->size = size;
    }

    ~Image() {
      delete[] data;
    }
};


class Webcam {

public:
  Webcam(const std::string& device = "/dev/video0",
  int width = 640,
  int height = 480);

  ~Webcam();

  /** Captures and returns a frame from the webcam.
  *
  * The returned object contains a field 'data' with the image data in RGB888
  * format (ie, RGB24), as well as 'width', 'height' and 'size' (equal to
  * width * height * 3)
  *
  * This call blocks until a frame is available or until the provided
  * timeout (in seconds).
  *
  * Throws a runtime_error if the timeout is reached.
  */
  const Image* frame(int timeout = 1);
private:
  void init_mmap();

  void open_device();
  void close_device();

  void init_device();
  void uninit_device();

  void start_capturing();
  void stop_capturing();

  bool read_frame();

  std::string device;
  int fd;

  Image *rgb_frame;
  struct buffer          *buffers;
  unsigned int     n_buffers;

  size_t xres, yres;
  size_t stride;

  bool force_format = true;
};
