buffer = Procedural.TextureBuffer(128)
Procedural.Cloud(buffer):process()
Procedural.Crack(buffer):setParameterImage(bufferGradient):process()
tests:addTextureBuffer(buffer)
dotfile = tests:getDotFile("texture_10", "Crack_Demo")
dotfile:set("Cloud", "texture_cloud", "Gradient", "texture_gradient", "Crack", "texture_crack")
