#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);
extern bsp_leds_t g_bsp_leds;

void hal_entry(void)
{
#if BSP_TZ_SECURE_BUILD
    R_BSP_NonSecureEnter();
#endif

    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;
    const uint32_t delay = 1500;  // 500 ms delay

    bsp_leds_t leds = g_bsp_leds;

    // Nếu không có đèn LED trên mạch thì dừng chương trình.
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;
        }
    }

    // Vòng lặp chính điều khiển lần lượt các đèn
    while (1)
    {
        R_BSP_PinAccessEnable();

        // Điều khiển 3 đèn RGB đơn: lần lượt bật R, G, B
        for (uint32_t i = 0; i < 3; i++)  // 3 đèn RGB đơn
        {
            // Bật đèn LED hiện tại
            R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[i], BSP_IO_LEVEL_HIGH);

            // Đợi trong 500ms
            R_BSP_SoftwareDelay(delay, bsp_delay_units);

            // Tắt đèn LED hiện tại
            R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[i], BSP_IO_LEVEL_LOW);
        }

        // Điều khiển đèn LED 7 màu
        for (int color = 0; color < 7; color++)
        {
            // Tắt hết đèn RGB đơn trước khi chuyển sang đèn 7 màu
            for (uint32_t i = 0; i < 3; i++)
            {
                R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[i], BSP_IO_LEVEL_LOW);
            }

            // Điều khiển đèn LED 7 màu (giả sử là chân số 3, 4, 5 cho RGB)
            switch (color)
            {
                case 0:  // Màu đỏ
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[3], BSP_IO_LEVEL_HIGH); // R
                    break;
                case 1:  // Màu xanh lá
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[4], BSP_IO_LEVEL_HIGH); // G
                    break;
                case 2:  // Màu xanh dương
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[5], BSP_IO_LEVEL_HIGH); // B
                    break;
                case 3:  // Màu vàng (đỏ + xanh lá)
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[3], BSP_IO_LEVEL_HIGH); // R
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[4], BSP_IO_LEVEL_HIGH); // G
                    break;
                case 4:  // Màu tím (đỏ + xanh dương)
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[3], BSP_IO_LEVEL_HIGH); // R
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[5], BSP_IO_LEVEL_HIGH); // B
                    break;
                case 5:  // Màu lục lam (xanh lá + xanh dương)
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[4], BSP_IO_LEVEL_HIGH); // G
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[5], BSP_IO_LEVEL_HIGH); // B
                    break;
                case 6:  // Màu trắng (đỏ + xanh lá + xanh dương)
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[3], BSP_IO_LEVEL_HIGH); // R
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[4], BSP_IO_LEVEL_HIGH); // G
                    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[5], BSP_IO_LEVEL_HIGH); // B
                    break;
            }

            // Đợi trong 500ms
            R_BSP_SoftwareDelay(delay, bsp_delay_units);

            // Tắt tất cả các kênh màu sau mỗi chu kỳ để tránh giữ sáng một màu
            for (uint32_t i = 3; i < 6; i++)
            {
                R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[i], BSP_IO_LEVEL_LOW);
            }
        }

        R_BSP_PinAccessDisable();
    }
}
