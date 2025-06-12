#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <string> 

using namespace std;

enum Estado {
    ESPERANDO_USUARIO,
    ESPERANDO_PIN,
    MOSTRANDO_OPCIONES,
    ESPERANDO_MONTO,
    SELECCIONAR_MONTO,
    OPERACION_FINALIZADA_DEPOSITO,
    OPERACION_FINALIZADA_RETIRO,
    OPERACION_FALLIDA,
};

// Cuenta
class Cuenta {
    private:
    int numeroCuenta;
    int nip;
    double saldo;
    double saldoPendiente;

    public:
    Cuenta(int cuentaIngresada,int nipIngresado,double saldoInicial) {
        numeroCuenta = cuentaIngresada;
        nip = nipIngresado;
        saldo = saldoInicial;
        saldoPendiente = 0.0;
    }

    int obtenerNumeroCuenta() const {return numeroCuenta; }
    bool esNipValido(int nipUsuario) const { return nip == nipUsuario; }
    double obtenerSaldo() const {return saldo; }
    double obtenerSaldoPendiente() const {return saldoPendiente; }
    double obtenerSaldoTotal() const {return saldo + saldoPendiente; }
    bool retirar(double monto) {
        if(monto > saldo) return false;
        saldo -= monto;
        return true;
    }
    void depositarPendiente(double monto) {
        saldoPendiente += monto;
    }
    void verificarDeposito() {
        saldo += saldoPendiente;
        saldoPendiente = 0.0;
    }
};

//Base de datos
class BD {
    private:
    Cuenta** listaCuentas;

    public:
    BD() {
        listaCuentas = new Cuenta*[3];
        listaCuentas[0] = new Cuenta(12345, 11111, 1000.0);
        listaCuentas[1] = new Cuenta(23456, 22222, 500.0);
        listaCuentas[2] = new Cuenta(34567, 33333, 750.0);
        listaCuentas[2] = new Cuenta(15151, 22222, 100.0);
    }

    Cuenta* autenticarUsuario(int cuenta,int nip) {
        for(int i = 0 ; i < 3 ; i++){
            if(listaCuentas[i]->obtenerNumeroCuenta() == cuenta && listaCuentas[i]->esNipValido(nip)) {
                return listaCuentas[i];
        }
    }

    return nullptr;
}
    ~BD() {
        for (int i = 0; i < 3; i++) {
            delete listaCuentas[i];
        }
        delete[] listaCuentas;
    }
};

class CajeroAutomatico {
public:
    CajeroAutomatico();
    ~CajeroAutomatico();
    void Iniciar();

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;

    //Manejo de estados y entradas
    Estado estadoActual;
    string inputActual;

    // Logica General
    BD bd;
    int numeroCuenta;
    Cuenta* Token;

    // Allegro Logica
    void DibujarPantalla();
    void CrearBotones();
    void ProcesarEntrada(int x, int y);
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
    display = al_create_display(800, 600);
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
    double last_time = al_get_time();
    while (true) {
        double current_time = al_get_time();
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

                    // Llamamos a Procesar Entrada con las coordenadas del clic
                    ProcesarEntrada(x, y);
                }
            }

            DibujarPantalla();
            al_flip_display();
        }
    }
}

