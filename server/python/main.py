import serial
import keyboard as k

def setup():
    try:
        with serial.Serial() as ser:
            ser.baudrate = 9600
            ser.port = "COM4"
            ser.oper()
            ser.write("Hello World")
    except TimeoutError:
        print("Error en la inicializacion del puerto")


def main():
    try:
        ser = serial.Serial('com4')
    except TimeoutError:
        print("Error en la inicializacion del puerto")
    finally:
        print("Done")


if __name__ == '__main__':
    main();