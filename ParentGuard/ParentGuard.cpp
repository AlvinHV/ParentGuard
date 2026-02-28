//
//  ParentGuard.cpp
//  ParentGuard
//
//  Created by Alvin on 28.02.26.
//

#include <iostream>
#include "ParentGuard.hpp"
#include "ParentGuardPriv.hpp"

void ParentGuard::HelloWorld(const char * s)
{
    ParentGuardPriv *theObj = new ParentGuardPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void ParentGuardPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

