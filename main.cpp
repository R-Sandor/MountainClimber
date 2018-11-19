///Collaborated with Joseph Rugh, Raphael Sandor, and Derek Tiller
#include <iostream>
#include <sstream>
#include "Triangle.h"
#include <list>
#include <random>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

const int NUMBER_OF_ITERATIONS = 250;
const int TRIANGLE_BASE_SIZE   = 100;

void displayList(std::list<Triangle> triangles, ofstream& fout);
double compNormal(double [], double [],double []);
bool isUseable(double);

int main()
{
    ofstream fout;
    fout.open("gnuplot.txt");

    // 4 corners of the triangle
    double c1 [3] = { 0, 0, 0 };
    double c2 [3] = { TRIANGLE_BASE_SIZE, 0, 0 };
    double c3 [3] = { 0, 0, TRIANGLE_BASE_SIZE };
    double c4 [3] = { TRIANGLE_BASE_SIZE, 0, TRIANGLE_BASE_SIZE };

    // Requesting input for the top of the triangle
    cout << "triangle top:\n";
    double topX = 0, topY = 0, topZ = 0;
    while (true)
    {
        cout << "x: ";
        cin >> topX;
        if (topX <= 0 || topX > TRIANGLE_BASE_SIZE) continue;
        cout << "y: ";
        cin >> topY;
        if (topY <= 0 || topY > TRIANGLE_BASE_SIZE) continue;
        cout << "z: ";
        cin >> topZ;
        if (topZ <= 0 || topZ > TRIANGLE_BASE_SIZE) continue;
        break;
    }

    double top [3] = { topX, topY, topZ };

    list<Triangle> triangles;
    list<Triangle>::iterator triItr;

    // Creating the initial 4 triangles
    Triangle bottomTri = Triangle(c1, c2, top);
    Triangle rightTri = Triangle(c2, c4, top);
    Triangle topTri = Triangle(c4, c3, top);
    Triangle leftTri = Triangle(c3, c1, top);

    // Initializing all the links of the triangles
    bottomTri.setBltcPtr(&leftTri);
    bottomTri.setBrtcPtr(&rightTri);

    rightTri.setBltcPtr(&bottomTri);
    rightTri.setBrtcPtr(&topTri);

    topTri.setBltcPtr(&rightTri);
    topTri.setBrtcPtr(&leftTri);

    leftTri.setBltcPtr(&topTri);
    leftTri.setBrtcPtr(&bottomTri);

    // Adding the initial triangles
    triangles.push_back(bottomTri);
    triangles.push_back(rightTri);
    triangles.push_back(topTri);
    triangles.push_back(leftTri);

    default_random_engine gen(time(NULL));

    double bl [3], br [3], tc [3];

    for (int i = 0; i < NUMBER_OF_ITERATIONS; i++)
    {
        // Replacing the first triangle with 3 new triangles
        triItr = triangles.begin();
        triItr->getBottomLeft(bl);
        triItr->getBottomRight(br);
        triItr->getTopCenter(tc);

        // Calculating average of the three
        double ax = (bl[0] + br[0] + tc[0]) / 3;
        double ay = (bl[1] + br[1] + tc[1]) / 3;
        double az = (bl[2] + br[2] + tc[2]) / 3;

        // Displacement of the average values
        std::uniform_real_distribution<double> deltaDist(-3, 5);
        double dx = deltaDist(gen);
        double dy = deltaDist(gen);
        double dz = deltaDist(gen);

        double dist [3] = { ax+dx, ay+dy, az+dz };

        // Making the three new triangles
        Triangle newTri1 = Triangle(bl, br, dist);
        Triangle* newTriPtr1 = &newTri1;
        Triangle newTri2 = Triangle(br, tc, dist);
        Triangle* newTriPtr2 = &newTri2;
        Triangle newTri3 = Triangle(tc, bl, dist);
        Triangle* newTriPtr3 = &newTri3;

        // Linking the pointers
        newTri1.setBltcPtr(newTriPtr3);
        newTri1.setBrtcPtr(newTriPtr2);
        newTri1.setBlbrPtr(triItr->getBlbrPtr());

        newTri2.setBltcPtr(newTriPtr1);
        newTri2.setBrtcPtr(newTriPtr3);
        newTri2.setBlbrPtr(triItr->getBrtcPtr());

        newTri3.setBltcPtr(newTriPtr2);
        newTri3.setBrtcPtr(newTriPtr1);
        newTri3.setBlbrPtr(triItr->getBltcPtr());

        // Pushing back the three new triangles
        triangles.push_back(newTri1);
        triangles.push_back(newTri2);
        triangles.push_back(newTri3);

        // Removing the triangle
        triangles.pop_front();
    }

    displayList(triangles, fout);

    fout.close();

    fout.open("commands.txt");
    fout << "splot 'gnuplot.txt' with lines\n";
    fout << "pause -1\n";
    fout.close();

    system("gnuplot commands.txt");

    return 0;
}

void displayList(list<Triangle> triangles, ofstream& fout)
{
    list<Triangle>::iterator itr = triangles.begin();
    while (itr != triangles.end())
    {
        itr->display(fout);
        itr++;
    }
}

double compNormal(double bl[], double br[],double tc[])
{
    double a, b, c, d, e, f;
    a = bl[0]-tc[0];
    b = bl[1]-tc[1];
    c = bl[2]-tc[2];
    d = br[0]-tc[0];
    e = br[1]-tc[1];
    f = br[2]-tc[2];

    double alpha, beta, gamma;
    alpha = (b*f)-(c*e);
    beta  = (a*f)-(c*d);
    gamma = (a*e)-(d*b);

    double length, useable;
    length = sqrt(pow(alpha,2.0) + pow(beta,2.0) + pow(gamma,2.0));
    useable = (gamma/length);

    return useable;

}

bool isUseable(double useable)
{
    if (useable > .707)
        return true;
    else
        return false;
}

