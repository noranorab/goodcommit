#include <iostream>
#include <git2.h>
using namespace std;

int main(){
    cout << "Welcome to GoodCommit" << endl;
    // std::cout << "Let's Start !" << std::endl;
    // std::cout << "Enter your Git Repository :" << std::endl;
    // std::cin >> std::endl;

    // prompt the user for entering the repo
    git_libgit2_init();
    
    cout << "Welcome to GoodCommit " << endl;

    return 0;
}