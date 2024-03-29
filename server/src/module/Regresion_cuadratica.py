#Matrices:

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

#Regresion cuadratica:

class Regresion_Cuadratica(Matrices):
    
    Realizar : bool = True
    Num_test: int = 0
    elementos_reg: dict = {}

    def __init__(self, list_x : list, list_y: list) -> None:
        super().__init__()
        self.__a : float = 0 
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
        self.Matriz: list = super().Calcular_matriz()
    
    def __Multiplicacion_const(self, num: int, fila: int) -> None:
        for i in range(len(self.Matriz[fila])):
            self.Matriz[fila][i] *= num

    def __Segunda_operacion(self, num:int, fila: int) -> bool:
        aux: float = self.Matriz[fila][num]
        if(aux != 0):
           self.Matriz[fila] = [self.Matriz[fila][j] - (self.Matriz[num][j]) * aux for j in range(len(self.Matriz[fila]))]
        else:
            return False
        return True
    
    def __Intercambiar(self, fila : int, fila2 : int) -> None:
        aux: list = [0.0 , 0.0, 0.0, 0.0]
        for i in range(len(aux)):
            aux[i] = self.Matriz[fila][i]
            self.Matriz[fila][i] = self.Matriz[fila2][i]
            self.Matriz[fila2][i] = aux[i]
    
    def __Gauss_Jordan(self) -> bool:
        #self.test()
        if self.Realizar is True:
            for i in range(self.tamaño_matriz):
                if self.Matriz[i][0] == 1:
                    self.__Intercambiar(0, i)
                    break
                elif self.Matriz[i][0] == 0:
                    if self.Matriz[1][0] != 0:
                        self.__Intercambiar(1, i)
                        break
                elif self.Matriz[i][0] == 0 and self.Matriz[i][1] == 0:
                    if self.Matriz[2][0] != 0 and self.Matriz[2][1] != 0:
                        self.__Intercambiar(2, i)
                        break
            if self.Matriz[0][0] != 1:
                self.__Multiplicacion_const((1 / self.Matriz[0][0]),0)
            #self.test()
            for i in range(1,self.tamaño_matriz): #Primer argumento es la fila del pivote. El segundo es donde se aplica
                self.__Segunda_operacion(0, i)
            #self.test()
            if self.Matriz[1][1] != 1 and self.Matriz[1][1] != 0:
                self.__Multiplicacion_const((1 / self.Matriz[1][1]), 1)
            elif self.Matriz[1][1] == 0 and self.Matriz[2][1] != 0:
                self.__Intercambiar(1,2)
            if not(self.__Segunda_operacion(1,2)):
                return False
            if self.Matriz[2][2] != 1 and self.Matriz[2][2] != 0:
                self.__Multiplicacion_const((1 / self.Matriz[2][2]), 2)
            elif self.Matriz[2][2] == 0:
                return False
            #self.test()
            if not(self.__Segunda_operacion(1, 0)) and self.Matriz[0][2] == 0 and self.Matriz[1][2] == 0:
                return True
            self.__Segunda_operacion(2, 0)
            self.__Segunda_operacion(2, 1)
            #self.test()
            if self.Matriz[0][0] == 1 and self.Matriz[1][1] == 1 and self.Matriz[2][2] == 1:
                return True
            return False
        else:
            return False
        
    def a(self) -> float:
        self.__a = self.Matriz[0][3]
        return self.__a
    

    def x1(self) -> float:
        self.__x1 = self.Matriz[1][3]
        return self.__x1

    def x2(self) -> float:
        self.__x2 = self.Matriz[2][3]
        return self.__x2

    def Calcular_regresion(self) -> None:
        if(self.__Gauss_Jordan()):
            self.Realizar = True
            self.Num_test = 0
        else:
            self.Realizar = False
            self.Num_test = 0

    def test(self) -> None:
        print(f"Numero de prueba: {self.Num_test}")
        for i in range(self.tamaño_matriz):
            for j in range(self.tamaño_matriz+1):
                print("{:.2f} \t".format(self.Matriz[i][j]), end=" ")
            print(" ")
        self.Num_test += 1

    def reset(self) -> None:
        self.Realizar = True
    
    def get_realizar(self) -> bool:
        return self.Realizar
    

if __name__ == '__main__':
    list1 : list = [-2,1,4,3,0]
    list2 : list = [5,4,1,-1,0]
    regresion = Regresion_Cuadratica(list1, list2)
    if(regresion.get_realizar()):
        regresion.Calcular_regresion()
        print("{:.2f}x2 + {:.2f}x + {:.2f}".format(regresion.x2(), regresion.x1(), regresion.a()))
