#include "RadialSelector.h"
#include "Gyro_QMI8658.h"
#include <math.h>
#include "lvgl.h"

#define NUM_ITEMS 5
#define RADIUS 140
#define SELECTOR_RADIUS 80

static const char *items[NUM_ITEMS] = {
    "One", "Two", "Three", "Four", "Five"
};

static lv_obj_t *item_labels[NUM_ITEMS];
static lv_obj_t *highlight_circle;

static lv_coord_t center_x = 120;
static lv_coord_t center_y = 120;

void update_selector_from_yaw(float yaw_deg) {
    yaw_deg = fmod(yaw_deg, 360.0f);
    if (yaw_deg < 0) yaw_deg += 360.0f;

    int index = (int)((yaw_deg / 360.0f) * NUM_ITEMS) % NUM_ITEMS;
    float angle_rad = (360.0f / NUM_ITEMS) * index * (M_PI / 180.0f);

    int x = center_x + cos(angle_rad) * RADIUS;
    int y = center_y + sin(angle_rad) * RADIUS;

    lv_obj_set_pos(highlight_circle, x - SELECTOR_RADIUS, y - SELECTOR_RADIUS);
}

void lvgl_radial_selector() {
    center_x = lv_obj_get_width(lv_scr_act()) / 2;
    center_y = lv_obj_get_height(lv_scr_act()) / 2;

    // Draw each label positioned around the circle
    for (int i = 0; i < NUM_ITEMS; i++) {
        float angle_deg = (360.0f / NUM_ITEMS) * i;
        float angle_rad = angle_deg * (M_PI / 180.0f);

        int x = center_x + cos(angle_rad) * RADIUS;
        int y = center_y + sin(angle_rad) * RADIUS;

        item_labels[i] = lv_label_create(lv_scr_act());
        lv_label_set_text(item_labels[i], items[i]);
        lv_obj_set_style_text_align(item_labels[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(item_labels[i], &lv_font_montserrat_16, 0);
        lv_obj_align(item_labels[i], LV_ALIGN_TOP_LEFT, x - 20, y - 10);
    }

    highlight_circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(highlight_circle, SELECTOR_RADIUS * 2, SELECTOR_RADIUS * 2);
    lv_obj_set_style_radius(highlight_circle, LV_RADIUS_CIRCLE, 0);
    
    lv_obj_set_style_bg_opa(highlight_circle, LV_OPA_TRANSP, 0); // transparent middle
    lv_obj_set_style_border_width(highlight_circle, 3, 0);        // outline thickness
    lv_obj_set_style_border_color(highlight_circle, lv_palette_main(LV_PALETTE_RED), 0);
    
    lv_obj_clear_flag(highlight_circle, LV_OBJ_FLAG_SCROLLABLE);    

    update_selector_from_yaw(0);  // Set initial position
}
