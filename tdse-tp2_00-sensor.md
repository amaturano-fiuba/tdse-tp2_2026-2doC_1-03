# Análisis de los Módulos: Tarea Sensor e Interfaz de Sistema

Este documento describe la arquitectura, la máquina de estados finitos (FSM) y la evolución de las variables clave del subsistema de sensores y la cola de eventos del sistema.

---

## ⚙️ Funcionamiento General

El código implementa un controlador de sensores (pulsadores) basado en una FSM no bloqueante orientada a eventos . Su función principal es monitorear el estado físico de los pines GPIO y, al detectar un cambio de estado, encolar eventos en una estructura FIFO (`event_task_system_queue`) que sirve de interfaz hacia la tarea del sistema .

---

## 📊 Evolución de Variables de la Tarea Sensor

En el archivo `task_sensor.c`, la cantidad de sensores está determinada por `SENSOR_DTA_QTY`, cuyo valor es `1` (correspondiente a `ID_BTN_A`) .

### `index`
* **Inicio (`task_sensor_init`):** Se inicializa en `0` y ejecuta una única iteración en el bucle de inicialización .
* **Ejecución (`task_sensor_update`):** Toma el valor `0` en cada ciclo periódico de ejecución para procesar el único sensor configurado .

### `task_sensor_dta_list[index].tick`
* **Unidad de Medida:** Milisegundos (`mS`) / Ticks de temporización del sistema .
* **Inicio (`task_sensor_init`):** No se inicializa explícitamente (toma el valor por defecto `0` al ser variable global) .
* **Ejecución (`task_sensor_update`):** Mantiene su valor en `0` durante la ejecución normal . Solo se fuerza a `DEL_BTN_MIN` (`0`) si la FSM cae en la rama `default` debido a un estado no válido .

### `task_sensor_dta_list[index].state`
* **Inicio (`task_sensor_init`):** Se configura en `ST_BTN_IDLE` .
* **Ejecución (`task_sensor_update`):**
  * Permanece en `ST_BTN_IDLE` mientras el botón no se presione (`EV_BTN_UP`) .
  * Transiciona a `ST_BTN_ACTIVE` al detectar la presión del botón (`EV_BTN_DOWN`) .
  * Regresa a `ST_BTN_IDLE` cuando el botón es liberado (`EV_BTN_UP`) .

### `task_sensor_dta_list[index].event`
* **Inicio (`task_sensor_init`):** Se inicializa en `EV_BTN_UP` .
* **Ejecución (`task_sensor_update`):** Se evalúa dinámicamente al inicio de `task_sensor_statechart()` mediante la lectura del pin GPIO: asigna `EV_BTN_DOWN` si el botón está presionado o `EV_BTN_UP` si no lo está .

---

## 🔄 Comportamiento de `task_sensor_statechart(uint32_t index)`

Esta función ejecuta la lógica de la FSM para el sensor evaluado :

1. **Lectura de Hardware:** Mide el nivel del pin GPIO con `HAL_GPIO_ReadPin()` y determina si coincide con `pressed`. Actualiza `event` con `EV_BTN_DOWN` o `EV_BTN_UP` .
2. **Evaluación de Estados:**
   * **`ST_BTN_IDLE`:** Si el evento es `EV_BTN_DOWN`, llama a `put_event_task_system(p_task_sensor_cfg->signal_down)` (que envía `EV_SYS_ACTIVE`) y cambia a `ST_BTN_ACTIVE` .
   * **`ST_BTN_ACTIVE`:** Si el evento es `EV_BTN_UP`, llama a `put_event_task_system(p_task_sensor_cfg->signal_up)` (que envía `EV_SYS_IDLE`) y retorna a `ST_BTN_IDLE` .
   * **`default`:** Restablece la FSM a condiciones seguras: `tick = DEL_BTN_MIN` (0), `state = ST_BTN_IDLE` y `event = EV_BTN_UP` .

---

## 📥 Evolución de la Cola de Eventos (`event_task_system_queue`)

La cola de eventos maneja un buffer circular con capacidad fija de 16 elementos (`QUEUE_LENGTH = 16`) .

| Variable | Valor Inicial (`init_event_task_system`) | Comportamiento durante `task_sensor_update()` |
| :--- | :--- | :--- |
| **`head`** | `0`  | Incrementa en `1` cada vez que la FSM detecta un cambio de estado y llama a `put_event_task_system()` . Vuelve a `0` si alcanza el límite de `16` . |
| **`tail`** | `0`  | No cambia en este módulo . Incrementa en `1` (con *wrap-around* a `0`) únicamente cuando la tarea del sistema consume un evento mediante `get_event_task_system()` . |
| **`count`** | `0`  | Incrementa en `1` al publicar un evento (`put_event_task_system`) . Decrementa en `1` cuando la tarea del sistema remueve un evento (`get_event_task_system`) . |
| **`queue[i]`** | `EMPTY` (`255`) para las 16 posiciones . | La posición `queue[head]` recibe el evento generado (`EV_SYS_ACTIVE` o `EV_SYS_IDLE`) . Al ser consumido por la tarea destino, la posición `queue[tail]` se reescribe con `EMPTY` . |
