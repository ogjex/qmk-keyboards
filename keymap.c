#include "action.h"
#include "action_code.h"
#include "keycodes.h"
#include QMK_KEYBOARD_H

#include "features/tapdance.h"
#include "definitions/keycodes.h"

// Define our tap dance states
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    td_state_t state;
    bool       recording;
} td_tap_t;

 //Our custom tap dance keys; add any other tap dance keys to this enum
enum {
    TD_RESET,
    TD_DELETE,
    TD_AE_ENTER,
    TD_TEST_STRING,
};

// define the various layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
    // Base key input layer--------------------         -----------------------------------------------
    TD(TD_TEST_STRING), KC_W, KC_E, KC_R, KC_T,         KC_Y, KC_U, KC_I, KC_O, KC_P,
    //-----------------------------------------         -----------------------------------------------
    KC_A, KC_S, KC_D, KC_F, KC_G,                       KC_H, KC_J, KC_K, KC_L, TD(TD_AE_ENTER),
    //-----------------------------------------         -----------------------------------------------
    KC_Z, KC_X , KC_C, KC_V, TD(TD_DELETE),           KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
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
    KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, TD(TD_DELETE),         KC_0, KC_1, KC_2, KC_3, KC_PEQL,
    // ----------------------------------------         ---------------------------------------------
                                TO(0), KC_TRNS,         TO(4), KC_TRNS
    ),


    [3] = LAYOUT(
    // Navigation layer, from base layer 0-----         --------------------------------------------
    KC_ESC, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,         KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    // ----------------------------------------         --------------------------------------------
    KC_TAB, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,         KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_ENT,
    // ----------------------------------------         --------------------------------------------
    KC_BTN2, KC_CUT, KC_COPY, KC_PASTE, KC_DEL,           KC_ACL0, KC_ACL1, KC_ACL2, KC_PGDN, KC_PGUP,
    // ----------------------------------------         --------------------------------------------
                            TO(0), OSM(MOD_LSFT),         KC_BTN1, OSM(MOD_LCTL)
    ),

    [4] = LAYOUT(
    // Reset layer, from layer 3---------------         --------------------------------------------
    TD(TD_RESET), KC_Q, KC_NO, KC_NO, KC_NO,           KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // ----------------------------------------         ---------------------------------------------
                                TO(0), KC_NO,           KC_NO, KC_NO

    ),
};

// -------------------------------------------------------------------------------------
// TAP DANCE INITIALISATION
// -------------------------------------------------------------------------------------
// initiate handlers to define the types of taps

td_state_t dance_state(tap_dance_state_t *state);

static td_tap_t tap_state = {.state = TD_NONE};

td_state_t dance_state(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else
            return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted)
            return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return TD_DOUBLE_HOLD;
        else
            return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed)
            return TD_TRIPLE_TAP;
        else
            return TD_TRIPLE_HOLD;
    } else
        return TD_UNKNOWN;
}

// example of a full tap dance function structure
void td_send_success_strings(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING("key tapped once");
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING("key tapped twice");
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING("key held once");
            break;
        case TD_DOUBLE_HOLD:
            SEND_STRING("key held twice");
            break;
        case TD_TRIPLE_TAP:
            SEND_STRING("key tapped thrice");
            break;
        case TD_TRIPLE_HOLD:
            SEND_STRING("key held thrice");
            break;
        default:
            break;
    }
}

// defining DK å
void td_aa(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_A);
            break;
        case TD_SINGLE_HOLD:
            tap_code(KC_LBRC);
            break;
        default:
            break;
    }
}

// defining temporary ø
void td_oe_enter(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_O);
            break;
        case TD_SINGLE_HOLD:
            tap_code(KC_LQUOT);
            break;
        default:
            break;
    }
}

// defining ae and enter tapdance
void td_ae_enter(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_SCLN);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_ENT);
            break;
        default:
            reset_tap_dance(state);
            break;
    }
}

// defining delete key macro
void td_delete(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_DEL);

            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_DEL) SS_UP(X_LCTL));

            break;
        case TD_DOUBLE_HOLD:
            SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_END) SS_TAP(X_DEL) SS_UP(X_LSFT) SS_UP(X_END));

            break;
        default:
            break;
    }
}

// defining backspace key macro
void td_bspace(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_BSPC);

            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_BSPC) SS_UP(X_LCTL));

            break;
        case TD_DOUBLE_HOLD:
            SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_HOME) SS_TAP(X_BSPC) SS_UP(X_LSFT) SS_UP(X_HOME));

            break;
        default:
            break;
    }
}

// enable keyboard reset key
void safe_reset(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_HOLD:
            SEND_STRING("keyboard should be reset now");
            reset_keyboard();
            break;
        default:
            break;
    }
}

// defining tab switcher tapdance key
void td_tabswitch(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_DOWN(X_LALT) SS_TAP(X_TAB) SS_UP(X_LALT));
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_TAB) SS_UP(X_LGUI));

            break;
        default:
            break;
    }
}

// defining home tapdance key
void td_home(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_HOME);

            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_BSPC) SS_UP(X_LCTL));

            break;
        default:
            break;
    }
}


//Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
  [TD_RESET] = ACTION_TAP_DANCE_FN(safe_reset),
  [TD_DELETE] = ACTION_TAP_DANCE_FN(td_delete),
  [TD_AE_ENTER] = ACTION_TAP_DANCE_FN(td_ae_enter),
  [TD_TEST_STRING] = ACTION_TAP_DANCE_FN(td_send_success_strings)

};

// old code from here

// -------------------------------------------------------------------------------------
// ONESHOT MOD INITIALISATION
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// MACRO INITIALISATION
// -------------------------------------------------------------------------------------

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
