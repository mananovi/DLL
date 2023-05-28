#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "client.h"

using namespace std;

using namespace GCurLib;

const int sizeContainer = 100;

int main()
{
    Client demoClient;
    try
    {

        demoClient.populateContainer(sizeContainer);

        cout << "Coordinates of points and derivatives all Curves in the container at t = PI/4\n";

        list< shared_ptr<Curve3> >& curveContainer = demoClient.curveContainer();
        vector3 vecPoint, vecDerivat;
        cout << "Points:                |                  Derivatives:\n\n";
        for (auto it = curveContainer.begin(); it != curveContainer.end(); ++it)
        {
            vecPoint = (**it).pointAt(M_PI / 4.0);
            vecDerivat = (**it).derivative1At(M_PI / 4.0);
            cout << vecPoint.x << ", " << vecPoint.y << ", " << vecPoint.z << "    |    "
                << vecDerivat.x << ", " << vecDerivat.y << ", " << vecDerivat.z << '\n';
        }

        cout << "\n\nRadii of elements in the circleContainer, populated through selection circles from curveContainer:\n\n";

        demoClient.selectionCircles();
        demoClient.sortCircles();
        vector< shared_ptr<Curve3> >& circleContainer = demoClient.circleContainer();

        Circle* circEl;
        for (auto it = circleContainer.begin(); it != circleContainer.end(); ++it)
        {
            circEl = dynamic_cast<Circle*>(it->get());
            if (circEl != nullptr)
            {
                cout << circEl->radius() << '\n';
            }
        }

        cout << "\n\nTotal sum of radii of elements in the circleContainer:\n\n";

        cout << demoClient.sumCirclesRadii() << '\n';

        cout << "\n\nThe same operation using parallel computations:\n\n";

        cout << demoClient.sumCirclesRadiiPar() << '\n';
    }
    catch (Error er)
    {
        cout << er.m_mess << '\n';
    }

    getchar();
}