#include <stdio.h>
#include <math.h>

int main() {
    double x, y, r, theta;

    scanf("%lf", &x);
    scanf("%lf", &y);

    if (x == 0.0 && y == 0.0)
      printf("NO\n");
    else {
      r = sqrt(x * x + y * y);
      if (x > 0)                theta = atan(y / x);
      else if (x < 0 && y >= 0) theta = atan(y / x) + M_PI;
      else if (x < 0 && y < 0)  theta = atan(y / x) - M_PI;
      else if (y >= 0)          theta = M_PI / 2.0;
      else                      theta = - (M_PI / 2.0);
      printf("%.3lf %.3lf\n", r, theta);
    }

    return 0;
}

