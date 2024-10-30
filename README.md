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

# cpu_cache_access_timings.c
```
bash-3.2$ cat low_cycles_percent.csv
Size(KB),12,14,16,18,19,20,21,22,24,25
Size_10K,0.19,5.53,24.18,18.07,0.02,4.35,0.03,9.60,5.27,2.86
Size_20K,0.13,5.81,25.52,18.06,0.01,4.51,0.04,8.42,3.54,4.10
Size_30K,0.07,6.38,28.77,20.28,0.00,4.91,0.02,6.64,2.65,2.46
Size_40K,0.08,6.60,29.24,20.52,0.00,4.98,0.02,6.41,2.58,2.07
Size_50K,0.08,5.78,26.20,18.37,0.01,4.56,0.04,7.31,2.98,4.26
Size_60K,0.28,6.23,26.07,18.05,0.01,4.35,0.04,7.47,3.08,4.36
Size_70K,0.36,6.11,25.65,17.66,0.01,4.21,0.03,7.29,2.92,5.16
Size_80K,0.46,5.55,22.63,15.52,0.01,3.59,0.04,7.91,2.99,8.17
```
![image](https://github.com/user-attachments/assets/57817580-b244-4a88-9ae3-093ce0b845da)
Google colab - https://colab.research.google.com/drive/1HkwFOEaR2OuYGJCeFLMTF8FmjluFeTQv?usp=sharing

