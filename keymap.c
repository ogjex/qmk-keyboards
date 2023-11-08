#include QMK_KEYBOARD_H

#include "features/tapdance.h"
#include "definitions/keycodes.h"

// define the various layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
    // Base key input layer--------------------         -----------------------------------------------
    TD(TD_TEST_STRING), KC_W, KC_E, KC_R, KC_T,         KC_Y, KC_U, KC_I, KC_O, KC_P,
    //-----------------------------------------         -----------------------------------------------
    KC_A, KC_S, KC_D, KC_F, KC_G,                       KC_H, KC_J, KC_K, KC_L, TD(TD_SCOLON_ENTER),
    //-----------------------------------------         -----------------------------------------------
    KC_Z, TD(TD_DK_LAYR), KC_C, KC_V, KC_DEL,           KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
    //-----------------------------------------         -----------------------------------------------
                        OSM(MOD_LSFT), KC_SPC,          OSL(1), OSM(MOD_LCTL)
    ),


    [1] = LAYOUT(
    // Signs and symbols layer, from layer 0---         ----------------------------------------------
    KC_ESC, KC_AT, KC_HASH, KC_DLR, KC_PERC,            KC_AMPR, KC_PSLS, KC_PIPE, KC_PMNS, KC_QUES,
    // ----------------------------------------         ----------------------------------------------
    KC_TAB, KC_CIRC, KC_AT, KC_QUOT, KC_PIPE,            KC_EXLM, KC_ASTR, KC_LPRN, KC_QUES, KC_ENT,
    // ----------------------------------------         ----------------------------------------------
    KC_LT, KC_GT, KC_TILD, KC_GRV, KC_TRNS,             KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, TO(3),
    // ----------------------------------------         ----------------------------------------------
                            TO(0), KC_TRNS,             TO(2), KC_TRNS
    ),


    [2] = LAYOUT(
    // Numpad layer, from layer 1--------------         ---------------------------------------------
    KC_ESC, KC_TRNS, KC_PSLS, KC_PAST, KC_PMNS,         KC_PEQL, KC_7, KC_8, KC_9, KC_BSPC,
    // ----------------------------------------         ---------------------------------------------
    KC_TAB, KC_NO, KC_NO, KC_NO, KC_PPLS,               KC_PERC, KC_4, KC_5, KC_6, KC_ENT,
    // ----------------------------------------         ---------------------------------------------
    KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_DEL,         KC_0, KC_1, KC_2, KC_3, KC_PEQL,
    // ----------------------------------------         ---------------------------------------------
                                TO(0), KC_TRNS,         TO(4), KC_TRNS
    ),


    [3] = LAYOUT(
    // Navigation layer, from base layer 0-----         --------------------------------------------
    KC_ESC, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,         KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    // ----------------------------------------         --------------------------------------------
    KC_TAB, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,         KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_ENT,
    // ----------------------------------------         --------------------------------------------
    KC_NO, KC_BTN2, KC_LGUI, KC_LALT, KC_DEL,           KC_ACL0, KC_ACL1, KC_ACL2, KC_PGDN, KC_PGUP,
    // ----------------------------------------         --------------------------------------------
                                TO(0), KC_BTN2,         KC_BTN1, OSM(MOD_LCTL)
    ),

    [4] = LAYOUT(
    // Reset layer, from layer 3---------------         --------------------------------------------
    TD(TD_RESET), KC_NO, KC_NO, KC_NO, KC_NO,           KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
                                TO(0), KC_NO,           KC_NO, KC_NO

    ),
};




#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
