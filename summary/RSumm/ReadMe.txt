---------------------------------
|								|
|			RSumm				|
|								|
---------------------------------

* Installation

 -> Linux:
	- $apt-get install cmake
	- $cd directory_of_RSumm/build
	- $cmake ..
	- $make

 -> Windows:

	/* Nothing yet */

 -> MacOS:

	/* Nothing yet */

-----------------------------------------------------------------
* Running:

 -> Linux:
	- $cd directory_of_RSumm/bin
	- $./Summ -path dc1 [dc2 dc3 ...] cr -cst cstrel -pond/unit

	Where:
		-> path = denso/profundo/segmentado
		-> text(i) = directory of document i [1; *]
		-> cr = compression rate [0.00; 1.00]
		-> cst = activate cst
		-> if cst is activated:
			+ cstrel = directory of the document containing the cst relations for the texts
			+ tocst = ponderada/unitaria

	Obs.: Examples of all the documents previously quoted are in the directory: directory_of_RSumm/Examples

 -> Windows:

	/* Nothing yet */

 -> MacOS:

	/* Nothing yet */

-----------------------------------------------------------------
