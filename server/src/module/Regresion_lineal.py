class Matrices:

    size: int = 5

    def __init__(self) -> None:
        self.x: list = [0.0 for i in range(self.size)]
        self.y: list = [0.0 for i in range(self.size)]
        self.matrices = []
        self.contador: int = self.size - 1
        self.tamaño_matriz: int = 3

    def __del__(self) -> None:
        print("Finalizado")

    def Update(self, value_x: float, value_y: float) -> bool:
        self.contador += 1
        if (contador := self.contador % (self.size)) != (self.size):
            self.contador = contador
            self.y[contador] = value_y
            self.x[contador] = value_x
            return True
        else:
            return False

    def Calcular_matriz(self) -> list:
        matrices = [[0.0]*(self.tamaño_matriz + 1)
                    for i in range(self.tamaño_matriz)]
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

    def get_Matriz(self) -> dict:
        values: dict = {
            self.x[0]: self.y[0],
            self.x[1]: self.y[1],
            self.x[2]: self.y[2],
            self.x[3]: self.y[3],
            self.x[4]: self.y[4],
        }
        return values

class Regresion_lineal(Matrices):

    Realizar: bool = True
    elementos_reg : dict = {}
    
    def __init__(self, list_x: list, list_y: list):
        super().__init__()
        self.__a: float = 0
        self.__x1: float = 0
        self.__x2: float = 0
        if len(list_x) != self.size and len(list_y) != self.size:
            self.Realizar = False
        for i in range(self.size):
            if super().Update(list_x[i], list_y[i]):
                continue
            else:
                break
        self.elementos_reg = super().get_Matriz()
    
    def __Calcular_matriz(self) -> list:
        regresion: list = []
        if self.Realizar is True:
            sum_x, sum_x2, sum_y, sum_yx = 0.0, 0.0, 0.0, 0.0
            for i in range(self.size):
                sum_yx += self.x[i] * self.y[i]
                sum_y += self.y[i]
                sum_x += self.x[i]
                sum_x2 += self.x[i] * self.x[i]
            regresion.append(((self.size * sum_yx) - ( sum_y * sum_x)) / ((self.size*sum_x2) - (sum_x*sum_x)))
            regresion.append(((sum_y * sum_x2) - (sum_yx * sum_x)) / ((self.size*sum_x2) - (sum_x*sum_x)))
        return regresion

    def Calcular_regresion(self) -> None:
       valores: list = self.__Calcular_matriz()
       self.__x1 = valores[0]
       self.__a = valores[1]

    def a(self) -> float:
        return self.__a

    def x1(self) -> float:
        return self.__x1

    def x2(self) -> float:
        return self.__x2
    
    def reset(self) -> None:
        self.Realizar = True
    
    def get_realizar(self) -> bool:
        return self.Realizar
    
if __name__ == "__main__":
    list1: list = [-2, 1, 4, 3, 0]
    list2: list = [5, 4, 1, -1, 0]
    regresion = Regresion_lineal(list1, list2)
    if (regresion.get_realizar()):
        regresion.Calcular_regresion()
        print("{:.2f}x2 + {:.2f}x + {:.2f}".format(regresion.x2(),regresion.x1(), regresion.a()))
