/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 */

/********************** inclusions *******************************************/
#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "app.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define DEL_LED_MIN     0ul
#define DEL_LED_MED     250ul
#define DEL_LED_MAX     500ul

#define ACTUATOR_CFG_QTY    (sizeof(task_actuator_cfg_list)/sizeof(task_actuator_cfg_t))
#define ACTUATOR_DTA_QTY    ACTUATOR_CFG_QTY

/********************** internal data declaration ****************************/

/* Configuración de actuadores mapeados al hardware */
const task_actuator_cfg_t task_actuator_cfg_list[] = {
    {ID_LED_BARRIER_OPEN,  LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET, GPIO_PIN_RESET, DEL_LED_MAX},
    {ID_LED_BARRIER_CLOSE, LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET, GPIO_PIN_RESET, DEL_LED_MAX}
};

task_actuator_dta_t task_actuator_dta_list[ACTUATOR_DTA_QTY];

/********************** internal functions declaration ***********************/
void task_actuator_statechart(uint32_t index);

/********************** internal data definition *****************************/

/********************** external functions definition ************************/
void task_actuator_init(void *parameters)
{
    uint32_t index;
    const task_actuator_cfg_t *p_task_actuator_cfg;
    task_actuator_dta_t *p_task_actuator_dta;

    LOGGER_INFO(" ");
    LOGGER_INFO("  %s is running - Tick [mS] = %lu", GET_NAME(task_actuator_init), HAL_GetTick());

    for (index = 0; index < ACTUATOR_DTA_QTY; index++)
    {
        p_task_actuator_cfg = &task_actuator_cfg_list[index];
        p_task_actuator_dta = &task_actuator_dta_list[index];

        p_task_actuator_dta->state = ST_LED_OFF;
        p_task_actuator_dta->event = EV_LED_OFF;
        p_task_actuator_dta->flag  = false;
        p_task_actuator_dta->tick  = DEL_LED_MIN;

        HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
    }
}

void task_actuator_update(void *parameters)
{
    uint32_t index;

    for (index = 0; index < ACTUATOR_DTA_QTY; index++)
    {
        task_actuator_statechart(index);
    }
}

void task_actuator_statechart(uint32_t index)
{
    const task_actuator_cfg_t *p_task_actuator_cfg = &task_actuator_cfg_list[index];
    task_actuator_dta_t *p_task_actuator_dta = &task_actuator_dta_list[index];

    switch (p_task_actuator_dta->state)
    {
        case ST_LED_OFF:
            if ((true == p_task_actuator_dta->flag) && (EV_LED_ON == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
                p_task_actuator_dta->state = ST_LED_ON;
            }
            else if ((true == p_task_actuator_dta->flag) && (EV_LED_BLINK == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                p_task_actuator_dta->tick = p_task_actuator_cfg->tick_max;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
                p_task_actuator_dta->state = ST_LED_BLINK;
            }
            break;

        case ST_LED_ON:
            if ((true == p_task_actuator_dta->flag) && (EV_LED_OFF == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
                p_task_actuator_dta->state = ST_LED_OFF;
            }
            else if ((true == p_task_actuator_dta->flag) && (EV_LED_BLINK == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                p_task_actuator_dta->tick = p_task_actuator_cfg->tick_max;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
                p_task_actuator_dta->state = ST_LED_BLINK;
            }
            break;

        case ST_LED_BLINK:
            if ((true == p_task_actuator_dta->flag) && (EV_LED_OFF == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
                p_task_actuator_dta->state = ST_LED_OFF;
            }
            else if ((true == p_task_actuator_dta->flag) && (EV_LED_ON == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
                p_task_actuator_dta->state = ST_LED_ON;
            }
            else if ((true == p_task_actuator_dta->flag) && (EV_LED_BLINK == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                p_task_actuator_dta->tick = p_task_actuator_cfg->tick_max;
                HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
            }
            else
            {
                if (p_task_actuator_dta->tick > 0)
                {
                    p_task_actuator_dta->tick--;
                }
                else if (0 == p_task_actuator_dta->tick)
                {
                    p_task_actuator_dta->tick = p_task_actuator_cfg->tick_max;
                    HAL_GPIO_TogglePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin);
                }
            }
            break;

        default:
            p_task_actuator_dta->tick  = DEL_LED_MIN;
            p_task_actuator_dta->state = ST_LED_OFF;
            p_task_actuator_dta->event = EV_LED_OFF;
            p_task_actuator_dta->flag  = false;
            break;
    }
}
