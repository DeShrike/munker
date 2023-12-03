#ifndef _PNGHELPER_H_
#define _PNGHELPER_H_

#define __unused __attribute__((unused))

int write_png_file(const char* filename, const char* title, int width, int height, uint32_t* pixels);

#endif // _PNGHELPER_H_
