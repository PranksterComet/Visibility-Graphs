#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <glut.h>   // freeglut.h might be a better alternative, if available.
#include <cstdlib>
#include <map>
#include <time.h>
#include <set>
#include <iterator>

#define ll long long int 

using namespace std;

#define PI 3.1415926535898

//Colors
GLfloat red[3] = { 1.0, 0.0, 0.0 };
GLfloat green[3] = { 0.0, 1.0, 0.0 };
GLfloat blue[3] = { 0.0, 0.0, 1.0 };
GLfloat black[3] = { 0.0, 0.0, 0.0 };
GLfloat white[3] = { 1.0, 1.0, 1.0 };
GLfloat gray[3] = { 0.5, 0.5, 0.5 };
GLfloat yellow[3] = { 1.0, 1.0, 0.0 };
GLfloat magenta[3] = { 1.0, 0.0, 1.0 };
GLfloat cyan[3] = { 0.0, 1.0, 1.0 };

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D( left, right, bottom, top );
	// Centre Origin: gluOrtho2D(-1*(GLdouble)w/2, (GLdouble)w/2, -1*(GLdouble)h/2, (GLdouble)h/2);
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void draw_circle(double r, double x, double y) {
	//glColor3fv(blue);
	int precision = 100;
	double theta = 0;
	glBegin(GL_POLYGON);
	for (int i = 0; i < precision; i++) {
		theta = i * 2 * PI / precision;
		glVertex2f(x + r * cos(theta), y + r * sin(theta));
	}
	glEnd();
}
	

void printVec(vector<ll> v) {
	int n = v.size();
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	cout << '\n';
}

struct Point {
	int x, y;
	Point(int x1, int y1) : x(x1), y(y1) {}
	
};

struct lineSeg {
	Point p1, p2;
	lineSeg(Point p, Point q) : p1(p), p2(q) {}
};

lineSeg operator-(Point p1, Point p2) {
	lineSeg l(p2, p1);
	return l;
}

Point operator*(int n, Point q) {
	Point p(n * q.x, n * q.y);
	return p;
}

bool operator==(Point p, Point q) {
	if (p.x == q.x and p.y == q.y) return true;
	return false;
}

ll SqMag(lineSeg l) {
	return pow((l.p2.x - l.p1.x), 2) + pow((l.p2.y - l.p1.y), 2);
}

Point Shift(Point A, Point B) {
	A.x -= B.x;
	A.y -= B.y;
	return A;
}

Point O(0, 0);
Point ogn(O.x, O.y);
Point Inf(500, 500);
lineSeg ref(O, Inf);
int WIN_SIZE = 500;
int pointX, pointY = -10;
set<pair<Point, int>> s;
set<pair<lineSeg, int>> tree;
lineSeg sweepl = Inf - O;
map<lineSeg, int> key;
vector<Point> pts; // first point is origin
vector<lineSeg> lines;
vector <vector<int>> rel(1000, vector<int>(1000, -1));
//rel[i][j] will tell if the ith point is p1 or p2 in jth lineseg

vector<vector<int>> own(1000);
//own[i] will contain the list of all lineSegs that contain pts[i]

vector<vector<int>> vis(1000);
//vis[i] is the vector of indices of points that are visible to pts[i]

vector<ll> Int(lineSeg l1, lineSeg l2) {
	ll x1 = l1.p1.x;
	ll y1 = l1.p1.y;
	ll x2 = l1.p2.x;
	ll y2 = l1.p2.y;
	ll x3 = l2.p1.x;
	ll y3 = l2.p1.y;
	ll x4 = l2.p2.x;
	ll y4 = l2.p2.y;
	vector<ll> v;
	ll x_num = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
	ll x_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	ll y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
	ll y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	v = { x_num, x_den, y_num, y_den };

	if (x_den == 0 || y_den == 0) {
		v = { -999, -999, -999, -999 };
	}

	return v;
}

