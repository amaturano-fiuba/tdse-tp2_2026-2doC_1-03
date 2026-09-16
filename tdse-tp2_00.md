# Trabajo Práctico: Máquina de Estados Finitos en C ⚙️

Este repositorio contiene la implementación en lenguaje C de una **Máquina de Estados Finitos (FSM - Finite State Machine)**. 

El código modela el comportamiento clásico de un **Molinete de Subte (Torniquete)** utilizando una estructura de control basada en `switch-case` anidados.

## 📖 Conceptos Clave

El sistema se compone de tres elementos fundamentales:
1. **Estados:** Las condiciones en las que puede estar el sistema.
   * `Locked` (Bloqueado)
   * `Unlocked` (Desbloqueado)
2. **Eventos (Entradas):** Los estímulos que el sistema recibe del mundo exterior.
   * `Coin` (Insertar moneda)
   * `Push` (Empujar el molinete)
3. **Transiciones:** Las reglas lógicas que dictan el cambio de un estado a otro en base al evento recibido.

## 💻 Implementación en C

El modelo utiliza `enum` para definir los estados y eventos de forma legible. La lógica central se resuelve evaluando primero el *estado actual* y luego el *evento recibido*.

```c
#include <stdio.h>

// 1. Definimos los Estados
typedef enum {
    ESTADO_BLOQUEADO,
    ESTADO_DESBLOQUEADO
} EstadoMolinete;

// 2. Definimos los Eventos
typedef enum {
    EVENTO_MONEDA, // "Coin"
    EVENTO_EMPUJE  // "Push"
} Evento;

// 3. Función principal de transición
EstadoMolinete procesar_evento(EstadoMolinete estado_actual, Evento evento) {
    switch (estado_actual) {
        
        case ESTADO_BLOQUEADO:
            if (evento == EVENTO_MONEDA) {
                printf("Moneda insertada. Desbloqueando...\n");
                return ESTADO_DESBLOQUEADO;
            } else if (evento == EVENTO_EMPUJE) {
                printf("Molinete bloqueado. No puedes pasar.\n");
                return ESTADO_BLOQUEADO;
            }
            break;

        case ESTADO_DESBLOQUEADO:
            if (evento == EVENTO_EMPUJE) {
                printf("Persona pasa. Bloqueando molinete...\n");
                return ESTADO_BLOQUEADO;
            } else if (evento == EVENTO_MONEDA) {
                printf("Molinete ya desbloqueado. Gracias por la moneda extra.\n");
                return ESTADO_DESBLOQUEADO;
            }
            break;
    }
    return estado_actual;
}

int main() {
    // Estado inicial por defecto
    EstadoMolinete mi_molinete = ESTADO_BLOQUEADO; 

    printf("--- INICIANDO SIMULACION DEL MOLINETE ---\n");

    // Secuencia de prueba
    mi_molinete = procesar_evento(mi_molinete, EVENTO_EMPUJE); // Falla al pasar
    mi_molinete = procesar_evento(mi_molinete, EVENTO_MONEDA); // Desbloquea
    mi_molinete = procesar_evento(mi_molinete, EVENTO_MONEDA); // Moneda extra
    mi_molinete = procesar_evento(mi_molinete, EVENTO_EMPUJE); // Pasa y bloquea

    return 0;
}
