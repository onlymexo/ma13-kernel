
# 🦁 MA13 Kernel OS

[![Kernel Version](https://img.shields.io/badge/Kernel-v0.3--IT--Layout-brightgreen.svg)](#)
[![Architecture](https://img.shields.io/badge/Arch-x86%20%2F%20i386-blue.svg)](#)
[![Language](https://img.shields.io/badge/Language-C%20%2F%20Assembly-purple.svg)](#)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](#)

**MA13** è un sistema operativo bare-metal a 32-bit (x86 / i386) sviluppato totalmente da zero in **C** e **Assembly NASM** senza l'uso di librerie standard (`-ffreestanding`). Zero bloatware, solo codice puro interfacciato direttamente con l'hardware, bootloader Multiboot tramite GRUB e una shell interattiva retro-hacker in pieno stile Matrix.

---

## 🔥 Feature & Driver Hardware

- **Driver Grafico VGA (`0xB8000`):** Buffer video gestito a basso livello a 80x25 caratteri con colori custom (verde/giallo/ciano su sfondo nero).
- **Cursore Hardware VGA:** Controllo dinamico delle porte I/O `0x3D4` e `0x3D5` per il cursore lampeggiante in tempo reale.
- **Tastiera Italiana (IT-IT Native):** Mapping completo degli scancode PS/2 per il layout italiano con supporto per `SHIFT` e caratteri speciali (`!`, `"`, `/`, `=`, `?`, `:`, `_`).
- **Shell CLI & App Native:**
  - `fetch`: System specs (RAM, uptime, architettura) affiancate dall'ASCII Art del Leone.
  - `matrix`: Pioggia di codice Matrix animata a schermo intero.
  - `calc`: Calcolatrice bare-metal con parsing per addizioni, sottrazioni, moltiplicazioni e divisioni (`calc 10 + 20`).
  - `clear` / `help`: Controllo del buffer schermo e guida rapida.

---

## 🛠️ Requisiti di Sistema (Arch Linux)

Per compilare il sorgente e testarlo nella macchina virtuale QEMU su Arch Linux, installa la toolchain con:


sudo pacman -S base-devel gcc nasm grub xorriso mtools dosfstools qemu-desktop

## Compilazione e Avvio

chmod +x build.sh
./build.sh


## comandi e uso

fetch  	Mostra le info di sistema e il logo del Leone in ASCII Art
matrix  	Attiva l'animazione Matrix Rain
calc <expr>  	Calcolatrice bare-metal (es. calc 10+20, calc 50 / 2)
clear	  Pulisce lo schermo mantenendo il cursore hardware
help   	Mostra la lista dei comandi disponibili

## AUTORE

Developer: onlymexo (Madyan)

Architecture: x86 Protected Mode (32-bit)

## I MIEI SOCIAL
tiktok:https://www.tiktok.com/@ildowndizona
instagram:https://www.instagram.com/utenteritardato104
youtube:https://www.youtube.com/@mady__tech
