from Matrices import Matrices


class Regresion_lineal(Matrices):

    Realizar: bool = True
    
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
        super().get_Matriz() #activar en caso de ver los datos ingresados

    
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
