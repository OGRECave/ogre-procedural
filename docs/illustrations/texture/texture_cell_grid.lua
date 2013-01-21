buffer = Procedural.TextureBuffer(1024)
Procedural.Cell(buffer):setDensity(4):setMode(Procedural.Cell_MODE_GRID):setPattern(Procedural.Cell_PATTERN_CROSS):process()
tests:addTextureBuffer(buffer)