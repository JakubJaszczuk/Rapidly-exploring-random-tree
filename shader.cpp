#include "shader.hpp"

//funkcja ładuje shader z pliku, kompiluje go i zwraca jego idendyfikator
GLint loadShaderFromFile(const GLenum type, const char* nazwaPliku)
{
	std::ifstream plik(nazwaPliku, std::ios::binary | std::ios::in);		//otwórz plik z shaderem w trybie binarnym
	if (!plik.is_open())													//sprawdzenie czy plk został otwarty poprawnie
	{
		std::cout << "Niepoprawny odczyt pliku shadera" << std::endl;		//informacja o błędzie
		std::exit(1);
	}
	plik.seekg(0, std::ios::end);			//ustaw pozycję odczytu na koniec pliku
	unsigned int lenght = plik.tellg();		//odczytaj pozycję, zwraca długość pliku
	if (lenght <= 0)
	{
		std::cout << "Niepoprawny odczyt pliku shadera" << std::endl;		//informacja o błędzie
		std::exit(1);
	}
	plik.seekg(0, std::ios::beg);			//ustaw odczyt na początek
	char* bufor = new char[lenght + 1];		//bufor na kod żródłowy i znak null
	plik.read(bufor, lenght);				//wczytaj cały plik
	bufor[lenght] = '\0';					//dodaj znak null
	plik.close();							//zamknij plik

	GLint shader = glCreateShader(type);	//utworzenie idendyfikatora shadera
	glShaderSource(shader, 1, const_cast<GLchar**>(&bufor), nullptr);	//przekaż kod źródłowy shadera
	delete[] bufor;							//usuń bufor z kodem źródłowym shadera

	glCompileShader(shader);				//skompiluj shader
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);	//pobierz informacje o powodzeniu kompilacji i zapisz w status
	if (status != GL_TRUE)					//jeżeli kompilacja się niepowiodła wyświetl log shadera
	{
		std::cout << "Niepoprawna kompilacja shadera " << nazwaPliku << std::endl;
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);		//pobierz długość logu
		char* log = new char[logLength];							//bufor na log
		glGetShaderInfoLog(shader, logLength, nullptr, log);		//pobierz log informacyjny shadera
		std::cout << log << std::endl;								//wyświetl log
		delete[] log;
	}
	return shader;		//zwróć idendyfikator
}

void LinkValidateProgram(GLint program)
{
	glLinkProgram(program);			//konsolidacja programu
	GLint status;					//zmienna do przechowywania stanu konsolidacji i walidacji
	glGetProgramiv(program, GL_LINK_STATUS, &status);		//pobierz stan linkowania
	if (status == GL_FALSE)
	{
		//pobranie i wyświetlenie logu informacyjnego
		GLint loglength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
		char* log = new char[loglength];
		glGetProgramInfoLog(program, loglength, nullptr, log);
		std::cout << log << std::endl;
		delete[] log;
		std::exit(1);
	}
	glValidateProgram(program);		//walidacja programu(opcjonalna)
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);		//pobierz stan walidacji
	if (status == GL_FALSE)
	{
		//pobranie i wyświetlenie logu informacyjnego
		GLint loglength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
		char* log = new char[loglength];
		glGetProgramInfoLog(program, loglength, nullptr, log);
		std::cout << log << std::endl;
		delete[] log;
		std::exit(1);
	}
}

//moja funkcja
void attachShaders(GLint program, std::vector<GLint> shaders)
{
	for (GLint shader : shaders)
	{
		glAttachShader(program, shader);
	}
}
