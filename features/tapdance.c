#include QMK_KEYBOARD_H

#include "tapdance.h"

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

// example of a tap dance function structure
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
        default:
            break;
    }
}


// old code from here

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


