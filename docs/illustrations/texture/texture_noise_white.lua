buffer = Procedural.TextureBuffer(1024)
Procedural.Noise(buffer):setType(Procedural.Noise_NOISE_WHITE):process()
tests:addTextureBuffer(buffer)