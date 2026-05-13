/**
 * @file lv_conf.h
 * LVGL 8.x configuration for ESP32-C3 / GC9A01 240x240 circular display
 */
#if 1  /* Set to "1" to enable */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   0
#define LV_COLOR_SCREEN_TRANSP 0
#define LV_COLOR_MIX_ROUND_OFS 0
#define LV_COLOR_CHROMA_KEY lv_color_hex(0x00ff00)

/*====================
    MEMORY SETTINGS
 *====================*/
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE   (32 * 1024U)
#define LV_MEM_ADR    0
#define LV_MEM_AUTO_DEFRAG 1
#define LV_MEM_BUF_MAX_NUM 16
#define LV_MALLOC  lv_mem_alloc
#define LV_REALLOC lv_mem_realloc
#define LV_FREE    lv_mem_free

/*====================
    HAL SETTINGS
 *====================*/
#define LV_DISP_DEF_REFR_PERIOD  16   /* ~60 FPS target */
#define LV_INDEV_DEF_READ_PERIOD 16
#define LV_TICK_CUSTOM 0
#define LV_DPI_DEF     130

/*=================
 * DRAW SETTINGS
 *================*/
#define LV_DRAW_COMPLEX    1
#define LV_SHADOW_CACHE_SIZE 0
#define LV_CIRCLE_CACHE_SIZE 4
#define LV_IMG_CACHE_DEF_SIZE 0
#define LV_GRADIENT_MAX_STOPS 2
#define LV_GRAD_CACHE_DEF_SIZE 0
#define LV_DITHER_GRADIENT 0

/*===================
 * GPU / HW ACCEL
 *==================*/
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_SWM341_DMA  0
#define LV_USE_GPU_NXP_PXP     0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_GPU_SDL         0

/*===================
 * LOGGING
 *==================*/
#define LV_USE_LOG 0
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/*=====================
 *  COMPILER SETTINGS
 *====================*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 4
#define LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY
#define LV_ATTRIBUTE_FAST_MEM
#define LV_ATTRIBUTE_DMA
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning
#define LV_USE_LARGE_COORD 0

/*=================
 * FONT SETTINGS
 *================*/
#define LV_FONT_MONTSERRAT_8   0
#define LV_FONT_MONTSERRAT_10  0
#define LV_FONT_MONTSERRAT_12  1
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_16  1
#define LV_FONT_MONTSERRAT_20  1
#define LV_FONT_MONTSERRAT_24  1
#define LV_FONT_MONTSERRAT_28  0
#define LV_FONT_MONTSERRAT_36  0
#define LV_FONT_MONTSERRAT_48  0
#define LV_FONT_UNSCII_8       0
#define LV_FONT_UNSCII_16      0
#define LV_FONT_CUSTOM_DECLARE
#define LV_FONT_DEFAULT        &lv_font_montserrat_14
#define LV_FONT_FMT_TXT_LARGE  0
#define LV_USE_FONT_SUBPX      0
#define LV_USE_FONT_COMPRESSED 0

/*=================
 * TEXT SETTINGS
 *================*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3
#define LV_TXT_COLOR_CMD "#"
#define LV_USE_BIDI        0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 *  WIDGET USAGE
 *================*/
#define LV_USE_ARC       1
#define LV_USE_BAR       1
#define LV_USE_BTN       1
#define LV_USE_BTNMATRIX 0
#define LV_USE_CANVAS    0
#define LV_USE_CHECKBOX  0
#define LV_USE_DROPDOWN  0
#define LV_USE_IMG       0
#define LV_USE_LABEL     1
  #define LV_LABEL_TEXT_SELECTION 1
  #define LV_LABEL_LONG_TXT_HINT  0
#define LV_USE_LINE     0
#define LV_USE_ROLLER   0
#define LV_USE_SLIDER   0
#define LV_USE_SWITCH   0
#define LV_USE_TEXTAREA 0
#define LV_USE_TABLE    1
  #define LV_TABLE_COLUMN_MAX 10
#define LV_USE_CHART    1
  #define LV_CHART_AXIS_TICK_LABEL_MAX_LEN 256
#define LV_USE_ANIMIMG    0   /* requires LV_USE_IMG — keep disabled */
#define LV_USE_CALENDAR   0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN     0
#define LV_USE_KEYBOARD   0
#define LV_USE_LED        1
#define LV_USE_LIST       1
#define LV_USE_MENU       0
#define LV_USE_METER      0
#define LV_USE_MSGBOX     0
#define LV_USE_SPAN       0
#define LV_USE_SPINBOX    0
#define LV_USE_SPINNER    1
#define LV_USE_TABVIEW    0
#define LV_USE_TILEVIEW   0
#define LV_USE_WIN        0

/*==================
 * THEMES
 *================*/
#define LV_USE_THEME_DEFAULT 1
  #define LV_THEME_DEFAULT_DARK 1
  #define LV_THEME_DEFAULT_GROW 0
  #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#define LV_USE_THEME_BASIC 0
#define LV_USE_THEME_MONO  0

/*==================
 * LAYOUTS
 *================*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 0

/*=====================
 * 3RD PARTY LIBS
 *====================*/
#define LV_USE_FS_STDIO  0
#define LV_USE_FS_POSIX  0
#define LV_USE_FS_WIN32  0
#define LV_USE_FS_FATFS  0
#define LV_USE_PNG       0
#define LV_USE_BMP       0
#define LV_USE_SJPG      0
#define LV_USE_GIF       0
#define LV_USE_QRCODE    0
#define LV_USE_FREETYPE  0
#define LV_USE_RLOTTIE   0
#define LV_USE_FFMPEG    0

/*==================
 * OTHERS
 *================*/
#define LV_USE_SNAPSHOT   0
#define LV_USE_MONKEY     0
#define LV_USE_GRIDNAV    0
#define LV_USE_FRAGMENT   0
#define LV_USE_IMGFONT    0
#define LV_USE_MSG        0
#define LV_USE_IME_PINYIN 0

/*==================
 * EXAMPLES / DEMOS
 *================*/
#define LV_BUILD_EXAMPLES        0
#define LV_USE_DEMO_WIDGETS      0
#define LV_USE_DEMO_BENCHMARK    0
#define LV_USE_DEMO_STRESS       0
#define LV_USE_DEMO_MUSIC        0

#endif /* LV_CONF_H */
#endif /* End of "Set to '1' to enable" */
