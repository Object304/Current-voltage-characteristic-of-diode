import serial
import struct
import os

def main():
    ser = serial.Serial('COM3', baudrate=9600, timeout=1)

    output_path = os.path.join(os.path.dirname(__file__), "output.txt")

    print("Ожидание пакета...")

    while True:
        byte = ser.read(1)
        if not byte:
            continue
        if byte[0] != 0xAA:
            continue

        packet = ser.read(9)
        if len(packet) != 9:
            print("Ошибка: неполный пакет")
            continue

        data_bytes = byte + packet[:-1]
        crc_received = packet[-1]

        crc_calc = 0
        for b in data_bytes:
            crc_calc ^= b

        if crc_calc != crc_received:
            print(f"Ошибка CRC (ожидалось {crc_calc:#02x}, получено {crc_received:#02x})")
            continue

        x = struct.unpack('<f', packet[0:4])[0]
        y = struct.unpack('<f', packet[4:8])[0]

        print(f"Принято: x={x:.3f}, y={y:.3f}")

        with open(output_path, "a", encoding="utf-8") as f:
            f.write(f"{x:.6f}\t{y:.6f}\n")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nВыход из программы.")
