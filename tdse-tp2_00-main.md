# Análisis del Firmware STM32 🧠💻

Este documento detalla el análisis del funcionamiento del código fuente principal y el flujo de configuración del reloj en los archivos base de este proyecto STM32.

---

## 1. Análisis del Código Fuente

### `startup_stm32f103rbtx.s` (Archivo de Arranque)
Este archivo en lenguaje ensamblador es lo primero que ejecuta el microcontrolador al encender o tras un reinicio (reset).
* **Tabla de Vectores:** Define la tabla de vectores de interrupción (`g_pfnVectors`), estableciendo el puntero de pila inicial (`_estack`) y los punteros a los manejadores de excepciones, como `Reset_Handler` y `SysTick_Handler`.
* **`Reset_Handler`:** Es el punto de entrada real. Aquí se llama a `SystemInit` para inicializar el reloj básico. Luego, copia el segmento de datos `.data` de la Flash a la SRAM, inicializa el segmento `.bss` en cero, llama a los constructores estáticos (`__libc_init_array`) y, finalmente, hace un salto a la función `main()` de C.

### `main.c` (Programa Principal)
Coordina la inicialización del hardware y el flujo de la aplicación.
* **Inicialización HAL:** Comienza con `HAL_Init()`, que reinicia los periféricos e inicializa el temporizador SysTick a 1ms.
* **Configuración del Reloj (`SystemClock_Config`):** Configura el oscilador interno (HSI) y activa el multiplicador PLL. Toma el HSI dividido por 2 y lo multiplica por 16 para elevar la frecuencia de trabajo del microcontrolador.
* **Periféricos y Bucle Infinito:** Configura los puertos GPIO (un LED `LD2_Pin` y un botón `B1_Pin` con interrupción) y el puerto serie UART2. Finalmente, tras llamar a `app_init()`, entra en un bucle infinito `while (1)` ejecutando `app_update()` continuamente.

### `stm32f1xx_it.c` (Manejador de Interrupciones)
Contiene las rutinas de servicio de interrupción (ISR).
* **`SysTick_Handler()`:** Interrupción del temporizador del sistema. Llama a `HAL_IncTick()`, que incrementa el contador global de milisegundos de la HAL.
* **`EXTI15_10_IRQHandler()`:** Captura las interrupciones externas (pines 10 a 15) y delega el evento del botón a `HAL_GPIO_EXTI_IRQHandler(B1_Pin)`.

---

## 2. Evolución del Reloj y el Tiempo

A continuación se detalla cómo evolucionan las variables de control de reloj (`SystemCoreClock`) y tiempo (`SysTick`) desde el encendido hasta el bucle de la aplicación.

### Fase 1: `Reset_Handler`
* **SystemCoreClock:** El procesador ejecuta `SystemInit` (desde CMSIS). Esto configura el reloj base con el oscilador interno por defecto (típicamente HSI a 8 MHz). La variable `SystemCoreClock` se inicializa con este valor.
* **SysTick:** El temporizador está apagado.

### Fase 2: Entrada a `main()` y llamada a `HAL_Init()`
* **SystemCoreClock:** Mantiene el valor por defecto de 8 MHz.
* **SysTick:** La librería HAL inicializa el hardware del SysTick para que interrumpa cada 1 milisegundo basándose en la frecuencia actual (8 MHz).

### Fase 3: Ejecución de `SystemClock_Config()`
* **SystemCoreClock:** Se configuran los buses AHB/APB y se activa el PLL. Al llamar a `HAL_RCC_ClockConfig()`, la variable global `SystemCoreClock` se actualiza internamente a la nueva frecuencia (ej. 64 MHz, derivado de 8MHz/2 * 16).
* **SysTick:** `HAL_RCC_ClockConfig()` detecta el cambio de frecuencia y reconfigura los registros de recarga (reload registers) del hardware del SysTick de forma transparente. Esto asegura que la interrupción siga ocurriendo *exactamente* cada 1 milisegundo, sin importar la nueva velocidad del núcleo.

### Fase 4: Bucle principal `while (1)`
* **SystemCoreClock:** Se mantiene estacionario en su frecuencia máxima configurada (64 MHz).
* **SysTick:** Mientras el bucle ejecuta `app_update()`, el hardware interrumpe el flujo cada milisegundo, salta a `stm32f1xx_it.c` para ejecutar `SysTick_Handler()`, incrementa la variable de tiempo base (tick) y retorna al bucle principal de la aplicación.
