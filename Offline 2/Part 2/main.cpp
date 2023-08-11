#include <bits/stdc++.h>
#include "Point.cpp"
#include "Transformation.cpp"
#include "Triangle.cpp"
#include "bitmap_image.hpp"

using namespace std;

double getLeftIntersectingColumn(double y, const Triangle& t) {
    double minX = 9999;
    double x1, x2, x3;
    if ((t.v1.y <= y && t.v2.y >= y) || (t.v1.y >= y && t.v2.y <= y)) {
        x1 = t.v1.x + (y - t.v1.y) * (t.v2.x - t.v1.x) / (t.v2.y - t.v1.y);
        minX = min(minX, x1);
    }
    if ((t.v2.y <= y && t.v3.y >= y) || (t.v2.y >= y && t.v3.y <= y)) {
        x2 = t.v2.x + (y - t.v2.y) * (t.v3.x - t.v2.x) / (t.v3.y - t.v2.y);
        minX = min(minX, x2);
    }
    if ((t.v3.y <= y && t.v1.y >= y) || (t.v3.y >= y && t.v1.y <= y)) {
        x3 = t.v3.x + (y - t.v3.y) * (t.v1.x - t.v3.x) / (t.v1.y - t.v3.y);
        minX = min(minX, x3);
    }

    return minX;
}

double getRightIntersectingColumn(double y, const Triangle& t) {
    double maxX = -9999;
    double x1, x2, x3;
    if ((t.v1.y <= y && t.v2.y >= y) || (t.v1.y >= y && t.v2.y <= y)) {
        x1 = t.v1.x + (y - t.v1.y) * (t.v2.x - t.v1.x) / (t.v2.y - t.v1.y);
        maxX = max(maxX, x1);
    }
    if ((t.v2.y <= y && t.v3.y >= y) || (t.v2.y >= y && t.v3.y <= y)) {
        x2 = t.v2.x + (y - t.v2.y) * (t.v3.x - t.v2.x) / (t.v3.y - t.v2.y);
        maxX = max(maxX, x2);
    }
    if ((t.v3.y <= y && t.v1.y >= y) || (t.v3.y >= y && t.v1.y <= y)) {
        x3 = t.v3.x + (y - t.v3.y) * (t.v1.x - t.v3.x) / (t.v1.y - t.v3.y);
        maxX = max(maxX, x3);
    }

    return maxX;
}

