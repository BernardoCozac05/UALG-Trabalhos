#include <stdio.h>

int main() {
    double angle1, angle2, angle3;

    scanf("%lf", &angle1);
    scanf("%lf", &angle2);

    if (angle1 <= 0   || angle2 <= 0   ||
	angle1 + angle2 >= 180)
        printf("NO\n");
    else {
        angle3 = 180.0 - (angle1 + angle2);
        printf("%.3lf\n", angle3);
    }

    return 0;
}

