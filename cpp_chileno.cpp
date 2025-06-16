#include <iostream>
#include <string>
using namespace std;

auto duplicar(auto x) {
return (x * 2);
}

auto es_par(auto n) {
if ((n * (2 == 0))) {
cout << "Es par" << endl;
}
else {
cout << "Es impar" << endl;
}
}

int main() {
int a;
float b;
string saludo;
a = 10;
b = 3.140000;
saludo = "Hola Mundo";
cout << saludo << endl;
cin >> a;
cout << "El doble es: " << duplicar(a);
 << endl;
es_par(a);
while ((a > 0)) {
cout << a << endl;
a = (a - 1);
}
for (int i;
i = 0;
; (i < 5); i = (i + 1);
) {
cout << i << endl;
}
return 0;
}