int main() {

    string inputFilePath = "Test Cases (Updated 2 Aug)/1/"; //! ""

    // Stage 1
    // Modeling Transformation

    ifstream input(inputFilePath + "scene.txt");
    ofstream output("stage1.txt");

    Point eye;
    Point look;
    Point up;
    double fovY;
    double aspectRatio;
    double near;
    double far;

    input >> eye.x >> eye.y >> eye.z;
    input >> look.x >> look.y >> look.z;
    input >> up.x >> up.y >> up.z;
    input >> fovY >> aspectRatio >> near >> far;

    output << fixed << setprecision(7);

    stack<Transformation> S;
    Transformation M;
    S.push(M);

    string command = "";
    int lineNumber = 4;
    int triangleCount = 0;

    while(true){
        input >> command;
        lineNumber++;

        if(command=="triangle"){
            triangleCount++;
            Point v1, v2, v3;
            input >> v1.x >> v1.y >> v1.z;
            input >> v2.x >> v2.y >> v2.z;
            input >> v3.x >> v3.y >> v3.z;
            lineNumber += 3;

            v1 = S.top().transform(v1);
            v2 = S.top().transform(v2);
            v3 = S.top().transform(v3);

            output << v1.x << " " << v1.y << " " << v1.z << endl;
            output << v2.x << " " << v2.y << " " << v2.z << endl;
            output << v3.x << " " << v3.y << " " << v3.z << endl;
            output << endl;
        } else if(command=="translate"){
            double tx, ty, tz;
            input >> tx >> ty >> tz;
            lineNumber++;

            Transformation T = Transformation::translationMatrix(tx, ty, tz);
            M = S.top() * T;
            S.pop();
            S.push(M);
        } else if(command=="scale"){
            double sx, sy, sz;
            input >> sx >> sy >> sz;
            lineNumber++;

            Transformation T = Transformation::scalingMatrix(sx, sy, sz);
            M = S.top() * T;
            S.pop();
            S.push(M);
        } else if(command=="rotate"){
            double angle, ax, ay, az;
            input >> angle >> ax >> ay >> az;
            lineNumber++;

            Transformation T = Transformation::rotationMatrix(angle, ax, ay, az);
            M = S.top() * T;
            S.pop();
            S.push(M);
        } else if(command=="push"){
            M = S.top();
            S.push(M);
        } else if(command=="pop"){
            S.pop();
        } else if(command=="end"){
            break;
        } else {
            cout << "Invalid command at line " << lineNumber << endl;
            break;
        }
    }
    input.close();
    output.close();

    // Stage 2
    // View Transformation

    input.open("stage1.txt");
    output.open("stage2.txt");

    Transformation V = Transformation::viewMatrix(eye, look, up);

    for (int i = 0; i < triangleCount; i++) {
        Point v1, v2, v3;
        input >> v1.x >> v1.y >> v1.z;
        input >> v2.x >> v2.y >> v2.z;
        input >> v3.x >> v3.y >> v3.z;

        v1 = V.transform(v1);
        v2 = V.transform(v2);
        v3 = V.transform(v3);

        output << v1.x << " " << v1.y << " " << v1.z << endl;
        output << v2.x << " " << v2.y << " " << v2.z << endl;
        output << v3.x << " " << v3.y << " " << v3.z << endl;
        output << endl;
    }

    input.close();
    output.close();

    // Stage 3
    // Projection Transformation

    input.open("stage2.txt");
    output.open("stage3.txt");

    Transformation P = Transformation::projectionMatrix(fovY, aspectRatio, near, far);

    for (int i = 0; i < triangleCount; i++) {
        Point v1, v2, v3;
        input >> v1.x >> v1.y >> v1.z;
        input >> v2.x >> v2.y >> v2.z;
        input >> v3.x >> v3.y >> v3.z;

        v1 = P.transform(v1);
        v2 = P.transform(v2);
        v3 = P.transform(v3);

        output << v1.x << " " << v1.y << " " << v1.z << endl;
        output << v2.x << " " << v2.y << " " << v2.z << endl;
        output << v3.x << " " << v3.y << " " << v3.z << endl;
        output << endl;
    }

    input.close();
    output.close();

    // Stage 4
    // Clipping and scan conversion (Z-buffer algo)

    int screenWidth, screenHeight;

    input.open(inputFilePath + "config.txt");
    input >> screenWidth >> screenHeight;
    input.close();

    input.open("stage3.txt");

    vector<Triangle> triangles;
    for (int i = 0; i < triangleCount; i++) {
        Point v1, v2, v3;
        input >> v1.x >> v1.y >> v1.z;
        input >> v2.x >> v2.y >> v2.z;
        input >> v3.x >> v3.y >> v3.z;

        Triangle t(v1, v2, v3);
        triangles.push_back(t);
    }

    // for (int i = 0; i < triangleCount; i++) {
    //     triangles[i].print();
    // }

    input.close();

    // init z-buffer and frame buffer
    double rightLimit = 1.0;
    double leftLimit = -1.0;
    double topLimit = 1.0;
    double bottomLimit = -1.0;
    double z_max = 2.0;
    double z_front_limit = -1.0;

    double dx = (rightLimit - leftLimit) / screenWidth;
    double dy = (topLimit - bottomLimit) / screenHeight;
    double Top_Y = topLimit - dy/2.0;
    double Left_X = leftLimit + dx/2.0;
    double Bottom_Y = bottomLimit + dy/2.0;
    double Right_X = rightLimit - dx/2.0;

    cout << dx << " " << dy << endl;
    cout << Top_Y << " " << Left_X << " " << Bottom_Y << " " << Right_X << endl;

    double **zBuffer = new double*[screenHeight];
    for (int i = 0; i < screenHeight; i++) {
        zBuffer[i] = new double[screenWidth];
        for (int j = 0; j < screenWidth; j++) {
            zBuffer[i][j] = z_max;
        }
    }

    bitmap_image image(screenWidth, screenHeight);
    image.set_all_channels(0, 0, 0);
    image.save_image("test.png");

    for (int i = 0; i < triangleCount; i++) {
        Triangle t = triangles[i];
        Point v1 = t.v1;
        Point v2 = t.v2;
        Point v3 = t.v3;
        Point n = t.getNormalToPlane();
        // cout << "Normal: " << n.x << " " << n.y << " " << n.z << endl;
        // plane equation Ax + By + Cz + D = 0
        double A = n.x, B = n.y, C = n.z;
        double D = -n.x * v1.x - n.y * v1.y - n.z * v1.z;
        Point centroid = t.getCentroid();

        double topScanline, bottomScanline;
        double maxY = max(max(v1.y, v2.y), v3.y);
        if (maxY > Top_Y) topScanline = Top_Y;
        else {
            if (abs(centroid.y - ceil(maxY / dy) * dy) <= abs(centroid.y - floor(maxY / dy) * dy))
                topScanline = ceil(maxY / dy) * dy;
            else topScanline = floor(maxY / dy) * dy;
        }
        double minY = min(min(v1.y, v2.y), v3.y);
        if (minY < Bottom_Y) bottomScanline = Bottom_Y;
        else {
            if (abs(centroid.y - ceil(minY / dy) * dy) < abs(centroid.y - floor(minY / dy) * dy))
                bottomScanline = ceil(minY / dy) * dy;
            else bottomScanline = floor(minY / dy) * dy;
        }
        // bottomScanline = floor(minY / dy) * dy;

        // cout << v1.y << " " << v2.y << " " << v3.y << endl;
        cout << "Top Scanline: " << topScanline << " ";
        cout << "Bottom Scanline: " << bottomScanline << endl;

        for (double y = topScanline; y >= bottomScanline; y -= dy) {
            double leftIntersectingColumn, rightIntersectingColumn;
            //! fix this
            double minX = getLeftIntersectingColumn(y, t);
            if (minX < Left_X) leftIntersectingColumn = Left_X;
            else leftIntersectingColumn = (minX / dx) * dx;
            // leftIntersectingColumn = max(minX, Left_X);

            double maxX = getRightIntersectingColumn(y, t);
            if (maxX > Right_X) rightIntersectingColumn = Right_X;
            else rightIntersectingColumn = (maxX / dx) * dx;
            // rightIntersectingColumn = min(maxX, Right_X);

            cout << "Left Intersecting Column: " << leftIntersectingColumn << " ";
            cout << "Right Intersecting Column: " << rightIntersectingColumn << endl;

            for (double x = leftIntersectingColumn; x <= rightIntersectingColumn; x += dx) {
                // Calculate z-value
                // from plane equation
                double z_value = (-D - A * x - B * y) / C;
                // Compare with z-buffer and z front limit
                int row = ((Top_Y - y) / dy);
                int col = ((x - Left_X) / dx);
                if (z_value < zBuffer[row][col] && z_value > z_front_limit) {
                    zBuffer[row][col] = z_value;
                    // Set color
                    image.set_pixel(col, row, t.color[0], t.color[1], t.color[2]);
                }
            }
        }
    }

    // output z-buffer value
    output.open("z_buffer.txt");
    output << fixed << setprecision(6);
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            if (zBuffer[i][j] < z_max)
                output << zBuffer[i][j] << "\t";
            else output << "\t";
        }
        output << endl;
    }
    output.close();

    // save final image
    image.save_image("out.bmp");

    // free memory
    for (int i = 0; i < screenHeight; i++) {
        delete[] zBuffer[i];
    }
    delete[] zBuffer;

    // free image memory
    image.clear();

    return 0;
}
