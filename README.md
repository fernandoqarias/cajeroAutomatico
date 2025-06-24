# ATM
### Cajero Automático - Simulación con Paradigma Orientado a Objetos

## Descripción
Este proyecto simula un cajero automático (ATM) utilizando el paradigma de programación orientada a objetos en C++. El sistema permite a los usuarios realizar operaciones bancarias como consultas de saldo, retiros y depósitos, todo a través de una interfaz gráfica simulada con la librería **Allegro**. 
El sistema también incluye funcionalidades como la creación de cuentas y validación de NIP, así como una simulación realista de la gestión de billetes y monedas.

## Características
- **Autenticación de Usuario:** Validación mediante número de cuenta y NIP.
- **Creación de Cuentas:** Permite crear una cuenta bancaria con saldo inicial.
- **Consulta de Saldo:** Muestra el saldo actual de la cuenta del usuario.
- **Retiro de Efectivo:** Permite realizar retiros con billetes y monedas disponibles.
- **Depósito de Efectivo:** Permite realizar depósitos en bolivianos.
- **Gestión de Archivos:** El sistema maneja archivos de texto para almacenar la información de las cuentas bancarias.

## Tecnologías Utilizadas
- **Lenguaje:** C++
- **Librerías:** [Allegro 5](https://liballeg-org.translate.goog/index.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc)
- **Paradigma:** Orientación a Objetos
- **Archivos de Datos:** Archivos de texto para almacenar la información de las cuentas bancarias [fstream](https://cplusplus-com.translate.goog/reference/fstream/fstream/?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc).

## Instalación

1. **Clonar el repositorio:**
   ```bash
   git clone https://github.com/fernandoqarias/cajeroAutomatico.git
   ```
2. Instalar Allegro 5
   Asegurate de tener Allegro 5 instalado en tu maquina version utilizada
   en este proyecto.
   Para este proyecto usamos la ultima version de los repositorios de [archlinux](https://wiki.archlinux.org/title/Allegro)<br>
  Name            : allegro<br>
  Version         : 5.2.10.1-4<br>
  Puedes descargar esta version para windows en este [link](https://liballeg-org.translate.goog/download.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc)<br>

3. Compilar Proyecto
   Si tienes un entorno como [g++](https://www.fdi.ucm.es/profesor/luis/fp/devtools/mingw.html)
   ```bash
   g++ -o atm atm.cpp -lallegro -lallegro_primitives -lallegro_font
   ./atm
   ```
## Uso 
Al iniciar el programa, el sistema pedirá que ingreses tu número de cuenta.
Luego, debes ingresar tu NIP para acceder a las funcionalidades del cajero.
Una vez autenticado, podrás elegir entre consultar tu saldo, retirar dinero o realizar un depósito.
Flujo de operaciones<br>
Inicio: Solicita número de cuenta y NIP.
Menú Principal: Permite elegir entre consultar saldo, retirar dinero, hacer un depósito o salir.
Retiro: Elige un monto predefinido o ingresa uno manualmente.
Depósito: Ingresa la cantidad que deseas depositar.

### Consulta de Saldo<br>
Después de ingresar tu número de cuenta y NIP, puedes consultar tu saldo actual en el ATM.

### Retiro de Efectivo<br>
El cajero mostrará las opciones de retiro, como $20, $50, $100, etc. Si el saldo es suficiente, el monto será retirado y actualizado en la base de datos del sistema.

## Contribución
Si deseas contribuir a este proyecto, por favor sigue los siguientes pasos:
Haz un fork de este repositorio.
```git checkout -b feature/nueva-funcionalidad```
Realiza tus cambios con un commit y realiza tu push.
Abre un pull request describiendo tu cambio gracias!.

## Imagen de la Interfaz
![Imagen](https://media.discordapp.net/attachments/1371708428753440820/1384463412385550358/Shot-2025-06-17-052249.png?ex=68528587&is=68513407&hm=fb0d7868c94f25182a695fa141e9bfdbe27f749cb324ce5454682e879f13e609&=&format=webp&quality=lossless&width=1132&height=796)
## Autores
Oscar Santiago Abuawad Velasco - oscar.abuawad@ucb.edu.bo

Fernando Quiroz - fernando.quiroz@ucb.edu.bo


## Agradecimientos
Gracias al profesor Ing Erik Pozo Irusta por su orientación en el desarrollo de este proyecto en la materia Programacion 1 en la Universidad Catolica Sede Santa Cruz.

## Contacto
Si tienes preguntas o sugerencias, no dudes en contactarnos a través de los correos electrónicos proporcionados.
