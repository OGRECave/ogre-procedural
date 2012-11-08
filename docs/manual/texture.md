Textures {#textures}
========

It is really easy to create synthetic textures. There are two kinds of "texture filters":
- _Rendering filters_ which are drawing new images
- _Modification filters_ which are manipulating existing images

[TOC]

## Renderer

### Cell

<table border="0" cellspacing="20"><tr><td>\image html texture_cell_default.png "Figure 1a. Default cells"</td><td>\image html texture_cell_chess.png "Figure 1b. Cells: cone + chess"</td><td>\image html texture_cell_grid.png "Figure 1c. Cells: cross + grid"</td></tr></table>

See Procedural::Cell

### Cloud

![Cloud](texture_cloud.png)

See Procedural::Cloud

### Gradient

![Gradient](texture_gradient.png)

See Procedural::Gradient

### Image

![Image](texture_image.png)

See Procedural::Image

### Labyrinth

![Labyrinth](texture_labyrinth.png)

See Procedural::Labyrinth

### Marble

![Marble](texture_marble.png)

See Procedural::Marble

### Noise

<table border="0" width="100%"><tr><td>\image html texture_noise_white.png "Figure 7a. White noise"</td><td>\image html texture_noise_perlin.png "Figure 7b. Perlin noise"</td></tr></table>

See Procedural::Noise

### Solid

![Solid](texture_solid.png)

See Procedural::Solid

### Textile

![Textile](texture_textile.png)

See Procedural::Textile

### Wood

![Wood](texture_wood.png)

See Procedural::Wood

## Manipulation

### Abnormals

![Abnormals](texture_abnormals.png)

See Procedural::Abnormals

### Alpha

![Alpha](texture_alpha.png)

See Procedural::Alpha

### AlphaMask

![AlphaMask](texture_alphamask.png)

See Procedural::AlphaMask

### Blur

<table border="0" width="100%"><tr><td>\image html texture_blur_1.png "Figure 4a. Blur: Mean"</td><td>\image html texture_blur_2.png "Figure 4b. Blur: Gauss"</td></tr></table>

See Procedural::Blur

### Channel

<table border="0" width="100%"><tr><td>\image html texture_channel_1.png "Figure 5a. Channel: blue"</td><td>\image html texture_channel_2.png "Figure 5b. Channel: gray"</td></tr></table>

See Procedural::Channel

### Colours

![Colours](texture_colours.png)

See Procedural::Colours

### Combine

![Combine](texture_combine.png)

See Procedural::Combine

### Convolution

![Convolution](texture_convolution.png)

See Procedural::Convolution

### Crack

![Crack](texture_crack.png)

See Procedural::Crack

### Dilate

![Dilate](texture_dilate.png)

See Procedural::Dilate

### Distort

![Distort](texture_distort.png)

See Procedural::Distort

### EdgeDetection

![EdgeDetection](texture_edgedetection.png)

See Procedural::EdgeDetection

### Flip

<table border="0" width="100%"><tr><td>\image html texture_flip_1.png "Figure 13a. Flip: point"</td><td>\image html texture_flip_2.png "Figure 13b. Flip: vertical"</td><td>\image html texture_flip_3.png "Figure 13b. Flip: horizontal"</td></tr></table>

See Procedural::Flip

### Glow

![Glow](texture_glow.png)
See Procedural::Glow

### Invert

![Invert](texture_invert.png)

See Procedural::Invert

### Jitter

![Jitter](texture_jitter.png)

See Procedural::Jitter

### Lerp

![Lerp](texture_lerp.png)

See Procedural::Lerp

### Light

![Light](texture_light.png)

See Procedural::Light

### Lookup

![Lookup](texture_lookup.png)

See Procedural::Lookup
### Normals

![Normals](texture_normals.png)

See Procedural::Normals

### OilPaint

![OilPaint](texture_oilpaint.png)

See Procedural::OilPaint

### RandomPixels

![RandomPixels](texture_randompixels.png)

See Procedural::RandomPixels

### Rectangle

![Rectangle](texture_rectangle.png)

See Procedural::Rectangle

### RotationZoom

![RotationZoom](texture_rotationzoom.png)

See Procedural::RotationZoom

### Segment

![Segment](texture_segment.png)

See Procedural::Segment

### Sharpen

<table border="0" width="100%"><tr><td>\image html texture_sharpen_1.png "Figure 26a. Sharpen: basic"</td><td>\image html texture_sharpen_2.png "Figure 26b. Sharpen: gaussian"</td></tr></table>

See Procedural::Sharpen

### Threshold

![Threshold](texture_threshold.png)

See Procedural::Threshold

### Vortex

![Vortex](texture_vortex.png)

See Procedural::Vortex

## Example

The Sample_Material project demonstrates how to use various texture generators and modifiers to build a texture, with it's associated normal map.

Here is a diagram of how the final texture is obtained:

\dotfile texture_01.gv

And here is the code to build it :

\include Material.cpp