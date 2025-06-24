#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <string> 
#include <fstream>
#include <cmath>

using namespace std;

enum Estado {
    ESPERANDO_USUARIO,
    CREAR_CUENTA_ESPERAR_NUMERO_CUENTA,
    NUMERO_DE_CUENTA_EXISTENTE,
    CREAR_CUENTA_ESPERAR_NIP,
    CREAR_CUENTA_SALDO,
    CREAR_CUENTA,
    CUENTA_CREADA_EXITO,
    ESPERANDO_NIP,
    MOSTRANDO_OPCIONES,
    ESPERANDO_MONTO_DEPOSITO,
    ESPERANDO_MONTO_RETIRO,  
    OPERACION_FALLIDA_RETIRO,
    VER_MOVIMIENTOS,
    MOSTRAR_CORTE_RETIRADO,
    MOSTRAR_DEPOSITO_EXITOSO,
};


// Cuenta
class SistemaBanco {
    public:
    SistemaBanco() {}
    public:
    void registrarCliente(int numeroCuenta,int nip,float saldo,float ingreso,float egreso);
    bool verificarNip(int numCuenta,int n);
    float obtenerSaldo(int numCuenta);
    bool existeCuenta(int numCuenta);
    void Retirar(int numCuenta,float montoRetiro);
    float Ingresos(int numCuenta);
    float Egresos(int numCuenta);
    void Depositar(int numCuenta,float montoIngresado);
    void mostrarSaldo(int numCuenta, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* display);
    ~SistemaBanco() {
        cout << endl;
    }
};

void SistemaBanco::registrarCliente(int numeroCuenta,int nip,float saldo,float ingreso,float egreso) {
    string nombre_archivo = to_string(numeroCuenta);
    ofstream archivo(nombre_archivo);
    if(archivo.is_open()) {
        archivo << numeroCuenta << endl;
        archivo << nip << endl;
        archivo << saldo << endl;
        archivo << ingreso << endl;
        archivo << egreso << endl;
        archivo.close();
    } else {
        cout << "Error al guardar el archivo";
    }
}

bool SistemaBanco::existeCuenta(int numCuenta) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    return archivo.is_open();
}

bool SistemaBanco::verificarNip(int numCuenta,int n) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    
    if(archivo.is_open()) {
        string linea;
        getline(archivo,linea);

        if(getline(archivo,linea)) {
            if(stoi(linea) == n) {
                archivo.close();
                return true;
            }
        }
        archivo.close();
    }
    return false;
}

void SistemaBanco::mostrarSaldo(int numCuenta, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* display) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de la cuenta." << endl;
        return;
    }

    string linea;
    getline(archivo, linea); 
    getline(archivo, linea);
    getline(archivo, linea); 

    float saldo = stof(linea);
    al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Saldo actual: %.2f Bs", saldo);

    archivo.close();
}

float SistemaBanco::obtenerSaldo(int numCuenta) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    if(archivo.is_open()) {
        string linea;
        getline(archivo,linea);
        getline(archivo,linea);
        if(getline(archivo,linea)) {
            return stof(linea);
        }
    }
    return 0.0;
}

void SistemaBanco::Retirar(int numCuenta,float montoRetiro) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    if(!archivo.is_open()) {
        cout << "error abrir archivo" << endl;
        return;
    }
    string linea;
    int numeroCuenta,nip;
    float saldo,ingreso,egreso;
    getline(archivo,linea); numeroCuenta = stoi(linea);
    getline(archivo, linea); nip = stoi(linea);
    getline(archivo, linea); saldo = stof(linea);
    getline(archivo, linea); ingreso = stof(linea);
    getline(archivo, linea); egreso = stof(linea);

    if(montoRetiro > saldo) {
        archivo.close();
        return;
    }

    saldo -= montoRetiro;
    egreso += montoRetiro;
    archivo.close();

    ofstream archivoSalida(nombre_archivo);

    if(!archivoSalida.is_open()) {
        return;
    }

    archivoSalida << numeroCuenta << endl;
    archivoSalida << nip << endl;
    archivoSalida << saldo << endl;
    archivoSalida << ingreso << endl;
    archivoSalida << egreso << endl;

    archivoSalida.close();
}

