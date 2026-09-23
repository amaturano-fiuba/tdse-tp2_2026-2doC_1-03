# Informe de Entrega — TP2 Actividad 03: Integración de la Tarea de Sistema

## 1. Información General
* **Materia:** Taller de Desarrollo de Sistemas Embebidos (TDSE)
* **Trabajo Práctico:** TP2 — Actividad 03
* **Nombre del Proyecto:** `tdse-tp2_03-model_integration`
* **Archivo de Documentación:** `tdse-tp2_03-system.md`
* **Fecha:** 2026-09-23
* **Placa Objetivo:** STM32 NUCLEO-F103RB
* **Entorno de Desarrollo:** STM32CubeIDE

---

## 2. Objetivos de la Entrega (Paso 02)
1. Documentar la inicialización y migración del proyecto para la Actividad 03 (`tdse-tp2_03-model_integration`).
2. Configurar la infraestructura del proyecto para integrar la tarea de control centralizada (`task_system`) junto con la cola de eventos inter-tarea.
3. Reconfigurar los perfiles de depuración GDB (`.launch` y `.cfg`) asociados a la nueva actividad.
4. Verificar la correcta compilación y vinculación con la placa NUCLEO-F103RB vía ST-LINK.

---

## 3. Especificaciones del Hardware y Periféricos

### Microcontrolador y Placa
* **MCU:** STM32F103RBT6 (ARM Cortex-M3, 72 MHz)
* **Programador/Depurador:** ST-LINK/V2-1 integrado en placa NUCLEO.
* **Interfaz de Depuración:** SWD (Serial Wire Debug).

### Periféricos e Interfaz del Sistema
* **Buses / Pines de comunicación:**
  * Entradas GPIO configuradas para los 3 sensores de entrada (`BTN_A`, `BTN_B`, `BTN_C`).
  * Salidas GPIO para la señalización del estado del sistema a través de actuadores.
  * Comunicación serie UART/USART activa para el monitoreo de eventos mediante `logger`.

---

## 4. Estructura del Proyecto
El proyecto `tdse-tp2_03-model_integration` mantiene la siguiente organización de archivos:

```text
tdse-tp2_03-model_integration/
├── Core/
│   ├── Inc/               # Encabezados generados por CubeMX (main.h, stm32f1xx_it.h)
│   └── Src/               # Código fuente base (main.c, stm32f1xx_it.c, system_stm32f1xx.c)
├── Drivers/               # Drivers HAL de STM32F1xx y CMSIS
├── app/                   # Módulos de aplicación (task_sensor, task_system, task_actuator)
├── tdse-tp2_03-model_integration.ioc     # Configuración de periféricos STM32CubeMX
├── tdse-tp2_03-model_integration.cfg     # Script de configuración OpenOCD / ST-LINK
├── tdse-tp2_03-model_integration.launch  # Perfil de depuración GDB
└── tdse-tp2_03-system.md                 # Registro de la entrega (este documento)
