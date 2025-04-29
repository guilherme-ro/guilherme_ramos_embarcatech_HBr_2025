# 📄 Documentação Técnica — Estruturação de código para arquitetura modular

Este documento complementa o `README.md` com Estrutura do Projeto e Referências.

---

### ⚙️ Estrutura do Projeto

```bash
projetos/
└── leitura-sinais-analogicos-joystick/
    ├── README.md
    ├── src/
    │   └── CMakeLists.txt
    │   └── pico_sdk_import.cmake
    │   └── app/
    │       └── main.c
    │   └── drivers/
    │       └── led_embutido.c
    │   └── hal/
    │       └── hal_led.c
    │   └── include/
    │       └── hal_led.h
    │       └── led_embutido.h
    │       └── lwipopts.h
    ├── assets/
    │   └── led_RP2040_blink.jpg
    └── docs/
        └── documentacao.md
```

---

### 🧩 Referências

https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html
https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf
https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
