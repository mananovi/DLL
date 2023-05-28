#include "pch.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GeometricCurvesLibrary.h"

namespace GCurLib
{
    const double minBoundRad = 1.0;
    const double maxBoundRad = 20.0;
    const double minBoundStep = 1.0;
    const double maxBoundStep = 10.0;

    //---------------Error-----------------------//
    Error::Error(const string& mess, double val)
    {
        m_mess = mess;
        printf("Error: %s/n :%lf", m_mess.c_str(), val);
    }

    Error::Error(string&& mess, double val)
    {
        m_mess = move(mess);
        printf("Error: %s/n :%lf", m_mess.c_str(), val);
    }

    //----------------vector3---------------------//
    vector3&& operator+(const vector3& lhs, const vector3& rhs)
    {
        return move(vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z));
    }

    vector3&& operator-(const vector3& lhs, const vector3& rhs)
    {
        return move(vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z));
    }

    //---------------Curve3-----------------------//
    Curve3::Curve3() : m_position(0.0, 0.0, 0.0)
    {}

    Curve3::Curve3(double x_, double y_, double z_) : m_position(x_, y_, z_)
    {}

    Curve3::Curve3(const vector3& position) : m_position(position)
    {}

    Curve3::Curve3(vector3&& position) : m_position(move(position))
    {}

    //---------------Circle-----------------------//
    Circle::Circle(const double radius, const vector3& pos) : Curve3(pos)
    {
        if (radius > 0.0)
        {
            m_radius = radius;
        }
        else if (radius <= 0.0)
        {
            throw Error(string("Unable to create a circle with null or negative radius"), radius);
        }
    }

    vector3 Circle::pointAt(double t) const
    {
        return Curve3::m_position + vector3(m_radius * cos(t), m_radius * sin(t), 0.0);
    }
    vector3 Circle::derivative1At(double t) const
    {
        return vector3(-m_radius * sin(t), m_radius * cos(t), 0.0);
    }

    //-----------------Ellipse--------------------------//
    Ellipse::Ellipse(const double radiusA, const double radiusB, const vector3& pos) : Curve3(pos)
    {
        if (radiusA > 0.0 && radiusB > 0.0)
        {
            m_radiusA = radiusA;
            m_radiusB = radiusB;
        }
        else
        {
            throw Error(string("Unable to create an Ellipse with null or negative radii"), min(radiusA, radiusB));
        }
    }

    vector3 Ellipse::pointAt(double t) const
    {
        return Curve3::m_position + vector3(m_radiusA * cos(t), m_radiusB * sin(t), 0.0);
    }
    vector3 Ellipse::derivative1At(double t) const
    {
        return vector3(-m_radiusA * sin(t), m_radiusB * cos(t), 0.0);
    }

    //---------------------Helix-----------------------//
    Helix::Helix(const double radius, const double step, const vector3& pos) : Circle(radius, pos)
    {
        if (step > 0.0)
        {
            m_step = step;
        }
        else
        {
            throw Error(string("Unable to create a Helix with null or negative step"), step);
        }
    }

    vector3 Helix::pointAt(double t) const
    {
        if (t < 0.0)
        {
            throw Error(string("Helix not determined for negative semiaxis"), t);
        }

        return Curve3::m_position + vector3(Circle::m_radius * cos(t), Circle::m_radius * sin(t), (m_step * t) / (2.0 * M_PI));
    }
    vector3 Helix::derivative1At(double t) const
    {
        if (t < 0.0)
        {
            throw Error(string("Helix derivative not determined for negative semiaxis"), t);
        }

        return vector3(-Circle::m_radius * sin(t), Circle::m_radius * cos(t), m_step / (2.0 * M_PI));
    }

    //------------------------------------------------//
    bool isLess(shared_ptr<Curve3> l, shared_ptr<Curve3> r)
    {
        Circle* lCircle = dynamic_cast<Circle*>(l.get());
        Circle* rCircle = dynamic_cast<Circle*>(r.get());
        if (lCircle != nullptr && rCircle != nullptr)
        {
            return lCircle->radius() < rCircle->radius();
        }
        else
        {
            return true;                                // if not circles, then not sort
        }
    }


} //namespace GCurLib