void CajeroAutomatico::DibujarPantalla() {
    al_clear_to_color(al_map_rgb(169, 169, 169)); // Fondo gris

    // Dibujar la pantalla (pantalla de estado)
    al_draw_filled_rectangle(50, 30, 750, 150, al_map_rgb(255, 255, 255));  // Pantalla blanca
    al_draw_text(font, al_map_rgb(0, 0, 0), 400, 100, ALLEGRO_ALIGN_CENTER, inputActual.c_str());  // Mostrar el número

    // Mostrar el mensaje en la pantalla dependiendo del estado
    if(estadoActual == ESPERANDO_USUARIO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 70, ALLEGRO_ALIGN_CENTER, "Bienvenido,Ingrese su Numero de cuenta");  // Mostrar mensaje Ingrese su Numero de cuenta
    }
    else if (estadoActual == ESPERANDO_PIN) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 70, ALLEGRO_ALIGN_CENTER, "Ingrese su NIP:");  // Mostrar mensaje Ingrese su NIP
    } else if (estadoActual == MOSTRANDO_OPCIONES) {
        char saldoFormateado[50];
        sprintf(saldoFormateado, "BOB %.2f", Token->obtenerSaldo());
        al_draw_text(font, al_map_rgb(0, 0, 0), 150, 50, ALLEGRO_ALIGN_CENTER, saldoFormateado);
        al_draw_text(font, al_map_rgb(0, 0, 0), 300, 50, ALLEGRO_ALIGN_CENTER, "1. Depositar");
        al_draw_text(font, al_map_rgb(0, 0, 0), 450, 50, ALLEGRO_ALIGN_CENTER, "2. Retirar");
    } else if (estadoActual == ESPERANDO_MONTO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 50, ALLEGRO_ALIGN_CENTER, "Monto en centavos:");
        al_draw_text(font, al_map_rgb(0, 0, 0), 320, 100, ALLEGRO_ALIGN_CENTER, "BOB");
    }

    else if(estadoActual == SELECCIONAR_MONTO) {
        char saldoFormateado[50];
        sprintf(saldoFormateado, "BOB %.2f", Token->obtenerSaldo());
        al_draw_text(font, al_map_rgb(0, 0, 0), 150, 50, ALLEGRO_ALIGN_CENTER, saldoFormateado);
        al_draw_text(font, al_map_rgb(0, 0, 0), 300, 50, ALLEGRO_ALIGN_CENTER, "1. 10");
        al_draw_text(font, al_map_rgb(0, 0, 0), 450, 50, ALLEGRO_ALIGN_CENTER, "2. 20");
        al_draw_text(font, al_map_rgb(0, 0, 0), 300, 60, ALLEGRO_ALIGN_CENTER, "3. 50");
        al_draw_text(font, al_map_rgb(0, 0, 0), 450, 60, ALLEGRO_ALIGN_CENTER, "4. 100");
        al_draw_text(font, al_map_rgb(0, 0, 0), 300, 70, ALLEGRO_ALIGN_CENTER, "5. 200");
        al_draw_text(font, al_map_rgb(0, 0, 0), 450, 70, ALLEGRO_ALIGN_CENTER, "6. Cancelar");
    }

    else if(estadoActual == OPERACION_FINALIZADA_RETIRO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 70, ALLEGRO_ALIGN_CENTER, "Retiro exitoso , retire el dinero porfavor.");
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "1. Otra Operacion | 2. Salir ");

    }

    else if(estadoActual == OPERACION_FALLIDA) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 70, ALLEGRO_ALIGN_CENTER, "Retiro fallido. Saldo insuficiente");
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "1. Otra Operacion | 2. Salir ");
    }

    else if(estadoActual == OPERACION_FINALIZADA_DEPOSITO) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 70, ALLEGRO_ALIGN_CENTER, "Dinero recibido , se encuentra pendiente.");
        al_draw_text(font, al_map_rgb(0, 0, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "1. Otra Operacion | 2. Salir ");
    } 

    // Crear los botones dentro del teclado
    CrearBotones();
}

void CajeroAutomatico::CrearBotones() {
    int xStart = 150, yStart = 200, btnSize = 60, gap = 10;

    // Crear los botones del 1 al 9 dentro del teclado
    for (int i = 0; i < 9; i++) {
        int x = xStart + (i % 3) * (btnSize + gap);
        int y = yStart + (i / 3) * (btnSize + gap);

        // Fondo Boton
        al_draw_filled_rectangle(x, y, x + btnSize, y + btnSize, al_map_rgb(255, 255, 255));  // Botones blancos

        // Numero color negro
        al_draw_textf(font, al_map_rgb(0, 0, 0), x + btnSize / 2, y + btnSize / 3, ALLEGRO_ALIGN_CENTER, "%d", i + 1);
    }

    // Filas
    int zeroX = xStart + (btnSize + gap) * 1;  // Ajuste para la posición correcta de 0
    int zeroY = yStart + 3 * (btnSize + gap);  // En la fila de Enter y Clear
    al_draw_filled_rectangle(zeroX, zeroY, zeroX + btnSize, zeroY + btnSize, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), zeroX + btnSize / 2, zeroY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "0");

    // Enter
    int enterX = xStart + (btnSize + gap) * 2;  // Ajuste para Enter
    int enterY = zeroY;
    al_draw_filled_rectangle(enterX, enterY, enterX + btnSize, enterY + btnSize, al_map_rgb(34, 206, 40)); // RGB Verde
    al_draw_textf(font, al_map_rgb(0, 0, 0), enterX + btnSize / 2, enterY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "Enter");

    // Clear
    int clearX = enterX + btnSize + gap;  // Ajuste para Clear
    al_draw_filled_rectangle(clearX, enterY, clearX + btnSize, enterY + btnSize, al_map_rgb(236, 45, 45)); // RGB Rojo
    al_draw_textf(font, al_map_rgb(0, 0, 0), clearX + btnSize / 2, enterY + btnSize / 3, ALLEGRO_ALIGN_CENTER, "CLR");
}



