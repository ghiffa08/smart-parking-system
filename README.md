# 🅿️ Smart Parking System

Sistem parkir otomatis berbasis IoT menggunakan ESP32 dengan deteksi kendaraan dan kontrol portal otomatis.

## 📋 Deskripsi

Smart Parking System adalah proyek IoT yang dirancang untuk mengotomatisasi sistem parkir dengan menggunakan sensor ultrasonik untuk deteksi kendaraan dan servo motor untuk mengontrol portal masuk. Sistem ini dilengkapi dengan LCD display untuk menampilkan status real-time dan indikator LED serta buzzer untuk feedback kepada pengguna.

## ✨ Fitur Utama

- 🚗 **Deteksi Kendaraan Otomatis** - Menggunakan sensor ultrasonik HC-SR04
- 🚪 **Portal Otomatis** - Kontrol servo motor untuk membuka/menutup portal
- 📺 **Display Real-time** - LCD 16x2 I2C menampilkan jarak dan status portal
- 💡 **Indikator Visual** - LED hijau (portal terbuka) dan merah (portal tertutup)
- 🔊 **Notifikasi Audio** - Buzzer dengan pola beep berbeda untuk setiap aksi
- ⏰ **Auto-close Timer** - Portal menutup otomatis setelah durasi tertentu
- 🛡️ **Error Handling** - Timeout protection dan validasi sensor

## 🔧 Komponen Hardware

| Komponen | Jumlah | Deskripsi |
|----------|--------|-----------|
| ESP32 | 1 | Mikrokontroler utama |
| Sensor Ultrasonik HC-SR04 | 1 | Deteksi jarak kendaraan |
| Servo Motor SG90 | 1 | Kontrol portal |
| LCD 16x2 I2C | 1 | Display informasi |
| LED | 2 | Indikator status (hijau/merah) |
| Buzzer | 1 | Notifikasi audio |
| Resistor 220Ω | 2 | Untuk LED |
| Breadboard/PCB | 1 | Tempat rangkaian |
| Kabel Jumper | Secukupnya | Koneksi antar komponen |

## 📐 Diagram Koneksi

```
ESP32          HC-SR04         Servo Motor       LCD I2C
-----          -------         -----------       -------
GPIO 5    -->  Trig            
GPIO 19   -->  Echo            
GPIO 18   -->                  Signal Pin
GPIO 21   -->                                    SDA
GPIO 22   -->                                    SCL
GPIO 13   -->  LED Hijau
GPIO 12   -->  LED Merah
GPIO 2    -->  Buzzer
3.3V      -->  VCC             VCC (Red)         VCC
GND       -->  GND             GND (Brown)       GND
```

## 📥 Instalasi

### Prerequisites
- [PlatformIO IDE](https://platformio.org/) atau [Arduino IDE](https://www.arduino.cc/en/software)
- Library yang diperlukan (otomatis diinstall oleh PlatformIO)

### Clone Repository
```bash
git clone https://github.com/username/smart-parking.git
cd smart-parking
```

### Setup PlatformIO
```bash
# Install dependencies
pio lib install

# Build project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

### Manual Library Installation (Arduino IDE)
Jika menggunakan Arduino IDE, install library berikut:
- `ESP32Servo`
- `LiquidCrystal_I2C`

## ⚙️ Konfigurasi

Anda dapat menyesuaikan parameter sistem di file `main.cpp`:

```cpp
// Konstanta yang dapat disesuaikan
const uint8_t DISTANCE_THRESHOLD = 10;     // Jarak deteksi (cm)
const uint8_t SERVO_CLOSE_ANGLE = 0;       // Sudut servo tertutup
const uint8_t SERVO_OPEN_ANGLE = 90;       // Sudut servo terbuka
const uint16_t GATE_OPEN_DURATION = 3000;  // Durasi terbuka (ms)
const uint16_t BUZZER_BEEP_DURATION = 200; // Durasi buzzer (ms)
```

## 🚀 Cara Penggunaan

1. **Power ON** - Nyalakan sistem, LCD akan menampilkan "Smart Parking System Ready"
2. **Auto Detection** - Sistem otomatis mendeteksi kendaraan yang mendekati
3. **Portal Opens** - Ketika kendaraan terdeteksi (≤10cm), portal terbuka otomatis
4. **Visual/Audio Feedback** - LED hijau menyala dan buzzer berbunyi 1x
5. **Auto Close** - Portal menutup otomatis setelah 3 detik tidak ada kendaraan
6. **Status Display** - LCD menampilkan jarak real-time dan status portal

## 📊 Status Indikator

| Kondisi | LED Hijau | LED Merah | Buzzer | LCD Display |
|---------|-----------|-----------|--------|-------------|
| Portal Tertutup | OFF | ON | - | "Portal: TERTUTUP" |
| Portal Terbuka | ON | OFF | 1 beep | "Portal: TERBUKA" |
| Portal Menutup | OFF | ON | 2 beep | "Portal: TERTUTUP" |
| Error Sensor | - | - | - | "Jarak: -- cm" |

## 🐛 Troubleshooting

### Sensor Ultrasonik Tidak Bekerja
- Periksa koneksi Trig (GPIO 5) dan Echo (GPIO 19)
- Pastikan sensor mendapat power 5V yang stabil
- Cek jarak antara sensor dan objek (2cm - 400cm)

### Servo Tidak Bergerak
- Periksa koneksi signal servo ke GPIO 18
- Pastikan servo mendapat power yang cukup
- Cek apakah servo tidak terhalang secara fisik

### LCD Tidak Menyala
- Periksa koneksi I2C (SDA: GPIO 21, SCL: GPIO 22)
- Cek alamat I2C LCD (default: 0x27)
- Pastikan power LCD tersambung dengan benar

### LED/Buzzer Tidak Berfungsi
- Periksa koneksi pin GPIO
- Cek resistor pada rangkaian LED
- Pastikan tidak ada short circuit

## 🔧 Pengembangan Lanjutan

Ide untuk pengembangan selanjutnya:
- 📱 **Web Interface** - Monitoring via WiFi
- 📊 **Database Logging** - Menyimpan data kendaraan
- 🎥 **Camera Integration** - CCTV dan plate recognition
- 📱 **Mobile App** - Aplikasi smartphone
- 🌐 **IoT Cloud** - Integrasi dengan platform cloud
- 💳 **Payment System** - Sistem pembayaran digital

## 📄 Lisensi

Proyek ini dilisensikan di bawah [MIT License](LICENSE) - lihat file LICENSE untuk detail.

## 👨‍💻 Kontributor

- **[Your Name]** - *Initial work* - [GitHub Profile](https://github.com/yourusername)

## 🙏 Acknowledgments

- Terima kasih kepada komunitas Arduino dan ESP32
- Inspirasi dari berbagai proyek IoT smart parking
- Library developers yang telah menyediakan tools yang excellent

## 📞 Support

Jika Anda memiliki pertanyaan atau menemukan bug, silakan:
- Buat [Issue](https://github.com/ghiffa08/smart-parking/issues) di GitHub
- Contact via email: haikalcode08@gmail.com
- Join diskusi di [Discussions](https://github.com/ghiffa08/smart-parking/discussions)

---

⭐ **Jangan lupa untuk memberikan star jika proyek ini membantu Anda!**

**Made with ❤️ for IoT Community**
