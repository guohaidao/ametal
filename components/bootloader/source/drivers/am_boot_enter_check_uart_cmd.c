/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ����Ӧ�ó����׼�ӿ�ʵ�֣�����ָ������������
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  yrh, first implementation.
 * \endinternal
 */
#include "am_boot_enter_check_uart_cmd.h"
#include "am_uart.h"
#include "am_softimer.h"
#include "am_vdebug.h"
#include <string.h>

#define CMD_ERROR                0
#define CMD_RIGHT                1

/** \brief �û������ */
#define USER_COMMAND_LEN         5

/** \brief �û����Զ��������, ������0x5a,0xa6��Ϊ֡ͷ��������0x0d��β�������Զ����м���������ַ�  */
static char user_command[USER_COMMAND_LEN] = {0x5a, 0xa6, 0x11, 0x66, 0x0d};

/** \brief ������Ž��յ����� */
static char command_rec[USER_COMMAND_LEN + 1] = {0};
volatile static uint8_t command_error = 0, index_t = 0, cmd_flag = CMD_ERROR;

/** \brief ���ݽ��ճ�ʱ������ʱ���ṹ��  */
static am_softimer_t    receive_callback_timer;

/**
 * \brief ���ڽ��ջص�����
 */
static void __uart_rec_callback(void *p_arg, char inchar)
{
    if(index_t < sizeof(command_rec) ) {
        command_rec[index_t++] = inchar;
        am_softimer_start(&receive_callback_timer, 500);
    }
}

/**
 * \brief ���ڽ�������������ʱ���ص�����
 */
static void __callback_timer_handle(void *p_arg)
{
    int i;
    /* \brief ���յ������ַ���������  */
    if(index_t != USER_COMMAND_LEN) {
        command_error = 1;
    } else {
        for(i = 0; i < USER_COMMAND_LEN; i++) {
            if(user_command[i] != command_rec[i]) {
                command_error = 1;
                break;
            }
        }
        /* ���յ��û���������ȷ �������̼�����״̬ */
        if(i == USER_COMMAND_LEN){
            cmd_flag = CMD_RIGHT;
        }
    }
    memset(command_rec, 0, sizeof(command_rec));
    am_softimer_stop(&receive_callback_timer);
    index_t = 0;
}

am_bool_t  __boot_enter_check_uart_cmd(void *p_drv)
{

    if(cmd_flag == CMD_RIGHT) {
        cmd_flag = CMD_ERROR;
         return AM_TRUE;
    }
    if(command_error == 1) {
        command_error = 0;
        am_kprintf("app : command error\r\n");
    }
    return AM_FALSE;
}

static const struct am_boot_enter_check_drv_funcs __g_enter_check_uart_cmd__drv_funcs = {
    __boot_enter_check_uart_cmd,
};

static am_boot_enter_check_uart_cmd_dev_t __g_enter_check_uart_cmd_dev;

am_boot_enter_check_handle_t am_boot_enter_check_uart_cmd_init(am_uart_handle_t uart_handle)
{
    __g_enter_check_uart_cmd_dev.enter_check_serv.p_drv   = &__g_enter_check_uart_cmd_dev;
    __g_enter_check_uart_cmd_dev.enter_check_serv.p_funcs = &__g_enter_check_uart_cmd__drv_funcs;
    __g_enter_check_uart_cmd_dev.uart_handle              = uart_handle;

    /* ʹ�ܴ����ж�ģʽ */
    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);
    /* ע�ᷢ�ͻص����� */
    am_uart_callback_set(uart_handle, AM_UART_CALLBACK_RXCHAR_PUT, __uart_rec_callback, NULL);

    am_softimer_init(&receive_callback_timer, __callback_timer_handle, NULL);

    return &__g_enter_check_uart_cmd_dev.enter_check_serv;
}

/* end of file */