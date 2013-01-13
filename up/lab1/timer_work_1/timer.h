#ifndef __TIMER_H
#define __TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define bit_get(byte, bit) (byte) & (1<<(bit))
#define bit_set(byte, bit) (byte) |= (1<<(bit))
#define bit_unset(byte, bit) (byte) &= ~(1<<(bit))

#define bit_set_cond(byte, bit, cond) \
    if (cond) bit_set(byte, bit); \
    else bit_unset(byte, bit)

/*
 * timer_set_prescale(PRESCALE_1);
 * timer_set_prescale(PRESCALE_128);
 */

/*
 * These are numbered 000 -> 111, which are what TCCR2B CS2[0-2] expects.
 */
typedef enum {
    PRESCALE_0,
    PRESCALE_1,
    PRESCALE_8,
    PRESCALE_32,
    PRESCALE_64,
    PRESCALE_128,
    PRESCALE_256,
    PRESCALE_1024,
} prescale_t;

static inline
void timer_set_prescale(prescale_t scale) {
    bit_set_cond(TCCR2B, CS20, scale & 1);
    bit_set_cond(TCCR2B, CS21, scale & 2);
    bit_set_cond(TCCR2B, CS22, scale & 4);
}

/*
 * Usage:
 * timer_set_mode(TIMER_NORMAL);
 */

typedef enum {
    TIMER_NORMAL,
    TIMER_PWM_MAX,
    TIMER_CTC,
    TIMER_PWM_FAST_MAX,
    _TIMER_INVALID1,
    TIMER_PWM,
    _TIMER_INVALID2,
    TIMER_PWM_FAST
} timer_mode_t;

static inline
void timer_set_mode(timer_mode_t mode) {
    bit_set_cond(TCCR2A, WGM20, mode & 1);
    bit_set_cond(TCCR2A, WGM21, mode & 2);
    bit_set_cond(TCCR2B, WGM22, mode & 4);
}

/*
 * Usage:
 * timer_on_compare_a(TIMER_COMPARE_NONE);
 * timer_on_compare_a(TIMER_COMPARE_TOGGLE);
 * timer_on_compare_a(TIMER_COMPARE_CLEAR);
 * timer_on_compare_a(TIMER_COMPARE_SET);
 */

typedef enum {
    TIMER_COMPARE_NONE,
    TIMER_COMPARE_TOGGLE,
    TIMER_COMPARE_CLEAR,
    TIMER_COMPARE_SET
} timer_compare_t;

static inline
void timer_on_compare_a(timer_compare_t action) {
    bit_set_cond(TCCR2A, COM2A0, action & 1);
    bit_set_cond(TCCR2A, COM2A1, action & 2);
}

static inline
void timer_on_compare_b(timer_compare_t action) {
    bit_set_cond(TCCR2A, COM2B0, action & 1);
    bit_set_cond(TCCR2A, COM2B1, action & 2);
}

static inline
void timer_set_compare_a(uint8_t value) {
    OCR2A = value;
}

static inline
void timer_set_compare_b(uint8_t value) {
    OCR2B = value;
}

#endif