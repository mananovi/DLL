#ifndef CLIENT_H
#define CLIENT_H

#include "GeometricCurvesLibrary.h"
#include <memory>
#include <list>
#include <vector>
#include <thread>

using namespace GCurLib;

class join_threads
{
public:
    explicit join_threads(vector<thread>& threads_) : threads(threads_) {}
//    void setThreads(vector<thread>& threads_) { threads = threads_; }
    ~join_threads();
private:
    vector<thread>& threads;
};

class Client
{
public:
    Client();
    list< shared_ptr<Curve3> > & curveContainer() { return m_CurveContainer; }
    vector< shared_ptr<Curve3> > & circleContainer() { return m_CircleContainer; }
    void populateContainer(int size);
    void selectionCircles();
    void sortCircles();
    double sumCirclesRadii();
    double sumCirclesRadiiPar();
private:
    shared_ptr<Curve3> createCurve();
    double randDouble(double min, double max);
    void sumBlock(vector< shared_ptr<Curve3> >::iterator beg, vector< shared_ptr<Curve3> >::iterator end, double& result);
    list< shared_ptr<Curve3> > m_CurveContainer;
    vector< shared_ptr<Curve3> > m_CircleContainer;
    vector<thread> mThreads;
    const double minBoundRad;
    const double maxBoundRad;
    const double minBoundStep;
    const double maxBoundStep;
};


#endif // CLIENT_H
