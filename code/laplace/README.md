# `laplace`

Solve the [Laplace's equation](https://en.wikipedia.org/wiki/Laplace%27s_equation).

The (Dirichlet) boundary conditions are described by an input image, with the `-i xxx.ppm` option, in the [PPM format](http://netpbm.sourceforge.net/doc/ppm.html).
In the image, the red channel is used to give the positive values (source: 0 → 0, 255 → 1),
while the blue is used for the negative values (sink: 0 → 0, 255 → -1).
The image should be `width x 4`, where `width` is the choice of the user. The 4 rows of pixels are used in the following way:

1. the first one for the top (north) boundary conditions,
2. the second one for the bottom (south) boundary conditions,
3. the third one for the left (west) boundary conditions, and
4. the fourth (last) one for the right (south) boundary conditions.

Convergence is set with `-t x.xx`, which set the minimum amount of change allowed. `-N xx` sets the maximum number of iteration.

Output, which is a PPM image, is controlled by `-o`. If the option is not provided, output is `output_xxx.ppm`.
Again, red represent the positive values, while blue represent the negative ones.