/* 
 * File:   WorkerThread.h
 * Author: marcin
 *
 * Created on 15 wrzesień 2011, 02:16
 */

#ifndef WORKERTHREAD_H
#define	WORKERTHREAD_H

#include <QThread>
#include <QTimer>

class WorkerThread : public QThread {
    Q_OBJECT
public:

    WorkerThread();

    WorkerThread(const WorkerThread& orig);
    virtual ~WorkerThread();
    int il_iter;
    void (*Funkcja)(); //wskaznik na funkcje ktora iteruje

protected:
    void run();
signals:
    void done();
private slots:
    void doTheWork();

};

#endif	/* WORKERTHREAD_H */

