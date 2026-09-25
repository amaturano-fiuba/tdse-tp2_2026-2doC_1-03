# Análisis de los Módulos: Tarea Actuador e Interfaz de Actuador

Este documento describe la arquitectura y el comportamiento de la **Tarea del Actuador**, la cual representa la etapa final de salida en esta arquitectura controlada por eventos. Su objetivo es recibir comandos emitidos por la tarea del Sistema y traducirlos en acciones físicas de hardware (como encender o apagar un LED) mediante el uso de una Máquina de Estados Finitos (FSM).

---

## ⚙️ Funcionamiento General

A diferencia de la Tarea del Sistema que utiliza una cola circular, la Tarea del Actuador se comunica a través de una interfaz de variables compartidas directamente. La tarea del Sistema invoca la interfaz del actuador modificando sus variables (evento y bandera), y la Tarea del Actuador procesa estos cambios de forma asíncrona en su bucle de actualización periódico.

---

## 📊 Evolución de Variables de la Tarea del Actuador

En el archivo `task_actuator.c`, la cantidad de actuadores a controlar está definida por `ACTUATOR_DTA_QTY`, cuyo valor es `1` (correspondiente al único elemento en `task_actuator_cfg_list`, que es `ID_LED_A`).

### `index`
*   **Inicio (`task_actuator_init`):** Se inicializa en `0` como iterador de un bucle `for` y se ejecuta una única vez, ya que la cantidad de actuadores es 1.
*   **Ejecución (`task_actuator_update`):** En cada ciclo de la función principal, toma el valor `0` para invocar la función de estado (`task_actuator_statechart`) sobre el único actuador disponible.

### `task_actuator_dta_list[index].tick`
*   **Unidad de Medida:** Milisegundos (`mS`) o Ticks de temporización del sistema (indicado por el comentario `period = 1mS` y las macros de retardo).
*   **Evolución:** No se inicializa explícitamente en `task_actuator_init()` (toma el valor `0` por defecto en memoria global). Durante el ciclo normal de actualización no sufre cambios; únicamente se fuerza a `DEL_LED_MIN` (`0`) si la FSM detecta un estado no válido y cae en la rama `default`.

### `task_actuator_dta_list[index].state`
*   **Inicio:** Se configura inicialmente en `ST_LED_IDLE`.
*   **Ejecución:** Cambia al estado `ST_LED_ACTIVE` cuando recibe y procesa el evento de activación (`EV_LED_ACTIVE`), y retorna a `ST_LED_IDLE` al procesar el evento de reposo (`EV_LED_IDLE`).

### `task_actuator_dta_list[index].event`
*   **Inicio:** Se inicializa en `EV_LED_IDLE`.
*   **Ejecución:** Su valor cambia de manera asíncrona cuando la tarea del sistema llama a la función de interfaz `put_event_task_actuator()`, sobrescribiendo la variable con el nuevo evento a procesar.

### `task_actuator_dta_list[index].flag`
*   **Inicio:** Se inicializa en `false`.
*   **Ejecución:** Cambia a `true` asíncronamente cuando un nuevo evento es cargado a través de la interfaz. Al ser procesado por la FSM en `task_actuator_statechart()`, la bandera es bajada inmediatamente a `false` para acusar recibo y evitar volver a ejecutar la misma acción.

---

## 🔄 Comportamiento de la FSM (`task_actuator_statechart`)

La función `void task_actuator_statechart(uint32_t index)` gestiona la lógica de conmutación del actuador apuntado por el `index`.

1.  **Lectura de Punteros:** Obtiene la configuración de hardware (`gpio_port`, `pin`, estados de encendido/apagado) y los datos dinámicos correspondientes al actuador actual.
2.  **Estado `ST_LED_IDLE`:** 
    *   Si la bandera (`flag`) es `true` y el evento es `EV_LED_ACTIVE`, el sistema reconoce una orden de encendido.
    *   Inmediatamente cambia el `flag` a `false`, escribe en el pin GPIO el estado `led_on` mediante `HAL_GPIO_WritePin`, y transiciona al estado `ST_LED_ACTIVE`.
3.  **Estado `ST_LED_ACTIVE`:**
    *   Si el `flag` es `true` y el evento es `EV_LED_IDLE`, reconoce una orden de apagado.
    *   Baja el `flag` a `false`, escribe en el pin GPIO el estado `led_off`, y regresa al estado `ST_LED_IDLE`.
4.  **Estado por defecto (`default`):** 
    *   Como mecanismo de protección contra corrupción de datos, si el estado no coincide con ninguno de los esperados, restablece los valores seguros: `tick` a `DEL_LED_MIN`, `state` a `ST_LED_IDLE`, `event` a `EV_LED_IDLE` y el `flag` a `false`.

---

## 📥 Interfaz del Actuador (`task_actuator_interface.c`)

El archivo de interfaz proporciona la función `put_event_task_actuator(task_actuator_ev_t event, task_actuator_id_t identifier)`, que permite la inyección de señales externas.

*   **`identifier`:** Es el índice del actuador a modificar. Al ejecutarse el programa principal, siempre recibe la constante estática `ID_LED_A` desde el sistema, por lo que no evoluciona a otros valores.
*   **`event`:** Evoluciona adquiriendo el valor suministrado en el argumento de la función (`EV_LED_ACTIVE` o `EV_LED_IDLE`), sobrescribiendo el campo `event` de la estructura del actuador correspondiente.
*   **`flag`:** Cada vez que esta función es invocada para un actuador específico, su campo `flag` es sobreescrito de forma incondicional con el valor `true`. Esto funciona como el mecanismo de notificación (disparo) para que la FSM del actuador evalúe y ejecute la transición correspondiente en su próxima actualización periódica.
