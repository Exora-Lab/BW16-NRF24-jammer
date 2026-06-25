This project generates a constant carrier signal using the nRF24L01 RF module, with
automatic channel hopping across the entire 2.4GHz spectrum (channels 0-125).
It's designed for:
- RF testing and analysis
- Spectrum monitoring
- RF interference testing
- Educational demonstrations of RF principles
- Wireless signal generation

⚠️ **DISCLAIMER**
This code is provided for educational and research purposes. Users are responsible
for complying with local laws, regulations, and radio-frequency requirements when
using wireless communication equipment.

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

## 🚀 Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/nRF24L01-Constant-Carrier-Hopper.git
