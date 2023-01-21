from module.Regresion_cuadratica import Regresion_Cuadratica

from module.Regresion_lineal import Regresion_lineal

def realizar(tipo_regresion: str, list1: list, list2: list):
    valores: list = []
    elementos_reg : dict = {}
    if tipo_regresion == "lineal":
        regresion = Regresion_lineal(list1, list2)
        if (regresion.get_realizar()):
            regresion.Calcular_regresion()
           # print("{:.2f}x2 + {:.2f}x + {:.2f}".format(regresion.x2(),
            #    regresion.x1(), regresion.a()))
            valores.append(regresion.a())
            valores.append(regresion.x1())
            valores.append(regresion.x2())
            elementos_reg = regresion.elementos_reg
    elif tipo_regresion == "cuadratica":
        regresion2 = Regresion_Cuadratica(list1, list2)
        if (regresion2.get_realizar()):
            regresion2.Calcular_regresion()
            #print("{:.2f}x2 + {:.2f}x + {:.2f}".format(regresion2.x2(),
             #   regresion2.x1(), regresion2.a()))
            valores.append(regresion2.a())
            valores.append(regresion2.x1())
            valores.append(regresion2.x2())
            elementos_reg = regresion2.elementos_reg
    return valores, elementos_reg


if __name__ == '__main__':
    list1: list = [-2, 1, 4, 3, 0]
    list2: list = [5, 4, 1, -1, 0]
    for element in realizar("cuadratica",list1,list2):
        print(element)