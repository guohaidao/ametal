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
 * \brief LPC84X SPI (�жϷ�ʽ) �û������ļ�
 * \sa am_hwconf_lpc84x_spi1_int.c
 *
 * \internal
 * \par Modification history
 * - 1.02 15-11-25  sky, modified
 * - 1.01 15-10-10  aii, second implementation
 * - 1.00 15-07-10  aii, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc_spi_int.h"
#include "am_lpc82x_spi_dma.h"
#include "hw/amhw_lpc84x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_spi1_int
 * \copydoc am_hwconf_lpc84x_spi1_int.c
 * @{
 */

/**
 * \brief SPI1 ƽ̨��ʼ��
 */
am_local void __lpc84x_spi1_int_plfm_init (void)
{
    am_gpio_pin_cfg(__SPI1_SCK_PIN , PIO_FUNC_SPI1_SCK);
    am_gpio_pin_cfg(__SPI1_MOSI_PIN, PIO_FUNC_SPI1_MOSI);
    am_gpio_pin_cfg(__SPI1_MISO_PIN, PIO_FUNC_SPI1_MISO);

    /* CS_Pin ���û����� STD ����ʱ���д��룬�˴��������� */

    amhw_lpc84x_clk_spi1_clk_sel_set (AMHW_LPC84X_CLK_DEVICE_FRG0CLK);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SPI1);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SPI1);
}

/**
 * \brief SPI1 ƽ̨���ʼ��
 */
am_local void __lpc84x_spi1_int_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SPI1);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_SPI1);

    am_gpio_pin_cfg(__SPI1_SCK_PIN , AM_GPIO_PULLUP | AM_GPIO_INPUT);
    am_gpio_pin_cfg(__SPI1_MOSI_PIN, AM_GPIO_PULLUP | AM_GPIO_INPUT);
    am_gpio_pin_cfg(__SPI1_MISO_PIN, AM_GPIO_PULLUP | AM_GPIO_INPUT);
}

/** \brief SPI1 (�жϷ�ʽ) �豸��Ϣ */
am_local am_const struct am_lpc_spi_int_devinfo __g_lpc84x_spi1_int_devinfo = {
    LPC84X_SPI1_BASE,                /* SPI1 �Ĵ��������ַ */
    INUM_SPI1,                       /* SPI1 �жϺ� */
    CLK_SPI1,                        /* SPI1 ʱ�Ӻ� */
    __lpc84x_spi1_int_plfm_init,     /* ƽ̨��ʼ������ */
    __lpc84x_spi1_int_plfm_deinit    /* ƽ̨���ʼ������ */
};

/** \brief SPI1 (�жϷ�ʽ) �豸ʵ�� */
am_local am_lpc_spi_int_dev_t __g_lpc84x_spi1_int_dev;

/**
 * \brief SP0I (�жϷ�ʽ) ʵ����ʼ��
 */
am_spi_handle_t am_lpc84x_spi1_int_inst_init (void)
{
    return am_lpc_spi_int_init(&__g_lpc84x_spi1_int_dev,
                               &__g_lpc84x_spi1_int_devinfo);
}

/**
 * \brief SPI1 (�жϷ�ʽ) ʵ�����ʼ��
 */
void am_lpc84x_spi1_int_inst_deinit (am_spi_handle_t handle)
{
    am_lpc_spi_int_deinit(handle);
}

/**
 * @}
 */

/* end of file */