# Guía de Implementación del Cajero Automático (ATM)

Este repositorio contiene el código fuente para un **Cajero Automático (ATM)** simulado utilizando **Programación Orientada a Objetos (OOP)** y la biblioteca **Allegro** para la interfaz gráfica. El sistema permite a los usuarios realizar transacciones como consultar saldo, realizar retiros y depósitos.

## Estructura del Proyecto

El código está organizado en varios módulos que gestionan diferentes aspectos del sistema. Aquí se describen las clases principales y los módulos del sistema.

---

## Clases Principales

### 1. **Clase `Cuenta`**

La clase `Cuenta` representa una cuenta bancaria de un usuario. Se encarga de gestionar el saldo, validar el PIN, y realizar operaciones de retiro y depósito.

#### Métodos:
- **`obtenerNumeroCuenta()`**: Retorna el número de cuenta.
- **`esNipValido(int nipUsuario)`**: Verifica si el NIP proporcionado por el usuario es válido.
- **`obtenerSaldo()`**: Obtiene el saldo actual de la cuenta.
- **`obtenerSaldoPendiente()`**: Devuelve el saldo pendiente (depósitos en espera).
- **`obtenerSaldoTotal()`**: Suma el saldo y el saldo pendiente.
- **`retirar(double monto)`**: Realiza un retiro de dinero si el monto es válido.
- **`depositarPendiente(double monto)`**: Realiza un depósito pendiente (espera de verificación).
- **`verificarDeposito()`**: Suma el depósito pendiente al saldo total de la cuenta.

```cpp
class Cuenta {
private:
    int numeroCuenta;
    int nip;
    double saldo;
    double saldoPendiente;

public:
    Cuenta(int cuentaIngresada, int nipIngresado, double saldoInicial);
    int obtenerNumeroCuenta() const;
    bool esNipValido(int nipUsuario) const;
    double obtenerSaldo() const;
    double obtenerSaldoPendiente() const;
    double obtenerSaldoTotal() const;
    bool retirar(double monto);
    void depositarPendiente(double monto);
    void verificarDeposito();
};
```

### 2. **Clase BD (Base de Datos)**

La clase `BD` simula la base de datos que almacena las cuentas de los usuarios. Permite autenticar usuarios y administrar la lista de cuentas.

#### Métodos:
- **`autenticarUsuario(int cuenta, int nip)`**: Verifica si el número de cuenta y el NIP corresponden a un usuario válido.

```cpp
class BD {
private:
    Cuenta** listaCuentas;

public:
    BD();
    Cuenta* autenticarUsuario(int cuenta, int nip);
    ~BD();
};

```
### 3. **Clase CajeroAutomatico**

La clase `CajeroAutomatico` gestiona la interacción con el usuario a través de una interfaz gráfica, utilizando la biblioteca **Allegro**. Permite realizar operaciones como consultas de saldo, depósitos y retiros.

#### Métodos:
- **`Iniciar()`**: Inicia el cajero y controla el flujo de las transacciones.
- **`DibujarPantalla()`**: Dibuja la interfaz gráfica del cajero.
- **`CrearBotones()`**: Crea los botones interactivos para la interfaz.
- **`ProcesarEntrada(int x, int y)`**: Procesa la entrada del usuario basada en las coordenadas del clic.

```cpp
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
    Estado estadoActual;
    string inputActual;
    BD bd;
    int numeroCuenta;
    Cuenta* Token;

    void DibujarPantalla();
    void CrearBotones();
    void ProcesarEntrada(int x, int y);
};
```

## Módulos del Proyecto

### 1. **Interfaz Gráfica con Allegro**

La interfaz gráfica se desarrolla utilizando la biblioteca **Allegro**. A través de Allegro, se gestionan eventos de entrada (teclado, ratón) y la visualización de los diferentes estados del sistema.

#### Funcionalidades:
- Pantalla de bienvenida para ingresar el número de cuenta.
- Pantalla para ingresar el NIP de la cuenta.
- Menú de opciones para consultar saldo, realizar depósitos y retiros.
- Botones interactivos para las diferentes opciones.

### 2. **Manejo de Estados del Sistema**

El sistema utiliza una **máquina de estados** para gestionar las transiciones entre los diferentes estados de la aplicación, como la espera de la cuenta, la entrada del NIP y las operaciones de retiro y depósito.

#### Estados del Sistema:
- **ESPERANDO_USUARIO**: Estado en que se espera la entrada del número de cuenta.
- **ESPERANDO_PIN**: Estado en que se espera la entrada del NIP.
- **MOSTRANDO_OPCIONES**: Estado en que el usuario ve las opciones de transacción (consultar saldo, retirar, depositar).
- **SELECCIONAR_MONTO**: Estado en que el usuario selecciona el monto a retirar.
- **OPERACION_FINALIZADA_RETIRO**: Estado cuando la operación de retiro se completa.
- **OPERACION_FINALIZADA_DEPOSITO**: Estado cuando el depósito es exitoso.
- **OPERACION_FALLIDA**: Estado cuando ocurre un error en la transacción.

---

## Instalación

### En **Arch Linux**:

1. **Clonar el repositorio:**

```bash
   git clone https://github.com/tu_usuario/atm.git
   cd atm
```
## Instalación

1. **Instalar Allegro:**

   En **Arch Linux**:

```bash
   sudo pacman -S allegro
```
### En **Windows**:

Para instalar Allegro en **Windows**, sigue estos pasos:

1. **Descargar Allegro**:
   - Ve a la página oficial de Allegro: [Allegro Official Website](https://liballeg.org/).
   - Descarga el archivo instalador o los archivos binarios.

2. **Compilar el Proyecto**:
   - Asegúrate de que Allegro esté correctamente instalado y configurado.
   - Usa un compilador como **MinGW** o **Visual Studio** para compilar el proyecto en Windows.

## Compilar el Proyecto:
```
g++ -o cajero atm_cajero.cpp -lallegro -lallegro_primitives -lallegro_font
```


