buffer = Procedural.TextureBuffer(1024)
Procedural.Noise(buffer):setType(Procedural.Noise_NOISE_PERLIN):process()
tests:addTextureBuffer(buffer)