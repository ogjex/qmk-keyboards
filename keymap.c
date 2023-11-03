#include QMK_KEYBOARD_H



 //Our custom tap dance key; add any other tap dance keys to this enum
enum {
    TD_RESET = 0,
    TD_SCOLON_ENTER,
    TD_DK_LAYR = 0
};


// define the various layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
    // Base key input layer--------------------         -----------------------------------------------
    KC_Q, KC_W, KC_E, KC_R, KC_T,                       KC_Y, KC_U, KC_I, KC_O, KC_P,
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
    KC_ESC, KC_TRNS, KC_PSLS, KC_PAST, KC_PMNS,         KC_PEQL, KC_P7, KC_P8, KC_P9, KC_BSPC,
    // ----------------------------------------         ---------------------------------------------
    KC_TAB, KC_NO, KC_NO, KC_NO, KC_PPLS,               KC_PERC, KC_P4, KC_P5, KC_P6, KC_PENT,
    // ----------------------------------------         ---------------------------------------------
    KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_DEL,         KC_P0, KC_P1, KC_P2, KC_P3, KC_PEQL,
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

typedef struct {
  bool is_press_action;
  int state;
} tap;

//Define a type for as many tap dance states as you need
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS};

//Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
  .is_press_action = true,
  .state = 0
};

//Declare the functions to be used with your tap dance key(s)

//Function associated with all tap dances
int cur_dance (tap_dance_state_t *state);

//Functions associated with individual tap dances
void ql_finished (tap_dance_state_t *state, void *user_data);
void ql_reset (tap_dance_state_t *state, void *user_data);

void safe_reset(tap_dance_state_t *state, void *user_data) {
  if (state->count >= 3) {
    // Reset the keyboard if you tap the key more than three times
    reset_keyboard();
    reset_tap_dance(state);
  }
}
//Determine the current tap dance state
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}

//Functions that control what our tap dance key does
void ql_finished (tap_dance_state_t *state, void *user_data) {
  ql_tap_state.state = cur_dance(state);
  switch (ql_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_X);
      SEND_STRING("X has been typed, you tapped once!");
      break;
    case DOUBLE_TAP:
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
  }
}

void ql_reset (tap_dance_state_t *state, void *user_data) {
  //if the key was held down and now is released then switch off the layer
  if (ql_tap_state.state==SINGLE_HOLD) {
    layer_off(2);
  }
  ql_tap_state.state = 0;
}

//Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
  [TD_RESET] = ACTION_TAP_DANCE_FN(safe_reset),
  [TD_SCOLON_ENTER] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_ENTER),
  [TD_DK_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)
};



#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
