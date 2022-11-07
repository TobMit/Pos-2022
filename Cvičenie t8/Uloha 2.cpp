#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    cout << "Ja som proces s ID: " << getpid() << endl;
    cout << "Moj predok ma PID " << getppid() << endl;
    return 0;
}