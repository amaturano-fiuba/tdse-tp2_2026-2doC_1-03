## Verificación e implementación del componente Sistema (Paso 02)

### 1. Verificación de la máquina de estados de la tarea sistema (`task_system`)

Se probó la lógica de control del sistema y la correcta emisión de solicitudes hacia los actuadores (`task_actuator`):

1. **Estado de Inicialización** (`ST_SYS_INIT`): Configuración inicial de las variables y arranque seguro del sistema.
2. **Estado de Reposo/Espera** (`ST_SYS_IDLE`): El sistema permanece inactivo a la espera de eventos de entrada (pulsadores, sensores, etc.).
3. **Estado Activo/Operativo** (`ST_SYS_ACTIVE`): Procesamiento de eventos y envío de los comandos correspondientes (encendido, apagado, destello) a los actuadores.
