#include <stdio.h>

void print_spaces(int count) {
    if (count <= 0) return;
    printf(" ");
    print_spaces(count - 1);
}

void print_stars(int count) {
    if (count <= 0) return;
    printf("*");
    print_stars(count - 1);
}

void print_upper_part(int size, int current_width) {
    if (current_width > size) return; 
    print_spaces((size - current_width) / 2); 
    print_stars(current_width);
    printf("\n");
    print_upper_part(size, current_width + 2);
}

void print_lower_part(int size, int current_width) {
    if (current_width <= 0) return; 
    print_spaces((size - current_width) / 2); 
    print_stars(current_width);
    printf("\n");
    print_lower_part(size, current_width - 2);
}

void print_diamond(int n) {
    print_upper_part(n, 2 - (n%2));  
    print_lower_part(n, n - 2); 
    return;
}

void print_all_diamonds(int max_size, int current_size) {
  if (current_size > 2 * max_size - 1) return;
  int s = (current_size<=max_size)?current_size:(current_size - 2*(current_size-max_size));
  print_diamond(s);
  print_all_diamonds(max_size, current_size + 1);
}

int main() {
    int n;
    if ((scanf("%d", &n)!=1) || n<=0) {printf("NO\n");return 0;}
    print_all_diamonds(n, 1);
    return 0;
}

