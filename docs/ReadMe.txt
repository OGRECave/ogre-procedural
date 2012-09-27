How to build the docs
---------------------

1. Install doxygen, graphviz, python, xlstproc and add their bin directory to the system PATH
2. Install asciidoc and set ASCIIDOC_DIR in CMake
3. Build the illustrations project to create illustrations images, which will be used in the manual and doxygen docs
4. Build the doc target to create the doxygen api docs
5. Build the manual target to create the manual
