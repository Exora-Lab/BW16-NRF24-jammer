// EXORA  //
//TikTok: @exoraofficial2 //

#include <SPI.h>

// Pin definitions for BW16
#define CE_PIN   6
#define CSN_PIN  5
//CSN	PB2
//CE	PB3
//MOSI	PA12
//MISO	PA13
//SCK	PA14

// nRF24L01 Commands
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define RF24_NOP      0xFF
#define ACTIVATE      0x50

// Registers
#define NRF_CONFIG    0x00
#define EN_AA         0x01
#define EN_RXADDR     0x02
#define SETUP_AW      0x03
#define SETUP_RETR    0x04
#define RF_CH         0x05
#define RF_SETUP      0x06
#define NRF_STATUS    0x07
#define TX_ADDR       0x10

// Bit mnemonics
#define PWR_UP        1
#define PRIM_RX       0
#define CONT_WAVE     7
#define PLL_LOCK      4
#define RF_DR         3
#define RF_PWR_LOW    1
#define RF_PWR_HIGH   2

bool carrierStarted = false;
unsigned long lastHeartbeat = 0;

void setup() {
Serial.begin(115200);
Serial.println(F("\n███████  ██   ██   ██████   ██████    █████"));
  Serial.println(F("██        ██ ██   ██    ██  ██   ██  ██   ██"));
  Serial.println(F("█████      ███    ██    ██  ██████   ███████"));
  Serial.println(F("██        ██ ██   ██    ██  ██ ██    ██   ██"));
  Serial.println(F("███████  ██   ██   ██████   ██  ██   ██   ██\n"));
  
  // Initialize control pins
  pinMode(CE_PIN, OUTPUT);
  pinMode(CSN_PIN, OUTPUT);
  digitalWrite(CE_PIN, LOW);
  digitalWrite(CSN_PIN, HIGH);
  
  // Initialize SPI for BW16
  SPI.begin();
  

  
  delay(100);
  
  if (!initRadio()) {
    Serial.println(F("ERROR: Radio initialization failed!"));
    Serial.println(F("Check power supply (3.3V only!)"));
    while (1) {
      delay(1000);
      Serial.print(F("."));
    }
  }
  
  Serial.println(F("\n✓ Radio initialized successfully"));
  Serial.println(F("✓ Starting constant carrier with channel hopping\n"));
  
  // Start constant carrier on channel 45
  startConstCarrier(45);
}

uint8_t spi_transfer(uint8_t data) {
  digitalWrite(CSN_PIN, LOW);
  uint8_t result = SPI.transfer(data);
  digitalWrite(CSN_PIN, HIGH);
  return result;
}

void write_register(uint8_t reg, uint8_t value) {
  digitalWrite(CSN_PIN, LOW);
  SPI.transfer(W_REGISTER | (reg & 0x1F));
  SPI.transfer(value);
  digitalWrite(CSN_PIN, HIGH);
}

uint8_t read_register(uint8_t reg) {
  digitalWrite(CSN_PIN, LOW);
  SPI.transfer(R_REGISTER | (reg & 0x1F));
  uint8_t result = SPI.transfer(0xFF);
  digitalWrite(CSN_PIN, HIGH);
  return result;
}

void flush_tx() {
  digitalWrite(CSN_PIN, LOW);
  SPI.transfer(FLUSH_TX);
  digitalWrite(CSN_PIN, HIGH);
}

void set_channel(uint8_t channel) {
  write_register(RF_CH, channel);
}

uint8_t get_channel() {
  return read_register(RF_CH);
}

void set_power_up(bool enable) {
  uint8_t config = read_register(NRF_CONFIG);
  if (enable) {
    config |= (1 << PWR_UP);
  } else {
    config &= ~(1 << PWR_UP);
  }
  write_register(NRF_CONFIG, config);
  delayMicroseconds(150);  // Wait for power up/down
}

