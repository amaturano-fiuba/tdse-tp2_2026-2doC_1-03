# Análisis del Firmware: STM32 Bare Metal (Event-Triggered System) ⏱️⚙️

El código proporcionado implementa una arquitectura **"Bare Metal - Event-Triggered Systems (ETS)"** para un microcontrolador STM32. En este modelo, la ejecución de las tareas es impulsada por el tiempo, basándose en un "tick" o latido del sistema.

A continuación, se detalla la función de cada archivo y la evolución de las variables clave utilizadas para el perfilamiento (profiling) del sistema.

---

## 📂 Funcionalidad de los Archivos

*   **`app.c`**: Es el núcleo de la aplicación. Inicializa el sistema, registra las tareas (sensor, sistema y actuador) y maneja el bucle principal (`app_update()`) para ejecutarlas secuencial y periódicamente, midiendo su tiempo de ejecución.
*   **`app_it.c`**: Maneja las rutinas de interrupción específicas de la aplicación, como la actualización de los ticks mediante `HAL_SYSTICK_Callback()` y las interrupciones externas (ej. pulsación de un botón).
*   **`systick.c`**: Proporciona una función de retardo bloqueante en microsegundos (`systick_delay_us()`) interactuando directamente con los registros del hardware SysTick.
*   **`logger.c` / `logger.h`**: Implementan un sistema de registro para enviar mensajes (logs) utilizando *Semihosting*, imprimiendo a través del depurador hacia la consola del host. Estas macros desactivan las interrupciones temporalmente mientras imprimen para asegurar la atomicidad de la operación.
*   **`dwt.h`**: Contiene funciones *inline* para interactuar con el *Data Watchpoint and Trace (DWT)* del procesador Cortex-M. Esto permite un conteo de ciclos de reloj extremadamente preciso, ideal para perfilar el tiempo que tarda una función en ejecutarse (en microsegundos).

---

## 🚀 Flujo de Ejecución General

1.  **Inicialización (`app_init`)**: Se configuran las tareas, se reinician los contadores del sistema y de perfilamiento (DWT), y se habilitan las interrupciones.
2.  **Bucle Principal (`app_update`)**: El sistema verifica continuamente si ha ocurrido unAquí tienes el texto estructurado y formateado en Markdown, ideal para copiar y pegar directamente en tu archivo `README.md`. He organizado la información con encabezados, listas y negritas para que sea fácil de leer y escanear.

***

