#include <CImg.h>
#include <chrono>
#include <iostream>

#define WIDTH 105000
#define HEIGHT 60000
#define ITERATIONS 255
#define TYPE unsigned char

typedef TYPE type;

using namespace std;

int main() {
	cimg_library::CImg<type> mandelbrot(WIDTH, HEIGHT, 1, 1);

	omp_set_num_threads(omp_get_num_procs());

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			double zx = 0, zy = 0, cx = (x / (double)WIDTH) * 3.5 - 2.5, cy = (y / (double)HEIGHT) * 2 - 1;
			type i;
			for (i = 0; i < ITERATIONS && (zx * zx + zy * zy) <= 4; ++i) {
				double xtemp = zx * zx - zy * zy + cx;
				zy = 2 * zx * zy + cy;
				zx = xtemp;
			}
			mandelbrot(x, y, 0) = i;
		}
	}

	cout << chrono::duration_cast<chrono::duration<float>>(chrono::high_resolution_clock::now() - start).count() << " seconds" << endl;

	mandelbrot.save("mandelbrot.png");
	mandelbrot.display();

	return 0;
}
