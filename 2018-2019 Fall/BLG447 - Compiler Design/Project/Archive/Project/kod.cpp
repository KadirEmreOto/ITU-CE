//
//  main.cpp
//  Project
//
//  Created by Kadir Emre Oto on 14.11.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#include "Symbol.hpp"
#include "Grammar.hpp"


int main(int argc, const char * argv[]) {
    
    Grammar G;
    G.load("gramer.txt");  // TODO: argv[1] may be used
    G.display();
    
    return 0;
}
