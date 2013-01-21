buffer = Procedural.TextureBuffer(1024)
Procedural.Gradient(buffer):setColours(Procedural.ColourValue_Black, Procedural.ColourValue_Red, Procedural.ColourValue_Green, Procedural.ColourValue_Blue):process()
tests:addTextureBuffer(buffer)
	