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

![Figure 2. Cloud](texture_cloud.png)

See Procedural::Cloud

### Gradient

![Figure 3. Gradient](texture_gradient.png)

See Procedural::Gradient

### Image

![Figure 4. Image](texture_image.png)

See Procedural::Image

### Labyrinth

![Figure 5. Labyrinth](texture_labyrinth.png)

See Procedural::Labyrinth

### Marble

![Figure 6. Marble](texture_marble.png)

See Procedural::Marble

### Noise

<table border="0" width="100%"><tr><td>\image html texture_noise_white.png "Figure 7a. White noise"</td><td>\image html texture_noise_perlin.png "Figure 7b. Perlin noise"</td></tr></table>

See Procedural::Noise

### Solid

![Figure 8. Solid](texture_solid.png)

See Procedural::Solid

### Textile

![Figure 9. Textile](texture_textile.png)

See Procedural::Textile

### Wood

![Figure 10. Wood](texture_wood.png)

See Procedural::Wood

## Manipulation

### Abnormals

![Figure 1. Abnormals](texture_abnormals.png)

See Procedural::Abnormals

### Alpha

![Figure 2. Alpha](texture_alpha.png)

See Procedural::Alpha

### AlphaMask

![Figure 3. AlphaMask](texture_alphamask.png)

See Procedural::AlphaMask

### Blur

<table border="0" width="100%"><tr><td>\image html texture_blur_1.png "Figure 4a. Blur: Mean"</td><td>\image html texture_blur_2.png "Figure 4b. Blur: Gauss"</td></tr></table>

See Procedural::Blur

### Channel

<table border="0" width="100%"><tr><td>\image html texture_channel_1.png "Figure 5a. Channel: blue"</td><td>\image html texture_channel_2.png "Figure 5b. Channel: gray"</td></tr></table>

See Procedural::Channel

### Colours

![Figure 6. Colours](texture_colours.png)

See Procedural::Colours

### Combine

![Figure 7. Combine](texture_combine.png)

See Procedural::Combine

### Convolution

![Figure 8. Convolution](texture_convolution.png)

See Procedural::Convolution

### Crack

![Figure 9. Crack](texture_crack.png)

See Procedural::Crack

### Cycloid

<table border="0" width="100%"><tr><td>\image html texture_cycloid_hypocycloid.png "Figure 10a. Cycloid: Hypocycloid"</td><td>\image html texture_cycloid_hypotrochoid.png "Figure 10b. Cycloid: Hypotrochoid"</td></tr>
<tr><td>\image html texture_cycloid_epicycloid.png "Figure 10c. Cycloid: Epicycloid"</td><td>\image html texture_cycloid_epitrochoid.png "Figure 10d. Cycloid: Epitrochoid"</td></tr>
<tr><td>\image html texture_cycloid_rose.png "Figure 10e. Cycloid: Rose curve"</td><td>\image html texture_cycloid_lissajous.png "Figure 10f. Cycloid: Lissajous curve"</td></tr></table>

See Procedural::Cycloid

### Dilate

![Figure 11. Dilate](texture_dilate.png)

See Procedural::Dilate

### Distort

![Figure 12. Distort](texture_distort.png)

See Procedural::Distort

### EdgeDetection

![Figure 13. EdgeDetection](texture_edgedetection.png)

See Procedural::EdgeDetection

### Flip

<table border="0" width="100%"><tr><td>\image html texture_flip_1.png "Figure 14a. Flip: point"</td><td>\image html texture_flip_2.png "Figure 14b. Flip: vertical"</td><td>\image html texture_flip_3.png "Figure 14c. Flip: horizontal"</td></tr></table>

See Procedural::Flip

### Glow

![Figure 15. Glow](texture_glow.png)
See Procedural::Glow

### Invert

![Figure 16. Invert](texture_invert.png)

See Procedural::Invert

### Jitter

![Figure 17. Jitter](texture_jitter.png)

See Procedural::Jitter

### Lerp

![Figure 18. Lerp](texture_lerp.png)

See Procedural::Lerp

### Light

![Figure 19. Light](texture_light.png)

See Procedural::Light

### Lookup

![Figure 20. Lookup](texture_lookup.png)

See Procedural::Lookup
### Normals

![Figure 21. Normals](texture_normals.png)

See Procedural::Normals

### OilPaint

![Figure 22. OilPaint](texture_oilpaint.png)

See Procedural::OilPaint

### RandomPixels

![Figure 23. RandomPixels](texture_randompixels.png)

See Procedural::RandomPixels

### RectangleTexture

![Figure 24. RectangleTexture](texture_rectangle.png)

See Procedural::RectangleTexture

### RotationZoom

![Figure 25. RotationZoom](texture_rotationzoom.png)

See Procedural::RotationZoom

### Segment

![Figure 26. Segment](texture_segment.png)

See Procedural::Segment

### Sharpen

<table border="0" width="100%"><tr><td>\image html texture_sharpen_1.png "Figure 27a. Sharpen: basic"</td><td>\image html texture_sharpen_2.png "Figure 27b. Sharpen: gaussian"</td></tr></table>

See Procedural::Sharpen

### Threshold

![Figure 28. Threshold](texture_threshold.png)

See Procedural::Threshold

### Vortex

![Figure 29. Vortex](texture_vortex.png)

See Procedural::Vortex

## Example

The Sample_Material project demonstrates how to use various texture generators and modifiers to build a texture, with it's associated normal map.

Here is a diagram of how the final texture is obtained:

\dotfile texture_01.gv

And here is the code to build it :

\include Material.cpp