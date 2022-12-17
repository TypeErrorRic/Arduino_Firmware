from lib.blutooth import conectiones as cont

#Crear librerias:

Conections = cont('COM4')

def main():
    Conections.loop()

if __name__ == "__main__":
    main()
