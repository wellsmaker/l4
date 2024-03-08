/*
 * Copyright (c) 2017 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <arch/irq.h>
#include <arch/cpu.h>


/* These values are not included in the resulting binary, but instead form the
 * header of the initList section, which is used by gen_isr_tables.py to create
 * the vector and sw isr tables,
 */
GENERIC_SECTION(.irq_info) struct isr_list_header isr_header = {
	.table_size = IRQ_TABLE_SIZE,
	.offset = CONFIG_GEN_IRQ_START_VECTOR,
};


/* These are placeholder tables. They will be replaced by the real tables
 * generated by gen_isr_tables.py.
 *
 * irq_spurious and isr_wrapper are used as placeholder values to
 * ensure that they are not optimized out in the first link. The first
 * link must contain the same symbols as the second one for the code
 * generation to work.
 */

/* Some arches don't use a vector table, they have a common exception entry
 * point for all interrupts. Don't generate a table in this case.
 */
#ifdef CONFIG_GEN_IRQ_VECTOR_TABLE
u32_t __irq_vector_table irq_vector_table[IRQ_TABLE_SIZE] = {
	[0 ...(IRQ_TABLE_SIZE - 1)] = (u32_t)&isr_wrapper,
};
#endif

/* If there are no interrupts at all, or all interrupts are of the 'direct'
 * type and bypass the sw_isr_table, then do not generate one.
 */
#ifdef CONFIG_GEN_SW_ISR_TABLE
struct isr_table_entry __sw_isr_table sw_isr_table[IRQ_TABLE_SIZE] = {
	[0 ...(IRQ_TABLE_SIZE - 1)] = {(void *)0x42, (void *)&irq_spurious},
};
#endif