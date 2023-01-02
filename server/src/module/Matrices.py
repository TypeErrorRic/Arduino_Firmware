
class Matrices:

    size : int = 5

    def __init__(self) -> None:
        self.x: list = [0.0 for i in range(self.size)]
        self.y: list = [0.0 for i in range(self.size)]
        self.matrices = []
        self.contador : int = self.size - 1
        self.tamaño_matriz: int = 3
    
    def __del__(self) -> None:
        print("Finalizado")

    def Update(self, value_x: float, value_y: float) -> bool:
        self.contador += 1
        if (contador := self.contador % (self.size)) != (self.size -1):
            self.contador = contador
            self.y[contador] = value_y
            self.x[contador] = value_x
            return True
        else:
            return False
    
    def Calcular_matriz(self) -> list:
        matrices = [[0.0]*(self.tamaño_matriz + 1) for i in range(self.tamaño_matriz)]
        sum_x, sum_x2, sum_x3, sum_x4, sum_y, sum_yx, sum_yx2 = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        matrices[0][0] = self.size
        for i in range(self.size):
            sum_x += self.x[i]
            sum_x2 += self.x[i] * self.x[i]
            sum_x3 += self.x[i] * self.x[i] * self.x[i]
            sum_x4 += self.x[i] * self.x[i] * self.x[i] * self.x[i]
            sum_y += self.y[i]
            sum_yx += self.x[i] * self.y[i]
            sum_yx2 += self.x[i] * self.x[i] * self.y[i]
        matrices[0][1] = sum_x
        matrices[0][2] = sum_x2
        matrices[0][3] = sum_y
        matrices[1][0] = sum_x
        matrices[1][1] = sum_x2
        matrices[1][2] = sum_x3
        matrices[1][3] = sum_yx
        matrices[2][0] = sum_x2
        matrices[2][1] = sum_x3
        matrices[2][2] = sum_x4
        matrices[2][3] = sum_yx2
        return matrices
    
    def get_Matriz(self) -> None:
        print("Valores: ")
        for i in range(self.size):
            for j in range(2):
                if j == 0:
                    print(f"{self.x[i]} \t", end=" ")
                else:
                    print(self.y[i])
        print("Matriz obtenida")

    
        