bool operator<(const pair<lineSeg, int>& L, const pair<lineSeg, int>& M) {


	lineSeg A = L.first;
	lineSeg B = M.first;
	lineSeg sl = Inf - O;
	vector<ll> va = Int(A, sl);
	vector<ll> vb = Int(B, sl);
	ll val1 = (va[0] - (O.x) * va[1]) * vb[1];
	ll val2 = (vb[0] - (O.x) * vb[1]) * va[1];

	if (abs(val1) < abs(val2)) {
		return true;
	}

	return false;
}

int Sign(Point p, lineSeg l) { // Always Order l in angComp
	p.x -= l.p1.x;
	p.y -= l.p1.y;
	l.p2.x -= l.p1.x;
	l.p2.y -= l.p1.y;
	int ar = p.x * l.p2.y - p.y * l.p2.x;
	if (ar == 0) {
		return ar;
	}
	else {
		return ar / abs(ar);
	}
	// 1 if p is to the right of l, -1 if to the left, 0 if on
}

bool angComp(Point p1, Point p2, Point o) {
	p1 = Shift(p1, o);
	p2 = Shift(p2, o);

	Point fp(5, 0);
	o.x = 0;
	o.y = 0;
	lineSeg ax = fp - o;

	int ar = p1.x * p2.y - p2.x * p1.y;

	if (Sign(p1, ax) == Sign(p2, ax)) {
		int val = Sign(p1, ax);

		if (val == 0 && p2.x < 0 && p1.x > 0) {
			return true;
		}

		else if (ar == 0) {
			return SqMag(p1 - o) < SqMag(p2 - o);
		}

		else if (ar > 0) return true;
	}

	else {
		int v1 = Sign(p1, ax);
		int v2 = Sign(p2, ax);
		if (v1 == 0) {
			if (p1.x > 0) {
				return true;
			}

			else if (p1.x < 0) {
				if (v2 > 0) return true;
			}
		}

		else if (v2 == 0) {
			if (p2.x < 0 && v1 < 0) return true;
		}

		else if (v1 == -1 && v2 == 1) return true;

	}

	return false;
}

bool operator<(const pair<Point, int>& a, const pair<Point, int>& b) {
	Point p1 = a.first;
	Point p2 = b.first;
	return angComp(p1, p2, O);
}

void initTree(Point p) {
	//First Initialize elements in tree
	//Sweep Line drawn between first point and O. 
	tree.clear();

	Inf.x = O.x + 500; // this might not be necessary
	Inf.y = O.y;
	lineSeg swpInit = Inf - O;
	int tot = lines.size();
	for (int i = 0; i < tot; i++) {
		lineSeg l = lines[i];
		
		int v1 = Sign(l.p1, swpInit); // Only works for origin
		int v2 = Sign(l.p2, swpInit);
		int v3 = Sign(O, l);
		int v4 = Sign(Inf, l);

		if (!(v1 == v2 || v3 == v4)) {
			tree.insert({ l, i });
			rel[2 * i][i] = 1 - rel[2 * i][i];
			rel[2 * i + 1][i] = 1 - rel[2 * i + 1][i];
		}
	}

	//s.erase(s.begin());

}

void visGraph() {
	for (auto pr : s) { // was prev point on the same line, if so was prev
		// pt  visible? was there any prev point on the same sweep line
		// init assump no collinear points
		// What happens when tree is empty?

		Point p = pr.first;
		Inf = p;
		int j = pr.second;
		cout << "Point: " << j << '\n';
		//Check visibility

		if (tree.size() == 0) {
			vis[0].push_back(j);
		}
		
		else {
			pair<lineSeg, int> fp = *tree.begin();
			lineSeg h = fp.first;
			int ind = fp.second;
			if (h.p2 == p || h.p1 == p) {
				vis[0].push_back(j);
				cout << j << "is touching sl" << '\n';
			}

			else if (Sign(h.p1, p - O) == Sign(h.p2, p - O) || Sign(p, h) == Sign(O, h)){
				vis[0].push_back(j);
				cout << j << "is touching sl" << '\n';
			}
		}

		for (auto e : own[j]) {
			int val = rel[j][e];
			cout << val << '\n';
			if (val == 0) { //  p is p1 
				tree.insert({ lines[e], e });
				cout << "Inserting ls: " << e << '\n';
			}
			else if (val == 1) { // p is p2
				tree.erase({ lines[e], e });
				cout << "Deleting ls: " << e << '\n';
			}
		}

		cout << "Curr Size: " << tree.size() << '\n';
	}

}

