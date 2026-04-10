import csv
import serial
from datetime import datetime

PORT = "COM9"
BAUD = 115200
OUTPUT_FILE = f"gyro_data_{datetime.now().strftime('%Y-%m-%d_%H-%M-%S')}.csv"

def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)

    with open(OUTPUT_FILE, "w", newline="") as f:
        writer = csv.writer(f)

        # Wait for header
        while True:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if line:
                print(line)
                if line.startswith("time_ms,"):
                    writer.writerow(line.split(","))
                    break

        print("Recording data... Press Ctrl+C to stop.")

        try:
            while True:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line:
                    continue

                parts = line.split(",")
                if len(parts) != 10:
                    continue

                writer.writerow(parts)
                f.flush()
                print(parts)

        except KeyboardInterrupt:
            print("\nStopped recording.")

        finally:
            ser.close()

if __name__ == "__main__":
    main()