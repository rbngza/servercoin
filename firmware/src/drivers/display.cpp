#include "display.h"
#include "../config.h"
#include "../ui/ui.h"   // ui_next_screen / ui_prev_screen

#include <LovyanGFX.hpp>
#include <lvgl.h>

// ─── LovyanGFX hardware description ─────────────────────────────────────────
// Pins are read from config.h — change them there if your board differs.

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01   _panel;
    lgfx::Bus_SPI        _bus;
    lgfx::Light_PWM      _light;
    lgfx::Touch_CST816S  _touch;

public:
    LGFX() {
        // SPI bus
        {
            auto cfg        = _bus.config();
            cfg.spi_host    = SPI2_HOST;
            cfg.spi_mode    = 0;
            cfg.freq_write  = 80000000;
            cfg.freq_read   = 16000000;
            cfg.spi_3wire   = true;
            cfg.use_lock    = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk    = DISP_SCK;
            cfg.pin_mosi    = DISP_MOSI;
            cfg.pin_miso    = -1;
            cfg.pin_dc      = DISP_DC;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }
        // Panel
        {
            auto cfg          = _panel.config();
            cfg.pin_cs        = DISP_CS;
            cfg.pin_rst       = DISP_RST;
            cfg.pin_busy      = -1;
            cfg.panel_width   = 240;
            cfg.panel_height  = 240;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;
            cfg.readable      = false;
            cfg.invert        = true;   // GC9A01 needs colour inversion
            cfg.rgb_order     = false;
            cfg.dlen_16bit    = false;
            cfg.bus_shared    = false;
            _panel.config(cfg);
        }
        // Backlight
        {
            auto cfg        = _light.config();
            cfg.pin_bl      = DISP_BL;
            cfg.invert      = false;
            cfg.freq        = 44100;
            cfg.pwm_channel = 7;
            _light.config(cfg);
            _panel.setLight(&_light);
        }
        // Capacitive touch (CST816S, I2C 0x15)
        {
            auto cfg            = _touch.config();
            cfg.i2c_port        = 0;  // I2C port 0
            cfg.i2c_addr        = 0x15;
            cfg.pin_sda         = TOUCH_SDA;
            cfg.pin_scl         = TOUCH_SCL;
            cfg.pin_int         = TOUCH_INT;
            cfg.pin_rst         = TOUCH_RST;
            cfg.freq            = 400000;
            cfg.x_min           = 0;
            cfg.x_max           = 239;
            cfg.y_min           = 0;
            cfg.y_max           = 239;
            cfg.bus_shared      = false;
            cfg.offset_rotation = 0;
            _touch.config(cfg);
            _panel.setTouch(&_touch);
        }
        setPanel(&_panel);
    }
};

static LGFX gfx;

// ─── LVGL draw buffers (two × 240×40 pixels = ~38 KB total) ─────────────────
// Larger buffers = fewer SPI transactions per frame = better FPS.
static lv_disp_draw_buf_t draw_buf;
static lv_color_t         buf1[240 * 40];
static lv_color_t         buf2[240 * 40];

// ─── Swipe gesture detection ─────────────────────────────────────────────────
static int16_t _swipe_start_x = -1;
static int16_t _swipe_last_x  =  0;
static int16_t _swipe_last_y  =  0;

// ─── LVGL callbacks ──────────────────────────────────────────────────────────

static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;
    gfx.startWrite();
    gfx.setAddrWindow(area->x1, area->y1, w, h);
    gfx.writePixels(reinterpret_cast<lgfx::rgb565_t *>(color_p), w * h);
    gfx.endWrite();
    lv_disp_flush_ready(drv);
}

static void lvgl_touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    uint16_t tx, ty;
    if (gfx.getTouch(&tx, &ty)) {
        data->point.x = (lv_coord_t)tx;
        data->point.y = (lv_coord_t)ty;
        data->state   = LV_INDEV_STATE_PR;

        if (_swipe_start_x < 0) _swipe_start_x = (int16_t)tx;
        _swipe_last_x = (int16_t)tx;
        _swipe_last_y = (int16_t)ty;
    } else {
        data->state = LV_INDEV_STATE_REL;

        if (_swipe_start_x >= 0) {
            int16_t dx = _swipe_last_x - _swipe_start_x;
            // Only count as swipe if horizontal movement dominates
            if (abs(dx) > 40) {
                if (dx < 0) ui_next_screen();   // swipe left  → next
                else        ui_prev_screen();   // swipe right → prev
            }
            _swipe_start_x = -1;
        }
    }
}

// ─── Public init ─────────────────────────────────────────────────────────────

void display_init() {
    gfx.init();
    gfx.setRotation(2);  // 180° — USB port faces up
    gfx.setBrightness(220);
    gfx.fillScreen(TFT_BLACK);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 240 * 40);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = 240;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    // Dark theme with cyan primary
    lv_theme_t *theme = lv_theme_default_init(
        disp,
        lv_color_hex(0x00d4ff),  // primary:   cyan
        lv_color_hex(0xff6b35),  // secondary: orange
        true,                    // dark mode
        &lv_font_montserrat_14);
    lv_disp_set_theme(disp, theme);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_touch_cb;
    lv_indev_drv_register(&indev_drv);
}
