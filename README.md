# benchmarks
Измерения времени выполнения трех разных скриптов показывают, что самый быстрый скрипт - это написанный на C скрипт random-c, который выполняется за 0.107 секунды. Скрипты, написанные на PHP и Python, выполняются медленнее: скрипт random.php на PHP выполняется за 1.882 секунды, а скрипт random-.py на Python - за 1.363 секунды.
    real - реальное время, затраченное на выполнение программы от начала до конца;
    user - время, затраченное на выполнение пользовательского кода;
    sys - время, затраченное на выполнение системных вызовов, связанных с выполнением программы.

```
# time ./random-c

real	0m0.107s
user	0m0.098s
sys	0m0.008s

# time php random.php


real	0m1.882s
user	0m0.528s
sys	0m1.353s

# time python3 random-.py

real	0m1.363s
user	0m1.335s
sys	0m0.028s

Script          Time            Percentage      Times slower
random-c        0.107s          100.00%         1.00x
random.php      1.882s          1761.68%        17.59x
random.py       1.363s          1273.83%        12.73x


```

```
import re

# Определяем минимальное время выполнения
times = {'random-c': 0.107, 'random.php': 1.882, 'random.py': 1.363}
min_time = min(times.values())

# Выводим результаты в процентах от минимального времени
print('Script\t\tTime\t\tPercentage\tTimes slower')
for name, time in times.items():
    percentage = round(time / min_time * 100, 2)
    times_slower = round(time / min_time, 2)
    print(f'{name}\t{time:.3f}s\t{percentage:.2f}%\t\t{times_slower:.2f}x')

```
