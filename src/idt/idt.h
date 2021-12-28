/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:58:23
 * @modify date 2021-12-26 10:58:23
 * @desc [description]
 */

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_descr32
{
    uint16_t lower_bits_offset;
    uint16_t segment_selector;
    uint8_t zero;
    uint8_t type_attributes;
    uint16_t upper_bits_offset;

} __attribute__((packed));

struct idtr_descr32
{
    uint16_t limit; // -1
    uint32_t base; // this point to the array position of the first intterrupt.

} __attribute__((packed));

void idt_init();
void enable_interrupts();
void disable_interrupts();

#endif