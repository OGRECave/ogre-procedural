tb = Procedural.SphereGenerator():setNumRings(6):setNumSegments(12):buildTriangleBuffer()
Procedural.UnweldVerticesModifier():setInputTriangleBuffer(tb):modify()
Procedural.CalculateNormalsModifier():setInputTriangleBuffer(tb):setComputeMode(Procedural.CalculateNormalsModifier_NCM_TRIANGLE):modify()
tests:addTriangleBuffer(tb)