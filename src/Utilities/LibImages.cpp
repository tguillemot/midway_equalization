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

/**
 * @file LibImages.cpp
 * @brief Usefull functions on images
 *
 * @author Marc Lebrun <marc.lebrun.ik@gmail.com>
 **/

#include "LibImages.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "io_png.h"
#include "mt19937ar.h"
#include "utils.h"

using namespace std;

int loadImage(
    const char*    p_name,
    vector<float> &o_im,
    ImageSize     &o_imSize,
    const bool     p_verbose)
{
  //! read input image
  if (p_verbose)
  {
    cout << endl << "Read input image '" << p_name << "'...";
  }

  float *imTmp = NULL;
  size_t w, h, c;

  imTmp = read_png_f32(p_name, &w, &h, &c);
  if (!imTmp)
  {
    cout << "error :: " << p_name << " not found or not a correct png image" << endl;
    return EXIT_FAILURE;
  }
  
  if (p_verbose)
  {
    cout << "done." << endl;
  }

  //! test if image is really a color image and exclude the alpha channel
  if (c > 2)
  {
    unsigned k = 0;
    while (k < w * h && imTmp[k] == imTmp[w * h + k] && imTmp[k] == imTmp[2 * w * h + k])
    {
      k++;
    }
    c = (k == w * h ? 1 : 3);
  }

  //! Some image informations
  if (p_verbose)
  {
    cout << "image size :" << endl;
    cout << " - width          = " << w << endl;
    cout << " - height         = " << h << endl;
    cout << " - nb of channels = " << c << endl;
  }

  //! Initializations
  o_imSize.width      = w;
  o_imSize.height     = h;
  o_imSize.nChannels  = c;
  o_imSize.wh         = w * h;
  o_imSize.whc        = w * h * c;
  o_im.resize(w * h * c);
  for (unsigned k = 0; k < w * h * c; k++)
    o_im[k] = imTmp[k];

  //! Free Memory
  delete[] imTmp;

  return EXIT_SUCCESS;
}

int saveImage(
    char*                p_name,
    const vector<float> &i_im,
    const ImageSize     &p_imSize,
    const float          p_min,
    const float          p_max)
{
  //! Allocate Memory
  float* imTmp = new float[p_imSize.whc];

  //! Check for boundary problems
  for (unsigned k = 0; k < p_imSize.whc; k++)
  {
    imTmp[k] = i_im[k] < p_min ? p_min : (i_im[k] > p_max ? p_max : i_im[k]);
  }

  if (write_png_f32(p_name, imTmp, p_imSize.width, p_imSize.height,
                    p_imSize.nChannels))
  {
    cout << "... failed to save png image :'" << p_name << "'" << endl;
    return EXIT_FAILURE;
  }

  //! Free Memory
  delete[] imTmp;

  return EXIT_SUCCESS;
}

void addNoise(
    const vector<float> &i_im,
    vector<float>       &o_imNoisy,
    const float          p_sigma,
    const bool           p_verbose)
{
  if (p_verbose)
  {
    cout << "Add noise [sigma = " << p_sigma << "] ...";
  }

  //! Initialization
  o_imNoisy = i_im;
  mt_init_genrand((unsigned long int) time (NULL) + (unsigned long int) getpid());

  //! Add noise
  for (unsigned k = 0; k < i_im.size(); ++k)
  {
    const double a = mt_genrand_res53();
    const double b = mt_genrand_res53();

    o_imNoisy[k] += p_sigma * (float) (sqrtl(-2.0l * log(a)) * cos(2.0l * M_PI * b));
    o_imNoisy[k] = Crop(o_imNoisy[k], 0.f, 255.f); 
  }

  if (p_verbose)
  {
    cout << "done." << endl;
  }
}

int computePsnr(
    const vector<float> &i_im1,
    const vector<float> &i_im2,
    float               &o_psnr,
    float               &o_rmse,
    const char*          p_imageName,
    const bool           p_verbose)
{
  if (i_im1.size() != i_im2.size())
  {
    cout << "Can't compute PSNR & RMSE: images have different sizes: " << endl;
    cout << "i_im1 : " << i_im1.size() << endl;
    cout << "i_im2 : " << i_im2.size() << endl;
    return EXIT_FAILURE;
  }

  float sum = 0.f;
  for (unsigned k = 0; k < i_im1.size(); k++)
    sum += (i_im1[k] - i_im2[k]) * (i_im1[k] - i_im2[k]);

  o_rmse = sqrtf(sum / (float) i_im1.size());
  o_psnr = 20.f * log10f(255.f / o_rmse);

  if (p_verbose)
  {
    cout << p_imageName << endl;
    cout << "PSNR = " << o_psnr << endl;
    cout << "RMSE = " << o_rmse << endl;
  }

  return EXIT_SUCCESS;
}

vector<float> ComputeNormalizedCumulativeHistogram(
    const float  *data_ptr,
    const size_t  size,
    const float   min,
    const float   max,
    const size_t  histogram_size)
{
  //! Initialization of the histogram with 0 
  vector<float> histogram(histogram_size, 0);
  size_t size_max = histogram_size - 1; 
  
  //! Computation of the histogram
  for(size_t pos=0; pos<size; ++pos)
  {
    ++histogram[(size_t) ( size_max*(data_ptr[pos]- min)/(max - min))];
  }
  
  //! Normalization of the cumulative histogram
  for(size_t pos=0; pos<histogram_size-1; ++pos)
  {
    histogram[pos+1] += histogram[pos];
    histogram[pos]   /= size;
  }
  histogram[histogram_size-1] /= size;

  return histogram;
}

vector<float> ComputeNormalizedHistogram(
    const float  *data_ptr,
    const size_t  size,
    const float   min,
    const float   max,
    const size_t  histogram_size)
{
  //! Initialization of the histogram with 0 
  vector<float> histogram(histogram_size, 0);
  size_t size_max = histogram_size - 1; 
  
  //! Computation of the histogram
  for(size_t pos=0; pos<size; ++pos)
  {
    ++histogram[(size_t) ( size_max*(data_ptr[pos]- min)/(max - min))];
  }
  
  //! Normalization of the cumulative histogram
  for(size_t pos=0; pos<histogram_size; ++pos)
  {
    histogram[pos]   /= size;
  }

  return histogram;
}
