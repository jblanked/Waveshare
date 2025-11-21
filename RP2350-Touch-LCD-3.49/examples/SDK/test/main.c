#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "battery/battery.h"
#include "lcd/lcd.h"
#include "touch/touch.h"
#include "qmi/qmi.h"
#include "hardware/clocks.h"
#include "sd/sdcard.h"
#include "sd/fat32.h"
#include "assets.h"

#define PLL_SYS_KHZ 150 * 1000

// Test data for writing to SD card
const char test_data[] = "Hello from Waveshare Touch LCD! This is a test write to the SD card. Block test successful!\n";
uint8_t read_buffer[512];  // Buffer for reading SD card blocks
uint8_t write_buffer[512]; // Buffer for writing SD card blocks

void touch_callback(uint gpio, uint32_t events)
{
    if (gpio != TOUCH_INT_PIN)
    {
        return;
    }

    TouchVector last_touch_point = touch_get_point();
    if (last_touch_point.x != 0 || last_touch_point.y != 0)
    {
        printf("Touch at (%d, %d)\r\n", last_touch_point.x, last_touch_point.y);
        printf("Battery percentage: %d%%, voltage: %.2f V\r\n", battery_get_percentage(), battery_get_voltage());
    }
}

void test_sd_card_operations()
{
    printf("\n=== Waveshare Touch LCD SD Card Test ===\n");

    // Initialize SD card
    printf("Initializing SD card...\n");
    sd_init();

    // Initialize SD card
    printf("Initializing SD card communication...\n");
    sd_error_t init_result = sd_card_init();
    if (init_result != SD_OK)
    {
        printf("ERROR: SD card initialization failed: %s\n", sd_error_string(init_result));
        printf("Check SPI connections:\n");
        printf("  MISO (GPIO %d) - SD card DO pin\n", SD_MISO);
        printf("  MOSI (GPIO %d) - SD card DI pin\n", SD_MOSI);
        printf("  SCK  (GPIO %d) - SD card CLK pin\n", SD_SCK);
        printf("  CS   (GPIO %d) - SD card CS pin\n", SD_CS);
        return;
    }
    printf("✓ SD card initialized successfully!\n");

    // Check card type
    if (sd_is_sdhc())
    {
        printf("✓ Card type: SDHC/SDXC (High Capacity)\n");
    }
    else
    {
        printf("✓ Card type: SDSC (Standard Capacity)\n");
    }

    // Prepare test data
    printf("\nPreparing test data...\n");
    memset(write_buffer, 0, sizeof(write_buffer));
    strncpy((char *)write_buffer, test_data, sizeof(test_data));
    printf("Test data: %.60s...\n", write_buffer);

    // Test block write (block 100 - safe area on most cards)
    uint32_t test_block = 100;
    printf("\nWriting test data to block %d...\n", test_block);
    sd_error_t write_result = sd_write_block(test_block, write_buffer);
    if (write_result != SD_OK)
    {
        printf("ERROR: Write failed: %s\n", sd_error_string(write_result));
        return;
    }
    printf("✓ Write successful!\n");

    // Clear read buffer
    memset(read_buffer, 0, sizeof(read_buffer));

    // Test block read
    printf("Reading data back from block %d...\n", test_block);
    sd_error_t read_result = sd_read_block(test_block, read_buffer);
    if (read_result != SD_OK)
    {
        printf("ERROR: Read failed: %s\n", sd_error_string(read_result));
        return;
    }
    printf("✓ Read successful!\n");

    // Verify data
    printf("\nVerifying data integrity...\n");
    printf("Original:  %.60s...\n", write_buffer);
    printf("Read back: %.60s...\n", read_buffer);

    if (memcmp(write_buffer, read_buffer, strlen(test_data)) == 0)
    {
        printf("✓ Data verification PASSED! Read/Write working correctly.\n");
    }
    else
    {
        printf("✗ Data verification FAILED! Data corruption detected.\n");
        printf("This might indicate SPI timing issues or connection problems.\n");
        return;
    }

    // Test multiple block operations
    printf("\nTesting multiple block operations...\n");
    uint32_t start_block = 200;
    uint32_t num_blocks = 3;

    // Prepare multi-block test data
    uint8_t multi_write_buffer[512 * 3];
    uint8_t multi_read_buffer[512 * 3];

    for (int i = 0; i < 3; i++)
    {
        snprintf((char *)&multi_write_buffer[i * 512], 512,
                 "Block %d: Waveshare Touch LCD multi-block test data. Block number %d of 3.\n",
                 start_block + i, i + 1);
    }

    printf("Writing %d blocks starting at block %d...\n", num_blocks, start_block);
    sd_error_t multi_write_result = sd_write_blocks(start_block, num_blocks, multi_write_buffer);
    if (multi_write_result != SD_OK)
    {
        printf("ERROR: Multi-block write failed: %s\n", sd_error_string(multi_write_result));
    }
    else
    {
        printf("✓ Multi-block write successful!\n");

        printf("Reading %d blocks back...\n", num_blocks);
        sd_error_t multi_read_result = sd_read_blocks(start_block, num_blocks, multi_read_buffer);
        if (multi_read_result != SD_OK)
        {
            printf("ERROR: Multi-block read failed: %s\n", sd_error_string(multi_read_result));
        }
        else
        {
            printf("✓ Multi-block read successful!\n");

            // Show some data from each block
            for (int i = 0; i < 3; i++)
            {
                printf("Block %d: %.50s...\n", start_block + i, &multi_read_buffer[i * 512]);
            }

            if (memcmp(multi_write_buffer, multi_read_buffer, 512 * 3) == 0)
            {
                printf("✓ Multi-block verification PASSED!\n");
            }
            else
            {
                printf("✗ Multi-block verification FAILED!\n");
            }
        }
    }

    printf("\n=== SD Card Test Complete ===\n");
    printf("If all tests passed, your Waveshare Touch LCD SD card interface is working correctly!\n");
}