void SistemaBanco::Depositar(int numCuenta,float montoIngresado) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);

    if(!archivo.is_open()) {
        return;
    }
    string linea;
    int numeroCuenta,nip;
    float saldo,ingreso,egreso;
    getline(archivo,linea); numeroCuenta = stoi(linea);
    getline(archivo, linea); nip = stoi(linea);
    getline(archivo, linea); saldo = stof(linea);
    getline(archivo, linea); ingreso = stof(linea);
    getline(archivo, linea); egreso = stof(linea);

    saldo += montoIngresado;
    ingreso += montoIngresado;
    archivo.close();

    ofstream archivoSalida(nombre_archivo);

    if(!archivoSalida.is_open()) {
        return;
    }

    archivoSalida << numeroCuenta << endl;
    archivoSalida << nip << endl;
    archivoSalida << saldo << endl;
    archivoSalida << ingreso << endl;
    archivoSalida << egreso << endl;

    archivoSalida.close();
}

float SistemaBanco::Ingresos(int numCuenta) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    if(!archivo.is_open()) {
        cout << "no se pudo abrir el archivo";
        return 0.0;
    }
    string linea;
    getline(archivo,linea);
    getline(archivo,linea);
    getline(archivo,linea);
    getline(archivo,linea);

    float ingreso = stof(linea);
    archivo.close();
    return ingreso;
}

float SistemaBanco::Egresos(int numCuenta) {
    string nombre_archivo = to_string(numCuenta);
    ifstream archivo(nombre_archivo);
    if(!archivo.is_open()) {
        cout << "no se pudo abrir el archivo";
        return 0.0;
    }
    string linea;
    getline(archivo,linea);
    getline(archivo,linea);
    getline(archivo,linea);
    getline(archivo,linea);
    getline(archivo,linea);

    float egreso = stof(linea);
    archivo.close();
    return egreso;
}

class Dispensador {
public:
    float denominaciones[11] = {200, 100, 50, 20, 10, 5, 2, 1, 0.5, 0.2, 0.1};
    int cantidadBilletes[11] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    int cortesEntregados[11] = {0};

    ~Dispensador(){
        cout<<endl;
    }
    
    bool retirarMonto(float monto);
    void mostrarCortes(ALLEGRO_FONT* font, int x, int y);
    void aceptarDeposito(float monto);
};

bool Dispensador::retirarMonto(float monto) {
    monto = round(monto * 10.0f) / 10.0f;
    float montoRestante = monto;

    int cortesTemporales[11] = {0};

    cout << "Intentando cubrir el monto: " << monto << " Bs" << endl;

    for (int i = 0; i < 11; i++) {
        if (montoRestante >= denominaciones[i]) {
            int cantidad = static_cast<int>(montoRestante / denominaciones[i]);
            if (cantidad > cantidadBilletes[i]) {
                cantidad = cantidadBilletes[i];
            }
            cortesTemporales[i] = cantidad;
            montoRestante -= cantidad * denominaciones[i];
            montoRestante = round(montoRestante * 100.0f) / 100.0f; // redondeo a centavos
        }
    }

    if (montoRestante > 0.001f) {
        cout << "No se puede cubrir el monto exacto. Falta: " << montoRestante << " Bs" << endl;
        return false;
    }

    for (int i = 0; i < 11; i++) {
        cortesEntregados[i] = 0;
    }

    for (int i = 0; i < 11; i++) {
        cantidadBilletes[i] -= cortesTemporales[i];
        cortesEntregados[i] = cortesTemporales[i];
    }

    return true;
}

void Dispensador::aceptarDeposito(float monto) {
    int cantidadDeposito;
    float montoRestante = round(monto * 10.0f) / 10.0f;

    for (int i = 0; i < 11; i++) {
        if (montoRestante >= denominaciones[i]) {
            cantidadDeposito = static_cast<int>(montoRestante / denominaciones[i]);
            montoRestante -= cantidadDeposito * denominaciones[i];
            montoRestante = round(montoRestante * 100.0f) / 100.0f;
            cantidadBilletes[i] += cantidadDeposito;
        }
    }

    if (montoRestante > 0.001f) {
        cout << "No se pudo cubrir el monto exacto con las denominaciones disponibles." << endl;
    }
}

void Dispensador::mostrarCortes(ALLEGRO_FONT* font, int x, int y) {
    for (int i = 0; i < 11; i++) {
        if (cortesEntregados[i] > 0) {
            al_draw_textf(font, al_map_rgb(0, 0, 0), x, y + (i * 10), ALLEGRO_ALIGN_LEFT, 
                          "%d cortes de %.2f Bs", cortesEntregados[i], denominaciones[i]);
        }
    }
}

