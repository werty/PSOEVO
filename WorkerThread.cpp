/* 
 * File:   WorkerThread.cpp
 * Author: marcin
 * 
 * Created on 15 wrzesień 2011, 02:16
 */

#include "WorkerThread.h"



WorkerThread::WorkerThread(const WorkerThread& orig) {
}

WorkerThread::~WorkerThread() {
}

WorkerThread::WorkerThread() : QThread()
{
  // We have to do this to make sure our thread has the
  // correct affinity.
  moveToThread(this);

  // This will do nothing until the user calls start().
}

void WorkerThread::run()
{
  // This schedules the doTheWork() function
  // to run just after our event loop starts up
  QTimer::singleShot(0, this, SLOT(doTheWork()));

  // This starts the event loop. Note that
  // exec() does not return until the
  // event loop is stopped.
  exec();
}

void WorkerThread::doTheWork()
{
  // Do the heavy-duty blocking stuff here
  // (simulated by a 5 second sleep for
  // this example)
  sleep(5);

  // When you're done, emit the results:
  emit done();

  // And some more sleeping for fun
  sleep(3);
  emit done();

  // ...
}