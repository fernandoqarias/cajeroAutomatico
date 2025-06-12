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
