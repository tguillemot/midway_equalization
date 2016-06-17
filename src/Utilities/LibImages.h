/*
 * Copyright (c) 2013, Marc Lebrun <marc.lebrun.ik@gmail.com>
 * Copyright (c) 2015, Thierry Guillemot
 * <thierry.guillemot.work@gmail.com> All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILITIES_LIBIMAGES_H_
#define UTILITIES_LIBIMAGES_H_

#include <vector>
#include <string>
#include <fftw3.h>

/**
 * @brief Structure containing size informations of an image.
 *
 * @param type : type of color space use by the image;
 * @param width : width of the image;
 * @param height : height of the image;
 * @param nChannels : number of channels in the image;
 * @param wh : equal to width * height. Provided for convenience;
 * @param whc : equal to width * height * nChannels.
 *  Provided for convenience.
 **/
struct ImageSize
{
    size_t     width;
    size_t     height;
    size_t     nChannels;
    size_t     wh;
    size_t     whc;
};

/**
 * @brief Load image, check the number of channels.
 *
 * @param p_name : name of the image to read;
 * @param o_im : vector which will contain the image : R, G and B
 *  concatenated;
 * @param o_imSize : will contain the size of the image;
 * @param p_verbose : if true, print some informations.
 *
 * @return EXIT_SUCCESS if the image has been loaded, EXIT_FAILURE
 *  otherwise.
 **/
int loadImage(
    const char*         p_name,
    std::vector<float> &o_im,
    ImageSize          &o_imSize,
    const bool          p_verbose
    );

/**
 * @brief write image.
 *
 * @param p_name : path+name+extension of the image;
 * @param i_im : vector which contains the image;
 * @param p_imSize : size of the image;
 * @param p_min, p_max : range of data (usually [0, 255]).
 *
 * @return EXIT_SUCCESS if the image has been saved, EXIT_FAILURE
 *  otherwise
 **/
int saveImage(
    char* p_name,
    const std::vector<float>  &i_im,
    const ImageSize           &p_imSize,
    const float                p_min,
    const float                p_max
    );

/**
 * @brief add noise to img.
 *
 * @param i_im : original noise-free image;
 * @param o_imNoisy = im + noise;
 * @param p_sigma : standard deviation of the noise;
 * @param p_verbose : if true, print some informations.
 *
 * @return none.
 **/
void addNoise(
    const std::vector<float> &i_im,
    std::vector<float>       &o_imNoisy,
    const float               p_sigma,
    const bool                p_verbose
    );

/**
 * @brief Compute PSNR and RMSE between i_im1 and i_im2
 *
 * @param i_im1 : pointer to an allocated array of pixels;
 * @param i_im2 : pointer to an allocated array of pixels;
 * @param o_psnr  : will contain the PSNR;
 * @param o_rmse  : will contain the RMSE;
 * @param p_imageName: name of the image;
 * @param p_verbose: if true, print values of PSNR and RMSE.
 *
 * @return EXIT_FAILURE if both images haven't the same size.
 **/
int computePsnr(
    const std::vector<float> &i_im1,
    const std::vector<float> &i_im2,
    float                    &o_psnr,
    float                    &o_rmse,
    const char*               p_imageName,
    const bool                p_verbose
    );

/**
 * @brief Compute the normalized cumulative histogram
 *
 * @param data_ptr: pointer over a channel of the image data
 * @param size: size of the data
 * @param min: minimum value of the data represented by the histogram
 * @param max: maximum value of the data represented by the histogram
 * @param histogram_size: numbers of bins of the histogram
 *
 * @return Vector containing the normalized cumulative histogram of
 *  histogram_size bins
 */
std::vector<float> ComputeNormalizedCumulativeHistogram(
    const float* data_ptr,
    const size_t size,
    const float  min,
    const float  max,
    const size_t histogram_size
    );

/**
 * @brief Compute the normalized histogram
 *
 * @param data_ptr: pointer over a channel of the image data
 * @param size: size of the data
 * @param min: minimum value of the data represented by the histogram
 * @param max: maximum value of the data represented by the histogram
 * @param histogram_size: numbers of bins of the histogram
 *
 * @return Vector containing the normalized histogram of
 *  histogram_size bins
 */
std::vector<float> ComputeNormalizedHistogram(
    const float* data_ptr,
    const size_t size,
    const float  min,
    const float  max,
    const size_t histogram_size
    );

#endif // UTILITIES_LIBIMAGES_H_
