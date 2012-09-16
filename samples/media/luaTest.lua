tests = Procedural.LuaTests_getInstance()
Procedural.SphereGenerator():setNumRings(8):setRadius(4.0):realizeMesh("sphere")
tests:addMesh("sphere")