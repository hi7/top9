#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>

#define U 24
#define H 12
#define D 6

long
rgba(uchar r, uchar g, uchar b, uchar a);

Point
add(Point p1, Point p2);

void
box(long color, Point loc, int d, int inv);

void
laser(Point loc, int d);

void
thrust(Point loc, int d);

long LGRAY = (0xAA << 24) + (0xAA << 16) + (0xAA << 8) + 0xFF;
long GRAY = (0x88 << 24) + (0x88 << 16) + (0x88 << 8) + 0xFF;
long RED = (0xFF << 24) + (0x00 << 16) + (0x00 << 8) + 0xFF;
long BLUE = (0x00 << 24) + (0x00 << 16) + (0xFF << 8) + 0xFF;

typedef struct Ship Ship;
struct Ship {
	Point pos;
	double theta;
};

void
drawship(Ship ship, Point center) {
	double c = cos(ship.theta), s = sin(ship.theta);
	Point loc = add(add(ship.pos, Pt(c*24, s*24)), center);
	//TODO: rotate ship
	draw(screen, Rect(loc.x-H-4, loc.y-2*U-4, loc.x+H+8, loc.y+2*U+8), display->black, nil, ZP);

	box(RED, add(loc, Pt(0, -U)), 9, -1);
	box(LGRAY, loc, U, 1);
	box(BLUE, add(loc, Pt(0, U)), 9, 1);
	laser(add(loc, Pt(0, -U*2)), H);
	thrust(add(loc, Pt(0, U*2)), H);
}

void
main() {
	initdraw(nil, nil, "Trader or Pirate");
	Point center = Pt((screen->r.max.x + screen->r.min.x) / 2, (screen->r.max.y + screen->r.min.y) / 2);
	Ship ship = {
		.pos = Pt(0, 0),
		.theta = 0,
	};
	draw(screen, screen->r, display->black, nil, ZP);
	while(1) {
		//pos.y -= 1;
		ship.theta += 0.1;
		drawship(ship, center);
		flushimage(display, 1);
	//while(1) {
		sleep(1000/15);
	}
  //closedisplay(display));
}

long
rgba(uchar r, uchar g, uchar b, uchar a) {
	return (r << 24) + (g << 16) + (b << 8) + a;
}

Point
add(Point p1, Point p2) {
	return Pt(p1.x + p2.x, p1.y + p2.y);
}

void
drawpoly(long color, Point loc, Point p[], int n) {
	Image *palette = allocimagemix(display, color, color);
	for(int i=0; i < n; i++) {
		line(screen, add(loc, p[i]), add(loc, p[(i + 1) % n]), Enddisc, Enddisc, 1, palette, Pt(0, 0));
	}
}

void
box(long color, Point loc, int d, int inv) {
	int x1 = loc.x - H, x2 = loc.x + H, hi = H*inv;
	Point f[] = {
		x1, loc.y-hi+(U-d)*inv,
		x2, loc.y-hi+(U-d)*inv,
		x2, loc.y+hi,
		x1, loc.y+hi,
		x1, loc.y-hi
	};
	Image *palette = allocimagemix(display, color, color);
	fillpoly(screen, f, 5, 1, palette, Pt(0, 0));
	Point p[4] = { -H, -H, H, -H, H, H, -H, H };
	drawpoly(GRAY, loc, p, 4);
}

void
laser(Point loc, int d) {
	Point f[5] = {
		loc.x-H, loc.y+H,
		loc.x-H+d, loc.y+H-d,
		loc.x+H-d, loc.y+H-d,
		loc.x+H, loc.y+H,
		loc.x-H, loc.y+H,
	};
	long color = rgba(0xFF, 0x00, 0x00, 0xFF);
	Image *fillpalette = allocimagemix(display, color, color);
	fillpoly(screen, f, 5, 1, fillpalette, Pt(0, 0));

	Point p[] = { -H, H, 0, 0, H, H };
	Image *palette = allocimagemix(display, GRAY, GRAY);
	for(int i=0; i < 3; i++) {
		line(screen, add(loc, p[i]), add(loc, p[(i + 1) % 3]), Enddisc, Enddisc, 1, palette, Pt(0, 0));
	}
}

void
thrust(Point loc, int d) {
	Point f[5] = {
		loc.x-D, loc.y-H,
		loc.x-D-d/2, loc.y-H+d,
		loc.x+D+d/2, loc.y-H+d,
		loc.x+D, loc.y-H,
		loc.x-D, loc.y-H,
	};
	long color = rgba(0x00, 0x00, 0xFF, 0xFF);
	Image *fillpalette = allocimagemix(display, color, color);
	fillpoly(screen, f, 5, 1, fillpalette, Pt(0, 0));

	Point p[] = {
		-H, 0,
		-D, -H,
		D, -H,
		H, 0
	};
	Image *palette = allocimagemix(display, GRAY, GRAY);
	for(int i=0; i <= 3; i++) {
		line(screen, add(loc, p[i]), add(loc, p[(i + 1) % 4]), Enddisc, Enddisc, 1, palette, Pt(0, 0));
	}
}
