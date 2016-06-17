/*
 * Copyright (c) 2015, Thierry Guillemot <thierry.guillemot.work@gmail.com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Midway/midway.h"
#include "Utilities/LibImages.h"

using namespace std;

/**
 * @brief print usage function
 */
void PrintUsage()
{
  cerr << "usage : in-1.png in-2.png out-1.png out-2.png [options]" << endl
       << "options :" << endl
       << "-dithering sigma : add a gaussian noise of variance sigma before computing midway" << endl
       << "-verbose : activate verbose mode" << endl;
}

/**
 * @brief main function call
 */
int main(int argc, char *const *argv)
{
  std::vector<float> im_1;           
  std::vector<float> im_2;           
  ImageSize          imSize1;       
  ImageSize          imSize2;      
  float              sigma_dithering = .0f;
  size_t             image_dynamic   = 8;
  bool               verbose         = false; 
  
  //! Parsing and analysing the function arguments
  if( argc<5)
  {
    PrintUsage();
    return EXIT_FAILURE;
  }
  
  for (int pos = 5; pos < argc; ++pos)
  {
    if (strcmp(argv[pos], "-dithering") == 0)
    {
      if (++pos == argc)
      {
        cerr << "Wrong dithering value" << endl;
        PrintUsage();
        return EXIT_FAILURE;
      }
      sigma_dithering = atof(argv[pos]);
    }
    else if (strcmp(argv[pos], "-verbose") == 0)
    {
      cout << argv[pos] << endl;
      verbose = true;
    }
    else
    {
      PrintUsage();
      return EXIT_FAILURE;
    }
  }

  //! Load images 
  if (loadImage(argv[1], im_1, imSize1, verbose) != EXIT_SUCCESS)
  {
    cerr << "Failed reading image " << argv[1] << endl;
    return EXIT_FAILURE;
  }
  if (loadImage(argv[2], im_2, imSize2, verbose) != EXIT_SUCCESS)
  {
    cerr << "Failed reading image " << argv[2] << endl;
    return EXIT_FAILURE;
  }

  //! Apply dithering
  if (sigma_dithering)
  {
    addNoise(im_1, im_1, sigma_dithering, verbose);
    addNoise(im_2, im_2, sigma_dithering, verbose);
  }
  
  //! Launch Midway equalization
  if (Launch(im_1, im_2, imSize1, imSize2, 1<<image_dynamic, verbose,
             &im_1, &im_2) != EXIT_SUCCESS)
  {
    cerr << "Failed applying midway equalization" << endl;
    return EXIT_FAILURE;
  }

  //! Save images
  if (saveImage(argv[3], im_1, imSize1, 0.f, 255.f) != EXIT_SUCCESS)
  {
    cerr << "Failed writing image " << argv[3] << endl;
    return EXIT_FAILURE;
  }
  if (saveImage(argv[4], im_2, imSize2, 0.f, 255.f) != EXIT_SUCCESS)
  {
    cerr << "Failed writing image " << argv[4] << endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
