# Análisis de los Módulos: Tarea de Sistema e Interfaz de Actuador

Este documento detalla la arquitectura lógica de la **Tarea del Sistema**, que actúa como el núcleo de la arquitectura controlada por eventos (ETS) al procesar las entradas (sensores) y emitir comandos hacia las salidas (actuadores).

---

## ⚙️ Funcionamiento General

La Tarea del Sistema funciona monitoreando constantemente una cola circular de eventos. Cuando se detecta un evento entrante, este es procesado por una Máquina de Estados Finitos (FSM) que, dependiendo de la transición, envía comandos (eventos) específicos al actuador mediante su interfaz.

---

## 📊 Evolución de Variables de la Tarea del Sistema

En el archivo `task_system.c`, la cantidad de modos de ejecución está definida por `SYSTEM_DTA_QTY = 1` (el modo `NORMAL`). Las variables de `task_system_dta_list` evolucionan de la siguiente manera:

*   **`index`:** 
    *   *Inicio (`task_system_init`):* Se utiliza como iterador, tomando el valor `0` para inicializar el único modo disponible.
    *   *Ejecución:* No se itera dinámicamente; la FSM accede directamente a la posición usando la variable global `g_task_system_mode` (cuyo valor es `NORMAL`, es decir, `0`).
*   **`tick` (Unidad: milisegundos/ticks):** 
    *   *Evolución:* Asume el valor por defecto `0` al inicio y se mantiene constante. Solo es forzado a `DEL_SYS_MIN` (0) como medida de seguridad si la FSM cae en un estado no contemplado (`default`).
*   **`state`:** 
    *   *Inicio:* Se inicializa en `ST_SYS_IDLE`.
    *   *Ejecución:* Transiciona a `ST_SYS_ACTIVE` al procesar el evento `EV_SYS_ACTIVE`, y regresa a `ST_SYS_IDLE` al procesar `EV_SYS_IDLE`.
*   **`event`:** 
    *   *Inicio:* Se inicializa en `EV_SYS_IDLE`.
    *   *Ejecución:* Adquiere el valor del último evento extraído de la cola mediante `get_event_task_system()`.
*   **`flag`:** 
    *   *Inicio:* Se inicializa en `false`.
    *   *Ejecución:* Cambia a `true` al confirmar la lectura de un nuevo evento en la cola, y vuelve a `false` inmediatamente después de procesarlo dentro de la FSM para evitar repeticiones.

---

## 🔄 Comportamiento de la FSM (`task_system_normal_statechart`)

Esta función (que no requiere el argumento `index` en esta implementación) controla la lógica principal del sistema:

1.  **Extracción de Eventos:** Consulta la cola mediante `any_event_task_system()`. Si hay eventos pendientes, levanta su `flag` y extrae el evento.
2.  **Evaluación (`ST_SYS_IDLE`):** Si recibe `EV_SYS_ACTIVE` (y `flag == true`), baja el `flag`, envía el comando `EV_LED_ACTIVE` al actuador mediante `put_event_task_actuator()`, y avanza al estado `ST_SYS_ACTIVE`.
3.  **Evaluación (`ST_SYS_ACTIVE`):** Si recibe `EV_SYS_IDLE` (y `flag == true`), baja el `flag`, emite `EV_LED_IDLE` al actuador, y regresa a `ST_SYS_IDLE`.
4.  **Estado por defecto (`default`):** Restaura todas las variables a su condición inicial de reposo (`ST_SYS_IDLE`) en caso de anomalías.

---

## 📥 Evolución de la Cola de Eventos (`event_task_system_queue`)

La cola de eventos tiene una longitud máxima de 16 posiciones (`QUEUE_LENGTH`).

*   **`i`:** Iterador efímero que cuenta del 0 al 15 únicamente durante la ejecución de `init_event_task_system()` para limpiar la cola.
*   **`head`:** Inicia en `0`. Es incrementado asíncronamente por otras tareas (como el sensor) cada vez que depositan eventos en la cola.
*   **`tail`:** Inicia en `0`. Durante `task_system_update()`, se incrementa en `1` cada vez que se consume un evento mediante `get_event_task_system()`, reiniciándose a `0` si alcanza el límite de 16 (comportamiento *wrap-around*).
*   **`count`:** Disminuye en `1` cada vez que el sistema consume un evento en su bucle principal mediante `get_event_task_system()`.
*   **`queue[i]`:** Todas las posiciones inician en `EMPTY` (`255`). Al consumir un evento, el valor guardado en la posición referenciada por `tail` se transfiere a la FSM y dicha celda se sobrescribe inmediatamente con `EMPTY`.

---

## 📤 Evolución de Variables de la Tarea del Actuador

A través del archivo `task_actuator_interface.c`, el Sistema despacha comandos modificando las variables del actuador en `task_actuator_dta_list`.

*   **`identifier`:** El Sistema pasa el argumento constante `ID_LED_A` al invocar la interfaz; su valor es estático y no evoluciona.
*   **`event`:** Cambia dinámicamente según la FSM del Sistema. Recibe el valor `EV_LED_ACTIVE` cuando el sistema sale del reposo, y `EV_LED_IDLE` al retornar a este.
*   **`flag`:** Cada vez que el Sistema despacha un comando a través de `put_event_task_actuator()`, esta bandera se fuerza incondicionalmente a `true`, notificando al actuador que debe procesar un nuevo evento.
