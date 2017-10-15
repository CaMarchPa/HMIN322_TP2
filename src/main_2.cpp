#include <iostream>
#include <math.h>
#include "ImageBase.h"

float psnr(ImageBase img_o, ImageBase img_e);

int main(int argc, char const *argv[]) {
    char inImgName[250];
	// char outImgName[250];

	if (argc != 2) {
		printf("ERROR : arguments not complete\n");
		return 1;
	}

	sscanf (argv[1],"%s",inImgName);

	ImageBase img;
	img.load(inImgName);
    int height = img.getHeight();
    int width = img.getWidth();

    int half_height = height / 2;
    int half_width = width / 2;

    ImageBase Y(width, height, false);
    ImageBase Cb(half_width, half_height, false);
    ImageBase Cr(half_width, half_height, false);
    ImageBase Cb_resampled(width, height, false);
    ImageBase Cr_resampled(width, height, false);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Y[j][i] = 0.3 * img[j*3][i*3+0] + 0.6 * img[j*3][i*3+1] + 0.1 * img[j*3][i*3+2];
            if (j < half_height && i < half_width) {
                Cb[j][i] = -0.1687 * img[j*2*3][i*2*3+0] - 0.3313 * img[j*2*3][i*2*3+1] + 0.5 * img[j*2*3][i*2*3+2] + 128;
                Cr[j][i] = 0.5 * img[j*2*3][i*2*3+0] - 0.4187 * img[j*2*3][i*2*3+1] - 0.0813 * img[j*2*3][i*2*3+2] + 128;
            }
        }
    }

    int line;
    int col;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    float p1, p2, p3, p4; /* nearby pixels */

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            tmp = (float) j / (float) (height - 1) * (half_height - 1);
            line = (int) floor(tmp);

            if (line < 0)
                line = 0;
            else
                if (line >= half_height - 1)
                    line = half_height - 2;
            u = tmp - line;
            tmp = (float) (i) / (float) (width - 1) * (half_width - 1);
            col = (int) floor(tmp);
            if ( col < 0)
                col = 0;
            else
                if (col >= half_width - 1)
                    col = half_width - 2;
            t = tmp - col;

            /* Coefficients */
            d1 = (1 - t) * (1 - u);
            d2 = t * (1 - u);
            d3 = t * u;
            d4 = (1 - t) * u;

            /*neighborhood pixels*/
            p1 = Cb[line][col];
            p2 = Cb[line][col + 1];
            p3 = Cb[line + 1][col + 1];
            p4 = Cb[line + 1][col];
            Cb_resampled[j][i] = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;

            /*neighborhood pixels*/
            p1 = Cr[line][col];
            p2 = Cr[line][col + 1];
            p3 = Cr[line + 1][col + 1];
            p4 = Cr[line + 1][col];
            Cr_resampled[j][i] = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;
        }
    }

    // Y.save("../img/Y_prime.pgm");
    Cb.save("../img/Cb_prime.pgm");
    // Cr_resampled.save("../img/Cr_res.pgm");
    // Cr.save("../img/Cr_prime.pgm");
    // Cb_resampled.save("../img/Cb_res.pgm");

    ImageBase out(width, height, img.getColor());

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            out[j*3][i*3+0] = Y[j][i] + 1.402 * (Cr_resampled[j][i] - 128);
            out[j*3][i*3+1] = Y[j][i] - 0.34414 * (Cb_resampled[j][i] - 128) - 0.71414 * (Cr_resampled[j][i] - 128);
            out[j*3][i*3+2] = Y[j][i] + 1.772 * (Cb_resampled[j][i] - 128);
        }
    }
    out.save("../img/out_ycbcr.ppm");
    return 0;
}
