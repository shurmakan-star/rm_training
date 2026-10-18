#include <iostream>
#include <cmath>
#include "Angle.hpp"

using namespace std;

int main() {
    double v0 = 10.0; 
    double g = 9.8;   
    double d;

    cout << "请输入目标距离 d (m): ";
    cin >> d;

    Angle myAngle(v0, g);
    double rad = myAngle.solver(d);
    const double PI = 3.1415926535897; 
    if (rad != -1.0) {
        double degree = rad * 180.0 / PI;
        cout << "弧度制角度: " << rad << " rad" << endl;
        cout << "角度制角度: " << degree << " 度" << endl;
    }

    return 0;
}