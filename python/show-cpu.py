#!/usr/bin/python3
import psutil
from subprocess import check_output
import time
import Adafruit_CharLCD as LCD


if __name__ == "__main__":
    # Raspberry Pi pin setup
    lcd_rs = 25
    lcd_en = 24
    lcd_d4 = 23
    lcd_d5 = 17
    lcd_d6 = 18
    lcd_d7 = 22
    lcd_backlight = 2

    # Define LCD column and row size for 16x2 LCD.
    lcd_columns = 16
    lcd_rows = 2

    lcd = LCD.Adafruit_CharLCD(
            lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6,
            lcd_d7, lcd_columns, lcd_rows, lcd_backlight)

    cpuOld, memOld, tempOld = "", "", ""

    while True:
        cpu = "C:" + str(psutil.cpu_percent(interval=1)).split(".")[0]
        temp = "T:" + str(psutil.sensors_temperatures()["cpu_thermal"][0].current).split(".")[0]
        mem = "M:" + str(psutil.virtual_memory().percent).split(".")[0]
        ip = check_output(['hostname', '-I']).decode('utf-8').strip('\n')
        if cpu != cpuOld or mem != memOld or temp != tempOld:
            lcd.clear()

            lcd.home()
            lcd.message(ip)

            lcd.set_cursor(0, 1)
            lcd.message(cpu)

            lcd.set_cursor(6, 1)
            lcd.message(temp)

            lcd.set_cursor(11, 1)
            lcd.message(mem)

            time.sleep(2.0)

        cpuOld, memOld, tempOld = cpu, mem, temp
