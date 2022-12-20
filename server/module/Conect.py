import bluetooth

class Conections():

    Adress = ""
    target_name = "TWS"

    def __init__(self) -> None:
        pass

    def __call__(self) -> None:
        nearby_devices = bluetooth.discover_devices(lookup_names=True)
        print("Found {} devices.".format(len(nearby_devices)))
        for addr, name in nearby_devices:
            print("  {} - {}".format(addr, name))
            if(name == self.target_name):
                self.search(addr)

    def search(self, adress: str) -> None:
        self.Adress = adress
        print("Dispositvo encontrado")




if __name__ == "__main__":
    sock = Conections()
    sock()

    
