buffer = Procedural.TextureBuffer(1024)
Procedural.Cell(buffer):setDensity(4):process()
tests:addTextureBuffer(buffer)