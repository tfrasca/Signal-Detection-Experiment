from serial import Serial
from scipy.stats import norm
z = norm.ppf

total = 0
hits = 0
false_alarms = 0
misses = 0
correct_rejects = 0
start_recording = False
count_down = False

def analyze_results():
  hit_rate = hits / (hits + misses)
  if hit_rate == 1:
    hit_rate  = 1 - .5 / (hits+misses)
  fa_rate = false_alarms / (false_alarms + correct_rejects)
  if fa_rate == 1:
    fa_rate  = 1 - .5 / (false_alarms + correct_rejects)
  z_hit = z(hit_rate)
  z_fa = z(fa_rate)
  d_prime = z_hit - z_fa
  beta = -.5* (z_hit + z_fa)
  return (z_hit, z_fa, d_prime, beta)

try:
  listener = Serial("/dev/ttyUSB0")
  line = listener.readline().decode().strip("\n")
  print(line)
  while not line == "End Experiment":
    line = listener.readline().decode().strip("\n")
    if "experiment will commense" in line:
      start_recording = True
    if len(line) > 5 or count_down:
      print(line)
    line = line.strip()
    if start_recording:
      line = line.strip()
      if line == "h":
        hits +=1
      elif line == "fa":
        false_alarms +=1
      elif line == "m":
        misses +=1
      elif line == "cr":
        correct_rejects +=1
    if count_down:
      line = line.strip()
      if line == "0":
        count_down = False

  total = hits + misses + false_alarms + correct_rejects
  z_hit, z_fa, d_prime, beta = analyze_results()
  print ("Total tests:", total)
  print ("Hits:", hits)
  print ("Misses:", misses)
  print ("False Alarms:", false_alarms)
  print ("Correct Rejects:", correct_rejects)
  print ("z(hit):", z_hit)
  print ("z(false alarm):", z_fa)
  print ("d-prime", d_prime)
  print ("beta", beta)
  listener.close()
except KeyboardInterrupt:
  print("")
  print("closing listener")
  listener.close()