class CajeroAutomatico {
public:
    CajeroAutomatico();
    ~CajeroAutomatico();
    void Iniciar();

private:
    //allegro
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    //Manejo de estados y entradas
    Estado estadoActual;
    string inputActual;
    // Allegro Logica
    void DibujarPantalla();
    void CrearBotones();
    void ProcesarEntrada(int x, int y);
    void DibujarDispensador();
    void DibujarRanura();
    //Sistema
    SistemaBanco sistema;
    //Dispensador
    Dispensador dispensador;
    //Creacion de cuenta variables
    string nuevo_numeroCuenta;
    string nuevo_nip;
    float saldo_inicial;
    //Variables para iniciar sesion
    int numeroCuenta;
    int inicio_nip;
    int nuevo_usuario;
    float saldo_pantalla;
    //varialble retirar
    int montoRetirar;
    //cortes
    int cortes[10];
};

CajeroAutomatico::CajeroAutomatico()
    : estadoActual(ESPERANDO_USUARIO), inputActual("") {
    // Iniciar Allegro
    if (!al_init()) {
        al_draw_text(font, al_map_rgb(255, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "Error al inicializar Allegro.");
        exit(-1);
    }

    al_init_primitives_addon();
    al_init_font_addon();
    al_install_mouse();

    // Crear pantalla y componentes
    display = al_create_display(1280, 900);
    if (!display) {
        al_draw_text(font, al_map_rgb(255, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "No se pudo crear la pantalla.");
        exit(-1);
    }

    font = al_create_builtin_font();
    if (!font) {
        al_draw_text(font, al_map_rgb(255, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "No se pudo crear la fuente.");
        exit(-1);
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_draw_text(font, al_map_rgb(255, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "No se pudo crear la cola de eventos.");
        exit(-1);
    }

    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        al_draw_text(font, al_map_rgb(255, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "No se pudo crear el temporizador.");
        exit(-1);
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
}

CajeroAutomatico::~CajeroAutomatico() {
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
}

void CajeroAutomatico::Iniciar() {
    float last_time = al_get_time();
    while (true) {
        float current_time = al_get_time();
        if (current_time - last_time >= 1.0 / 60.0) {
            last_time = current_time;

            ALLEGRO_EVENT event;
            while (al_get_next_event(event_queue, &event)) {
                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return;
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    int x = event.mouse.x;
                    int y = event.mouse.y;
                    cout << "Clic en: (" << x << ", " << y << ")" << endl;

                    ProcesarEntrada(x, y);
                }
            }

            DibujarPantalla();
            al_flip_display();
        }
    }
}


void CajeroAutomatico::DibujarPantalla() {
    al_clear_to_color(al_map_rgb(169, 169, 169));

    al_draw_filled_rectangle(50, 30, 640, 450, al_map_rgb(255, 255, 255)); 

    al_draw_text(font, al_map_rgb(0, 0, 0), 320, 100, ALLEGRO_ALIGN_CENTER, inputActual.c_str());

    DibujarRanura();

    if (estadoActual == ESPERANDO_USUARIO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Bienvenido, Ingrese su Número de cuenta");
        al_draw_text(font, al_map_rgb(0, 0, 0), 190, 400, ALLEGRO_ALIGN_CENTER, "No tienes cuenta?,ingresa 0");
    } else if (estadoActual == ESPERANDO_NIP) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Ingrese su NIP:");
    } else if (estadoActual == MOSTRANDO_OPCIONES) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "1. Depositar | 2. Retirar | 3. Movimientos");
        al_draw_text(font, al_map_rgb(0, 0, 0), 160, 400, ALLEGRO_ALIGN_CENTER, "Salir ingrese 0");
        sistema.mostrarSaldo(numeroCuenta, font, display);
    } else if (estadoActual == ESPERANDO_MONTO_DEPOSITO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Por favor,ingrese el monto que desea depositar");
    } else if (estadoActual == ESPERANDO_MONTO_RETIRO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Por favor, escriba el monto que desea retirar");
    } else if(estadoActual == MOSTRAR_CORTE_RETIRADO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Retiro concretado con exito , saque el dinero de la ranura");
        al_draw_text(font, al_map_rgb(0, 0, 0), 190, 400, ALLEGRO_ALIGN_CENTER, "Escribe 0 para volver al menu");
        dispensador.mostrarCortes(font, 530, 620); 
    } else if(estadoActual == OPERACION_FALLIDA_RETIRO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Saldo insuficiente para retirar o revise el dispensador");
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Presione ENTER para volver al menu");
    } else if(estadoActual == CREAR_CUENTA_ESPERAR_NUMERO_CUENTA) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Escriba un numero de cuenta 5 digitos");
        al_draw_text(font, al_map_rgb(0, 0, 0), 160, 400, ALLEGRO_ALIGN_CENTER, "Salir ingrese 0");
    } else if(estadoActual == CREAR_CUENTA_ESPERAR_NIP) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Escriba un NIP 5 digitos");
    } else if(estadoActual == CREAR_CUENTA_SALDO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Porfavor digite su saldo inicial");
    } else if(estadoActual == CUENTA_CREADA_EXITO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 50, ALLEGRO_ALIGN_CENTER, "Cuenta Creada con exito! Bienvenido a esta familia");
        al_draw_text(font, al_map_rgb(0, 0, 0), 160, 400, ALLEGRO_ALIGN_CENTER, "Salir ingrese 0");
    } else if(estadoActual == VER_MOVIMIENTOS) {
        float ingresos = sistema.Ingresos(numeroCuenta);
        float egresos = sistema.Egresos(numeroCuenta); 
        al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 60, ALLEGRO_ALIGN_CENTER, "Ingresos: %.2f Bs", ingresos);
        al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Egresos: %.2f Bs", egresos);
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 400, ALLEGRO_ALIGN_CENTER, "Escribe 0 para volver al menu");
    } else if(estadoActual == CREAR_CUENTA) {
        al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 100, ALLEGRO_ALIGN_CENTER, "presione ENTER para finalizar");
    } else if(estadoActual == MOSTRAR_DEPOSITO_EXITOSO) {
        al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "Deposito exitoso porfavor revise su saldo");
        al_draw_text(font, al_map_rgb(0, 0, 0), 190, 400, ALLEGRO_ALIGN_CENTER, "Escribe 0 para volver al menu");
    } else if(estadoActual == NUMERO_DE_CUENTA_EXISTENTE) {
        al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 70, ALLEGRO_ALIGN_CENTER, "EL numero de cuenta ya existe!");
        al_draw_text(font, al_map_rgb(0, 0, 0), 190, 400, ALLEGRO_ALIGN_CENTER, "Escribe 0 para volver");   
    }

    //Dibujar extras
    CrearBotones();
    DibujarDispensador();
}

