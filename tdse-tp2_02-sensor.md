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
