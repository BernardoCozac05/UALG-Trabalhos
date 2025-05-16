#include <stdio.h>

int main() {
    double x, y;         // Coordenadas do ponto
    double xe, ye;       // Coordenadas do canto inferior esquerdo do retângulo
    double xd, yd;       // Coordenadas do canto superior direito do retângulo
    scanf(" %lf %lf", &xe, &ye);
   
    scanf(" %lf %lf", &xd, &yd);
    
    scanf(" %lf %lf", &x, &y);

    // Verificação de consistência dos dados do retângulo
    if (xe >= xd || ye >= yd) {
      printf("NO\n");
    } else {
        // Verificação se o ponto está dentro ou fora do retângulo (incluindo bordas)
        if (x >= xe && x <= xd && y >= ye && y <= yd) {
            printf("IN\n");
        } else {
            printf("OUT\n");
        }
    }

    return 0;
}
