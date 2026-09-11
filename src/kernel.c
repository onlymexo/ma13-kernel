#define VGA_ADDRESS 0xB8000

// Palette Colori VGA
#define COLOR_BLACK 0x0
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

#define KEYBOARD_PORT 0x60
#define KEYBOARD_STATUS 0x64

// I/O Port Helper
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

int cursor_x = 0;
int cursor_y = 0;
unsigned char current_theme = (COLOR_BLACK << 4) | COLOR_LIGHT_GREEN;
char command_buffer[128];
int buffer_idx = 0;
unsigned long ticks = 0;
int shift_pressed = 0;

// LAYOUT ITALIANO (IT-IT) BASE
const char scancode_map_it[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 'i', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'e', '+', '\n',
     0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'a', '\\',   0,
   'u', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',   0, '*',   0, ' '
};

// LAYOUT ITALIANO SHIFT (SIMBOLI SUPERIORI)
const char scancode_map_it_shift[128] = {
    0,  27, '!', '"', '£', '$', '%', '&', '/', '(', ')', '=', '?', '^', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', '*', '\n',
     0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ']', ' ', '|',   0,
   '/', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_',   0, '*',   0, ' '
};

// CONTROLLO CURSORE HARDWARE VGA
void enable_cursor(unsigned char cursor_start, unsigned char cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y) {
    unsigned short pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}

void clear_screen() {
    char* vga = (char*) VGA_ADDRESS;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vga[i] = ' ';
        vga[i+1] = current_theme;
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(cursor_x, cursor_y);
}

void put_char_color(char c, unsigned char fg, unsigned char bg) {
    char* vga = (char*) VGA_ADDRESS;
    unsigned char attribute = (bg << 4) | fg;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 6) {
            cursor_x--;
            vga[(cursor_y * 80 + cursor_x) * 2] = ' ';
            vga[(cursor_y * 80 + cursor_x) * 2 + 1] = attribute;
        }
    } else {
        vga[(cursor_y * 80 + cursor_x) * 2] = c;
        vga[(cursor_y * 80 + cursor_x) * 2 + 1] = attribute;
        cursor_x++;
    }

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= 25) {
        clear_screen();
    }
    update_cursor(cursor_x, cursor_y);
}

void print_color(const char* str, unsigned char fg, unsigned char bg) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char_color(str[i], fg, bg);
    }
}

void print_prompt() {
    print_color("MA13", COLOR_YELLOW, COLOR_BLACK);
    print_color("> ", COLOR_LIGHT_CYAN, COLOR_BLACK);
}

int str_equals(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

int str_starts_with(const char* str, const char* prefix) {
    int i = 0;
    while (prefix[i] != '\0') {
        if (str[i] != prefix[i]) return 0;
        i++;
    }
    return 1;
}

void print_num(int n) {
    if (n == 0) {
        put_char_color('0', COLOR_WHITE, COLOR_BLACK);
        return;
    }
    char buf[12];
    int i = 0;
    if (n < 0) {
        put_char_color('-', COLOR_WHITE, COLOR_BLACK);
        n = -n;
    }
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        put_char_color(buf[j], COLOR_WHITE, COLOR_BLACK);
    }
}

