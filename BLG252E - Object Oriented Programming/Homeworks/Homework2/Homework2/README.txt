
All classes were divided into header files for clarity.
    - AdderRemover.hpp & AdderRemover.cpp
    - ARTraverser.hpp & ARTraverser.cpp
    - FAFR.hpp & FAFR.cpp
    - LALR.hpp & LALR.cpp
    - FALR.hpp & FALR.cpp
    - LAFR.hpp & LAFR.cpp
    - Node.hpp & Node.cpp

Thus, the main code must include this piece of code:
    #include <iostream>

    #include "FAFR.hpp"
    #include "LALR.hpp"
    #include "FALR.hpp"
    #include "LAFR.hpp"
    #include "AdderRemover.hpp"

    using namespace std;


Also, tester is supposed to delete objects in main code which are allocated dynamically to avoid leak. Following code should be inserted at the end of main function which is given as test case.

    for(int j=0; j<4; j++){
        delete myList[j];
    }


Compile Command: g++ main.cpp AdderRemover.cpp ARTraverser.cpp FAFR.cpp LALR.cpp FALR.cpp LAFR.cpp Node.cpp -o kod
Run Command: ./kod

If no unnecessary cpp file exists in current directory, this compile command can be also used:
Compile Command: g++ *.cpp -o kod
Run Command: ./kod
    
