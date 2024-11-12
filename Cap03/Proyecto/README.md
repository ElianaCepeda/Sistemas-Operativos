# Proyecto de Sistemas Operativos: Sistema de Distribución de Noticias

Este proyecto implementa un sistema de distribución de noticias usando procesos en C y comunicación mediante pipes nominales (FIFOs). El sistema permite que un `publicador` envíe noticias a un `sistema de comunicación`, el cual las distribuye a los `suscriptores` interesados en temas específicos.

## Estructura del Proyecto

- `src/`: Carpeta que contiene los archivos fuente (`.c` y `.h`).
  - **`publicador.c`**: Programa que lee noticias desde un archivo y las envía al sistema de comunicación.
  - **`suscriptor.c`**: Programa que permite a los usuarios suscribirse a temas específicos y recibir noticias relacionadas.
  - **`sistema_comunicacion.c`**: Programa que gestiona la recepción de noticias y las distribuye a los suscriptores interesados.
  - **`utils.h`**: Archivo de cabecera con funciones de utilidad, como la validación de formato de noticias.
- `data/`: Carpeta que contiene archivos de datos, como `noticias1.txt`, con las noticias para el programa `publicador`.
- `build/`: Carpeta donde se almacenan los archivos objeto (`.o`) generados y los ejecutables resultantes.

## Compilación

Este proyecto incluye un `Makefile` para facilitar la compilación de los componentes.

### Requisitos Previos

- **GCC**: Compilador de C para compilar los archivos fuente.

### Pasos de Compilación

Desde la raíz del proyecto, ejecuta el siguiente comando:

```bash
make
```

Este comando compilará los archivos en `src/` y generará los ejecutables `publicador`, `suscriptor`, y `sistema` en el directorio de trabajo. Los archivos objeto se almacenarán en `build/`.

### Limpieza de Archivos

Para eliminar los archivos generados por la compilación, incluyendo el directorio `build/` y los pipes creados (`pipePSC` y `pipeSSC`), usa:

```bash
make clean
```

## Ejecución del Proyecto en Múltiples Consolas

Para ejecutar el sistema correctamente, sigue estos pasos en tres consolas distintas:

### 1. Sistema de Comunicación (Consola 1)

El `sistema` debe ejecutarse primero, ya que es responsable de crear los pipes y gestionar la comunicación. Ejecuta el siguiente comando en **Consola 1**:

```bash
./sistema -p pipePSC -s pipeSSC -t 10
```

- `-p pipePSC`: Nombre del pipe nominal que los publicadores usarán para enviar noticias.
- `-s pipeSSC`: Nombre del pipe nominal que los suscriptores usarán para enviar suscripciones.
- `-t 10`: Tiempo de inactividad (en segundos) después del cual el sistema finalizará si no recibe nuevas noticias.

**Ejemplo de salida esperada**:
```
>>> SYSTEM STARTED <<<
```

### 2. Publicador (Consola 2)

Una vez que el `sistema` esté en ejecución y haya mostrado `>>> SYSTEM STARTED <<<`, ejecuta el `publicador` en **Consola 2** para enviar noticias:

```bash
./publicador -p pipePSC -f data/noticias1.txt -t 3
```

- `-p pipePSC`: Nombre del pipe nominal que `sistema` usa para recibir noticias.
- `-f data/noticias1.txt`: Archivo de texto que contiene las noticias.
- `-t 3`: Intervalo de tiempo (en segundos) entre el envío de cada noticia.

**Ejemplo de salida esperada**:
```
Noticia enviada: A: Exposición de arte moderno en el museo local.
Noticia enviada: P: Las elecciones se celebrarán el próximo mes.
...
```

### 3. Suscriptor (Consola 3)

Después de iniciar el `sistema`, puedes ejecutar el `suscriptor` en **Consola 3** para suscribirse a temas de interés y recibir noticias:

```bash
./suscriptor -s pipeSSC
```

- `-s pipeSSC`: Nombre del pipe nominal que el `sistema` usa para recibir suscripciones.

Al iniciar, el programa solicitará al usuario que ingrese los temas de interés. Puedes ingresar una o más letras que representen los temas (e.g., `A E S`).

**Ejemplo de interacción**:
```
Enter topics of interest (e.g., A E S): A
Waiting for news on selected topics...
```

## Archivo de Noticias (`noticias1.txt`)

El archivo de noticias (`data/noticias1.txt`) debe tener el siguiente formato:

```
A: Exposición de arte moderno en el museo local.
P: Las elecciones se celebrarán el próximo mes.
E: La famosa banda de rock se presenta este fin de semana.
C: Científicos descubren una nueva especie de insecto.
S: Robo en una joyería del centro comercial.
```

Cada línea representa una noticia que comienza con una letra en mayúscula (el tipo de noticia) seguida de `:` y el contenido de la noticia.

## Ejemplo Completo de Flujo de Trabajo

1. **Inicia el sistema** en **Consola 1**:
   ```bash
   ./sistema -p pipePSC -s pipeSSC -t 10
   ```

2. **Ejecuta el publicador** en **Consola 2**:
   ```bash
   ./publicador -p pipePSC -f data/noticias1.txt -t 3
   ```

3. **Ejecuta uno o más suscriptores** en **Consola 3**:
   ```bash
   ./suscriptor -s pipeSSC
   ```

   Ingresa los temas de interés cuando se te solicite (e.g., `A` para recibir noticias de arte).

4. El `sistema` distribuirá las noticias de acuerdo con los temas especificados por cada `suscriptor`.

## Notas Finales

- Asegúrate de que los pipes `pipePSC` y `pipeSSC` no existan antes de iniciar el `sistema`, ya que serán creados automáticamente.
- Los `suscriptores` deben eliminar su pipe único (`/tmp/pipeS_<PID>`) al finalizar la transmisión.
- La finalización de la transmisión es automática después de un período de inactividad especificado en `sistema`.
