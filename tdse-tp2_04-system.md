## Verificación e implementación del componente Actuador (Paso 02)

### 1. Verificación de la máquina de estados de la tarea actuador (`task_actuator`)
Se probó la respuesta de los actuadores ante las solicitudes enviadas por el sistema (`task_system`):
1. **Estado Apagado (`ST_LED_OFF`):** El LED/barrera permanece desactivado (GPIO en nivel bajo).
2. **Estado Encendido (`ST_LED_ON`):** El LED/barrera se activa permanentemente (GPIO en nivel alto).
3. **Estado Destello (`ST_LED_BLINK`):** Conmutación periódica del pin GPIO administrada por temporización no bloqueante (`tick_max`).

---

### 2. Métricas de rendimiento de la tarea actuador (`task_actuator`)

| Métrica | Valor Obtenido | Unidad / Descripción |
| :--- | :---: | :--- |
| **NOE** (*Number Of Executions*) | XXXXX | Cantidad de ejecuciones |
| **LET** (*Last Execution Time*) | X | Microsegundos ($\mu s$) |
| **BCET** (*Best-Case Execution Time*) | X | Microsegundos ($\mu s$) |
| **WCET** (*Worst-Case Execution Time*) | X | Microsegundos ($\mu s$) |

> **Análisis:** El WCET de `task_actuator` representa el tiempo requerido para evaluar las condiciones de transición de estados, actualizar los contadores de *ticks* y realizar la conmutación de los pines GPIO.

---

### 3. Estado de la estructura de datos (`Live Expressions`)

| Type | Value |
| --- | --- |
| task_actuator_dta_t [2] | [2] |
| task_actuator_dta_t | {...} |
| uint32_t | 0 |
| task_actuator_st_t | ST_LED_OFF |
| task_actuator_ev_t | EV_LED_OFF |
| _Bool | false |
| task_actuator_cfg_t [2] | [2] |