```markdown
# Análisis del Sistema: Bare Metal - Event-Triggered System (ETS)

Este proyecto implementa una arquitectura "Bare Metal - Event-Triggered Systems (ETS)" para un microcontrolador STM32. La ejecución de las tareas es impulsada por el tiempo a través de un "tick" del sistema. A continuación, se detalla la arquitectura, el propósito de cada archivo y el comportamiento de las variables clave.

## ⚙️ Funcionamiento General

El sistema utiliza un temporizador (SysTick) para generar interrupciones periódicas que actúan como la base de tiempo. El flujo de ejecución es el siguiente:

1. **Inicialización (`app_init`):** Se configuran las tareas, se reinician los contadores del sistema y de perfilamiento (DWT), y se habilitan las interrupciones.
2. **Bucle Principal (`app_update`):** El sistema verifica continuamente si ha ocurrido un "tick" del sistema (evento de tiempo). Si es así, ejecuta secuencialmente todas las tareas definidas en `task_cfg_list` (sensor, sistema, actuador).
3. **Perfilamiento (Profiling):** Durante la ejecución de las tareas, el sistema mide cuánto tiempo (en microsegundos) tarda en ejecutarse cada una utilizando el contador de ciclos del hardware (DWT).

---

## 📂 Funcionalidad de los Archivos

* **`app.c`**: Es el núcleo de la aplicación. Inicializa el sistema, registra las tareas y maneja el bucle principal (`app_update()`) para ejecutarlas periódicamente y medir su tiempo de ejecución.
* **`app_it.c`**: Maneja las rutinas de interrupción específicas de la aplicación, como la actualización de los ticks mediante `HAL_SYSTICK_Callback()` y las interrupciones externas (ej. pulsación de botones).
* **`systick.c`**: Proporciona una función de retardo bloqueante en microsegundos (`systick_delay_us()`) utilizando directamente los registros del hardware SysTick.
* **`logger.c` / `logger.h`**: Implementa un sistema de registro para enviar mensajes, utilizando *Semihosting* para imprimir a través del depurador. Desactiva las interrupciones mientras imprime para asegurar la atomicidad.
* **`dwt.h`**: Contiene funciones en línea para interactuar con el Data Watchpoint and Trace (DWT) del procesador Cortex-M. Permite un conteo de ciclos de reloj preciso para perfilar el tiempo de ejecución en microsegundos.

---

## 📊 Evolución de Variables Clave

### Variables Globales
* **`g_app_tick_cnt`**: 
  * *Inicio:* Se inicializa en `0` en `app_it_init()`.
  * *Evolución:* Se incrementa en `1` con cada interrupción en `HAL_SYSTICK_Callback()`. Se decrementa en `1` dentro de `app_update()` al procesar el evento de tiempo.
* **`g_app_runtime_us` (µs)**: 
  * *Inicio:* Asume el valor acumulado al iniciar el bucle.
  * *Evolución:* En cada ciclo dentro de `app_update()`, se reinicia a `0` y acumula el tiempo de ejecución (LET) de todas las tareas ejecutadas.
* **`index`**: 
  * *Inicio:* En `app_init()`, itera de `0` a `TASK_QTY - 1` para inicializar tareas.
  * *Evolución:* En `app_update()`, se usa como contador del bucle para ejecutar y medir cada tarea.

### Estructura de Tareas (`task_dta_list[index]`)
* **`NOE` (Number of Execution - Adimensional)**: 
  * *Inicio:* Inicializado en `0` (`TASK_X_NOE_INI`).
  * *Evolución:* Incrementa en `1` cada vez que la tarea se ejecuta.
* **`LET` (Last Execution Time - µs)**: 
  * *Inicio:* Inicializado en `0` (`TASK_X_LET_INI`).
  * *Evolución:* Toma el valor del tiempo transcurrido obtenido por `cycle_counter_get_time_us()` al finalizar la tarea.
* **`BCET` (Best-Case Execution Time - µs)**: 
  * *Inicio:* Inicializado en `1000` (`TASK_X_BCET_INI`).
  * *Evolución:* Si el `LET` actual es menor que el `BCET` registrado, se actualiza con el nuevo tiempo mínimo.
* **`WCET` (Worst-Case Execution Time - µs)**: 
  * *Inicio:* Inicializado en `0` (`TASK_X_WCET_INI`).
  * *Evolución:* Si el `LET` actual es mayor que el `WCET` registrado, se actualiza con el nuevo tiempo máximo.

---

## ⚠️ Impacto de usar `LOGGER_INFO()`

La macro `LOGGER_INFO()` formatea cadenas con `snprintf` y envía datos vía *semihosting*, deshabilitando temporalmente las interrupciones (`__asm("CPSID i")`). Usarla dentro de la función de actualización de una tarea tiene un impacto drástico:

* **Impacto en `WCET`**: Las operaciones de I/O bloqueantes y el *semihosting* detienen el procesador para comunicarse con el depurador. Esto causa un pico masivo en el `LET` de la tarea, actualizando el `WCET` con un valor extremadamente alto.
* **Impacto en `g_app_runtime_us`**: Como esta variable suma el `LET` de todas las tareas, el retraso del logger incrementará significativamente el tiempo total de ejecución. Esto puede causar que el sistema exceda el tiempo disponible (1 ms) antes del siguiente evento del temporizador, rompiendo el determinismo del sistema.

## Registro de valores de `task_dta_list[index]` (Paso 13)

Valores obtenidos luego de varias ejecuciones de `app_update()`:

| Tarea (index) | NOE (Número de ejecuciones) | LET (Último tiempo) [us] | BCET (Mejor tiempo) [us] | WCET (Peor tiempo) [us] |
| :--- | :---: | :---: | :---: | :---: |
| **task_dta_list[0] (Sensor)** | 15478 | 4 | 4 | 5 |
| **task_dta_list[1] (System)** | 15478 | 3 | 3 | 5 |
| **task_dta_list[2] (Actuator)** | 15478 | 2 | 2 | 4 |

**Aclaración de unidades de medida:**
* **NOE** (*Number of Executions*): Cantidad de ejecuciones (sin unidad / entero).
* **LET** (*Last Execution Time*): Microsegundos ($\mu s$).
* **BCET** (*Best-Case Execution Time*): Microsegundos ($\mu s$).
* **WCET** (*Worst-Case Execution Time*): Microsegundos ($\mu s$).
