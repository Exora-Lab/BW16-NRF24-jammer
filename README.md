**WARNING!!! JAMMING IS ILLEGAL------WARNING!!! JAMMING IS ILLEGAL**
_**EXORA_LAB**_

Here's my BW16 + NRF24 code, now open source. I spent months developing and testing it beacause there were
barely any useful resources online for this combination. fell free to use it, modify it, learn from it and improve it.
But please **give proper credit if you do**. thanks, and happy building

**EDUCATIONAL PURPOSE ONLY**
This project generates a constant carrier signal using the nRF24L01 RF module, with
automatic channel hopping across the entire 2.4GHz spectrum (channels 0-125).
It's designed for:
- RF testing and analysis
- Spectrum monitoring
- RF interference testing
- Educational demonstrations of RF principles
- Wireless signal generation

⚠️ **DISCLAIMER**
**_This code is provided for educational and research purposes. Users are responsible
for complying with local laws, regulations, and radio-frequency requirements when
using wireless communication equipment._**

**🎯 Features**
- ✅ Constant carrier generation with nRF24L01+
- ✅ Automatic channel hopping (0-125 range)
- ✅ Real-time status monitoring via Serial
- ✅ PLL lock detection
- ✅ Full 2Mbps, maximum power output
- ✅ Lightweight and efficient

**🔌 Hardware Requirements**
- BW16 (RTL8720DN) development board
- nRF24L01 RF module
- 3.3V power supply
- Jumper wires (female-female)
- USB cable for programming

**📡 Pin Connections**
| BW16 Pin | nRF24L01+ | Pin Description |
|----------|-----------|-----------------|
| PB2      | CSN       | Chip Select     |
| PB3      | CE        | Chip Enable     |
| PA12     | MOSI      | Master Out Slave In |
| PA13     | MISO      | Master In Slave Out |
| PA14     | SCK       | Serial Clock    |
| 3.3V     | VCC       | Power Supply    |
| GND      | GND       | Ground          |

<img width="1200" height="800" alt="NRF24L01-Pinout-NRF24L01-PA-LNA-" src="https://github.com/user-attachments/assets/11c24988-308b-4f10-bd75-d2ef8c422aff" />

<img width="2410" height="1393" alt="P2" src="https://github.com/user-attachments/assets/c5325861-ffa5-48e2-a373-23136b08174a" />

**If you like you can donate to GCASH Account**

<img width="1268" height="2398" alt="Project (20260625125854)" src="https://github.com/user-attachments/assets/4233ad9e-2618-4685-a20f-44dc8b602a34" />
