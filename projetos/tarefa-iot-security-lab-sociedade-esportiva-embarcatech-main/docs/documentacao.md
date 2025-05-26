# 📄 Documentação Técnica — Segurança em IoT com BitDogLab

Este documento complementa o `README.md` com Estrutura do Projeto e Referências.

---

### ⚙️ Estrutura do Projeto

```bash
projetos/
└── tarefa-iot-security-lab-sociedade-esportiva-embarcatech-main/
    ├── README.md
    ├── .gitignore
    ├── CMakeLists.txt
    ├── LICENSE
    ├── pico_sdk_import.cmake
    ├── mqtt_subscribe.c
    ├── iot_security_lab.c
    ├── lwipopts.h
    ├── include/
    │   └── mqtt_comm.h
    │   └── mqtt_comm_subscribe.h
    │   └── wifi_conn.h
    │   └── xor_cipher.h
    ├── src/
    │   └── mqtt_comm.c
    │   └── mqtt_comm_subscribe.c
    │   └── wifi_conn.c
    │   └── xor_cipher.c
    ├── assets/
    │   └── teste.png
    │   └── teste.png
    └── docs/
        └── documentacao.md
```

---

### 🧩 Referências

https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html

https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf

https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