void CajeroAutomatico::ProcesarEntrada(int x, int y) {
    int btnSize = 60, gap = 10;
    int xStart = 150, yStart = 200;

    // Procesar los botones del 1 al 9
    for (int i = 0; i < 9; i++) {
        int xPos = xStart + (i % 3) * (btnSize + gap);
        int yPos = yStart + (i / 3) * (btnSize + gap);
        if (x >= xPos && x <= xPos + btnSize && y >= yPos && y <= yPos + btnSize) {
             inputActual += to_string(i + 1);  // Agregar el digito al numero           
            return;
        }
    }

    // BOTON 0
    int zeroX = xStart + (btnSize + gap) * 1;  // Ajustado a la posición correcta de 0
    int zeroY = yStart + 3 * (btnSize + gap); 
    if (x >= zeroX && x <= zeroX + btnSize && y >= zeroY && y <= zeroY + btnSize) {
        inputActual += "0";  // Agregar 0
        return;
    }

    // BOTON ENTER
    int enterX = xStart + (btnSize + gap) * 2;  // Ajuste para "Enter"
    int enterY = zeroY;
    if (x >= enterX && x <= enterX + btnSize && y >= enterY && y <= enterY + btnSize) {

        //Esperando Usuario
        if(estadoActual == ESPERANDO_USUARIO) {
            if(inputActual.size() == 5) {
                numeroCuenta = stoi(inputActual);
                estadoActual = ESPERANDO_PIN;
                inputActual.clear();
            } else {
                inputActual.clear();
            }
        }

        //Esperando PIN
        else if (estadoActual == ESPERANDO_PIN) {
            if (inputActual.size() == 5) {  // Validar el PIN
                Cuenta* Autenticacion = bd.autenticarUsuario(numeroCuenta,stoi(inputActual)); //Crear Token
                
                if(Autenticacion != nullptr) {
                    Token = Autenticacion; // Asignar Token Cajero Automatico
                    estadoActual = MOSTRANDO_OPCIONES;
                    inputActual.clear();
                } else {
                inputActual.clear();
                }
            }

        //Mostrando Opciones
        } else if (estadoActual == MOSTRANDO_OPCIONES) {
            if (inputActual == "1") {
                estadoActual = ESPERANDO_MONTO;
                inputActual.clear();
            } else if (inputActual == "2") {
                estadoActual = SELECCIONAR_MONTO;
                inputActual.clear();
            }
        } else if (estadoActual == SELECCIONAR_MONTO) {
            if(stoi(inputActual) <= 6) {
            if(inputActual == "1" && Token->retirar(10)) {
                estadoActual = OPERACION_FINALIZADA_RETIRO;
            }
            else if(inputActual == "2" && Token->retirar(20) ) {
                estadoActual = OPERACION_FINALIZADA_RETIRO;
            }
            else if(inputActual == "3" && Token->retirar(50)) {
                estadoActual = OPERACION_FINALIZADA_RETIRO;
            }
            else if(inputActual == "4" && Token->retirar(100)) {
                estadoActual = OPERACION_FINALIZADA_RETIRO;
            }
            else if(inputActual == "5" && Token->retirar(200)) {
                estadoActual = OPERACION_FINALIZADA_RETIRO;
            }
            else if(inputActual == "6") {
                estadoActual = MOSTRANDO_OPCIONES;
                inputActual.clear();
            }
            else {
                estadoActual = OPERACION_FALLIDA;
                inputActual.clear();
            }
        }
        else {
            inputActual.clear();
        }
        inputActual.clear();
    }
        else if(estadoActual == OPERACION_FINALIZADA_RETIRO) {
            if(inputActual == "1") {
                estadoActual = MOSTRANDO_OPCIONES;
                inputActual.clear();
            } 
            else if(inputActual == "2") {
                estadoActual = ESPERANDO_USUARIO;
                inputActual.clear();
            }
        }

        else if(estadoActual == OPERACION_FALLIDA) {
            if(inputActual == "1") {
                estadoActual = MOSTRANDO_OPCIONES;
                inputActual.clear();
            } 
            else if(inputActual == "2") {
                estadoActual = ESPERANDO_USUARIO;
                inputActual.clear();
            }
        }

        else if(estadoActual == ESPERANDO_MONTO) {
            if(stoi(inputActual) > 0) {
                Token->depositarPendiente(stoi(inputActual)/100);
                estadoActual = OPERACION_FINALIZADA_DEPOSITO;
                inputActual.clear();
            } else {
                estadoActual = MOSTRANDO_OPCIONES;
                inputActual.clear();
            }
        }

        else if(estadoActual == OPERACION_FINALIZADA_DEPOSITO) {
            if(inputActual == "1") {
                estadoActual = MOSTRANDO_OPCIONES;
                inputActual.clear();
            } 
            else if(inputActual == "2") {
                estadoActual = ESPERANDO_USUARIO;
                inputActual.clear();
            }
        }
        return;
    }

    // CLEAR
    int clearX = enterX + btnSize + gap;  // Posicionar
    if (x >= clearX && x <= clearX + btnSize && y >= enterY && y <= enterY + btnSize) {
        inputActual.clear();  // Limpiar
        return;
    }
}

int main() {
    CajeroAutomatico cajero;
    cajero.Iniciar();
    return 0;
}
 
