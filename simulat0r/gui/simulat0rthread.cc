#include "simulat0rthread.h"

extern "C" {
void  simulator_main();
}

#include <QWidget>
#include <iostream>
using namespace std;

extern QWidget* hackptr;

Simulat0rThread::~Simulat0rThread() {
}

void Simulat0rThread::run()
{
  simulator_main();
}