void app_fetch() {
    print_color("\n", COLOR_WHITE, COLOR_BLACK);
    print_color("      .-.              OS: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("MA13 Kernel v0.3 Bare-Metal\n", COLOR_WHITE, COLOR_BLACK);

    print_color("     ( ( )             Keyboard: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("Italian Layout (IT-IT)\n", COLOR_WHITE, COLOR_BLACK);

    print_color("    .-`\"`-.            Dev: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("Madya\n", COLOR_WHITE, COLOR_BLACK);

    print_color("   /       \\           Cursor: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("VGA Hardware Blinking Cursor\n", COLOR_WHITE, COLOR_BLACK);

    print_color("  |  o   o  |          VGA Mode: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("80x25 Color Matrix\n", COLOR_WHITE, COLOR_BLACK);

    print_color("  \\  .-.  /           RAM Allocated: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("16 KB (Protected Stack)\n", COLOR_WHITE, COLOR_BLACK);

    print_color("   `-...-'             Architecture: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("i386 Protected Mode (32-bit)\n", COLOR_WHITE, COLOR_BLACK);

    print_color("  /|       |\\          Uptime Ticks: ", COLOR_YELLOW, COLOR_BLACK);
    print_num(ticks);
    print_color("\n", COLOR_WHITE, COLOR_BLACK);

    print_color(" (_|_______|_)         Status: ", COLOR_YELLOW, COLOR_BLACK);
    print_color("100% Operational\n\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
}

void app_matrix() {
    clear_screen();
    for (int i = 0; i < 400; i++) {
        char random_char = 'A' + (i % 26);
        put_char_color(random_char, COLOR_LIGHT_GREEN, COLOR_BLACK);
        if (i % 3 == 0) put_char_color(' ', COLOR_BLACK, COLOR_BLACK);
    }
    print_color("\n\n--- Matrix Rain End ---\n\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
}

void app_calc(const char* expr) {
    int a = 0, b = 0;
    char op = 0;
    int i = 4;

    while (expr[i] == ' ') i++;

    int found_a = 0;
    while (expr[i] >= '0' && expr[i] <= '9') {
        a = a * 10 + (expr[i] - '0');
        i++;
        found_a = 1;
    }

    while (expr[i] == ' ') i++;

    if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
        op = expr[i];
        i++;
    }

    while (expr[i] == ' ') i++;

    int found_b = 0;
    while (expr[i] >= '0' && expr[i] <= '9') {
        b = b * 10 + (expr[i] - '0');
        i++;
        found_b = 1;
    }

    if (!found_a || !found_b || op == 0) {
        print_color("Sintassi errata! Usa: calc 10+20 oppure calc 10 + 20\n\n", COLOR_RED, COLOR_BLACK);
        return;
    }

    print_color("Risultato: ", COLOR_LIGHT_CYAN, COLOR_BLACK);
    if (op == '+') print_num(a + b);
    else if (op == '-') print_num(a - b);
    else if (op == '*') print_num(a * b);
    else if (op == '/') {
        if (b == 0) print_color("Errore: Divisione per zero!", COLOR_RED, COLOR_BLACK);
        else print_num(a / b);
    }
    
    print_color("\n\n", COLOR_WHITE, COLOR_BLACK);
}

void execute_command() {
    command_buffer[buffer_idx] = '\0';
    print_color("\n", COLOR_WHITE, COLOR_BLACK);

    if (buffer_idx == 0) {
        print_prompt();
        return;
    }

    if (str_equals(command_buffer, "fetch")) {
        app_fetch();
    } else if (str_equals(command_buffer, "matrix")) {
        app_matrix();
    } else if (str_starts_with(command_buffer, "calc")) {
        app_calc(command_buffer);
    } else if (str_equals(command_buffer, "clear")) {
        clear_screen();
    } else if (str_equals(command_buffer, "help")) {
        print_color("=== COMANDI DISPONIBILI MA13 ===\n", COLOR_YELLOW, COLOR_BLACK);
        print_color("  fetch   - Mostra il Leone ASCII e specifiche IT\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
        print_color("  matrix  - Avvia l'animazione Matrix Rain\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
        print_color("  calc    - Calcolatrice (es. calc 10+20 o calc 10 / 2)\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
        print_color("  clear   - Pulisce lo schermo\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
        print_color("  help    - Mostra questa guida\n\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    } else {
        print_color("MA13: comando non riconosciuto. Scrivi 'help'\n\n", COLOR_LIGHT_RED, COLOR_BLACK);
    }

    buffer_idx = 0;
    print_prompt();
}

void kernel_main() {
    enable_cursor(14, 15); // Attiva cursore lampeggiante in basso al carattere
    clear_screen();

    print_color("===================================================\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    print_color("  __  __          __ ____  \n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color(" |  \\/  |        /_ |___ \\ \n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color(" | \\  / |   /\\    | | __) |\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color(" | |\\/| |  /  \\   | ||__ < \n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color(" | |  | | / /\\ \\  | |___) |\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color(" |_|  |_|/_/  \\_\\ |_|____/  KERNEL OS v0.3 (IT Layout)\n", COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_color("===================================================\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    print_color(" Benvenuto Madya! Tastiera IT abilitata con Cursore.\n\n", COLOR_WHITE, COLOR_BLACK);

    print_prompt();

    while (1) {
        ticks++;
        if (inb(KEYBOARD_STATUS) & 0x01) {
            unsigned char scancode = inb(KEYBOARD_PORT);

            // Gestione pressione/rilascio SHIFT (Left Shift 0x2A, Right Shift 0x36)
            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressed = 1;
            } else if (scancode == 0xAA || scancode == 0xB6) {
                shift_pressed = 0;
            } else if (!(scancode & 0x80)) { // Tasto premuto
                char char_pressed = shift_pressed ? scancode_map_it_shift[scancode] : scancode_map_it[scancode];
                if (char_pressed != 0) {
                    if (char_pressed == '\n') {
                        execute_command();
                    } else if (char_pressed == '\b') {
                        if (buffer_idx > 0) {
                            buffer_idx--;
                            put_char_color('\b', COLOR_LIGHT_GREEN, COLOR_BLACK);
                        }
                    } else {
                        if (buffer_idx < 127) {
                            command_buffer[buffer_idx++] = char_pressed;
                            put_char_color(char_pressed, COLOR_LIGHT_GREEN, COLOR_BLACK);
                        }
                    }
                }
            }
        }
    }
}
