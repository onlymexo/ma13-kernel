cat << 'EOF' > README.md
# 🦁 MA13 Kernel OS

[![Kernel Version](https://img.shields.io/badge/Kernel-v0.3--IT--Layout-brightgreen.svg)](#)
[![Architecture](https://img.shields.io/badge/Arch-x86%20%2F%20i386-blue.svg)](#)
[![Language](https://img.shields.io/badge/Language-C%20%2F%20Assembly-purple.svg)](#)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](#)

**MA13** è un sistema operativo bare-metal a 32-bit (x86 / i386) scritto totalmente da zero in **C** e **Assembly NASM** senza l'uso di librerie standard (`-ffreestanding`). Il sistema si avvia tramite GRUB (compatibile con specifica Multiboot) e carica un'interfaccia a riga di comando (CLI) in stile retro-hacker con palette verde Matrix.

---

## ✨ Caratteristiche Tecniche
- **Driver Grafico VGA (0xB8000):** Gestione diretta della memoria video per stampare testo a 80x25 con colori personalizzati.
- **Cursore Hardware VGA:** Interfacciamento diretto con le porte I/O `0x3D4` e `0x3D5` per gestire il cursore lampeggiante in tempo reale.
- **Layout Tastiera IT-IT Completo:** Driver PS/2 con mappatura nativa dei tasti italiani e gestione del tasto `SHIFT` per la stampa dei caratteri speciali (`!`, `"`, `/`, `=`, `?`, `:`, `_`).
- **Shell Interattiva & App Native:**
  - `fetch`: Mostra le info di sistema (Uptime, RAM allocata, architettura) affiancate dall'ASCII Art del Leone.
  - `matrix`: Effetto pioggia di codice verde stile Matrix a pieno schermo.
  - `calc`: Calcolatrice bare-metal con parser avanzato per addizioni, sottrazioni, moltiplicazioni e divisioni (es. `calc 10 + 20`).
  - `clear` / `help`: Gestione del buffer video e guida comandi integrata.
 
- ## 🛠️ Guida all'Installazione e Requisiti (Arch Linux)

- ## Clona il Repository
- git clone [https://github.com/onlymexo/ma13-kernel.git](https://github.com/onlymexo/ma13-kernel.git)
cd ma13-kernel

## Struttura del Progetto

ma13-kernel/
├── src/
│   ├── boot.asm      # Entry point assembly per Multiboot
│   ├── kernel.c      # Logica kernel, driver VGA, tastiera & CLI
│   └── linker.ld     # Linker script x86
├── iso/              # Struttura cartelle per la generazione ISO
├── build.sh          # Script di compilazione ed esecuzione
└── README.md         # Documentazione ufficiale

## Compilazione e Avvio

chmod +x build.sh
./build.sh

## Comandi della Shell

fetch	  Mostra il logo ASCII del Leone e le informazioni di sistema.
matrix	Avvia la pioggia di codice Matrix verde.
calc <expr>	   calcolatrice integrata (es. calc 10+20, calc 50 / 2).
clear	   Pulisce lo schermo mantenendo il cursore attivo.
help	   Mostra la lista dei comandi disponibili.

## autore

    Developer: onlymexo (Madyan)

    Architettura: x86 Protected Mode (32-bit)
## i miei social 
tiktok:https://www.tiktok.com/@ildowndizona
youtube:https://www.youtube.com/@mady__tech
instagram:https://www.instagram.com/utenteritardato104/
