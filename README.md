<center><h2>Line Integral Convolution on Surfaces (Version 1.00)</h2></center>
<center>
<a href="#LINKS">links</a>
<a href="#EXECUTABLES">executables</a>
<a href="#USAGE">usage</a>
<a href="#LIBRARY">library</a>
<a href="#COMPILATION">compilation</a>
<a href="#CHANGES">changes</a>
<a href="#SUPPORT">support</a>
</center>
<hr>
This software supports line integral convolution on surfaces using anisotropic diffusion. <hr>
<a name="LINKS"><b>LINKS</b></a><br>
<ul>
<b>Papers:</b><BR>
<A HREF="https://www.cs.jhu.edu/~misha/MyPapers/SIG22a.pdf">[Bunge, Herholz, Sorkine-Hornung, Botsch, and Kazhdan, 2022]</A><BR>
<a href="http://www.cs.jhu.edu/~misha/MyPapers/SIG18.pdf">[Prada, Kazhdan, Chuang, and Hoppe, 2018]</a><BR>
<a href="https://ieeexplore.ieee.org/document/856995">[Diewald, Preusser, and Rumpf, 2000]</a>
<BR>
<a href="https://dl.acm.org/doi/10.1145/166117.166151">[Cabral and Leedom, 1993]</a>
</UL>
<!--
<b>Executables: </b>
<a href="TSP.x64.zip">Win64</a><br>
<b>Source Code:</b>
<a href="TSP.Source.zip">ZIP</a>
<a href="https://github.com/mkazhdan/LICOnSurfaces">GitHub</a><br>
<B>Data:</B>
<A HREF="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/TSP.Data.zip">ZIP</A><br>
<b>Older Versions:</b>
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version9.55/">V9.55</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version9.50/">V9.50</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version9.10/">V9.10</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version9.05/">V9.05</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version9.00/">V9.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version8.00/">V8.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version7.00/">V7.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version6.06/">V6.06</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version6.00/">V6.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version5.01/">V5.01</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version5.00/">V5.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.75/">V4.75</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.60/">V4.60</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.50/">V4.50</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.08/">V4.08</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.07/">V4.07</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.06/">V4.06</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.05/">V4.05</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.03/">V4.03</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.02/">V4.02</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.01/">V4.01</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version4.00/">V4.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version3.00/">V3.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version2.00/">V2.00</a>,
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version1.00/">V1.00</a>
</ul>
-->
<hr>
<a name="EXECUTABLES"><b>EXECUTABLES</b></a><br>
<UL>


<DL>
<DETAILS>
<SUMMARY>
<font size="+1"><b>LineIntegralConvolution</b></font>:
Generates per-vertex colors visualizing the streamlines of a vector field. The vector field is obtained by seeding the nodes with random colors, performing anistropic diffusion along the vector-field direction, and then performing gradient-domain sharpening to accentuate the stream-lines.<BR>
</SUMMARY>
<dt><b>--in</b> &lt;<i>input mesh</i>&gt;</dt>
<dd> This mandatory string specifies the name of the file from which the mesh will be read.<BR>
The mesh is assumed to be represented in <A href="http://www.cc.gatech.edu/projects/large_models/ply.html">PLY</A> format, with the vector fields represented in one of two ways:
<UL>
<LI>As per-vertex quantities, encoded with the <CODE>vf_0</CODE>, <CODE>vf_1</CODE>, and <CODE>vf_2</CODE> property names -- corresponding to the <CODE>x</CODE>-, <CODE>y</CODE>-, and <CODE>z</CODE>-components of the vectors.
<LI>As per-face quantities, encoded with the <CODE>vf_0</CODE>, <CODE>vf_1</CODE>, and <CODE>vf_2</CODE> property names -- corresponding to the <CODE>x</CODE>-, <CODE>y</CODE>-, and <CODE>z</CODE>-components of the vectors.
</UL>
<dt>[<b>--out</b> &lt;<i>output mesh</i>&gt;]
<dd> This strings specifies the the name of the file to which the mesh will be written.<BR>
The mesh is assumed to be represented in <A href="http://www.cc.gatech.edu/projects/large_models/ply.html">PLY</A> format, with colors encoded using the <CODE>red</CODE>, <CODE>green</CODE>, and <CODE>blue</CODE> property names.
<dt>[<b>--degree</b> &lt;<i>Lagrange interpolant degree</i>&gt;]
<dd> This integer specifies the the degree of the Lagrange interpolant used for performing the anisotropic diffusion<BR>
While color values are output per-vertex, using a higher degree interpolant generates finer feature lines.<BR>
Degrees <CODE>1</CODE>, <CODE>2</CODE>, and <CODE>3</CODE> are supported.<BR>
The default value for this argument is <CODE>2</CODE>.
<dt>[<b>--sharpen</b> &lt;<i>gradient modulation weight</i>&gt;]
<dd> This floating point value specifies the scale applied to the gradients when performing sharpening.<BR>
The default value for this argument is <CODE>1024</CODE>.
</DETAILS>
</DL>

