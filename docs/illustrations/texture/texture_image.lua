buffer = Procedural.TextureBuffer(1024)
Procedural.Image(buffer):setFile("red_brick.jpg"):process()
tests:addTextureBuffer(buffer)