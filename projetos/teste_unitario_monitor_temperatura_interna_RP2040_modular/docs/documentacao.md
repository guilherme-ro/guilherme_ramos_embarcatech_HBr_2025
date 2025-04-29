# 📄 Documentação Técnica — Teste Unitário com a biblioteca de testes Unity

Este documento complementa o `README.md` com Estrutura do Projeto e Referências.

---

### ⚙️ Estrutura do Projeto

```bash
projetos/
└── teste_unitario_monitor_temperatura_interna_RP2040_modular/
    ├── README.md
    ├── src/
    │   └── CMakeLists.txt
    │   └── pico_sdk_import.cmake
    │   └── app/
    │       └── main.c
    │   └── drivers/
    │       └── ssd1306.c
    │   └── hal/
    │       └── hal_display.c
    │       └── hal_temp-sensor.c
    │   └── include/
    │       └── font.h
    │       └── hal_display.h
    │       └── hal_temp_sensor.h
    │       └── ssd1306.h
    │   └── tests/
    │       └── test_adc_to_celsius.c
    ├── assets/
    │   └── temperatura-interna-RP2040.jpg
    │   └── teste_unitario.png
    └── docs/
        └── documentacao.md
```

---

### 🧩 Referências

https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html

https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf

https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
