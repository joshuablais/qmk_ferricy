#include QMK_KEYBOARD_H
#include "print.h"

enum layers { _HOME, _NUM, _NAV };

// ---------------------------------------------------------------------
// Positional home-row mod helpers. Fixed hand/finger -> modifier mapping,
// defined once. Every layer just calls these with its own base keycode.
// Left hand outer->inner: GUI, Alt, Ctrl, Shift
// Right hand inner->outer: Shift, Ctrl, Alt, GUI
// ---------------------------------------------------------------------
#define HOME_L1(kc) LGUI_T(kc)   // left pinky  - outer - weak
#define HOME_L2(kc) LALT_T(kc)   // left ring   - outer - weak
#define HOME_L3(kc) LCTL_T(kc)   // left middle - inner - shifthr
#define HOME_L4(kc) LSFT_T(kc)   // left index  - inner - shifthr
#define HOME_R1(kc) RSFT_T(kc)   // right index  - inner - shifthr
#define HOME_R2(kc) RCTL_T(kc)   // right middle - inner - shifthr
#define HOME_R3(kc) RALT_T(kc)   // right ring   - outer - weak
#define HOME_R4(kc) RGUI_T(kc)   // right pinky  - outer - weak

// _HOME layer
#define W_A  HOME_L1(KC_A)
#define W_R  HOME_L2(KC_R)
#define S_S  HOME_L3(KC_S)
#define S_T  HOME_L4(KC_T)
#define S_N  HOME_R1(KC_N)
#define S_E  HOME_R2(KC_E)
#define W_I  HOME_R3(KC_I)
#define W_O  HOME_R4(KC_O)

// _NUM layer — N_LPRN/N_RPRN use a plain basic keycode as the mod-tap's
// tap argument (KC_LPRN/KC_RPRN are shifted 16-bit keycodes and illegal
// inside a mod-tap slot), then process_record_user substitutes the real
// shifted output on tap, same pattern as your original M_LPRN/M_RPRN.
#define N_HASH HOME_L1(KC_HASH)
#define N_DLR  HOME_L2(KC_DLR)
#define N_LPRN HOME_L3(KC_9)
#define N_RPRN HOME_L4(KC_0)
#define N_4    HOME_R1(KC_4)
#define N_5    HOME_R2(KC_5)
#define N_6    HOME_R3(KC_6)
#define N_COLN HOME_R4(KC_COLN)

// _NAV layer
#define G_F5   HOME_L1(KC_F5)
#define G_F6   HOME_L2(KC_F6)
#define G_F7   HOME_L3(KC_F7)
#define G_F8   HOME_L4(KC_F8)
#define G_DOWN HOME_R1(KC_DOWN)
#define G_UP   HOME_R2(KC_UP)
#define G_RGHT HOME_R3(KC_RGHT)
#define G_NO   HOME_R4(KC_NO)

enum custom_keycodes { M_EMAIL = SAFE_RANGE };

// ---------------------------------------------------------------------
// Generic tapping-term classification. Reads the modifier bits out of
// the keycode itself instead of enumerating every mod-tap key by name.
// & 0x0F strips the "right hand" flag bit so RCTL and LCTL both resolve
// to the same base modifier. Add a mod-tap to a fourth layer tomorrow
// and this function needs no edits.
// ---------------------------------------------------------------------
static uint16_t home_row_mod_term(uint16_t keycode, uint16_t default_term) {
    if (!IS_QK_MOD_TAP(keycode)) return default_term;

    uint8_t mod = QK_MOD_TAP_GET_MODS(keycode) & 0x0F;
    switch (mod) {
        case MOD_LGUI:
        case MOD_LALT:
            return 200;  // outer columns - weak - slower
        case MOD_LCTL:
        case MOD_LSFT:
            return 165;  // inner columns - shifthr - faster
        default:
            return default_term;
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return home_row_mod_term(keycode, TAPPING_TERM);
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    return home_row_mod_term(keycode, QUICK_TAP_TERM);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_HOME] = LAYOUT_split_3x5_2(
        KC_Q,  KC_W,  KC_F,  KC_P,  KC_B,        KC_J,  KC_L,  KC_U,    KC_Y,   KC_QUOT,
        W_A,   W_R,   S_S,   S_T,   KC_G,        KC_M,  S_N,   S_E,     W_I,    W_O,
        KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,        KC_K,  KC_H,  KC_COMM, KC_DOT, LT(0, KC_SLSH),
                  LT(_NUM, KC_BSPC), LT(_NAV, KC_TAB),   LT(_NAV, KC_ENT), LT(_NUM, KC_SPC)
    ),
    [_NUM] = LAYOUT_split_3x5_2(
        KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,     KC_PLUS, KC_7,   KC_8,   KC_9,   KC_ASTR,
        N_HASH,  N_DLR,   N_LPRN,  N_RPRN,  KC_GRV,      KC_MINS, N_4,    N_5,    N_6,    N_COLN,
        KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,     KC_EQL,  KC_1,   KC_2,   KC_3,   KC_BSLS,
                       LCTL(KC_BSPC), KC_DEL,            KC_SPC,  KC_0
    ),
    [_NAV] = LAYOUT_split_3x5_2(
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  M_EMAIL,    KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,
        G_F5,   G_F6,   G_F7,   G_F8,   KC_NO,      KC_LEFT, G_DOWN,  G_UP,   G_RGHT,  G_NO,
        QK_BOOT, KC_F10, KC_F11, KC_F12, KC_PSCR,    KC_NO,   LCTL(KC_V), LALT(KC_V), KC_NO, QK_BOOT,
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

        case N_LPRN:
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_LPRN);
                return false;
            }
            return true;   // hold -> real Ctrl

        case N_RPRN:
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_RPRN);
                return false;
            }
            return true;   // hold -> real Shift

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