//OpenGL functions


void drawLS(lineSeg l) {
	glBegin(GL_LINES);
	glVertex2f(l.p1.x, l.p1.y);
	glVertex2f(l.p2.x, l.p2.y);
	glEnd();
}

void mouse(int button, int state, int x, int y)
{
	pointX = (float)x;  // window_width * world_width;
	pointY = (float)(WIN_SIZE - y); // window_height * world_height;

	pointX = round(pointX);
	pointY = round(pointY);

	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
		cout << pointX << " " << pointY << '\n';
		// Add a vertex to the list of vertices...
		Point p = Point(int(pointX), int(pointY));
		pts.push_back(Point(int(pointX), int(pointY)));
		// Add pts into s in pairs after the indice swapping is done
		int n = pts.size();

		if (n % 2 == 0 && n>0) {
			Point q = pts[n - 2];
			Point p = pts[n - 1];
			
			if (!angComp(p, q, O)) {
				swap(p, q);
				swap(pts[n - 1], pts[n - 2]);
			}
			lines.push_back(lineSeg(p, q));
			int m = lines.size();

			s.insert({ p, n - 1 });
			s.insert({ q, n - 2 });

			rel[n - 1][m - 1] = 0; 
			rel[n - 2][m - 1] = 1;
			own[n - 1].push_back(m - 1);
			own[n - 2].push_back(m - 1);
		}

		glutPostRedisplay();
		
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		cout << pointX << " " << pointY << '\n';
		if (pts.size() == 0) {
			O.x = pointX;
			O.y = pointY;
		}

		else {
			Point p = (*s.begin()).first;
			initTree(p); //May have to relocate
			cout << "Tree Init Size: " << tree.size() << '\n';

			sweepl = p - O;

			cout << '\n';
			visGraph();
		}
		glutPostRedisplay();
	}
} 

void display() {  // Display function will draw the image.

	glClearColor(1, 1, 1, 1);  // (In fact, this is the default.)
	glClear(GL_COLOR_BUFFER_BIT);

	int cnt = 0;
	for (auto p : pts) {
		if (cnt % 2 == 0) glColor3fv(blue);
		else glColor3fv(red);
		draw_circle(5, p.x, p.y);
		cnt++;
	}

	//Draw Origin 
	glColor3fv(green);
	draw_circle(6, O.x, O.y);

	//Draw SweepLine
	//glColor3fv(yellow);
	//drawLS(sweepl);

	for (auto l : lines) {
		glColor3fv(green);
		drawLS(l);
	}

	/*
	for (auto el : tree) {
		lineSeg l = el.first;
		glColor3fv(black);
		drawLS(l);
	}
	*/

	for (int x : vis[0]) {
		lineSeg l = pts[x] - O;
		glColor3fv(black);
		drawLS(l);
	}

	glFlush();

}





int main(int argc, char** argv) {
	// bottom left origin

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Use single color buffer and no depth buffer.
	glutInitWindowSize(WIN_SIZE, WIN_SIZE);        // Size of display area, in pixels.
	glutInitWindowPosition(500, 100);    // Location of window in screen coordinates.
	glutCreateWindow("Visibility Graph"); // Parameter is window title.

	glutMouseFunc(mouse);
	glutDisplayFunc(display);            // Called when the window needs to be redrawn.
	glutReshapeFunc(reshape);            // Resizing coord sys with window

	cout << "Hi" << '\n';
	

	// Program ends when user closes the window.
	glutMainLoop(); // Run the event loop!  This function does not return.


	return 0;
}