#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <fstream>

using namespace std;
class Triangle
{
public:
    Triangle(double a[3], double b[3], double c[3])
    {
        moveArrValues(m_bottomLeft,  a);
        moveArrValues(m_bottomRight, b);
        moveArrValues(m_topCenter,   c);
    }

    void display(ofstream& fout)
    {
        printArr(m_bottomLeft, fout);
        std::cout << " | ";
        printArr(m_bottomRight, fout);
        std::cout << " | ";
        printArr(m_topCenter, fout);
        std::cout << "\n";

    }

    void getBottomLeft(double a[3])
    {
        moveArrValues(a, m_bottomLeft);
    }

    void getBottomRight(double a[3])
    {
        moveArrValues(a, m_bottomRight);
    }

    void getTopCenter(double a[3])
    {
        moveArrValues(a, m_topCenter);
    }

    void setBlbrPtr(Triangle* blbrPtr)
    {
        m_blbrPtr = blbrPtr;
    }

    void setBltcPtr(Triangle* bltcPtr)
    {
        m_bltcPtr = bltcPtr;
    }

    void setBrtcPtr(Triangle* brtcPtr)
    {
        m_brtcPtr = brtcPtr;
    }

    Triangle* getBlbrPtr()
    {
        return m_blbrPtr;
    }

    Triangle* getBltcPtr()
    {
        return m_bltcPtr;
    }

    Triangle* getBrtcPtr()
    {
        return m_brtcPtr;
    }

private:
    double m_bottomLeft  [3],
           m_bottomRight [3],
           m_topCenter   [3];

    Triangle* m_blbrPtr = NULL;
    Triangle* m_bltcPtr = NULL;
    Triangle* m_brtcPtr = NULL;

    void printArr(double a[3], ofstream& fout)
    {
        std::cout << "(" << a[0] << ", " << a[1] << ", " << a[2] << ")";
        fout << a[0] << ", " << a[2] << ", " << a[1] << endl;
    }

    void moveArrValues(double a[3], double b[3])
    {
        a[0] = b[0];
        a[1] = b[1];
        a[2] = b[2];
    }
};

#endif
