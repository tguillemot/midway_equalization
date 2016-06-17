# About
- Author    :: Thierry Guillemot <thierry.guillemot.work@gmail.com> and Julie Delon <julie.delon@parisdescartes.fr>
- Copyright :: (C) 2015 IPOL Image Processing On Line http://www.ipol.im/
- Licence   :: GPL V3+
- IPOL paper :: http://www.ipol.im/pub/pre/140/

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
# Overview
This source code provides an implementation of the "midway" image
equalization, as described in IPOL.

The 'bin/midway' program reads two PNG images, then apply contrast
changes to these images such that the output images have the same
distributions (this is done channel by channel).  This common
distribution is built as the midway between the two original ones.
If H1 and H2 are the cumulative histograms of the input images (for
the considered channel), the midway histogram is defined as
(0.5*(H1^{-1} + H2^{-1}))^{-1}. The contrast changes to be applied
are thus respectively x -> 0.5*(x + H2^{-1} o H1 (x)) for the first
image and x -> 0.5*(x + H1^{-1} o H2 (x)) for the second image. Both
resulting images are written as PNG images.

Only 8bit GRAY/RGB PNG images are handled.
# Requirement
The code is written in ANSI C, and should compile on any system with
an ANSI C compiler.

The libpng header and libraries are required on the system for
compilation and execution. On Linux, just use your package manager
to install it:
```
sudo apt-get install libpng
```

For more information, see http://www.libpng.org/pub/png/libpng.html

# Compilation
Simply use the provided makefile, with the command 'make'.  The
makefile will produce a program called : 'bin/midway'.

The 'midway' program is used to compute the midway equalization
of two images.

# Usage
## Midway
### Description
The 'bin/midway' program is used to apply the midway equalization over
two images. It takes 4 parameters with other optional:
```
bin/midway in1.png in2.png out1.png out2.png [options]
```
- in1.png  :: first input image.
- in2.png  :: second input image.
- out1.png :: first output image.
- out2.png :: second output image.

Options :
- -dithering sigma :: apply a Gaussian noise of standard deviation
   sigma before applying the midway equalization.
- -verbose :: activate the verbose mode

### Usage examples
If you want to compute the midway equalization between the files
'input/bat1.png' and 'input/bat2.png' without dithering and save the
resulting images into 'out1.png' and 'out2.png', you can use the
command:
```
bin/midway input/bat1.png input/bat2.png out1.png out2.png
```

The command:
```
bin/midway input/bat1.png input/bat2.png out1.png out2.png -dithering 2
```
will compute the midway equalization of the files
'input/bat1.png' and 'input/bat2.png' with a dithering of
sigma=2 and save the resulting images into 'out1.png' and
'out2.png'.

# Bugs reports
You can report any bug with the github interface:
https://github.com/tguillemot/parigi
