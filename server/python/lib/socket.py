import socket as socket
import bluetooth

class socket():

    _mylist = {}

    #Permite comprobar si ya se han realizado la busquedad de puertos
    def __new__(client) -> dict:
        if(client._mylist):
             return client
        else:
            nearby_devices = bluetooth.discover_devices(lookup_names=True)
            for addr, name in nearby_devices:
                client._mylist.setdefault(addr, name)
            return client
        
    def __init__(self) -> None:
        self.sock = socket.socket(
            socket.AF_BLUETOOTH,
            socket.SOCK_STREAM,
            socket.BITPROTO_RFCOMM
        )

    def __call__(self) -> None:
        nearby_devices = bluetooth.discover_devices(lookup_names=True)
        print("Found {} devices.".format(len(nearby_devices)))
        for addr, name in nearby_devices:
            print("  {} - {}".format(addr, name))
            self._mylist.setdefault(addr, name)

    async def conecion():
        pass
        
    
