buffer = Procedural.TextureBuffer(1024)
Procedural.Solid(buffer):setColour(Procedural.ColourValue(0.0, 0.5, 1, 1)):process()
tests:addTextureBuffer(buffer)