import bluetooth as blu

class Conection_bluetooth():
    def __init__(self) -> None:
        pass
    def Conexiones(self) -> None:
        nearby_devices = blu.discover_devices(lookup_names=True)
        print("Found {} devices.".format(len(nearby_devices)))
        for addr, name in nearby_devices:
            print("  {} - {}".format(addr, name))


if __name__ == '__main__':
    Conect = Conection_bluetooth()
    Conect.Conexiones()