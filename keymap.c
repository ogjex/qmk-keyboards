#include QMK_KEYBOARD_H

#include "features/tapdance.h"
#include "definitions/keycodes.h"

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
    TD_RESET = 0,
    TD_SCOLON_ENTER,
    TD_DK_LAYR = 0,
    TD_TEST_STRING
};

td_state_t dance_state(tap_dance_state_t *state);


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

// -------------------------------------------------------------------------------------
// TAP DANCE INITIALISATION
// -------------------------------------------------------------------------------------
// initiate handlers to define the types of taps

static td_tap_t tap_state = {.state = TD_NONE};

__attribute__((weak)) td_state_t dance_state(tap_dance_state_t *state) {
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

// enable keyboard reset key
void safe_reset(tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
        case TD_TRIPLE_HOLD:
            reset_keyboard();
            reset_tap_dance(state);
            SEND_STRING("keyboard should be reset now");
            break;
        default:
            break;
    }
}

// old code from here

// old function that switches layers
void ql_finished (tap_dance_state_t *state, void *user_data) {
    tap_state.state = dance_state(state);
    switch (tap_state.state) {
    case TD_SINGLE_TAP:
      tap_code(KC_X);
      SEND_STRING("X has been typed, you tapped once!");
      break;
    case TD_DOUBLE_TAP:
      //check to see if the layer is already set
      if (layer_state_is(2)) {
        //if already set, then switch it off
        layer_off(2);
         SEND_STRING("QMK layer turned off!");
      } else {
        //if not already set, then switch the layer on
        layer_on(2);

        SEND_STRING("QMK layer turned on, you tapped twice");
      }
      break;
    default:
      break;
  }
}

void ql_reset (tap_dance_state_t *state, void *user_data) {
  //if the key was held down and now is released then switch off the layer
  if (tap_state.state==TD_SINGLE_HOLD) {
    layer_off(2);
  }
  tap_state.state = 0;
}


//Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
  [TD_RESET] = ACTION_TAP_DANCE_FN(safe_reset),
  [TD_SCOLON_ENTER] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_ENTER),
  [TD_TEST_STRING] = ACTION_TAP_DANCE_FN(td_send_success_strings),
  [TD_DK_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)

};

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
