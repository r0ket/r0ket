#ifndef SIMULAT0RTHREAD_H
#define SIMULAT0RTHREAD_H

#include <QObject>
#include <QThread>

class Simulat0rThread : public QThread
{
  long loop;
  //     Q_OBJECT
     
     protected:
  virtual ~Simulat0rThread();
  virtual void run();
};

#endif
