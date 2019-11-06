# Einsteint-problem
Solution of Einstein problem with complex clause using C++ (BuDDy lib).

<i>Variant - 12 (11 in table)</i>


## Iterpretation
Find a robber/burglar if you know the following:

1) Cook lives in the center

2)

3)

4)

5)

6)

7)

8)

9)

10)

11)

12)

13)

14)

15)

16)


Solution:
Jobs

|       |       |       |
| ----- | ----- | ----- |
|    Teacher   |    Driver   |   Cook    |
|   Buisnessman    |   Robber   |   Unemployed   |
|   Pilot    |   Salesman   |    Programmer  |

Watches:

|       |       |       |
| ----- | ----- | ----- |
|    GShock   |    Hublot   |   Supreme    |
|   No watches    |   Patek Philippe   |   Omega   |
|   Swiss    |   Apple watches   |    Rolex  |




## Statement 
- We have <b>N = 9 objects.</b> They are allocated as follows: </br>

|       |       |       |
| ----- | ----- | ----- |
|   1    |   2    |   3    |
|   4    |   6   |    8  |
|   5    |   7   |    9  |


- Neighbors in such a grid are:

|       |       |       |
| ----- | ----- | ----- |
|   *    |    -   |   -    |
|   -    |   0   |    -  |
|   *    |    -  |   -   |

|       |       |       |
| ----- | ----- | ----- |
|   -    |    -   |   0    |
|   -    |   *   |    -  |
|   -    |    -  |   -   |

|       |       |       |
| ----- | ----- | ----- |
|   -    |    -   |   -    |
|   -    |   -   |    -  |
|   0    |    -  |   -   |

_Here '0' - object,  '*' - neignbors, '-' - no relationships_

- Also we have <b>M = 4 properties.</b> which have N different values.
  1) Job: Programmer, Buisnessman, Salesman, Cook, Driver, Pilot, Robber, Teacher, Unemployed
  2) Orientation: Lesbian, Gay, Transgenger, Asexual, Intersex, Queer, Ally, Transsexual, Bisexual
  3) Watches: GShock, Apple watches, No watches, Rolex, Swiss, Patek Philippe, Hublot, Omega, Supreme 
  4) Time leaving to home: 8am, 10am, 11am, 2pm, 4pm, 7pm, 9pm, 11pm, not leaving

- And we have n1 = 3, n2 = 5, n3 = 4, n4 = 4 limitations.
  1) Concrete assignment of property to object
  2) Limitation that assign two properties
  3) Limitation of placement (right, left, down, up)
  4) Limitation of placement (near)
  5) No two different objects have same value of one property
  6) Object's value of property only from given set of values
  7) Sum of object's neighbors known properties < K, K = [0, N*M]


## Constraints
- Ограничение №1 _(3 ограничения)_
    - Свойство №1 у объекта №8 равно 6
    - Свойство №2 у объекта №1 равно 5
    - Свойство №3 у объекта №7 равно 7
    
 - Ограничение №2 _(5 ограничений)_
    - Объект, у которого свойство №4 равно 9 имеет свойство №1 равным 3
    - Объект, у которого свойство №2 равно 5 имеет свойство №4 равным 8
    - Объект, у которого свойство №4 равно 7 имеет свойство №3 равным 5
    - Объект, у которого свойство №2 равно 3 имеет свойство №4 равным 8
    - Объект, у которого свойство №3 равно 6 имеет свойство №3 равным 6
 
 - Ограничение №3 _(4 ограничения)_
    - Объект, у которого свойство №3 равно 5 находится слева сверху от объекта, у которого свойство №4 равно 3
    - Объект, у которого свойство №4 равно 2 находится слева снизу от объекта, у которого свойство №1 равно 9
    - Объект, у которого свойство №3 равно 1 находится слева сверху от объекта, у которого свойство №2 равно 4
    - Объект, у которого свойство №2 равно 7 находится слева снизу от объекта, у которого свойство №1 равно 6
 
 - Ограничение №4 _(4 ограничения)_
    - Объект, у которого свойство №1 равно 9 является соседом объекту, у которого свойство №4 равно 1
    - Объект, у которого свойство №4 равно 5 является соседом объекту, у которого свойство №2 равно 9
    - Объект, у которого свойство №3 равно 1 является соседом объекту, у которого свойство №1 равно 4
    - Объект, у которого свойство №2 равно 7 является соседом объекту, у которого свойство №4 равно 2
    
 - Ограничение №5 _(? ограничения)_
    - У двух различных объектов значения любого параметра (свойства) не совпадают
 - Ограничение №6 _(? ограничения)_
    - Параметры принимают значения только из заданных множеств (значение свойств должно быть меньше N)
 - Ограничение №7 _(? ограничения)_
    - Сумма свойств объектов-соседей не должна быть больше K