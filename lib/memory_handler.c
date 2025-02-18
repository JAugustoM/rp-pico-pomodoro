#include "pico/stdlib.h"
#include "hardware/flash.h"
#include <hardware/sync.h>
#include "memory_handler.h"
#include "screen_handler.h"
#include <stdio.h>

const int FLASH_TARGET_OFFSET = (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE);
const int addr = XIP_BASE + FLASH_TARGET_OFFSET;
const uint8_t* base_adress = (uint8_t*) addr;

uint8_t buf[FLASH_PAGE_SIZE/sizeof(uint8_t)];

void initialize_memory() {
    if (base_adress[0] == 200) {
        printf("%u\n", base_adress[0]);
        foco = base_adress[1];
        printf("%u\n", base_adress[1]);
        descanso = base_adress[2];
        printf("%u\n", base_adress[2]);
        ciclos = base_adress[3];
        printf("%u\n", base_adress[3]);
    } else {
        uint32_t ints = save_and_disable_interrupts();
        flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);
        restore_interrupts (ints);
    }
}

void store_values(uint8_t foco_valor, uint8_t descanso_valor, uint8_t ciclos_valor) {
    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);

    buf[0] = 200;
    buf[1] = foco_valor;
    buf[2] = descanso_valor;
    buf[3] = ciclos_valor;

    flash_range_program((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), (uint8_t *)buf, FLASH_PAGE_SIZE);

    restore_interrupts(ints);

    printf("%u\n", base_adress[0]);
    printf("%u\n", base_adress[1]);
    printf("%u\n", base_adress[2]);
    printf("%u\n", base_adress[3]);
}