#include "lvgl.h"
#include <math.h>

#define NUM_ITEMS 6
#define RADIUS 80
#define CENTER_X 120
#define CENTER_Y 120

lv_obj_t* items[NUM_ITEMS];
lv_obj_t* pointer;
int selected_index = 0;

void update_pointer_position() {
    float angle = (360.0 / NUM_ITEMS) * selected_index;
    float rad = angle * (M_PI / 180.0);

    int x2 = CENTER_X + RADIUS * cos(rad);
    int y2 = CENTER_Y + RADIUS * sin(rad);

    // Draw line from center to item
    static lv_point_t line_points[2];
    line_points[0].x = CENTER_X;
    line_points[0].y = CENTER_Y;
    line_points[1].x = x2;
    line_points[1].y = y2;

    lv_line_set_points(pointer, line_points, 2);
}

void lvgl_radial_selector() {
    // Create item circles
    for (int i = 0; i < NUM_ITEMS; i++) {
        float angle = (360.0 / NUM_ITEMS) * i;
        float rad = angle * (M_PI / 180.0);

        int x = CENTER_X + RADIUS * cos(rad);
        int y = CENTER_Y + RADIUS * sin(rad);

        items[i] = lv_btn_create(lv_scr_act());
        lv_obj_set_size(items[i], 60, 60);
        lv_obj_set_style_radius(items[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(items[i], lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(items[i], LV_ALIGN_TOP_LEFT, x - 30, y - 30);  // Center align

        lv_obj_t* label = lv_label_create(items[i]);
        lv_label_set_text_fmt(label, "Item %d", i + 1);
        lv_obj_center(label);
    }

    // Create pointer (rotating hand)
    pointer = lv_line_create(lv_scr_act());
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 4);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_line, true);
    lv_obj_add_style(pointer, &style_line, 0);

    update_pointer_position();
}
