#include <iostream>
#include <string>
using namespace std;

auto factorial(auto n) {
if ((n == 0)) {
return 1;
}
else {
return 0;
}
}

int main() {
int numero;
int resultado;
string mensaje;
cout << "Ingrese un número para calcular su factorial:";
cin >> numero;
resultado = factorial(numero);
;
if ((resultado > 1000)) {
mensaje = "El factorial es muy grande";
}
else {
mensaje = "El factorial es: ";
}
cout << mensaje;
cout << resultado;
for (int i = 0; (i < 5); i = (i + 1)) {
cout << "Contador: " << i;
}
while ((numero > 0)) {
cout << numero;
numero = (numero - 1);
}
return 0;
}
