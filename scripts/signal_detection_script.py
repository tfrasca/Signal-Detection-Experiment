from serial import Serial

total = 0
hits = 0
false_alarms = 0
misses = 0
correct_rejects = 0
start_recording = False
count_down = False
try:
  listener = Serial("/dev/ttyUSB0")
  line = listener.readline().decode().strip("\n")
  print(line)
  while not line == "End Experiment":
    line = listener.readline().decode().strip("\n")
    if "start the experiment" in line:
      start_recording = True
      count_down = True
    if len(line) > 5 or count_down:
      print(line)
    line = line.strip()
    if start_recording and not count_down:
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
  print ("Total tests:", total)
  print ("Hits:", hits)
  print ("Misses:", misses)
  print ("False Alarms:", false_alarms)
  print ("Correct Rejects:", correct_rejects)
  listener.close()
except KeyboardInterrupt:
  print("")
  print("closing listener")
  listener.close()