void test_file_operations()
{
    printf("\n=== FAT32 File System Test ===\n");

    const char *test_filename = "waveshare_lcd_test.txt";
    const char *write_data = "Hello from Waveshare Touch LCD!\n"
                             "This is a test file created by the RP2040.\n"
                             "Line 3: SD card file operations working!\n"
                             "Line 4: Your VGM attachment is ready! 🎮\n"
                             "Line 5: Timestamp: Loop counter will be added here...\n";

    char read_buffer[512];
    size_t bytes_written = 0;
    size_t bytes_read = 0;
    fat32_file_t file;
    fat32_error_t result;

    // Test 1: Create/Write to file
    printf("Creating and writing to file: %s\n", test_filename);

    result = fat32_create(&file, test_filename);
    if (result != FAT32_OK)
    {
        printf("Failed to create file: %d\n", result);
        return;
    }

    result = fat32_write(&file, write_data, strlen(write_data), &bytes_written);
    if (result != FAT32_OK)
    {
        printf("Failed to write to file: %d\n", result);
        fat32_close(&file);
        return;
    }

    fat32_close(&file);
    printf("✓ File created and written successfully! (%zu bytes)\n", bytes_written);

    // Test 2: Read from file
    printf("Reading from file: %s\n", test_filename);

    result = fat32_open(&file, test_filename);
    if (result != FAT32_OK)
    {
        printf("Failed to open file for reading: %d\n", result);
        return;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    result = fat32_read(&file, read_buffer, sizeof(read_buffer) - 1, &bytes_read);
    if (result != FAT32_OK)
    {
        printf("Failed to read from file: %d\n", result);
        fat32_close(&file);
        return;
    }

    fat32_close(&file);
    printf("✓ File read successfully! (%zu bytes)\n", bytes_read);

    // Test 3: Verify data integrity
    printf("\nFile Contents:\n");
    printf("----------------------------------------\n");
    printf("%s", read_buffer);
    printf("----------------------------------------\n");

    if (strncmp(write_data, read_buffer, strlen(write_data)) == 0)
    {
        printf("✓ File data verification PASSED!\n");
    }
    else
    {
        printf("✗ File data verification FAILED!\n");
        printf("Expected: %.50s...\n", write_data);
        printf("Got:      %.50s...\n", read_buffer);
    }

    printf("\n=== File System Test Complete ===\n");
}

int main()
{
    stdio_init_all();

    set_sys_clock_khz(PLL_SYS_KHZ, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        PLL_SYS_KHZ * 1000,
        PLL_SYS_KHZ * 1000);

    // Wait a moment for serial connection
    sleep_ms(2000);

    // Run SD card tests
    test_sd_card_operations();

    // Initialize FAT32 if tests passed
    printf("\nInitializing FAT32 filesystem...\n");
    fat32_init();

    // Run file system tests
    test_file_operations();

    lcd_init(); // Initialize LCD in horizontal mode

    touch_init();                        // Initialize touch in gesture mode
    touch_set_callback(&touch_callback); // Set touch interrupt callback

    battery_init(); // Initialize battery monitoring

    qmi_init(); // Initialize QMI8658 sensor

    // Demo 1: Static shapes
    lcd_fill(COLOR_BLACK); // Clear screen to white

    // I adapted this from the 1.28 and 1.43 inch LCD demos
    // so I simply scaled everything
    uint16_t twenty_y = (uint16_t)LCD_HEIGHT * 0.04292;
    uint16_t twenty_x = (uint16_t)LCD_WIDTH * 0.04292;

    // Draw some shapes to demonstrate the API
    lcd_fill_rect(twenty_x * 3, twenty_y * 7, twenty_x * 5, twenty_y * 3, COLOR_RED);                                          // Red filled rectangle
    lcd_draw_rect(twenty_x * 10, twenty_y, twenty_x * 4, twenty_y * 3, COLOR_GREEN);                                           // Green rectangle outline
    lcd_fill_circle(twenty_x * 18, twenty_y * 8, twenty_x, COLOR_BLUE);                                                        // Blue filled circle
    lcd_draw_circle(twenty_x * 12, twenty_y * 8, twenty_x, COLOR_YELLOW);                                                      // Yellow circle outline
    lcd_draw_line(twenty_x, twenty_y * 12, twenty_x * 23, twenty_y * 12, COLOR_CYAN);                                          // Cyan horizontal line
    lcd_fill_triangle(twenty_x * 12, twenty_y * 15, twenty_x * 10, twenty_y * 19, twenty_x * 14, twenty_y * 19, COLOR_VIOLET); // Violet triangle
    lcd_draw_text(twenty_x * 5, twenty_y * 13, "RP2350 Touch LCD Demo", COLOR_WHITE);                                          // White text

    // Push static shapes to display
    lcd_swap();

    sleep_ms(5000); // Show static demo for 5 seconds

    uint8_t frame = 0;
    uint8_t rotations = 10;
    while (rotations--)
    {
        frame = 0;
        while (frame < 3)
        {
            lcd_fill(COLOR_BLACK);
            lcd_blit(LCD_WIDTH / 2, LCD_HEIGHT / 2, 16, 15, frame == 0 ? frame_0 : (frame == 1 ? frame_1 : frame_2));
            lcd_swap();
            sleep_ms(200);
            frame++;
        }
    }
    // Demo 2: Animation loop
    uint8_t angle = 0;

    float acc[3], gyro[3];
    unsigned int tim_count = 0;
    char acc_text[50];
    char gyro_text[50];

    while (true)
    {
        // Clear screen
        lcd_fill(COLOR_BLACK);

        qmi_read_xyz(acc, gyro, &tim_count);
        snprintf(acc_text, sizeof(acc_text), "X=%4.1f Y=%4.1f Z=%4.1f", acc[0], acc[1], acc[2]);
        snprintf(gyro_text, sizeof(gyro_text), "X=%4.1f Y=%4.1f Z=%4.1f", gyro[0], gyro[1], gyro[2]);
        lcd_draw_text((uint16_t)(twenty_x * 4.5), twenty_y * 6, acc_text, COLOR_WHITE);
        lcd_draw_text((uint16_t)(twenty_x * 4.5), twenty_y * 18, gyro_text, COLOR_WHITE);

        // Draw a circle that changes size
        uint8_t radius = 30 + (angle % 10);
        lcd_draw_circle(twenty_x * 12, twenty_y * 12, radius, COLOR_CYAN);

        // Draw rotating line
        int line_x = twenty_x * 12 + (angle % 40) - 20;
        int line_y = twenty_y * 12 + (angle % 30) - 15;
        lcd_draw_line(twenty_x * 12, twenty_y * 12, line_x, line_y, COLOR_YELLOW);

        // Update display
        lcd_swap();

        // Update for next frame
        angle++;
    }
}