void CajeroAutomatico::CrearBotones() {
    int xStart = 150, yStart = 500, btnSize = 60, gap = 10;

    // Teclado
    for (int i = 0; i < 9; i++) {
        int x = xStart + (i % 3) * (btnSize + gap);
        int y = yStart + (i / 3) * (btnSize + gap);

        // Fondo Boton
        al_draw_filled_rectangle(x, y, x + btnSize, y + btnSize, al_map_rgb(255, 255, 255));  // Botones blancos

        // Numero color negro
        al_draw_textf(font, al_map_rgb(0, 0, 0), x + btnSize / 2, y + btnSize / 3, ALLEGRO_ALIGN_CENTER, "%d", i + 1);
    }

    // Filas
    int zeroX = xStart + (btnSize + gap) * 1;
    int zeroY = yStart + 3 * (btnSize + gap);
    al_draw_filled_rectangle(zeroX, zeroY, zeroX + btnSize, zeroY + btnSize, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), zeroX + btnSize / 2, zeroY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "0");

    // Enter
    int enterX = xStart + (btnSize + gap) * 2;
    int enterY = zeroY;
    al_draw_filled_rectangle(enterX, enterY, enterX + btnSize, enterY + btnSize, al_map_rgb(34, 206, 40));
    al_draw_textf(font, al_map_rgb(0, 0, 0), enterX + btnSize / 2, enterY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "Enter");

    // Clear
    int clearX = enterX + btnSize + gap;
    al_draw_filled_rectangle(clearX, enterY, clearX + btnSize, enterY + btnSize, al_map_rgb(236, 45, 45));
    al_draw_textf(font, al_map_rgb(0, 0, 0), clearX + btnSize / 2, enterY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "CLR");

    // Punto
    int puntoX = xStart + (btnSize + gap) * 0;
    int puntoY = yStart + 3 * (btnSize + gap);
    al_draw_filled_rectangle(puntoX, puntoY, puntoX + btnSize, puntoY + btnSize, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), puntoX + btnSize / 2, puntoY + btnSize / 3, ALLEGRO_ALIGN_CENTER, ".");
}

