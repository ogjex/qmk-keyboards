// First figure out what this include means
#include QMK_KEYBOARD_H

enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL,
    MY_OTHER_MACRO,
};

// declare tap dance codes using the 'enum' list
enum {
    TD_01 = 0,
    TD_02 = 0
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case QMKBEST:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING("QMK is the best thing ever!");
        } else {
            // when keycode QMKBEST is released
        }
        break;

    case QMKURL:
        if (record->event.pressed) {
            // when keycode QMKURL is pressed
            SEND_STRING("https://qmk.fm/\n");
        } else {
            // when keycode QMKURL is released
        }
        break;

    case MY_OTHER_MACRO:
        if (record->event.pressed) {
           SEND_STRING(SS_LCTL("ac")); // selects all and copies
        }
        break;
    }
    return true;
};

// example of creating a custom function called 'safe_reset'
void safe_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 3) {
    // Reset the keyboard if you tap the key more than three times
    reset_keyboard();
    reset_tap_dance(state);
  }
}

// define the tap dance actions
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_01] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_TAB)
};

// define the various layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
    //--------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_Q, KC_W, KC_E, KC_R, KC_T,                       KC_Y, KC_U, KC_I, KC_O, KC_P,
    //--------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_A, KC_S, KC_D, KC_F, KC_G,                       KC_H, KC_J, KC_K, KC_L, KC_SCLN,
    //--------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_Z, KC_X, KC_C, KC_V, KC_DEL,                     KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
    //--------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
            OSM(MOD_LSFT), KC_LALT,                     OSL(1), OSM(MOD_LCTL)
    ),


    [1] = LAYOUT(
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_ESC, KC_AT, KC_HASH, KC_DLR, KC_PERC,            KC_AMPR, KC_PSLS, KC_PIPE, KC_PMNS, KC_QUES,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_TAB, KC_CIRC, KC_AT, C_QUOT, KC_PIPE,            KC_EXLM, KC_ASTR, KC_LPRN, KC_QUES, KC_ENT,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_LT, KC_GT, KC_TILD, KC_GRV, KC_TRNS,             KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, TO(3),
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                            TO(0), KC_TRNS,             TO(2), KC_TRNS
    ),


    [2] = LAYOUT(
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_ESC, KC_TRNS, KC_PSLS, KC_PAST, KC_PMNS,         KC_PEQL, KC_P7, KC_P8, KC_P9, KC_BSPC,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_TAB, KC_NO, KC_NO, KC_NO, KC_PPLS,               KC_PERC, KC_P4, KC_P5, KC_P6, KC_PENT,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_DEL,         KC_P0, KC_P1, KC_P2, KC_P3, KC_PEQL,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                TO(0), KC_TRNS,         KC_TRNS, KC_TRNS
    ),


    [3] = LAYOUT(
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_ESC, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,         KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_TAB, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,         KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_ENT,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
    KC_NO, KC_BTN2, KC_LGUI, KC_LALT, KC_DEL,           KC_BTN1, KC_ACL1, KC_ACL2, KC_PGDN, KC_PGUP,
    // --------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                TO(0), KC_TRNS,         KC_LGUI, OSM(MOD_LCTL)
    ),
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
