# Informe de Entrega — TP2 Actividad 02: Integración de Múltiples Sensores

## 1. Información General
* **Materia:** Taller de Desarrollo de Sistemas Embebidos (TDSE)
* **Trabajo Práctico:** TP2 — Actividad 02
* **Nombre del Proyecto:** `tdse-tp2_02-model_integration`
* **Archivo de Documentación:** `tdse-tp2_02-sensor.md`
* **Fecha:** 2026-09-23
* **Placa Objetivo:** STM32 NUCLEO-F103RB
* **Entorno de Desarrollo:** STM32CubeIDE

---

## 2. Objetivos de la Entrega (Paso 02)
1. Documentar la clonación y reconfiguración del proyecto base para la Actividad 02 (`tdse-tp2_02-model_integration`).
2. Configurar la infraestructura del proyecto para soportar múltiples instancias del modelo de sensor (3 Sensor Statecharts).
3. Reconfigurar y generar el perfil de depuración GDB (`.launch` y `.cfg`) asociado al ejecutable de la Actividad 02.
4. Verificar la limpia compilación y vinculación con la placa NUCLEO-F103RB vía ST-LINK.

---

## 3. Especificaciones del Hardware y Periféricos

### Microcontrolador y Placa
* **MCU:** STM32F103RBT6 (ARM Cortex-M3, 72 MHz)
* **Programador/Depurador:** ST-LINK/V2-1 integrado en placa NUCLEO.
* **Interfaz de Depuración:** SWD (Serial Wire Debug).

### Periféricos e Interfaz del Sensor
* **Buses / Pines de comunicación:**
  * Configuración de entradas GPIO para la matriz/grupo de 3 sensores (botones de entrada), ajustados desde el archivo `.ioc`.
  * Configuración de resistencias internas (Pull-up / Pull-down) según esquemático de conexionado.
  * Comunicación serie UART/USART activada para monitoreo y redirección de logs (`logger`).

---

## 4. Estructura del Proyecto
El proyecto `tdse-tp2_02-model_integration` mantiene la siguiente organización de archivos:

```text
tdse-tp2_02-model_integration/
├── Core/
│   ├── Inc/               # Encabezados generados por CubeMX (main.h, stm32f1xx_it.h)
│   └── Src/               # Código fuente base (main.c, stm32f1xx_it.c, system_stm32f1xx.c)
├── Drivers/               # Drivers HAL de STM32F1xx y CMSIS
├── app/                   # Módulos de aplicación y controladores para 3 máquinas de estado
├── tdse-tp2_02-model_integration.ioc     # Configuración de periféricos STM32CubeMX
├── tdse-tp2_02-model_integration.cfg     # Script de configuración OpenOCD / ST-LINK
├── tdse-tp2_02-model_integration.launch  # Perfil de depuración GDB
└── tdse-tp2_02-sensor.md                 # Registro de la entrega (este documento)

## Registro de métricas de rendimiento y verificación de 3 sensores (Paso 04)

### 1. Verificación de ejecución simultánea
Se verificó mediante *Live Expressions* el funcionamiento en paralelo de las 3 instancias del modelo de sensor (`task_sensor_dta_list[0]`, `task_sensor_dta_list[1]` y `task_sensor_dta_list[2]`). Cada FSM responde de manera independiente a las entradas de hardware configuradas (`BTN_A`, `BTN_B` y `BTN_C`) manteniendo sus estados (`ST_BTN_UP`, `ST_BTN_FALLING`, `ST_BTN_DOWN`, `ST_BTN_RISING`) y temporizadores de anti-rebote.

### 2. Métricas de rendimiento de la tarea de sensor (`task_dta_list[0]`)

| Métrica | Valor Obtenido | Unidad / Descripción |
| :--- | :---: | :--- |
| **NOE** (*Number Of Executions*) | 66212 | Cantidad de ejecuciones (entero) |
| **LET** (*Last Execution Time*) | 9 | Microsegundos ($\mu s$) |
| **BCET** (*Best-Case Execution Time*) | 9 | Microsegundos ($\mu s$) |
| **WCET** (*Worst-Case Execution Time*) | 11 | Microsegundos ($\mu s$) |

> **Análisis de rendimiento:** El tiempo de peor caso de ejecución (WCET) pasó de $6\,\mu s$ (registrado en la Actividad 01 con 1 solo sensor) a $11\,\mu s$. Este incremento es proporcional al procesamiento secuencial de las 3 máquinas de estado durante cada llamada a `task_sensor_update()`.
