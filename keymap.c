#include QMK_KEYBOARD_H
#include "print.h"   // at top of keymap.c

enum layers { _HOME, _NUM, _NAV };

// Home-row mods: weak (outer, slower) vs shifthr (inner, faster)
#define W_A  LGUI_T(KC_A)
#define W_R  LALT_T(KC_R)
#define S_S  LCTL_T(KC_S)
#define S_T  LSFT_T(KC_T)
#define S_N  RSFT_T(KC_N)
#define S_E  RCTL_T(KC_E)
#define W_I  RALT_T(KC_I)
#define W_O  RGUI_T(KC_O)

enum custom_keycodes { M_EMAIL = SAFE_RANGE, M_LPRN, M_RPRN };

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case W_A: case W_R: case W_I: case W_O:
            return 200;
        case S_S: case S_T: case S_N: case S_E:
            return 165;
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case W_A: case W_R: case W_I: case W_O:
            return 200;
        case S_S: case S_T: case S_N: case S_E:
            return 165;
        default:
            return QUICK_TAP_TERM;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_HOME] = LAYOUT_split_3x5_2(
        KC_Q,  KC_W,  KC_F,  KC_P,  KC_B,        KC_J,  KC_L,  KC_U,    KC_Y,   KC_QUOT,
        W_A,   W_R,   S_S,   S_T,   KC_G,        KC_M,  S_N,   S_E,     W_I,    W_O,
        KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,        KC_K,  KC_H,  KC_COMM, KC_DOT, LT(0, KC_SLSH),
                  LT(_NUM, KC_BSPC), LT(_NAV, KC_TAB),   LT(_NAV, KC_ENT), LT(_NUM, KC_SPC)
    ),

    [_NUM] = LAYOUT_split_3x5_2(
        KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,     KC_PLUS, KC_7, KC_8, KC_9, KC_ASTR,
        KC_HASH, KC_DLR,  M_LPRN, M_RPRN, KC_GRV,      KC_MINS, KC_4, KC_5, KC_6, KC_COLN,
        KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,     KC_EQL,  KC_1, KC_2, KC_3, KC_BSLS,
                       LCTL(KC_BSPC), KC_DEL,           KC_SPC,  KC_0
    ),

    [_NAV] = LAYOUT_split_3x5_2(
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  M_EMAIL,    KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,
        KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_NO,      KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, KC_NO,
        QK_BOOT,  KC_F10, KC_F11, KC_F12, KC_PSCR,    KC_NO,   LCTL(KC_V),   LALT(KC_V),  KC_NO,   QK_BOOT,
                        KC_NO, KC_TRNS,             KC_TRNS, KC_TRNS
    ),
};

enum combos { CMB_ESC, CMB_UNDER, CMB_DASH, CMB_CAPSWORD, COMBO_LENGTH };
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM esc_combo[]      = {S_T, S_N, COMBO_END};
const uint16_t PROGMEM under_combo[]    = {KC_D, KC_H, COMBO_END};
const uint16_t PROGMEM dash_combo[]     = {KC_P, KC_L, COMBO_END};
const uint16_t PROGMEM capsword_combo[] = {KC_Q, KC_QUOT, COMBO_END};

combo_t key_combos[] = {
    [CMB_ESC]      = COMBO(esc_combo, KC_ESC),
    [CMB_UNDER]    = COMBO(under_combo, KC_UNDS),
    [CMB_DASH]     = COMBO(dash_combo, KC_MINS),
    [CMB_CAPSWORD] = COMBO(capsword_combo, CW_TOGG),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uprintf("kc:%u row:%u col:%u layer:%u mods:%u\n",
                keycode, record->event.key.row, record->event.key.col,
                get_highest_layer(layer_state), get_mods());
    }
    switch (keycode) {
        case M_EMAIL:
            if (record->event.pressed) SEND_STRING("josh@joshblais.com");
            return false;
        case M_LPRN:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                tap_code(KC_9);
                unregister_code(KC_LSFT);
            }
            return false;
        case M_RPRN:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                tap_code(KC_0);
                unregister_code(KC_LSFT);
            }
            return false;
        case LT(0, KC_SLSH):
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_SLSH);
            } else if (record->event.pressed) {
                tap_code16(KC_SCLN);
            }
            return false;
    }
    return true;
}
