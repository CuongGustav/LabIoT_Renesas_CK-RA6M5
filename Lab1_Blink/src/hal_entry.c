#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);

extern bsp_leds_t g_bsp_leds;

//Hàm chính của chương tình
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD
    R_BSP_NonSecureEnter();
#endif
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS; //Định nghĩa đơn vị trễ là 1 s
    const uint32_t freq_in_hz = 2; //Tần số nhấp nháy là 2Hz, bật/tắt 2 lần mỗi giây
    const uint32_t delay = bsp_delay_units / freq_in_hz; //delay 500ms cho mỗi chu kỳ bật tắt

    //Kiểm tra số lượng LED
    bsp_leds_t leds = g_bsp_leds;  //g_bsp_leds là một cấu trúc chứa thông tin về các đèn LED trên bo mạch.
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;
        }
    }

    //Vòng lặp chính
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW; //điều khiển trạng thái Led, BSP_IO_LEVEL_LOW là tắt, BSP_IO_LEVEL_HIGH là bật
    while (1)
    {
        R_BSP_PinAccessEnable();
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            uint32_t pin = leds.p_leds[i];
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, pin_level);
        }

        R_BSP_PinAccessDisable();
        if (BSP_IO_LEVEL_LOW == pin_level)
        {
            pin_level = BSP_IO_LEVEL_HIGH;
        }
        else
        {
            pin_level = BSP_IO_LEVEL_LOW;
        }
        R_BSP_SoftwareDelay(delay, bsp_delay_units); // gọi hàm tạo khoảng trễ giữa mỗi lần bật/tắt
    }
}

//Quá trình khởi động hệ thống
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0
        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;
#endif
    }
    if (BSP_WARM_START_POST_C == event)
    {
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}