void CajeroAutomatico::DibujarDispensador() {
    int xStart = 800, yStart = 150, width = 250, height = 500;
    
    al_draw_filled_rectangle(xStart, yStart, xStart + width, yStart + height, al_map_rgb(255, 255, 255));  // Fondo blanco del dispensador

    al_draw_text(font, al_map_rgb(0, 0, 0), xStart + width / 2, yStart - 20, ALLEGRO_ALIGN_CENTER, "Dispensador");

    for (int i = 0; i < 11  ; i++) {
        al_draw_textf(font, al_map_rgb(0, 0, 0), xStart + 10, yStart + (i + 1) * 40, ALLEGRO_ALIGN_LEFT, "%.2f Bs: %d", dispensador.denominaciones[i], dispensador.cantidadBilletes[i]);
    }
}


void CajeroAutomatico::DibujarRanura() {
    int btnSize = 60;
    int gap = 10;
    int xStart = 500; 
    int yStart = 500; 

    int width = (btnSize + gap) * 3;
    int height = (btnSize + gap) * 4;

    al_draw_filled_rectangle(xStart, yStart, xStart + width, yStart + height, al_map_rgb(255, 255, 255));

    al_draw_rectangle(xStart, yStart, xStart + width, yStart + height, al_map_rgb(0, 0, 0), 2); // Borde negro

}

