#include <cstddef>
#include "convolution.h"
#include <omp.h>

void convolve(const float *image, float *output, std::size_t n,
              const float *mask, std::size_t m) {

    #pragma omp parallel for
    for (std::size_t x = 0; x < n; x++) {
        for (std::size_t y = 0; y < n; y++) {

            float sum = 0.0f;

            for (std::size_t i = 0; i < m; i++) {
                for (std::size_t j = 0; j < m; j++) {

                    int row = static_cast<int>(x) + static_cast<int>(i)
                            - static_cast<int>((m - 1) / 2);

                    int col = static_cast<int>(y) + static_cast<int>(j)
                            - static_cast<int>((m - 1) / 2);

                    float f;

                    bool row_inside =
                        (row >= 0 && row < static_cast<int>(n));

                    bool col_inside =
                        (col >= 0 && col < static_cast<int>(n));

                    if (row_inside && col_inside) {
                        f = image[row * n + col];
                    }
                    else if (!row_inside && !col_inside) {
                        f = 0.0f;
                    }
                    else {
                        f = 1.0f;
                    }

                    sum += mask[i * m + j] * f;
                }
            }

            output[x * n + y] = sum;
        }
    }
}