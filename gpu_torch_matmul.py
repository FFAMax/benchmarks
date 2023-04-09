import numpy as np
import time
import torch

size = int(1e3)  # Размер массива (1 ГБ = 1e3 МБ = 1e6 кБ = 1e9 Б)
a = np.random.rand(size, size).astype(np.float32)
b = np.random.rand(size, size).astype(np.float32)

start_time = time.time()

# Код инициализации массивов

end_time = time.time()
print(f"Время инициализации: {end_time - start_time:.5f} секунд")

start_time = time.time()

# Создаем тензоры PyTorch из массивов NumPy
a_tensor = torch.from_numpy(a).cuda()
b_tensor = torch.from_numpy(b).cuda()

# Умножаем тензоры на GPU
c_tensor = torch.matmul(a_tensor, b_tensor)

# Переносим результат обратно на CPU в виде массива NumPy
c = c_tensor.cpu().numpy()

end_time = time.time()
print(f"Время выполнения умножения: {end_time - start_time:.5f} секунд")

