import time

# Define the WPA2 key math function
def wpa2_key_math(password, ssid):
  import hashlib
  import hmac
  
  # Generate the PMK (Pairwise Master Key)
  pmk = hashlib.pbkdf2_hmac('sha1', password.encode('utf-8'), ssid.encode('utf-8'), 4096, 32)
  
  # Generate the PTK (Pairwise Transient Key)
  a_nonce = '00000000000000000000000000000001'.encode('utf-8')
  s_nonce = '00000000000000000000000000000002'.encode('utf-8')
  mac_ap = '112233445566'.encode('utf-8')
  mac_sta = 'aabbccddeeff'.encode('utf-8')
  data = b'\x01\x00' + mac_ap + mac_sta + a_nonce + s_nonce
  ptk = hmac.new(pmk, data, hashlib.sha1).digest()
  
  return ptk

# Run the benchmark
password = 'password'
ssid = 'example'
start_time = time.time()
for i in range(10000):
  ptk = wpa2_key_math(password, ssid)
end_time = time.time()
total_time = end_time - start_time
average_time = total_time / 10000

print('Total time:', total_time)
print('Average time:', average_time)

# Example:
# Total time: 39.921451807022095
# Average time: 0.003992145180702209

