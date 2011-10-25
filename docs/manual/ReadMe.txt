The manual is generated using asciidoc customised according to steve streeting's blog article.

Steps needed to build the manual :
- Unzip the Doc_Toolchain zip file (available from the website)
- Fix the hardcoded file paths in CatalogManager.properties and fo_steve.xsl
- Run the illustrations project to create all image files
- Copy the generated image files in the same folder as manual.txt
- Open a console
- Put yourself in the Doc_Toolchain directory
- run "build path/to/your/manual" (don't write the ".txt")