bool initRadio() {
  // Perform a soft reset by powering down and up
  set_power_up(false);
  delay(10);
  set_power_up(true);
  delay(5);
  
  // Configure for constant carrier mode
  write_register(EN_AA, 0x00);      // Disable Auto-Acknowledgment
  write_register(EN_RXADDR, 0x00);  // Disable all RX pipes
  write_register(SETUP_RETR, 0x00); // No retransmissions
  write_register(SETUP_AW, 0x03);   // 5-byte addresses
  
  // Configure RF: 2Mbps, Maximum Power
  // RF_SETUP bits: [7:CONT_WAVE] [6:PLL_LOCK] [5:RF_DR] [4:RF_PWR_HIGH] [3:RF_PWR_LOW] [2:0] reserved
  uint8_t rf_setup = (1 << RF_DR) | (3 << RF_PWR_LOW);  // 2Mbps, Max power
  write_register(RF_SETUP, rf_setup);
  
  // Flush TX FIFO
  flush_tx();
  
  // Load dummy payload into TX FIFO (required for constant carrier)
  digitalWrite(CSN_PIN, LOW);
  SPI.transfer(W_TX_PAYLOAD);
  for (int i = 0; i < 32; i++) {
    SPI.transfer(0xFF);
  }
  digitalWrite(CSN_PIN, HIGH);
  
  // Verify radio is responding
  uint8_t config = read_register(NRF_CONFIG);
  if (config == 0x00 || config == 0xFF) {
    return false;  // No response from radio
  }
  
  return true;
}

void startConstCarrier(uint8_t channel) {
  // Set the channel
  set_channel(channel);
  
  // Enable constant carrier and PLL lock
  uint8_t rf_setup = read_register(RF_SETUP);
  rf_setup |= (1 << CONT_WAVE);  // Enable constant carrier
  rf_setup |= (1 << PLL_LOCK);   // Enable PLL lock
  write_register(RF_SETUP, rf_setup);
  
  // Set CE high to start transmission
  digitalWrite(CE_PIN, HIGH);
  
  carrierStarted = true;
  
  Serial.print(F("   Carrier active on channel "));
  Serial.print(channel);
  Serial.print(F(" (~"));
  Serial.print(2400 + channel);
  Serial.println(F(" MHz)"));
}

void stopConstCarrier() {
  digitalWrite(CE_PIN, LOW);
  
  // Disable constant carrier
  uint8_t rf_setup = read_register(RF_SETUP);
  rf_setup &= ~(1 << CONT_WAVE);
  rf_setup &= ~(1 << PLL_LOCK);
  write_register(RF_SETUP, rf_setup);
  
  carrierStarted = false;
  
  Serial.println(F("   Carrier stopped"));
}

void performHopping() {
  if (!carrierStarted) {
    startConstCarrier(45);
  }
  
  // Choose random channel from 0 to 125 (full 2.4GHz band)
  uint8_t newChannel = random(126);
  set_channel(newChannel);
  
  // Wait for PLL to lock (minimum 130µs)
  delayMicroseconds(random(10, 90));
}

void printStatus() {
  uint8_t config = read_register(NRF_CONFIG);
  uint8_t rf_setup = read_register(RF_SETUP);
  uint8_t channel = get_channel();
  uint8_t status = read_register(NRF_STATUS);
  
  Serial.println(F("\n--- Radio Status ---"));
  Serial.print(F("Channel: "));
  Serial.print(channel);
  Serial.print(F(" ("));
  Serial.print(2400 + channel);
  Serial.println(F(" MHz)"));
  
  Serial.print(F("Constant Carrier: "));
  Serial.println((rf_setup & (1 << CONT_WAVE)) ? "ON" : "OFF");
  
  Serial.print(F("PLL Lock: "));
  Serial.println((rf_setup & (1 << PLL_LOCK)) ? "LOCKED" : "UNLOCKED");
  
  Serial.print(F("Power: "));
  Serial.println((config & (1 << PWR_UP)) ? "ON" : "OFF");
  
  Serial.print(F("Mode: "));
  Serial.println((config & (1 << PRIM_RX)) ? "RX" : "TX");
  
  Serial.print(F("Status: 0x"));
  Serial.println(status, HEX);
  Serial.println(F("--------------------\n"));
}

void loop() {
  // Perform channel hopping
  performHopping();
  
  // Small delay to control hopping rate (adjust as needed)
  // Lower delay = faster hopping, higher delay = slower hopping
  delayMicroseconds(100);
  
  // Print heartbeat every 5 seconds
  if (millis() - lastHeartbeat > 5000) {
    Serial.print(F("."));
    lastHeartbeat = millis();
  }
  
  // Optional: Print full status every 30 seconds
  
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 30000) {
    printStatus();
    lastStatus = millis();
  }
  
}