## Installation

### Prerequisites

The following software is required:

- Boost Libraries
- Armadillo Library (Numerical linear algebra in c++)
- SWIG    (Python Interface)
- Doxygen (Documentation for the C++ code)
- Sphinx  (HTML Manual)
  - The **breathe** module that bridges sphinx and doxygen should be installed (e.g., "pip install breathe")
  - The **bibtex** module should be installed (e.g., "pip install sphinxcontrib-bibtex")
- MPI (Estimation with parallel Markov chains)

### Generate makefiles with CMake

Out-of-source is the suggested build modality. To do this, first create a build folder (for example "tulipBin") as a sibling of the main source code folder (probably "gcode")

~~~
mkdir tulipBin
~~~

Cd into tulipBin

~~~
cd tulipBin
~~~

Now you can use the ccmake (command line) or cmake-gui (Qt-based CMake GUI) to create the make files.
For example, using ccmake from the tulipBin folder type

~~~
ccmake ../tulip
~~~

After running a first configuration step (by typing "c" on ccmake), four options will be shown that enable the creation of the Python interface and documentation. These are

~~~
buildPythonInterface (default ON)
buildDocumentation   (default ON)
USE_SYSTEM_DOXYGEN   (default OFF)
USE_SYSTEM_SWIG      (default OFF)
~~~

If you only want to compile the C++ code then the options "buildPythonInterface" and "buildDocumentation" should be switched off. 
If you want to build the C++ code and Python wrapper but not the Doxygen+Sphinx documentation then set

~~~
buildPythonInterface ON
buildDocumentation   OFF
~~~

The options "USE_SYSTEM_DOXYGEN" "USE_SYSTEM_SWIG" tell CMake to look for local installations of these two applications, or to download them and install from a remote source.
We suggest you to first install SWIG, DOXYGEN and SPHINX and to set these options as

~~~
USE_SYSTEM_SWIG    ON
USE_SYSTEM_DOXYGEN ON
~~~

Configure the code again, until CMake will let you generate the makefiles. Press "g" (ccmake) to generate the makefiles.

To build the code run (within "tulipBin")

~~~
make -j n
~~~

where n is the number of processors you want to use to build. 

### Generate the documentation

The sphinx documentation requires that **doxygen is run first** to generate the XML database.

To build the doxygen documentation type

~~~
make doxydoc
~~~

To build the sphinx documentation type (within "tulipBin")

~~~
make docs
~~~

You should be able to see the generated documentation by opening with a we browser the file

~~~
tulipBin/docs/sphinx/html/index.html
~~~

### Location of executables and python wrappers

After the build process has finished you should find:

- all the C++ executables in the "tulipBin/bin" folder.
- all the python modules in the "tulipBin/py" folder.