</UL>


<hr>
<a name="USAGE"><b>USAGE EXAMPLES (WITH SAMPLE DATA)</b></a><br>
For testing purposes, a number of <A HREF="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/TSP.Data.zip">textured mapped models</A> are provided (using the <U>.ply</U> extension).
Of these, <I>David</I> and <I>Julius</I> include normal maps (using the <U>.normap</U> extension), <I>Fertility</I> includes the eight harmonic vector-fields (using the <U>.vf</U> extension), and <I>Rooster</I> uses (partial) texture maps as well as a mask image and confidence maps.

<ul>

<dl>
<details>
<summary>
<font size="+1"><b>TextureFiltering</b></font>
</summary>
To run this executable you must specify the input mesh as well as the texture itself:
<blockquote><code>% Bin/*/TextureFiltering --in ../TSP.Data/David/david.ply ../TSP.Data/David/david.normap</code></blockquote>
This opens a viewer allowing the user to prescribe both global gradient modulation weights (through the slider) and local modulation weights (through a paint-brush interface, by depressing the [SHIFT] key and dragging with the left mouse button to smooth and the right mouse button to sharpen).<BR>
You can also bypass the viewer and output a globally sharpened/smoothed texture to a file:
<blockquote><code>% Bin/*/TextureFiltering --in ../TSP.Data/Julius/julius.ply ../TSP.Data/Julius/julius.normap --out julius.smooth.normap --modulation 0 --interpolation 100</code></blockquote>
Here a modulation weight less than 1 indicates that gradients should be dampened (resulting in smoothing) and a small interpolation weight reduces the interpolation penalty, exaggerating the smoothing.
</details>
</dl>

<dl>
<details>
<summary>
<font size="+1"><b>TextureStitching</b></font>
</summary>
This viewer can be run in one of two modes:
<OL>
<LI>
In addition to the input mesh, specify a (single) composite texture and mask.
If adjacent texels share the same mask color, they are assumed to come from the same source, and the gradient between them is preserved.
Otherwise, the gradient is set to zero. Additionally, a mask color of black is reserved to indicate that the texel value is unknown.<BR>
For example, running
<blockquote><code>% Bin/*/TextureFiltering --in Rooster/rooster.ply ../TSP.Data/Rooster/texels.png --mask ../TSP.Data/Rooster/mask.png</code></blockquote>
opens a viewer showing the stitched texture on the left and the composite texture on the right.
<LI>
In addition to the input mesh, specify (multiple) partial textures and associated confidence maps.
The code blends the gradients in regions of overlap, with weights determined by the mask.
Texel and confidence file names are specified using integer format specifiers, with zero-indexing.
Colors are transformed to scalar confidence values by computing the gray-scale value and normalizing to the range [0,1].<br>
For example, running
<blockquote><code>% Bin/*/TextureFiltering --in Rooster/rooster.ply ../TSP.Data/Rooster/texels-%02d.png --mask ../TSP.Data/Rooster/mask-%02d.png --multi</code></blockquote>
opens a viewer showing the stitched texture on the left and the first partial textures on the right.<BR>
Pressing the 't' key toggles forward through the partial textures and pressing 'T' toggles backwards.<BR>
Holding [SHIFT] and clicking on the stitched model replaces the blended gradients under the paint-brush with the gradients from the currently visualized partial-texture.<BR>
</OL>
You can also bypass the viewer and output the stitched texture to a file:
<blockquote><code>% Bin/*/TextureStitching --in Rooster/rooster.ply ../TSP.Data/Rooster/texels-%02d.png --mask ../TSP.Data/Rooster/mask-%02d.png --multi --out stitched.png</code></blockquote>
</details>
</dl>


<dl>
<details>
<summary>
<font size="+1"><b>LineIntegralConvolution</b></font>
</summary>
To run this executable you must specify the input mesh:
<blockquote><code>% Bin/*/LineIntegralConvolution --in ../TSP.Data/Fertility/fertility.ply</code></blockquote>
This opens a viewer visualizing a vector-field by performing anisotropic diffusion to simulate line-integral-convolution. (To start the iterative solver, press the [SPACE] key.) By default, the vector-field used is defined by the (maximal) principal curvature directions.<BR>
You can also explicitly prescribe the vector-field:
<blockquote><code>% Bin/*/LineIntegralConvolution --in ../TSP.Data/Fertility/fertility.ply --inVF ../TSP.Data/Fertility/harmonic-001.vf --intrinsicVF</code></blockquote>
(The <b>--intrinsicVF</b> flag is required because the vector-field in the file is represented using two intrinsic coordinates per triangle instead of three extrinsic ones.)<BR>
You can also bypass the viewer and output the line-integral-convolution texture to a file:
<blockquote><code>% Bin/*/LineIntegralConvolution --in ../TSP.Data/Hand/hand.ply --minimal --out hand.minimal.jpg</code></blockquote>
Here a visualization of the minimal principal curvature directions is written out as a texture image.
</details>
</dl>

