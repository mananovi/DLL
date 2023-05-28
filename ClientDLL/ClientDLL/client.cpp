#include "client.h"
#include <time.h>
#include <cstdlib>
#include <algorithm>


//-----------------------------Client------------------------------//
Client::Client() :
minBoundRad(1.0),
maxBoundRad(20.0),
minBoundStep(1.0),
maxBoundStep(10.0)
{
    srand(static_cast<unsigned>(time(nullptr)));
    m_CircleContainer.reserve(1000);
}

void Client::populateContainer(int size)
{
    for (int i = 0; i < size; ++i)
    {
        m_CurveContainer.push_back(createCurve());
    }
}

double Client::randDouble(double min, double max)
{
    double val = static_cast<double>(rand()) / RAND_MAX;
    return min + val * (max - min);
}

shared_ptr<Curve3> Client::createCurve()
{
    int type = rand() % 3;
    switch(type)
    {
        case 0:
        {
            double radius = randDouble(minBoundRad, maxBoundRad);
            return shared_ptr<Curve3>(new Circle(radius));
            break;
        }
        case 1:
        {
            double radiusA = randDouble(minBoundRad, maxBoundRad);
            double radiusB = randDouble(minBoundRad, maxBoundRad);
            return shared_ptr<Curve3>(new Ellipse(radiusA, radiusB));
            break;
        }
        case 2:
        {
            double radius = randDouble(minBoundRad, maxBoundRad);
            double step = randDouble(minBoundStep, maxBoundStep);
            return shared_ptr<Curve3>(new Helix(radius, step));
            break;
        }
    }
}

void Client::selectionCircles()
{
    for (auto it = m_CurveContainer.begin(); it != m_CurveContainer.end(); ++it)
    {
        if (dynamic_cast<Circle *>(it->get()) != nullptr)
        {
            m_CircleContainer.push_back(*it);
        }
    }
}

void Client::sortCircles()
{
    sort(m_CircleContainer.begin(), m_CircleContainer.end(), isLess);
}

double Client::sumCirclesRadii()
{
    double sum = 0.0;
    for (auto it = m_CircleContainer.begin(); it != m_CircleContainer.end(); ++it)
    {
        Circle * circEl = dynamic_cast<Circle *>(it->get());
        if (circEl != nullptr)
        {
            sum += circEl->radius();
        }
    }

    return sum;
}

void Client::sumBlock(vector< shared_ptr<Curve3> >::iterator beg, vector< shared_ptr<Curve3> >::iterator end, double& result)
{
    result = 0.0;
    Circle* circEl;
    for (auto it = beg; it != end; ++it)
    {
        circEl = dynamic_cast<Circle*>(it->get());
        if (circEl != nullptr)
        {
            result += circEl->radius();
        }
    }
}

double Client::sumCirclesRadiiPar()
{
    unsigned numThreads = thread::hardware_concurrency();
    unsigned blkSize = m_CircleContainer.size() / numThreads;
    vector<double> results(numThreads);
    vector<thread> asyncThreads;
    join_threads joiner(asyncThreads);
    auto blkStart = m_CircleContainer.begin();
    vector< shared_ptr<Curve3> >::iterator blkEnd;
    for (unsigned i = 0; i < numThreads - 1; ++i)
    {
        blkEnd = blkStart + blkSize;
        asyncThreads.push_back(move(thread(&Client::sumBlock, this, blkStart, blkEnd, ref(results[i]))));
        blkStart = blkEnd;
    }
    
    sumBlock(blkStart, m_CircleContainer.end(), results[numThreads - 1]);
    for (unsigned i = 0; i < numThreads - 1; ++i)
    {
        if (asyncThreads[i].joinable())
        {
            asyncThreads[i].join();
        }
    }
    
    double result = 0.0;
    for (auto it = results.begin(); it != results.end(); ++it)
    {
        result += *it;
    }

    return result;
}

//------------------------join_threads------------------------//
join_threads::~join_threads()
{
    for (unsigned long i = 0; i < threads.size(); ++i)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }
}

