# addon-compile-runBin
There are 2 addons in the git, compile and runBin.
compile writes v8 produced cacheddata to file.
runBin load and run the binary file instead of javascript.

# usage
````git clone https://github.com/liyingzh/addon-compile-runBin.git````

````cd addon-compile-runBin/compile````

````npm install````

````node-gyp configure````

````node-gyp build````

compile the test.js to test.bin with ````iojs ./````

copy the test.bin to addon-compile-runBin/runBin

building and running runBin is same as compile.

# NOTE FOR runBin
iojs need some change, look at

https://github.com/liyingzh/io.js/commit/d36df48bf6234fec18caeafb8d801aa962807cf8

then build and install iojs.
