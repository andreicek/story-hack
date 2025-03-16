#include <furi.h>
#include <furi_hal.h>

#define MIFARE_CLASSIC_BLOCKS 64
#define MIFARE_BLOCK_SIZE 16

typedef struct {
    uint8_t uid[4];
    uint8_t atqa[2];
    uint8_t sak;
    uint8_t blocks[MIFARE_CLASSIC_BLOCKS][MIFARE_BLOCK_SIZE];
} MifareClassicCard;

MifareClassicCard generate_mifare_card(uint32_t increment) {
    MifareClassicCard card;

    uint8_t base_uid[4] = {0x09, 0xD9, 0x23, 0x81};
    base_uid[3] = (base_uid[3] + (increment & 0xFF)) & 0xFF;

    for (int i = 0; i < 4; i++) {
        card.uid[i] = base_uid[i];
    }
    card.atqa[0] = 0x00;
    card.atqa[1] = 0x04;
    card.sak = 0x08;

    uint8_t template_card[MIFARE_CLASSIC_BLOCKS][MIFARE_BLOCK_SIZE] = {
        {base_uid[0], base_uid[1], base_uid[2], base_uid[3], 0x72, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x19, 0x01, 0x01, 0x01, 0x00, 0x01},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    };

    for (int i = 1; i < 16; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < MIFARE_BLOCK_SIZE; k++) {
                card.blocks[i * 4 + j][k] = 0x00;
            }
        }
        for (int k = 0; k < MIFARE_BLOCK_SIZE; k++) {
            card.blocks[i * 4 + 3][k] = template_card[3][k];
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < MIFARE_BLOCK_SIZE; j++) {
            card.blocks[i][j] = template_card[i][j];
        }
    }

    return card;
}

int32_t storyhack_app(void* p) {
    UNUSED(p);
    FURI_LOG_I("NFC_EMULATOR", "Starting Mifare Classic Emulation");

    return 0;
}
