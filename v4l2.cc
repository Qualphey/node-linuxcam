#include <iostream>
#include <fstream>
using namespace std;

#include <node.h>
#include <node_object_wrap.h>
using namespace v8;

#include <jpeglib.h>

#include "webcam.h"

#define XRES 640
#define YRES 480

Webcam* webcam;

void start(const FunctionCallbackInfo<Value>& args) {
  webcam = new Webcam("/dev/video0", XRES, YRES);
}

class image {
  public:
    uint8_t* data;
    unsigned long size;
};

image tojpeg(uint8_t* buffer, int width, int height) {
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jerr.trace_level = 10;
  jpeg_create_compress(&cinfo);
  uint8_t *jpeg_buffer_raw = NULL;
  unsigned long outbuffer_size = 0;
  jpeg_mem_dest(&cinfo, &jpeg_buffer_raw, &outbuffer_size);

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, 100, true);
  jpeg_start_compress(&cinfo, true);
  int row_stride = width * 3;
  JSAMPROW row_pointer[1];
  int counter = 0;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
//  jpegfile.write(reinterpret_cast<const char*>(jpeg_buffer_raw), outbuffer_size);
  // calling free(jpeg_buffer_raw); or delete[] jpeg_buffer_raw; generates an error
  image img;
  img.data = jpeg_buffer_raw;
  img.size = outbuffer_size;
  return img;
}

void update(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  auto frame = webcam->frame();

  /*  ofstream image;
    image.open("frame.ppm");
    image << "P6\n" << XRES << " " << YRES << " 255\n";
    image.write((char *) frame.data, frame.size);
    image.close(); */

  image img = tojpeg(frame.data, XRES, YRES);
  auto array_buffer = ArrayBuffer::New(isolate, img.data, img.size);
//  auto uint8_array = Uint8Array::New(array_buffer, 0, frame.size);

  args.GetReturnValue().Set(array_buffer);
}

void stop(const FunctionCallbackInfo<Value>& args) {

}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "start", start);
  NODE_SET_METHOD(exports, "update", update);
  NODE_SET_METHOD(exports, "stop", stop);
}

NODE_MODULE(v4l2, init)
