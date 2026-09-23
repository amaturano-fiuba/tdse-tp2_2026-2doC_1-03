# Informe de Entrega — TP2 Actividad 03: Implementación del Paso 03 (Integración Sistema/Actuador)

## 1. Descripción de la Implementación (Paso 03)
En este paso se implementó la integración de la tarea de control del sistema (`task_system`) y la tarea de actuadores (`task_actuator`) para procesar en paralelo los eventos generados por la matriz de 3 sensores (`BTN_A`, `BTN_B`, `BTN_C`) y reflejar las respuestas en los actuadores correspondientes.

---

## 2. Modificaciones Realizadas en el Código Fuente

### A. Módulo de Sistema (`task_system`)
* **Cola de eventos:** Se adaptó y verificó el mecanismo de paso de mensajes mediante `put_event_task_system()` y `get_event_task_system()`.
* **Procesamiento de eventos:** La máquina de estados de `task_system` procesa de forma no bloqueante las señales enviadas por los tres sensores (`EV_SYS_ACTIVE`, `EV_SYS_IDLE`, o los identificadores específicos de cada botón).

### B. Módulo de Actuadores (`task_actuator`)
* **Mapeo de salidas:** Se configuró el arreglo `task_actuator_cfg_list[]` en `task_actuator.c` y los descriptores en `board.h` para asociar las respuestas a los pines de salida correspondientes (p. ej., LED de la placa NUCLEO y/o actuadores externos).
* **Actualización periódica:** La función `task_actuator_update()` ejecuta el statechart del actuador en cada tick de $1\,\text{ms}$, cambiando de estado según las órdenes emitidas por `task_system`.

---

## 3. Arquitectura de Comunicación Inter-Tareas
1. **`task_sensor`**: Lee el estado físico de los 3 botones con anti-rebote ($25\,\text{ms}$) y deposita un evento en la cola del sistema mediante `put_event_task_system()`.
2. **`task_system`**: Consume el evento de la cola mediante `get_event_task_system()`, actualiza su estado interno y decide la acción a tomar.
3. **`task_actuator`**: Recibe la orden de conmutación desde la tarea de sistema y conmuta el estado lógico del periférico de salida (`HAL_GPIO_WritePin`).
