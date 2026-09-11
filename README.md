# 🦁 MA13 Kernel OS

**MA13** è un sistema operativo bare-metal a 32-bit (x86 / i386) sviluppato totalmente da zero in **C** e **Assembly NASM** senza l'uso di librerie standard (`-ffreestanding`). Zero bloatware, solo codice puro interfacciato direttamente con l'hardware, bootloader Multiboot tramite GRUB e una shell interattiva retro-hacker in pieno stile Matrix.

---

## 🔥 Feature & Driver Hardware

* **Driver Grafico VGA (`0xB8000`):** Buffer video gestito a basso livello a 80x25 caratteri con colori custom (verde/giallo/ciano su sfondo nero).
* **Cursore Hardware VGA:** Controllo dinamico delle porte I/O `0x3D4` e `0x3D5` per il cursore lampeggiante in tempo reale.
* **Tastiera Italiana (IT-IT Native):** Mapping completo degli scancode PS/2 per il layout italiano con supporto per `SHIFT` e caratteri speciali (`!`, `"`, `/`, `=`, `?`, `:`, `_`).
* **Shell CLI & App Native:**
  * **`fetch`**: System specs (RAM, uptime, architettura) affiancate dall'ASCII Art del Leone.
  * **`matrix`**: Pioggia di codice Matrix animata a schermo intero.
  * **`calc`**: Calcolatrice bare-metal con parsing per addizioni, sottrazioni, moltiplicazioni e divisioni (`calc 10 + 20`).
  * **`clear` / `help`**: Controllo del buffer schermo e guida rapida.

---

## 🛠️ Requisiti di Sistema (Arch Linux)

Per compilare il sorgente e testarlo nella macchina virtuale QEMU su Arch Linux, installa la toolchain con:

```bash
sudo pacman -S base-devel gcc nasm grub xorriso mtools dosfstools qemu-desktop



Compilazione e Avvio
Bash

chmod +x build.sh
./build.sh

Autore & Social

    Developer: onlymexo (Madyan)

    Architecture: x86 Protected Mode (32-bit)

🌐 I MIEI SOCIAL:

    🎵 TikTok: @ildowndizona

    📸 Instagram: @utenteritardato104

    🔴 YouTube: @mady__tech
