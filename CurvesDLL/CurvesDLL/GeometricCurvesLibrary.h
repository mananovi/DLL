#pragma once

#ifdef CURVESDLL_EXPORTS
#define CURVESDLL_API __declspec(dllexport)
#else
#define CURVESDLL_API __declspec(dllimport)
#endif


#include <string>
#include <memory>

using namespace std;

namespace GCurLib
{

    class CURVESDLL_API Error
    {
    public:
        Error(const string& mess, double val = 0.0);
        Error(string&& mess, double val = 0.0);
        string m_mess;
    };

    class CURVESDLL_API vector3
    {
    public:
        double x;
        double y;
        double z;
        vector3(double x_, double y_, double z_)
        {
            x = x_;
            y = y_;
            z = z_;
        }
        vector3()
        {
            x = 0.0;
            y = 0.0;
            z = 0.0;
        }
    };

    vector3&& operator+(const vector3& lhs, const vector3& rhs);
    vector3&& operator-(const vector3& lhs, const vector3& rhs);

    class CURVESDLL_API Curve3
    {
    public:
        Curve3();
        Curve3(double x_, double y_, double z_);
        explicit Curve3(const vector3& position);
        explicit Curve3(vector3&& position);
        virtual vector3 pointAt(double t) const { return m_position; };
        virtual vector3 derivative1At(double t) const { return vector3(); };
        virtual ~Curve3() {};
    protected:
        vector3 m_position;
    };

    class CURVESDLL_API Circle : public Curve3
    {
    public:
        Circle(const double radius, const vector3& pos = vector3());
        double radius() { return m_radius; }
        virtual vector3 pointAt(double t) const override;
        virtual vector3 derivative1At(double t) const override;
    protected:
        double m_radius;
    };

    class CURVESDLL_API Ellipse : public Curve3
    {
    public:
        Ellipse(const double radiusA, const double radiusB, const vector3& pos = vector3());
        virtual vector3 pointAt(double t) const override;
        virtual vector3 derivative1At(double t) const override;
    private:
        double m_radiusA;
        double m_radiusB;
    };

    class CURVESDLL_API Helix : public Circle
    {
    public:
        Helix(const double radius, const double step, const vector3& pos = vector3());
        virtual vector3 pointAt(double t) const override;
        virtual vector3 derivative1At(double t) const override;
    private:
        double m_step;
    };

    CURVESDLL_API bool isLess(shared_ptr<Curve3> l, shared_ptr<Curve3> r);


} //namespace GCurLib