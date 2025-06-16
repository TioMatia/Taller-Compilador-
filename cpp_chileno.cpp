#include <iostream>
#include <string>
using namespace std;

auto suma(auto suma_a, auto suma_b) {
cout << "------------------";
float sumaResultado;
sumaResultado = (suma_a + suma_b);
cout << "Resultado suma: " << sumaResultado;
cout << "------------------";
}

auto resta(auto resta_a, auto resta_b) {
cout << "------------------";
float restaResultado;
restaResultado = (resta_a - resta_b);
cout << "Resultado resta: " << restaResultado;
cout << "------------------";
}

auto multiplicacion(auto multiplicacion_a, auto multiplicacion_b) {
cout << "------------------";
float multiplicacionResultado;
multiplicacionResultado = (multiplicacion_a * multiplicacion_b);
cout << "Resultado multiplicacion: " << multiplicacionResultado;
cout << "------------------";
}

auto division(auto division_a, auto division_b) {
cout << "------------------";
float divisionResultado;
divisionResultado = (division_a / division_b);
cout << "Resultado division: " << divisionResultado;
cout << "------------------";
}

auto potencia(auto base, auto exponente) {
int resultado;
resultado = 1;
int i;
i = 0;
while ((i < exponente)) {
resultado = (resultado * base);
i = (i + 1);
}
cout << "------------------";
cout << "Resultado de base " << base << " Exponente: " << exponente << ": " << resultado;
cout << "------------------";
}

int main() {
float numeroSuma1;
float numeroSuma2;
float numeroResta1;
float numeroResta2;
float numeroMultiplicacion1;
float numeroMultiplicacion2;
float numeroDivision1;
float numeroDivision2;
int numeroBase;
int numeroExponente;
int opcion;
opcion = 999;
cout << "-------Calculadora------";
while ((opcion != 0)) {
cout << "1- Sumar 2 numeros";
cout << "2- Restar 2 numeros";
cout << "3- Dividir 2 numeros";
cout << "4- Multiplicar 2 numeros";
cout << "5- Obtener el exponente de un numero x sobre uno y";
cout << "0- Salir";
cout << "Digite una opcion: ";
cin >> opcion;
if ((opcion == 1)) {
cout << "Digite el primer digito: ";
cin >> numeroSuma1;
cout << "Digite el segundo digito: ";
cin >> numeroSuma2;
suma(numeroSuma1, numeroSuma2);
}
if ((opcion == 2)) {
cout << "Digite el primer digito: ";
cin >> numeroResta1;
cout << "Digite el segundo digito: ";
cin >> numeroResta2;
resta(numeroResta1, numeroResta2);
}
if ((opcion == 3)) {
cout << "Digite el primer digito: ";
cin >> numeroMultiplicacion1;
cout << "Digite el segundo digito: ";
cin >> numeroMultiplicacion2;
multiplicacion(numeroMultiplicacion1, numeroMultiplicacion2);
}
if ((opcion == 4)) {
cout << "Digite el primer digito: ";
cin >> numeroDivision1;
cout << "Digite el segundo digito: ";
cin >> numeroDivision2;
division(numeroDivision1, numeroDivision2);
}
if ((opcion == 5)) {
cout << "Digite el primer digito (base): ";
cin >> numeroBase;
cout << "Digite el segundo digito (exponente): ";
cin >> numeroExponente;
potencia(numeroBase, numeroExponente);
}
}
return 0;
}