void CajeroAutomatico::ProcesarEntrada(int x, int y) {
    int btnSize = 60, gap = 10;
    int xStart = 150, yStart = 500;

    float monto = 0.0;

    // Teclado
    for (int i = 0; i < 9; i++) {
        int xPos = xStart + (i % 3) * (btnSize + gap);
        int yPos = yStart + (i / 3) * (btnSize + gap);

        if (x >= xPos && x <= xPos + btnSize && y >= yPos && y <= yPos + btnSize) {
            inputActual += to_string(i + 1);
            return; 
        }
    }

    // 0
    int zeroX = xStart + (btnSize + gap) * 1;
    int zeroY = yStart + 3 * (btnSize + gap);
    if (x >= zeroX && x <= zeroX + btnSize && y >= zeroY && y <= zeroY + btnSize) {
        inputActual += "0";
        return;
    }

    // .
    int puntoX = xStart + (btnSize + gap) * 0; 
    int puntoY = yStart + 3 * (btnSize + gap); 
    if (x >= puntoX && x <= puntoX + btnSize && y >= puntoY && y <= puntoY + btnSize) {
        if (inputActual.find('.') == string::npos) { 
            inputActual += "."; 
        }
        return;
    }

    //  ENTER
    int enterX = xStart + (btnSize + gap) * 2;
    int enterY = zeroY;
    if (x >= enterX && x <= enterX + btnSize && y >= enterY && y <= enterY + btnSize) {

        switch (estadoActual) {
            case ESPERANDO_USUARIO:
                if (inputActual == "0") {
                    estadoActual = CREAR_CUENTA_ESPERAR_NUMERO_CUENTA;
                    inputActual.clear();
                }
                if (inputActual.empty()) {
                    cout << "ERROR: No se ingresó un número de cuenta." << endl;
                    return;
                }
                try {
                    numeroCuenta = stoi(inputActual);  // Convertir a numero de cuenta
                } catch (const std::invalid_argument&) {
                    cout << "ERROR: Número de cuenta inválido." << endl;
                    return;
                }

                if (sistema.existeCuenta(numeroCuenta)) {
                    estadoActual = ESPERANDO_NIP;
                    inputActual.clear();
                } else {
                    cout << "No se encontró la cuenta." << endl;
                    inputActual.clear();
                }
                break;

            case ESPERANDO_NIP:
                if (inputActual.empty()) {
                    cout << "ERROR: No se ingresó un NIP." << endl;
                    return;
                }
                try {
                    inicio_nip = stoi(inputActual);
                } catch (const std::invalid_argument&) {
                    cout << "ERROR: NIP inválido." << endl;
                    return;
                }

                if (sistema.verificarNip(numeroCuenta, inicio_nip)) {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                } else {
                    cout << "NIP incorrecto." << endl;
                    inputActual.clear();
                }
                break;

            case CREAR_CUENTA_ESPERAR_NUMERO_CUENTA:
                if (inputActual.empty()) {
                    cout << "ERROR No se ingresó un usuario." << endl;
                    return;
                }
                if(inputActual == "0") {
                    estadoActual = ESPERANDO_USUARIO;
                    inputActual.clear();
                    return;
                }
                if (inputActual.size() != 5) {
                    inputActual.clear();
                    return;
                } 
                try {
                    nuevo_usuario = stoi(inputActual);
                } catch(const std::invalid_argument&) {
                    return;
                }
                if(sistema.existeCuenta(nuevo_usuario)) {
                    estadoActual = NUMERO_DE_CUENTA_EXISTENTE;
                    inputActual.clear();
                    return;
                }
                estadoActual = CREAR_CUENTA_ESPERAR_NIP;
                inputActual.clear();
                break;
  
            case NUMERO_DE_CUENTA_EXISTENTE:
                if(inputActual == "0") {
                    estadoActual = CREAR_CUENTA_ESPERAR_NUMERO_CUENTA;
                    inputActual.clear();
                    return;
                }
                return;

            case CREAR_CUENTA_ESPERAR_NIP:
                if (inputActual.size() != 5) {
                    inputActual.clear();
                    return;
                }
                nuevo_nip = inputActual;
                estadoActual = CREAR_CUENTA_SALDO;
                inputActual.clear();
                break;

            case CREAR_CUENTA_SALDO:
                if (inputActual.empty()) {
                    cout << "ERROR: El saldo inicial no ha sido ingresado." << endl;
                    return;
                }

                try {
                    saldo_inicial = stof(inputActual);  // Convertir a saldo
                    if (saldo_inicial <= 0) {
                        cout << "ERROR: El saldo inicial debe ser mayor que 0." << endl;
                        return;
                    }
                    estadoActual = CREAR_CUENTA;
                    inputActual.clear();
                } catch (const std::invalid_argument&) {
                    cout << "ERROR: Saldo no válido." << endl;
                    return;
                }
                break;

            case CREAR_CUENTA:
                sistema.registrarCliente(nuevo_usuario, stoi(nuevo_nip), saldo_inicial, saldo_inicial, 0.0);
                estadoActual = CUENTA_CREADA_EXITO;
                inputActual.clear();
                break;

            case CUENTA_CREADA_EXITO:
                if (inputActual == "0") {
                    estadoActual = ESPERANDO_USUARIO;
                    inputActual.clear();
                }
                break;

            case MOSTRANDO_OPCIONES:
                if(inputActual.empty()) {
                    return;
                }
                if(inputActual == "0") {
                    estadoActual = ESPERANDO_USUARIO;
                    inputActual.clear();
                }
                if (inputActual == "1") {
                    estadoActual = ESPERANDO_MONTO_DEPOSITO;
                    inputActual.clear();
                } else if (inputActual == "2") {
                    estadoActual = ESPERANDO_MONTO_RETIRO;
                    inputActual.clear();
                } else if (inputActual == "3") {
                    estadoActual = VER_MOVIMIENTOS;
                    inputActual.clear();
                }
                break;

            case ESPERANDO_MONTO_RETIRO:
                if(inputActual.empty()) {
                    return;
                }

                monto = stof(inputActual);

                if(dispensador.retirarMonto(monto)) {
                    sistema.Retirar(numeroCuenta,monto);
                    estadoActual = MOSTRAR_CORTE_RETIRADO;
                    inputActual.clear();
                } else {
                    estadoActual = OPERACION_FALLIDA_RETIRO;
                    inputActual.clear();
                }
                break;
            
            case OPERACION_FALLIDA_RETIRO:
                 if(inputActual == "") {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                 }
                 
            case MOSTRAR_CORTE_RETIRADO:
                  if(inputActual == "0") {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                  }

            case VER_MOVIMIENTOS:
                  if(inputActual == "0") {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                  }

            case ESPERANDO_MONTO_DEPOSITO:
                  if(inputActual.empty()) {
                    return;
                  }

                  if(inputActual == "0") {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                  }

                  monto = stof(inputActual);
                  dispensador.aceptarDeposito(monto);
                  sistema.Depositar(numeroCuenta, monto);

                  estadoActual = MOSTRAR_DEPOSITO_EXITOSO;
                  inputActual.clear();
                  break;
            case MOSTRAR_DEPOSITO_EXITOSO:
                  if(inputActual.empty()) {
                    return;
                  }
                  if(inputActual == "0") {
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                  }
            default:
                cout << "ERROR: Estado no reconocido." << endl;
                break;
        }
    }

    // CLEAR
    int clearX = enterX + btnSize + gap; 
    if (x >= clearX && x <= clearX + btnSize && y >= enterY && y <= enterY + btnSize) {
        inputActual.clear(); 
        return;
    }
}

int main() {
    CajeroAutomatico cajero;
    cajero.Iniciar();
    return 0;
}
