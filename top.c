#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>

#define U 24
#define H 12
#define D 6
#define O 3

long
rgba(uchar r, uchar g, uchar b, uchar a);

Point
add(Point p1, Point p2);

void
box(long color, Point loc);

void
laser(Point loc);

void
thrust(Point loc);

void
main() {
  initdraw(nit, nil, "Trader or Pirate");
  Point center = Pt((screen->r.max.x + screen->r.min.x) / 2, (screen->r.max.y + screen->r.min.y) / 2);
  
  box(rgba(0xFF, 0x00, 0x00, 0xFF), add(center, Pt(0, -U-O)));
  box(rgba(0x88, 0x88, 0x88, 0xFF), add(center, Pt(0, -U-O)));
  box(rgba(0x00, 0xFF, 0xFF, 0xFF), add(center, Pt(0, -U-O)));
  laser(add(center, Pt(0, -U-O)));
  thrust(add(center, Pt(0, U+O)));
  
  flushimage(display, 1);
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
box(long color, Point loc) {
  Image *palette = allocimagemix(display, color, color);
  line(screen, add(loc, Pt(-H, -H)), add(loc, Pt(H, -H)), Enddisc, Enddisc, 1, palette, Pt(0, 0));
  line(screen, add(loc, Pt(H, -H)), add(loc, Pt(H, H)), Enddisc, Enddisc, 1, palette, Pt(0, 0));
  line(screen, add(loc, Pt(H, H)), add(loc, Pt(-H, H)), Enddisc, Enddisc, 1, palette, Pt(0, 0));
  line(screen, add(loc, Pt(-H, -H)), add(loc, Pt(-H, H)), Enddisc, Enddisc, 1, palette, Pt(0, 0));
}

void
laser(Point loc) {
  Point p[] = { Pt(-H, -H-O), Pt(0, -U-O), Pt(H, -H-O) };
  long color = rgba(0xFF, 0x00, 0x00, 0xFF);
  Image *palette = allocimagemix(display, color, color);
  for(int i=0; i <= 2; i++) {
    line(screen, add(loc, p[i]), add(loc, p[(i + 1) % 3]), Enddisc, Enddisc, 1, Pt(0, 0));
  }
}

void
thrust(Point loc) {
  Point p[] = { Pt(-H, U+O), Pt(-D, H+O), Pt(D, H+O), Pt(H, U+O) };
  long color = rgba(0x00, 0x00, 0xFF, 0xFF);
  Image *palette = allocimagemix(display, color, color);
  for(int i=0; i <= 3; i++) {
    line(screen, add(loc, p[i]), add(loc, p[(i + 1) % 4]), Enddisc, Enddisc, 1, Pt(0, 0));
  }
}
