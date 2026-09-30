# Depuración y Medición de Tiempos de Ejecución - TP2 Actividad 05

## Paso 04: Registro de métricas de `task_dta_list[0]` (`task_actuator`)

Se confirmó mediante la depuración en STM32CubeIDE el correcto funcionamiento del proyecto `tdse-tp2_05-model_integration` con la placa NUCLEO-F103RB y los 2 LEDs externos conectados[cite: 19]. 

Luego de varias ejecuciones de `app_update()`, se registraron los siguientes valores de tiempo y ejecuciones para `task_dta_list[0]`[cite: 19, 22]:

| Variable | Valor | Descripción | Unidad de Medida |
| :--- | :---: | :--- | :---: |
| **NOE** (*Number of Executions*) | `17190` | Número de ejecuciones de la tarea[cite: 22] | Ejecuciones |
| **LET** (*Last Execution Time*) | `9` | Tiempo de la última ejecución[cite: 22] | $\mu s$ |
| **BCET** (*Best Case Execution Time*) | `9` | Mejor tiempo de ejecución registrado (mínimo)[cite: 22] | $\mu s$ |
| **WCET** (*Worst Case Execution Time*) | `11` | Peor tiempo de ejecución registrado (máximo)[cite: 22] | $\mu s$ |

---

### Confirmación de Funcionamiento
* Se verificó la ejecución no bloqueante de la tarea de actuador a través de la máquina de estados[cite: 2, 19].
* El tiempo de ejecución se mantiene estable dentro del rango esperado ($9\ \mu s$ a $11\ \mu s$), garantizando el correcto control determinístico de los 2 LEDs (`ID_LED_BARRIER_OPEN` e `ID_LED_BARRIER_CLOSE`)[cite: 19, 22].
