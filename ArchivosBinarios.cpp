#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Persona {
	string nombre;
	int edad;
	double gpa;
public:
	//Constructor
	Persona(string nombre = "Default", int edad = 0, double gpa = 0) {
		this->nombre = nombre;
		this->edad = edad;
		this->gpa = gpa;
	}

	//Metodo escribir
	/*
	---------------NOTA---------------
	Debido a que un atributo es string y el size de esta puede variar, se necesita serializar y deserializar correctamente
	Esto se hace tomando en cuneta el size de la string que tambien vamos a escribir en el archivo como una variable size_t
	*/
	void escribir() {
		// Abrir archivo
		fstream file("personas.dat", ios::out | ios::binary | ios::app); // Out para escritura | Binary para binario | app para que no borre los registros
		if (file) { // Si tiene exito al abrir el archivo entra aqui
			

			size_t length = nombre.size(); // Size del atributo string
			file.write(reinterpret_cast<char*>(&length), sizeof(length));// Escritura del size_t length
			file.write(nombre.c_str(), length); // Escritura del nombre. Observese que se pasa a c string con el metodo .c_str()

			file.write(reinterpret_cast<char*>(&edad), sizeof(edad)); // Escritura de edad
			file.write(reinterpret_cast<char*>(&gpa), sizeof(gpa)); // Escritura de gpa

			file.close(); // Se cierra el archivo
		}
		else { // Si falla al abrir el archvio muestra un mensaje
			cout << "Ocurrio un error al abrir el archivo para escritura!\n";
		}
	}

	// Metodo print del objeto
	void mostrar() const {
		cout << "Nombre: " << nombre << ", Edad: " << edad << ", GPA: " << gpa << endl;
	}

	// Metodo de lectura
	static void leer() {
		fstream file("personas.dat", ios::in | ios::binary); // Abre el archivo en in para lecutra | binary para binario
		if (file) { // Si tiene exito al abrir
			while (true) { // Loop infinito
				size_t length;
				if (!file.read(reinterpret_cast<char*>(&length), sizeof(length))) { // Verifica si se puede hacer la lectura del size_t length
					break; // Si no, sale del loop
				}
				char* buffer = new char[length + 1]; // Buffer de size perfecto para la string. Seria length + 1 por en '\0' al final del string
				file.read(buffer, length); // Lee el nombre y lo guarda en el buffer.  Lee length cantidad de bytes
				buffer[length] = '\0'; // Establece el ultimo elemento del char* como '\0' (null termination)

				string nombre(buffer); // Establece una variable string que contenga el buffer que acabamos de hacer
				delete[] buffer; // borra el buffer ya que no se necesita
				int edad; double gpa; // Variables para los otros atributos
				file.read(reinterpret_cast<char*>(&edad), sizeof(edad)); // Lectura y almacenamiento de edad
				file.read(reinterpret_cast<char*>(&gpa), sizeof(gpa)); // Lectura y almacenamiento de gpa
				Persona persona(nombre, edad, gpa); // Creacion de un objeto
				persona.mostrar(); // Se imprime el objeto
			}
			file.close(); // Cerrar el archivo
		}
		else { // Si no tiene exito al abrir el archvio 
			cout << "Ocurrio un error al abrir el archivo para lectura!\n";
		}
	}
};

int main() {
	Persona p1("Alice", 25, 3.5);
	Persona p2("Bob", 30, 3.8);

	p1.escribir();
	p2.escribir();

	Persona::leer();

	return 0;
}
