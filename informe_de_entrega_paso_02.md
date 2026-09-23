# Informe de Entrega — TP2 Actividad 01: Integración del Sensor

## 1. Información General
* **Materia:** Taller de Desarrollo de Sistemas Embebidos (TDSE)
* **Trabajo Práctico:** TP2 — Actividad 01
* **Nombre del Proyecto:** `tdse-tp2_01-model_integration`
* **Archivo de Documentación:** `tdse-tp2_01-sensor.md`
* **Fecha:** 2026-09-23
* **Placa Objetivo:** STM32 NUCLEO-F103RB
* **Entorno de Desarrollo:** STM32CubeIDE

---

## 2. Objetivos de la Entrega (Paso 02)
1. Documentar la integración del modelo base del proyecto y la interfaz del sensor.
2. Registrar la configuración del entorno de depuración GDB (`.launch` y `.cfg`).
3. Verificar la correcta compilación y vinculación del software con el hardware mediante ST-LINK.

---

## 3. Especificaciones del Hardware y Periféricos

### Microcontrolador y Placa
* **MCU:** STM32F103RBT6 (ARM Cortex-M3, 72 MHz)
* **Programador/Depurador:** ST-LINK/V2-1 integrado en placa NUCLEO.
* **Interfaz de Depuración:** SWD (Serial Wire Debug).

### Periféricos e Interfaz del Sensor
* **Buses / Pines de comunicación:**
  * Configuración de pines GPIO / ADC / I2C / SPI según mapeo del archivo `.ioc`.
  * Comunicación serie UART/USART activada para monitoreo/redirección de logs.

---

## 4. Estructura del Proyecto
El proyecto `tdse-tp2_01-model_integration` mantiene la siguiente organización de archivos:

```text
tdse-tp2_01-model_integration/
├── Core/
│   ├── Inc/               # Encabezados generados por CubeMX (main.h, stm32f1xx_it.h)
│   └── Src/               # Código fuente base (main.c, stm32f1xx_it.c, system_stm32f1xx.c)
├── Drivers/               # Drivers HAL de STM32F1xx y CMSIS
├── app/                   # Módulos de aplicación y controladores del sensor
├── tdse-tp2_01-model_integration.ioc     # Configuración de periféricos STM32CubeMX
├── tdse-tp2_01-model_integration.cfg     # Script de configuración OpenOCD / ST-LINK
├── tdse-tp2_01-model_integration.launch  # Perfil de depuración GDB
└── tdse-tp2_01-sensor.md                 # Registro de la entrega (este documento)
```

---

## 5. Verificación y Depuración

### Compilación
* **Resultado:** Compilación exitosa (0 errores, 0 advertencias).
* **Binario Generado:** `tdse-tp2_01-model_integration.elf`

### Configuración del Servidor GDB (ST-LINK)
* **Interface:** SWD
* **Reset Mode:** Software System Reset / Connect under reset
* **Estado de la Depuración:** Servidor ST-LINK GDB iniciado correctamente. Carga de memoria Flash verificada.

---

## 6. Observaciones y Notas
* Se resolvió el error de comunicación GDB actualizando el firmware del ST-LINK desde la opción `Help -> ST-LINK Upgrade` en STM32CubeIDE.
* El archivo `.launch` ha quedado asociado al nuevo nombre del proyecto para futuras ejecuciones de depuración interactiva.

---

## 7. Próximos Pasos
* Implementar las lecturas periódicas del sensor dentro del bucle principal de la aplicación (`app/`).
* Realizar pruebas de calibración y procesamiento de datos del sensor.