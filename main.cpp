//
//  main.cpp
//  RDH
//
//  Created by Wang on 3/12/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#include <iostream>
//#include "Util.hpp"
//#include "Hs.hpp"
//#include "Pe.hpp"
#include "Test.hpp"
int main(int argc, const char * argv[]) {
    // insert code here...
     double time1 = clock();

    test_color_cpp();
//    int ival = -1;
//    BYTE bval = ival;
//    cout << bval <<endl;
    cout << (clock() - time1) * 1000 / CLOCKS_PER_SEC << "ms" <<endl;
    return 0;
}