<dl>
<details>
<summary>
<font size="+1"><b>Geodesics</b></font>
</summary>
To run this executable you must specify the input mesh:
<blockquote><code>% Bin/*/Geodesics --in ../TSP.Data/Bunny/bunny.ply</code></blockquote>
This opens a viewer allowing the user to prescribe the source of the geodesic by holding the [SHIFT] button and clicking on the source location with either mouse button.
</details>
</dl>


<dl>
<details>
<summary>
<font size="+1"><b>ReactionDiffusion</b></font>
</summary>
To run this executable you must specify the input mesh:
<blockquote><code>% Bin/*/ReactionDiffusion --in ../TSP.Data/Camel/camel.ply</code></blockquote>
This opens a viewer visualizing the "stripes" reaction-diffusion process. (To start the process, press the [SPACE] key.)<BR>
You can also bypass the viewer and output the reaction-diffusion texture to a file:
<blockquote><code>% Bin/*/ReactionDiffusion --in ../TSP.Data/David/david.ply --out david.dots.jpg --dots --outSteps 2000</code></blockquote>
Here a "dots" pattern is written out to an image. (Empirically, we have found that this reaction-diffusion process takes more steps to converge, hence the larger number of steps.)
</details>
</dl>

</ul>


<hr>
<details>
<summary>
<a name="COMPILATION"><b>COMPILATION AND EXECUTION</b></a><br>
</summary>
<UL>
<LI>The Windows executables require both the <B>glew</B> and <B>glut</B> dynamically linked libraries to run. These can be found <A HREF="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/TSP.DLLs.zip">here</A> and should be included either in the directory with the executables, or in the directory from which the executables are run.</LI>
<LI>Compiling under Windows requires both the <B>glew</B> and <B>glut</B> libraries. These can be found <A HREF="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/TSP.LIBs.zip">here</A> and should be placed in the output directory for linkage.</LI></LI>
<LI>Compilation requires a linear solver. By default, we use the <CODE>LDLt</CODE> implementation provided by  <A HREF="https://eigen.tuxfamily.org/">Eigen</A>. If you have <A HREF="https://www.intel.com/content/www/us/en/docs/oneapi/programming-guide/2024-1/intel-oneapi-math-kernel-library-onemkl.html">Intel's oneMKL</A>, we encourage you to use Eigen's <CODE>Pardiso</CODE> implementation. To to this you will need to enable the <CODE>USE_EIGEN_PARDISO</CODE> flag in <CODE>include/Src/PreProcessing.h</CODE>
</UL>
</details>

<hr>
<details>
<summary>
<a name="CHANGES"><b>HISTORY OF CHANGES</b></a><br>
</summary>

<!--
<a href="http://www.cs.jhu.edu/~misha/Code/TextureSignalProcessing/Version2.00/">Version 2.00</a>:
<ul>
<li> Added support for reaction-diffusion based on the Gray-Scott model.
</ul>
-->



</details>
